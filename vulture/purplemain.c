/*
 * Vulture - Win32 libpurple client
 *
 * purplemain.c: Fundamentals of the interface with libpurple, and the glib
 * main loop.
 *
 * Copyright (C) 2009, Gregor Dick <gdick@soc.pidgin.im>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

/**
 * \file purplemain.c
 * Fundamentals of the interface with libpurple, and the glib main loop.
 * 
 * Since the window manager can cause the UI thread to enter a modal loop for
 * various reasons, the glib main loop (and consequently libpurple) runs in its
 * own thread. This is not without its inconveniences: in particular, the UI
 * thread can't call libpurple directly; instead it must do so using the
 * message-queue mechanism. Another caveat is that this thread must wake up
 * whenever there are synchronous messages queued, which in practice means that
 * this thread should not make blocking calls, apart from to g_main_loop_run
 * itself.
 */

#include <windows.h>
#include <process.h>
#include <glib.h>

#include "purple.h"

#include "vulture.h"
#include "purplemain.h"
#include "purplequeue.h"
#include "purpleevloop.h"
#include "cmdline.h"
#include "purpleblist.h"
#include "purpleconv.h"
#include "purplestatus.h"


static UINT CALLBACK PurpleThread(void *lpvData);
static int InitLibpurple(void);
static void InitUI(void);
static void Quitting(void);



GMainLoop *g_lpgmainloop = NULL;


/**
 * Initialises libpurple by spawning a thread on which to run the glib main
 * loop. The queues are first initialised in the context of the calling thread
 * so that the caller may start enqueuing calls as soon as this function
 * returns.
 *
 * @param[out]	lphthread	Handle to new thread is returned here. -1L on
 *				error.
 */
void VultureInitLibpurple(HANDLE *lphthread)
{
	VulturePurpleInitQueue();

	*lphthread = (HANDLE)_beginthreadex(NULL, 0, PurpleThread, NULL, 0, NULL);
}


/**
 * Shuts down libpurple. Its thread should already have terminated before this
 * function is called.
 */
void VultureShutDownLibpurple(void)
{
	VulturePurpleDestroyQueue();
}


/**
 * Thread procedure for the thread in which libpurple runs.
 *
 * @param	lpvData		Currently unused.
 *
 * @return Exit code. Zero on success; non-zero on error.
 */
static UINT CALLBACK PurpleThread(void *lpvData)
{
	GSource *lpgsourceSync = VultureCreateSyncQueueSource();
	GSource *lpgsourceAsync = VultureCreateAsyncQueueSource();

	UNREFERENCED_PARAMETER(lpvData);

	g_lpgmainloop = g_main_loop_new(NULL, TRUE);

	/* Poll the queues. */
	g_source_attach(lpgsourceSync, NULL);
	g_source_attach(lpgsourceAsync, NULL);

	if(InitLibpurple() == 0)
		g_main_loop_run(g_lpgmainloop);
	else
	{
		/* TODO: Display a message and replace magic number. */
		exit(1);
	}

	g_main_loop_unref(g_lpgmainloop);
	g_source_unref(lpgsourceAsync);
	g_source_unref(lpgsourceSync);

	return 0;
}


/**
 * Begins initialising libpurple. This function will eventually pass control
 * to InitUI which does most of the work.
 *
 * @return Zero on success; non-zero on error.
 */
static int InitLibpurple(void)
{
	static PurpleCoreUiOps s_coreuiops = 
	{
		NULL,
		NULL,
		InitUI,
		Quitting,
		NULL,
		NULL,
		NULL,
		NULL,
	};

	gchar *szCustomUserDir;

	if((szCustomUserDir = VultureGetCustomUserDir()))
		purple_util_set_user_dir(szCustomUserDir);

	VulturePurpleEventLoopSetUIOps();
	purple_core_set_ui_ops(&s_coreuiops);

	/* Init the core, which will eventually call InitUI. */
	if(!purple_core_init(VULTURE_ID))
		return 1;

	VulturePostUIMessage(VUIMSG_PURPLEINITCOMPLETE, NULL);

	/* Sign in using specified or remembered state. */
	if (!purple_prefs_get_bool("/purple/savedstatus/startup_current_status"))
		purple_savedstatus_activate(purple_savedstatus_get_startup());
	purple_accounts_restore_current_statuses();

	return 0;
}


/**
 * Initialises higher-level libpurple components.
 *
 * \sa InitLibpurple.
 */
static void InitUI(void)
{
	static PurpleBlistUiOps s_blistuiops =
	{
		NULL, PurpleBlistNewNode, NULL, PurpleBlistUpdateNode, PurpleBlistRemoveNode, NULL, NULL,
		NULL, NULL, NULL,
		NULL, NULL, NULL, NULL
	};

	static PurpleConversationUiOps s_convuiops = 
	{
		PurpleNewConversation,		/* create_conversation	*/
		PurpleDestroyConversation,	/* destroy_conversation	*/
		NULL,				/* write_chat		*/
		NULL,				/* write_im		*/
		PurpleWriteConversation,	/* write_conv		*/
		PurpleChatAddUsers,		/* chat_add_users	*/
		PurpleChatRenameUser,		/* chat_rename_user	*/
		PurpleChatRemoveUsers,		/* chat_remove_users	*/
		NULL,				/* chat_update_user	*/
		NULL,				/* present		*/
		NULL,				/* has_focus		*/
		NULL,				/* custom_smiley_add	*/
		NULL,				/* custom_smiley_write	*/
		NULL,				/* custom_smiley_close	*/
		NULL,				/* send_confirm		*/
		NULL,				/* reserved		*/
		NULL,				/* reserved		*/
		NULL,				/* reserved		*/
		NULL,				/* reserved		*/
	};

	purple_blist_set_ui_ops(&s_blistuiops);
	purple_conversations_set_ui_ops(&s_convuiops);

	purple_signal_connect(purple_savedstatuses_get_handle(), "savedstatus-changed", GINT_TO_POINTER(VSH_STATUS), PURPLE_CALLBACK(PurpleStatusChanged), NULL);
	purple_signal_connect(purple_conversations_get_handle(), "conversation-updated", GINT_TO_POINTER(VSH_CONV), PURPLE_CALLBACK(PurpleConvChanged), NULL);
	purple_signal_connect(purple_blist_get_handle(), "buddy-status-changed", GINT_TO_POINTER(VSH_BLIST), PURPLE_CALLBACK(PurpleBuddyStatusChanged), NULL);

	/* Create and load libpurple's buddy-list. */
	purple_set_blist(purple_blist_new());
	purple_blist_load();
}


/**
 * PurpleCoreUiOps::quit callback.
 */
static void Quitting(void)
{
	/* The core is on its way out, so tell the UI to destroy itself. */
	VulturePostUIMessage(VUIMSG_QUIT, NULL);
}

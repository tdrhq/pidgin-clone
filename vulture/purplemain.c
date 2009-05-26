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

#include "vulture.h"
#include "purplemain.h"
#include "purplequeue.h"


static UINT CALLBACK PurpleThread(void *lpvData);


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
 * @return Always zero. This is used as the thread's exit code.
 */
static UINT CALLBACK PurpleThread(void *lpvData)
{
	GSource *lpgsourceSync = VultureCreateSyncQueueSource();
	GSource *lpgsourceAsync = VultureCreateAsyncQueueSource();

	UNREFERENCED_PARAMETER(lpvData);

	g_lpgmainloop = g_main_loop_new(NULL, TRUE);

	g_source_attach(lpgsourceSync, NULL);
	g_source_attach(lpgsourceAsync, NULL);

	g_main_loop_run(g_lpgmainloop);

	g_main_loop_unref(g_lpgmainloop);
	g_source_unref(lpgsourceAsync);
	g_source_unref(lpgsourceSync);

	return 0;
}

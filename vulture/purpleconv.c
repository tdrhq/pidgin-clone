/*
 * Vulture - Win32 libpurple client
 *
 * purpleconv.c: Conversation back-end.
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

#include <windows.h>
#include <glib.h>

#include "vulture.h"
#include "purple.h"
#include "purpleconv.h"
#include "vultureconv.h"
#include "purplemain.h"



/**
 * PurpleConversationUiOps::create_conversation callback. Called when a new
 * conversation is created.
 *
 * @param	lpconv	New conversation.
 */
void PurpleNewConversation(PurpleConversation *lpconv)
{
	/* The UI will eventually free this. */
	VULTURE_CONVERSATION *lpvconv = g_new(VULTURE_CONVERSATION, 1);
	
	lpconv->ui_data = lpvconv;

	lpvconv->lpconv = lpconv;
	lpvconv->hwndContainer = lpvconv->hwndConv = NULL;

	InitializeCriticalSection(&lpvconv->sync.cs);
	lpvconv->sync.szTitle = VultureUTF8ToTCHAR(purple_conversation_get_title(lpconv));

	VulturePostUIMessage(g_hwndMain, VUIMSG_NEWCONVERSATION, lpvconv);
}


/**
 * Called by the UI to free the data allocated by PurpleNewConversation when
 * the UI is done with it.
 *
 * @param	lpvconv		Data to free.
 */
void VultureFreeConversation(VULTURE_CONVERSATION *lpvconv)
{
	g_free(lpvconv->sync.szTitle);
	DeleteCriticalSection(&lpvconv->sync.cs);
	g_free(lpvconv);
}


/**
 * PurpleConversationUiOps::destroy_conversation callback. Called when a
 * conversation is being destroyed by the core. This should inform the
 * front-end to dismantle its UI. The sequence for an interactive request to
 * close a conversation is therefore: UI requests core to destroy convo, core
 * tells UI to destroy itself.
 *
 * @param	lpconv	Dying conversation.
 */
void PurpleDestroyConversation(PurpleConversation *lpconv)
{
	VulturePostUIMessage(g_hwndMain, VUIMSG_DESTROYEDCONVERSATION, (VULTURE_CONVERSATION*)lpconv->ui_data);
}

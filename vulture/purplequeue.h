/*
 * Vulture - Win32 libpurple client
 *
 * purplequeue.h: Event queue for the interface with libpurple.
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


#ifndef _VULTURE_PURPLEQUEUE_H_
#define _VULTURE_PURPLEQUEUE_H_


#include <windows.h>
#include <glib.h>

#include "vulture.h"


/** IDs of calls to libpurple. Used as values for iCallID member of
 * PURPLE_CALL. Where the lpvParam element has any significance, it is
 * documented here.
 */
enum PURPLE_CALL_ID
{
	PC_QUIT,

	/* (GLlist**) Used to return list of statuses. */
	PC_GETALLSAVEDSTATUSES,

	/* (VULTURE_GET_ACCOUNTS*) Specifies which accounts to get, and returns
	 * them.
	 */
	PC_GETACCOUNTS,

	/* (VULTURE_ACCOUNT*) Account to update. */
	PC_UPDATEPURPLEACCOUNT,

	/* (VULTURE_SAVED_STATUS*) Status to set. */
	PC_SETSAVEDSTATUS,

	/* (VULTURE_CONVERSATION*) Conversation to destroy. */
	PC_DESTROYCONVERSATION,

	/* (VULTURE_CONV_SEND*) Conversation send-message details. */
	PC_CONVSEND,

	/* (LPTSTR*) Used to return address of buffer. */
	PC_GETSTATUSMSG,

	/* (VULTURE_BLIST_NODE*) Node double-clicked.  */
	PC_BLISTNODEACTIVATED,

	/* (VULTURE_CONV_GET_STRING*) Retrieves the conversation's title. */
	PC_CONVGETTITLE,

	/* (VULTURE_IM_GET_STATUS_MSG*) */
	PC_IMGETSTATUSMSG,

	/* (VULTURE_GET_CHAT_FIELDS*) */
	PC_GETCHATFIELDS,

	/* (VULTURE_JOIN_CHAT_DATA*) */
	PC_JOINCHAT,

	/* (VULTURE_CONV_GET_STRING*) Retrieves the chat's topic. */
	PC_CHATGETTOPIC,

	/* (VULTURE_GET_IM_BUDDY_ICON*) */
	PC_GETIMBUDDYICON,

	/* (VULTURE_MAKE_CONTEXT_MENU*) */
	PC_MAKEBUDDYMENU,
};


void VulturePurpleInitQueue(void);
void VulturePurpleDestroyQueue(void);
void VultureEnqueueAsyncPurpleCall(int iCallID, void *lpvParam);
void VultureEnqueueSyncPurpleCall(int iCallID, void *lpvParam, HANDLE heventSync);
GSource* VultureCreateAsyncQueueSource(void);
GSource* VultureCreateSyncQueueSource(void);
void VulturePurpleWait(GArray *lpgarrayWaitContext);
void VultureEnqueueMultiSyncPurpleCall(int iCallID, void *lpvParam, GArray *lpgarrayWaitContext);
void VultureSingleSyncPurpleCall(int iCallID, void *lpvParam);



/**
 * Allocates an object used to wait for several synchronous libpurple calls.
 *
 * @return Wait context. Pass this to VultureEnqueueMultiSyncPurpleCall and
 * VulturePurpleWait.
 */
static INLINE GArray* VultureAllocPurpleWaitContext(void) { return g_array_new(FALSE, FALSE, sizeof(HANDLE)); }


#endif

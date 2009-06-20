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


#ifndef _VULTURE_PURPLEMAIN_H_
#define _VULTURE_PURPLEMAIN_H_

#include <windows.h>
#include <glib.h>

#include "vulture.h"
#include "vultureblist.h"


/* The significance of lpvParam is given for each message. */
enum ENUM_VULTURE_UI_MESSAGES
{
	/* (VULTURE_BLIST_NODE*) Node being updated. */
	VUIMSG_UPDATEBLISTNODE,

	VUIMSG_PURPLEINITCOMPLETE,

	/* (VULTURE_CONVERSATION*) */
	VUIMSG_NEWCONVERSATION,

	/* (VULTURE_CONVERSATION*) */
	VUIMSG_DESTROYEDCONVERSATION,

	/* (VULTURE_CONV_WRITE*) */
	VUIMSG_WRITECONVERSATION,

	VUIMSG_QUIT,

	/* (VULTURE_SAVED_STATUS*) New status. */
	VUIMSG_STATUSCHANGED,
};

/* HandlEs for reigistering signal handlERs. */
enum ENUM_VULTURE_SIGNAL_HANDLES
{
	VSH_STATUS = 1,
};

void VultureInitLibpurple(HANDLE *lphthread);
void VultureShutDownLibpurple(void);

extern GMainLoop *g_lpgmainloop;


/**
 * Posts a message from libpurple to the Windows message queue of the specified
 * window.
 *
 * @param	hwnd		Window.
 * @param	iCallID		ID of the message.
 * @param	lpvParam	Message-specific data.
 */
static INLINE void VulturePostUIMessage(HWND hwnd, int iMsg, void *lpvParam)
{
	if(IsWindow(hwnd))
		PostMessage(hwnd, WM_PURPLEUIMSG, iMsg, (LPARAM)lpvParam);
}


#endif

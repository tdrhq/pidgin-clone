/*
 * Vulture - Win32 libpurple client
 *
 * vultureconv.h: Conversation UI.
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

#ifndef _VULTURE_VULTURECONV_H_
#define _VULTURE_VULTURECONV_H_


#include <windows.h>

#include "purple.h"


typedef struct _VULTURE_CONVERSATION
{
	PurpleConversation	*lpconv;
	HWND			hwndConv;
	HWND			hwndContainer;
	int			iTabIndex;
	WNDPROC			wndprocInputOrig;

	/* Data still needed by the core thread after initialisation. */
	struct
	{
		CRITICAL_SECTION	cs;
		LPTSTR			szTitle;
	} sync;
} VULTURE_CONVERSATION;


typedef struct _VULTURE_CONV_WRITE
{
	VULTURE_CONVERSATION	*lpvconv;
	LPTSTR			szName;
	LPTSTR			szMessage;
	SYSTEMTIME		systimeMsg;
} VULTURE_CONV_WRITE;

typedef struct _VULTURE_CONV_SEND
{
	VULTURE_CONVERSATION	*lpvconv;
	LPTSTR			szMessage;
} VULTURE_CONV_SEND;


int VultureRegisterConvContainerWindowClass(void);
HWND VultureCreateConvContainer(void);
void VultureWriteConversation(VULTURE_CONV_WRITE *lpvcwrite);
void VultureFreeConvSend(VULTURE_CONV_SEND *lpvcsend);


#endif

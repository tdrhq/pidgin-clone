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
#include <commctrl.h>
#include <glib.h>

#include "purple.h"


typedef struct _VULTURE_CONVERSATION
{
	PurpleConversation	*lpconv;
	HWND			hwndConv;
	HWND			hwndContainer;
	int			iTabIndex;
	WNDPROC			wndprocInputOrig;
	PurpleConversationType	convtype;
} VULTURE_CONVERSATION;


/* These two structs inherit from VULTURE_CONVERSATION. */

typedef struct _VULTURE_CONVERSATION_IM
{
	VULTURE_CONVERSATION	vconv;
} VULTURE_CONVERSATION_IM;

typedef struct _VULTURE_CONVERSATION_CHAT
{
	VULTURE_CONVERSATION	vconv;
	GTree			*lpgtreePeople;
} VULTURE_CONVERSATION_CHAT;


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

typedef struct _VULTURE_CONV_CHANGED
{
	VULTURE_CONVERSATION	*lpvconv;
	PurpleConvUpdateType	pcut;
} VULTURE_CONV_CHANGED;

typedef struct _VULTURE_CONV_GET_STRING
{
	VULTURE_CONVERSATION	*lpvconv;
	LPTSTR			sz;
} VULTURE_CONV_GET_STRING;

typedef struct _VULTURE_ADD_CHAT_USER
{
	LPTSTR				szName;
	LPTSTR				szAlias;
	LPTSTR				szAliasKey;
	BOOL				bIsBuddy;
	PurpleConvChatBuddyFlags	pccbflags;
} VULTURE_ADD_CHAT_USER;

typedef struct _VULTURE_CHAT_ADD_USERS
{
	VULTURE_CONVERSATION	*lpvconvChat;
	GList			*lpglistNewUsers;
} VULTURE_CHAT_ADD_USERS;

typedef struct _VULTURE_CHAT_USER
{
	/* We don't store the name here. */

	LPTSTR				szAlias;
	LPTSTR				szAliasKey;
	BOOL				bIsBuddy;
	PurpleConvChatBuddyFlags	pccbflags;
	HTREEITEM			hti;
} VULTURE_CHAT_USER;

typedef struct _VULTURE_CHAT_RENAME_USER
{
	VULTURE_CONVERSATION	*lpvconvChat;
	LPTSTR			szOldName, szNewName, szNewAlias;
} VULTURE_CHAT_RENAME_USER;


int VultureRegisterConvContainerWindowClass(void);
HWND VultureCreateConvContainer(void);
void VultureWriteConversation(VULTURE_CONV_WRITE *lpvcwrite);
void VultureFreeConvSend(VULTURE_CONV_SEND *lpvcsend);


#endif

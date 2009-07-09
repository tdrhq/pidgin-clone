/*
 * Vulture - Win32 libpurple client
 *
 * purpleconv.h: Conversation back-end.
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

#ifndef _VULTURE_PURPLECONV_H_
#define _VULTURE_PURPLECONV_H_


#include <windows.h>
#include <glib.h>

#include "purple.h"
#include "vultureconv.h"

void PurpleNewConversation(PurpleConversation *lpconv);
void VultureFreeConversation(VULTURE_CONVERSATION *lpvconv);
void PurpleDestroyConversation(PurpleConversation *lpconv);
void PurpleWriteConversation(PurpleConversation *lpconv, const char *szName, const char *szAlias, const char *szMessage, PurpleMessageFlags pmflags, time_t timeMsg);
void VultureFreeConvWrite(VULTURE_CONV_WRITE *lpvcwrite);
void PurpleConversationSend(VULTURE_CONV_SEND *lpvcsend);
void PurpleConvChanged(PurpleConversation *lpconv, PurpleConvUpdateType pcut);
LPTSTR PurpleConvGetTitle(PurpleConversation *lpconv);
LPTSTR PurpleIMGetStatusText(PurpleConversation *lpconv);
GList* PurpleGetChatFields(PurpleAccount *lppac);
void PurpleJoinAdHocChat(PurpleAccount *lppac, GHashTable *lphashParameters);
void PurpleJoinChat(PurpleChat *lpchat);
void PurpleChatAddUsers(PurpleConversation *lpconv, GList *lpglistChatBuddies, gboolean bNewArrivals);
void VultureFreeChatAddUsers(VULTURE_CHAT_ADD_USERS *lpvchataddusers);
void PurpleChatRenameUser(PurpleConversation *lpconv, const char *szOldName, const char *szNewName, const char *szNewAlias);
void VultureFreeRenameUser(VULTURE_CHAT_RENAME_USER *lpvchatrenameuser);


#endif

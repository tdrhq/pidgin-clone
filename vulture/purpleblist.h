/*
 * Vulture - Win32 libpurple client
 *
 * purpleblist.h: libpurple end of buddy-list management.
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


#ifndef _VULTURE_PURPLEBLIST_H_
#define _VULTURE_PURPLEBLIST_H_


#include <windows.h>

#include "vultureblist.h"
#include "purple.h"



typedef struct _VULTURE_JOIN_CHAT_DATA
{
	PurpleAccount		*lppac;
	GHashTable		*lphashParameters;
	LPTSTR			szAlias;
	VULTURE_BLIST_NODE	*lpvblistnodeGroup;
	BOOL			bJoinFieldsOnly;
} VULTURE_JOIN_CHAT_DATA;


typedef struct _VULTURE_ADD_BUDDY_DATA
{
	PurpleAccount		*lppac;
	LPTSTR			szUsername;
	LPTSTR			szAlias;
	VULTURE_BLIST_NODE	*lpvblistnodeGroup;
	BOOL			bIMFieldsOnly;
} VULTURE_ADD_BUDDY_DATA;


void PurpleBlistNewNode(PurpleBlistNode *lpblistnode);
void PurpleBlistUpdateNode(PurpleBuddyList *lpbuddylist, PurpleBlistNode *lpblistnode);
void PurpleBlistRemoveNode(PurpleBuddyList *lpbuddylist, PurpleBlistNode *lpblistnode);
void VultureBListNodeRelease(VULTURE_BLIST_NODE *lpvblnode);
void PurpleBListNodeActivated(VULTURE_BLIST_NODE *lpvbn);
LPTSTR PurpleBuddyGetStatusText(PurpleBuddy *lpbuddy);
void PurpleBuddyStatusChanged(PurpleBuddy *lpbuddy, PurpleStatus *lpstatusOld, PurpleStatus *lpstatusNew);
void PurpleMakeBuddyMenu(HMENU hmenu, PurpleBlistNode *lpblistnode, GList **lplpglistVMA);
void PurpleMakeChatMenu(HMENU hmenu, PurpleBlistNode *lpblistnode, GList **lplpglistVMA);
void PurpleBlistAliasNode(PurpleBlistNode *lpblistnode, LPCTSTR szAlias);
void PurpleDeleteBlistNode(PurpleBlistNode *lpblistnode);
void PurpleCommonMakeMenu(HMENU hmenu, PurpleBlistNode *lpblistnode);
void PurpleBuddyIconChanged(PurpleBuddy *lpbuddy);
GList* PurpleGetGroups(void);
void VultureFreeGroupList(GList *lpglistGroups);
void PurpleAddBuddy(VULTURE_ADD_BUDDY_DATA *lpvabd);
void PurpleBlistUpdateLeaves(PurpleBlistNode *lpbln);
void PurpleAddChat(VULTURE_JOIN_CHAT_DATA *lpvjcd);


static INLINE void VultureBListNodeAddRef(VULTURE_BLIST_NODE *lpvblnode) { InterlockedIncrement(&lpvblnode->lRefCount); }


#endif

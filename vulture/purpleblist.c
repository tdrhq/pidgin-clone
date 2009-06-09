/*
 * Vulture - Win32 libpurple client
 *
 * purpleblist.c: libpurple end of buddy-list management.
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
#include "purpleblist.h"
#include "vultureblist.h"
#include "purplemain.h"


/**
 * Callback for when a new buddy-list node is created.
 *
 * @param	lpblistnode	Buddy-list node.
 */
void PurpleBlistNewNode(PurpleBlistNode *lpblistnode)
{
	/* As this is the first time we've seen this node, allocate some state
	 * data. The UI will free this when it removes the corresponding UI
	 * node.
	 */

	VULTURE_BLIST_NODE *lpvbn = (VULTURE_BLIST_NODE*)(lpblistnode->ui_data = g_new(VULTURE_BLIST_NODE, 1));

	lpvbn->szNodeText = NULL;
	lpvbn->hti = NULL;
	InitializeCriticalSection(&lpvbn->cs);
}


/**
 * Callback for when a buddy-list node must be updated.
 *
 * @param	lpbuddylist	Buddy-list. Unused.
 * @param	lpblistnode	Buddy-list node.
 */
void PurpleBlistUpdateNode(PurpleBuddyList *lpbuddylist, PurpleBlistNode *lpblistnode)
{
	VULTURE_BLIST_NODE *lpvbn;

	UNREFERENCED_PARAMETER(lpbuddylist);

	
	lpvbn = (VULTURE_BLIST_NODE*)lpblistnode->ui_data;

	EnterCriticalSection(&lpvbn->cs);
	{
		const char *szNodeText;

		lpvbn->lpvbnParent = lpblistnode->parent ? (VULTURE_BLIST_NODE*)lpblistnode->parent->ui_data : NULL;

		switch(lpblistnode->type)
		{
		case PURPLE_BLIST_GROUP_NODE:
			szNodeText = ((PurpleGroup*)lpblistnode)->name;
			break;

		case PURPLE_BLIST_CONTACT_NODE:
			szNodeText = purple_contact_get_alias((PurpleContact*)lpblistnode);
			break;

		default:
			szNodeText = PURPLE_BLIST_NODE_NAME(lpblistnode);
			break;
		}

		if(lpvbn->szNodeText) g_free(lpvbn->szNodeText);
		lpvbn->szNodeText = szNodeText ? VultureUTF8ToTCHAR(szNodeText) : NULL;
	}
	LeaveCriticalSection(&lpvbn->cs);

	VulturePostUIMessage(g_hwndMain, VUIMSG_UPDATEBLISTNODE, lpvbn);
}

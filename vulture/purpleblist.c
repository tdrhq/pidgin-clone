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



static BOOL ShouldShowNode(PurpleBlistNode *lpblistnode);



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

	lpvbn->lpblistnode = lpblistnode;
	lpvbn->szNodeText = NULL;
	lpvbn->hti = NULL;
	lpvbn->lRefCount = 1;
	lpvbn->lpvbnParent = NULL;
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

	if(!lpblistnode)
		return;

	if(!ShouldShowNode(lpblistnode))
		return;
		
	lpvbn = (VULTURE_BLIST_NODE*)lpblistnode->ui_data;

	EnterCriticalSection(&lpvbn->cs);
	{
		const char *szNodeText;

		if(lpvbn->lpvbnParent) VultureBListNodeRelease(lpvbn->lpvbnParent);
		lpvbn->lpvbnParent = lpblistnode->parent ? (VULTURE_BLIST_NODE*)lpblistnode->parent->ui_data : NULL;
		if(lpvbn->lpvbnParent) VultureBListNodeAddRef(lpvbn->lpvbnParent);

		switch(lpblistnode->type)
		{
		case PURPLE_BLIST_GROUP_NODE:
			szNodeText = ((PurpleGroup*)lpblistnode)->name;
			break;

		case PURPLE_BLIST_CONTACT_NODE:
			szNodeText = purple_contact_get_alias((PurpleContact*)lpblistnode);

			if(!szNodeText || !(*szNodeText))
			{
				PurpleBuddy *lpbuddy = purple_contact_get_priority_buddy((PurpleContact*)lpblistnode);
				szNodeText = purple_buddy_get_name(lpbuddy);
			}

			break;

		default:
			szNodeText = PURPLE_BLIST_NODE_NAME(lpblistnode);
			break;
		}

		if(lpvbn->szNodeText) g_free(lpvbn->szNodeText);
		lpvbn->szNodeText = szNodeText ? VultureUTF8ToTCHAR(szNodeText) : NULL;

		/* TODO: We should probably be less willing to give up. */
		if(lpvbn->szNodeText)
		{
			/* If out parent isn't showing, show it first. */
			if(lpvbn->lpvbnParent && !lpvbn->lpvbnParent->hti)
				PurpleBlistUpdateNode(lpbuddylist, lpvbn->lpvbnParent->lpblistnode);

			VultureBListNodeAddRef(lpvbn);
			VulturePostUIMessage(g_hwndMain, VUIMSG_UPDATEBLISTNODE, lpvbn);
		}
	}
	LeaveCriticalSection(&lpvbn->cs);
}



/**
 * Determines whether a buddy-list node should be shown.
 *
 * @param	lpblistnode	Buddy-list node.
 */
static BOOL ShouldShowNode(PurpleBlistNode *lpblistnode)
{
	switch(lpblistnode->type)
	{
	case PURPLE_BLIST_GROUP_NODE:
	case PURPLE_BLIST_CONTACT_NODE:
		{
			PurpleBlistNode *lpblistnodeRover;

			/* Show iff any of our children should be shown. */
			for(lpblistnodeRover = purple_blist_node_get_first_child(lpblistnode);
				lpblistnodeRover;
				lpblistnodeRover = purple_blist_node_get_sibling_next(lpblistnodeRover))
			{
				if(ShouldShowNode(lpblistnodeRover))
					return TRUE;
			}
		}

		break;

	case PURPLE_BLIST_CHAT_NODE:
		if(purple_account_is_connected(purple_chat_get_account((PurpleChat*)lpblistnode)))
			return TRUE;
		break;

	case PURPLE_BLIST_BUDDY_NODE:
		if(purple_account_is_connected(purple_buddy_get_account((PurpleBuddy*)lpblistnode)))
			return TRUE;
		break;

	default:
		break;
	}

	return FALSE;
}


/**
 * Callback for when a buddy-list is removed from the core's list.
 *
 * @param	lpbuddylist	Buddy-list. Unused.
 * @param	lpblistnode	Buddy-list node.
 */
void PurpleBlistRemoveNode(PurpleBuddyList *lpbuddylist, PurpleBlistNode *lpblistnode)
{
	UNREFERENCED_PARAMETER(lpbuddylist);

	if(!lpblistnode)
		return;

	VultureBListNodeRelease((VULTURE_BLIST_NODE*)lpblistnode->ui_data);
}


/**
 * Releases a reference to a VULTURE_BLIST_NODE and frees the node if there are
 * no outstanding references.
 *
 * @param	lpvblnode	Node to release.
 */
void VultureBListNodeRelease(VULTURE_BLIST_NODE *lpvblnode)
{
	if(InterlockedDecrement(&lpvblnode->lRefCount) == 0)
	{
		/* Released last reference. Free object. */

		if(lpvblnode->lpvbnParent) VultureBListNodeRelease(lpvblnode->lpvbnParent);
		if(lpvblnode->szNodeText) g_free(lpvblnode->szNodeText);
		DeleteCriticalSection(&lpvblnode->cs);

		g_free(lpvblnode);
	}
}

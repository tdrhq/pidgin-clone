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
#include "resource.h"
#include "purpleconv.h"



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
		
	lpvbn = (VULTURE_BLIST_NODE*)lpblistnode->ui_data;

	EnterCriticalSection(&lpvbn->cs);

		if(ShouldShowNode(lpblistnode))
		{
			const char *szNodeText;

			if(lpvbn->lpvbnParent) VultureBListNodeRelease(lpvbn->lpvbnParent);
			lpvbn->lpvbnParent = lpblistnode->parent ? (VULTURE_BLIST_NODE*)lpblistnode->parent->ui_data : NULL;
			if(lpvbn->lpvbnParent) VultureBListNodeAddRef(lpvbn->lpvbnParent);

			lpvbn->nodetype = lpblistnode->type;

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
				/* If our parent isn't showing, show it first. */
				if(lpvbn->lpvbnParent && !lpvbn->lpvbnParent->hti)
					PurpleBlistUpdateNode(lpbuddylist, lpvbn->lpvbnParent->lpblistnode);

				VultureBListNodeAddRef(lpvbn);
				VulturePostUIMessage(VUIMSG_UPDATEBLISTNODE, lpvbn);
			}
		}
		else if(lpvbn->hti)
		{
			VulturePostUIMessage(VUIMSG_REMOVEBLISTNODE, lpvbn);

			/* The parent may need to go, too. */
			if(lpvbn->lpvbnParent && lpvbn->lpvbnParent->hti)
				PurpleBlistUpdateNode(lpbuddylist, lpvbn->lpvbnParent->lpblistnode);
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
		{
			PurpleBuddy *lpbuddy = (PurpleBuddy*)lpblistnode;

			if(purple_account_is_connected(purple_buddy_get_account(lpbuddy)) &&
				(purple_presence_is_online(lpbuddy->presence) ||
				purple_blist_node_get_bool(lpblistnode, "show_offline")))
				return TRUE;
		}

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

	/* This pointer is about to become invalid. */
	((VULTURE_BLIST_NODE*)lpblistnode->ui_data)->lpblistnode = NULL;

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


/**
 * Performs the processing required when a buddy-list node has been double-
 * clicked. A close cousin of Pidgin's gtk_blist_row_activated_cb.
 *
 * @param	lpvbn	Node.
 */
void PurpleBListNodeActivated(VULTURE_BLIST_NODE *lpvbn)
{
	if(!lpvbn->lpblistnode)
		return;

	if(PURPLE_BLIST_NODE_IS_CONTACT(lpvbn->lpblistnode) || PURPLE_BLIST_NODE_IS_BUDDY(lpvbn->lpblistnode))
	{
		PurpleBuddy *lpbuddy;

		if(PURPLE_BLIST_NODE_IS_CONTACT(lpvbn->lpblistnode))
			lpbuddy = purple_contact_get_priority_buddy((PurpleContact*)lpvbn->lpblistnode);
		else
			lpbuddy = (PurpleBuddy*)lpvbn->lpblistnode;

		purple_conversation_new(PURPLE_CONV_TYPE_IM, lpbuddy->account, lpbuddy->name);
	}
	else if(PURPLE_BLIST_NODE_IS_CHAT(lpvbn->lpblistnode))
		PurpleJoinChat((PurpleChat*)lpvbn->lpblistnode);
}


/**
 * Retrieves a buddy's status text. Adapted from pidgin_blist_get_name_markup.
 *
 * @param	lpbuddy		Buddy.
 */
LPTSTR PurpleBuddyGetStatusText(PurpleBuddy *lpbuddy)
{
	PurplePlugin *lppluginPrpl;

	/* Find prpl for buddy. */
	if(lpbuddy && (lppluginPrpl = purple_find_prpl(purple_account_get_protocol_id(lpbuddy->account))))
	{
		PurplePluginProtocolInfo *lpprplinfo = PURPLE_PLUGIN_PROTOCOL_INFO(lppluginPrpl);

		/* If prpl supports status text, get the text. */
		if(lpprplinfo && lpprplinfo->status_text)
		{
			char *szStatus = lpprplinfo->status_text(lpbuddy);

			return szStatus ? VultureUTF8ToTCHAR(szStatus) : NULL;
		}
	}

	return NULL;
}


/**
 * Called in response to buddy-status-changed signal.
 *
 * @param	lpbuddy		Buddy.
 * @param	lpstatusOld	Old status.
 * @param	lpstatusNew	New status.
 */
void PurpleBuddyStatusChanged(PurpleBuddy *lpbuddy, PurpleStatus *lpstatusOld, PurpleStatus *lpstatusNew)
{
	/* Are we speaking to this buddy? */
	PurpleConversation *lpconv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, lpbuddy->name, lpbuddy->account);

	/* If so, tell the UI to update the status message in the conversation.
	 */
	if(lpconv)
		VulturePostUIMessage(VUIMSG_UPDATEIMSTATUSTEXT, lpconv->ui_data);
}


/**
 * Builds the context menu for a buddy-list node.
 *
 * @param[in,out]	hmenu		Basic menu loaded from the resources,
 *					which will be augmented.
 * @param		lpblistnode	Buddy-list node for a buddy (not
 *					contact).
 * @param[out]		lplpglistVMA	Used to return a list populated with
 *					pointers to item-data for the menu
 *					items that we add, which the caller
 *					should g_free (and then g_list_free)
 *					once it's done with the menu, but which
 *					otherwise it probably doesn't care
 *					about.
 */
void PurpleMakeBuddyMenu(HMENU hmenu, PurpleBlistNode *lpblistnode, GList **lplpglistVMA)
{
	GList *lpglistPMA;
	PurplePluginProtocolInfo *lpprplinfo;
	PurpleConnection *lpconnection;
	UINT uiNextID = IDM_DYNAMIC_FIRST;

	*lplpglistVMA = NULL;

	if(!lpblistnode)
		return;

	lpconnection = ((PurpleBuddy*)lpblistnode)->account->gc;

	if(lpconnection &&
		(lpprplinfo = PURPLE_PLUGIN_PROTOCOL_INFO(lpconnection->prpl)) &&
		lpprplinfo->blist_node_menu)
	{
		/* Insert at the bottom of the first section. */
		lpglistPMA = lpprplinfo->blist_node_menu(lpblistnode);
		PurpleInsertDynamicMenu(hmenu, VultureGetMenuPosFromID(hmenu, IDM_BLIST_CONTEXT_VIEWLOG) + 1, &uiNextID, lpglistPMA, lplpglistVMA, lpblistnode);
		g_list_free(lpglistPMA);
	}

	/* Insert at the top of the second section. */
	lpglistPMA = purple_blist_node_get_extended_menu(lpblistnode);
	PurpleInsertDynamicMenu(hmenu, VultureGetMenuPosFromID(hmenu, IDM_BLIST_CONTEXT_BLOCK), &uiNextID, lpglistPMA, lplpglistVMA, lpblistnode);
	g_list_free(lpglistPMA);

	/* Enable/disable/check stuff as appropriate. */
	CheckMenuItem(hmenu, IDM_BLIST_CONTEXT_SHOWOFFLINE, purple_blist_node_get_bool(lpblistnode, "show_offline") ? MF_CHECKED : MF_UNCHECKED);
}

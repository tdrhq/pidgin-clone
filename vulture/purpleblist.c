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
static void AddCommonMenuItems(HMENU hmenu, PurpleBlistNode *lpblistnode, GList **lplpglistVMA, PurpleConnection *lpconnection, int iProtoIndex, int iExtendedIndex);
static void DeleteBuddyFromAccount(PurpleBuddy *lpbuddy);
static void UpdateCachedStatus(PurpleBlistNode *lpblistnode);
static int GetStatusIconIndex(PurpleBuddy *lpbuddy);



/**
 * Callback for when a new buddy-list node is created.
 *
 * @param	lpblistnode	Buddy-list node.
 */
void PurpleBlistNewNode(PurpleBlistNode *lpblistnode)
{
	lpblistnode->ui_data = NULL;
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

	if(!lpvbn)
	{
		lpvbn = (VULTURE_BLIST_NODE*)(lpblistnode->ui_data = g_new(VULTURE_BLIST_NODE, 1));
		lpvbn->lpblistnode = lpblistnode;
		lpvbn->szNodeText = NULL;
		lpvbn->hti = NULL;
		lpvbn->lRefCount = 1;
		lpvbn->lpvbnParent = NULL;
		lpvbn->bExpanded = FALSE;
		lpvbn->szStatusText = NULL;
		lpvbn->iStatusIcon = 0;
		lpvbn->ui.bIconCacheValid = FALSE;
		lpvbn->ui.editnodestate = VENS_NOEDIT;
		InitializeCriticalSection(&lpvbn->cs);
	}

	EnterCriticalSection(&lpvbn->cs);

		if(ShouldShowNode(lpblistnode))
		{
			const char *szNodeText;

			if(lpvbn->lpvbnParent) VultureBListNodeRelease(lpvbn->lpvbnParent);
			lpvbn->lpvbnParent = lpblistnode->parent ? (VULTURE_BLIST_NODE*)lpblistnode->parent->ui_data : NULL;
			if(lpvbn->lpvbnParent) VultureBListNodeAddRef(lpvbn->lpvbnParent);

			lpvbn->nodetype = lpblistnode->type;

			UpdateCachedStatus(lpblistnode);

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

			case PURPLE_BLIST_BUDDY_NODE:

				/* Maybe our contact needs to update its status
				 * text.
				 */
				UpdateCachedStatus(lpblistnode->parent);

				szNodeText = purple_buddy_get_alias((PurpleBuddy*)lpblistnode);

				if(szNodeText && *szNodeText)
					break;

				/* Otherwise, fall through. */

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

				/* Update either our own node or our parent's,
				 * if we're a collapsed buddy.
				 */
				if(!PURPLE_BLIST_NODE_IS_BUDDY(lpblistnode) ||
					(lpblistnode->parent &&
					lpblistnode->parent->ui_data &&
					((VULTURE_BLIST_NODE*)lpblistnode->parent->ui_data)->bExpanded))
				{
					VultureBListNodeAddRef(lpvbn);
					VulturePostUIMessage(VUIMSG_UPDATEBLISTNODE, lpvbn);
				}
				else if(lpblistnode->parent && lpblistnode->parent->ui_data)
				{
					VultureBListNodeAddRef(lpblistnode->parent->ui_data);
					VulturePostUIMessage(VUIMSG_UPDATEBLISTNODE, lpblistnode->parent->ui_data);
				}
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

		return FALSE;

	case PURPLE_BLIST_CHAT_NODE:
		return purple_account_is_connected(purple_chat_get_account((PurpleChat*)lpblistnode));

	case PURPLE_BLIST_BUDDY_NODE:
		{
			PurpleBuddy *lpbuddy = (PurpleBuddy*)lpblistnode;
			return purple_account_is_connected(purple_buddy_get_account(lpbuddy)) &&
				(purple_presence_is_online(lpbuddy->presence) ||
				purple_blist_node_get_bool(lpblistnode, "show_offline"));
		}

	default:
		return FALSE;
	}
}


/**
 * Callback for when a buddy-list is removed from the core's list.
 *
 * @param	lpbuddylist	Buddy-list. Unused.
 * @param	lpblistnode	Buddy-list node.
 */
void PurpleBlistRemoveNode(PurpleBuddyList *lpbuddylist, PurpleBlistNode *lpblistnode)
{
	VULTURE_BLIST_NODE *lpvbn;

	UNREFERENCED_PARAMETER(lpbuddylist);

	if(!lpblistnode)
		return;

	lpvbn = lpblistnode->ui_data;

	EnterCriticalSection(&lpvbn->cs);
	{
		/* This pointer is about to become invalid. */
		lpvbn->lpblistnode = NULL;

		if(lpvbn->hti)
			VulturePostUIMessage(VUIMSG_REMOVEBLISTNODE, lpvbn);

		/* The parent may need to go, too. */
		if(lpvbn->lpvbnParent && lpvbn->lpvbnParent->hti)
			PurpleBlistUpdateNode(lpbuddylist, lpvbn->lpvbnParent->lpblistnode);
	}
	LeaveCriticalSection(&lpvbn->cs);

	VultureBListNodeRelease((VULTURE_BLIST_NODE*)lpblistnode->ui_data);

	lpblistnode->ui_data = NULL;
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
		if(lpvblnode->szStatusText) g_free(lpvblnode->szStatusText);
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

	if(!lpbuddy)
		return NULL;

	if(PURPLE_BUDDY_IS_ONLINE(lpbuddy))
	{
		/* Find prpl for buddy. */
		if((lppluginPrpl = purple_find_prpl(purple_account_get_protocol_id(lpbuddy->account))))
		{
			PurplePluginProtocolInfo *lpprplinfo = PURPLE_PLUGIN_PROTOCOL_INFO(lppluginPrpl);

			/* If prpl supports status text, get the text. */
			if(lpprplinfo && lpprplinfo->status_text)
			{
				char *szStatus = lpprplinfo->status_text(lpbuddy);

				return szStatus ? VultureUTF8ToTCHAR(szStatus) : NULL;
			}
		}
	}
	else
	{
		TCHAR szBuffer[128];
		LoadString(g_hInstance, IDS_OFFLINE, szBuffer, NUM_ELEMENTS(szBuffer));
		return g_memdup(szBuffer, sizeof(szBuffer));
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
	PurpleConversation *lpconv;
	VULTURE_BLIST_NODE *lpvbnContact = ((PurpleBlistNode*)lpbuddy)->parent ? ((PurpleBlistNode*)lpbuddy)->parent->ui_data : NULL;

	UpdateCachedStatus((PurpleBlistNode*)lpbuddy);
	UpdateCachedStatus(((PurpleBlistNode*)lpbuddy)->parent);

	if(lpvbnContact)
	{
		if(lpvbnContact->bExpanded)
		{
			VULTURE_BLIST_NODE *lpvbn = ((PurpleBlistNode*)lpbuddy)->ui_data;

			VultureBListNodeAddRef(lpvbn);
			VulturePostUIMessage(VUIMSG_UPDATEBLISTNODE, lpvbn);
		}

		VultureBListNodeAddRef(lpvbnContact);
		VulturePostUIMessage(VUIMSG_UPDATEBLISTNODE, lpvbnContact);
	}

	/* Are we speaking to this buddy? */
	lpconv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, lpbuddy->name, lpbuddy->account);

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
	*lplpglistVMA = NULL;

	if(!lpblistnode)
		return;

	AddCommonMenuItems(
		hmenu,
		lpblistnode,
		lplpglistVMA,
		((PurpleBuddy*)lpblistnode)->account->gc,
		VultureGetMenuPosFromID(hmenu, IDM_BLIST_CONTEXT_VIEWLOG) + 1,
		VultureGetMenuPosFromID(hmenu, IDM_BLIST_CONTEXT_BLOCK));

	/* Enable/disable/check stuff as appropriate. */
	CheckMenuItem(hmenu, IDM_BLIST_CONTEXT_SHOWOFFLINE, purple_blist_node_get_bool(lpblistnode, "show_offline") ? MF_CHECKED : MF_UNCHECKED);
}


/**
 * Adds dynamic menu items common to various sorts of buddy-list node.
 *
 * @param[in,out]	hmenu		Menu to add to.
 * @param		lpblistnode	List node.
 * @param[in,out]	lplpglistVMA	VMA pointers will be appended to this
 *					list as necessary. Free them later.
 * @param		lpconnect	Connection for node.
 * @param		iProtoIndex	Menu index at which to insert prpl-
 *					specific menu items.
 * @param		iExtendedIndex	Menu index at which to insert extended
 *					menu items.
 */
static void AddCommonMenuItems(HMENU hmenu, PurpleBlistNode *lpblistnode, GList **lplpglistVMA, PurpleConnection *lpconnection, int iProtoIndex, int iExtendedIndex)
{
	GList *lpglistPMA;
	PurplePluginProtocolInfo *lpprplinfo;
	UINT uiNextID = IDM_DYNAMIC_FIRST;

	if(lpconnection &&
		(lpprplinfo = PURPLE_PLUGIN_PROTOCOL_INFO(lpconnection->prpl)) &&
		lpprplinfo->blist_node_menu)
	{
		/* Insert at the bottom of the first section. */
		lpglistPMA = lpprplinfo->blist_node_menu(lpblistnode);
		PurpleInsertDynamicMenu(hmenu, iProtoIndex, &uiNextID, lpglistPMA, lplpglistVMA, lpblistnode);
		g_list_free(lpglistPMA);
	}

	/* Insert at the top of the second section. */
	lpglistPMA = purple_blist_node_get_extended_menu(lpblistnode);
	PurpleInsertDynamicMenu(hmenu, iExtendedIndex, &uiNextID, lpglistPMA, lplpglistVMA, lpblistnode);
	g_list_free(lpglistPMA);
}


/**
 * Builds the context menu for a chat node.
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
void PurpleMakeChatMenu(HMENU hmenu, PurpleBlistNode *lpblistnode, GList **lplpglistVMA)
{
	int iIndex = VultureGetMenuPosFromID(hmenu, IDM_BLIST_CONTEXT_VIEWLOG) + 1;

	*lplpglistVMA = NULL;

	if(!lpblistnode)
		return;

	AddCommonMenuItems(
		hmenu,
		lpblistnode,
		lplpglistVMA,
		((PurpleChat*)lpblistnode)->account->gc,
		iIndex,
		iIndex);

	/* Enable/disable/check stuff as appropriate. */
	CheckMenuItem(hmenu, IDM_BLIST_CONTEXT_AUTOJOIN, purple_blist_node_get_bool(lpblistnode, "vulture-autojoin") ? MF_CHECKED : MF_UNCHECKED);
}


/**
 * Sets up common context menu items/
 *
 * @param[in,out]	hmenu		Menu.
 * @param		lpblistnode	Buddy-list node.
 */
void PurpleCommonMakeMenu(HMENU hmenu, PurpleBlistNode *lpblistnode)
{
	if(!PURPLE_BLIST_NODE_IS_BUDDY(lpblistnode))
		EnableMenuItem(hmenu, IDM_BLIST_CONTEXT_REMOVEICON, purple_buddy_icons_node_has_custom_icon(lpblistnode) ? MF_ENABLED : MF_GRAYED);
}


/**
 * Sets the alias or name of a buddy-list node, as appropriate for the type.
 *
 * @param	lpblistnode	Buddy-list node.
 * @param	szAlias		New alias/name.
 */
void PurpleBlistAliasNode(PurpleBlistNode *lpblistnode, LPCTSTR szAlias)
{
	gchar *szAliasUTF8;

	if(!lpblistnode)
		return;

	szAliasUTF8 = VultureTCHARToUTF8(szAlias);

	if(PURPLE_BLIST_NODE_IS_BUDDY(lpblistnode))
		purple_blist_alias_buddy((PurpleBuddy*)lpblistnode, szAliasUTF8);
	else if(PURPLE_BLIST_NODE_IS_CONTACT(lpblistnode))
		purple_blist_alias_contact((PurpleContact*)lpblistnode, szAliasUTF8);
	else if(PURPLE_BLIST_NODE_IS_CHAT(lpblistnode))
		purple_blist_alias_chat((PurpleChat*)lpblistnode, szAliasUTF8);
	else if(PURPLE_BLIST_NODE_IS_GROUP(lpblistnode))
		purple_blist_rename_group((PurpleGroup*)lpblistnode, szAliasUTF8);

	g_free(szAliasUTF8);
}


/**
 * Removes a buddy-list from libpurple's buddy list.
 *
 * @param	lpblistnode	Buddy-list node.
 */
void PurpleDeleteBlistNode(PurpleBlistNode *lpblistnode)
{
	if(!lpblistnode)
		return;

	if(PURPLE_BLIST_NODE_IS_CHAT(lpblistnode))
	{
		/* Chats are easy, since they only exist in the buddy list and
		 * have no children.
		 */
		purple_blist_remove_chat((PurpleChat*)lpblistnode);
	}
	else if(PURPLE_BLIST_NODE_IS_BUDDY(lpblistnode))
	{
		DeleteBuddyFromAccount((PurpleBuddy*)lpblistnode);
		purple_blist_remove_buddy((PurpleBuddy*)lpblistnode);
	}
	else if(PURPLE_BLIST_NODE_IS_CONTACT(lpblistnode))
	{
		PurpleBlistNode *lpblistnodeBuddies;

		/* Remove any buddies from their respective accounts. */
		for(lpblistnodeBuddies = lpblistnode->child;
			lpblistnodeBuddies;
			lpblistnodeBuddies = lpblistnodeBuddies->next)
		{
			if(PURPLE_BLIST_NODE_IS_BUDDY(lpblistnodeBuddies))
				DeleteBuddyFromAccount((PurpleBuddy*)lpblistnodeBuddies);
		}

		purple_blist_remove_contact((PurpleContact*)lpblistnode);
	}
	else if(PURPLE_BLIST_NODE_IS_GROUP(lpblistnode))
	{
		PurpleBlistNode *lpblistnodeInGroup = lpblistnode->child;

		/* Traverse tree, deleting everything, and being careful with
		 * the list pointers.
		 */
		while(lpblistnodeInGroup)
		{
			if(PURPLE_BLIST_NODE_IS_CONTACT(lpblistnodeInGroup))
			{
				PurpleContact *lpcontact = (PurpleContact*)lpblistnodeInGroup;
				PurpleBlistNode *lpblistnodeBuddies = lpblistnode->child;
				lpblistnodeInGroup = lpblistnodeInGroup->next;

				while(lpblistnodeBuddies)
				{
					if(PURPLE_BLIST_NODE_IS_BUDDY(lpblistnodeBuddies))
					{
						PurpleBuddy *lpbuddy = (PurpleBuddy*)lpblistnodeBuddies;
						lpblistnodeBuddies = lpblistnodeBuddies->next;

						DeleteBuddyFromAccount(lpbuddy);
						purple_blist_remove_buddy(lpbuddy);
					}
					else
						lpblistnodeBuddies = lpblistnodeBuddies->next;
				}

				purple_blist_remove_contact(lpcontact);
			}
			else if(PURPLE_BLIST_NODE_IS_CHAT(lpblistnodeInGroup))
			{
				PurpleChat *lpchat = (PurpleChat*)lpblistnodeInGroup;
				lpblistnodeInGroup = lpblistnodeInGroup->next;
				purple_blist_remove_chat(lpchat);
			}
			else
				lpblistnodeInGroup = lpblistnodeInGroup->next;
		}

		purple_blist_remove_group((PurpleGroup*)lpblistnode);
	}
}


/**
 * Removes a buddy from its account, if that account is connected.
 *
 * @param	lpbuddy		Buddy.
 */
static void DeleteBuddyFromAccount(PurpleBuddy *lpbuddy)
{
	PurpleAccount *lpaccount = purple_buddy_get_account(lpbuddy);

	if(purple_account_is_connected(lpaccount))
		purple_account_remove_buddy(lpaccount, lpbuddy, purple_buddy_get_group(lpbuddy));
}


/**
 * Updates the status information cached in a buddy-list node.
 *
 * @param	lpblistnode	Buddy-list node.
 */
static void UpdateCachedStatus(PurpleBlistNode *lpblistnode)
{
	VULTURE_BLIST_NODE *lpvblistnode;

	if(!lpblistnode || !lpblistnode->ui_data)
		return;

	lpvblistnode = lpblistnode->ui_data;

	EnterCriticalSection(&lpvblistnode->cs);
	{
		if(lpvblistnode->szStatusText) g_free(lpvblistnode->szStatusText);
		lpvblistnode->szStatusText = NULL;

		if(PURPLE_BLIST_NODE_IS_BUDDY(lpblistnode))
		{
			lpvblistnode->szStatusText = PurpleBuddyGetStatusText((PurpleBuddy*)lpblistnode);
			lpvblistnode->iStatusIcon = GetStatusIconIndex((PurpleBuddy*)lpblistnode);
		}
		else if(PURPLE_BLIST_NODE_IS_CONTACT(lpblistnode))
		{
			PurpleBuddy *lpbuddy = purple_contact_get_priority_buddy((PurpleContact*)lpblistnode);

			if(lpbuddy)
			{
				lpvblistnode->szStatusText = PurpleBuddyGetStatusText(lpbuddy);
				lpvblistnode->iStatusIcon = GetStatusIconIndex(lpbuddy);
			}
		}
	}
	LeaveCriticalSection(&lpvblistnode->cs);
}


/**
 * Retrieves the status icon index for a buddy.
 *
 * @param	lpbuddy		Buddy.
 */
static int GetStatusIconIndex(PurpleBuddy *lpbuddy)
{
	const struct { PurpleStatusPrimitive statusprim; int iIndex; } c_rgprimiconpair[] =
	{
		{PURPLE_STATUS_UNAVAILABLE, SICON_BUSY},
		{PURPLE_STATUS_AWAY, SICON_AWAY},
		{PURPLE_STATUS_EXTENDED_AWAY, SICON_EXTAWAY},
		{PURPLE_STATUS_OFFLINE, SICON_OFFLINE},
		{PURPLE_STATUS_INVISIBLE, SICON_INVISIBLE}
	};

	PurplePresence *lppresence = purple_buddy_get_presence(lpbuddy);
	int i;

	for(i = 0; i < (int)NUM_ELEMENTS(c_rgprimiconpair); i++)
	{
		if(purple_presence_is_status_primitive_active(lppresence, c_rgprimiconpair[i].statusprim))
			return c_rgprimiconpair[i].iIndex;
	}

	return SICON_AVAILABLE;
}


/**
 * Called in response to buddy-icon-changed signal.
 *
 * @param	lpbuddy		Buddy.
 */
void PurpleBuddyIconChanged(PurpleBuddy *lpbuddy)
{
	VULTURE_BLIST_NODE *lpvbnContact = ((PurpleBlistNode*)lpbuddy)->parent ? ((PurpleBlistNode*)lpbuddy)->parent->ui_data : NULL;

	if(lpvbnContact)
	{
		if(lpvbnContact->bExpanded)
		{
			VULTURE_BLIST_NODE *lpvbn = ((PurpleBlistNode*)lpbuddy)->ui_data;

			VultureBListNodeAddRef(lpvbn);
			VulturePostUIMessage(VUIMSG_INVALIDATEICONCACHE, lpvbn);

			VultureBListNodeAddRef(lpvbn);
			VulturePostUIMessage(VUIMSG_UPDATEBLISTNODE, lpvbn);
		}

		VultureBListNodeAddRef(lpvbnContact);
		VulturePostUIMessage(VUIMSG_INVALIDATEICONCACHE, lpvbnContact);

		VultureBListNodeAddRef(lpvbnContact);
		VulturePostUIMessage(VUIMSG_UPDATEBLISTNODE, lpvbnContact);
	}

	/* We don't need to update any conversations showing our icon: this
	 * will be done in response to the conversation-updated signal.
	 */
}

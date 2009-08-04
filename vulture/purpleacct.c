/*
 * Vulture - Win32 libpurple client
 *
 * purpleacct.c: libpurple side of account-handling.
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
#include "purpleacct.h"
#include "acctmanager.h"
#include "purpleconv.h"


/**
 * Get a list of VULTURE_ACCOUNTs representing either all PurpleAccounts or
 * only those online, as required. Free the list with VultureFreeAccountList.
 *
 * @param	bOnlineOnly		Whether to retrieve online accounts
 *					only.
 * @param[out]	lplpglistAccounts	List to populate.
 */
void PurpleGetAccounts(BOOL bOnlineOnly, GList **lplpglistAccounts)
{
	GList *lpglistPurpleAcc;

	*lplpglistAccounts = NULL;

	for(lpglistPurpleAcc = purple_accounts_get_all(); lpglistPurpleAcc; lpglistPurpleAcc = lpglistPurpleAcc->next)
	{
		if(!bOnlineOnly || purple_account_is_connected((PurpleAccount*)lpglistPurpleAcc->data))
		{
			VULTURE_ACCOUNT *lpvac = g_new(VULTURE_ACCOUNT, 1);

			lpvac->lppac = (PurpleAccount*)lpglistPurpleAcc->data;
			lpvac->bEnabled = purple_account_get_enabled(lpvac->lppac, VULTURE_ID);

			lpvac->szUsername = VultureUTF8ToTCHAR(purple_account_get_username(lpvac->lppac));
			lpvac->szProtocolID = VultureUTF8ToTCHAR(purple_account_get_protocol_id(lpvac->lppac));

			*lplpglistAccounts = g_list_prepend(*lplpglistAccounts, lpvac);
		}
	}

	*lplpglistAccounts = g_list_reverse(*lplpglistAccounts);
}


/**
 * Frees a list of VULTURE_ACCOUNTS.
 *
 * @param	lpglistAccounts		List to free.
 */
void VultureFreeAccountList(GList *lpglistAccounts)
{
	GList *lpglistRover;

	for(lpglistRover = lpglistAccounts; lpglistRover; lpglistRover = lpglistRover->next)
	{
		VULTURE_ACCOUNT *lpvac = (VULTURE_ACCOUNT*)lpglistRover->data;

		g_free(lpvac->szUsername);
		g_free(lpvac->szProtocolID);
		g_free(lpvac);
	}

	g_list_free(lpglistAccounts);
}


/**
 * Updates the PurpleAccount underlying a VULTURE_ACCOUNT with properties from
 * the latter.
 *
 * @param	lpvac	Account.
 */
void PurpleApplyVultureAccount(VULTURE_ACCOUNT *lpvac)
{
	char *szUsername, *szProtocolID;

	szUsername = VultureTCHARToUTF8(lpvac->szUsername);
	szProtocolID = VultureTCHARToUTF8(lpvac->szProtocolID);

	purple_account_set_username(lpvac->lppac, szUsername);
	purple_account_set_protocol_id(lpvac->lppac, szProtocolID);
	purple_account_set_enabled(lpvac->lppac, VULTURE_ID, lpvac->bEnabled);

	g_free(szProtocolID);
	g_free(szUsername);
}


/**
 * Called in response to the signed-on account signal. Modelled on Pidgin's
 * account_signon_cb.
 *
 * @param	lpconnection	Connection for the account.
 * @param	lpvUnused	Ignored.
 */
void PurpleAccountSignedOn(PurpleConnection *lpconnection, gpointer lpvUnused)
{
	PurpleAccount *lpaccount = purple_connection_get_account(lpconnection);
	PurpleBlistNode *lpblistnodeGroup;

	UNREFERENCED_PARAMETER(lpvUnused);

	/* Loop through all groups. */
	for(lpblistnodeGroup = purple_get_blist()->root; lpblistnodeGroup; lpblistnodeGroup = lpblistnodeGroup->next)
	{
		if(PURPLE_BLIST_NODE_IS_GROUP(lpblistnodeGroup))
		{
			PurpleBlistNode *lpblistnodeChat;

			/* Loop through all chats in this group belonging to
			 * this account, autojoining any that request it.
			 */
			for(lpblistnodeChat = lpblistnodeGroup->child; lpblistnodeChat; lpblistnodeChat = lpblistnodeChat->next)
			{
				PurpleChat *lpchat = (PurpleChat*)lpblistnodeChat;

				if(PURPLE_BLIST_NODE_IS_CHAT(lpblistnodeChat) &&
					lpchat->account == lpaccount &&
					purple_blist_node_get_bool((PurpleBlistNode*)lpchat, "vulture-autojoin"))
				{
					PurpleJoinChat(lpchat);
				}
			}
		}
	}
}

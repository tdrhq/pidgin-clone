/**
 * purple
 *
 * Purple is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
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
#include "internal.h"

#include "account.h"
#include "debug.h"
#include "privacy.h"
#include "server.h"
#include "util.h"

static PurplePrivacyUiOps *privacy_ops = NULL;
GSList *get_account_members(PurpleAccount *account, PurplePrivacyListType type);

gboolean
purple_privacy_permit_add(PurpleAccount *account, const char *who,
						gboolean local_only)
{
	PurpleBuddy *buddy = NULL;
	char *name = NULL;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	purple_debug_info("Privacy","In purple_privacy_permit_add\n"); /* remove me laters */

	name = g_strdup(purple_normalize(account, who));

	purple_privacy_update_contact(account, name, local_only, TRUE, TRUE);

	if (!local_only && purple_account_is_connected(account))
		serv_add_permit(purple_account_get_connection(account), name);

	if (privacy_ops != NULL && privacy_ops->permit_added != NULL)
		privacy_ops->permit_added(account, name);

	/* privacy laters: Change signals later */
	/* This lets the UI know a buddy has had its privacy setting changed */
	buddy = purple_find_buddy(account, name);
	if (buddy != NULL) {
		purple_signal_emit(purple_blist_get_handle(),
                "buddy-privacy-changed", buddy);
	}
	g_free(name);
	return TRUE;
}

gboolean
purple_privacy_permit_remove(PurpleAccount *account, const char *who,
						   gboolean local_only)
{
	/* privacy laters: privacy settings after removing from the allow list would depend upon the account type and its privacy state.
	For now, lets assume that if you are not in buddy list and have been removed from the allow list, you are "everyone else".
	so there needs to be no information stored about you. If you are in buddy list then it depends on account type and state */

	PurpleBuddy *b = NULL;
	char *name = NULL;
 
	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	purple_debug_info("Privacy","In purple_privacy_permit_remove\n"); /* remove me laters */

	name = g_strdup(purple_normalize(account, who));

	if(!(b = purple_find_buddy(account, name)))
	{
		if(!(b = purple_find_privacy_contact(account, name)))
		{
			g_free(name);
			return FALSE;
		}
		else
			purple_blist_remove_buddy(b);	/* when in allow list,  you are in no other privacy list */
	}
	else
	{
		/* privacy laters: This now depends upon the account type and state, for now set TRUE, TRUE */
		purple_privacy_update_contact(account, name, local_only, TRUE, TRUE);
	}

	/* privacy laters: we have freed the buddy, check if some function in following statements don't require any data that we freed */

	if (!local_only && purple_account_is_connected(account))
		serv_rem_permit(purple_account_get_connection(account), name);

	if (privacy_ops != NULL && privacy_ops->permit_removed != NULL)
		privacy_ops->permit_removed(account, name);

	b = purple_find_buddy(account, name);
	if (b != NULL) {
		purple_signal_emit(purple_blist_get_handle(),
                "buddy-privacy-changed", b);
	}
	g_free(name);
	return TRUE;
}

gboolean
purple_privacy_deny_add(PurpleAccount *account, const char *who,
					  gboolean local_only)
{
	PurpleBuddy *buddy = NULL;
	char *name = NULL;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	purple_debug_info("Privacy","In purple_privacy_deny_add\n"); /* remove me laters */

	name = g_strdup(purple_normalize(account, who));
	purple_privacy_update_contact(account, name, local_only, TRUE, TRUE);

	if (!local_only && purple_account_is_connected(account))
		serv_add_deny(purple_account_get_connection(account), name);

	if (privacy_ops != NULL && privacy_ops->deny_added != NULL)
		privacy_ops->deny_added(account, name);

	/* privacy laters: Change signals later */
	/* This lets the UI know a buddy has had its privacy setting changed */
	buddy = purple_find_buddy(account, name);
	if (buddy != NULL) {
		purple_signal_emit(purple_blist_get_handle(),
                "buddy-privacy-changed", buddy);
	}
	g_free(name);
	return TRUE;
}

gboolean
purple_privacy_deny_remove(PurpleAccount *account, const char *who,
						 gboolean local_only)
{
	/* privacy laters: privacy settings according to account type and state */
	PurpleBuddy *b = NULL;
	char *name = NULL;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	purple_debug_info("Privacy","In purple_privacy_deny_remove\n"); /* remove me laters */

	name = g_strdup(purple_normalize(account, who));

	if(!(b = purple_find_buddy(account, name)))
	{
		if(!(b = purple_find_privacy_contact(account, name)))
		{
			g_free(name);
			return FALSE;
		}
		else
		{
			/* privacy laters: you can be (in)visible list along with block list, will figure out how laters, for now remove 
			this contact from list */
			purple_blist_remove_buddy(b);
		}
	}
	else
	{
		/* privacy laters: This now depends upon the account type and state, for now set TRUE, TRUE */
		purple_privacy_update_contact(account, name, local_only, TRUE, TRUE);
	}

	/* privacy laters: we have freed the buddy, check if some function in following statements don't require any data that we freed */

	if (!local_only && purple_account_is_connected(account))
		serv_rem_deny(purple_account_get_connection(account), name);

	if (privacy_ops != NULL && privacy_ops->deny_removed != NULL)
		privacy_ops->deny_removed(account, name);

	b = purple_find_buddy(account, name);
	if (b != NULL) {
		purple_signal_emit(purple_blist_get_handle(),
                "buddy-privacy-changed", b);
	}
	g_free(name);
	return TRUE;
}

/**
 * This makes sure your permit list contains all buddies from your
 * buddy list and ONLY buddies from your buddy list.
 */
static gboolean 
add_all_buddies_to_permit_list(PurpleAccount *account, gboolean local)
{
	GSList *p_list = NULL, *l_tmp = NULL;
	PurpleBuddy *b = NULL;

	g_return_val_if_fail(account != NULL, FALSE);

	p_list = purple_find_privacy_contacts(account, NULL);
	for(l_tmp = p_list; l_tmp; l_tmp = l_tmp->next)
	{
		b = l_tmp->data;
		if(purple_find_buddy(account, b->name))
			purple_privacy_permit_add(account, b->name, local);
		else
			purple_privacy_permit_remove(account, b->name, local);
	}

	g_slist_free(p_list);
	return TRUE; 
}

/*
 * TODO: All callers of this function pass in FALSE for local and
 *       restore and I don't understand when you would ever want to
 *       use TRUE for either of them.  I think both parameters could
 *       safely be removed in the next major version bump.
 */
void
purple_privacy_allow(PurpleAccount *account, const char *who, gboolean local,
						gboolean restore)
{
	GSList *list = NULL, *l_tmp = NULL;
	PurplePrivacyType type = account->perm_deny;
	PurpleBuddy *b = NULL;

	switch (account->perm_deny) {
		case PURPLE_PRIVACY_ALLOW_ALL:
			return;
		case PURPLE_PRIVACY_ALLOW_USERS:
			purple_privacy_permit_add(account, who, local);
			break;
		case PURPLE_PRIVACY_DENY_USERS:
			purple_privacy_deny_remove(account, who, local);
			break;
		case PURPLE_PRIVACY_DENY_ALL:
			if (!restore) {
				/* Empty the allow-list. */
				list = purple_find_privacy_contacts(account, NULL);
				for(l_tmp = list; l_tmp; l_tmp=l_tmp->next)
				{
					b = l_tmp->data;
					purple_privacy_permit_remove(account, b->name, local);
				}
			}
			purple_privacy_permit_add(account, who, local);
			account->perm_deny = PURPLE_PRIVACY_ALLOW_USERS;
			break;
		case PURPLE_PRIVACY_ALLOW_BUDDYLIST:
			if (!purple_find_buddy(account, who)) {
				add_all_buddies_to_permit_list(account, local);
				purple_privacy_permit_add(account, who, local);
				account->perm_deny = PURPLE_PRIVACY_ALLOW_USERS;
			}
			break;
		default:
			g_return_if_reached();
	}

	g_slist_free(list);
	/* Notify the server if the privacy setting was changed */
	if (type != account->perm_deny && purple_account_is_connected(account))
		serv_set_permit_deny(purple_account_get_connection(account));
}

/*
 * TODO: All callers of this function pass in FALSE for local and
 *       restore and I don't understand when you would ever want to
 *       use TRUE for either of them.  I think both parameters could
 *       safely be removed in the next major version bump.
 */
void
purple_privacy_deny(PurpleAccount *account, const char *who, gboolean local,
					gboolean restore)
{
	GSList *list = NULL, *l_tmp = NULL;
	PurpleBuddy *b = NULL;
	PurplePrivacyType type = account->perm_deny;

	switch (account->perm_deny) {
		case PURPLE_PRIVACY_ALLOW_ALL:
			if (!restore) {
				/* Empty the deny-list. */
				list = purple_find_privacy_contacts(account, NULL);
				for(l_tmp = list; l_tmp; l_tmp=l_tmp->next)
				{
					b = l_tmp->data;
					purple_privacy_deny_remove(account, b->name, local);
				}
			}
			purple_privacy_deny_add(account, who, local);
			account->perm_deny = PURPLE_PRIVACY_DENY_USERS;
			break;
		case PURPLE_PRIVACY_ALLOW_USERS:
			purple_privacy_permit_remove(account, who, local);
			break;
		case PURPLE_PRIVACY_DENY_USERS:
			purple_privacy_deny_add(account, who, local);
			break;
		case PURPLE_PRIVACY_DENY_ALL:
			break;
		case PURPLE_PRIVACY_ALLOW_BUDDYLIST:
			if (purple_find_buddy(account, who)) {
				add_all_buddies_to_permit_list(account, local);
				purple_privacy_permit_remove(account, who, local);
				account->perm_deny = PURPLE_PRIVACY_ALLOW_USERS;
			}
			break;
		default:
			g_return_if_reached();
	}

	g_slist_free(list);
	/* Notify the server if the privacy setting was changed */
	if (type != account->perm_deny && purple_account_is_connected(account))
		serv_set_permit_deny(purple_account_get_connection(account));
}

gboolean
purple_privacy_check_message(PurpleAccount *account, const char *who)
{
	/* Privacy later: When functions that set privacy will work based on account types/states, then this function will work perfectly */
	PurpleBuddy *b = NULL;
	char *name = NULL;
	gboolean receive_message;

	b = purple_find_privacy_contact(account, who);
	name = 	g_strdup(purple_normalize(account, who));

	if(b)
		receive_message = b->privacy_receive_message;
	else
	{
		/* privacy later: depends upon account type/state, for now return false */
		receive_message = FALSE;
	}
	g_free(name);
	return receive_message;
}

gboolean
purple_privacy_check_presence(PurpleAccount *account, const char *who)
{
	/* Privacy later: When functions that set privacy will work based on account types/states, then this function will work perfectly */
	PurpleBuddy *b = NULL;
	char *name = NULL;
	gboolean send_presence;

	b = purple_find_privacy_contact(account, who);
	name = 	g_strdup(purple_normalize(account, who));

	if(b)
		send_presence = b->privacy_send_presence;
	else
	{
		/* privacy later: depends upon account type/state, for now return false */
		send_presence = FALSE;
	}
	g_free(name);
	return send_presence;
}

gboolean
purple_privacy_check(PurpleAccount *account, const char *who)
{
	return purple_privacy_check_message(account,who);
}


void
purple_privacy_set_ui_ops(PurplePrivacyUiOps *ops)
{
	privacy_ops = ops;
}

PurplePrivacyUiOps *
purple_privacy_get_ui_ops(void)
{
	return privacy_ops;
}

void
purple_privacy_init(void)
{
}

gboolean purple_privacy_update_contact(PurpleAccount *account, const char *who, gboolean local_only, gboolean receive_message, gboolean send_presence)
{
	PurpleBuddy *b;
	PurpleGroup *g;
	char *name = NULL;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	name = g_strdup(purple_normalize(account, who));

	if((b = purple_find_privacy_contact(account, name)))
	{
		b->privacy_receive_message = receive_message;
		b->privacy_send_presence = send_presence;
		b->local_only = local_only;
	}
	else
	{
		if(!(g = purple_find_group(PURPLE_PRIVACY_GROUP)))
		{
			g = purple_group_new(PURPLE_PRIVACY_GROUP);
			purple_blist_add_group(g, NULL);
		}

		b = purple_buddy_new(account, name, NULL);
		b->privacy_receive_message = receive_message;
		b->privacy_send_presence = send_presence;
		b->local_only = local_only;
		purple_blist_add_buddy(b, NULL, g, NULL);
	}

	g_free(name);
	return TRUE;

	/* put the name in list on the server 
	if (!local_only && purple_account_is_connected(account))
		serv_add_deny(purple_account_get_connection(account), name); */

	/* notify UI that a user was denied
	if (privacy_ops != NULL && privacy_ops->deny_added != NULL)
		privacy_ops->deny_added(account, name); */

	/* emit signal
	buddy = purple_find_buddy(account, name);
	if (buddy != NULL) {
		purple_signal_emit(purple_blist_get_handle(),
                "buddy-privacy-changed", buddy);
	}
	return TRUE; */
}

gboolean purple_privacy_update_presence_setting(PurpleAccount *account, const char *who, gboolean send_presence)
{
	PurpleBuddy *b = NULL;
	char *name = NULL;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	name = g_strdup(purple_normalize(account, who));

	if((b = purple_find_privacy_contact(account, name)))
	{
		b->privacy_send_presence = send_presence;
		g_free(name);
		return TRUE;
	}
	else
	{
		g_free(name);
		return FALSE;
	}

	/* notify UI that a user was denied
	if (privacy_ops != NULL && privacy_ops->deny_added != NULL)
		privacy_ops->deny_added(account, name); */

	/* emit signal
	buddy = purple_find_buddy(account, name);
	if (buddy != NULL) {
		purple_signal_emit(purple_blist_get_handle(),
                "buddy-privacy-changed", buddy);
	}*/

}

gboolean purple_privacy_update_message_setting(PurpleAccount *account, const char *who, gboolean receive_message)
{
	PurpleBuddy *b = NULL;
	char *name = NULL;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	name = g_strdup(purple_normalize(account, who));

	if((b = purple_find_privacy_contact(account, name)))
	{
		b->privacy_receive_message = receive_message;
		g_free(name);
		return TRUE;
	}
	else
	{
		g_free(name);
		return FALSE;
	}

	/* notify UI that a user was denied
	if (privacy_ops != NULL && privacy_ops->deny_added != NULL)
		privacy_ops->deny_added(account, name); */

	/* emit signal
	buddy = purple_find_buddy(account, name);
	if (buddy != NULL) {
		purple_signal_emit(purple_blist_get_handle(),
                "buddy-privacy-changed", buddy);
	}*/

}
		
GSList *get_account_members(PurpleAccount *account, PurplePrivacyListType type)
{
	GSList *account_buddies = NULL, *list = NULL, *l_tmp = NULL;
	PurpleBuddy *b = NULL;
	gboolean receive_message = FALSE, send_presence = FALSE;

	switch(type)
	{
		case PURPLE_PRIVACY_ALLOW_LIST:
			receive_message = TRUE;
			send_presence = TRUE;
			break;
		case PURPLE_PRIVACY_BLOCK_MESSAGE_LIST:
			receive_message = FALSE;
			send_presence = TRUE;
			break;
		case PURPLE_PRIVACY_BLOCK_BOTH_LIST:
			receive_message = FALSE;
			send_presence = FALSE;
			break;
		case PURPLE_PRIVACY_INVISIBLE_LIST:
			receive_message = TRUE;
			send_presence = FALSE;
			break;
		case PURPLE_PRIVACY_BUDDY_LIST:
		case PURPLE_PRIVACY_VISIBLE_LIST:
			/* do nothing, we won't reach here anyways */
			break;
	}

	account_buddies = purple_find_privacy_contacts(account, NULL);
	for(l_tmp = account_buddies; l_tmp ; l_tmp=l_tmp->next)
	{
		b = l_tmp->data;
		if((b->privacy_receive_message == receive_message) && (b->privacy_send_presence == send_presence))
			list = g_slist_prepend(list, b->name);
	}

	g_slist_free(account_buddies);
	return list;
}

/* Privacy laters: GSList *purple_privacy_list_get_members(PurplePrivacyListType type) */

GSList *purple_privacy_list_get_members_by_account(PurpleAccount *account, PurplePrivacyListType type)
{
	g_return_val_if_fail(account != NULL, FALSE);

	if(type == PURPLE_PRIVACY_BUDDY_LIST)
		return purple_find_buddies(account, NULL);

	if(type != PURPLE_PRIVACY_VISIBLE_LIST)
		return get_account_members(account, type);
	else
	{
		GSList *l_allow = NULL, *l_block_msg = NULL;
		l_allow = get_account_members(account, PURPLE_PRIVACY_ALLOW_LIST);
		l_block_msg = get_account_members(account, PURPLE_PRIVACY_BLOCK_MESSAGE_LIST);
		return g_slist_concat(l_allow, l_block_msg);
	}
}

gboolean purple_privacy_sync_lists(PurpleAccount *account, GSList *buddy_l, GSList *allow_l, GSList *block_msg_l, GSList *block_both_l, GSList *visible_l, GSList *invisible_l)
{
	GSList *p_contacts = NULL, *l_tmp = NULL;
	PurpleBuddy *b = NULL;
	
	g_return_val_if_fail(account != NULL, FALSE);
	
	p_contacts = purple_find_privacy_contacts(account, NULL);

	/* Remove any contact not local, and not in any of the lists */
	for(l_tmp = p_contacts; l_tmp != NULL; l_tmp = l_tmp->next)
	{
		b = l_tmp->data;
		if(b->local_only == FALSE)
			if(	(g_slist_find_custom(buddy_l, b->name,(GCompareFunc)strcmp) == NULL) &&
				(g_slist_find_custom(allow_l, b->name,(GCompareFunc)strcmp) == NULL) &&
				(g_slist_find_custom(block_msg_l, b->name,(GCompareFunc)strcmp) == NULL) &&
				(g_slist_find_custom(block_both_l, b->name,(GCompareFunc)strcmp) == NULL) &&
				(g_slist_find_custom(visible_l, b->name,(GCompareFunc)strcmp) == NULL) &&
				(g_slist_find_custom(invisible_l, b->name,(GCompareFunc)strcmp) == NULL) )

					purple_blist_remove_buddy(b);
	}

	/* What if there was a non-buddy contact in some privacy list(s), but now you have added it in your buddy list */
	/* remove contact from privacy group, if now its in the buddy list */
	for(l_tmp = buddy_l; l_tmp != NULL; l_tmp = l_tmp->next)
		if((b = purple_find_buddy_in_group(account, (const char *)l_tmp->data, purple_find_group(PURPLE_PRIVACY_GROUP))))
			purple_blist_remove_buddy(b);

	/* Add the contacts from the master list to the blist */
	/* Process buddy list first. Assuming that we want to receive message
	and send presence from buddies in case they don't exist on any other list */

	for(l_tmp = buddy_l ; l_tmp != NULL; l_tmp = l_tmp->next)
		purple_privacy_update_contact(account, l_tmp->data, FALSE, TRUE, TRUE);

	for(l_tmp = allow_l ; l_tmp != NULL; l_tmp = l_tmp->next)
		purple_privacy_update_contact(account, l_tmp->data, FALSE, TRUE, TRUE);

	for(l_tmp = block_msg_l ; l_tmp != NULL; l_tmp = l_tmp->next)
		purple_privacy_update_contact(account, l_tmp->data, FALSE, FALSE, TRUE);

	for(l_tmp = block_both_l ; l_tmp != NULL; l_tmp = l_tmp->next)
		purple_privacy_update_contact(account, l_tmp->data, FALSE, FALSE, FALSE);

	/* Can't determine contact's message privacy setting by its presence on visible/invisible list alone. So we process these cases in the last, 		assuming message privacy setting has already been done or assumed to be willing to receive messages */
	/* Create contact with TRUE, TRUE state first, before updating its presence according to visible/invisible list */
	for(l_tmp = visible_l ; l_tmp != NULL; l_tmp = l_tmp->next)
	{		
		purple_privacy_update_contact(account, l_tmp->data, FALSE, TRUE, TRUE);
		purple_privacy_update_presence_setting(account, l_tmp->data, TRUE);
	}
	for(l_tmp = invisible_l ; l_tmp != NULL; l_tmp = l_tmp->next)
	{		
		purple_privacy_update_contact(account, l_tmp->data, FALSE, TRUE, TRUE);
		purple_privacy_update_presence_setting(account, l_tmp->data, FALSE);
	}
	g_slist_free(p_contacts);

	return TRUE;	
}

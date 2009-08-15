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
#include "savedstatuses.h"
#include "server.h"
#include "util.h"

static int privacy_handle;

static PurplePrivacySetting purple_privacy_state;

static PurplePrivacyUiOps *privacy_ops = NULL;
GSList *get_account_members(PurpleAccount *account, PurplePrivacyListType type);
static void acc_signed_on_cb(PurpleConnection *gc, void *data);
static void acc_signed_off_cb(PurpleConnection *gc, void *data);

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
	/* Set default purple privacy state to PURPLE_PRIVACY_ALLOW_BUDDYLIST */
	purple_privacy_state = PURPLE_PRIVACY_ALLOW_BUDDYLIST;

	/* Register the purple event callbacks. */
	purple_signal_connect(purple_connections_get_handle(), "signed-on",
						purple_privacy_get_handle(), PURPLE_CALLBACK(acc_signed_on_cb), NULL);
	purple_signal_connect(purple_connections_get_handle(), "signed-off",
						purple_privacy_get_handle(), PURPLE_CALLBACK(acc_signed_off_cb), NULL);
}

void
purple_privacy_uninit(void)
{
	purple_signals_disconnect_by_handle(purple_privacy_get_handle());
	purple_signals_unregister_by_instance(purple_privacy_get_handle());
}

gboolean purple_privacy_set_blocking_context(PurpleAccount *account, const char *who, gboolean local_only, PurplePrivacyContext context)
{
	PurpleBuddy *b;
	PurpleGroup *g;
	char *name = NULL;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if((b = purple_find_privacy_contact(account, name)))
	{
		b->privacy_block_context = context;
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
		b->privacy_block_context = context;
		b->local_only = local_only;
		purple_blist_add_buddy(b, NULL, g, NULL);
	}

	g_free(name);
	return TRUE;
}

PurplePrivacyContext purple_privacy_get_blocking_context(PurpleAccount *account, const char *who)
{
	PurpleBuddy *b;
	char *name = NULL;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if((b = purple_find_privacy_contact(account, name)))
		return b->privacy_block_context;
	else
	{
		if((b = purple_find_privacy_contact(account, "all-others")))
			return b->privacy_block_context;
		else
			return 0;
	}
}

gboolean purple_privacy_set_block_all(PurpleAccount *account, const char *who, gboolean local, gboolean server)
{
	char *name;
	PurplePrivacyContext context;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	context = PURPLE_PRIVACY_BLOCK_ALL | PURPLE_PRIVACY_BLOCK_MESSAGE | PURPLE_PRIVACY_BLOCK_PRESENCE |
			PURPLE_PRIVACY_BLOCK_FT | PURPLE_PRIVACY_BLOCK_CONF;

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if(local)
		purple_privacy_set_blocking_context(account, name, !server, context);

	if(server && purple_account_is_connected(account) && 
				purple_privacy_check_list_support(account, PURPLE_PRIVACY_BLOCK_BOTH_LIST) &&
				(!purple_strequal(name, "all-others")) )
		serv_privacy_list_add(purple_account_get_connection(account), PURPLE_PRIVACY_BLOCK_BOTH_LIST, name);

	g_free(name);
	return TRUE;
}

gboolean purple_privacy_set_block_presence(PurpleAccount *account, const char *who, gboolean local, gboolean server)
{
	char *name;
	PurplePrivacyContext context;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if(!purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_PRESENCE))
	{
		if(local)
		{
			context = ( purple_privacy_get_blocking_context(account, name) | PURPLE_PRIVACY_BLOCK_PRESENCE );
			purple_privacy_set_blocking_context(account, name, !server, context);
		}

		if(server && purple_account_is_connected(account) && purple_privacy_check_list_support(account, PURPLE_PRIVACY_INVISIBLE_LIST) )
			serv_privacy_list_add(purple_account_get_connection(account), PURPLE_PRIVACY_INVISIBLE_LIST, name);
	}

	g_free(name);
	return TRUE;
}

gboolean purple_privacy_set_block_message(PurpleAccount *account, const char *who)
{
	char *name;
	PurplePrivacyContext context;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if(!purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_MESSAGE))
	{
		context = ( purple_privacy_get_blocking_context(account, name) | PURPLE_PRIVACY_BLOCK_MESSAGE );
		purple_privacy_set_blocking_context(account, name, TRUE, context);
	}

	g_free(name);
	return TRUE;
}

gboolean purple_privacy_set_block_ft(PurpleAccount *account, const char *who)
{
	char *name;
	PurplePrivacyContext context;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if(!purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_FT))
	{
		context = ( purple_privacy_get_blocking_context(account, name) | PURPLE_PRIVACY_BLOCK_FT );
		purple_privacy_set_blocking_context(account, name, TRUE, context);
	}

	g_free(name);
	return TRUE;
}

gboolean purple_privacy_set_block_conf(PurpleAccount *account, const char *who)
{
	char *name;
	PurplePrivacyContext context;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if(!purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_CONF))
	{
		context = ( purple_privacy_get_blocking_context(account, name) | PURPLE_PRIVACY_BLOCK_CONF );
		purple_privacy_set_blocking_context(account, name, TRUE, context);
	}

	g_free(name);
	return TRUE;
}

gboolean purple_privacy_unset_block_message(PurpleAccount *account, const char *who)
{
	char *name;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if(purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_MESSAGE))
	{
		if(purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_ALL))
		{
			purple_privacy_unset_block_all(account, name, TRUE, TRUE);
			purple_privacy_set_block_presence(account, name, TRUE, TRUE);
			purple_privacy_set_block_ft(account, name);
			purple_privacy_set_block_conf(account, name);
		}
		else
		{
			PurplePrivacyContext context;
			context = purple_privacy_get_blocking_context(account, name) & (~PURPLE_PRIVACY_BLOCK_MESSAGE);
			purple_privacy_set_blocking_context(account, name, TRUE, context);
		}
	}

	g_free(name);
	return TRUE;
}

gboolean purple_privacy_unset_block_conf(PurpleAccount *account, const char *who)
{
	char *name;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if(purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_CONF))
	{
		if(purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_ALL))
		{
			purple_privacy_unset_block_all(account, name, TRUE, TRUE);
			purple_privacy_set_block_presence(account, name, TRUE, TRUE);
			purple_privacy_set_block_ft(account, name);
			purple_privacy_set_block_message(account, name);
		}
		else
		{
			PurplePrivacyContext context;
			context = purple_privacy_get_blocking_context(account, name) & (~PURPLE_PRIVACY_BLOCK_CONF);
			purple_privacy_set_blocking_context(account, name, TRUE, context);
		}
	}

	g_free(name);
	return TRUE;
}

gboolean purple_privacy_unset_block_ft(PurpleAccount *account, const char *who)
{
	char *name;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if(purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_FT))
	{
		if(purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_ALL))
		{
			purple_privacy_unset_block_all(account, name, TRUE, TRUE);
			purple_privacy_set_block_presence(account, name, TRUE, TRUE);
			purple_privacy_set_block_conf(account, name);
			purple_privacy_set_block_message(account, name);
		}
		else
		{
			PurplePrivacyContext context;
			context = purple_privacy_get_blocking_context(account, name) & (~PURPLE_PRIVACY_BLOCK_FT);
			purple_privacy_set_blocking_context(account, name, TRUE, context);
		}
	}

	g_free(name);
	return TRUE;
}

gboolean purple_privacy_unset_block_all(PurpleAccount *account, const char *who, gboolean local, gboolean server)
{
	char *name;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if(purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_ALL))
	{
		if(local)
			purple_privacy_set_blocking_context(account, name, !server, 0);

		if(server && purple_account_is_connected(account) && 
				purple_privacy_check_list_support(account, PURPLE_PRIVACY_BLOCK_BOTH_LIST) &&
					(!purple_strequal(who, "all-others")) )
			serv_privacy_list_remove(purple_account_get_connection(account), PURPLE_PRIVACY_BLOCK_BOTH_LIST, name);
	}

	g_free(name);
	return TRUE;
}

gboolean purple_privacy_unset_block_presence(PurpleAccount *account, const char *who, gboolean local, gboolean server)
{
	char *name;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if(purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_PRESENCE))
	{
		if(local)
		{
			if(purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_ALL))
			{
				purple_privacy_unset_block_all(account, name, TRUE, TRUE);
				purple_privacy_set_block_conf(account, name);
				purple_privacy_set_block_message(account, name);
				purple_privacy_set_block_ft(account, name);
			}
			else
			{
				PurplePrivacyContext context;
				
				context = ( purple_privacy_get_blocking_context(account, who) & (~PURPLE_PRIVACY_BLOCK_PRESENCE) );
				purple_privacy_set_blocking_context(account, name, !server, context);
			}
		}

		if(server && purple_account_is_connected(account) && 
				purple_privacy_check_list_support(account, PURPLE_PRIVACY_INVISIBLE_LIST) &&
					(!purple_strequal(who, "all-others")) )
			serv_privacy_list_remove(purple_account_get_connection(account), PURPLE_PRIVACY_INVISIBLE_LIST, name);
	}

	g_free(name);
	return TRUE;
}

gboolean purple_privacy_check(PurpleAccount *account, const char *who, PurplePrivacyContext context)
{
	char *name;
	gboolean value;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(who     != NULL, FALSE);

	if(purple_strequal(who, "all-others"))
		name = g_strdup(who);
	else
		name = g_strdup(purple_normalize(account, who));

	if((purple_privacy_get_blocking_context(account, name) & context) == context)
		value = TRUE;
	else
		value = FALSE;

	g_free(name);
	return value;
}
	
gboolean purple_privacy_check_list_support(PurpleAccount *account, PurplePrivacyListType type)
{
	return ( (account->privacy_spec->supported_privacy_lists & type) == type ? TRUE : FALSE );
}

GSList *get_account_members(PurpleAccount *account, PurplePrivacyListType type)
{
	GSList *account_buddies = NULL, *list = NULL, *l_tmp = NULL;
	PurpleBuddy *b = NULL;
	PurplePrivacyContext context = 0;

	switch(type)
	{
		case PURPLE_PRIVACY_ALLOW_LIST:
			context = 0;
			break;
		case PURPLE_PRIVACY_BLOCK_MESSAGE_LIST:
			context = PURPLE_PRIVACY_BLOCK_MESSAGE;
			break;
		case PURPLE_PRIVACY_BLOCK_BOTH_LIST:
			context = PURPLE_PRIVACY_BLOCK_ALL;
			break;
		case PURPLE_PRIVACY_INVISIBLE_LIST:
			context = PURPLE_PRIVACY_BLOCK_PRESENCE;
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
		if( ((b->privacy_block_context & context) == context ) && (purple_strequal(b->name, "all-others") == FALSE) )
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

	/* remove contact from privacy group, if now it is in the buddy list */
	for(l_tmp = buddy_l; l_tmp != NULL; l_tmp = l_tmp->next)
		if((b = purple_find_buddy_in_group(account, (const char *)l_tmp->data, purple_find_group(PURPLE_PRIVACY_GROUP))))
			purple_blist_remove_buddy(b);

	/* Add the contacts from the master list to the blist */
	/* Process buddy list first. Assuming that we want to receive message
	and send presence from buddies in case they don't exist on any other list */

	for(l_tmp = buddy_l ; l_tmp != NULL; l_tmp = l_tmp->next)
		purple_privacy_unset_block_all(account, l_tmp->data, TRUE, FALSE);

	for(l_tmp = allow_l ; l_tmp != NULL; l_tmp = l_tmp->next)
		purple_privacy_unset_block_all(account, l_tmp->data, TRUE, FALSE);

	/* Privacy laters: this is not correct */
	for(l_tmp = block_msg_l ; l_tmp != NULL; l_tmp = l_tmp->next)
		purple_privacy_set_block_message(account, l_tmp->data);

	for(l_tmp = block_both_l ; l_tmp != NULL; l_tmp = l_tmp->next)
		purple_privacy_set_block_all(account, l_tmp->data, TRUE, FALSE);

	for(l_tmp = visible_l ; l_tmp != NULL; l_tmp = l_tmp->next)
		purple_privacy_unset_block_presence(account, l_tmp->data, TRUE, FALSE);

	for(l_tmp = invisible_l ; l_tmp != NULL; l_tmp = l_tmp->next)
		purple_privacy_set_block_presence(account, l_tmp->data, TRUE, FALSE);

	g_slist_free(p_contacts);

	return TRUE;	
}

gboolean purple_privacy_account_supports_invisible_status(PurpleAccount *account)
{
	g_return_val_if_fail(account != NULL, FALSE);

	if (purple_account_get_status_type_with_primitive(account, PURPLE_STATUS_INVISIBLE))
		return TRUE;
	else
		return FALSE;
}

gboolean purple_privacy_account_status_invisible(PurpleAccount *account)
{
	PurpleStatus *status;
	PurpleStatusType *status_type;

	g_return_val_if_fail(account != NULL, FALSE);

	status = purple_account_get_active_status(account);
	status_type = purple_status_get_type(status);
	
	if(purple_status_type_get_primitive(status_type) == PURPLE_STATUS_INVISIBLE)
		return TRUE;
	else
		return FALSE;
}

gboolean purple_privacy_set_account_status_invisible(PurpleAccount *account)
{
	PurpleStatusType *status_type;

	g_return_val_if_fail(account != NULL, FALSE);

	/* Check whether this account supports invisible status */
	if ( !(status_type = purple_account_get_status_type_with_primitive(account, PURPLE_STATUS_INVISIBLE)) )
		return FALSE;

	/* Enable the status type with primitive "PURPLE_STATUS_INVISIBLE" */
	purple_account_set_status(account, purple_status_type_get_id(status_type), TRUE, NULL);
	
	return TRUE;
}

static gboolean test_status(PurpleSavedStatus *saved_status)
{
	gboolean ret = FALSE;
	PurpleStatusPrimitive prim;

	g_return_val_if_fail(saved_status != NULL, FALSE);

	prim = purple_savedstatus_get_type(saved_status);

	switch(prim)
	{
		case PURPLE_STATUS_INVISIBLE:
		case PURPLE_STATUS_OFFLINE:
		case PURPLE_STATUS_UNSET:
		case PURPLE_STATUS_NUM_PRIMITIVES: /* shouldn't have this value */
			ret = FALSE;
			break;
		default:
			ret = TRUE;
			break;
	}

	return ret;
}

/* Privacy Laters: this next function isn't perfect just yet.
Doesn't change status window status when current status is invisible and we change it to 
visible from the privacy UI */
gboolean purple_privacy_set_account_status_visible(PurpleAccount *account)
{
	PurpleSavedStatus *saved_status;

	g_return_val_if_fail(account != NULL, FALSE);

	saved_status = purple_savedstatus_get_current();
	if( test_status(saved_status) ) {
		purple_savedstatus_activate_for_account(saved_status, account);
		return TRUE;
	}
	else	{
		/* Our current saved status is Invisible or not Online, check default status */
		saved_status = purple_savedstatus_get_default();
		if( test_status(saved_status) ) {
			purple_savedstatus_activate_for_account(saved_status, account);
			return TRUE;
		}
		else	{
			/* Both current and default saved status are Invisible or not online, we choose "available" */
			if( (saved_status = purple_savedstatus_find_transient_by_type_and_message(PURPLE_STATUS_AVAILABLE, NULL)) )
				purple_savedstatus_activate_for_account(saved_status, account);
			else
				purple_account_set_status(account, "available", TRUE, NULL);

			return TRUE;
		}
	}
	/* Couldn't find any suitable status */
	return FALSE;
}

PurplePrivacySetting purple_privacy_obtain_global_state(void)
{
	return purple_privacy_state;
}

PurplePrivacySetting purple_privacy_obtain_account_state(PurpleAccount *account)
{
	g_return_val_if_fail(account != NULL, FALSE);

	return account->account_privacy_state;
}

NativePrivacySetting purple_privacy_obtain_native_state(PurpleAccount *account)
{
	g_return_val_if_fail(account != NULL, FALSE);

	return account->native_privacy_state;
}

void *purple_privacy_get_handle(void)
{
	return &privacy_handle;
}

/* Checks for the existence of "all-others" meta contact, adds it to blist if not found */
static void set_all_others_meta_contact(PurpleAccount *account)
{
	PurplePrivacyContext context;
	PurplePrivacySetting state;

	if(account == NULL)
		return;

	state = purple_privacy_obtain_account_state(account);

	switch (state)
	{
		case PURPLE_PRIVACY_ALLOW_ALL:
			context = 0;
			break;
		case PURPLE_PRIVACY_BLOCK_MSG_NONBUDDY:
			context = PURPLE_PRIVACY_BLOCK_MESSAGE;
			break;
		case PURPLE_PRIVACY_ALLOW_BUDDYLIST:
		case PURPLE_PRIVACY_CUSTOM:
			context = PURPLE_PRIVACY_BLOCK_ALL | PURPLE_PRIVACY_BLOCK_MESSAGE | PURPLE_PRIVACY_BLOCK_PRESENCE |
						PURPLE_PRIVACY_BLOCK_FT | PURPLE_PRIVACY_BLOCK_CONF;
			break;
	}

	purple_privacy_set_blocking_context(account, "all-others", TRUE, context);
}

static void
acc_signed_on_cb(PurpleConnection *gc, void *data)
{
	PurplePrivacySetting state;
	PurpleAccount *account = purple_connection_get_account(gc);

	state = account->account_privacy_state;

	if( (account->native_privacy_state == PURPLE_PRIVACY_NATIVE_DENY_USERS) &&
			(account->account_privacy_state == PURPLE_PRIVACY_ALLOW_ALL) )
	{
			if(purple_privacy_state != PURPLE_PRIVACY_ALLOW_ALL)
				state = purple_privacy_state;
			else
				state = PURPLE_PRIVACY_CUSTOM;
	}

	purple_privacy_set_account_state(account, state);
}

static void autoset_global_state(void)
{
	GList *list = NULL, *l = NULL;
	PurpleAccount *account;
	PurplePrivacySetting state;

	state = PURPLE_PRIVACY_ALLOW_BUDDYLIST;
	list = purple_accounts_get_all_active();
	if(list)
	{
		account = list->data;
		state = purple_privacy_obtain_account_state(account);
	}
	for( l = list; l != NULL; l = l->next )
	{
		account = l->data;
		if( state != purple_privacy_obtain_account_state(account) ) {
			purple_privacy_state = PURPLE_PRIVACY_CUSTOM;
			return;
		}
	}

	purple_privacy_state = state;
}

static void acc_signed_off_cb(PurpleConnection *gc, void *data)
{
	autoset_global_state();
}

static void set_list_local(PurpleAccount *account, GSList *l, gboolean local)
{
	PurpleBuddy *b;

	while( l != NULL)
	{
		b = purple_find_privacy_contact(account, (char *)l->data);
		if(b)
			b->local_only = local;

		l = l->next;
	}
}

void purple_privacy_set_account_state(PurpleAccount *account, PurplePrivacySetting state)
{
	GSList *list = NULL, *l = NULL;

	if(state == account->account_privacy_state)
		return;

	account->account_privacy_state = state;
	autoset_global_state();

	if(state == PURPLE_PRIVACY_ALLOW_ALL)
	{
		if ( (list = purple_privacy_list_get_members_by_account(account, PURPLE_PRIVACY_BLOCK_BOTH_LIST)) )
		{
			/* We empty deny list on the server, set local list inactive so we have the list to retrieve laters */
			for(l = list; l != NULL; l = l->next)
				serv_privacy_list_remove(purple_account_get_connection(account), PURPLE_PRIVACY_BLOCK_BOTH_LIST, (char *)l->data);

			set_list_local(account, list, TRUE);
			account->privacy_list_active = FALSE;
		}
		g_slist_free(list);
		account->native_privacy_state = PURPLE_PRIVACY_NATIVE_ALLOW_ALL;
	}
	else
	{
		/* Retrieve local deny list and sync with the server : in case privacy list is inactive */
		if( !(account->privacy_list_active) )
		{
			if ( (list = purple_privacy_list_get_members_by_account(account, PURPLE_PRIVACY_BLOCK_BOTH_LIST)) )
			{
				for(l = list; l != NULL; l = l->next)
					serv_privacy_list_add(purple_account_get_connection(account), PURPLE_PRIVACY_BLOCK_BOTH_LIST, (char *)l->data);

				account->native_privacy_state = PURPLE_PRIVACY_NATIVE_DENY_USERS;
			}
			else
				account->native_privacy_state = PURPLE_PRIVACY_NATIVE_ALLOW_ALL;
				
			g_slist_free(list);
			set_list_local(account, list, FALSE);		
			account->privacy_list_active = TRUE;
		}
	}
	set_all_others_meta_contact(account);
}

void purple_privacy_set_global_state(PurplePrivacySetting state)
{
	GList *l = NULL, *list = NULL;
	PurpleAccount *account;

	list = purple_accounts_get_all_active();
	
	if(state != PURPLE_PRIVACY_CUSTOM)	{
		for( l = list; l != NULL; l = l->next )	{
			account = l->data;
			purple_privacy_set_account_state(account, state);
		}
	}

	g_list_free(list);
}


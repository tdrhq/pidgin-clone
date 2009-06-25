/*
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
 *
 */
#define _PURPLE_BUDDY_C_
#define _BLIST_HELPERS_

#include "internal.h"
#include "blist.h"
#include "conversation.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "notify.h"
#include "prefs.h"
#include "privacy.h"
#include "prpl.h"
#include "server.h"
#include "signals.h"
#include "util.h"
#include "value.h"
#include "xmlnode.h"

static GObjectClass *parent_class = NULL;

void
parse_buddy(PurpleGroup *group, PurpleContact *contact, xmlnode *bnode)
{
	PurpleAccount *account;
	PurpleBuddy *buddy;
	char *name = NULL, *alias = NULL;
	const char *acct_name, *proto, *protocol;
	xmlnode *x;

	acct_name = xmlnode_get_attrib(bnode, "account");
	protocol = xmlnode_get_attrib(bnode, "protocol");
	protocol = _purple_oscar_convert(acct_name, protocol); /* XXX: Remove */
	proto = xmlnode_get_attrib(bnode, "proto");
	proto = _purple_oscar_convert(acct_name, proto); /* XXX: Remove */

	if (!acct_name || (!proto && !protocol))
		return;

	account = purple_accounts_find(acct_name, proto ? proto : protocol);

	if (!account)
		return;

	if ((x = xmlnode_get_child(bnode, "name")))
		name = xmlnode_get_data(x);

	if (!name)
		return;

	if ((x = xmlnode_get_child(bnode, "alias")))
		alias = xmlnode_get_data(x);

	buddy = purple_buddy_new(account, name, alias);
	purple_blist_add_buddy(buddy, contact, group,
	purple_blist_get_last_child((PurpleBlistNode*)contact));

	for (x = xmlnode_get_child(bnode, "setting"); x; x = xmlnode_get_next_twin(x)) {
		parse_setting((PurpleBlistNode*)buddy, x);
	}

	g_free(name);
	g_free(alias);
}

static void
append_buddy(gpointer key, gpointer value, gpointer user_data)
{
	GSList **list = user_data;
	*list = g_slist_prepend(*list, value);
}

GSList *
purple_blist_get_buddies()
{
	GSList *buddies = NULL;

	if (!purplebuddylist)
		return NULL;

	g_hash_table_foreach(purplebuddylist->buddies, append_buddy, &buddies);
	return buddies;
}

void
purple_blist_update_buddy_status(PurpleBuddy *buddy, PurpleStatus *old_status)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurplePresence *presence;
	PurpleStatus *status;
	PurpleBlistNode *cnode;

	g_return_if_fail(buddy != NULL);

	presence = purple_buddy_get_presence(buddy);
	status = purple_presence_get_active_status(presence);

	purple_debug_info("blist", "Updating buddy status for %s (%s)\n",
			buddy->name, purple_account_get_protocol_name(buddy->account));

	if (purple_status_is_online(status) &&
		!purple_status_is_online(old_status)) {

		purple_signal_emit(purple_blist_get_handle(), "buddy-signed-on", buddy);

		cnode = buddy->node.parent;
		if (++(PURPLE_CONTACT(cnode)->online) == 1)
			PURPLE_GROUP(cnode->parent)->online++;
	} else if (!purple_status_is_online(status) &&
				purple_status_is_online(old_status)) {

		purple_blist_node_set_int(&buddy->node, "last_seen", time(NULL));
		purple_signal_emit(purple_blist_get_handle(), "buddy-signed-off", buddy);

		cnode = buddy->node.parent;
		if (--(PURPLE_CONTACT(cnode)->online) == 0)
			PURPLE_GROUP(cnode->parent)->online--;
	} else {
		purple_signal_emit(purple_blist_get_handle(),
		                 "buddy-status-changed", buddy, old_status,
		                 status);
	}

	/*
	 * This function used to only call the following two functions if one of
	 * the above signals had been triggered, but that's not good, because
	 * if someone's away message changes and they don't go from away to back
	 * to away then no signal is triggered.
	 *
	 * It's a safe assumption that SOMETHING called this function.  PROBABLY
	 * because something, somewhere changed.  Calling the stuff below
	 * certainly won't hurt anything.  Unless you're on a K6-2 300.
	 */
	purple_contact_invalidate_priority_buddy(purple_buddy_get_contact(buddy));
	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode *)buddy);
}

void
purple_blist_update_buddy_icon(PurpleBuddy *buddy)
{
	purple_blist_update_node_icon((PurpleBlistNode *)buddy);
}

/*
 * TODO: Maybe remove the call to this from server.c and call it
 * from oscar.c and toc.c instead?
 */
void purple_blist_rename_buddy(PurpleBuddy *buddy, const char *name)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	struct _purple_hbuddy *hb, *hb2;
	GHashTable *account_buddies;

	g_return_if_fail(buddy != NULL);

	hb = g_new(struct _purple_hbuddy, 1);
	hb->name = g_strdup(purple_normalize(buddy->account, buddy->name));
	hb->account = buddy->account;
	hb->group = ((PurpleBlistNode *)buddy)->parent->parent;
	g_hash_table_remove(purplebuddylist->buddies, hb);
	
	account_buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, buddy->account);
	g_hash_table_remove(account_buddies, hb);

	g_free(hb->name);
	hb->name = g_strdup(purple_normalize(buddy->account, name));
	g_hash_table_replace(purplebuddylist->buddies, hb, buddy);

	hb2 = g_new(struct _purple_hbuddy, 1);
	hb2->name = g_strdup(hb->name);
	hb2->account = buddy->account;
	hb2->group = ((PurpleBlistNode *)buddy)->parent->parent;

	g_hash_table_replace(account_buddies, hb2, buddy);

	g_free(buddy->name);
	buddy->name = g_strdup(name);

	purple_blist_schedule_save();

	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode *)buddy);
}

void purple_blist_alias_buddy(PurpleBuddy *buddy, const char *alias)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleConversation *conv;
	char *old_alias;
	char *new_alias = NULL;

	g_return_if_fail(buddy != NULL);

	if ((alias != NULL) && (*alias != '\0'))
		new_alias = purple_utf8_strip_unprintables(alias);

	if (!purple_strings_are_different(buddy->alias, new_alias)) {
		g_free(new_alias);
		return;
	}

	old_alias = buddy->alias;

	if ((new_alias != NULL) && (*new_alias != '\0'))
		buddy->alias = g_strdup(alias);
	else {
		buddy->alias = NULL;
		g_free(new_alias); /* could be "\0" */
	}

	purple_blist_schedule_save();

	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode *)buddy);

	conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, buddy->name,
											   buddy->account);
	if (conv)
		purple_conversation_autoset_title(conv);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-aliased",
					 buddy, old_alias);
	g_free(old_alias);
}

void purple_blist_server_alias_buddy(PurpleBuddy *buddy, const char *alias)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleConversation *conv;
	char *old_alias;
	char *new_alias = NULL;

	g_return_if_fail(buddy != NULL);

	if ((alias != NULL) && (*alias != '\0') && g_utf8_validate(alias, -1, NULL))
		new_alias = purple_utf8_strip_unprintables(alias);

	if (!purple_strings_are_different(buddy->server_alias, new_alias)) {
		g_free(new_alias);
		return;
	}

	old_alias = buddy->server_alias;

	if ((new_alias != NULL) && (*new_alias != '\0'))
		buddy->server_alias = new_alias;
	else {
		buddy->server_alias = NULL;
		g_free(new_alias); /* could be "\0"; */
	}

	purple_blist_schedule_save();

	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode *)buddy);

	conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, buddy->name,
											   buddy->account);
	if (conv)
		purple_conversation_autoset_title(conv);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-aliased",
					 buddy, old_alias);
	g_free(old_alias);
}

PurpleBuddy *purple_buddy_new(PurpleAccount *account, const char *name, const char *alias)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleBuddy *buddy;

	g_return_val_if_fail(account != NULL, NULL);
	g_return_val_if_fail(name != NULL, NULL);

	buddy = g_object_new(PURPLE_BUDDY_TYPE, NULL);
  g_object_ref(buddy);
	buddy->account  = account;
	buddy->name     = purple_utf8_strip_unprintables(name);
	buddy->alias    = purple_utf8_strip_unprintables(alias);
	buddy->presence = purple_presence_new_for_buddy(buddy);
	((PurpleBlistNode *)buddy)->type = PURPLE_BLIST_BUDDY_NODE;

	purple_presence_set_status_active(buddy->presence, "offline", TRUE);

	purple_blist_node_initialize_settings((PurpleBlistNode *)buddy);

	if (ops && ops->new_node)
		ops->new_node((PurpleBlistNode *)buddy);

	PURPLE_DBUS_REGISTER_POINTER(buddy, PurpleBuddy);
	return buddy;
}

void
purple_buddy_destroy(PurpleBuddy *buddy)
{
	g_return_if_fail(PURPLE_IS_BUDDY(buddy));
  g_object_unref(G_OBJECT(buddy));
}

void
purple_buddy_set_icon(PurpleBuddy *buddy, PurpleBuddyIcon *icon)
{
	g_return_if_fail(buddy != NULL);

	if (buddy->icon != icon)
	{
		purple_buddy_icon_unref(buddy->icon);
		buddy->icon = (icon != NULL ? purple_buddy_icon_ref(icon) : NULL);
	}

	purple_signal_emit(purple_blist_get_handle(), "buddy-icon-changed", buddy);

	purple_blist_update_node_icon((PurpleBlistNode*)buddy);
}

PurpleAccount *
purple_buddy_get_account(const PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	return buddy->account;
}

const char *
purple_buddy_get_name(const PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	return buddy->name;
}

PurpleBuddyIcon *
purple_buddy_get_icon(const PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	return buddy->icon;
}

gpointer
purple_buddy_get_protocol_data(const PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	return buddy->proto_data;
}

void
purple_buddy_set_protocol_data(PurpleBuddy *buddy, gpointer data)
{
	g_return_if_fail(buddy != NULL);

	buddy->proto_data = data;
}

const char *purple_buddy_get_alias_only(PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	if ((buddy->alias != NULL) && (*buddy->alias != '\0')) {
		return buddy->alias;
	} else if ((purple_buddy_get_server_alias(buddy) != NULL) &&
		   (*purple_buddy_get_server_alias(buddy) != '\0')) {

		return purple_buddy_get_server_alias(buddy);
	}

	return NULL;
}

const char *purple_buddy_get_contact_alias(PurpleBuddy *buddy)
{
	PurpleContact *c;

	g_return_val_if_fail(buddy != NULL, NULL);

	/* Search for an alias for the buddy. In order of precedence: */
	/* The buddy alias */
	if (buddy->alias != NULL)
		return buddy->alias;

	/* The contact alias */
	c = purple_buddy_get_contact(buddy);
	if ((c != NULL) && (c->alias != NULL))
		return c->alias;

	/* The server alias */
	if ((purple_buddy_get_server_alias(buddy)) && (*purple_buddy_get_server_alias(buddy)))
		return purple_buddy_get_server_alias(buddy);

	/* The buddy's user name (i.e. no alias) */
	return buddy->name;
}

const char *purple_buddy_get_alias(PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	/* Search for an alias for the buddy. In order of precedence: */
	/* The buddy alias */
	if (buddy->alias != NULL)
		return buddy->alias;

	/* The server alias */
	if ((purple_buddy_get_server_alias(buddy)) && (*purple_buddy_get_server_alias(buddy)))
		return purple_buddy_get_server_alias(buddy);

	/* The buddy's user name (i.e. no alias) */
	return buddy->name;
}

const char *purple_buddy_get_local_buddy_alias(PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy, NULL);
	return buddy->alias;
}

const char *purple_buddy_get_server_alias(PurpleBuddy *buddy)
{
        g_return_val_if_fail(buddy != NULL, NULL);

	if ((buddy->server_alias) && (*buddy->server_alias))
	    return buddy->server_alias;

	return NULL;
}

const char *purple_buddy_get_local_alias(PurpleBuddy *buddy)
{
	PurpleContact *c;

	g_return_val_if_fail(buddy != NULL, NULL);

	/* Search for an alias for the buddy. In order of precedence: */
	/* The buddy alias */
	if (buddy->alias != NULL)
		return buddy->alias;

	/* The contact alias */
	c = purple_buddy_get_contact(buddy);
	if ((c != NULL) && (c->alias != NULL))
		return c->alias;

	/* The buddy's user name (i.e. no alias) */
	return buddy->name;
}

PurpleBuddy *purple_find_buddy_in_group(PurpleAccount *account, const char *name,
		PurpleGroup *group)
{
	struct _purple_hbuddy hb;
	PurpleBuddy *ret;

	g_return_val_if_fail(purplebuddylist != NULL, NULL);
	g_return_val_if_fail(account != NULL, NULL);
	g_return_val_if_fail((name != NULL) && (*name != '\0'), NULL);

	hb.name = g_strdup(purple_normalize(account, name));
	hb.account = account;
	hb.group = (PurpleBlistNode*)group;

	ret = g_hash_table_lookup(purplebuddylist->buddies, &hb);
	g_free(hb.name);

	return ret;
}

PurpleContact *purple_buddy_get_contact(PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	return PURPLE_CONTACT(PURPLE_BLIST_NODE(buddy)->parent);
}

PurplePresence *purple_buddy_get_presence(const PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);
	return buddy->presence;
}

PurpleGroup *purple_buddy_get_group(PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	if (((PurpleBlistNode *)buddy)->parent == NULL)
		return NULL;

	return (PurpleGroup *)(((PurpleBlistNode*)buddy)->parent->parent);
}

xmlnode *
buddy_to_xmlnode(PurpleBlistNode *bnode)
{
	xmlnode *node, *child;
	PurpleBuddy *buddy;

	buddy = (PurpleBuddy *)bnode;

	node = xmlnode_new("buddy");
	xmlnode_set_attrib(node, "account", purple_account_get_username(buddy->account));
	xmlnode_set_attrib(node, "proto", purple_account_get_protocol_id(buddy->account));

	child = xmlnode_new_child(node, "name");
	xmlnode_insert_data(child, buddy->name, -1);

	if (buddy->alias != NULL)
	{
		child = xmlnode_new_child(node, "alias");
		xmlnode_insert_data(child, buddy->alias, -1);
	}

	/* Write buddy settings */
	g_hash_table_foreach(buddy->node.settings, value_to_xmlnode, node);

	return node;
}

/******************/
/*  GObject Code  */
/******************/

/* GObject destructor function */
static void
purple_buddy_finalize(GObject *object)
{
  PurpleBuddy *buddy = PURPLE_BUDDY(object);
	PurplePlugin *prpl;
	PurplePluginProtocolInfo *prpl_info;

	/*
	 * Tell the owner PRPL that we're about to free the buddy so it
	 * can free proto_data
	 */
	prpl = purple_find_prpl(purple_account_get_protocol_id(buddy->account));
	if (prpl) {
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);
		if (prpl_info && prpl_info->buddy_free)
			prpl_info->buddy_free(buddy);
	}

	/* Delete the node */
	purple_buddy_icon_unref(buddy->icon);
	g_hash_table_destroy(buddy->node.settings);
	purple_presence_destroy(buddy->presence);
	g_free(buddy->name);
	g_free(buddy->alias);
	g_free(buddy->server_alias);

	PURPLE_DBUS_UNREGISTER_POINTER(buddy);

  #warning: Uhh, need some explanation here. -Aluink
	/* FIXME: Once PurpleBuddy is a GObject, timeout callbacks can
	 * g_object_ref() it when connecting the callback and
	 * g_object_unref() it in the handler.  That way, it won't
	 * get freed while the timeout is pending and this line can
	 * be removed. */
	while (g_source_remove_by_user_data((gpointer *)buddy));
  parent_class->finalize(object);
}

static void
purple_buddy_class_init(PurpleBuddyClass *klass)
{
  GObjectClass *obj_class = G_OBJECT_CLASS(klass);

  parent_class = g_type_class_peek_parent(klass);
  obj_class->finalize = purple_buddy_finalize;
}

static void
purple_buddy_init(GTypeInstance *instance, gpointer class)
{

}

GType
purple_buddy_get_gtype(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleBuddyClass),
			NULL,					/* base_init		*/
			NULL,					/* base_finalize	*/
			(GClassInitFunc)purple_buddy_class_init,
			NULL,
			NULL,					/* class_data		*/
			sizeof(PurpleBuddy),
			0,						/* n_preallocs		*/
			purple_buddy_init,					/* instance_init	*/
			NULL					/* value_table		*/
		};

		type = g_type_register_static(PURPLE_BLIST_NODE_TYPE,
									  "PurpleBuddy",
									  &info, 0);
	}

	return type;
}

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
#include "dbus-maybe.h"
#include "debug.h"
#include "server.h"
#include "signals.h"
#include "util.h"
#include "xmlnode.h"
#include "buddy.h"

static GObjectClass *parent_class = NULL;

struct _PurpleBuddyPrivate {
	char *name;                             /**< The name of the buddy. */
	char *alias;                            /**< The user-set alias of the buddy */
	char *server_alias;                     /**< The server-specified alias of the buddy.  (i.e. MSN "Friendly Names") */
	void *proto_data;                       /**< This allows the prpl to associate whatever data it wants with a buddy */
	PurpleBuddyIcon *icon;                    /**< The buddy icon. */
	PurpleAccount *account;					/**< the account this buddy belongs to */
	PurplePresence *presence;
};

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
	purple_blist_get_last_child(PURPLE_BLIST_NODE(contact)));

	for (x = xmlnode_get_child(bnode, "setting"); x; x = xmlnode_get_next_twin(x)) {
		parse_setting(PURPLE_BLIST_NODE(buddy), x);
	}

	g_free(name);
	g_free(alias);
}

void
purple_blist_update_buddy_status(PurpleBuddy *buddy, PurpleStatus *old_status)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurplePresence *presence;
	PurpleStatus *status;
	PurpleBuddyPrivate *priv;

	g_return_if_fail(buddy != NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	presence = purple_buddy_get_presence(buddy);
	status = purple_presence_get_active_status(presence);

	purple_debug_info("blist", "Updating buddy status for %s (%s)\n",
			priv->name, purple_account_get_protocol_name(priv->account));

	if (purple_status_is_online(status) &&
		!purple_status_is_online(old_status)) {

		purple_signal_emit(purple_blist_get_handle(), "buddy-signed-on", buddy);
	} else if (!purple_status_is_online(status) &&
				purple_status_is_online(old_status)) {

		purple_blist_node_set_int(&buddy->node, "last_seen", time(NULL));
		purple_signal_emit(purple_blist_get_handle(), "buddy-signed-off", buddy);
	} else {
		purple_signal_emit(purple_blist_get_handle(),
		                 "buddy-status-changed", buddy, old_status,
		                 status);
	}
	purple_contact_buddy_status_update(purple_buddy_get_contact(buddy), status, old_status);

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
		ops->update(PURPLE_BLIST_NODE(buddy));
}

void
purple_blist_update_buddy_icon(PurpleBuddy *buddy)
{
	purple_blist_update_node_icon(PURPLE_BLIST_NODE(buddy));
}

void purple_buddy_set_alias(PurpleBuddy *buddy, const char *alias)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleConversation *conv;
	char *old_alias;
	char *new_alias = NULL;
	PurpleBuddyPrivate *priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	g_return_if_fail(buddy != NULL);

	if ((alias != NULL) && (*alias != '\0'))
		new_alias = purple_utf8_strip_unprintables(alias);

	if (!purple_strings_are_different(priv->alias, new_alias)) {
		g_free(new_alias);
		return;
	}

	old_alias = priv->alias;

	if ((new_alias != NULL) && (*new_alias != '\0'))
		priv->alias = g_strdup(alias);
	else {
		priv->alias = NULL;
		g_free(new_alias); /* could be "\0" */
	}

	if (ops && ops->save_node)
		ops->save_node(PURPLE_BLIST_NODE(buddy));

	if (ops && ops->update)
		ops->update(PURPLE_BLIST_NODE(buddy));

	conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, priv->name,
											   priv->account);
	if (conv)
		purple_conversation_autoset_title(conv);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-aliased",
					 buddy, old_alias);
	g_free(old_alias);
}

void purple_buddy_set_name(PurpleBuddy *buddy, const char *name)
{
	PurpleBuddyPrivate *priv = PURPLE_BUDDY_GET_PRIVATE(buddy);
	g_return_if_fail(buddy);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	if(priv->name)
		g_free(priv->name);

	priv->name = g_strdup(name);

}

void purple_blist_server_alias_buddy(PurpleBuddy *buddy, const char *alias)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleConversation *conv;
	char *old_alias;
	char *new_alias = NULL;
	PurpleBuddyPrivate *priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	g_return_if_fail(buddy != NULL);

	if ((alias != NULL) && (*alias != '\0') && g_utf8_validate(alias, -1, NULL))
		new_alias = purple_utf8_strip_unprintables(alias);

	if (!purple_strings_are_different(priv->server_alias, new_alias)) {
		g_free(new_alias);
		return;
	}

	old_alias = priv->server_alias;

	if ((new_alias != NULL) && (*new_alias != '\0'))
		priv->server_alias = new_alias;
	else {
		priv->server_alias = NULL;
		g_free(new_alias); /* could be "\0"; */
	}

	if (ops && ops->save_node)
		ops->save_node(PURPLE_BLIST_NODE(buddy));

	if (ops && ops->update)
		ops->update(PURPLE_BLIST_NODE(buddy));

	conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, priv->name,
											   priv->account);
	if (conv)
		purple_conversation_autoset_title(conv);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-aliased",
					 buddy, old_alias);
	g_free(old_alias);
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
	PurpleBuddyPrivate *priv;
	g_return_if_fail(buddy != NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	if (priv->icon != icon)
	{
		purple_buddy_icon_unref(priv->icon);
		priv->icon = (icon != NULL ? purple_buddy_icon_ref(icon) : NULL);
	}

	purple_signal_emit(purple_blist_get_handle(), "buddy-icon-changed", buddy);

	purple_blist_update_node_icon(PURPLE_BLIST_NODE(buddy));
}

PurpleAccount *
purple_buddy_get_account(const PurpleBuddy *buddy)
{
	PurpleBuddyPrivate *priv;
	g_return_val_if_fail(buddy != NULL, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	return priv->account;
}

const char *
purple_buddy_get_name(const PurpleBuddy *buddy)
{
	PurpleBuddyPrivate *priv;
	g_return_val_if_fail(buddy != NULL, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	return priv->name;
}

PurpleBuddyIcon *
purple_buddy_get_icon(const PurpleBuddy *buddy)
{
	PurpleBuddyPrivate *priv;
	g_return_val_if_fail(buddy != NULL, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	return priv->icon;
}

gpointer
purple_buddy_get_protocol_data(const PurpleBuddy *buddy)
{
	PurpleBuddyPrivate *priv;
	g_return_val_if_fail(buddy != NULL, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	return priv->proto_data;
}

void
purple_buddy_set_protocol_data(PurpleBuddy *buddy, gpointer data)
{
	PurpleBuddyPrivate *priv;
	g_return_if_fail(buddy != NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	priv->proto_data = data;
}

const char *purple_buddy_get_alias_only(PurpleBuddy *buddy)
{
	PurpleBuddyPrivate *priv;
	g_return_val_if_fail(buddy != NULL, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	if ((priv->alias != NULL) && (*priv->alias != '\0')) {
		return priv->alias;
	} else if ((purple_buddy_get_server_alias(buddy) != NULL) &&
		   (*purple_buddy_get_server_alias(buddy) != '\0')) {

		return purple_buddy_get_server_alias(buddy);
	}

	return NULL;
}

const char *purple_buddy_get_contact_alias(PurpleBuddy *buddy)
{
	PurpleContact *c;
	PurpleBuddyPrivate *priv;

	g_return_val_if_fail(buddy != NULL, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	/* Search for an alias for the buddy. In order of precedence: */
	/* The buddy alias */
	if (priv->alias != NULL)
		return priv->alias;

	/* The contact alias */
	c = purple_buddy_get_contact(buddy);
	if ((c != NULL) && (purple_contact_get_alias(c)	!= NULL))
		return purple_contact_get_alias(c);

	/* The server alias */
	if ((purple_buddy_get_server_alias(buddy)) && (*purple_buddy_get_server_alias(buddy)))
		return purple_buddy_get_server_alias(buddy);

	/* The buddy's user name (i.e. no alias) */
	return priv->name;
}

const char *purple_buddy_get_alias(PurpleBuddy *buddy)
{
	PurpleBuddyPrivate *priv;
	g_return_val_if_fail(buddy != NULL, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	/* Search for an alias for the buddy. In order of precedence: */
	/* The buddy alias */
	if (priv->alias != NULL)
		return priv->alias;

	/* The server alias */
	if ((purple_buddy_get_server_alias(buddy)) && (*purple_buddy_get_server_alias(buddy)))
		return purple_buddy_get_server_alias(buddy);

	/* The buddy's user name (i.e. no alias) */
	return priv->name;
}

const char *purple_buddy_get_local_buddy_alias(PurpleBuddy *buddy)
{
	PurpleBuddyPrivate *priv;
	g_return_val_if_fail(buddy, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);
	return priv->alias;
}

const char *purple_buddy_get_server_alias(PurpleBuddy *buddy)
{
	PurpleBuddyPrivate *priv;
	g_return_val_if_fail(buddy != NULL, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	if ((priv->server_alias) && (*priv->server_alias))
	    return priv->server_alias;

	return NULL;
}

const char *purple_buddy_get_local_alias(PurpleBuddy *buddy)
{
	PurpleContact *c;
	PurpleBuddyPrivate *priv;

	g_return_val_if_fail(buddy != NULL, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	/* Search for an alias for the buddy. In order of precedence: */
	/* The buddy alias */
	if (priv->alias != NULL)
		return priv->alias;

	/* The contact alias */
	c = purple_buddy_get_contact(buddy);
	if ((c != NULL) && (purple_contact_get_alias(c) != NULL))
		return purple_contact_get_alias(c);

	/* The buddy's user name (i.e. no alias) */
	return priv->name;
}

PurplePresence *purple_buddy_get_presence(const PurpleBuddy *buddy)
{
	PurpleBuddyPrivate *priv;
	g_return_val_if_fail(buddy != NULL, NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);
	return priv->presence;
}

xmlnode *
buddy_to_xmlnode(PurpleBlistNode *bnode)
{
	xmlnode *node, *child;
	PurpleBuddy *buddy;
	PurpleBuddyPrivate *priv;

	buddy = (PurpleBuddy *)bnode;
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	node = xmlnode_new("buddy");
	xmlnode_set_attrib(node, "account", purple_account_get_username(priv->account));
	xmlnode_set_attrib(node, "proto", purple_account_get_protocol_id(priv->account));

	child = xmlnode_new_child(node, "name");
	xmlnode_insert_data(child, priv->name, -1);

	if (priv->alias != NULL)
	{
		child = xmlnode_new_child(node, "alias");
		xmlnode_insert_data(child, priv->alias, -1);
	}

	/* Write buddy settings */
	g_hash_table_foreach(purple_blist_node_get_settings(PURPLE_BLIST_NODE(buddy)), value_to_xmlnode, node);

	return node;
}

static void
purple_buddy_set_account(PurpleBuddy *buddy, PurpleAccount *account)
{
	PurpleBuddyPrivate *priv;
	g_return_if_fail(buddy != NULL);
	priv = PURPLE_BUDDY_GET_PRIVATE(buddy);
	priv->account = account;
}

/******************/
/*  GObject Code  */
/******************/

enum {
	PROP_0,
	PROP_ACCOUNT,
	PROP_NAME,
	PROP_ALIAS,
	PROP_LAST
};

#define PROP_ACCOUNT_S "account"
#define PROP_NAME_S "name"
#define PROP_ALIAS_S "alias"

PurpleBuddy *purple_buddy_new(PurpleAccount *account, const char *name, const char *alias)
{
	g_return_val_if_fail(account != NULL, NULL);
	g_return_val_if_fail(name != NULL, NULL);

	return g_object_new(PURPLE_BUDDY_TYPE, 
									PROP_ACCOUNT_S, account,
									PROP_NAME_S, name,
									PROP_ALIAS_S, alias,
									NULL);
}

/* GObject destructor function */
static void
purple_buddy_finalize(GObject *object)
{
	PurpleBuddy *buddy = PURPLE_BUDDY(object);
	PurpleBuddyPrivate *priv = PURPLE_BUDDY_GET_PRIVATE(buddy);
	PurplePlugin *prpl;
	PurplePluginProtocolInfo *prpl_info;

	/*
	 * Tell the owner PRPL that we're about to free the buddy so it
	 * can free proto_data
	 */
	prpl = purple_find_prpl(purple_account_get_protocol_id(priv->account));
	if (prpl) {
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);
		if (prpl_info && prpl_info->buddy_free)
			prpl_info->buddy_free(buddy);
	}

	/* Delete the node */
	purple_buddy_icon_unref(priv->icon);
	purple_presence_destroy(priv->presence);
	g_free(priv->name);
	g_free(priv->alias);
	g_free(priv->server_alias);

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

PurpleGroup *
purple_buddy_get_group(PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	return PURPLE_GROUP(purple_blist_node_find_container(PURPLE_BLIST_NODE(buddy), PURPLE_GROUP_TYPE));

}

static void
purple_buddy_set_property(GObject *obj, guint param_id, const GValue *value,
		GParamSpec *pspec)
{
	PurpleBuddy *buddy = PURPLE_BUDDY(obj);
	switch(param_id){
		case PROP_ACCOUNT:
			purple_buddy_set_account(buddy, g_value_get_object(value));
			break;
		case PROP_NAME:
			purple_buddy_set_name(buddy, g_value_get_string(value));
			break;
		case PROP_ALIAS:
			purple_buddy_set_alias(buddy, g_value_get_string(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
	}
}

static void
purple_buddy_get_property(GObject *obj, guint param_id, GValue *value,
		GParamSpec *pspec)
{
	PurpleBuddy *buddy = PURPLE_BUDDY(obj);
	switch(param_id){
		case PROP_ACCOUNT:
			g_value_set_object(value, purple_buddy_get_account(buddy));
			break;
		case PROP_NAME:
			g_value_set_string(value, purple_buddy_get_name(buddy));
			break;
		case PROP_ALIAS:
			g_value_set_string(value, purple_buddy_get_name(buddy));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_buddy_class_init(PurpleBuddyClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);
	obj_class->finalize = purple_buddy_finalize;

	obj_class->set_property = purple_buddy_set_property;
	obj_class->get_property = purple_buddy_get_property;

	g_type_class_add_private(klass, sizeof(PurpleBuddyPrivate));

	g_object_class_install_property(obj_class, PROP_ACCOUNT,
			g_param_spec_string(PROP_ACCOUNT_S, _("Account"),
				_("The account for the buddy."), NULL,
				G_PARAM_CONSTRUCT_ONLY)
			);
	g_object_class_install_property(obj_class, PROP_NAME,
			g_param_spec_string(PROP_NAME_S, _("Name"),
				_("The name for the buddy."), NULL,
				G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY)
			);
	g_object_class_install_property(obj_class, PROP_ALIAS,
			g_param_spec_string(PROP_ALIAS_S, _("Alias"),
				_("The alias for the buddy."), NULL,
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT)
			);
}

static void
purple_buddy_init(GTypeInstance *instance, gpointer class)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleBuddy *buddy = PURPLE_BUDDY(instance);
	PurpleBuddyPrivate *priv = PURPLE_BUDDY_GET_PRIVATE(buddy);

	priv->presence = purple_presence_new_for_buddy(buddy);

	purple_presence_set_status_active(priv->presence, "offline", TRUE);

	if (ops && ops->new_node)
		ops->new_node(PURPLE_BLIST_NODE(buddy));

	PURPLE_DBUS_REGISTER_POINTER(buddy, PurpleBuddy);
}

GType
purple_buddy_get_type(void)
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

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
#define _PURPLE_CHAT_C
#define _BLIST_HELPERS_

#include "internal.h"
#include "chat.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "server.h"
#include "signals.h"
#include "xmlnode.h"

static void
chat_component_to_xmlnode(gpointer key, gpointer value, gpointer user_data)
{
	const char *name;
	const char *data;
	xmlnode *node, *child;

	name = (const char *)key;
	data = (const char *)value;
	node = (xmlnode *)user_data;

	g_return_if_fail(data != NULL);

	child = xmlnode_new_child(node, "component");
	xmlnode_set_attrib(child, "name", name);
	xmlnode_insert_data(child, data, -1);
}

xmlnode *
chat_to_xmlnode(PurpleBlistNode *cnode)
{
	xmlnode *node, *child;
	PurpleChat *chat;

	chat = (PurpleChat *)cnode;

	node = xmlnode_new("chat");
	xmlnode_set_attrib(node, "proto", purple_account_get_protocol_id(chat->account));
	xmlnode_set_attrib(node, "account", purple_account_get_username(chat->account));

	if (chat->alias != NULL)
	{
		child = xmlnode_new_child(node, "alias");
		xmlnode_insert_data(child, chat->alias, -1);
	}

	/* Write chat components */
	g_hash_table_foreach(chat->components, chat_component_to_xmlnode, node);

	/* Write chat settings */
	g_hash_table_foreach(purple_blist_node_get_settings(PURPLE_BLIST_NODE(chat)), value_to_xmlnode, node);

	return node;
}

void
parse_chat(PurpleGroup *group, xmlnode *cnode)
{
	PurpleChat *chat;
	PurpleAccount *account;
	const char *acct_name, *proto, *protocol;
	xmlnode *x;
	char *alias = NULL;
	GHashTable *components;

	acct_name = xmlnode_get_attrib(cnode, "account");
	protocol = xmlnode_get_attrib(cnode, "protocol");
	proto = xmlnode_get_attrib(cnode, "proto");

	if (!acct_name || (!proto && !protocol))
		return;

	account = purple_accounts_find(acct_name, proto ? proto : protocol);

	if (!account)
		return;

	if ((x = xmlnode_get_child(cnode, "alias")))
		alias = xmlnode_get_data(x);

	components = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	for (x = xmlnode_get_child(cnode, "component"); x; x = xmlnode_get_next_twin(x)) {
		const char *name;
		char *value;

		name = xmlnode_get_attrib(x, "name");
		value = xmlnode_get_data(x);
		g_hash_table_replace(components, g_strdup(name), value);
	}

	chat = purple_chat_new(account, alias, components);
	purple_blist_add_chat(chat, group,
			purple_blist_get_last_child((PurpleBlistNode*)group));

	for (x = xmlnode_get_child(cnode, "setting"); x; x = xmlnode_get_next_twin(x)) {
		parse_setting((PurpleBlistNode*)chat, x);
	}

	g_free(alias);
}

void purple_chat_set_alias(PurpleChat *chat, const char *alias)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	char *old_alias;
	char *new_alias = NULL;

	g_return_if_fail(chat != NULL);

	if ((alias != NULL) && (*alias != '\0'))
		new_alias = purple_utf8_strip_unprintables(alias);

	if (!purple_strings_are_different(chat->alias, new_alias)) {
		g_free(new_alias);
		return;
	}

	old_alias = chat->alias;

	if ((new_alias != NULL) && (*new_alias != '\0'))
		chat->alias = new_alias;
	else {
		chat->alias = NULL;
		g_free(new_alias); /* could be "\0" */
	}

	purple_blist_schedule_save();

	if (ops && ops->update)
		ops->update(PURPLE_BLIST_NODE(chat));

	purple_signal_emit(purple_blist_get_handle(), "blist-node-aliased",
					 chat, old_alias);
	g_free(old_alias);
}

void
purple_chat_destroy(PurpleChat *chat)
{
	/* This function is only a hack for api breakage */
	g_return_if_fail(PURPLE_IS_CHAT(chat));
	g_object_unref(G_OBJECT(chat));
}

const char *purple_chat_get_name(PurpleChat *chat)
{
	char *ret = NULL;
	PurplePlugin *prpl;
	PurplePluginProtocolInfo *prpl_info = NULL;

	g_return_val_if_fail(chat != NULL, NULL);

	if ((chat->alias != NULL) && (*chat->alias != '\0'))
		return chat->alias;

	prpl = purple_find_prpl(purple_account_get_protocol_id(chat->account));
	prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);

	if (prpl_info->chat_info) {
		struct proto_chat_entry *pce;
		GList *parts = prpl_info->chat_info(purple_account_get_connection(chat->account));
		pce = parts->data;
		ret = g_hash_table_lookup(chat->components, pce->identifier);
		g_list_foreach(parts, (GFunc)g_free, NULL);
		g_list_free(parts);
	}

	return ret;
}

PurpleAccount *
purple_chat_get_account(PurpleChat *chat)
{
	g_return_val_if_fail(chat != NULL, NULL);

	return chat->account;
}

static void
purple_chat_set_account(PurpleChat *chat, PurpleAccount *account)
{
	g_return_if_fail(chat != NULL);

	chat->account = account;
}

GHashTable *
purple_chat_get_components(PurpleChat *chat)
{
	g_return_val_if_fail(chat != NULL, NULL);

	return chat->components;
}

static void
purple_chat_set_components(PurpleChat *chat, GHashTable *components)
{
	g_return_if_fail(chat != NULL);

	chat->components = components;
}

/******************/
/*  GObject Code  */
/******************/

enum {
	PROP_0,
	PROP_ALIAS,
	PROP_ACCOUNT,
	PROP_COMPONENTS,
	PROP_LAST
};

#define PROP_ALIAS_S "alias"
#define PROP_ACCOUNT_S "account"
#define PROP_COMPONENTS_S "components"

static GObjectClass *parent_class = NULL;

PurpleChat *purple_chat_new(PurpleAccount *account, const char *alias, GHashTable *components)
{
	PurpleChat *chat;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(components != NULL, FALSE);

	chat = g_object_new(PURPLE_CHAT_TYPE, 
									PROP_ACCOUNT_S, account, 
									PROP_ALIAS_S, alias, 
									PROP_COMPONENTS_S, components,
									NULL);
	return chat;
}

static void
purple_chat_finalize(GObject *object)
{
	PurpleChat *chat = PURPLE_CHAT(object);
	g_hash_table_destroy(chat->components);
	g_free(chat->alias);
	PURPLE_DBUS_UNREGISTER_POINTER(chat);
	parent_class->finalize(object);
}

static void
purple_chat_set_property(GObject *obj, guint param_id, const GValue *value,
		GParamSpec *pspec)
{
	PurpleChat *chat = PURPLE_CHAT(obj);
	switch(param_id){
		case PROP_ALIAS:
			purple_chat_set_alias(chat, g_value_get_string(value));
			break;
		case PROP_ACCOUNT:
			purple_chat_set_account(chat, g_value_get_object(value));
			break;
		case PROP_COMPONENTS:
			purple_chat_set_components(chat, g_value_get_object(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
	}
}

static void
purple_chat_get_property(GObject *obj, guint param_id, GValue *value,
		GParamSpec *pspec)
{
	PurpleChat *chat = PURPLE_CHAT(obj);
	switch(param_id){
		case PROP_ALIAS:
			g_value_set_string(value, purple_chat_get_name(chat));
			break;
		case PROP_COMPONENTS:
			g_value_set_object(value, purple_chat_get_components(chat));
			break;
		case PROP_ACCOUNT:
			g_value_set_object(value, purple_chat_get_account(chat));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_chat_class_init(PurpleChatClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);
	obj_class->finalize = purple_chat_finalize;

	/* Setup properties */
	obj_class->get_property = purple_chat_get_property;
	obj_class->set_property = purple_chat_set_property;

	g_object_class_install_property(obj_class, PROP_ALIAS,
			g_param_spec_string(PROP_ALIAS_S, _("Alias"),
				_("The alias for the chat."), NULL,
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT)
			);

	g_object_class_install_property(obj_class, PROP_ACCOUNT,
			g_param_spec_string(PROP_ACCOUNT_S, _("Account"),
				_("The account for the chat."), NULL,
				G_PARAM_CONSTRUCT_ONLY)
			);

	g_object_class_install_property(obj_class, PROP_COMPONENTS,
			g_param_spec_string(PROP_COMPONENTS_S, _("Components"),
				_("The components for the chat."), NULL,
				G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY)
			);
}

static void
purple_chat_init(GTypeInstance *instance, gpointer class)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleChat *chat = PURPLE_CHAT(instance);

	if (ops != NULL && ops->new_node != NULL)
		ops->new_node((PurpleBlistNode *)chat);

	PURPLE_DBUS_REGISTER_POINTER(chat, PurpleChat);
}

GType
purple_chat_get_type(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleChatClass),
			NULL,					/* base_init		*/
			NULL,					/* base_finalize	*/
			(GClassInitFunc)purple_chat_class_init,
			NULL,
			NULL,					/* class_data		*/
			sizeof(PurpleChat),
			0,						/* n_preallocs		*/
			purple_chat_init,					/* instance_init	*/
			NULL					/* value_table		*/
		};

		type = g_type_register_static(PURPLE_BLIST_NODE_TYPE,
										"PurpleChat",
										&info, 0);
	}

	return type;
}

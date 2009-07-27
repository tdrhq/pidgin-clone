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
#define _PURPLE_GROUP_C_
#define _BLIST_HELPERS_

#include "internal.h"
#include "group.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "server.h"
#include "signals.h"
#include "xmlnode.h"

static PurpleBlistNodeClass *parent_class = NULL;

xmlnode *
group_to_xmlnode(PurpleBlistNode *gnode)
{
	xmlnode *node, *child;
	PurpleGroup *group;
	PurpleBlistNode *cnode;

	group = PURPLE_GROUP(gnode);

	node = xmlnode_new("group");
	xmlnode_set_attrib(node, "name", group->name);

	/* Write settings */
	g_hash_table_foreach(purple_blist_node_get_settings(PURPLE_BLIST_NODE(group)), value_to_xmlnode, node);

	/* Write contacts and chats */
	for (cnode = gnode->child; cnode != NULL; cnode = cnode->next)
	{
		if (!PURPLE_BLIST_NODE_SHOULD_SAVE(cnode))
			continue;
		if (PURPLE_IS_CONTACT(cnode))
		{
			child = contact_to_xmlnode(cnode);
			xmlnode_insert_child(node, child);
		}
		else if (PURPLE_IS_CHAT(cnode))
		{
			child = chat_to_xmlnode(cnode);
			xmlnode_insert_child(node, child);
		}
	}

	return node;
}

GSList *purple_group_get_accounts(PurpleGroup *group)
{
	GSList *l = NULL;
	PurpleBlistNode *gnode, *cnode, *bnode;

	gnode = PURPLE_BLIST_NODE(group);

	for (cnode = gnode->child;  cnode; cnode = cnode->next) {
		if (PURPLE_IS_CHAT(cnode)) {
			if (!g_slist_find(l, ((PurpleChat *)cnode)->account))
				l = g_slist_append(l, ((PurpleChat *)cnode)->account);
		} else if (PURPLE_IS_CONTACT(cnode)) {
			for (bnode = cnode->child; bnode; bnode = bnode->next) {
				if (PURPLE_IS_BUDDY(bnode)) {
					if (!g_slist_find(l, purple_buddy_get_account(PURPLE_BUDDY(bnode))))
						l = g_slist_append(l, purple_buddy_get_account(PURPLE_BUDDY(bnode)));
				}
			}
		}
	}

	return l;
}

static void
purple_group_add_update(PurpleGroup *group, PurpleBlistNode *child)
{
	PurpleChat *chat;
	PurpleContact *contact;

	if(PURPLE_IS_CHAT(child)){
		chat = PURPLE_CHAT(child);
		if (purple_account_is_connected(chat->account)) {
			group->online++;
			group->currentsize++;
		}
	} else if(PURPLE_IS_CONTACT(child)){
		contact = PURPLE_CONTACT(child);
		if(purple_contact_get_online(contact) > 0)
			group->online++;
		if(purple_contact_get_currentsize(contact) > 0)
			group->currentsize++;
	} else {
		#warning: is this an ok case?
		g_warn_if_reached();
	}
	group->totalsize++;

	purple_blist_schedule_save();
	purple_signal_emit(purple_blist_node_get_handle(), "node-added", child);

}

static void
purple_group_add_sibling(PurpleBlistNode *node, PurpleBlistNode *location)
{
	g_return_if_fail(node);
	g_return_if_fail(PURPLE_IS_GROUP(purple_blist_node_parent(location)));

	parent_class->add_sibling(node, location);
	purple_group_add_update(PURPLE_GROUP(purple_blist_node_parent(location)), node);
}

static void
purple_group_add_child(PurpleBlistNode *parent, PurpleBlistNode *child)
{
	g_return_if_fail(parent);
	g_return_if_fail(child);

	parent_class->add_child(parent, child);
	purple_group_add_update(PURPLE_GROUP(parent), child);
}

static void
purple_group_remove_node(PurpleBlistNode *child)
{
	PurpleGroup *group;
	PurpleContact *contact;
	PurpleChat *chat;

	g_return_if_fail(child);
	g_return_if_fail(purple_blist_node_parent(child));
	group = PURPLE_GROUP(child->parent);

	if(PURPLE_IS_CHAT(child)){
		chat = PURPLE_CHAT(child);
		if (purple_account_is_connected(chat->account)) {
			group->online--;
			group->currentsize--;
		}
	} else if(PURPLE_IS_CONTACT(child)){
		contact = PURPLE_CONTACT(child);
		if(purple_contact_get_online(contact) > 0)
			group->online--;
		if(purple_contact_get_currentsize(contact) > 0)
			group->currentsize--;
	} else {
		#warning: Is this an ok case?
		g_warn_if_reached();
	}
	group->totalsize--;

	parent_class->remove(child);

	purple_blist_schedule_save();

	purple_signal_emit(purple_blist_node_get_handle(), "node-removed", child);
}

void
purple_group_destroy(PurpleGroup *group)
{
	/* This function is only a hack for api breakage */
	g_return_if_fail(PURPLE_IS_GROUP(group));
	g_object_unref(G_OBJECT(group));
}

const char *purple_group_get_name(PurpleGroup *group)
{
	g_return_val_if_fail(group != NULL, NULL);

	return group->name;
}

int purple_blist_get_group_size(PurpleGroup *group, gboolean offline)
{
	if (!group)
		return 0;

	return offline ? group->totalsize : group->currentsize;
}

int purple_blist_get_group_online_count(PurpleGroup *group)
{
	if (!group)
		return 0;

	return group->online;
}

GList *purple_group_get_buddies(PurpleGroup *group)
{
	GList *buddies = NULL;
	PurpleBlistNode *gnode;
	PurpleBlistNode *itr;

	g_return_val_if_fail(group,NULL);

	gnode = PURPLE_BLIST_NODE(group);
	for(itr = gnode->child;itr;itr = itr->next){
		if(PURPLE_IS_CONTACT(itr)){
			buddies = g_list_concat(buddies, purple_contact_get_buddies(PURPLE_CONTACT(itr)));
		}
	}

	return buddies;
}

void
purple_group_contact_updated(PurpleGroup *group, PurpleContact *contact)
{
	g_return_if_fail(group);
	g_return_if_fail(contact);

	if (purple_contact_get_online(contact) > 0)
		group->online++;
	else
		group->online--;

	if (purple_contact_get_currentsize(contact) > 0)
		group->currentsize++;
	else
		group->currentsize--;
}

static void
purple_group_add_contact(PurpleGroup *group, PurpleContact *contact, PurpleBlistNode *node)
{
	g_return_if_fail(group);
	g_return_if_fail(contact);

	if (node && (PURPLE_IS_CONTACT(node) ||
				PURPLE_IS_CHAT(node))) {
		purple_blist_node_add_sibling(PURPLE_BLIST_NODE(contact), node);
	} else {
		purple_blist_node_add_child(PURPLE_BLIST_NODE(group), PURPLE_BLIST_NODE(contact));
	}	
	purple_group_contact_updated(group, contact);
	group->totalsize++;
}

static void
purple_group_set_name(PurpleGroup *group, const char *name)
{
	g_return_if_fail(group != NULL);
	group->name = purple_utf8_strip_unprintables(name);
}

/******************/
/*  GObject Code  */
/******************/

enum {
	PROP_0,
	PROP_NAME,
	PROP_LAST
};

#define PROP_NAME_S ("name")

PurpleGroup *purple_group_new(const char *name)
{
	PurpleGroup *group;

	g_return_val_if_fail(name  != NULL, NULL);
	g_return_val_if_fail(*name != '\0', NULL);

	group = purple_find_group(name);
	if (group != NULL)
		 return group;

	return g_object_new(	PURPLE_GROUP_TYPE, 
												PROP_NAME_S, name, 
												NULL);
}


static void
purple_group_finalize(GObject *object)
{
	PurpleGroup *group = PURPLE_GROUP(object);
	g_free(group->name);
	PURPLE_DBUS_UNREGISTER_POINTER(group);
	G_OBJECT_CLASS(parent_class)->finalize(object);
}

static void
purple_group_set_property(GObject *obj, guint param_id, const GValue *value,
		GParamSpec *pspec)
{
	PurpleGroup *group = PURPLE_GROUP(obj);
	switch(param_id){
		case PROP_NAME:
			purple_group_set_name(group, g_value_get_string(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
	}
}

static void
purple_group_get_property(GObject *obj, guint param_id, GValue *value,
		GParamSpec *pspec)
{
	PurpleGroup *group = PURPLE_GROUP(obj);
	switch(param_id){
		case PROP_NAME:
			g_value_set_string(value, purple_group_get_name(group));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
	}
}

static void
purple_group_class_init(PurpleGroupClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = PURPLE_BLIST_NODE_CLASS(klass);	
	parent_class->add_sibling = purple_group_add_sibling;
	parent_class->add_child = purple_group_add_child;
	parent_class->remove = purple_group_remove_node;

	parent_class = g_type_class_peek_parent(klass);

	obj_class->finalize = purple_group_finalize;
	obj_class->set_property = purple_group_set_property;
	obj_class->get_property = purple_group_get_property;

	g_object_class_install_property(obj_class, PROP_NAME,
			g_param_spec_string(PROP_NAME_S, _("Name"),
				_("The name for the group."), NULL,
				G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY)
			);
}

static void
purple_group_init(GTypeInstance *instance, gpointer class)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleGroup *group = PURPLE_GROUP(instance);

	group->totalsize = 0;
	group->currentsize = 0;
	group->online = 0;

	if (ops && ops->new_node)
		ops->new_node(PURPLE_BLIST_NODE(group));

	PURPLE_DBUS_REGISTER_POINTER(group, PurpleGroup);
}

GType
purple_group_get_type(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleGroupClass),
			NULL,					/* base_init		*/
			NULL,					/* base_finalize	*/
			(GClassInitFunc)purple_group_class_init,
			NULL,
			NULL,					/* class_data		*/
			sizeof(PurpleGroup),
			0,						/* n_preallocs		*/
			purple_group_init,					/* instance_init	*/
			NULL					/* value_table		*/
		};

		type = g_type_register_static(PURPLE_BLIST_NODE_TYPE,
									  "PurpleGroup",
									  &info, 0);
	}

	return type;
}

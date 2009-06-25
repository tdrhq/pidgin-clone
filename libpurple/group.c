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

xmlnode *
group_to_xmlnode(PurpleBlistNode *gnode)
{
	xmlnode *node, *child;
	PurpleGroup *group;
	PurpleBlistNode *cnode;

	group = (PurpleGroup *)gnode;

	node = xmlnode_new("group");
	xmlnode_set_attrib(node, "name", group->name);

	/* Write settings */
	g_hash_table_foreach(group->node.settings, value_to_xmlnode, node);

	/* Write contacts and chats */
	for (cnode = gnode->child; cnode != NULL; cnode = cnode->next)
	{
		if (!PURPLE_BLIST_NODE_SHOULD_SAVE(cnode))
			continue;
		if (PURPLE_BLIST_NODE_IS_CONTACT(cnode))
		{
			child = contact_to_xmlnode(cnode);
			xmlnode_insert_child(node, child);
		}
		else if (PURPLE_BLIST_NODE_IS_CHAT(cnode))
		{
			child = chat_to_xmlnode(cnode);
			xmlnode_insert_child(node, child);
		}
	}

	return node;
}

/*
 * TODO: If merging, prompt the user if they want to merge.
 */
void purple_blist_rename_group(PurpleGroup *source, const char *name)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleGroup *dest;
	gchar *old_name;
	gchar *new_name;
	GList *moved_buddies = NULL;
	GSList *accts;

	g_return_if_fail(source != NULL);
	g_return_if_fail(name != NULL);

	new_name = purple_utf8_strip_unprintables(name);

	if (*new_name == '\0' || purple_strequal(new_name, source->name)) {
		g_free(new_name);
		return;
	}

	dest = purple_find_group(new_name);
	if (dest != NULL && purple_utf8_strcasecmp(source->name, dest->name) != 0) {
		/* We're merging two groups */
		PurpleBlistNode *prev, *child, *next;

		prev = purple_blist_get_last_child((PurpleBlistNode*)dest);
		child = ((PurpleBlistNode*)source)->child;

		/*
		 * TODO: This seems like a dumb way to do this... why not just
		 * append all children from the old group to the end of the new
		 * one?  PRPLs might be expecting to receive an add_buddy() for
		 * each moved buddy...
		 */
		while (child)
		{
			next = child->next;
			if (PURPLE_BLIST_NODE_IS_CONTACT(child)) {
				PurpleBlistNode *bnode;
				purple_blist_add_contact((PurpleContact *)child, dest, prev);
				for (bnode = child->child; bnode != NULL; bnode = bnode->next) {
					purple_blist_add_buddy((PurpleBuddy *)bnode, (PurpleContact *)child,
							NULL, bnode->prev);
					moved_buddies = g_list_append(moved_buddies, bnode);
				}
				prev = child;
			} else if (PURPLE_BLIST_NODE_IS_CHAT(child)) {
				purple_blist_add_chat((PurpleChat *)child, dest, prev);
				prev = child;
			} else {
				purple_debug(PURPLE_DEBUG_ERROR, "blist",
						"Unknown child type in group %s\n", source->name);
			}
			child = next;
		}

		/* Make a copy of the old group name and then delete the old group */
		old_name = g_strdup(source->name);
		purple_blist_remove_group(source);
		source = dest;
		g_free(new_name);
	} else {
		/* A simple rename */
		PurpleBlistNode *cnode, *bnode;

		/* Build a GList of all buddies in this group */
		for (cnode = ((PurpleBlistNode *)source)->child; cnode != NULL; cnode = cnode->next) {
			if (PURPLE_BLIST_NODE_IS_CONTACT(cnode))
				for (bnode = cnode->child; bnode != NULL; bnode = bnode->next)
					moved_buddies = g_list_append(moved_buddies, bnode);
		}

		old_name = source->name;
		source->name = new_name;
	}

	/* Save our changes */
	purple_blist_schedule_save();

	/* Update the UI */
	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode*)source);

	/* Notify all PRPLs */
	/* TODO: Is this condition needed?  Seems like it would always be TRUE */
	if(old_name && !purple_strequal(source->name, old_name)) {
		for (accts = purple_group_get_accounts(source); accts; accts = g_slist_remove(accts, accts->data)) {
			PurpleAccount *account = accts->data;
			PurpleConnection *gc = NULL;
			PurplePlugin *prpl = NULL;
			PurplePluginProtocolInfo *prpl_info = NULL;
			GList *l = NULL, *buddies = NULL;

			gc = purple_account_get_connection(account);

			if(gc)
				prpl = purple_connection_get_prpl(gc);

			if(gc && prpl)
				prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);

			if(!prpl_info)
				continue;

			for(l = moved_buddies; l; l = l->next) {
				PurpleBuddy *buddy = (PurpleBuddy *)l->data;

				if(buddy && buddy->account == account)
					buddies = g_list_append(buddies, (PurpleBlistNode *)buddy);
			}

			if(prpl_info->rename_group) {
				prpl_info->rename_group(gc, old_name, source, buddies);
			} else {
				GList *cur, *groups = NULL;

				/* Make a list of what the groups each buddy is in */
				for(cur = buddies; cur; cur = cur->next) {
					PurpleBlistNode *node = (PurpleBlistNode *)cur->data;
					groups = g_list_prepend(groups, node->parent->parent);
				}

				purple_account_remove_buddies(account, buddies, groups);
				g_list_free(groups);
				purple_account_add_buddies(account, buddies);
			}

			g_list_free(buddies);
		}
	}
	g_list_free(moved_buddies);
	g_free(old_name);
}

PurpleGroup *purple_buddy_get_group(PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	if (((PurpleBlistNode *)buddy)->parent == NULL)
		return NULL;

	return (PurpleGroup *)(((PurpleBlistNode*)buddy)->parent->parent);
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

PurpleGroup *purple_group_new(const char *name)
{
  PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
  PurpleGroup *group;

  g_return_val_if_fail(name  != NULL, NULL);
  g_return_val_if_fail(*name != '\0', NULL);

  group = purple_find_group(name);
  if (group != NULL)
    return group;

  group = g_object_new(PURPLE_GROUP_TYPE, NULL);
  group->name = purple_utf8_strip_unprintables(name);
  group->totalsize = 0;
  group->currentsize = 0;
  group->online = 0;
  purple_blist_node_initialize_settings((PurpleBlistNode *)group);
  ((PurpleBlistNode *)group)->type = PURPLE_BLIST_GROUP_NODE;

  if (ops && ops->new_node)
    ops->new_node((PurpleBlistNode *)group);

  PURPLE_DBUS_REGISTER_POINTER(group, PurpleGroup);
  return group;
}


/******************/
/*  GObject Code  */
/******************/

static GObjectClass *parent_class = NULL;

static void
purple_group_finalize(GObject *object)
{
  PurpleGroup *group = PURPLE_GROUP(object);
	g_hash_table_destroy(group->node.settings);
	g_free(group->name);
	PURPLE_DBUS_UNREGISTER_POINTER(group);
	parent_class->finalize(object);
}

static void
purple_group_class_init(PurpleGroupClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);
	obj_class->finalize = purple_group_finalize;
}

static void
purple_group_init(GTypeInstance *instance, gpointer class)
{

}

GType
purple_group_get_gtype(void)
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

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
#define _PURPLE_BLIST_NODE_C_
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

PurpleBlistNode *
purple_blist_get_last_sibling(PurpleBlistNode *node)
{
	PurpleBlistNode *n = node;
	if (!n)
		return NULL;
	while (n->next)
		n = n->next;
	return n;
}

PurpleBlistNode *
purple_blist_get_last_child(PurpleBlistNode *node)
{
	if (!node)
		return NULL;
	return purple_blist_get_last_sibling(node->child);
}
  
void
parse_setting(PurpleBlistNode *node, xmlnode *setting)
{
  const char *name = xmlnode_get_attrib(setting, "name");
  const char *type = xmlnode_get_attrib(setting, "type");
  char *value = xmlnode_get_data(setting);

  if (!value)
    return;

  if (!type || purple_strequal(type, "string"))
    purple_blist_node_set_string(node, name, value);
  else if (purple_strequal(type, "bool"))
    purple_blist_node_set_bool(node, name, atoi(value));
  else if (purple_strequal(type, "int"))
    purple_blist_node_set_int(node, name, atoi(value));

  g_free(value);
}

/*****************************************************************************
 * Public API functions                                                      *
 *****************************************************************************/

static PurpleBlistNode *get_next_node(PurpleBlistNode *node, gboolean godeep)
{
	if (node == NULL)
		return NULL;

	if (godeep && node->child)
		return node->child;

	if (node->next)
		return node->next;

	return get_next_node(node->parent, FALSE);
}

PurpleBlistNode *purple_blist_node_next(PurpleBlistNode *node, gboolean offline)
{
	PurpleBlistNode *ret = node;

	if (offline)
		return get_next_node(ret, TRUE);
	do
	{
		ret = get_next_node(ret, TRUE);
	} while (ret && PURPLE_BLIST_NODE_IS_BUDDY(ret) &&
			!purple_account_is_connected(purple_buddy_get_account((PurpleBuddy *)ret)));

	return ret;
}

PurpleBlistNode *purple_blist_node_get_parent(PurpleBlistNode *node)
{
	return node ? node->parent : NULL;
}

PurpleBlistNode *purple_blist_node_get_first_child(PurpleBlistNode *node)
{
	return node ? node->child : NULL;
}

PurpleBlistNode *purple_blist_node_get_sibling_next(PurpleBlistNode *node)
{
	return node? node->next : NULL;
}

PurpleBlistNode *purple_blist_node_get_sibling_prev(PurpleBlistNode *node)
{
	return node? node->prev : NULL;
}

void *
purple_blist_node_get_ui_data(const PurpleBlistNode *node)
{
	g_return_val_if_fail(node, NULL);

	return node->ui_data;
}

void
purple_blist_node_set_ui_data(PurpleBlistNode *node, void *ui_data) {
	g_return_if_fail(node);

	node->ui_data = ui_data;
}
  
void
purple_blist_update_node_icon(PurpleBlistNode *node)
{
  PurpleBlistUiOps *ops = purple_blist_get_ui_ops();

  g_return_if_fail(node != NULL);

  if (ops && ops->update)
    ops->update(purplebuddylist, node);
}

void purple_blist_add_chat(PurpleChat *chat, PurpleGroup *group, PurpleBlistNode *node)
{
	PurpleBlistNode *cnode = (PurpleBlistNode*)chat;
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();

	g_return_if_fail(chat != NULL);
	g_return_if_fail(PURPLE_BLIST_NODE_IS_CHAT((PurpleBlistNode *)chat));

	if (node == NULL) {
		if (group == NULL)
			group = purple_group_new(_("Chats"));

		/* Add group to blist if isn't already on it. Fixes #2752. */
		if (!purple_find_group(group->name)) {
			purple_blist_add_group(group,
					purple_blist_get_last_sibling(purplebuddylist->root));
		}
	} else {
		group = (PurpleGroup*)node->parent;
	}

	/* if we're moving to overtop of ourselves, do nothing */
	if (cnode == node)
		return;

	if (cnode->parent) {
		/* This chat was already in the list and is
		 * being moved.
		 */
		((PurpleGroup *)cnode->parent)->totalsize--;
		if (purple_account_is_connected(chat->account)) {
			((PurpleGroup *)cnode->parent)->online--;
			((PurpleGroup *)cnode->parent)->currentsize--;
		}
		if (cnode->next)
			cnode->next->prev = cnode->prev;
		if (cnode->prev)
			cnode->prev->next = cnode->next;
		if (cnode->parent->child == cnode)
			cnode->parent->child = cnode->next;

		if (ops && ops->remove)
			ops->remove(purplebuddylist, cnode);
		/* ops->remove() cleaned up the cnode's ui_data, so we need to
		 * reinitialize it */
		if (ops && ops->new_node)
			ops->new_node(cnode);

		purple_blist_schedule_save();
	}

	if (node != NULL) {
		if (node->next)
			node->next->prev = cnode;
		cnode->next = node->next;
		cnode->prev = node;
		cnode->parent = node->parent;
		node->next = cnode;
		((PurpleGroup *)node->parent)->totalsize++;
		if (purple_account_is_connected(chat->account)) {
			((PurpleGroup *)node->parent)->online++;
			((PurpleGroup *)node->parent)->currentsize++;
		}
	} else {
		if (((PurpleBlistNode *)group)->child)
			((PurpleBlistNode *)group)->child->prev = cnode;
		cnode->next = ((PurpleBlistNode *)group)->child;
		cnode->prev = NULL;
		((PurpleBlistNode *)group)->child = cnode;
		cnode->parent = (PurpleBlistNode *)group;
		group->totalsize++;
		if (purple_account_is_connected(chat->account)) {
			group->online++;
			group->currentsize++;
		}
	}

	purple_blist_schedule_save();

	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode *)cnode);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-added",
			cnode);
}

void purple_blist_add_buddy(PurpleBuddy *buddy, PurpleContact *contact, PurpleGroup *group, PurpleBlistNode *node)
{
	PurpleBlistNode *cnode, *bnode;
	PurpleGroup *g;
	PurpleContact *c;
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	struct _purple_hbuddy *hb, *hb2;
	GHashTable *account_buddies;

	g_return_if_fail(buddy != NULL);
	g_return_if_fail(PURPLE_BLIST_NODE_IS_BUDDY((PurpleBlistNode*)buddy));

	bnode = (PurpleBlistNode *)buddy;

	/* if we're moving to overtop of ourselves, do nothing */
	if (bnode == node || (!node && bnode->parent &&
				contact && bnode->parent == (PurpleBlistNode*)contact
				&& bnode == bnode->parent->child))
		return;

	if (node && PURPLE_BLIST_NODE_IS_BUDDY(node)) {
		c = (PurpleContact*)node->parent;
		g = (PurpleGroup*)node->parent->parent;
	} else if (contact) {
		c = contact;
		g = PURPLE_GROUP(PURPLE_BLIST_NODE(c)->parent);
	} else {
		g = group;
		if (g == NULL)
			g = purple_group_new(_("Buddies"));
		/* Add group to blist if isn't already on it. Fixes #2752. */
		if (!purple_find_group(g->name)) {
			purple_blist_add_group(g,
					purple_blist_get_last_sibling(purplebuddylist->root));
		}
		c = purple_contact_new();
		purple_blist_add_contact(c, g,
				purple_blist_get_last_child((PurpleBlistNode*)g));
	}

	cnode = (PurpleBlistNode *)c;

	if (bnode->parent) {
		if (PURPLE_BUDDY_IS_ONLINE(buddy)) {
			((PurpleContact*)bnode->parent)->online--;
			if (((PurpleContact*)bnode->parent)->online == 0)
				((PurpleGroup*)bnode->parent->parent)->online--;
		}
		if (purple_account_is_connected(buddy->account)) {
			((PurpleContact*)bnode->parent)->currentsize--;
			if (((PurpleContact*)bnode->parent)->currentsize == 0)
				((PurpleGroup*)bnode->parent->parent)->currentsize--;
		}
		((PurpleContact*)bnode->parent)->totalsize--;
		/* the group totalsize will be taken care of by remove_contact below */

		if (bnode->parent->parent != (PurpleBlistNode*)g)
			serv_move_buddy(buddy, (PurpleGroup *)bnode->parent->parent, g);

		if (bnode->next)
			bnode->next->prev = bnode->prev;
		if (bnode->prev)
			bnode->prev->next = bnode->next;
		if (bnode->parent->child == bnode)
			bnode->parent->child = bnode->next;

		if (ops && ops->remove)
			ops->remove(purplebuddylist, bnode);

		purple_blist_schedule_save();

		if (bnode->parent->parent != (PurpleBlistNode*)g) {
			hb = g_new(struct _purple_hbuddy, 1);
			hb->name = g_strdup(purple_normalize(buddy->account, buddy->name));
			hb->account = buddy->account;
			hb->group = bnode->parent->parent;
			g_hash_table_remove(purplebuddylist->buddies, hb);

			account_buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, buddy->account);
			g_hash_table_remove(account_buddies, hb);

			g_free(hb->name);
			g_free(hb);
		}

		if (!bnode->parent->child) {
			purple_blist_remove_contact((PurpleContact*)bnode->parent);
		} else {
			purple_contact_invalidate_priority_buddy((PurpleContact*)bnode->parent);
			if (ops && ops->update)
				ops->update(purplebuddylist, bnode->parent);
		}
	}

	if (node && PURPLE_BLIST_NODE_IS_BUDDY(node)) {
		if (node->next)
			node->next->prev = bnode;
		bnode->next = node->next;
		bnode->prev = node;
		bnode->parent = node->parent;
		node->next = bnode;
	} else {
		if (cnode->child)
			cnode->child->prev = bnode;
		bnode->prev = NULL;
		bnode->next = cnode->child;
		cnode->child = bnode;
		bnode->parent = cnode;
	}

	if (PURPLE_BUDDY_IS_ONLINE(buddy)) {
		if (++(PURPLE_CONTACT(bnode->parent)->online) == 1)
			PURPLE_GROUP(bnode->parent->parent)->online++;
	}
	if (purple_account_is_connected(buddy->account)) {
		if (++(PURPLE_CONTACT(bnode->parent)->currentsize) == 1)
			PURPLE_GROUP(bnode->parent->parent)->currentsize++;
	}
	PURPLE_CONTACT(bnode->parent)->totalsize++;

	hb = g_new(struct _purple_hbuddy, 1);
	hb->name = g_strdup(purple_normalize(buddy->account, buddy->name));
	hb->account = buddy->account;
	hb->group = ((PurpleBlistNode*)buddy)->parent->parent;

	g_hash_table_replace(purplebuddylist->buddies, hb, buddy);

	account_buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, buddy->account);

	hb2 = g_new(struct _purple_hbuddy, 1);
	hb2->name = g_strdup(hb->name);
	hb2->account = buddy->account;
	hb2->group = ((PurpleBlistNode*)buddy)->parent->parent;

	g_hash_table_replace(account_buddies, hb2, buddy);

	purple_contact_invalidate_priority_buddy(purple_buddy_get_contact(buddy));

	purple_blist_schedule_save();

	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode*)buddy);

	/* Signal that the buddy has been added */
	purple_signal_emit(purple_blist_get_handle(), "buddy-added", buddy);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-added",
			PURPLE_BLIST_NODE(buddy));
}

void purple_blist_merge_contact(PurpleContact *source, PurpleBlistNode *node)
{
	PurpleBlistNode *sourcenode = (PurpleBlistNode*)source;
	PurpleBlistNode *targetnode;
	PurpleBlistNode *prev, *cur, *next;
	PurpleContact *target;
g_return_if_fail(source != NULL);
	g_return_if_fail(node != NULL);

	if (PURPLE_BLIST_NODE_IS_CONTACT(node)) {
		target = (PurpleContact *)node;
		prev = purple_blist_get_last_child(node);
	} else if (PURPLE_BLIST_NODE_IS_BUDDY(node)) {
		target = (PurpleContact *)node->parent;
		prev = node;
	} else {
		return;
	}

	if (source == target || !target)
		return;

	targetnode = (PurpleBlistNode *)target;
	next = sourcenode->child;

	while (next) {
		cur = next;
		next = cur->next;
		if (PURPLE_BLIST_NODE_IS_BUDDY(cur)) {
			purple_blist_add_buddy((PurpleBuddy *)cur, target, NULL, prev);
			prev = cur;
		}
	}
}

void
purple_blist_node_destroy(PurpleBlistNode *node)
{
	PurpleBlistUiOps *ui_ops;
	PurpleBlistNode *child, *next_child;

	ui_ops = purple_blist_get_ui_ops();
	child = node->child;
	while (child) {
		next_child = child->next;
		purple_blist_node_destroy(child);
		child = next_child;
	}

	/* Allow the UI to free data */
	node->parent = NULL;
	node->child  = NULL;
	node->next   = NULL;
	node->prev   = NULL;
	if (ui_ops && ui_ops->remove)
		ui_ops->remove(purplebuddylist, node);

	if (PURPLE_BLIST_NODE_IS_BUDDY(node))
		purple_buddy_destroy((PurpleBuddy*)node);
	else if (PURPLE_BLIST_NODE_IS_CHAT(node))
		purple_chat_destroy((PurpleChat*)node);
	else if (PURPLE_BLIST_NODE_IS_CONTACT(node))
		purple_contact_destroy((PurpleContact*)node);
	else if (PURPLE_BLIST_NODE_IS_GROUP(node))
		purple_group_destroy((PurpleGroup*)node);
}

void purple_blist_node_initialize_settings(PurpleBlistNode *node)
{
	if (node->settings)
		return;

	node->settings = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
			(GDestroyNotify)purple_g_value_slice_free);
}

void purple_blist_node_remove_setting(PurpleBlistNode *node, const char *key)
{
	g_return_if_fail(node != NULL);
	g_return_if_fail(node->settings != NULL);
	g_return_if_fail(key != NULL);

	g_hash_table_remove(node->settings, key);

	purple_blist_schedule_save();
}

void
purple_blist_node_set_flags(PurpleBlistNode *node, PurpleBlistNodeFlags flags)
{
	g_return_if_fail(node != NULL);

	node->flags = flags;
}

PurpleBlistNodeFlags
purple_blist_node_get_flags(PurpleBlistNode *node)
{
	g_return_val_if_fail(node != NULL, 0);

	return node->flags;
}

PurpleBlistNodeType
purple_blist_node_get_type(PurpleBlistNode *node)
{
	g_return_val_if_fail(node != NULL, PURPLE_BLIST_OTHER_NODE);
	return node->type;
}

gboolean
purple_blist_node_has_setting(PurpleBlistNode *node,
                              const char *key)
{
	g_return_val_if_fail(node != NULL, FALSE);
	g_return_val_if_fail(node->settings != NULL, FALSE);
	g_return_val_if_fail(key != NULL, FALSE);

	return (g_hash_table_lookup(node->settings, key) != NULL);
}

void
purple_blist_node_set_bool(PurpleBlistNode* node, const char *key, gboolean data)
{
	GValue *value;

	g_return_if_fail(node != NULL);
	g_return_if_fail(node->settings != NULL);
	g_return_if_fail(key != NULL);

	value = purple_g_value_slice_new(G_TYPE_BOOLEAN);
	g_value_set_boolean(value, data);

	g_hash_table_replace(node->settings, g_strdup(key), value);

	purple_blist_schedule_save();
}

gboolean
purple_blist_node_get_bool(PurpleBlistNode* node, const char *key)
{
	GValue *value;

	g_return_val_if_fail(node != NULL, FALSE);
	g_return_val_if_fail(node->settings != NULL, FALSE);
	g_return_val_if_fail(key != NULL, FALSE);

	value = g_hash_table_lookup(node->settings, key);

	if (value == NULL)
		return FALSE;

	g_return_val_if_fail(G_VALUE_HOLDS_BOOLEAN(value), FALSE);

	return g_value_get_boolean(value);
}

void
purple_blist_node_set_int(PurpleBlistNode* node, const char *key, int data)
{
	GValue *value;

	g_return_if_fail(node != NULL);
	g_return_if_fail(node->settings != NULL);
	g_return_if_fail(key != NULL);

	value = purple_g_value_slice_new(G_TYPE_INT);
	g_value_set_int(value, data);

	g_hash_table_replace(node->settings, g_strdup(key), value);

	purple_blist_schedule_save();
}

int
purple_blist_node_get_int(PurpleBlistNode* node, const char *key)
{
	GValue *value;

	g_return_val_if_fail(node != NULL, 0);
	g_return_val_if_fail(node->settings != NULL, 0);
	g_return_val_if_fail(key != NULL, 0);

	value = g_hash_table_lookup(node->settings, key);

	if (value == NULL)
		return 0;

	g_return_val_if_fail(G_VALUE_HOLDS_INT(value), 0);

	return g_value_get_int(value);
}

void
purple_blist_node_set_string(PurpleBlistNode* node, const char *key, const char *data)
{
	GValue *value;

	g_return_if_fail(node != NULL);
	g_return_if_fail(node->settings != NULL);
	g_return_if_fail(key != NULL);

	value = purple_g_value_slice_new(G_TYPE_STRING);
	g_value_set_string(value, data);

	g_hash_table_replace(node->settings, g_strdup(key), value);

	purple_blist_schedule_save();
}

const char *
purple_blist_node_get_string(PurpleBlistNode* node, const char *key)
{
	GValue *value;

	g_return_val_if_fail(node != NULL, NULL);
	g_return_val_if_fail(node->settings != NULL, NULL);
	g_return_val_if_fail(key != NULL, NULL);

	value = g_hash_table_lookup(node->settings, key);

	if (value == NULL)
		return NULL;

	g_return_val_if_fail(G_VALUE_HOLDS_STRING(value), NULL);

	return g_value_get_string(value);
}

GList *
purple_blist_node_get_extended_menu(PurpleBlistNode *n)
{
	GList *menu = NULL;

	g_return_val_if_fail(n != NULL, NULL);

	purple_signal_emit(purple_blist_get_handle(),
			"blist-node-extended-menu",
			n, &menu);
	return menu;
}

/******************/
/*  GObject Code  */
/******************/

static void
purple_blist_node_class_init(PurpleBlistNodeClass *klass)
{

}

static void
purple_blist_node_init(GTypeInstance *instance, gpointer class)
{

}

GType
purple_blist_node_get_gtype(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleBlistNodeClass),
			NULL,					/* base_init		*/
			NULL,					/* base_finalize	*/
			(GClassInitFunc)purple_blist_node_class_init,
			NULL,
			NULL,					/* class_data		*/
			sizeof(PurpleBlistNode),
			0,						/* n_preallocs		*/
			purple_blist_node_init,					/* instance_init	*/
			NULL					/* value_table		*/
		};

		type = g_type_register_static(PURPLE_TYPE_OBJECT,
									  "PurpleBlistNode",
									  &info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}

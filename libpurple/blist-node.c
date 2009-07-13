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
#include "blist-node.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "server.h"
#include "signals.h"
#include "xmlnode.h"

struct _PurpleBlistNodePrivate {
	GHashTable *settings;                 /**< per-node settings; keys are
	                                           <tt>gchar *</tt>, values are
	                                           slice-allocated
	                                           <tt>GValue</tt>.  */

	void *ui_data;                        /**< The UI can put data here.      */
	PurpleBlistNodeFlags flags;           /**< The buddy flags                */
};

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
	} while (ret && PURPLE_IS_BUDDY(ret) &&
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
	PurpleBlistNodePrivate *priv;

	g_return_val_if_fail(node, NULL);
	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	return priv->ui_data;
}

void
purple_blist_node_set_ui_data(PurpleBlistNode *node, void *ui_data) {
	PurpleBlistNodePrivate *priv;

	g_return_if_fail(node);

	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	priv->ui_data = ui_data;
}
  
void
purple_blist_update_node_icon(PurpleBlistNode *node)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();

	g_return_if_fail(node != NULL);

	if (ops && ops->update)
		ops->update(purplebuddylist, node);
}



void purple_blist_merge_contact(PurpleContact *source, PurpleBlistNode *node)
{
	PurpleBlistNode *sourcenode = (PurpleBlistNode*)source;
	PurpleBlistNode *targetnode;
	PurpleBlistNode *prev, *cur, *next;
	PurpleContact *target;
g_return_if_fail(source != NULL);
	g_return_if_fail(node != NULL);

	if (PURPLE_IS_CONTACT(node)) {
		target = (PurpleContact *)node;
		prev = purple_blist_get_last_child(node);
	} else if (PURPLE_IS_BUDDY(node)) {
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
		if (PURPLE_IS_BUDDY(cur)) {
			purple_blist_add_buddy((PurpleBuddy *)cur, target, NULL, prev);
			prev = cur;
		}
	}
}

void
purple_blist_node_destroy(PurpleBlistNode *node)
{
	/* This function is only a hack for api breakage */
	g_return_if_fail(PURPLE_IS_BLIST_NODE(node));
	g_object_unref(G_OBJECT(node));
}

void purple_blist_node_initialize_settings(PurpleBlistNode *node)
{
	PurpleBlistNodePrivate *priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	if (priv->settings)
		return;

	priv->settings = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
			(GDestroyNotify)purple_g_value_slice_free);
}

void purple_blist_node_remove_setting(PurpleBlistNode *node, const char *key)
{
	PurpleBlistNodePrivate *priv;

	g_return_if_fail(node != NULL);

	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	g_return_if_fail(priv->settings != NULL);
	g_return_if_fail(key != NULL);

	g_hash_table_remove(priv->settings, key);

	purple_blist_schedule_save();
}

void
purple_blist_node_set_flags(PurpleBlistNode *node, PurpleBlistNodeFlags flags)
{
	PurpleBlistNodePrivate *priv;
	g_return_if_fail(node != NULL);
	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	priv->flags = flags;
}

PurpleBlistNodeFlags
purple_blist_node_get_flags(PurpleBlistNode *node)
{
	PurpleBlistNodePrivate *priv;
	g_return_val_if_fail(node != NULL, 0);
	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	return priv->flags;
}

gboolean
purple_blist_node_has_setting(PurpleBlistNode *node,
                              const char *key)
{
	PurpleBlistNodePrivate *priv;

	g_return_val_if_fail(node != NULL, FALSE);

	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	g_return_val_if_fail(priv->settings != NULL, FALSE);
	g_return_val_if_fail(key != NULL, FALSE);

	return (g_hash_table_lookup(priv->settings, key) != NULL);
}

void
purple_blist_node_set_bool(PurpleBlistNode* node, const char *key, gboolean data)
{
	GValue *value;
	PurpleBlistNodePrivate *priv;

	g_return_if_fail(node != NULL);

	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	g_return_if_fail(priv->settings != NULL);
	g_return_if_fail(key != NULL);

	value = purple_g_value_slice_new(G_TYPE_BOOLEAN);
	g_value_set_boolean(value, data);

	g_hash_table_replace(priv->settings, g_strdup(key), value);

	purple_blist_schedule_save();
}

gboolean
purple_blist_node_get_bool(PurpleBlistNode* node, const char *key)
{
	GValue *value;
	PurpleBlistNodePrivate *priv;

	g_return_val_if_fail(node != NULL, FALSE);

	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	g_return_val_if_fail(priv->settings != NULL, FALSE);
	g_return_val_if_fail(key != NULL, FALSE);

	value = g_hash_table_lookup(priv->settings, key);

	if (value == NULL)
		return FALSE;

	g_return_val_if_fail(G_VALUE_HOLDS_BOOLEAN(value), FALSE);

	return g_value_get_boolean(value);
}

void
purple_blist_node_set_int(PurpleBlistNode* node, const char *key, int data)
{
	GValue *value;
	PurpleBlistNodePrivate *priv;

	g_return_if_fail(node != NULL);

	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	g_return_if_fail(priv->settings != NULL);
	g_return_if_fail(key != NULL);

	value = purple_g_value_slice_new(G_TYPE_INT);
	g_value_set_int(value, data);

	g_hash_table_replace(priv->settings, g_strdup(key), value);

	purple_blist_schedule_save();
}

int
purple_blist_node_get_int(PurpleBlistNode* node, const char *key)
{
	GValue *value;
	PurpleBlistNodePrivate *priv;

	g_return_val_if_fail(node != NULL, 0);

	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	g_return_val_if_fail(priv->settings != NULL, 0);
	g_return_val_if_fail(key != NULL, 0);

	value = g_hash_table_lookup(priv->settings, key);

	if (value == NULL)
		return 0;

	g_return_val_if_fail(G_VALUE_HOLDS_INT(value), 0);

	return g_value_get_int(value);
}

void
purple_blist_node_set_string(PurpleBlistNode* node, const char *key, const char *data)
{
	GValue *value;
	PurpleBlistNodePrivate *priv;

	g_return_if_fail(node != NULL);

	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	g_return_if_fail(priv->settings != NULL);
	g_return_if_fail(key != NULL);

	value = purple_g_value_slice_new(G_TYPE_STRING);
	g_value_set_string(value, data);

	g_hash_table_replace(priv->settings, g_strdup(key), value);

	purple_blist_schedule_save();
}

GHashTable *
purple_blist_node_get_settings(PurpleBlistNode *node)
{
	PurpleBlistNodePrivate *priv;
	g_return_val_if_fail(node != NULL, NULL);

	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	return priv->settings;
}	

const char *
purple_blist_node_get_string(PurpleBlistNode* node, const char *key)
{
	GValue *value;
	PurpleBlistNodePrivate *priv;

	g_return_val_if_fail(node != NULL, NULL);

	priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);

	g_return_val_if_fail(priv->settings != NULL, NULL);
	g_return_val_if_fail(key != NULL, NULL);

	value = g_hash_table_lookup(priv->settings, key);

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

gboolean
purple_blist_node_contains(PurpleBlistNode *parent, PurpleBlistNode *node)
{
	return node->parent == parent;
}

void
purple_blist_node_add_child(PurpleBlistNode *parent, PurpleBlistNode *child)
{
	if (parent->child)
		parent->child->prev = child;
	child->prev = NULL;
	child->next = parent->child;
	parent->child = child;
	child->parent = parent;
}

void
purple_blist_node_add_sibling_before(PurpleBlistNode *child, PurpleBlistNode *location)
{
	g_return_if_fail(location);
	g_return_if_fail(child);
	child->next = location;
	child->prev = location->prev;
	location->prev->next = child;
	location->prev = child;
}

void
purple_blist_node_add_sibling_after(PurpleBlistNode *child, PurpleBlistNode *location)
{
	g_return_if_fail(child);
	g_return_if_fail(location);
	if (location->next)
		location->next->prev = child;
	child->next = location->next;
	child->prev = location;
	child->parent = location->parent;
	location->next = child;
}

void
purple_blist_node_remove(PurpleBlistNode *child)
{
	/* Remove the node from its parent */
	if (child->prev)
		child->prev->next = child->next;
	if (child->next)
		child->next->prev = child->prev;
	/* Is the child the first in the list? */
	if ((child->parent != NULL) && (child->parent->child == child))
		child->parent->child = child->next;

	purple_blist_node_strip(child);
}

void
purple_blist_node_strip(PurpleBlistNode *node)
{
	g_return_if_fail(node);
	node->parent = node->child = node->next = node->prev = NULL;
}

gboolean
purple_blist_node_is_empty(PurpleBlistNode *node)
{
	return node->child;
}

/******************/
/*  GObject Code  */
/******************/

static GObjectClass *parent_class = NULL;

static void
purple_blist_node_finalize(GObject *object)
{
	PurpleBlistNode *node = PURPLE_BLIST_NODE(object);
	PurpleBlistNodePrivate *priv = PURPLE_BLIST_NODE_GET_PRIVATE(node);
	PurpleBlistUiOps *ui_ops;
	PurpleBlistNode *child, *next_child;

	ui_ops = purple_blist_get_ui_ops();
	child = node->child;
	while (child) {
		next_child = child->next;
		g_object_unref(child);
		child = next_child;
	}

	/* Allow the UI to free data */
	node->parent = NULL;
	node->child  = NULL;
	node->next   = NULL;
	node->prev   = NULL;

	g_hash_table_destroy(priv->settings);

	if (ui_ops && ui_ops->remove)
		ui_ops->remove(purplebuddylist, node);

	if (PURPLE_IS_BUDDY(node))
		g_object_unref(node);
	else if (PURPLE_IS_CHAT(node))
		g_object_unref(node);
	else if (PURPLE_IS_CONTACT(node))
		g_object_unref(node);
	else if (PURPLE_IS_GROUP(node))
		g_object_unref(node);

	parent_class->finalize(object);
}

static void *
purple_blist_node_handle()
{
	static int handle;
	return &handle;
}

static void
purple_blist_node_class_init(PurpleBlistNodeClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);
	obj_class->finalize = purple_blist_node_finalize;

	g_type_class_add_private(klass, sizeof(PurpleBlistNodePrivate));

	klass->add_node = purple_blist_node_add_child;
	klass->remove_node = purple_blist_node_remove;

	purple_signal_register( purple_blist_node_handle(),
													"node-updated",
													purple_marshal_VOID__POINTER,
													NULL,
													1,
													purple_value_new(PURPLE_TYPE_SUBTYPE, PURPLE_SUBTYPE_BLIST_NODE));


}

static void
purple_blist_node_init(GTypeInstance *instance, gpointer class)
{
	purple_blist_node_initialize_settings(PURPLE_BLIST_NODE(instance));
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

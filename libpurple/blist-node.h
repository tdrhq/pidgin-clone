/**
 * @file blist.h Buddy List API
 * @ingroup core
 * @see @ref blist-signals
 */

/* purple
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

#ifndef _PURPLE_BLIST_NODE_H_
#define _PURPLE_BLIST_NODE_H_

#include <glib.h>

#define PURPLE_BUDDY_IS_ONLINE(b) \
	((b) != NULL && purple_account_is_connected(purple_buddy_get_account(b)) && \
	 purple_presence_is_online(purple_buddy_get_presence(b)))

typedef enum
{
	PURPLE_BLIST_NODE_FLAG_NO_SAVE      = 1 << 0 /**< node should not be saved with the buddy list */

} PurpleBlistNodeFlags;

/**
 * @since 2.6.0
 */
#define PURPLE_BLIST_NODE_HAS_FLAG(b, f) (purple_blist_node_get_flags((PurpleBlistNode*)(b)) & (f))
#define PURPLE_BLIST_NODE_SHOULD_SAVE(b) (! PURPLE_BLIST_NODE_HAS_FLAG(b, PURPLE_BLIST_NODE_FLAG_NO_SAVE))

/** @copydoc _PurpleBlistNode */
typedef struct _PurpleBlistNode PurpleBlistNode;
typedef struct _PurpleBlistNodePrivate PurpleBlistNodePrivate;
typedef struct _PurpleBlistNodeClass PurpleBlistNodeClass;
#define PURPLE_BLIST_NODE_TYPE                  (purple_blist_node_get_gtype ())
#define PURPLE_BLIST_NODE(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), PURPLE_BLIST_NODE_TYPE, PurpleBlistNode))
#define PURPLE_IS_BLIST_NODE(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PURPLE_BLIST_NODE_TYPE))
#define PURPLE_BLIST_NODE_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), PURPLE_BLIST_NODE_TYPE, PurpleBlistNodeClass))
#define PURPLE_IS_BLIST_NODE_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), PURPLE_BLIST_NODE_TYPE))
#define PURPLE_GET_BLIST_NODE_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), PURPLE_BLIST_NODE_TYPE, PurpleBlistNodeClass))
#define PURPLE_BLIST_NODE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), PURPLE_BLIST_NODE_TYPE, PurpleBlistNodePrivate))


/**
 * A Buddy list node.  This can represent a group, a buddy, or anything else.
 * This is a base class for PurpleBuddy, PurpleContact, PurpleGroup, and for
 * anything else that wants to put itself in the buddy list. */
struct _PurpleBlistNode {
	PurpleObject _parent;
	PurpleBlistNode *prev;                /**< The sibling before this buddy. */
	PurpleBlistNode *next;                /**< The sibling after this buddy.  */
	PurpleBlistNode *parent;              /**< The parent of this node        */
	PurpleBlistNode *child;               /**< The child of this node         */

	PurpleBlistNodePrivate *priv;         /**< The private members            */
};

struct _PurpleBlistNodeClass {
	PurpleObjectClass parent;

	void (*add_node)(PurpleBlistNode *parent, PurpleBlistNode *child, PurpleBlistNode *location);
	void (*remove_node)(PurpleBlistNode *child);

	void (*purple_reserved1)(void);
	void (*purple_reserved2)(void);
	void (*purple_reserved3)(void);
	void (*purple_reserved4)(void);
};

/**
 * Returns the next node of a given node. This function is to be used to iterate
 * over the tree returned by purple_get_blist.
 *
 * @param node		A node.
 * @param offline	Whether to include nodes for offline accounts
 * @return	The next node
 * @see purple_blist_node_get_parent
 * @see purple_blist_node_get_first_child
 * @see purple_blist_node_get_sibling_next
 * @see purple_blist_node_get_sibling_prev
 */
PurpleBlistNode *purple_blist_node_next(PurpleBlistNode *node, gboolean offline);

/**
 * Returns the parent node of a given node.
 *
 * @param node A node.
 * @return  The parent node.
 * @since 2.4.0
 * @see purple_blist_node_get_first_child
 * @see purple_blist_node_get_sibling_next
 * @see purple_blist_node_get_sibling_prev
 * @see purple_blist_node_next
 */
PurpleBlistNode *purple_blist_node_get_parent(PurpleBlistNode *node);

/**
 * Returns the the first child node of a given node.
 *
 * @param node A node.
 * @return  The child node.
 * @since 2.4.0
 * @see purple_blist_node_get_parent
 * @see purple_blist_node_get_sibling_next
 * @see purple_blist_node_get_sibling_prev
 * @see purple_blist_node_next
 */
PurpleBlistNode *purple_blist_node_get_first_child(PurpleBlistNode *node);

/**
 * Returns the sibling node of a given node.
 *
 * @param node A node.
 * @return  The sibling node.
 * @since 2.4.0
 * @see purple_blist_node_get_parent
 * @see purple_blist_node_get_first_child
 * @see purple_blist_node_get_sibling_prev
 * @see purple_blist_node_next
 */
PurpleBlistNode *purple_blist_node_get_sibling_next(PurpleBlistNode *node);

/**
 * Returns the previous sibling node of a given node.
 *
 * @param node A node.
 * @return  The sibling node.
 * @since 2.4.0
 * @see purple_blist_node_get_parent
 * @see purple_blist_node_get_first_child
 * @see purple_blist_node_get_sibling_next
 * @see purple_blist_node_next
 */
PurpleBlistNode *purple_blist_node_get_sibling_prev(PurpleBlistNode *node);

/**
 * Check whether a node is contained in a parent node. Simply checks if the node's parent is the other node.
 *
 * @param parent The containing node
 * @param node The node to check with.
 * @return Returns TRUE if node's parent is parent.
 */
gboolean purple_blist_node_contains(PurpleBlistNode *parent, PurpleBlistNode *node);

/**
 * Add a node as the first child of a given node.
 *
 * @param parent The parent node
 * @param child The child node to add to parent
 */
void purple_blist_node_add_child(PurpleBlistNode *parent, PurpleBlistNode *child);

/**
 * Add a node before a given node.
 *
 * @param node The node to add
 * @param location The node to insert before, not NULL
 */
void purple_blist_node_add_sibling_before(PurpleBlistNode *node, PurpleBlistNode *location);

/**
 * Add a node after a given node.
 *
 * @param node The node to add
 * @param location The node to insert after, not NULL
 */
void purple_blist_node_add_sibling_after(PurpleBlistNode *node, PurpleBlistNode *location);

/**
 * Remove a node from its parent
 *
 * @param node The node to remove
 */
void purple_blist_node_remove(PurpleBlistNode *node);

/**
 * Isolate a node by making all its references NULL.
 *
 * @param node The node to isolate
 */
void purple_blist_node_strip(PurpleBlistNode *node);

/**
 * Returns the UI data of a given node.
 *
 * @param node The node.
 * @return The UI data.
 * @since 2.6.0
 */
gpointer purple_blist_node_get_ui_data(const PurpleBlistNode *node);

/**
 * Sets the UI data of a given node.
 *
 * @param node The node.
 * @param ui_data The UI data.
 *
 * @since 2.6.0
 */
void purple_blist_node_set_ui_data(PurpleBlistNode *node, gpointer ui_data);

/**
 * Checks whether a node has a particular setting.
 *
 * @param node  The node in question
 * @param key   The name of a setting
 * @return @c TRUE if @a node has any value set for @a key, and @c FALSE
 *         otherwise.
 */
gboolean purple_blist_node_has_setting(PurpleBlistNode *node, const char *key);

/**
 * Associates a boolean with a node in the buddy list
 *
 * @param node  The node to associate the data with
 * @param key   The identifier for the data
 * @param value The value to set
 */
void purple_blist_node_set_bool(PurpleBlistNode *node, const char *key, gboolean value);

/**
 * Retrieves a named boolean setting from a node in the buddy list
 *
 * @param node  The node to retrieve the data from
 * @param key   The identifier of the data
 *
 * @return The value, or FALSE if there is no setting
 */
gboolean purple_blist_node_get_bool(PurpleBlistNode *node, const char *key);

/**
 * Associates an integer with a node in the buddy list
 *
 * @param node  The node to associate the data with
 * @param key   The identifier for the data
 * @param value The value to set
 */
void purple_blist_node_set_int(PurpleBlistNode *node, const char *key, int value);

/**
 * Return the settings table
 *
 * @param node The node
 *
 * @return the hash table
 */
GHashTable *purple_blist_node_get_settings(PurpleBlistNode *node);

/**
 * Retrieves a named integer setting from a node in the buddy list
 *
 * @param node  The node to retrieve the data from
 * @param key   The identifier of the data
 *
 * @return The value, or 0 if there is no setting
 */
int purple_blist_node_get_int(PurpleBlistNode *node, const char *key);

/**
 * Associates a string with a node in the buddy list
 *
 * @param node  The node to associate the data with
 * @param key   The identifier for the data
 * @param value The value to set
 */
void purple_blist_node_set_string(PurpleBlistNode *node, const char *key,
		const char *value);

/**
 * Retrieves a named string setting from a node in the buddy list
 *
 * @param node  The node to retrieve the data from
 * @param key   The identifier of the data
 *
 * @return The value, or NULL if there is no setting
 */
const char *purple_blist_node_get_string(PurpleBlistNode *node, const char *key);

/**
 * Removes a named setting from a blist node
 *
 * @param node  The node from which to remove the setting
 * @param key   The name of the setting
 */
void purple_blist_node_remove_setting(PurpleBlistNode *node, const char *key);

/**
 * Set the flags for the given node.  Setting a node's flags will overwrite
 * the old flags, so if you want to save them, you must first call
 * purple_blist_node_get_flags and modify that appropriately.
 *
 * @param node  The node on which to set the flags.
 * @param flags The flags to set.  This is a bitmask.
 */
void purple_blist_node_set_flags(PurpleBlistNode *node, PurpleBlistNodeFlags flags);

/**
 * Get the current flags on a given node.
 *
 * @param node The node from which to get the flags.
 *
 * @return The flags on the node.  This is a bitmask.
 */
PurpleBlistNodeFlags purple_blist_node_get_flags(PurpleBlistNode *node);

/*@}*/

/**
 * Retrieves the extended menu items for a buddy list node.
 * @param n The blist node for which to obtain the extended menu items.
 * @return  A list of PurpleMenuAction items, as harvested by the
 *          blist-node-extended-menu signal.
 */
GList *purple_blist_node_get_extended_menu(PurpleBlistNode *n);

/**
 * Get the GType for PurpleBlistNode
 */
GType purple_blist_node_get_gtype(void);

#endif

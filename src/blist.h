/**
 * @file blist.h Buddy List API
 * @ingroup core
 *
 * gaim
 *
 * Gaim is the legal property of its developers, whose names are too numerous
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* I can't believe I let ChipX86 inspire me to write good code. -Sean */

#ifndef _BLIST_H_
#define _BLIST_H_

#include <glib.h>

typedef struct _GaimBuddyList GaimBuddyList;
typedef struct _GaimBlistUiOps GaimBlistUiOps;
typedef struct _GaimBlistNode GaimBlistNode;

typedef struct _GaimBlistNodeAction GaimBlistNodeAction;

typedef struct _GaimChat GaimChat;
typedef struct _GaimGroup GaimGroup;
typedef struct _GaimContact GaimContact;
typedef struct _GaimBuddy GaimBuddy;

#include "account.h"
#include "buddyicon.h"

/**************************************************************************/
/* Enumerations                                                           */
/**************************************************************************/
typedef enum
{
	GAIM_BLIST_GROUP_NODE,
	GAIM_BLIST_CONTACT_NODE,
	GAIM_BLIST_BUDDY_NODE,
	GAIM_BLIST_CHAT_NODE,
	GAIM_BLIST_OTHER_NODE

} GaimBlistNodeType;

#define GAIM_BLIST_NODE_IS_CHAT(n) ((n)->type == GAIM_BLIST_CHAT_NODE)
#define GAIM_BLIST_NODE_IS_BUDDY(n) ((n)->type == GAIM_BLIST_BUDDY_NODE)
#define GAIM_BLIST_NODE_IS_CONTACT(n) ((n)->type == GAIM_BLIST_CONTACT_NODE)
#define GAIM_BLIST_NODE_IS_GROUP(n) ((n)->type == GAIM_BLIST_GROUP_NODE)

typedef enum
{
	GAIM_BUDDY_SIGNING_OFF = -1,
	GAIM_BUDDY_OFFLINE = 0,
	GAIM_BUDDY_ONLINE,
	GAIM_BUDDY_SIGNING_ON

} GaimBuddyPresenceState;

#define GAIM_BUDDY_IS_ONLINE(b) ((b)->account->gc && \
		((b)->present == GAIM_BUDDY_ONLINE || \
		 (b)->present == GAIM_BUDDY_SIGNING_ON))


/**************************************************************************/
/* Data Structures                                                        */
/**************************************************************************/

/**
 * A Buddy list node.  This can represent a group, a buddy, or anything else.  This is a base class for struct buddy and
 * struct group and for anything else that wants to put itself in the buddy list. */
struct _GaimBlistNode {
	GaimBlistNodeType type;             /**< The type of node this is       */
	GaimBlistNode *prev;                /**< The sibling before this buddy. */
	GaimBlistNode *next;                /**< The sibling after this buddy.  */
	GaimBlistNode *parent;              /**< The parent of this node        */
	GaimBlistNode *child;               /**< The child of this node         */
	GHashTable *settings;               /**< per-node settings              */
	void          *ui_data;             /**< The UI can put data here.      */
};

/**
 * A buddy.  This contains everything Gaim will ever need to know about someone on the buddy list.  Everything.
 */
struct _GaimBuddy {
	GaimBlistNode node;                     /**< The node that this buddy inherits from */
	char *name;                             /**< The screenname of the buddy. */
	char *alias;                            /**< The user-set alias of the buddy */
	char *server_alias;                     /**< The server-specified alias of the buddy.  (i.e. MSN "Friendly Names") */
	GaimBuddyPresenceState present;         /**< This is 0 if the buddy appears offline, 1 if he appears online, and 2 if
						    he has recently signed on */
	int evil;                               /**< The warning level */
	time_t signon;                          /**< The time the buddy signed on. */
	int idle;                               /**< The time the buddy has been idle in minutes. */
	int uc;                                 /**< This is a cryptic bitmask that makes sense only to the prpl.  This will get changed */
	void *proto_data;                       /**< This allows the prpl to associate whatever data it wants with a buddy */
	GaimBuddyIcon *icon;                    /**< The buddy icon. */
	GaimAccount *account;           /**< the account this buddy belongs to */
	guint timer;							/**< The timer handle. */
};

/**
 * A contact.  This contains everything Gaim will ever need to know about a contact.
 */
struct _GaimContact {
	GaimBlistNode node;		/**< The node that this contact inherits from. */
	char *alias;            /**< The user-set alias of the contact */
	int totalsize;		    /**< The number of buddies in this contact */
	int currentsize;	    /**< The number of buddies in this contact corresponding to online accounts */
	int online;			    /**< The number of buddies in this contact who are currently online */
	GaimBuddy *priority;    /**< The "top" buddy for this contact */
};


/**
 * A group.  This contains everything Gaim will ever need to know about a group.
 */
struct _GaimGroup {
	GaimBlistNode node;                    /**< The node that this group inherits from */
	char *name;                            /**< The name of this group. */
	int totalsize;			       /**< The number of chats and contacts in this group */
	int currentsize;		       /**< The number of chats and contacts in this group corresponding to online accounts */
	int online;			       /**< The number of chats and contacts in this group who are currently online */
};

/**
 * A chat.  This contains everything Gaim needs to put a chat room in the
 * buddy list.
 */
struct _GaimChat {
	GaimBlistNode node;      /**< The node that this chat inherits from */
	char *alias;             /**< The display name of this chat. */
	GHashTable *components;  /**< the stuff the protocol needs to know to join the chat */
	GaimAccount *account; /**< The account this chat is attached to */
};


/**
 * The Buddy List
 */
struct _GaimBuddyList {
	GaimBlistNode *root;          /**< The first node in the buddy list */
	GHashTable *buddies;          /**< Every buddy in this list */
	GaimBlistUiOps *ui_ops;       /**< The UI operations for the buddy list */

	void *ui_data;                /**< UI-specific data. */
};

/**
 * Buddy list UI operations.
 *
 * Any UI representing a buddy list must assign a filled-out gaim_blist_window_ops
 * structure to the buddy list core.
 */
struct _GaimBlistUiOps
{
	void (*new_list)(GaimBuddyList *list); /**< Sets UI-specific data on a buddy list. */
	void (*new_node)(GaimBlistNode *node);      /**< Sets UI-specific data on a node. */
	void (*show)(GaimBuddyList *list);     /**< The core will call this when its finished doing it's core stuff */
	void (*update)(GaimBuddyList *list,
		       GaimBlistNode *node);            /**< This will update a node in the buddy list. */
	void (*remove)(GaimBuddyList *list,
		       GaimBlistNode *node);            /**< This removes a node from the list */
	void (*destroy)(GaimBuddyList *list);  /**< When the list gets destroyed, this gets called to destroy the UI. */
	void (*set_visible)(GaimBuddyList *list,
			    gboolean show);             /**< Hides or unhides the buddy list */
	void (*request_add_buddy)(GaimAccount *account, const char *username,
							  const char *group, const char *alias);
	void (*request_add_chat)(GaimAccount *account, GaimGroup *group, 
							 const char *alias);
	void (*request_add_group)(void);
};


struct _GaimBlistNodeAction {
	char *label;
	void (*callback)(GaimBlistNode *, gpointer);
	gpointer data;
};


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/** @name Buddy List API                                                  */
/**************************************************************************/
/*@{*/

/**
 * Creates a new buddy list
 */
GaimBuddyList *gaim_blist_new();

/**
 * Sets the main buddy list.
 *
 * @return The main buddy list.
 */
void gaim_set_blist(GaimBuddyList *blist);

/**
 * Returns the main buddy list.
 *
 * @return The main buddy list.
 */
GaimBuddyList *gaim_get_blist(void);

/**
 * Shows the buddy list, creating a new one if necessary.
 *
 */
void gaim_blist_show();


/**
 * Destroys the buddy list window.
 */
void gaim_blist_destroy();

/**
 * Hides or unhides the buddy list.
 *
 * @param show   Whether or not to show the buddy list
 */
void gaim_blist_set_visible(gboolean show);

/**
 * Updates a buddy's status.
 *
 * This needs to not take an int.
 *
 * @param buddy   The buddy whose status has changed
 * @param status  The new status in cryptic prpl-understood code
 */
void gaim_blist_update_buddy_status(GaimBuddy *buddy, int status);


/**
 * Updates a buddy's presence.
 *
 * @param buddy    The buddy whose presence has changed
 * @param presence The new presence
 */
void gaim_blist_update_buddy_presence(GaimBuddy *buddy, int presence);

/**
 * Updates a buddy's signon time.
 *
 * @param buddy  The buddy whose idle time has changed.
 * @param signon The buddy's signon time since the dawn of the UNIX epoch.
 */
void gaim_blist_update_buddy_signon(GaimBuddy *buddy, time_t signon);

/**
 * Updates a buddy's idle time.
 *
 * @param buddy  The buddy whose idle time has changed
 * @param idle   The buddy's idle time in minutes.
 */
void gaim_blist_update_buddy_idle(GaimBuddy *buddy, int idle);


/**
 * Updates a buddy's warning level.
 *
 * @param buddy   The buddy whose warning level has changed.
 * @param warning The warning level as an int from 0 to 100.
 */
void gaim_blist_update_buddy_evil(GaimBuddy *buddy, int warning);

/**
 * Updates a buddy's icon.
 *
 * @param buddy  The buddy whose buddy icon has changed
 */
void gaim_blist_update_buddy_icon(GaimBuddy *buddy);

/**
 * Renames a buddy in the buddy list.
 *
 * @param buddy  The buddy whose name will be changed.
 * @param name   The new name of the buddy.
 */
void gaim_blist_rename_buddy(GaimBuddy *buddy, const char *name);


/**
 * Aliases a buddy in the buddy list.
 *
 * @param buddy  The buddy whose alias will be changed.
 * @param alias  The buddy's alias.
 */
void gaim_blist_alias_buddy(GaimBuddy *buddy, const char *alias);

/**
 * Sets the server-sent alias of a buddy in the buddy list.
 *
 * @param buddy  The buddy whose alias will be changed.
 * @param alias  The buddy's "official" alias.
 */
void gaim_blist_server_alias_buddy(GaimBuddy *buddy, const char *alias);

/**
 * Aliases a chat in the buddy list.
 *
 * @param chat  The chat whose alias will be changed.
 * @param alias The chat's new alias.
 */
void gaim_blist_alias_chat(GaimChat *chat, const char *alias);

/**
 * Renames a group
 *
 * @param group  The group to rename
 * @param name   The new name
 */
void gaim_blist_rename_group(GaimGroup *group, const char *name);

/**
 * Creates a new chat for the buddy list
 *
 * @param account    The account this chat will get added to
 * @param alias      The alias of the new chat
 * @param components The info the prpl needs to join the chat
 * @return           A newly allocated chat
 */
GaimChat *gaim_chat_new(GaimAccount *account, const char *alias, GHashTable *components);

/**
 * Gets the alias of the chat, or the chat name if the alias does not exist
 *
 * @param chat    The chat
 * @return        The display name of the chat
 */
char *gaim_chat_get_display_name(GaimChat *chat);

/**
 * Adds a new chat to the buddy list.
 *
 * The chat will be inserted right after node or appended to the end
 * of group if node is NULL.  If both are NULL, the buddy will be added to
 * the "Chats" group.
 *
 * @param chat  The new chat who gets added
 * @param group  The group to add the new chat to.
 * @param node   The insertion point
 */
void gaim_blist_add_chat(GaimChat *chat, GaimGroup *group, GaimBlistNode *node);

/**
 * Creates a new buddy
 *
 * @param account    The account this buddy will get added to
 * @param screenname The screenname of the new buddy
 * @param alias      The alias of the new buddy (or NULL if unaliased)
 * @return           A newly allocated buddy
 */
GaimBuddy *gaim_buddy_new(GaimAccount *account, const char *screenname, const char *alias);

/**
 * Sets a buddy's icon.
 *
 * This should only be called from within Gaim. You probably want to
 * call gaim_buddy_icon_set_data().
 *
 * @param buddy The buddy.
 * @param icon  The buddy icon.
 *
 * @see gaim_buddy_icon_set_data()
 */
void gaim_buddy_set_icon(GaimBuddy *buddy, GaimBuddyIcon *icon);

/**
 * Returns a buddy's icon.
 *
 * @param buddy The buddy.
 *
 * @return The buddy icon.
 */
GaimBuddyIcon *gaim_buddy_get_icon(const GaimBuddy *buddy);

/**
 * Returns a buddy's contact.
 *
 * @param buddy The buddy.
 *
 * @return The buddy's contact.
 */
GaimContact *gaim_buddy_get_contact(GaimBuddy *buddy);

/**
 * Adds a new buddy to the buddy list.
 *
 * The buddy will be inserted right after node or prepended to the
 * group if node is NULL.  If both are NULL, the buddy will be added to
 * the "Buddies" group.
 *
 * @param buddy   The new buddy who gets added
 * @param contact The optional contact to place the buddy in.
 * @param group   The group to add the new buddy to.
 * @param node    The insertion point
 */
void gaim_blist_add_buddy(GaimBuddy *buddy, GaimContact *contact, GaimGroup *group, GaimBlistNode *node);

/**
 * Creates a new group
 *
 * You can't have more than one group with the same name.  Sorry.  If you pass
 * this the * name of a group that already exists, it will return that group.
 *
 * @param name   The name of the new group
 * @return       A new group struct
*/
GaimGroup *gaim_group_new(const char *name);

/**
 * Adds a new group to the buddy list.
 *
 * The new group will be inserted after insert or prepended to the list if
 * node is NULL.
 *
 * @param group  The group
 * @param node   The insertion point
 */
void gaim_blist_add_group(GaimGroup *group, GaimBlistNode *node);

/**
 * Creates a new contact
 *
 * @return       A new contact struct
 */
GaimContact *gaim_contact_new();

/**
 * Adds a new contact to the buddy list.
 *
 * The new contact will be inserted after insert or prepended to the list if
 * node is NULL.
 *
 * @param contact The contact
 * @param group   The group to add the contact to
 * @param node    The insertion point
 */
void gaim_blist_add_contact(GaimContact *contact, GaimGroup *group, GaimBlistNode *node);

/**
 * Merges two contacts
 *
 * All of the buddies from source will be moved to target
 *
 * @param source  The contact to merge
 * @param node    The place to merge to (a buddy or contact)
 */
void gaim_blist_merge_contact(GaimContact *source, GaimBlistNode *node);

/**
 * Returns the highest priority buddy for a given contact.
 *
 * @param contact  The contact
 * @return The highest priority buddy
 */
GaimBuddy *gaim_contact_get_priority_buddy(GaimContact *contact);

/**
 * Sets the alias for a contact.
 *
 * @param contact  The contact
 * @param alias    The alias to set, or NULL to unset
 */
void gaim_contact_set_alias(GaimContact *contact, const char *alias);

/**
 * Gets the alias for a contact.
 *
 * @param contact  The contact
 * @return  The alias, or NULL if it is not set.
 */
const char *gaim_contact_get_alias(GaimContact *contact);

/**
 * Removes a buddy from the buddy list and frees the memory allocated to it.
 *
 * @param buddy   The buddy to be removed
 */
void gaim_blist_remove_buddy(GaimBuddy *buddy);

/**
 * Removes a contact, and any buddies it contains, and frees the memory
 * allocated to it.
 *
 * @param contact The contact to be removed
 */
void gaim_blist_remove_contact(GaimContact *contact);

/**
 * Removes a chat from the buddy list and frees the memory allocated to it.
 *
 * @param chat   The chat to be removed
 */
void gaim_blist_remove_chat(GaimChat *chat);

/**
 * Removes a group from the buddy list and frees the memory allocated to it and to
 * its children
 *
 * @param group   The group to be removed
 */
void gaim_blist_remove_group(GaimGroup *group);

/**
 * Returns the alias of a buddy.
 *
 * @param buddy   The buddy whose name will be returned.
 * @return        The alias (if set), server alias (if option is set), or NULL.
 */
const char *gaim_get_buddy_alias_only(GaimBuddy *buddy);


/**
 * Returns the correct name to display for a buddy.
 *
 * @param buddy   The buddy whose name will be returned.
 * @return        The alias (if set), server alias (if option is set), screenname, or "Unknown"
 */
const char *gaim_get_buddy_alias(GaimBuddy *buddy);

/**
 * Returns the correct name to display for a blist chat.
 *
 * @param chat   The chat whose name will be returned.
 * @return       The alias (if set), or first component value.
 */
const char *gaim_chat_get_name(GaimChat *chat);

/**
 * Finds the buddy struct given a screenname and an account
 *
 * @param name    The buddy's screenname
 * @param account The account this buddy belongs to
 * @return        The buddy or NULL if the buddy does not exist
 */
GaimBuddy *gaim_find_buddy(GaimAccount *account, const char *name);

/**
 * Finds the buddy struct given a screenname, an account, and a group
 *
 * @param name    The buddy's screenname
 * @param account The account this buddy belongs to
 * @param group   The group to look in
 * @return        The buddy or NULL if the buddy does not exist in the group
 */
GaimBuddy *gaim_find_buddy_in_group(GaimAccount *account, const char *name,
		GaimGroup *group);

/**
 * Finds all buddies struct given a screenname and an account
 *
 * @param name    The buddy's screenname
 * @param account The account this buddy belongs to
 *
 * @return        A GSList of buddies (which must be freed), or NULL if the buddy doesn't exist
 */
GSList *gaim_find_buddies(GaimAccount *account, const char *name);


/**
 * Finds a group by name
 *
 * @param name    The groups name
 * @return        The group or NULL if the group does not exist
 */
GaimGroup *gaim_find_group(const char *name);

/**
 * Finds a chat by name.
 *
 * @param account The chat's account.
 * @param name    The chat's name.
 *
 * @return The chat, or @c NULL if the chat does not exist.
 */
GaimChat *gaim_blist_find_chat(GaimAccount *account, const char *name);

/**
 * Returns the group of which the chat is a member.
 *
 * @param chat The chat.
 *
 * @return The parent group, or @c NULL if the chat is not in a group.
 */
GaimGroup *gaim_chat_get_group(GaimChat *chat);

/**
 * Returns the group of which the buddy is a member.
 *
 * @param buddy   The buddy
 * @return        The group or NULL if the buddy is not in a group
 */
GaimGroup *gaim_find_buddys_group(GaimBuddy *buddy);


/**
 * Returns a list of accounts that have buddies in this group
 *
 * @param g The group
 *
 * @return A list of gaim_accounts
 */
GSList *gaim_group_get_accounts(GaimGroup *g);

/**
 * Determines whether an account owns any buddies in a given group
 *
 * @param g       The group to search through.
 * @param account The account.
 *
 * @return TRUE if there are any buddies in the group, or FALSE otherwise.
 */
gboolean gaim_group_on_account(GaimGroup *g, GaimAccount *account);

/**
 * Called when an account gets signed on.  Tells the UI to update all the
 * buddies.
 *
 * @param account   The account
 */
void gaim_blist_add_account(GaimAccount *account);


/**
 * Called when an account gets signed off.  Sets the presence of all the buddies to 0
 * and tells the UI to update them.
 *
 * @param account   The account
 */
void gaim_blist_remove_account(GaimAccount *account);


/**
 * Determines the total size of a group
 *
 * @param group  The group
 * @param offline Count buddies in offline accounts
 * @return The number of buddies in the group
 */
int gaim_blist_get_group_size(GaimGroup *group, gboolean offline);

/**
 * Determines the number of online buddies in a group
 *
 * @param group The group
 * @return The number of online buddies in the group, or 0 if the group is NULL
 */
int gaim_blist_get_group_online_count(GaimGroup *group);

/*@}*/

/****************************************************************************************/
/** @name Buddy list file management API                                                */
/****************************************************************************************/

/*@{*/
/**
 * Saves the buddy list to file
 */
void gaim_blist_save();

/**
 * Parses the toc-style buddy list used in older versions of Gaim and for SSI in toc.c
 *
 * @param account  This is the account that the buddies and groups from config will get added to
 * @param config   This is the toc-style buddy list data
 */
void gaim_blist_parse_toc_buddy_list(GaimAccount *account, char *config);


/**
 * Loads the buddy list from ~/.gaim/blist.xml.
 */
void gaim_blist_load();

/**
 * Requests from the user information needed to add a buddy to the
 * buddy list.
 *
 * @param account  The account the buddy is added to.
 * @param username The username of the buddy.
 * @param group    The name of the group to place the buddy in.
 * @param alias    The optional alias for the buddy.
 */
void gaim_blist_request_add_buddy(GaimAccount *account, const char *username,
								  const char *group, const char *alias);

/**
 * Requests from the user information needed to add a chat to the
 * buddy list.
 *
 * @param account The account the buddy is added to.
 * @param group   The optional group to add the chat to.
 * @param alias   The optional alias for the chat.
 */
void gaim_blist_request_add_chat(GaimAccount *account, GaimGroup *group,
								 const char *alias);

/**
 * Requests from the user information needed to add a group to the
 * buddy list.
 */
void gaim_blist_request_add_group(void);

/**
 * Associates a boolean with a node in the buddy list
 *
 * @param node  The node to associate the data with
 * @param key   The identifier for the data
 * @param value The value to set
 */
void gaim_blist_node_set_bool(GaimBlistNode *node, const char *key, gboolean value);

/**
 * Retrieves a named boolean setting from a node in the buddy list
 *
 * @param node  The node to retrieve the data from
 * @param key   The identifier of the data
 *
 * @return The value, or FALSE if there is no setting
 */
gboolean gaim_blist_node_get_bool(GaimBlistNode *node, const char *key);

/**
 * Associates an integer with a node in the buddy list
 *
 * @param node  The node to associate the data with
 * @param key   The identifier for the data
 * @param value The value to set
 */
void gaim_blist_node_set_int(GaimBlistNode *node, const char *key, int value);

/**
 * Retrieves a named integer setting from a node in the buddy list
 *
 * @param node  The node to retrieve the data from
 * @param key   The identifier of the data
 *
 * @return The value, or 0 if there is no setting
 */
int gaim_blist_node_get_int(GaimBlistNode *node, const char *key);

/**
 * Associates a string with a node in the buddy list
 *
 * @param node  The node to associate the data with
 * @param key   The identifier for the data
 * @param value The value to set
 */
void gaim_blist_node_set_string(GaimBlistNode *node, const char *key,
		const char *value);

/**
 * Retrieves a named string setting from a node in the buddy list
 *
 * @param node  The node to retrieve the data from
 * @param key   The identifier of the data
 *
 * @return The value, or NULL if there is no setting
 */
const char *gaim_blist_node_get_string(GaimBlistNode *node, const char *key);

/**
 * Removes a named setting from a blist node
 *
 * @param node  The node from which to remove the setting
 * @param key   The name of the setting
 */
void gaim_blist_node_remove_setting(GaimBlistNode *node, const char *key);

/**
 * Associates some data with the group in the xml buddy list
 *
 * @param g      The group the data is associated with
 * @param key    The key used to retrieve the data
 * @param value  The data to set
 * @deprecated   Replaced by gaim_blist_node_set_bool(), gaim_blist_node_set_int()
 * 		 and gaim_blist_node_set_string() to enable types and consolidate functionality.
 * 		 This function is scheduled to be removed in the near future.
 */
void gaim_group_set_setting(GaimGroup *g, const char *key, const char *value);

/**
 * Retrieves data from the XML buddy list set by gaim_group_set_setting())
 *
 * @param g      The group to retrieve data from
 * @param key    The key to retrieve the data with
 * @return       The associated data or NULL if no data is associated
 * @deprecated   Replaced by gaim_blist_node_get_bool(), gaim_blist_node_get_int()
 * 		 and gaim_blist_node_get_string() to enable types and consolidate functionality.
 * 		 This function is scheduled to be removed in the near future.
 */
const char *gaim_group_get_setting(GaimGroup *g, const char *key);

/**
 * Associates some data with the chat in the xml buddy list
 *
 * @param c      The chat the data is associated with
 * @param key    The key used to retrieve the data
 * @param value  The data to set
 * @deprecated   Replaced by gaim_blist_node_set_bool(), gaim_blist_node_set_int()
 * 		 and gaim_blist_node_set_string() to enable types and consolidate functionality.
 * 		 This function is scheduled to be removed in the near future.
 */
void gaim_chat_set_setting(GaimChat *c, const char *key, const char *value);

/**
 * Retrieves data from the XML buddy list set by gaim_chat_set_setting())
 *
 * @param c      The chat to retrieve data from
 * @param key    The key to retrieve the data with
 *
 * @return       The associated data or NULL if no data is associated
 * @deprecated   Replaced by gaim_blist_node_get_bool(), gaim_blist_node_get_int()
 * 		 and gaim_blist_node_get_string() to enable types and consolidate functionality.
 * 		 This function is scheduled to be removed in the near future.
 */
const char *gaim_chat_get_setting(GaimChat *c, const char *key);

/**
 * Associates some data with the buddy in the xml buddy list
 *
 * @param b      The buddy the data is associated with
 * @param key    The key used to retrieve the data
 * @param value  The data to set
 * @deprecated   Replaced by gaim_blist_node_set_bool(), gaim_blist_node_set_int()
 * 		 and gaim_blist_node_set_string() to enable types and consolidate functionality.
 * 		 This function is scheduled to be removed in the near future.
 */
void gaim_buddy_set_setting(GaimBuddy *b, const char *key, const char *value);

/**
 * Retrieves data from the XML buddy list set by gaim_buddy_set_setting())
 *
 * @param b      The buddy to retrieve data from
 * @param key    The key to retrieve the data with
 * @return       The associated data or NULL if no data is associated
 * @deprecated   Replaced by gaim_blist_node_get_bool(), gaim_blist_node_get_int()
 * 		 and gaim_blist_node_get_string() to enable types and consolidate functionality.
 * 		 This function is scheduled to be removed in the near future.
 */
const char *gaim_buddy_get_setting(GaimBuddy *b, const char *key);

/*@}*/


/**
 * Retrieves the extended menu items for a buddy list node.
 * @param n	The blist node for which to obtain the extended menu items.
 * @return	list of GaimBlistNodeAction items, as harvested by the
 *		blist-node-extended-menu signal.
 */
GList *gaim_blist_node_get_extended_menu(GaimBlistNode *n);


/**
 * Creates a new GaimBlistNodeAction.
 * @param label		The text label to display for this action.
 * @param callback	The function to be called when the action is used on
 *			a selected GaimBlistNode.
 * @param data		Additional data, to be passed to the callback
 */
GaimBlistNodeAction  *gaim_blist_node_action_new(char *label,
		void (*callback)(GaimBlistNode *, gpointer), gpointer data);


/**************************************************************************/
/** @name UI Registration Functions                                       */
/**************************************************************************/
/*@{*/

/**
 * Sets the UI operations structure to be used for the buddy list.
 *
 * @param ops The ops struct.
 */
void gaim_blist_set_ui_ops(GaimBlistUiOps *ops);

/**
 * Returns the UI operations structure to be used for the buddy list.
 *
 * @return The UI operations structure.
 */
GaimBlistUiOps *gaim_blist_get_ui_ops(void);

/*@}*/

/**************************************************************************/
/** @name Buddy List Subsystem                                            */
/**************************************************************************/
/*@{*/

/**
 * Returns the handle for the buddy list subsystem.
 *
 * @return The buddy list subsystem handle.
 */
void *gaim_blist_get_handle(void);

/**
 * Initializes the buddy list subsystem.
 */
void gaim_blist_init(void);

/**
 * Uninitializes the buddy list subsystem.
 */
void gaim_blist_uninit(void);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* _BLIST_H_ */

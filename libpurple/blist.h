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
#ifndef _PURPLE_BLIST_H_
#define _PURPLE_BLIST_H_

/* I can't believe I let ChipX86 inspire me to write good code. -Sean */

#include <glib.h>

#include "blist-node.h"
#include "buddy.h"
#include "contact.h"
#include "group.h"
#include "chat.h"

/** @copydoc _PurpleBlistUiOps */
typedef struct _PurpleBlistUiOps PurpleBlistUiOps;

/**************************************************************************/
/* Enumerations                                                           */
/**************************************************************************/
#include "account.h"
#include "buddyicon.h"
#include "status.h"

/**************************************************************************/
/* Data Structures                                                        */
/**************************************************************************/

/**
 * Buddy list UI operations.
 *
 * Any UI representing a buddy list must assign a filled-out PurpleBlistUiOps
 * structure to the buddy list core.
 */
struct _PurpleBlistUiOps
{
	void (*new_list)(); /**< Sets UI-specific data on a buddy list. */
	void (*new_node)(PurpleBlistNode *node); /**< Sets UI-specific data on a node. */
	void (*show)();     /**< The core will call this when it's finished doing its core stuff */
	void (*update)(PurpleBlistNode *node);       /**< This will update a node in the buddy list. */
	void (*remove)(PurpleBlistNode *node);       /**< This removes a node from the list */
	void (*destroy)();  /**< When the list is destroyed, this is called to destroy the UI. */
	void (*set_visible)(gboolean show);            /**< Hides or unhides the buddy list */
	void (*request_add_buddy)(PurpleAccount *account, const char *username,
							  const char *group, const char *alias);
	void (*request_add_chat)(PurpleAccount *account, PurpleGroup *group,
							 const char *alias, const char *name);
	void (*request_add_group)(void);

	/**
	 * This is called when a node has been modified and should be saved.
	 *
	 * Implementation of this UI op is OPTIONAL. If not implemented, it will
	 * be set to a fallback function that saves data to blist.xml like in
	 * previous libpurple versions.
	 *
	 * @attrib node    The node which has been modified.
	 *
	 * @since 2.6.0.
	 */
	void (*save_node)(PurpleBlistNode *node);

	/**
	 * Called when a node is about to be removed from the buddy list.
	 * The UI op should update the relevant data structures to remove this
	 * node (for example, removing a buddy from the group this node is in).
	 *
	 * Implementation of this UI op is OPTIONAL. If not implemented, it will
	 * be set to a fallback function that saves data to blist.xml like in
	 * previous libpurple versions.
	 *
	 * @attrib node  The node which has been modified.
	 * @since 2.6.0.
	 */
	void (*remove_node)(PurpleBlistNode *node);

	/**
	 * Called to save all the data for an account. If the UI sets this,
	 * the callback must save the privacy and buddy list data for an account.
	 * If the account is NULL, save the data for all accounts.
	 *
	 * Implementation of this UI op is OPTIONAL. If not implemented, it will
	 * be set to a fallback function that saves data to blist.xml like in
	 * previous libpurple versions.
	 *
	 * @attrib account  The account whose data to save. If NULL, save all data
	 *                  for all accounts.
	 * @since 2.6.0.
	 */
	void (*save_account)(PurpleAccount *account);

	void (*_purple_reserved1)(void);
};

void parse_setting(PurpleBlistNode *node, xmlnode *setting);
void value_to_xmlnode(gpointer key, gpointer hvalue, gpointer user_data);
gboolean purple_strings_are_different(const char *one, const char *two);

/* The global static members */
struct _list_account_buddies {
	GSList *list;
	PurpleAccount *account;
};

struct _purple_hbuddy {
	char *name;
	PurpleAccount *account;
	PurpleBlistNode *group;
};


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/** @name Buddy List API                                                  */
/**************************************************************************/
/*@{*/

/**
 * Returns the root node of the main buddy list.
 *
 * @return The root node.
 */
PurpleBlistNode *purple_blist_get_root(void);

/**
 * Returns a list of every buddy in the list.  Use of this function is
 * discouraged if you do not actually need every buddy in the list.  Use
 * purple_find_buddies instead.
 *
 * @return A list of every buddy in the list. Caller is responsible for
 *         freeing the list.
 *
 * @see purple_find_buddies
 * @since 2.6.0
 */
GSList *purple_blist_get_buddies(void);

/**
 * Returns the UI data for the list.
 *
 * @return The UI data for the list.
 *
 * @since 2.6.0
 */
gpointer purple_blist_get_ui_data(void);

/**
 * Sets the UI data for the list.
 *
 * @param ui_data The UI data for the list.
 *
 * @since 2.6.0
 */
void purple_blist_set_ui_data(gpointer ui_data);

/**
 * Shows the buddy list, creating a new one if necessary.
 */
void purple_blist_show(void);


/**
 * Destroys the buddy list window.
 *
 * @deprecated The UI is responsible for cleaning up the
 *             PurpleBuddyList->ui_data. purple_blist_uninit() will free the
 *             PurpleBuddyList* itself.
 */
void purple_blist_destroy(void);

/**
 * Hides or unhides the buddy list.
 *
 * @param show   Whether or not to show the buddy list
 */
void purple_blist_set_visible(gboolean show);

/**
 * Updates a buddy's status.
 *
 * This should only be called from within Purple.
 *
 * @param buddy      The buddy whose status has changed.
 * @param old_status The status from which we are changing.
 */
void purple_blist_update_buddy_status(PurpleBuddy *buddy, PurpleStatus *old_status);

/**
 * Updates a node's custom icon.
 *
 * @param node  The PurpleBlistNode whose custom icon has changed.
 *
 * @since 2.5.0
 */
void purple_blist_update_node_icon(PurpleBlistNode *node);

#if !(defined PURPLE_DISABLE_DEPRECATED) || (defined _PURPLE_BUDDY_C_)
/**
 * Updates a buddy's icon.
 *
 * @param buddy  The buddy whose buddy icon has changed
 * @deprecated Use purple_blist_update_node_icon() instead.
 */
void purple_blist_update_buddy_icon(PurpleBuddy *buddy);
#endif

/**
 * Aliases a contact in the buddy list.
 *
 * @param contact The contact whose alias will be changed.
 * @param alias   The contact's alias.
 */
void purple_blist_alias_contact(PurpleContact *contact, const char *alias);

/**
 * Sets the server-sent alias of a buddy in the buddy list.
 * PRPLs should call serv_got_alias() instead of this.
 *
 * @param buddy  The buddy whose alias will be changed.
 * @param alias  The buddy's "official" alias.
 */
void purple_blist_server_alias_buddy(PurpleBuddy *buddy, const char *alias);

/**
 * Renames a group
 *
 * @param group  The group to rename
 * @param name   The new name
 */
void purple_blist_rename_group(PurpleGroup *group, const char *name);

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
void purple_blist_add_chat(PurpleChat *chat, PurpleGroup *group, PurpleBlistNode *node);

/**
 *
 * THIS METHOD IS BROKEN AND WILL SOON BE REMOVED
 * IT'S ONLY AROUND TO LOOK AT THE LOGIC DURING THE
 * REWRITE
 *
 * Adds a new buddy to the buddy list.
 *
 * The buddy will be inserted right after node or prepended to the
 * group if node is NULL.  If both are NULL, the buddy will be added to
 * the "Buddies" group.
 *
 * @param buddy   The new buddy who gets added
 * @param contact The optional contact to place the buddy in.
 * @param group   The group to add the new buddy to.
 * @param node    The insertion point.  Pass in NULL to add the node as
 *                the first child in the given group.
 * @deprecated
 */
void purple_blist_add_buddy(PurpleBuddy *buddy, PurpleContact *contact, PurpleGroup *group, PurpleBlistNode *node);

/**
 * Finds the buddy struct given a name and an account
 *
 * @param account The account this buddy belongs to
 * @param name    The buddy's name
 * @return        The buddy or NULL if the buddy does not exist
 */
PurpleBuddy *purple_find_buddy(PurpleAccount *account, const char *name);

/**
 * Finds the buddy struct given a name, an account, and a group
 *
 * @param account The account this buddy belongs to
 * @param name    The buddy's name
 * @param group   The group to look in
 * @return        The buddy or NULL if the buddy does not exist in the group
 */
PurpleBuddy *purple_find_buddy_in_group(PurpleAccount *account, const char *name,
		PurpleGroup *group);

/**
 * Finds all PurpleBuddy structs given a name and an account
 *
 * @param account The account this buddy belongs to
 * @param name    The buddy's name (or NULL to return all buddies for the account)
 *
 * @return        A GSList of buddies (which must be freed), or NULL if the buddy doesn't exist
 */
GSList *purple_find_buddies(PurpleAccount *account, const char *name);


/**
 * Finds a group by name
 *
 * @param name    The group's name
 * @return        The group or NULL if the group does not exist
 */
PurpleGroup *purple_find_group(const char *name);

/**
 * Finds a chat by name.
 *
 * @param account The chat's account.
 * @param name    The chat's name.
 *
 * @return The chat, or @c NULL if the chat does not exist.
 */
PurpleChat *purple_blist_find_chat(PurpleAccount *account, const char *name);

/**
 * Called when an account connects.  Tells the UI to update all the
 * buddies.
 *
 * @param account   The account
 */
void purple_blist_add_account(PurpleAccount *account);

/**
 * Called when an account disconnects.  Sets the presence of all the buddies to 0
 * and tells the UI to update them.
 *
 * @param account   The account
 */
void purple_blist_remove_account(PurpleAccount *account);

/*@}*/

/****************************************************************************************/
/** @name Buddy list file management API                                                */
/****************************************************************************************/

/**
 * Loads the buddy list from ~/.purple/blist.xml.
 */
void purple_blist_load(void);

/**
 * Schedule a save of the blist.xml file.  This is used by the privacy
 * API whenever the privacy settings are changed.  If you make a change
 * to blist.xml using one of the functions in the buddy list API, then
 * the buddy list is saved automatically, so you should not need to
 * call this.
 */
void purple_blist_schedule_save(void);

/**
 * Requests from the user information needed to add a buddy to the
 * buddy list.
 *
 * @param account  The account the buddy is added to.
 * @param username The username of the buddy.
 * @param group    The name of the group to place the buddy in.
 * @param alias    The optional alias for the buddy.
 */
void purple_blist_request_add_buddy(PurpleAccount *account, const char *username,
								  const char *group, const char *alias);

/**
 * Requests from the user information needed to add a chat to the
 * buddy list.
 *
 * @param account The account the buddy is added to.
 * @param group   The optional group to add the chat to.
 * @param alias   The optional alias for the chat.
 * @param name    The required chat name.
 */
void purple_blist_request_add_chat(PurpleAccount *account, PurpleGroup *group,
								 const char *alias, const char *name);

/**
 * Requests from the user information needed to add a group to the
 * buddy list.
 */
void purple_blist_request_add_group(void);

/**************************************************************************/
/** @name UI Registration Functions                                       */
/**************************************************************************/
/*@{*/

/**
 * Sets the UI operations structure to be used for the buddy list.
 *
 * @param ops The ops struct.
 */
void purple_blist_set_ui_ops(PurpleBlistUiOps *ops);

/**
 * Returns the UI operations structure to be used for the buddy list.
 *
 * @return The UI operations structure.
 */
PurpleBlistUiOps *purple_blist_get_ui_ops(void);

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
void *purple_blist_get_handle(void);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* _PURPLE_BLIST_H_ */

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
#ifndef _PURPLE_GROUP_H_
#define _PURPLE_GROUP_H_

#include <glib.h>

/** @copydoc _PurpleGroup */
typedef struct _PurpleGroup PurpleGroup;
typedef struct _PurpleGroupPrivate PurpleGroupPrivate;
typedef struct _PurpleGroupClass PurpleGroupClass;

#include "buddy.h"
#include "chat.h"
#include "contact.h"
#include "blist-node.h"

#define PURPLE_GROUP_TYPE                  (purple_group_get_type ())
#define PURPLE_GROUP(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), PURPLE_GROUP_TYPE, PurpleGroup))
#define PURPLE_IS_GROUP(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PURPLE_GROUP_TYPE))
#define PURPLE_GROUP_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), PURPLE_GROUP_TYPE, PurpleGroupClass))
#define PURPLE_IS_GROUP_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), PURPLE_GROUP_TYPE))
#define PURPLE_GET_GROUP_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), PURPLE_GROUP_TYPE, PurpleGroupClass))

/**
 * A group.  This contains everything Purple will ever need to know about a group.
 */
struct _PurpleGroup {
	PurpleBlistNode node;                    /**< The node that this group inherits from */
	PurpleGroupPrivate *priv;
};

struct _PurpleGroupClass {
	PurpleBlistNodeClass parent;
};

/**
 * Creates a new group
 *
 * You can't have more than one group with the same name.  Sorry.  If you pass
 * this the name of a group that already exists, it will return that group.
 *
 * @param name   The name of the new group
 * @return       A new group struct
*/
PurpleGroup *purple_group_new(const char *name);

/**
 * Destroys a group
 *
 * @param group  The group to destroy
*/
void purple_group_destroy(PurpleGroup *group);

/**
 * Returns a list of accounts that have buddies in this group
 *
 * @param g The group
 *
 * @return A GSList of accounts (which must be freed), or NULL if the group
 *         has no accounts.
 */
GSList *purple_group_get_accounts(PurpleGroup *g);

/**
 * Determines whether an account owns any buddies in a given group
 *
 * @param g       The group to search through.
 * @param account The account.
 *
 * @return TRUE if there are any buddies in the group, or FALSE otherwise.
 */
gboolean purple_group_on_account(PurpleGroup *g, PurpleAccount *account);

/**
 * Set the name of this group
 *
 * @param group the group
 * @param name the name to set it to
 */
void purple_group_set_name(PurpleGroup *group, const char *name);

/**
 * Returns the name of a group.
 *
 * @param group The group.
 *
 * @return The a copy of the group name, caller is responsible for freeing it
 */
char *purple_group_get_name(const PurpleGroup *group);

/**
 * Determines the total size of a group
 *
 * @param group  The group
 * @param offline Count buddies in offline accounts
 * @return The number of buddies in the group
 */
int purple_group_get_size(const PurpleGroup *group, gboolean offline);

void purple_group_set_online(PurpleGroup *group, int online);

/**
 * Determines the number of online buddies in a group
 *
 * @param group The group
 * @return The number of online buddies in the group, or 0 if the group is NULL
 */
int purple_group_get_online(const PurpleGroup *group);

void purple_group_set_currentsize(PurpleGroup *group, int currentsize);

/**
 * Update the counts based on a changed online/offline status of a contact
 *
 * @param group The group to alter
 * @param contact The contact that's changed
 */
void purple_group_contact_updated(PurpleGroup *group, PurpleContact *contact);

/**
 * Get a list of all the buddies in this group
 *
 * @param group The group
 * @return a list of buddies
 */
GList *purple_group_get_buddies(PurpleGroup *group);

/**
 * Get the GType for PurpleGroup
 */
GType purple_group_get_type(void);

#endif

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
#ifndef _PURPLE_BUDDY_H_
#define _PURPLE_BUDDY_H_

#include <glib.h>

/** @copydoc _PurpleBuddy */
typedef struct _PurpleBuddy PurpleBuddy;
typedef struct _PurpleBuddyPrivate PurpleBuddyPrivate;
typedef struct _PurpleBuddyClass PurpleBuddyClass;

#include "buddyicon.h"
#include "status.h"

#define PURPLE_BUDDY_TYPE                  (purple_buddy_get_type ())
#define PURPLE_BUDDY(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), PURPLE_BUDDY_TYPE, PurpleBuddy))
#define PURPLE_IS_BUDDY(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PURPLE_BUDDY_TYPE))
#define PURPLE_BUDDY_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), PURPLE_BUDDY_TYPE, PurpleBuddyClass))
#define PURPLE_IS_BUDDY_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), PURPLE_BUDDY_TYPE))
#define PURPLE_GET_BUDDY_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), PURPLE_BUDDY_TYPE, PurpleBuddyClass))
#define PURPLE_BUDDY_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), PURPLE_BUDDY_TYPE, PurpleBuddyPrivate))

#if !(defined PURPLE_HIDE_STRUCTS) || (defined _PURPLE_BUDDY_C_)
/**
 * A buddy.  This contains everything Purple will ever need to know about someone on the buddy list.  Everything.
 */
struct _PurpleBuddy {
	PurpleBlistNode node;                     /**< The node that this buddy inherits from */
	PurpleBuddyPrivate *priv;               /**< The private members */
};

struct _PurpleBuddyClass {
	PurpleBlistNodeClass parent;
};
#endif

/**
 * Creates a new buddy.
 *
 * This function only creates the PurpleBuddy. Use purple_blist_add_buddy
 * to add the buddy to the list and purple_account_add_buddy to sync up
 * with the server.
 *
 * @param account    The account this buddy will get added to
 * @param name       The name of the new buddy
 * @param alias      The alias of the new buddy (or NULL if unaliased)
 * @return           A newly allocated buddy
 *
 * @see purple_account_add_buddy
 * @see purple_blist_add_buddy
 */
PurpleBuddy *purple_buddy_new(PurpleAccount *account, const char *name, const char *alias);

/**
 * Destroys a buddy
 *
 * @param buddy     The buddy to destroy
 */
void purple_buddy_destroy(PurpleBuddy *buddy);

/**
 * Sets a buddy's icon.
 *
 * This should only be called from within Purple. You probably want to
 * call purple_buddy_icon_set_data().
 *
 * @param buddy The buddy.
 * @param icon  The buddy icon.
 *
 * @see purple_buddy_icon_set_data()
 */
void purple_buddy_set_icon(PurpleBuddy *buddy, PurpleBuddyIcon *icon);

/**
 * Gets the server alias for a buddy.
 *
 * @param buddy  The buddy whose name will be returned
 * @return  The server alias, or NULL if it is not set.
 */
const char *purple_buddy_get_server_alias(PurpleBuddy *buddy);

/**
 * Returns the local alias for the buddy, or @c NULL if none exists.
 *
 * @param buddy  The buddy
 * @return       The local alias for the buddy
 *
 * @since 2.6.0
 */
const char *purple_buddy_get_local_buddy_alias(PurpleBuddy *buddy);

/**
 * Returns a buddy's account.
 *
 * @param buddy The buddy.
 *
 * @return The account
 */
PurpleAccount *purple_buddy_get_account(const PurpleBuddy *buddy);

/**
 * Returns a buddy's name
 *
 * @param buddy The buddy.
 *
 * @return The name.
 */
const char *purple_buddy_get_name(const PurpleBuddy *buddy);

/**
 * Set the name of the buddy, frees existing name if not NULL.
 *
 * @param buddy The buddy
 * @param name The name
 */
void purple_buddy_set_name(PurpleBuddy *buddy, const char *name);

/**
 * Returns a buddy's icon.
 *
 * @param buddy The buddy.
 *
 * @return The buddy icon.
 */
PurpleBuddyIcon *purple_buddy_get_icon(const PurpleBuddy *buddy);

/**
 * Returns a buddy's protocol-specific data.
 *
 * This should only be called from the associated prpl.
 *
 * @param buddy The buddy.
 * @return      The protocol data.
 *
 * @see purple_buddy_set_protocol_data()
 * @since 2.6.0
 */
gpointer purple_buddy_get_protocol_data(const PurpleBuddy *buddy);

/**
 * Aliases a buddy in the buddy list.
 *
 * @param buddy  The buddy whose alias will be changed.
 * @param alias  The buddy's alias.
 */
void purple_buddy_set_alias(PurpleBuddy *buddy, const char *alias);

/**
* Returns the alias of a buddy.
*
* @param buddy   The buddy whose name will be returned.
* @return        The alias (if set), server alias (if set),
*                or NULL.
*/
const char *purple_buddy_get_alias_only(PurpleBuddy *buddy);

/**
 * Sets a buddy's protocol-specific data.
 *
 * This should only be called from the associated prpl.
 *
 * @param buddy The buddy.
 * @param data  The data.
 *
 * @see purple_buddy_get_protocol_data()
 * @since 2.6.0
 */
void purple_buddy_set_protocol_data(PurpleBuddy *buddy, gpointer data);

/**
 * Returns a buddy's presence.
 *
 * @param buddy The buddy.
 *
 * @return The buddy's presence.
 */
PurplePresence *purple_buddy_get_presence(const PurpleBuddy *buddy);

/**
 * Get the GType for PurpleBuddy
 */
GType purple_buddy_get_type(void);

#endif

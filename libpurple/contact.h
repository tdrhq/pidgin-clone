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
#ifndef _PURPLE_CONTACT_H_
#define _PURPLE_CONTACT_H_

#include <glib.h>

/** @copydoc _PurpleContact */
typedef struct _PurpleContact PurpleContact;
typedef struct _PurpleContactPrivate PurpleContactPrivate;
typedef struct _PurpleContactClass PurpleContactClass;

#include "buddy.h"

#define PURPLE_CONTACT_TYPE                  (purple_contact_get_type ())
#define PURPLE_CONTACT(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), PURPLE_CONTACT_TYPE, PurpleContact))
#define PURPLE_IS_CONTACT(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PURPLE_CONTACT_TYPE))
#define PURPLE_CONTACT_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), PURPLE_CONTACT_TYPE, PurpleContactClass))
#define PURPLE_IS_CONTACT_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), PURPLE_CONTACT_TYPE))
#define PURPLE_GET_CONTACT_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), PURPLE_CONTACT_TYPE, PurpleContactClass))

/**
 * A contact.  This contains everything Purple will ever need to know about a contact.
 */
struct _PurpleContact {
	PurpleBlistNode node;		/**< The node that this contact inherits from. */
	PurpleContactPrivate *priv;
};

struct _PurpleContactClass {
	PurpleBlistNodeClass parent;
};

/**
 * Creates a new contact
 *
 * @return       A new contact struct
 */
PurpleContact *purple_contact_new(void);

#if !(defined PURPLE_DISABLE_DEPRECATED) || (defined _PURPLE_BUDDY_C_)
/**
 * Returns the correct alias for this user, ignoring server aliases.  Used
 * when a user-recognizable name is required.  In order: buddy's alias; buddy's
 * contact alias; buddy's user name.
 *
 * @param buddy  The buddy whose alias will be returned.
 * @return       The appropriate name or alias.
 * @deprecated   Try purple_buddy_get_alias(), if server aliases are okay.
 */
const char *purple_buddy_get_local_alias(PurpleBuddy *buddy);
#endif

/**
 * Returns the correct name to display for a buddy. In order of precedence:
 * the buddy's alias; the buddy's server alias; the buddy's contact alias;
 * the buddy's user name.
 *
 * @param buddy   The buddy whose name will be returned.
 * @return        The appropriate name or alias, or NULL
 */
const char *purple_buddy_get_alias(PurpleBuddy *buddy);

/**
 * Destroys a contact
 *
 * @param contact  The contact to destroy
 */
void purple_contact_destroy(PurpleContact *contact);

/**
 * Returns the highest priority buddy for a given contact.
 *
 * @param contact  The contact
 * @return The highest priority buddy
 */
PurpleBuddy *purple_contact_get_priority_buddy(PurpleContact *contact);

#if !(defined PURPLE_DISABLE_DEPRECATED) || (defined _PURPLE_CONTACT_C_)
/**
 * Sets the alias for a contact.
 *
 * @param contact  The contact
 * @param alias    The alias to set, or NULL to unset
 *
 * @deprecated Use purple_blist_alias_contact() instead.
 */
void purple_contact_set_alias(PurpleContact *contact, const char *alias);
#endif

/**
 * Gets the alias for a contact.
 *
 * @param contact  The contact
 * @return  The alias, or NULL if it is not set.
 */
const char *purple_contact_get_alias(PurpleContact *contact);

/**
 * Get a list of buddies for this contact
 *
 * @param contact The contact
 * @return the list of buddies
 */
GList *purple_contact_get_buddies(PurpleContact *contact);

/**
 * Returns a buddy's contact.
 *
 * @param buddy The buddy.
 *
 * @return The buddy's contact.
 */
PurpleContact *purple_buddy_get_contact(PurpleBuddy *buddy);

/* Not sure if these will stick around, so not bothering with
 * docs just yet 
 */
int purple_contact_get_totalsize(PurpleContact *contact);
void purple_contact_set_totalsize(PurpleContact *contact, int value);

/**
 * Returns the number of online buddies for this contact.
 *
 * @param contact The contact
 * @return The # of online buddies
 */
int purple_contact_get_online(PurpleContact *contact);

/**
 * Sets the number of online contacts for this contact
 *
 * @param contact The contact
 * @param value The number to set to
 */
void purple_contact_set_online(PurpleContact *contact, int value);

/**
 * Update the contact based on a buddy status change
 *
 * @param contact The contact
 * @param status The current status of the buddy
 * @param old_status The old status of the buddy
 */
void purple_contact_buddy_status_update(PurpleContact *contact, PurpleStatus *status, PurpleStatus *old_status);

/* See comments for totalsize accessors */
void purple_contact_set_currentsize(PurpleContact *contact, int value);

/**
 * Returns the number of buddies that correspond to online accounts
 *
 * @param contact The contact
 */
int purple_contact_get_currentsize(PurpleContact *contact);

/**
 * Merges two contacts
 *
 * All of the buddies from source will be moved to target
 *
 * @param source  The contact to merge
 * @param node    The place to merge to (a buddy or contact)
 */
void purple_blist_merge_contact(PurpleContact *source, PurpleBlistNode *node);

/**
 * Returns the correct name to display for a buddy, taking the contact alias
 * into account. In order of precedence: the buddy's alias; the buddy's
 * contact alias; the buddy's server alias; the buddy's user name.
 *
 * @param buddy  The buddy whose name will be returned
 * @return       The appropriate name or alias, or NULL.
 *
 */
const char *purple_buddy_get_contact_alias(PurpleBuddy *buddy);

/**
 * Determines whether an account owns any buddies in a given contact
 *
 * @param contact  The contact to search through.
 * @param account  The account.
 *
 * @return TRUE if there are any buddies from account in the contact, or FALSE otherwise.
 */
gboolean purple_contact_on_account(PurpleContact *contact, PurpleAccount *account);

/**
 * Invalidates the priority buddy so that the next call to
 * purple_contact_get_priority_buddy recomputes it.
 *
 * @param contact  The contact
 */
void purple_contact_invalidate_priority_buddy(PurpleContact *contact);

/**
 * Get the GType for PurpleContact
 */
GType purple_contact_get_type(void);

#endif

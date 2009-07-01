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
#ifndef _PURPLE_CHAT_H_
#define _PURPLE_CHAT_H_

#include <glib.h>

/** @copydoc _PurpleChat */
typedef struct _PurpleChat PurpleChat;
typedef struct _PurpleChatClass PurpleChatClass;

#include "blist-node.h"

#define PURPLE_CHAT_TYPE                  (purple_chat_get_type ())
#define PURPLE_CHAT(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), PURPLE_CHAT_TYPE, PurpleChat))
#define PURPLE_IS_CHAT(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PURPLE_CHAT_TYPE))
#define PURPLE_CHAT_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), PURPLE_CHAT_TYPE, PurpleChatClass))
#define PURPLE_IS_CHAT_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), PURPLE_CHAT_TYPE))
#define PURPLE_GET_CHAT_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), PURPLE_CHAT_TYPE, PurpleChatClass))

#if !(defined PURPLE_HIDE_STRUCTS) || (defined _PURPLE_CHAT_C_)

/**
 * A chat.  This contains everything Purple needs to put a chat room in the
 * buddy list.
 */
struct _PurpleChat {
	PurpleBlistNode node;      /**< The node that this chat inherits from */
	char *alias;             /**< The display name of this chat. */
	GHashTable *components;  /**< the stuff the protocol needs to know to join the chat */
	PurpleAccount *account; /**< The account this chat is attached to */
};

struct _PurpleChatClass {
	PurpleBlistNodeClass parent;
};
#endif

/**
 * Creates a new chat for the buddy list
 *
 * @param account    The account this chat will get added to
 * @param alias      The alias of the new chat
 * @param components The info the prpl needs to join the chat.  The
 *                   hash function should be g_str_hash() and the
 *                   equal function should be g_str_equal().
 * @return           A newly allocated chat
 */
PurpleChat *purple_chat_new(PurpleAccount *account, const char *alias, GHashTable *components);

/**
 * Destroys a chat
 *
 * @param chat       The chat to destroy
 */
void purple_chat_destroy(PurpleChat *chat);

/**
 * Returns the correct name to display for a blist chat.
 *
 * @param chat   The chat whose name will be returned.
 * @return       The alias (if set), or first component value.
 */
const char *purple_chat_get_name(PurpleChat *chat);

/**
 * Returns the account the chat belongs to.
 *
 * @param chat  The chat.
 *
 * @return  The account the chat belongs to.
 *
 * @since 2.4.0
 */
PurpleAccount *purple_chat_get_account(PurpleChat *chat);

/**
 * Get a hashtable containing information about a chat.
 *
 * @param chat  The chat.
 *
 * @constreturn  The hashtable.
 *
 * @since 2.4.0
 */
GHashTable *purple_chat_get_components(PurpleChat *chat);

/**
 * Aliases a chat in the buddy list.
 *
 * @param chat  The chat whose alias will be changed.
 * @param alias The chat's new alias.
 */
void purple_chat_set_alias(PurpleChat *chat, const char *alias);

/**
 * Get the GType for PurpleChat
 */
GType purple_chat_get_type(void);

#endif

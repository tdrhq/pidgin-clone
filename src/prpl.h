/**
 * @file prpl.h Protocol Plugin functions
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

/* this file should be all that prpls need to include. therefore, by including
 * this file, they should get glib, proxy, gaim_connection, prpl, etc. */

#ifndef _GAIM_PRPL_H_
#define _GAIM_PRPL_H_

typedef struct _GaimPluginProtocolInfo GaimPluginProtocolInfo;

/**************************************************************************/
/** @name Basic Protocol Information                                      */
/**************************************************************************/
/*@{*/

/** Default protocol plugin description */
#define GAIM_PRPL_DESC(x) \
		"Allows gaim to use the " (x) " protocol.\n\n"      \
		"Now that you have loaded this protocol, use the "  \
		"Account Editor to add an account that uses this "  \
		"protocol. You can access the Account Editor from " \
		"the \"Accounts\" button on the login window or "   \
		"in the \"Tools\" menu in the buddy list window."

/** Default protocol */
#define GAIM_PROTO_DEFAULT "prpl-oscar"

/*@}*/

/**
 * Flags applicable to outgoing/incoming IMs from prpls.
 */
typedef enum
{
	GAIM_CONV_IM_AUTO_RESP = 0x0001,    /**< Auto response.    */
	GAIM_CONV_IM_IMAGES    = 0x0002     /**< Contains images.  */
} GaimConvImFlags;

typedef enum
{
	GAIM_CONV_CHAT_WHISPER = 0x0001,    /**< Whispered message.*/
	GAIM_CONV_CHAT_DELAYED = 0x0002     /**< Delayed message.  */

} GaimConvChatFlags;

typedef enum {
	GAIM_ICON_SCALE_DISPLAY = 0x01,		/**< We scale the icon when we display it */
	GAIM_ICON_SCALE_SEND = 0x02			/**< We scale the icon before we send it to the server */
} GaimIconScaleRules;


/**
 * A description of a Buddy Icon specification.  This tells Gaim what kind of image file
 * it should give this prpl, and what kind of image file it should expect back.
 * Dimensions less than 1 should be ignored and the image not scaled.
 */
typedef struct {
	char *format;                       /**< This is a comma-delimited list of image formats or NULL if icons are not supported. 
					     * The core nor the prpl will actually check to see if the data it's given matches this, it's entirely
					     * up to the UI to do what it wants */
	int min_width;                          /**< The minimum width of this icon  */
	int min_height;                         /**< The minimum height of this icon */
	int max_width;                          /**< The maximum width of this icon  */
	int max_height;                         /**< The maximum height of this icon */
	GaimIconScaleRules scale_rules;		/**< How to stretch this icon */
} GaimBuddyIconSpec;

/* This #define exists just to make it easier to fill out the buddy icon field in he prpl info struct for protocols that couldn't care less. */
#define NO_BUDDY_ICONS {NULL, 0, 0, 0, 0, 0}

#include "blist.h"
#include "proxy.h"
#include "plugin.h"
#include "status.h"

struct proto_chat_entry {
	char *label;
	char *identifier;
	gboolean is_int;
	int min;
	int max;
	gboolean secret;
};

/**
 * Protocol options
 *
 * These should all be stuff that some plugins can do and others can't.
 */
typedef enum
{
	/**
	 * Use a unique name, not an alias, for chat rooms.
	 *
	 * Jabber lets you choose what name you want for chat.
	 * So it shouldn't be pulling the alias for when you're in chat;
	 * it gets annoying.
	 */
	OPT_PROTO_UNIQUE_CHATNAME = 0x00000004,

	/**
	 * Chat rooms have topics.
	 *
	 * IRC and Jabber support this.
	 */
	OPT_PROTO_CHAT_TOPIC = 0x00000008,

	/**
	 * Don't require passwords for sign-in.
	 *
	 * Zephyr doesn't require passwords, so there's no need for
	 * a password prompt.
	 */
	OPT_PROTO_NO_PASSWORD = 0x00000010,

	/**
	 * Notify on new mail.
	 *
	 * MSN and Yahoo notify you when you have new mail.
	 */
	OPT_PROTO_MAIL_CHECK = 0x00000020,

	/**
	 * Buddy icon support.
	 *
	 * Oscar and Jabber have buddy icons.
	 *
	 * *We'll do this a bit more sophisticated like, now.
	 *
	 * OPT_PROTO_BUDDY_ICON = 0x00000040,
	 */

	/**
	 * Images in IMs.
	 *
	 * Oscar lets you send images in direct IMs.
	 */
	OPT_PROTO_IM_IMAGE = 0x00000080,

	/**
	 * Allow passwords to be optional.
	 *
	 * Passwords in IRC are optional, and are needed for certain
	 * functionality.
	 */
	OPT_PROTO_PASSWORD_OPTIONAL = 0x00000100,

	/**
	 * Allows font size to be specified in sane point size
	 *
	 * Probably just Jabber and Y!M
	 */
	OPT_PROTO_USE_POINTSIZE = 0x00000200

} GaimProtocolOptions;

/** Some structs defined in roomlist.h */
struct _GaimRoomlist;
struct _GaimRoomlistRoom;

/**
 * A protocol plugin information structure.
 *
 * Every protocol plugin initializes this structure. It is the gateway
 * between gaim and the protocol plugin.
 */
struct _GaimPluginProtocolInfo
{
	GaimProtocolOptions options;  /**< Protocol options.          */

	GList *user_splits;      /* A GList of GaimAccountUserSplit */
	GList *protocol_options; /* A GList of GaimAccountOption    */
	
	GaimBuddyIconSpec icon_spec; /* The icon spec. */
	
	/**
	 * Returns the base icon name for the given buddy and account.
	 * If buddy is NULL, it will return the name to use for the account's icon
	 */
	const char *(*list_icon)(GaimAccount *account, GaimBuddy *buddy);

	/**
	 * Fills the four char**'s with string identifiers for "emblems"
	 * that the UI will interpret and display as relevant
	 */
	void (*list_emblems)(GaimBuddy *buddy, char **se, char **sw,
						  char **nw, char **ne);

	/**
	 * Gets a short string representing this buddy's status.  This will
	 * be shown on the buddy list.
	 */
	char *(*status_text)(GaimBuddy *buddy);

	/**
	 * Gets a string to put in the buddy list tooltip.
	 */
	char *(*tooltip_text)(GaimBuddy *buddy);

	GList *(*status_types)(GaimAccount *account);

	GList *(*blist_node_menu)(GaimBlistNode *node);
	GList *(*chat_info)(GaimConnection *);
	GHashTable *(*chat_info_defaults)(GaimConnection *, const char *chat_name);

	/* All the server-related functions */
	void (*login)(GaimAccount *);
	void (*close)(GaimConnection *);
	int  (*send_im)(GaimConnection *, const char *who,
					const char *message,
					GaimConvImFlags flags);
	void (*set_info)(GaimConnection *, const char *info);
	int  (*send_typing)(GaimConnection *, const char *name, int typing);
	void (*get_info)(GaimConnection *, const char *who);
	void (*set_status)(GaimAccount *account, GaimStatus *status);

	void (*set_idle)(GaimConnection *, int idletime);
	void (*change_passwd)(GaimConnection *, const char *old_pass,
						  const char *new_pass);
	void (*add_buddy)(GaimConnection *, GaimBuddy *buddy, GaimGroup *group);
	void (*add_buddies)(GaimConnection *, GList *buddies, GList *groups);
	void (*remove_buddy)(GaimConnection *, GaimBuddy *buddy, GaimGroup *group);
	void (*remove_buddies)(GaimConnection *, GList *buddies, GList *groups);
	void (*add_permit)(GaimConnection *, const char *name);
	void (*add_deny)(GaimConnection *, const char *name);
	void (*rem_permit)(GaimConnection *, const char *name);
	void (*rem_deny)(GaimConnection *, const char *name);
	void (*set_permit_deny)(GaimConnection *);
	void (*warn)(GaimConnection *, const char *who, gboolean anonymous);
	void (*join_chat)(GaimConnection *, GHashTable *components);
	void (*reject_chat)(GaimConnection *, GHashTable *components);
	char *(*get_chat_name)(GHashTable *components);
	void (*chat_invite)(GaimConnection *, int id,
						const char *who, const char *message);
	void (*chat_leave)(GaimConnection *, int id);
	void (*chat_whisper)(GaimConnection *, int id,
						 const char *who, const char *message);
	int  (*chat_send)(GaimConnection *, int id, const char *message);
	void (*keepalive)(GaimConnection *);

	/* new user registration */
	void (*register_user)(GaimAccount *);

	/* get "chat buddy" info and away message */
	void (*get_cb_info)(GaimConnection *, int, const char *who);
	void (*get_cb_away)(GaimConnection *, int, const char *who);

	/* save/store buddy's alias on server list/roster */
	void (*alias_buddy)(GaimConnection *, const char *who,
						const char *alias);

	/* change a buddy's group on a server list/roster */
	void (*group_buddy)(GaimConnection *, const char *who,
						const char *old_group, const char *new_group);

	/* rename a group on a server list/roster */
	void (*rename_group)(GaimConnection *, const char *old_name,
						 GaimGroup *group, GList *moved_buddies);

	void (*buddy_free)(GaimBuddy *);

	void (*convo_closed)(GaimConnection *, const char *who);

	const char *(*normalize)(const GaimAccount *, const char *);

	void (*set_buddy_icon)(GaimConnection *, const char *filename);

	void (*remove_group)(GaimConnection *gc, GaimGroup *group);

	char *(*get_cb_real_name)(GaimConnection *gc, int id, const char *who);

	void (*set_chat_topic)(GaimConnection *gc, int id, const char *topic);

	GaimChat *(*find_blist_chat)(GaimAccount *account, const char *name);

	/* room listing prpl callbacks */
	struct _GaimRoomlist *(*roomlist_get_list)(GaimConnection *gc);
	void (*roomlist_cancel)(struct _GaimRoomlist *list);
	void (*roomlist_expand_category)(struct _GaimRoomlist *list, struct _GaimRoomlistRoom *category);

	/* file transfer callbacks */
	gboolean (*can_receive_file)(GaimConnection *, const char *who);
	void (*send_file)(GaimConnection *, const char *who, const char *filename);
};

#define GAIM_IS_PROTOCOL_PLUGIN(plugin) \
	((plugin)->info->type == GAIM_PLUGIN_PROTOCOL)

#define GAIM_PLUGIN_PROTOCOL_INFO(plugin) \
	((GaimPluginProtocolInfo *)(plugin)->info->extra_info)

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/** @name Protocol Plugin API                                             */
/**************************************************************************/
/*@{*/

/**
 * Notifies Gaim that an account's idle state and time have changed.
 *
 * This is meant to be called from protocol plugins.
 *
 * @param account   The account.
 * @param idle      The user's idle state.
 * @param idle_time The user's idle time.
 */
void gaim_prpl_got_account_idle(GaimAccount *account, gboolean idle,
								time_t idle_time);

/**
 * Notifies Gaim of an account's log-in time.
 *
 * This is meant to be called from protocol plugins.
 *
 * @param account    The account the user is on.
 * @param login_time The user's log-in time.
 */
void gaim_prpl_got_account_login_time(GaimAccount *account, const char *name,
									  time_t login_time);

/**
 * Notifies Gaim that an account's status has changed.
 *
 * This is meant to be called from protocol plugins.
 *
 * @param account   The account the user is on.
 * @param status_id The status ID.
 * @param attr_id   The first attribute ID, or NULL for no attribute updates.
 * @param ...       A NULL-terminated list of attribute IDs and values,
 *                  beginning with the value for @a attr_id.
 */
void gaim_prpl_got_account_status(GaimAccount *account,
								  const char *status_id, const char *attr_id,
								  ...);

/**
 * Notifies Gaim that an account's warning level has changed.
 *
 * This is meant to be called from protocol plugins.
 *
 * @param account  The account the user is on.
 * @param username The user that warned the account.
 * @param level    The new warning level.
 */
void gaim_prpl_got_account_warning_level(GaimAccount *account,
										 const char *username,
										 unsigned int level);


/**
 * Notifies Gaim that a user's idle state and time have changed.
 *
 * This is meant to be called from protocol plugins.
 *
 * @param account   The account the user is on.
 * @param name      The screen name of the user.
 * @param idle      The user's idle state.
 * @param idle_time The user's idle time.
 */
void gaim_prpl_got_user_idle(GaimAccount *account, const char *name,
							 gboolean idle, time_t idle_time);

/**
 * Notifies Gaim of a user's log-in time.
 *
 * This is meant to be called from protocol plugins.
 *
 * @param account    The account the user is on.
 * @param name       The screen name of the user.
 * @param login_time The user's log-in time.
 */
void gaim_prpl_got_user_login_time(GaimAccount *account, const char *name,
								   time_t login_time);

/**
 * Notifies Gaim that a user's status has changed.
 *
 * This is meant to be called from protocol plugins.
 *
 * @param account   The account the user is on.
 * @param name      The screen name of the user.
 * @param status_id The status ID.
 * @param attr_id   The first attribute ID, or NULL for no attribute updates.
 * @param ...       A NULL-terminated list of attribute IDs and values,
 *                  beginning with the value for @a attr_id.
 */
void gaim_prpl_got_user_status(GaimAccount *account, const char *name,
							   const char *status_id, const char *attr_id, ...);

/**
 * Notifies Gaim that a user's warning level has changed.
 *
 * This is meant to be called from protocol plugins.
 *
 * @param account The account the user is on.
 * @param name    The screen name of the user.
 * @param level   The new warning level.
 */
void gaim_prpl_got_user_warning_level(GaimAccount *account, const char *name,
									  unsigned int level);

/**
 * Informs the server that an account's status changed.
 *
 * @param account The account the user is on.
 * @param status  The status that was activated, or deactivated
 *                (in the case of independent statuses).
 */
void gaim_prpl_set_account_status(GaimAccount *account, GaimStatus *status);

/*@}*/

/**************************************************************************/
/** @name Protocol Plugin Subsystem API                                   */
/**************************************************************************/
/*@{*/

/**
 * Finds a protocol plugin structure of the specified type.
 *
 * @param id The protocol plugin;
 */
GaimPlugin *gaim_find_prpl(const char *id);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* _PRPL_H_ */

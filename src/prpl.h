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

/**
 * Protocol types and numbers.
 *
 * Do not assume a new protocol number without talking to
 * Rob Flynn or Sean Egan first!
 */
typedef enum
{
	GAIM_PROTO_TOC = 0,     /**< AIM TOC protocol          */
	GAIM_PROTO_OSCAR,       /**< AIM OSCAR protocol        */
	GAIM_PROTO_YAHOO,       /**< Yahoo Messenger protocol  */
	GAIM_PROTO_ICQ,         /**< Outdated ICQ protocol     */
	GAIM_PROTO_MSN,         /**< MSN Messenger protocol    */
	GAIM_PROTO_IRC,         /**< IRC protocol              */
	GAIM_PROTO_FTP,         /**< FTP protocol              */
	GAIM_PROTO_VGATE,       /**< VGATE protocol            */
	GAIM_PROTO_JABBER,      /**< Jabber protocol           */
	GAIM_PROTO_NAPSTER,     /**< Napster/OpenNAP protocol  */
	GAIM_PROTO_ZEPHYR,      /**< MIT Zephyr protocol       */
	GAIM_PROTO_GADUGADU,    /**< Gadu-Gadu protocol        */
	GAIM_PROTO_SAMETIME,    /**< SameTime protocol         */
	GAIM_PROTO_TLEN,        /**< TLEN protocol             */
	GAIM_PROTO_RVP,         /**< RVP protocol              */
	GAIM_PROTO_BACKRUB,     /**< Instant Massager protocol */
	GAIM_PROTO_MOO,         /**< MOO protocol              */
	GAIM_PROTO_ELVIN,       /**< Elvin Protocol            */
	GAIM_PROTO_TREPIA,      /**< Trepia IM protocol        */
	GAIM_PROTO_BATTLENET,   /**< Battle.NET protocol       */
	GAIM_PROTO_SSCP,	/**< SSCP (ConfMgr) protocol   */
	GAIM_PROTO_BLOGGER,	/**< Blogger xml-rpc protocol  */
	GAIM_PROTO_UNTAKEN      /**< Untaken protocol number   */

} GaimProtocol;


#include "blist.h"
#include "proxy.h"
#include "plugin.h"

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

/**
 * Protocol options
 *
 * These should all be stuff that some plugins can do and others can't.
 */
typedef enum
{
	/**
	 * TOC and Oscar send HTML-encoded messages;
	 * most other protocols don't.
	 */
#if 0
	#define OPT_PROTO_HTML            0x00000001 this should be per-connection */
#endif

	/**
	 * Synchronize the time between the local computer and the server.
	 *
	 * TOC and Oscar have signon time, and the server's time needs
	 * to be adjusted to match your computer's time.
	 *
	 * We wouldn't need this if everyone used NTP.
	 */
	OPT_PROTO_CORRECT_TIME = 0x00000002,

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
	 */
	OPT_PROTO_BUDDY_ICON = 0x00000040,

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

/** Custom away message. */
#define GAIM_AWAY_CUSTOM _("Custom")

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

	GList *(*away_states)(GaimConnection *gc);
	GList *(*actions)(GaimConnection *gc);

	GList *(*buddy_menu)(GaimConnection *, const char *);
	GList *(*chat_info)(GaimConnection *);

	/* All the server-related functions */

	/*
	 * A lot of these (like get_dir) are protocol-dependent and should
	 * be removed. ones like set_dir (which is also protocol-dependent)
	 * can stay though because there's a dialog (i.e. the prpl says you
	 * can set your dir info, the ui shows a dialog and needs to call
	 * set_dir in order to set it)
	 */
	void (*login)(GaimAccount *);
	void (*close)(GaimConnection *);
	int  (*send_im)(GaimConnection *, const char *who,
					const char *message,
					GaimConvImFlags flags);
	void (*set_info)(GaimConnection *, const char *info);
	int  (*send_typing)(GaimConnection *, const char *name, int typing);
	void (*get_info)(GaimConnection *, const char *who);
	void (*set_away)(GaimConnection *, const char *state, const char *message);
	void (*get_away)(GaimConnection *, const char *who);
	void (*set_dir)(GaimConnection *, const char *first,
					const char *middle, const char *last,
					const char *maiden, const char *city,
					const char *state, const char *country, int web);
	void (*get_dir)(GaimConnection *, const char *who);
	void (*dir_search)(GaimConnection *, const char *first,
					   const char *middle, const char *last,
					   const char *maiden, const char *city,
					   const char *state, const char *country,
					   const char *email);
	void (*set_idle)(GaimConnection *, int idletime);
	void (*change_passwd)(GaimConnection *, const char *old_pass,
						  const char *new_pass);
	void (*add_buddy)(GaimConnection *, const char *name, GaimGroup *group);
	void (*add_buddies)(GaimConnection *, GList *buddies);
	void (*remove_buddy)(GaimConnection *, const char *name,
						const char *group);
	void (*remove_buddies)(GaimConnection *, GList *buddies,
						   const char *group);
	void (*add_permit)(GaimConnection *, const char *name);
	void (*add_deny)(GaimConnection *, const char *name);
	void (*rem_permit)(GaimConnection *, const char *name);
	void (*rem_deny)(GaimConnection *, const char *name);
	void (*set_permit_deny)(GaimConnection *);
	void (*warn)(GaimConnection *, const char *who, int anonymous);
	void (*join_chat)(GaimConnection *, GHashTable *components);
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
	void (*rename_group)(GaimConnection *, const char *old_group,
						 const char *new_group, GList *members);

	void (*buddy_free)(GaimBuddy *);

	void (*convo_closed)(GaimConnection *, const char *who);

	const char *(*normalize)(const GaimAccount *, const char *);

	void (*set_buddy_icon)(GaimConnection *, const char *filename);

	void (*remove_group)(GaimConnection *gc, const char *group);

	char *(*get_cb_real_name)(GaimConnection *gc, int id, const char *who);

	void (*set_chat_topic)(GaimConnection *gc, int id, const char *topic);

	GaimChat *(*find_blist_chat)(GaimAccount *account, const char *name);

	/* room listing prpl callbacks */
	struct _GaimRoomlist *(*roomlist_get_list)(GaimConnection *gc);
	void (*roomlist_cancel)(struct _GaimRoomlist *list);
	void (*roomlist_expand_catagory)(struct _GaimRoomlist *list, struct _GaimRoomlistRoom *catagory);
};

#define GAIM_IS_PROTOCOL_PLUGIN(plugin) \
	((plugin)->info->type == GAIM_PLUGIN_PROTOCOL)

#define GAIM_PLUGIN_PROTOCOL_INFO(plugin) \
	((GaimPluginProtocolInfo *)(plugin)->info->extra_info)


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns the plugin ID for a protocol number.
 *
 * @param protocol The protocol number.
 *
 * @return The plugin ID for those numbers that support it.
 */
const char *gaim_prpl_num_to_id(GaimProtocol protocol);

/**
 * Returns the plugin number for a protocol ID.
 *
 * @param id The protocol ID.
 *
 * @return The protocol ID for valid protocol plugin IDs.
 */
GaimProtocol gaim_prpl_id_to_num(const char *id);

/**
 * Finds a protocol plugin structure of the specified type.
 *
 * @param type The protocol plugin;
 */
GaimPlugin *gaim_find_prpl(const char *id);

#ifdef __cplusplus
}
#endif

#endif /* _PRPL_H_ */

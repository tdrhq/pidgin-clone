/**
 * purple - Telepathy Protocol Plugin
 *
 * Copyright (C) 2009, Felix Kerekes <sttwister@soc.pidgin.im>
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

#include <glib.h>

#include <telepathy-glib/connection-manager.h>
#include <telepathy-glib/connection.h>
#include <telepathy-glib/channel.h>
#include <telepathy-glib/dbus.h>
#include <telepathy-glib/debug.h>
#include <telepathy-glib/util.h>

#include "internal.h"

#include "accountopt.h"
#include "debug.h"
#include "notify.h"
#include "plugin.h"
#include "version.h"

#define TELEPATHY_ID "prpl-sttwister-telepathy"
#define TELEPATHY_DISPLAY_VERSION "1.0"

#define TELEPATHY_STATUS_ONLINE   "online"
#define TELEPATHY_STATUS_AWAY     "away"
#define TELEPATHY_STATUS_OFFLINE  "offline"

static void
telepathy_input_user_info(PurplePluginAction *action)
{
    	PurpleConnection *gc = (PurpleConnection *)action->context;
	PurpleAccount *acct = purple_connection_get_account(gc);
	purple_debug_info("telepathy", "showing 'Set User Info' dialog for %s\n",
		acct->username);

	purple_account_request_change_user_info(acct);
}

static GList *
telepathy_actions(PurplePlugin *plugin, gpointer context)
{
	PurplePluginAction *action;
    	purple_debug_info("telepathy", "Setting actions...\n");
	action = purple_plugin_action_new(
		_("Set User Info..."), telepathy_input_user_info);
	return g_list_append(NULL, action);
}


static gboolean
telepathy_plugin_load(PurplePlugin *plugin)
{
	purple_debug_info("telepathy", "Loading prpl...\n");

	return TRUE;
}

static const char *
telepathy_list_icon(PurpleAccount *acct, PurpleBuddy *buddy)
{
	return "null";
}

static GList *
telepathy_status_types(PurpleAccount *acct)
{
	GList *types = NULL;
	PurpleStatusType *type;

	purple_debug_info("telepathy", "Returning status types for %s: %s, %s, %s\n",
		acct->username,
		TELEPATHY_STATUS_ONLINE, TELEPATHY_STATUS_AWAY, TELEPATHY_STATUS_OFFLINE);

	type = purple_status_type_new_with_attrs(PURPLE_STATUS_AVAILABLE,
		TELEPATHY_STATUS_ONLINE, NULL, TRUE, TRUE, FALSE,
		"message", _("Message"), purple_value_new(PURPLE_TYPE_STRING),
		NULL);
	types = g_list_prepend(types, type);

	type = purple_status_type_new_with_attrs(PURPLE_STATUS_AWAY,
		TELEPATHY_STATUS_AWAY, NULL, TRUE, TRUE, FALSE,
		"message", _("Message"), purple_value_new(PURPLE_TYPE_STRING),
		NULL);
	types = g_list_prepend(types, type);

	type = purple_status_type_new_with_attrs(PURPLE_STATUS_OFFLINE,
		NULL, NULL, TRUE, TRUE, FALSE,
		"message", _("Message"), purple_value_new(PURPLE_TYPE_STRING),
		NULL);
	types = g_list_prepend(types, type);

	return g_list_reverse(types);
}

static void
telepathy_login(PurpleAccount *acct)
{
	PurpleConnection *gc = purple_account_get_connection(acct);

	purple_debug_info("telepathy", "Logging in as %s\n", acct->username);

	purple_connection_update_progress(gc, _("Connecting"),
		0,   /* which connection step this is */
		2);  /* total number of steps */

	purple_connection_update_progress(gc, _("Connected"),
		1,   /* which connection step this is */
		2);  /* total number of steps */
	purple_connection_set_state(gc, PURPLE_CONNECTED);
}

static void
telepathy_close(PurpleConnection *gc)
{
	purple_debug_info("telepathy", "We're closing, sorry :(\n");
}

static void
telepathy_destroy(PurplePlugin *plugin) {
	purple_debug_info("telepathy", "Shutting down\n");
}

static PurplePluginProtocolInfo telepathy_prpl_info =
{
	OPT_PROTO_CHAT_TOPIC,  /* options */
	NULL,               /* user_splits, initialized in nullprpl_init() */
	NULL,               /* protocol_options, initialized in nullprpl_init() */
	{   /* icon_spec, a PurpleBuddyIconSpec */
	    	"png,jpg,gif",                   /* format */
		0,                               /* min_width */
		0,                               /* min_height */
		128,                             /* max_width */
		128,                             /* max_height */
		10000,                           /* max_filesize */
		PURPLE_ICON_SCALE_DISPLAY,       /* scale_rules */
	},
	telepathy_list_icon,                  /* list_icon */
	NULL,                                /* list_emblem */
	NULL,                /* status_text */
	NULL,               /* tooltip_text */
	telepathy_status_types,               /* status_types */
	NULL,            /* blist_node_menu */
	NULL,                  /* chat_info */
	NULL,         /* chat_info_defaults */
	telepathy_login,                      /* login */
	telepathy_close,                      /* close */
	NULL,                    /* send_im */
	NULL,                   /* set_info */
	NULL,                /* send_typing */
	NULL,                   /* get_info */
	NULL,                 /* set_status */
	NULL,                   /* set_idle */
	NULL,              /* change_passwd */
	NULL,                  /* add_buddy */
	NULL,                /* add_buddies */
	NULL,               /* remove_buddy */
	NULL,             /* remove_buddies */
	NULL,                 /* add_permit */
	NULL,                   /* add_deny */
	NULL,                 /* rem_permit */
	NULL,                   /* rem_deny */
	NULL,            /* set_permit_deny */
	NULL,                  /* join_chat */
	NULL,                /* reject_chat */
	NULL,              /* get_chat_name */
	NULL,                /* chat_invite */
	NULL,                 /* chat_leave */
	NULL,               /* chat_whisper */
	NULL,                  /* chat_send */
	NULL,                                /* keepalive */
	NULL,              /* register_user */
	NULL,                /* get_cb_info */
	NULL,                                /* get_cb_away */
	NULL,                /* alias_buddy */
	NULL,                /* group_buddy */
	NULL,               /* rename_group */
	NULL,                                /* buddy_free */
	NULL,               /* convo_closed */
	NULL,                  /* normalize */
	NULL,             /* set_buddy_icon */
	NULL,               /* remove_group */
	NULL,                                /* get_cb_real_name */
	NULL,             /* set_chat_topic */
	NULL,                                /* find_blist_chat */
	NULL,          /* roomlist_get_list */
	NULL,            /* roomlist_cancel */
	NULL,   /* roomlist_expand_category */
	NULL,           /* can_receive_file */
	NULL,                                /* send_file */
	NULL,                                /* new_xfer */
	NULL,            /* offline_message */
	NULL,                                /* whiteboard_prpl_ops */
	NULL,                                /* send_raw */
	NULL,                                /* roomlist_room_serialize */
	NULL,	                               /* unregister_user */
	NULL,                                /* send_attention */
	NULL,                                /* attention_types */
	sizeof(PurplePluginProtocolInfo),    /* struct_size */
	NULL,                                /* get_account_text_table */
	NULL,                                /* initiate_media */
	NULL,                                /* get_media_caps */
};

static PurplePluginInfo telepathy_info =
{
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_PROTOCOL,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_DEFAULT,

	TELEPATHY_ID,
	"Telepathy",
	TELEPATHY_DISPLAY_VERSION,

	N_("Telepathy Protocol Plugin"),
	N_("Telepathy Protocol Plugin"),
	"Felix Kerekes <sttwister [at] yahoo [dot] com>",                          
	"http://google.com",     

	telepathy_plugin_load,                   
	NULL,                          
	telepathy_destroy,                          

	NULL,                          
	&telepathy_prpl_info,                          
	NULL,                        

	telepathy_actions,                   

	NULL,                          
	NULL,                          
	NULL,                          
	NULL                           
};                               

static void                        
telepathy_init(PurplePlugin *plugin)
{                                  
	PurpleAccountOption *option;

	purple_debug_info("telepathy", "Initing plugin...\n");

	option = purple_account_option_string_new(_("Test option"), "test", "this is default");
	telepathy_prpl_info.protocol_options = g_list_append(telepathy_prpl_info.protocol_options, option);
}

/*
PURPLE_INIT_PLUGIN(telepathy, telepathy_init, telepathy_info)
*/

static void
list_connection_managers_cb (TpConnectionManager * const *cms,
				  gsize n_cms,
				  const GError *error,
				  gpointer user_data,
				  GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "Failed to list connection managers: %s\n", error->message);
		g_error_free((GError*)error);
	}
	else
	{
		purple_debug_info("telepathy", "Got %d connection managers\n", n_cms);
	}
}

G_MODULE_EXPORT gboolean purple_init_plugin(PurplePlugin *plugin); 
G_MODULE_EXPORT gboolean purple_init_plugin(PurplePlugin *plugin)
{
	static TpDBusDaemon *daemon = NULL;
	GError *error = NULL;

	purple_debug_info("telepathy", "Querying telepathy for connectin managers...\n");

	/* the daemon is used to communicate via DBus */
	daemon = tp_dbus_daemon_new(&error);
	purple_debug_info("telepathy", "Querying telepathy for connectin managers...\n");
	if (daemon == NULL)
	{
	    purple_debug_error("telepathy", "Cannot create DBus daemon: %s\n", error->message);
	    g_error_free(error);
	    return FALSE;
	}
	if (daemon != NULL)
	    g_object_unref(daemon);
	return FALSE;

	tp_list_connection_managers(daemon, list_connection_managers_cb, NULL, NULL, NULL);

	
	return TRUE;
}

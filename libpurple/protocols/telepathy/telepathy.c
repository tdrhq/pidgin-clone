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
#include <telepathy-glib/channel.h>
#include <telepathy-glib/dbus.h>

#include "internal.h"

#include "accountopt.h"
#include "debug.h"
#include "notify.h"
#include "plugin.h"
#include "prefs.h"
#include "version.h"

#define TELEPATHY_ID "prpl-telepathy"
#define TELEPATHY_DISPLAY_VERSION "1.0"

#define TELEPATHY_STATUS_ONLINE   "online"
#define TELEPATHY_STATUS_AWAY     "away"
#define TELEPATHY_STATUS_OFFLINE  "offline"

static void *module_handle;
static gchar *module_path;

typedef struct
{
	TpConnectionManager *cm;
	TpConnectionManagerProtocol *protocol;
} telepathy_data;

typedef struct
{
	const gchar *telepathy_name;
	const gchar *dbus_type;
	const gchar *human_name;
} OptionMapping;

static const OptionMapping options[] = {
	{ "server", "s", N_("Server")},
	{ "port", "q", N_("Port")},
	{ "require-encryption", "b", N_("Require Encryption")},
	{ "ident", "s", N_("Ident")},
	{ "fullname", "s", N_("Full Name")},
	{ "stun-server", "s", N_("STUN Server")},
	{ "stun-port", "q", N_("STUN Port")},
	{ NULL, NULL, NULL}
};

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
	return TRUE;
}

static const char *
telepathy_list_icon(PurpleAccount *acct, PurpleBuddy *buddy)
{
	return "telepathy";
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
telepathy_destroy(PurplePlugin *plugin)
{
	purple_debug_info("telepathy", "Shutting down\n");

	/* TODO: Free the PurplePluginInfo */
}

static PurplePluginProtocolInfo telepathy_prpl_info =
{
	0,  /* options */
	NULL,               /* user_splits, initialized in nullprpl_init() */
	NULL,               /* protocol_options, initialized in nullprpl_init() */
	NO_BUDDY_ICONS,
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

/* transform a telepathy parameter name into a user-friendly one */
static gchar*
telepathy_transform_param_name(const gchar* param)
{
	gchar *name;
	int i,len;

	g_return_val_if_fail(param != NULL, NULL);

	name = g_strdup(param);

	/* capitalize first letter */
	name[0] = g_ascii_toupper(name[0]);

	len = strlen(name);
	for (i = 0; i<len; ++i)
	{
	    if (name[i] == '-')
	    {
		name[i] = ' ';
		if (i+1 < len)
		{
		    /* capitalize first letter of each word */
		    name[i+1] = g_ascii_toupper(name[i+1]);
		}
	    }
	}
	return name;
}

/* TODO: Handle "as" types */
static gchar *
get_human_name(const gchar *telepathy_name,
               const gchar *dbus_type,
               PurplePrefType *purple_type_out)
{
	int i;

	/* map the type */
	if (g_strcmp0(dbus_type, "s") == 0)
		*purple_type_out = PURPLE_PREF_STRING;
 	else if (g_strcmp0(dbus_type, "i") == 0 || g_strcmp0(dbus_type, "n") == 0 || g_strcmp0(dbus_type, "q") == 0)
		*purple_type_out = PURPLE_PREF_INT;
 	else if (g_strcmp0(dbus_type, "b") == 0)
		*purple_type_out = PURPLE_PREF_BOOLEAN;
	else
	{
		purple_debug_warning("telepathy", "Unknown DBus signature \"%s\" for option \"%s\"",
			dbus_type, telepathy_name);
		return NULL;
	}

	/* check for a standard specification name */
	for (i = 0; options[i].telepathy_name != NULL; ++i)
	{
		if (g_strcmp0(options[i].telepathy_name, telepathy_name) == 0 &&
			g_strcmp0(options[i].dbus_type, dbus_type) == 0)
			return g_strdup(_(options[i].human_name));
	}

	/* non-standard parameter, beatuify the name a bit */
	return telepathy_transform_param_name(telepathy_name);
}

static int
get_int_value(const GValue *value,
              const gchar *signature)
{
	if (g_strcmp0(signature, "q") == 0)
		return g_value_get_uint(value);
	else if (g_strcmp0(signature, "i") == 0 && g_strcmp0(signature, "n") == 0)
		return g_value_get_int(value);
	else
		return 0;
}

/* add parameters extracted from telepathy as libpurple options */
static void
add_protocol_options(PurplePlugin *plugin,
                     TpConnectionManagerProtocol *protocol)
{
	gchar *signature, *name, *human_name;
	int i;
	PurpleAccountOption *option = NULL;
	PurplePluginProtocolInfo *protocol_info = plugin->info->extra_info;
	PurplePrefType *type;
	TpConnectionManagerParam *param;
	gboolean has_default;

	/* by default, don't prompt for password, we'll unflag this if we get a password parameter */
	protocol_info->options = protocol_info->options | OPT_PROTO_NO_PASSWORD;

	for (i = 0; protocol->params[i].name != NULL; ++i)
	{
		param = &protocol->params[i];

		name = param->name;
		signature = param->dbus_signature;

		/* check for well known parameters */
		if (g_strcmp0(name, "account") == 0 && g_strcmp0(signature, "s") == 0)
		{
			/* is there anything to be done here? */
		}
		else if (g_strcmp0(name, "password") == 0 && g_strcmp0(signature, "s") == 0)
		{
			/* prompt for a password */
		    	protocol_info->options = protocol_info->options ^ OPT_PROTO_NO_PASSWORD;

			/* is the password required? */
			if ((param->flags & TP_CONN_MGR_PARAM_FLAG_REQUIRED) != TP_CONN_MGR_PARAM_FLAG_REQUIRED)
			{
				protocol_info->options = protocol_info->options | OPT_PROTO_PASSWORD_OPTIONAL;	
			}
		}
		else 
		{
			type = g_new(PurplePrefType, 1);
			human_name = get_human_name(name, signature, type);
			has_default = param->flags & TP_CONN_MGR_PARAM_FLAG_HAS_DEFAULT;

			if (*type == PURPLE_PREF_STRING)
			{
				option = purple_account_option_string_new(human_name, param->name, NULL);
				if (has_default)
					purple_account_option_set_default_string(option, g_value_get_string(&param->default_value));
			}
			else if (*type == PURPLE_PREF_INT)
			{
				option = purple_account_option_int_new(human_name, param->name, 0);
				if (has_default)
					purple_account_option_set_default_int(option, get_int_value(&param->default_value, signature));
			}
			else if (*type == PURPLE_PREF_BOOLEAN)
			{
				option = purple_account_option_bool_new(human_name, param->name, FALSE);
				if (has_default)
					purple_account_option_set_default_bool(option, g_value_get_boolean(&param->default_value));
			}

			if (option != NULL)
			{
				protocol_info->protocol_options = g_list_append(protocol_info->protocol_options, option);
			}
		}
	}
}

static gboolean
export_prpl(TpConnectionManager *cm,
            TpConnectionManagerProtocol *protocol)
{
	/* create a plugin struct and copy all the information from the template */
	PurplePlugin *plugin = purple_plugin_new(TRUE, NULL);
	telepathy_data *data = g_new0(telepathy_data, 1);

	plugin->info = g_memdup(&telepathy_info, sizeof(telepathy_info));
	plugin->info->extra_info = g_memdup(&telepathy_prpl_info, sizeof(telepathy_prpl_info));
	plugin->extra = data;

	data->cm = cm;
	data->protocol = protocol;

	/* correct the plugin id and name, everything else can remain the same */
	plugin->info->id = g_strdup_printf("%s-%s-%s", TELEPATHY_ID, tp_connection_manager_get_name(cm), protocol->name);
	plugin->info->name = g_strdup_printf("Telepathy/%s/%s", tp_connection_manager_get_name(cm), protocol->name);

	/* this is needed to tell libpurple that these plugins also reside in the module */
	g_module_open(module_path, G_MODULE_BIND_LOCAL);
	plugin->handle = module_handle;

	purple_debug_info("telepathy", "Exporting prpl %s\n", plugin->info->name);

	/* actually load the plugin */
	if (!purple_plugin_load(plugin))
		return FALSE;
	if (!purple_plugin_register(plugin))
		return FALSE;

	/* add the options */
	add_protocol_options(plugin, protocol);

	/* this is needed because the plugins are only added to the load queue. FIXME perhaps? */
	purple_plugins_probe("");

	return TRUE;
}

static void
list_connection_managers_cb (TpConnectionManager * const *cms,
			     gsize n_cms,
			     const GError *error,
			     gpointer user_data,
			     GObject *weak_object)
{
	int i,j;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Failed to list connection managers: %s\n", error->message);
	}
	else
	{
		/* for each connection manager, query the supported protocols */
		for (i = 0; i<n_cms; ++i)
		{
			for (j = 0; cms[i]->protocols[j] != NULL; ++j)
			{
				/* each protocol from each connection manager will be exported in its own prpl */
				export_prpl(cms[i], (TpConnectionManagerProtocol*)cms[i]->protocols[j]);
			}
		}
	}
}

/* TODO: Check for staticly linked module */
/* TODO: Get rid of the "Telepathy" prpl */
G_MODULE_EXPORT gboolean purple_init_plugin(PurplePlugin *plugin); 
G_MODULE_EXPORT gboolean purple_init_plugin(PurplePlugin *plugin)
{
	static TpDBusDaemon *daemon = NULL;
	GError *error = NULL;

	/* first plugin */
	purple_debug_info("telepathy", "Registering first plugin\n");
	plugin->info = &(telepathy_info);
	if (!purple_plugin_register(plugin))
		return FALSE;
	
	/* keep the module handle and path, we'll use these for all exported prpls */
	module_handle = plugin->handle;
	module_path = plugin->path;

	/* the daemon is used to communicate via DBus */
	daemon = tp_dbus_daemon_dup(&error);

	if (daemon == NULL)
	{
		purple_debug_error("telepathy", "Cannot create DBus daemon: %s\n", error->message);
		g_error_free(error);
		return FALSE;
	}

	/* the list of connection managers will be returned in list_connection_managers_cb */
	tp_list_connection_managers(daemon, list_connection_managers_cb, NULL, NULL, NULL);
	
	if (daemon != NULL)
		g_object_unref(daemon);

	return TRUE;
}

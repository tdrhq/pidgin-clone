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
#include <telepathy-glib/contact.h>
#include <telepathy-glib/dbus.h>
#include <telepathy-glib/interfaces.h>
#include <telepathy-glib/util.h>

#include "internal.h"

#include "account.h"
#include "accountopt.h"
#include "debug.h"
#include "notify.h"
#include "plugin.h"
#include "prefs.h"
#include "util.h"
#include "version.h"

#define TELEPATHY_ID "prpl-telepathy"
#define TELEPATHY_DISPLAY_VERSION "1.0"

#define TELEPATHY_STATUS_ONLINE   "available"
#define TELEPATHY_STATUS_AWAY     "away"
#define TELEPATHY_STATUS_OFFLINE  "offline"

static void *module_handle;
static gchar *module_path;

typedef struct
{
	TpConnectionManager *cm;
	TpConnectionManagerProtocol *protocol;
	TpConnection *connection;
	PurpleConnection *gc;
	PurpleAccount *acct;

	/* This flag avoids having a channel processed twice via both NewChannels and quering the Channels property */
	gboolean listing_channels;

	/* This will hold pointers to telepathy_text_channel for buddies that have an active conversation */
	GHashTable *text_Channels;
	
	/* This will map contact handles to TpContact */
	GHashTable *contacts;

} telepathy_data;

typedef struct
{
	GList *pending_Messages;
	TpChannel *channel;

	/* This flag avoids having a message processed twice via both Received signal and ListPendingMessages */
	gboolean received_Pending_Messages;
} telepathy_text_channel;

static void
destroy_text_channel(telepathy_text_channel *tp_channel)
{
	g_free(tp_channel);
}

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

static gchar*
telepathy_status_text(PurpleBuddy* buddy)
{
	const gchar *name = purple_buddy_get_name(buddy);

	PurplePresence *presence = purple_buddy_get_presence(buddy);

	if (presence != NULL)
	{
		PurpleStatus *status = purple_presence_get_active_status(presence);
		
		if (status != NULL)
		{
			const gchar *message = purple_status_get_attr_string(status, "message");

			purple_debug_info("telepathy", "Returning status for %s\n", name);

			return g_strdup(message);
		}
		else
		{
			GList *list = purple_presence_get_statuses(presence);

			if (list == NULL)
			{
				purple_debug_error("telepathy", "list is NULL!!\n");
			}

			purple_debug_warning("telepathy", "User %s has no active status!\n", name);

			purple_presence_switch_status(presence, "offline");
			return NULL;
		}
	}
	else
	{
		purple_debug_error("telepathy", "User %s has no presence!\n", name);
		return NULL;
	}
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
contact_notify_cb (TpContact *contact,
		   GParamSpec *pspec,
		   gpointer user_data)
{
	PurplePlugin *plugin = user_data;
	telepathy_data *data = plugin->extra;

	const gchar *name = tp_contact_get_identifier(contact);
	const gchar *presence_status = tp_contact_get_presence_status(contact);
	const gchar *presence_message = tp_contact_get_presence_message(contact);
	const gchar *alias = tp_contact_get_alias(contact);

	PurpleBuddy *buddy = purple_find_buddy(data->acct, tp_contact_get_identifier(contact));

	if (buddy == NULL)
	{
		purple_debug_warning("telepathy", "Received TpContact notify for non-existent buddy (%s)!\n", name);
		return;
	}

	purple_blist_alias_buddy(buddy, alias);

	purple_debug_info("telepathy", "%s is now %s (Status: \"%s\")\n",
			name, presence_status, presence_message);

	purple_prpl_got_user_status(data->acct, name, presence_status,
			"message", presence_message, NULL);
}


static void
contacts_ready_cb (TpConnection *connection,
                   guint n_contacts,
                   TpContact * const *contacts,
                   guint n_failed,
                   const TpHandle *failed,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "Contacts ready error: %s\n", error->message);
	}
	else
	{
		int i;

		purple_debug_info("telepathy", "Contacts:\n");
		for (i = 0; i<n_contacts; ++i)
		{
			TpContact *contact = contacts[i];
			PurpleBuddy *buddy;
			PurplePlugin *plugin = user_data;
			telepathy_data *data = plugin->extra;
			guint handle;

			purple_debug_info("telepathy", "  Contact ready: %s\n", tp_contact_get_alias(contact));

			/* the buddy might already be stored locally */
			buddy = purple_find_buddy(data->acct, tp_contact_get_identifier(contact));

			/* TODO: Get rid of the NO_SAVE flag so we can get that blist stored locally.
			 * Fix setting statuses for buddies stored locally 
			 */
			if (buddy == NULL)
			{
				purple_debug_info("telepathy", "New user detected!!!\n");
				buddy = purple_buddy_new(data->acct, tp_contact_get_identifier(contact), tp_contact_get_alias(contact));
				purple_blist_node_set_flags((PurpleBlistNode *)buddy, PURPLE_BLIST_NODE_FLAG_NO_SAVE);
			}

			handle = tp_contact_get_handle(contact);

			if (g_hash_table_lookup(data->contacts, (gpointer)handle) == NULL)
			{
				/* if we haven't cached the handle yet, the buddy isn't in the buddy list either */
				purple_blist_add_buddy(buddy, NULL, NULL, NULL);

				g_hash_table_insert(data->contacts, (gpointer)handle, contact);

				purple_debug_info("telepathy", "Connecting to notify for %s\n", tp_contact_get_identifier(contact));

				g_object_ref(contact);
				g_signal_connect(contact, "notify", G_CALLBACK (contact_notify_cb), user_data);
				contact_notify_cb (contact, NULL, user_data);
			}
		}
	}
}

static void
handle_list_channel (TpChannel *channel,
                     PurplePlugin *plugin)
{
	telepathy_data *data = plugin->extra;
	TpConnection *connection = data->connection;
	const TpIntSet *members;
	TpIntSetIter iter;
	GArray *handles;

	static const TpContactFeature features[] = {
		TP_CONTACT_FEATURE_ALIAS,
		TP_CONTACT_FEATURE_PRESENCE
	};

	members = tp_channel_group_get_members(channel);

	if (members == NULL)
	{
		purple_debug_error("telepathy", "Error while getting member list\n");
		return;
	}

	iter.set = members;
	iter.element = (guint)(-1);

	handles = tp_intset_to_array (members);

	/* we want to create a TpContact for each member of this channel */
	if (handles->len)
	{
		tp_connection_get_contacts_by_handle (connection,
				handles->len, (const TpHandle *) handles->data,
				G_N_ELEMENTS (features), features,
				contacts_ready_cb,
				plugin, NULL, NULL);

	}
	g_array_free (handles, TRUE);

}

static void
write_message_to_conversation (const gchar *from,
                               guint timestamp,
			       const gchar *msg,
			       gpointer user_data)
{
	PurplePlugin *plugin = user_data;
	telepathy_data *data = plugin->extra;

	/* if a conversation was not yet establish, create a new one */
	PurpleConversation *conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, from, data->acct);
	PurpleConvIm *im;

	/* escape HTML special characters */
	gchar *escaped_message = g_markup_escape_text(msg, -1);

	/* also change \n to <br> */
	gchar *final_message = purple_strdup_withhtml(escaped_message);
	g_free(escaped_message);

	if (conv == NULL)
	{
	    conv = purple_conversation_new(PURPLE_CONV_TYPE_IM, data->acct, from);
	}
	im = purple_conversation_get_im_data(conv);

	purple_debug_info("telepathy", "Contact %s says \"%s\" (escaped: \"%s\")\n", from, msg, final_message);

	/* transmit the message to the UI */
	purple_conv_im_write(im, from, final_message, 0, timestamp);

	g_free(final_message);
}

static void
list_pending_messages_cb  (TpChannel *proxy,
                           const GPtrArray *out_Pending_Messages,
                           const GError *error,
                           gpointer user_data,
                           GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "ListPendingMessages error: %s\n", error->message);
	}
	else
	{
		PurplePlugin *plugin = user_data;
		telepathy_data *data = plugin->extra;

		int i;

		GHashTable *properties = tp_channel_borrow_immutable_properties(proxy);
		gchar *who = (gchar *)tp_asv_get_string(properties, TP_IFACE_CHANNEL ".TargetID");

		telepathy_text_channel *tp_channel = g_hash_table_lookup(data->text_Channels, who);

		if (tp_channel == NULL)
		{
			purple_debug_warning("telepathy", "Received message from %s, but there's no channel struct for the buddy!\n", who);
		}
		else
		{
			/* we should now allow received_cb to handle incoming messages */
			tp_channel->received_Pending_Messages = TRUE;
		}

		for (i = 0; i<out_Pending_Messages->len; ++i)
		{
			/* unpack the relevant info from (uuuuus) */
			GValueArray *arr = g_ptr_array_index(out_Pending_Messages, i);
			guint timestamp = g_value_get_uint(g_value_array_get_nth(arr, 1));
			gchar *msg = (gchar *)g_value_get_string(g_value_array_get_nth(arr, 5));
			
			/* get the identifier from channel instead of contact since contact might not be ready for offline messages */
			gchar *from = (gchar *)tp_channel_get_identifier(proxy);

			write_message_to_conversation(from, timestamp, msg, user_data);
		}
	}
}

static void
received_cb (TpChannel *proxy,
             guint arg_ID,
             guint arg_Timestamp,
             guint arg_Sender,
             guint arg_Type,
             guint arg_Flags,
             const gchar *arg_Text,
             gpointer user_data,
             GObject *weak_object)
{
	PurplePlugin *plugin = user_data;
	telepathy_data *data = plugin->extra;

	GHashTable *properties = tp_channel_borrow_immutable_properties(proxy);
	gchar *who = (gchar *)tp_asv_get_string(properties, TP_IFACE_CHANNEL ".TargetID");

	telepathy_text_channel *tp_channel = g_hash_table_lookup(data->text_Channels, who);

	if (tp_channel == NULL)
	{
		purple_debug_warning("telepathy", "Received message from %s, but there's no channel struct for the buddy!\n", who);
	}
	else
	{
		/* will this message get caught by ListPendingMessages? */
		if (!tp_channel->received_Pending_Messages)
			return;

		write_message_to_conversation(who, arg_Timestamp, arg_Text, user_data);

	}
}

static void
send_cb (TpChannel *proxy,
         const GError *error,
         gpointer user_data,
         GObject *weak_object)
{
	if (error != NULL)
	{
		PurplePlugin *plugin = user_data;
		telepathy_data *data = plugin->extra;

		const gchar *who = tp_channel_get_identifier(proxy);

		gchar *error_message = g_strconcat(_("There was an error sending your message to "), who, NULL);
		gchar *error_message2 = g_strconcat(error_message, ": ", error->message, NULL);

		/* display the error in the conversation */
		if (!purple_conv_present_error(who, data->acct, error_message2))
		{
			/* display as a popup if there is no active conversation with the user */
			purple_notify_error(purple_connections_get_handle(),
					_("Error sending message"),
					error_message,
					error->message);
		}

		g_free(error_message2);
		g_free(error_message);
			
		purple_debug_error("telepathy", "Send error: %s\n", error->message);
	}
}

static void
text_channel_invalidated_cb (TpProxy *self,
                             guint    domain,
                             gint     code,
                             gchar   *message,
                             gpointer user_data)
{
	PurplePlugin *plugin = user_data;
	telepathy_data *data = plugin->extra;

	/* remove the cached TpChannel proxy when the channel closes */
	const gchar *who = tp_channel_get_identifier((TpChannel *)self);

	telepathy_text_channel *tp_channel = NULL;
	
	if (data->text_Channels)
		tp_channel = g_hash_table_lookup(data->text_Channels, who);

	purple_debug_info("telepathy", "Text channel with %s closed!\n", who);

	if (tp_channel)
	{
		tp_channel->channel = NULL;
	}
}

static void
handle_text_channel (TpChannel *channel,
                     PurplePlugin *plugin)
{
	telepathy_data *data = plugin->extra;

	GError *error = NULL;

	GHashTable *properties = tp_channel_borrow_immutable_properties(channel);
	gchar *who = (gchar *)tp_asv_get_string(properties, TP_IFACE_CHANNEL ".TargetID");

	telepathy_text_channel *tp_channel;

	purple_debug_info("telepathy", "Saving TpChannel proxy for %s\n", who);

	tp_channel = g_hash_table_lookup(data->text_Channels, who);

	/* if tp_channel exists, then we requested this channel, else it's an incoming request so we must cache it */
	if (tp_channel == NULL)
	{
		tp_channel = g_new0(telepathy_text_channel, 1);
		g_hash_table_insert(data->text_Channels, who, tp_channel);
	}

	tp_channel->channel = channel;

	tp_cli_channel_type_text_connect_to_received(channel, received_cb, plugin, NULL, NULL, &error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error connecting to Received signal: %s\n", error->message);
	}

	tp_channel->received_Pending_Messages = FALSE;

	g_signal_connect(channel, "invalidated", G_CALLBACK(text_channel_invalidated_cb), plugin);

	/* the Clear parameter is deprecated, we need to use AcknowledgePendingMessages */
	tp_cli_channel_type_text_call_list_pending_messages(channel, -1, FALSE, list_pending_messages_cb, plugin, NULL, NULL);

	/* send pending messages */
	while (tp_channel->pending_Messages != NULL)
	{
		purple_debug_info("telepathy", "Sending pending message \"%s\" to %s\n", (gchar *)tp_channel->pending_Messages->data, who);

		tp_cli_channel_type_text_call_send(channel, -1, TP_CHANNEL_TEXT_MESSAGE_TYPE_NORMAL, tp_channel->pending_Messages->data, send_cb, plugin, NULL, NULL);

		/* the message was duped */
		g_free(tp_channel->pending_Messages->data);

		tp_channel->pending_Messages = g_list_delete_link(tp_channel->pending_Messages, tp_channel->pending_Messages);
	}
}

static void
channel_ready_cb (TpChannel *channel,
                  const GError *error,
                  gpointer user_data)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "Channel ready error: %s\n", error->message);
	}
	else
	{
		GQuark handle_Type;

		handle_Type = tp_channel_get_channel_type_id(channel);

		if (handle_Type == TP_IFACE_QUARK_CHANNEL_TYPE_CONTACT_LIST)
		{
			handle_list_channel(channel, user_data);
		}
		else if (handle_Type == TP_IFACE_QUARK_CHANNEL_TYPE_TEXT)
		{
			handle_text_channel(channel, user_data);
		}
	}
}

static void
channel_invalidated_cb (TpProxy *self,
                        guint    domain,
                        gint     code,
                        gchar   *message,
                        gpointer user_data)
{
	purple_debug_info("telepathy", "Channel invalidated: %s\n", message);	

	g_object_unref(self);
}

static void
handle_new_channel (PurplePlugin* plugin,
                    const GValueArray *channel_Properties)
{
	char *object_Path = g_value_get_boxed(g_value_array_get_nth((GValueArray *)channel_Properties, 0));
	GHashTable *map = g_value_get_boxed(g_value_array_get_nth((GValueArray *)channel_Properties, 1));

	GError *error = NULL;
	TpConnection *connection = ((telepathy_data *)plugin->extra)->connection;
	TpChannel *channel = tp_channel_new_from_properties(connection, object_Path, map, &error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error while creating TpChannel: %s\n", error->message);
		g_error_free(error);
		return;
	}
	
	purple_debug_info("telepathy", "New channel: %s\n", object_Path);

	tp_channel_call_when_ready(channel, channel_ready_cb, plugin);

	g_signal_connect(channel, "invalidated", G_CALLBACK (channel_invalidated_cb), plugin);
}

static void
new_channels_cb (TpConnection *proxy,
                 const GPtrArray *arg_Channels,
                 gpointer user_data,
                 GObject *weak_object)
{
	int i;
	PurplePlugin *plugin = user_data;
	telepathy_data *data =plugin->extra;

	if (data->listing_channels)
		return;

	purple_debug_info("telepathy", "NewChannels:\n");

	for (i = 0; i < arg_Channels->len; i++)
	{
		GValueArray *channel = g_ptr_array_index(arg_Channels, i);

		handle_new_channel(user_data, channel);
	}
}

static void
get_channels_cb (TpProxy *proxy,
                 const GValue *out_Value,
                 const GError *error,
                 gpointer user_data,
                 GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "Get Channels error: %s\n", error->message);
	}
	else
	{
		PurplePlugin *plugin = user_data;
		telepathy_data *data = plugin->extra;

		/* unpack the a(oa{sv}) struct */
		const GPtrArray *channels = g_value_get_boxed(out_Value);
		int i;

		data->listing_channels = FALSE;

		for (i = 0; i < channels->len; i++)
		{
			const GValueArray *channel = g_ptr_array_index(channels, i);
			handle_new_channel(user_data, channel);
		}

	}
}

static void
connection_ready_cb (TpConnection *connection,
                     const GError *error,
                     gpointer user_data)
{
	if (error != NULL)
	{
		purple_debug_info("telepathy", "Connection ready error: %s\n", error->message);
	}
	else
	{
		char **interfaces, **ptr;
		GError *error = NULL;
		PurplePlugin* plugin = user_data;
		telepathy_data *data = plugin->extra;

		purple_debug_info("telepathy", "Connection is ready. Interfaces implemented:\n");

		/* query interfaces */
		g_object_get(connection, "interfaces", &interfaces, NULL);
		for (ptr = interfaces; ptr && *ptr; ptr++)
		{
			purple_debug_info("telepathy", "  %s\n", *ptr);
		}
		g_strfreev(interfaces);

		tp_cli_connection_interface_requests_connect_to_new_channels(connection, new_channels_cb, user_data, NULL, NULL, &error);

		if (error != NULL)
		{
			purple_debug_error("telepathy", "Error while connecting to NewChannels signal: %s\n", error->message);
			g_error_free(error);
			error = NULL;
			return;
		}

		data->listing_channels = TRUE;
			
		/* query the Channels property of the Requests interface */
		tp_cli_dbus_properties_call_get(connection, -1, TP_IFACE_CONNECTION_INTERFACE_REQUESTS, "Channels", get_channels_cb, user_data, NULL, NULL);

	}
}

static void
status_changed_cb (TpConnection *proxy,
                   guint arg_Status,
                   guint arg_Reason,
                   gpointer user_data,
                   GObject *weak_object)
{
	PurplePlugin* plugin = user_data;
	telepathy_data *data = plugin->extra;

	if (arg_Status == TP_CONNECTION_STATUS_CONNECTED)
	{
		purple_debug_info("telepathy", "Connected!\n");

		purple_connection_update_progress(data->gc, _("Connected"),
				1,   /* which connection step this is */
				2);  /* total number of steps */

		purple_connection_set_state(data->gc, PURPLE_CONNECTED);

		data->text_Channels = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) destroy_text_channel);
		data->contacts = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_object_unref);
	}
	else if (arg_Status == TP_CONNECTION_STATUS_DISCONNECTED)
	{
		gchar *reason = NULL;
		PurpleConnectionError error = PURPLE_CONNECTION_ERROR_OTHER_ERROR;
		purple_debug_info("telepathy", "Disconnected! Reason: %d\n", arg_Reason);

		switch (arg_Reason)
		{
			case TP_CONNECTION_STATUS_REASON_NETWORK_ERROR:
				reason = "Network error";
				error = PURPLE_CONNECTION_ERROR_NETWORK_ERROR;
			break;

			case TP_CONNECTION_STATUS_REASON_AUTHENTICATION_FAILED:
				reason = "Authentication failed";
				error = PURPLE_CONNECTION_ERROR_AUTHENTICATION_FAILED;
			break;

			case TP_CONNECTION_STATUS_REASON_ENCRYPTION_ERROR:
				reason = "Encryption error";
				error = PURPLE_CONNECTION_ERROR_ENCRYPTION_ERROR;
			break;

			case TP_CONNECTION_STATUS_REASON_NAME_IN_USE:
				reason = "Name in use";
				error = PURPLE_CONNECTION_ERROR_NAME_IN_USE;
			break;

			case TP_CONNECTION_STATUS_REASON_CERT_NOT_PROVIDED:
				reason = "SSL Certificate not provided";
				error = PURPLE_CONNECTION_ERROR_CERT_NOT_PROVIDED;
			break;

			case TP_CONNECTION_STATUS_REASON_CERT_UNTRUSTED:
				reason = "SSL Certificate is isnged by an untrusted certifying authority";
				error = PURPLE_CONNECTION_ERROR_CERT_UNTRUSTED;
			break;

			case TP_CONNECTION_STATUS_REASON_CERT_EXPIRED:
				reason = "SSL Certificate expired";
				error = PURPLE_CONNECTION_ERROR_CERT_EXPIRED;
			break;

			case TP_CONNECTION_STATUS_REASON_CERT_NOT_ACTIVATED:
				reason = "SSL Certificate is not yet valid";
				error = PURPLE_CONNECTION_ERROR_CERT_NOT_ACTIVATED;
			break;

			case TP_CONNECTION_STATUS_REASON_CERT_HOSTNAME_MISMATCH:
				reason = "SSL Certificate hostname mismatch";
				error = PURPLE_CONNECTION_ERROR_CERT_HOSTNAME_MISMATCH;
			break;

			case TP_CONNECTION_STATUS_REASON_CERT_FINGERPRINT_MISMATCH:
				reason = "SSL Certificate fingerprint mismatch";
				error = PURPLE_CONNECTION_ERROR_CERT_FINGERPRINT_MISMATCH;
			break;

			case TP_CONNECTION_STATUS_REASON_CERT_SELF_SIGNED:
				reason = "SSL Certificate is self-signed";
				error = PURPLE_CONNECTION_ERROR_CERT_SELF_SIGNED;
			break;

			case TP_CONNECTION_STATUS_REASON_CERT_OTHER_ERROR:
				reason = "Error while validating the server's SSL Certificate";
				error = PURPLE_CONNECTION_ERROR_CERT_OTHER_ERROR;
			break;
		}

		if (reason != NULL && data->gc)
			purple_connection_error_reason(data->gc, error, reason);

		if (data->connection)
		{
			g_object_unref(data->connection);
			data->connection = NULL;
		}

		if (data->text_Channels != NULL)
		{
			g_hash_table_destroy(data->text_Channels);
			data->text_Channels = NULL;
		}
		if (data->contacts != NULL)
		{
			g_hash_table_destroy(data->contacts);
			data->contacts = NULL;
		}

	}
	else if (arg_Status == TP_CONNECTION_STATUS_CONNECTING)
	{
		purple_debug_info("telepathy", "Connecting! Reason: %d\n", arg_Reason);
		purple_connection_set_state(data->gc, PURPLE_CONNECTING);
		purple_connection_update_progress(data->gc, _("Connecting"),
				0,   /* which connection step this is */
				2);  /* total number of steps */

	}
}

static void
connection_connect_cb (TpConnection *proxy,
                       const GError *error,
                       gpointer user_data,
                       GObject *weak_object)
{
	/* if this fails, something must be broken somewhere */
	if (error != NULL)
	{
		purple_debug_error("telepathy", "Connect error: %s\n", error->message);
	}
}

static void
request_connection_cb (TpConnectionManager *proxy,
                       const gchar *out_Bus_Name,
                       const gchar *out_Object_Path,
                       const GError *error,
                       gpointer user_data,
                       GObject *weak_object)
{
    	PurplePlugin* plugin = user_data;
	telepathy_data* data = plugin->extra;

	if (error != NULL)
	{
		purple_debug_info("telepathy", "RequestConnection error: %s\n", error->message);

		purple_connection_error_reason(data->gc, PURPLE_CONNECTION_ERROR_OTHER_ERROR, error->message);
	}
	else
	{
		GError *error = NULL;

		TpDBusDaemon *daemon = tp_dbus_daemon_dup(&error);
		
		if (error != NULL)
		{
			purple_debug_error("telepathy", "Error creating dbus daemon: %s\n", error->message);
			g_error_free(error);
			return;
		}

		/* get the connection proxy straight out of the dbus interface */
		data->connection = tp_connection_new(daemon, out_Bus_Name, out_Object_Path, &error);

		if (error != NULL)
		{
			purple_debug_error("telepathy", "Error creating TpConnection object: %s\n", error->message);
			g_error_free(error);
			return;
		}

		tp_connection_call_when_ready(data->connection, connection_ready_cb, plugin);

		/* this will indicate any connection status change, also providing a reason */
		tp_cli_connection_connect_to_status_changed(data->connection, status_changed_cb, plugin, NULL, NULL, &error);

		if (error != NULL)
		{
			purple_debug_error("telepathy", "Error conencting to StatusChanged: %s\n", error->message);
			g_error_free(error);
			tp_cli_connection_call_disconnect(data->connection, -1, NULL, NULL, NULL, NULL);
			g_object_unref(data->connection);
			data->connection = NULL;
		}
		else
		{
			/* do some magic now :) */
			tp_cli_connection_call_connect(data->connection, -1, connection_connect_cb, plugin, NULL, NULL);
		}

		if (daemon != NULL)
			g_object_unref(daemon);
	}
}

static void
telepathy_login(PurpleAccount *acct)
{
	PurpleConnection *gc = purple_account_get_connection(acct);

	PurplePlugin* plugin = gc->prpl;
	telepathy_data *data = (telepathy_data*)plugin->extra;
	GHashTable *options = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, (GDestroyNotify) tp_g_value_slice_free);
	int i;

	data->gc = gc;
	data->acct = acct;

	purple_debug_info("telepathy", "Logging in as %s\n", acct->username);


	/* fill in the hash table with the other options, considering the right signatures */
	for (i = 0; data->protocol->params[i].name != NULL; ++i)
	{
		gchar *name;
		gchar *signature;
		name = data->protocol->params[i].name;
		signature = data->protocol->params[i].dbus_signature;

		/* some protocols might not require username or password, so check them before */
		if (g_strcmp0(name, "account") == 0 && acct->username != NULL)
			tp_asv_set_string(options, "account", acct->username);
		if (g_strcmp0(name, "password") == 0 && acct->password != NULL)
			tp_asv_set_string(options, "password", acct->password);

		/* account and password have already been added */
		if (g_strcmp0(name, "account") != 0 && g_strcmp0(name, "password"))
		{
			if (g_strcmp0(signature, "s") == 0)
			{
				if (g_strcmp0(purple_account_get_string(acct, name, ""), ""))
					tp_asv_set_string(options, name, purple_account_get_string(acct, name, ""));
			}
			else if (g_strcmp0(signature, "n") == 0)
			{
				if (purple_account_get_int(acct, name, 0) != 0)
					tp_asv_set_int32(options, name, purple_account_get_int(acct, name, 0));
			}
			else if (g_strcmp0(signature, "i") == 0)
			{
				if (purple_account_get_int(acct, name, 0) != 0)
					tp_asv_set_int32(options, name, purple_account_get_int(acct, name, 0));
			}
			else if (g_strcmp0(signature, "q") == 0)
			{
				if (purple_account_get_int(acct, name, 0) != 0)
					tp_asv_set_uint32(options, name, purple_account_get_int(acct, name, 0));
			}
			else if (g_strcmp0(signature, "b") == 0)
			{
				tp_asv_set_boolean(options, name, purple_account_get_bool(acct, name, FALSE));
			}
			else
				purple_debug_warning("telepathy", "Unknown signature \"%s\" for \"%s\"\n", signature, name);
		}
	}

	/* call RequestConnection with the specified parameters */
	tp_cli_connection_manager_call_request_connection(data->cm, -1, data->protocol->name, options, request_connection_cb, plugin, NULL, NULL);
}

static void
telepathy_close(PurpleConnection *gc)
{
	PurplePlugin* plugin = gc->prpl;
	telepathy_data *data = (telepathy_data*)plugin->extra;

	purple_debug_info("telepathy", "We're closing, sorry :(\n");

	/* make sure the connection is closed in dbus-land, 
	 * or else we won't be able to recreate the connection */
	if (data->connection)
	{
		tp_cli_connection_call_disconnect(data->connection, -1, NULL, NULL, NULL, NULL);
		g_object_unref(data->connection);
		data->connection = NULL;
		data->gc = NULL;
	}

	if (data->text_Channels != NULL)
	{
		g_hash_table_destroy(data->text_Channels);
		data->text_Channels = NULL;
	}
	if (data->contacts != NULL)
	{
		g_hash_table_destroy(data->contacts);
		data->contacts = NULL;
	}
}

static void
ensure_channel_cb (TpConnection *proxy,
                   gboolean out_Yours,
                   const gchar *out_Channel,
                   GHashTable *out_Properties,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "EnsureChannel error: %s\n", error->message);
	}
}

static int
telepathy_send_im (PurpleConnection *gc,
                   const char *who,
                   const char *message,
                   PurpleMessageFlags flags)
{
	PurplePlugin* plugin = purple_connection_get_prpl(gc);
	telepathy_data *data = plugin->extra;

	/* check if the channel was already created */
	telepathy_text_channel *tp_channel = g_hash_table_lookup(data->text_Channels, who);
	TpChannel *channel = NULL;

	/* strip HTML */
	char *stripped_message = purple_markup_strip_html(message);

	if (tp_channel != NULL)
	{
		channel = tp_channel->channel;
	}
	else
	{
		/* if this is the first message, we need to create the channel */
		GHashTable *map = tp_asv_new (
			TP_IFACE_CHANNEL ".ChannelType", G_TYPE_STRING, TP_IFACE_CHANNEL_TYPE_TEXT,
			TP_IFACE_CHANNEL ".TargetHandleType", G_TYPE_UINT, TP_HANDLE_TYPE_CONTACT,
			TP_IFACE_CHANNEL ".TargetID", G_TYPE_STRING, who,
			NULL);

		tp_channel = g_new0(telepathy_text_channel, 1);
		g_hash_table_insert(data->text_Channels, g_strdup(who), tp_channel);

		purple_debug_info("telepathy", "Creating text channel for %s\n", who);

		tp_cli_connection_interface_requests_call_ensure_channel(data->connection, -1, map, ensure_channel_cb, plugin, NULL, NULL);
	}

	purple_debug_info("telepathy", "Sending \"%s\" (stripped: \"%s\") to %s\n", message, stripped_message, who);

	if (channel == NULL)
	{
		/* add the message to the pending list */
		tp_channel->pending_Messages = g_list_append(tp_channel->pending_Messages, g_strdup(stripped_message));
	}
	else
	{
		/* The channel already exists, send the message */
		tp_cli_channel_type_text_call_send(channel, -1, TP_CHANNEL_TEXT_MESSAGE_TYPE_NORMAL, stripped_message, send_cb, plugin, NULL, NULL);
	}

	g_free(stripped_message);

	return 1;
}

static void
telepathy_destroy(PurplePlugin *plugin)
{
	purple_debug_info("telepathy", "Shutting down\n");

	/* free everything that we created */
	if (g_strcmp0(plugin->info->id, "prpl-telepathy") != 0) {
		telepathy_data *data;
		g_free(plugin->info->extra_info);
		g_free(plugin->info);
		data = plugin->extra;
		g_object_unref(data->cm);
		g_free(data);
	}
}

static PurplePluginProtocolInfo telepathy_prpl_info =
{
	0,  /* options */
	NULL,               /* user_splits, initialized in nullprpl_init() */
	NULL,               /* protocol_options, initialized in nullprpl_init() */
	NO_BUDDY_ICONS,
	telepathy_list_icon,                  /* list_icon */
	NULL,                                /* list_emblem */
	telepathy_status_text,                /* status_text */
	NULL,               /* tooltip_text */
	telepathy_status_types,               /* status_types */
	NULL,            /* blist_node_menu */
	NULL,                  /* chat_info */
	NULL,         /* chat_info_defaults */
	telepathy_login,                      /* login */
	telepathy_close,                      /* close */
	telepathy_send_im,                    /* send_im */
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
	else if (g_strcmp0(signature, "i") == 0 || g_strcmp0(signature, "n") == 0)
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
	TpConnectionManagerParam *param;
	gboolean has_default;
	PurplePrefType type = PURPLE_PREF_NONE;

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
			human_name = get_human_name(name, signature, &type);
			has_default = param->flags & TP_CONN_MGR_PARAM_FLAG_HAS_DEFAULT;

			if (type == PURPLE_PREF_STRING)
			{
				option = purple_account_option_string_new(human_name, param->name, NULL);
				if (has_default)
					purple_account_option_set_default_string(option, g_value_get_string(&param->default_value));
			}
			else if (type == PURPLE_PREF_INT)
			{
				option = purple_account_option_int_new(human_name, param->name, 0);
				if (has_default)
					purple_account_option_set_default_int(option, get_int_value(&param->default_value, signature));
			}
			else if (type == PURPLE_PREF_BOOLEAN)
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

static void
fix_protocol_accounts(PurplePlugin* plugin)
{
	PurpleAccount *account;
	GList *iter;

	for (iter = purple_accounts_get_all(); iter != NULL; iter = iter->next)
	{
		account = iter->data;

		/* check if the protocol matches the account */
		if (g_strcmp0(purple_account_get_protocol_id(account), plugin->info->id) == 0)
		{
			PurplePluginProtocolInfo* prpl_info;
			PurpleStatusType *status_type;

			purple_debug_info("telepathy", "Fixing account %s using %s\n",
					purple_account_get_username(account), purple_account_get_protocol_id(account));


			/* prpl-telepathy accounts are loaded before the prpl itself and thus some information
			 * (like status types) are invalid. We need to fix those in order to make that account work.
			 *
			 * Following code is copied from purple_account_new() in account.c
			 *
			 * FIXME: this is kinda hackish
			 */

			prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(plugin);
			if (prpl_info != NULL && prpl_info->status_types != NULL)
				purple_account_set_status_types(account, prpl_info->status_types(account));

			account->presence = purple_presence_new_for_account(account);

			status_type = purple_account_get_status_type_with_primitive(account, PURPLE_STATUS_AVAILABLE);
			if (status_type != NULL)
				purple_presence_set_status_active(account->presence,
						purple_status_type_get_id(status_type),
						TRUE);
			else
				purple_presence_set_status_active(account->presence,
						"offline",
						TRUE);
		}
	}


}

static gboolean
export_prpl(TpConnectionManager *cm,
            TpConnectionManagerProtocol *protocol)
{
	/* create a plugin struct and copy all the information from the template */
	PurplePlugin *plugin = purple_plugin_new(TRUE, NULL);

	/* TODO: telepathy_data should be connection specific, not prpl specific */
	telepathy_data *data = g_new0(telepathy_data, 1);

	plugin->info = g_memdup(&telepathy_info, sizeof(telepathy_info));
	plugin->info->extra_info = g_memdup(&telepathy_prpl_info, sizeof(telepathy_prpl_info));
	plugin->extra = data;

	data->cm = cm;
	data->protocol = protocol;
	data->connection = NULL;
	data->gc = NULL;
	g_object_ref(data->cm);

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

	/* fix accounts using the current protocol */
	fix_protocol_accounts(plugin);

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

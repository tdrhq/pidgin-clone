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

#include <telepathy-glib/account-manager.h>
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
#include "prpl.h"
#include "util.h"
#include "version.h"

#include "telepathy_account.h"
#include "telepathy_avatar.h"
#include "telepathy_channel.h"
#include "telepathy_channel_list.h"
#include "telepathy_channel_text.h"
#include "telepathy_connection.h"
#include "telepathy_contact.h"
#include "telepathy_utils.h"

static void *module_handle;
static gchar *module_path;
static TpAccountManager *account_Manager;

typedef struct
{
	TpConnectionManager *cm;
	TpConnectionManagerProtocol *protocol;
	PurplePlugin *plugin;

} telepathy_data;

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

static const gchar*
telepathy_get_status_string(PurpleStatus *status)
{
	const gchar* id = purple_status_get_id(status);

	int i;

	for (i = 0; statuses[i].id != NULL; ++i)
	{
		if (statuses[i].status_primitive != PURPLE_STATUS_AVAILABLE && g_strcmp0(id, statuses[i].id) == 0)
		{
			return statuses[i].description;
		}
	}

	return NULL;
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

			/* return the description of the current status if there's no custom message */
			if (g_strcmp0(message, "") == 0)
				message = telepathy_get_status_string(status);

			/* an empty message confuses the UI, NULL is better */
			if (g_strcmp0(message, "") == 0)
				return NULL;

			return g_strdup(message);
		}
		else
		{
			GList *list = purple_presence_get_statuses(presence);

			if (list == NULL)
			{
				purple_debug_error("telepathy", "Status list for %s is NULL\n", name);
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

static void
telepathy_tooltip_text (PurpleBuddy *buddy,
                        PurpleNotifyUserInfo *user_info,
			gboolean full)
{
	//const gchar *name = purple_buddy_get_name(buddy);
	const gchar *alias = purple_buddy_get_alias(buddy);
	gchar *status = telepathy_status_text(buddy);
	
	purple_debug_info("telepathy", "Showing tooltip text for %s\n",
			purple_buddy_get_name(buddy));

	purple_notify_user_info_add_pair(user_info, _("Alias"), alias);

	if (status != NULL)
	{
		purple_notify_user_info_add_pair(user_info, _("Status"), status);
		g_free(status);
	}
}

/* TODO: Do a proper query of the "status" property */
static GList *
telepathy_status_types(PurpleAccount *acct)
{
	GList *types = NULL;
	PurpleStatusType *type;

	int i;

	for (i = 0; statuses[i].id != NULL; ++i)
	{
		if (statuses[i].description == NULL)
		{
			type = purple_status_type_new(statuses[i].status_primitive, statuses[i].id, NULL, TRUE);
		}
		else
		{
			type = purple_status_type_new_with_attrs(statuses[i].status_primitive,
					statuses[i].id, statuses[i].description, TRUE, TRUE, FALSE,
					"message", _("Message"), purple_value_new(PURPLE_TYPE_STRING),
					NULL);
		}
		types = g_list_prepend(types, type);
	}

	return g_list_reverse(types);
}

static GList *
telepathy_chat_info(PurpleConnection *gc)
{
	GList *m = NULL;
	struct proto_chat_entry *pce;

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Room name:");
	pce->identifier = "room";
	pce->required = TRUE;
	m = g_list_append(m, pce);

	return m;
}

static void
telepathy_login(PurpleAccount *acct)
{
	purple_debug_info("telepathy", "Object path: %s\n",
			purple_account_get_string(acct, "objpath", NULL));
	PurpleConnection *gc = purple_account_get_connection(acct);

	PurplePlugin* plugin = gc->prpl;
	telepathy_data *data = (telepathy_data*)plugin->extra;
	GHashTable *options = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, (GDestroyNotify) tp_g_value_slice_free);
	int i;

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
	tp_cli_connection_manager_call_request_connection(data->cm, -1, data->protocol->name, options, request_connection_cb, gc, NULL, NULL);

	g_hash_table_destroy(options);
}

static void
telepathy_close(PurpleConnection *gc)
{
	telepathy_connection *data = purple_connection_get_protocol_data(gc);

	purple_debug_info("telepathy", "We're closing, sorry :(\n");

	if (data)
	{
		/* make sure the connection is closed in dbus-land, 
		 * or else we won't be able to recreate the connection */
		if (data->connection)
		{
			/* this will call status_changed_cb which will free everything */
			tp_cli_connection_call_disconnect(data->connection, -1, NULL, NULL, NULL, NULL);
		}

	}
}

static int
telepathy_send_im (PurpleConnection *gc,
                   const char *who,
                   const char *message,
                   PurpleMessageFlags flags)
{
	telepathy_connection *data = purple_connection_get_protocol_data(gc);

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

		tp_cli_connection_interface_requests_call_ensure_channel(data->connection, -1, map, ensure_channel_cb, data, NULL, NULL);
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
		tp_cli_channel_type_text_call_send(channel, -1, TP_CHANNEL_TEXT_MESSAGE_TYPE_NORMAL, stripped_message, send_cb, data, NULL, NULL);
	}

	g_free(stripped_message);

	return 1;
}

static unsigned int
telepathy_send_typing (PurpleConnection *gc, const char *name, PurpleTypingState state)
{
	telepathy_connection *data = purple_connection_get_protocol_data(gc);
	telepathy_text_channel *tp_channel = g_hash_table_lookup(data->text_Channels, name);

	TpChannel *channel = NULL;
	TpChannelChatState tp_state;
	
	if (tp_channel == NULL)
	{
		purple_debug_warning("telepathy", "Received typing notification for %s who doesn't have a cached telepathy_channel struct\n", name);
		return 0;
	}

	channel = tp_channel->channel;

	if (channel == NULL)
	{
		purple_debug_warning("telepathy", "Received typing notification for %s who doesn't have a cached TpChannel proxy\n", name);
		return 0;
	}

	/* Check that the ChatState interface is actually implemented */
	if (!tp_proxy_has_interface_by_id (channel,
			TP_IFACE_QUARK_CHANNEL_INTERFACE_CHAT_STATE))
	{
		return 0;
	}

	purple_debug_info("telepathy", "Sending typing notification to %s\n", name);

	switch (state)
	{
		case PURPLE_NOT_TYPING:
			tp_state = TP_CHANNEL_CHAT_STATE_INACTIVE;
		break;

		case PURPLE_TYPING:
			tp_state = TP_CHANNEL_CHAT_STATE_COMPOSING;
		break;

		case PURPLE_TYPED:
			tp_state = TP_CHANNEL_CHAT_STATE_PAUSED;
		break;

		default:
			purple_debug_warning("telepathy", "Channel chat state defaulted to ACTIVE!\n");
			tp_state = TP_CHANNEL_CHAT_STATE_ACTIVE;
		break;
	}

	tp_cli_channel_interface_chat_state_call_set_chat_state(channel, -1, tp_state, set_chat_state_cb, data, NULL, NULL);

	return 0;
}

static void
telepathy_get_info (PurpleConnection *gc, const char *who)
{
	PurpleNotifyUserInfo *user_info = purple_notify_user_info_new();
	PurpleBuddy *buddy = purple_find_buddy(
			purple_connection_get_account(gc),
			who);
	gchar *status = telepathy_status_text(buddy);

	purple_debug_info("telepathy", "Getting info for %s\n", who);

	purple_notify_user_info_add_pair(user_info,
			_("Alias"), purple_buddy_get_alias(buddy));

	if (status != NULL)
	{
		purple_notify_user_info_add_pair(user_info,
				_("Status"), status);
		g_free(status);
	}

	purple_notify_userinfo(gc, who, user_info, NULL, NULL);
}

static void
telepathy_set_status (PurpleAccount *account, PurpleStatus *status)
{
	PurpleConnection *gc = purple_account_get_connection(account);
	telepathy_connection *data = purple_connection_get_protocol_data(gc);

	const gchar *presence_id = purple_status_get_id(status);
	const gchar *presence_message = purple_status_get_attr_string(status, "message");

	purple_debug_info("telepathy", "Setting status %s (%s) for %s\n", 
			presence_id, presence_message, purple_account_get_username(account));

	tp_cli_connection_interface_simple_presence_call_set_presence(data->connection, -1,
			presence_id, presence_message, 
			set_presence_cb, data,
			NULL, NULL);
}

static void
telepathy_add_buddy (PurpleConnection *gc, PurpleBuddy *buddy, PurpleGroup *group)
{
	const gchar* buddy_name = purple_buddy_get_name(buddy);
	const gchar* group_name = purple_group_get_name(group);

	/* When adding a buddy to a new group, the PurpleGroup will automatically be created,
	 * but we still have to check if there's an equivalent Telepathy channel.
	 */

	telepathy_connection *connection_data = purple_connection_get_protocol_data(gc);
	telepathy_group *tp_group = g_hash_table_lookup(connection_data->groups, group_name);

	purple_debug_info("telepathy", "Adding buddy %s to group %s\n",
			buddy_name, group_name);

	/* If there's no channel present, we'll have to create it before requesting a handle
	 * for the contact.
	 */
	if (tp_group == NULL)
	{
		telepathy_connection *data = purple_connection_get_protocol_data(gc);

		/* Request a contact list channel representing a group witht the specified name */
		GHashTable *request = tp_asv_new (
			TP_IFACE_CHANNEL ".ChannelType", G_TYPE_STRING,
			TP_IFACE_CHANNEL_TYPE_CONTACT_LIST,
			TP_IFACE_CHANNEL ".TargetHandleType", G_TYPE_UINT,
			TP_HANDLE_TYPE_GROUP,
			TP_IFACE_CHANNEL ".TargetID", G_TYPE_STRING,
			group_name,
			NULL);

		purple_debug_info("telepathy", "Group %s does not exist. Creating it!\n",
				group_name);

		/* We need to store the buddy name so we can add it later after the group is ready */
		g_hash_table_insert(data->buddy_to_be_added,
				g_strdup(group_name), g_strdup(buddy_name));

		/* Create the group channel */
		tp_cli_connection_interface_requests_call_create_channel(data->connection, -1,
				request, create_group_channel_cb, data,
				NULL, NULL);

		g_hash_table_destroy(request);
	}
	else
	{
		/* Since the channel is created, all we have to do is request a handle and add
		 * it to the relevant lists
		 */

		gchar const *ids[] = { buddy_name, NULL };

		tp_connection_request_handles(connection_data->connection, -1,
				TP_HANDLE_TYPE_CONTACT, ids,
				add_contact_to_group_cb, tp_group,
				NULL, NULL);
	}
}

static void
telepathy_remove_buddy (PurpleConnection *gc, PurpleBuddy *buddy, PurpleGroup *group)
{
	const gchar* buddy_name = purple_buddy_get_name(buddy);
	telepathy_connection *connection_data = purple_connection_get_protocol_data(gc);
	
	gchar const *ids[] = { buddy_name, NULL };

	purple_debug_info("telepathy", "Removing buddy %s\n", buddy_name);

	tp_connection_request_handles(connection_data->connection, -1,
			TP_HANDLE_TYPE_CONTACT, ids,
			remove_contact_cb, connection_data,
			NULL, NULL);
}

static void
telepathy_join_chat (PurpleConnection *gc, GHashTable *components)
{
	telepathy_connection *data = purple_connection_get_protocol_data(gc);

	const gchar *name = g_hash_table_lookup(components, "room");
	telepathy_room_channel *tp_channel = g_hash_table_lookup(components, "tp_channel");

	/* Is this a channel the user requested? */
	if (tp_channel == NULL)
	{
		/* Request a room text channel */
		GHashTable *map = tp_asv_new (
			TP_IFACE_CHANNEL ".ChannelType",G_TYPE_STRING, TP_IFACE_CHANNEL_TYPE_TEXT,
			TP_IFACE_CHANNEL ".TargetHandleType", G_TYPE_UINT, TP_HANDLE_TYPE_ROOM,
			TP_IFACE_CHANNEL ".TargetID", G_TYPE_STRING, name,
			NULL);

		purple_debug_info("telepathy", "Requesting room text channel for %s\n", name);

		tp_cli_connection_interface_requests_call_ensure_channel(data->connection, -1,
				map, ensure_channel_cb, data, NULL, NULL);
	}
	else
	{
		/* This is an invitation request confirmation */
		GArray *arr = g_array_new(FALSE, FALSE, sizeof(TpHandle));

		g_array_append_val(arr, tp_channel->self_handle);

		purple_debug_info("telepathy", "Adding self handle to chatroom group\n");

		tp_cli_channel_interface_group_call_add_members(tp_channel->channel, -1,
				arr, NULL, add_members_cb, data, NULL, NULL);

		g_array_free(arr, TRUE);

		handle_room_text_channel(tp_channel->channel, data, TRUE);
	}
}

static void
invite_contact_to_chatroom_cb (TpConnection *connection,
                               TpHandleType handle_type,
                               guint n_handles,
                               const TpHandle *handles,
                               const gchar * const *ids,
                               const GError *error,
                               gpointer user_data,
                               GObject *weak_object)
{
	telepathy_chat_invitation *invitation = user_data;
	telepathy_room_channel *tp_channel = invitation->tp_channel;
	GArray *arr;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error while requesting handle for inviting"
				" contact to chatroom: %s\n", error->message);
		g_free(invitation->msg);
		return;
	}

	arr = g_array_new(0, 0, sizeof(TpHandle));

	/* Send the invitation by calling AddMembers with the handle. If the contact
	 * accepts it, he will just join the chat, nothing more we need to do.
	 */
	g_array_append_val(arr, handles[0]);

	tp_cli_channel_interface_group_call_add_members(tp_channel->channel, -1,
			arr, invitation->msg,
			NULL, NULL,
			NULL, NULL);

	g_array_free(arr, TRUE);

	g_free(invitation->msg);

}

static void
telepathy_reject_chat (PurpleConnection *gc, GHashTable *components)
{
	telepathy_room_channel *tp_channel = g_hash_table_lookup(components, "tp_channel");

	if (tp_channel != NULL)
	{
		tp_cli_channel_call_close(tp_channel->channel, -1, NULL, NULL, NULL, NULL);
	}
}

static void
telepathy_chat_invite (PurpleConnection *gc, int id, const char *message, const char *who)
{
	telepathy_connection *data = purple_connection_get_protocol_data(gc);
	
	telepathy_room_channel *tp_channel = g_hash_table_lookup(
			data->room_Channels, (gpointer)(TpHandle)id);

	gchar const *ids[] = { who, NULL };

	telepathy_chat_invitation *invitation;

	if (tp_channel == NULL)
	{
		purple_debug_error("telepathy", "No telepathy_room_channel struct cached"
				" for %d\n", id);
		return;
	}
	
	purple_debug_info("telepathy", "Inviting %s to chatroom\n", who);

	invitation = g_new(telepathy_chat_invitation, 1);
	invitation->tp_channel = tp_channel;
	invitation->msg = g_strdup(message);

	/* Request a handle that will be passed to AddMembers for this group */
	tp_connection_request_handles(data->connection, -1,
			TP_HANDLE_TYPE_CONTACT, ids,
			invite_contact_to_chatroom_cb, invitation, g_free, NULL);
}

static void
telepathy_chat_leave (PurpleConnection *gc, int id)
{
	telepathy_connection *data = purple_connection_get_protocol_data(gc);

	telepathy_room_channel *tp_channel = g_hash_table_lookup(
			data->room_Channels, (gpointer)(TpHandle)id);

	if (tp_channel == NULL)
	{
		purple_debug_error("telepathy", "No telepathy_room_channel struct cached"
				" for %d\n", id);
		return;
	}

	tp_cli_channel_call_close(tp_channel->channel, -1, NULL, NULL, NULL, NULL);
}

static int
telepathy_chat_send (PurpleConnection *gc, int id, const char *message, PurpleMessageFlags flags)
{
	telepathy_connection *data = purple_connection_get_protocol_data(gc);

	telepathy_room_channel *tp_channel = g_hash_table_lookup(
			data->room_Channels, (gpointer)(TpHandle)id);

	TpChannel *channel;

	char *stripped_message;

	if (tp_channel == NULL)
	{
		purple_debug_error("telepathy", "No telepathy_room_channel struct cached"
				" for %d\n", id);
		return 0;
	}
	
	channel = tp_channel->channel;

	if (channel == NULL)
	{
		purple_debug_error("telepathy", "No TpChannel proxy cached for %d\n", id);
		return 0;
	}

	/* strip HTML */
	stripped_message = purple_markup_strip_html(message);

	purple_debug_info("telepathy", "Sending \"%s\" (stripped: \"%s\") to %s\n",
			message, stripped_message, tp_channel_get_identifier(channel));

	tp_cli_channel_type_text_call_send(channel, -1,
		TP_CHANNEL_TEXT_MESSAGE_TYPE_NORMAL, stripped_message,
		chat_send_cb, data, NULL, NULL);

	g_free(stripped_message);

	serv_got_chat_in(gc, id, purple_account_get_username(data->acct), flags,
			message, time(NULL));
	return 1;
}

static void
telepathy_set_buddy_icon (PurpleConnection *gc, PurpleStoredImage *img)
{
	telepathy_connection *data = purple_connection_get_protocol_data(gc);

	guint size = purple_imgstore_get_size(img);
	gchar *mime_type = g_strdup_printf("images/%s", purple_imgstore_get_extension(img));

	/* different .jpg and .jpeg extensions correspond to the same MIME type */
	if (g_strcmp0(mime_type, "images/jpg") == 0)
	{
		g_free(mime_type);
		mime_type = "images/jpeg";
	}

	purple_debug_info("telepathy", "Setting icon (type: %s, size: %u)\n", mime_type, size);

	if (size > 0)
	{
		/* copy the data to a new GArray */
		GArray *array = g_array_sized_new(FALSE, FALSE, 1, size);
		g_array_append_vals(array, purple_imgstore_get_data(img), size);

		/* set the avatar */
		tp_cli_connection_interface_avatars_call_set_avatar(data->connection, -1, array, mime_type, set_avatar_cb, data, NULL, NULL);

		g_array_free(array, FALSE);
	}
	else
	{
		/* a 0-sized avatar means we should clear our avatar */
		tp_cli_connection_interface_avatars_call_clear_avatar(data->connection, -1, clear_avatar_cb, data, NULL, NULL);
	}
}

static void
set_properties_cb (TpProxy *proxy,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_info("telepathy", "SetProperties error: %s\n", error->message);
		return;
	}

	purple_debug_info("telepathy", "SetProperties succeeded!\n");
}

static void
telepathy_set_chat_topic (PurpleConnection *gc, int id, const char *topic)
{
	telepathy_connection *data = purple_connection_get_protocol_data(gc);
	telepathy_room_channel *tp_channel = g_hash_table_lookup(data->room_Channels, (gpointer)id);

	telepathy_property *tp_property;

	GPtrArray *arr;

	GValueArray *val_arr;

	GValue *topic_val;
	GValue *id_val;
	GValue *variant_val;

	if (tp_channel == NULL)
	{
		purple_debug_error("telepathy", "There is no cached telepathy_room_channel for"
				" updating status!\n");
		return;
	}

	tp_property = g_hash_table_lookup(tp_channel->properties_by_name,
			"subject");

	if (tp_property == NULL)
	{
		purple_debug_warning("telepathy", "Chatroom does not have a \"subject\" proprety!\n");
		return;
	}

	purple_debug_info("telepathy", "Setting topic for chatroom to %s\n", topic);

	/* This is the array of properties to set. DBus signature: a(uv) */
	arr = g_ptr_array_sized_new(1);

	/* This represents a single property to set. DBus signature: (uv) */
	val_arr = g_value_array_new(2);

	/* Turn everything into GValues */
	id_val = tp_g_value_slice_new_uint(tp_property->id);

	topic_val = tp_g_value_slice_new_string(topic);
	variant_val = tp_g_value_slice_new_boxed(G_TYPE_VALUE, topic_val);

	/* Fill in the DBus struct */
	g_value_array_append(val_arr, id_val);
	g_value_array_append(val_arr, variant_val);

	/* Add it to the array */
	g_ptr_array_add(arr, val_arr);

	/* Finally change that property */
	tp_cli_properties_interface_call_set_properties(tp_channel->channel, -1,
			arr, set_properties_cb, tp_channel, NULL, NULL);
	
	/* Free everything */
	tp_g_value_slice_free(variant_val);
	tp_g_value_slice_free(id_val);
	tp_g_value_slice_free(topic_val);

	g_value_array_free(val_arr);

	g_ptr_array_free(arr, TRUE);
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
	OPT_PROTO_CHAT_TOPIC,  /* options */
	NULL,               /* user_splits, initialized in nullprpl_init() */
	NULL,               /* protocol_options, initialized in nullprpl_init() */
	NO_BUDDY_ICONS,
	telepathy_list_icon,                  /* list_icon */
	NULL,                                /* list_emblem */
	telepathy_status_text,                /* status_text */
	telepathy_tooltip_text,               /* tooltip_text */
	telepathy_status_types,               /* status_types */
	NULL,            /* blist_node_menu */
	telepathy_chat_info,                  /* chat_info */
	NULL,         /* chat_info_defaults */
	telepathy_login,                      /* login */
	telepathy_close,                      /* close */
	telepathy_send_im,                    /* send_im */
	NULL,                   /* set_info */
	telepathy_send_typing,                /* send_typing */
	telepathy_get_info,                   /* get_info */
	telepathy_set_status,                 /* set_status */
	NULL,                   /* set_idle */
	NULL,              /* change_passwd */
	telepathy_add_buddy,                  /* add_buddy */
	NULL,                /* add_buddies */
	telepathy_remove_buddy,               /* remove_buddy */
	NULL,             /* remove_buddies */
	NULL,                 /* add_permit */
	NULL,                   /* add_deny */
	NULL,                 /* rem_permit */
	NULL,                   /* rem_deny */
	NULL,            /* set_permit_deny */
	telepathy_join_chat,                  /* join_chat */
	telepathy_reject_chat,                /* reject_chat */
	NULL,              /* get_chat_name */
	telepathy_chat_invite,                /* chat_invite */
	telepathy_chat_leave,                 /* chat_leave */
	NULL,               /* chat_whisper */
	telepathy_chat_send,                  /* chat_send */
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
	telepathy_set_buddy_icon,             /* set_buddy_icon */
	NULL,               /* remove_group */
	NULL,                                /* get_cb_real_name */
	telepathy_set_chat_topic,             /* set_chat_topic */
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

	NULL,                   

	NULL,                          
	NULL,	
	NULL,                          
	NULL                           
};                               

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
		purple_debug_warning("telepathy", "Unknown DBus signature \"%s\" for option \"%s\"\n",
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

	telepathy_data *data = g_new0(telepathy_data, 1);

	plugin->info = g_memdup(&telepathy_info, sizeof(telepathy_info));
	plugin->info->extra_info = g_memdup(&telepathy_prpl_info, sizeof(telepathy_prpl_info));
	plugin->extra = data;

	data->cm = cm;
	data->protocol = protocol;
	data->plugin = plugin;
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
	
	/* Create an AccountManager interface */
	account_Manager = tp_account_manager_new(daemon);

	if (account_Manager == NULL)
	{
		purple_debug_error("telepathy", "There is no AccountManager interface!\n");
		return FALSE;
	}

	/* Get all valid accounts from AccountManager */
	tp_cli_dbus_properties_call_get(account_Manager, -1,
			TP_IFACE_ACCOUNT_MANAGER, "ValidAccounts",
			get_valid_accounts_cb, NULL, NULL, NULL);

	if (daemon != NULL)
		g_object_unref(daemon);

	return TRUE;
}

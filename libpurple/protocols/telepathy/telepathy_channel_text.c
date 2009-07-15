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

#include "telepathy_channel_text.h"

#include <telepathy-glib/interfaces.h>

#include "internal.h"

#include "debug.h"

#include "telepathy_connection.h"
#include "telepathy_contact.h"

void
destroy_room_channel(telepathy_room_channel *tp_channel)
{
	g_free(tp_channel);
}

void
destroy_text_channel(telepathy_text_channel *tp_channel)
{
	g_free(tp_channel);
}

static void
write_message_to_chatroom (int id,
                           const gchar *from,
                           guint timestamp,
			   const gchar *msg,
			   gpointer user_data)
{
	telepathy_connection *data = user_data;

	/* escape HTML special characters */
	gchar *escaped_message = g_markup_escape_text(msg, -1);

	/* also change \n to <br> */
	gchar *final_message = purple_strdup_withhtml(escaped_message);
	g_free(escaped_message);

	purple_debug_info("telepathy", "Received from %s: \"%s\" (escaped: \"%s\")\n",
			from, msg, final_message);

	/* transmit the message to the UI */
	serv_got_chat_in(data->gc, id, from, PURPLE_MESSAGE_RECV, final_message, timestamp);

	g_free(final_message);
}

static void
chat_got_message (telepathy_room_channel *tp_channel,
                  guint arg_ID,
                  guint arg_Timestamp,
                  guint arg_Sender,
                  guint arg_Type,
                  guint arg_Flags,
                  const gchar *arg_Text)
{
	TpChannel *channel = tp_channel->channel;

	TpHandle handle = tp_channel_get_handle(channel, NULL);

	GArray *message_IDs;

	/* Get information about the contact who sent this message */
	const gchar *from;
	TpContact *contact = g_hash_table_lookup(
			tp_channel->contacts, (gpointer)arg_Sender);

	/* TODO: Get rid of the unknown sender bit */

	if (contact != NULL)
		from = tp_contact_get_alias(contact);
	else
		from = "<Unknown Sender>";

	/* drop message if it's not text */
	if ((arg_Flags & TP_CHANNEL_TEXT_MESSAGE_FLAG_NON_TEXT_CONTENT) == 0)
			write_message_to_chatroom(handle, from,
					arg_Timestamp, arg_Text, tp_channel->connection_data);

	/* this will hold the IDs of message that will be acknowledged */
	message_IDs = g_array_new(FALSE, FALSE, sizeof(guint));

	/* add the id to the array of acknowledge messages */
	g_array_append_val(message_IDs, arg_ID);

	/* acknowledge the messages now */
	tp_cli_channel_type_text_call_acknowledge_pending_messages(channel, -1, message_IDs,
			acknowledge_pending_messages_cb, tp_channel->connection_data, NULL, NULL);

	g_array_free(message_IDs, TRUE);

}

static void
chat_list_pending_messages_cb  (TpChannel *proxy,
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
		telepathy_connection *data = user_data;

		int i;

		const gchar *who = tp_channel_get_identifier(proxy);
		TpHandle handle = tp_channel_get_handle(proxy, NULL);

		/* Get the channel struct by channel handle
		 * (which is the same with the libpurple chat id)
		 */
		telepathy_room_channel *tp_channel = g_hash_table_lookup(
				data->room_Channels, (gpointer)handle);

		if (tp_channel == NULL)
		{
			purple_debug_warning("telepathy", "Pending message from %s,"
					" but there's no channel struct for the chatroom!\n", who);
			return;
		}

		for (i = 0; i<out_Pending_Messages->len; ++i)
		{
			/* unpack the relevant info from (uuuuus) */
			GValueArray *arr = g_ptr_array_index(out_Pending_Messages, i);

			guint msg_id = g_value_get_uint(g_value_array_get_nth(arr, 0));
			guint timestamp = g_value_get_uint(g_value_array_get_nth(arr, 1));
			guint sender = g_value_get_uint(g_value_array_get_nth(arr, 2));
			guint type = g_value_get_uint(g_value_array_get_nth(arr, 3));
			guint flags = g_value_get_uint(g_value_array_get_nth(arr, 4));
			gchar *msg = (gchar *)g_value_get_string(g_value_array_get_nth(arr, 5));

			/* Forward the message to purple-land */
			chat_got_message(tp_channel, msg_id, timestamp, sender, type, flags, msg);
		}
	}
}

static void
chat_received_cb (TpChannel *proxy,
                  guint arg_ID,
                  guint arg_Timestamp,
                  guint arg_Sender,
                  guint arg_Type,
                  guint arg_Flags,
                  const gchar *arg_Text,
                  gpointer user_data,
                  GObject *weak_object)
{
	telepathy_connection *data = user_data;

	GHashTable *properties = tp_channel_borrow_immutable_properties(proxy);
	gchar *who = (gchar *)tp_asv_get_string(properties, TP_IFACE_CHANNEL ".TargetID");
	TpHandle handle = tp_channel_get_handle(proxy, NULL);

	/* Get the channel struct by channel handle
	 * (which is the same with the libpurple chat id)
	 */
	telepathy_room_channel *tp_channel = g_hash_table_lookup(
			data->room_Channels, (gpointer)handle);

	if (tp_channel == NULL)
	{
		purple_debug_warning("telepathy", "Received message from %s,"
				" but there's no channel struct for the chatroom!\n", who);
	}
	else
	{
		/* Forward the message to purple-land */
		chat_got_message(tp_channel,
				arg_ID,
				arg_Timestamp,
				arg_Sender,
				arg_Type,
				arg_Flags,
				arg_Text);
	}
}

static void
chat_send_error_cb (TpChannel *proxy,
               guint arg_Error,
               guint arg_Timestamp,
               guint arg_Type,
               const gchar *arg_Text,
               gpointer user_data,
               GObject *weak_object)
{
	telepathy_connection *data = user_data;

	const gchar *who = tp_channel_get_identifier(proxy);

	const gchar *error_reason = NULL;
	gchar *error_message;
	gchar *error_message2;

	switch (arg_Error)
	{
		case TP_CHANNEL_TEXT_SEND_ERROR_UNKNOWN:
			error_reason = _("Unknown error");
		break;

		case TP_CHANNEL_TEXT_SEND_ERROR_OFFLINE:
			error_reason = _("Contact is offline");
		break;

		case TP_CHANNEL_TEXT_SEND_ERROR_INVALID_CONTACT:
			error_reason = _("Contact is invalid");
		break;

		case TP_CHANNEL_TEXT_SEND_ERROR_PERMISSION_DENIED:
			error_reason = _("Permission denied");
		break;

		case TP_CHANNEL_TEXT_SEND_ERROR_TOO_LONG:
			error_reason = _("The message is too long");
		break;

		case TP_CHANNEL_TEXT_SEND_ERROR_NOT_IMPLEMENTED:
			error_reason = _("Not implemented");
		break;
	}

	error_message = g_strdup_printf(_("There was an error sending your message to %s"), who);
	error_message2 = g_strdup_printf("%s: %s", error_message, error_reason);

	/* FIXME: purple_conv_present_error() only works for 1-to-1 IMs
	 * use serv_chat_send (PurpleConnection *, int, const char *, PurpleMessageFlags flags)
	 */

	/* display the error in the conversation */
	if (!purple_conv_present_error(who, data->acct, error_message2))
	{
		/* display as a popup if there is no active conversation with the user */
		purple_notify_error(purple_connections_get_handle(),
				_("Error sending message"),
				error_message,
				error_reason);
	}

	g_free(error_message2);
	g_free(error_message);

	purple_debug_error("telepathy", "SendError: %s\n", error_reason);
}

void
chat_send_cb (TpChannel *proxy,
              const GError *error,
              gpointer user_data,
              GObject *weak_object)
{
	if (error != NULL)
	{
		telepathy_connection *data = user_data;

		const gchar *who = tp_channel_get_identifier(proxy);

		gchar *error_message = g_strdup_printf(
				_("There was an error sending your message to %s"), who);
		gchar *error_message2 = g_strdup_printf("%s: %s", error_message, error->message);

		/* FIXME: purple_conv_present_error() only works for 1-to-1 IMs
		 * use serv_chat_send (PurpleConnection *, int, const char *, PurpleMessageFlags flags)
		 */

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
chat_get_contacts_cb (TpConnection *connection,
                      guint n_contacts,
                      TpContact * const *contacts,
                      guint n_failed,
                      const TpHandle *failed,
                      const GError *error,
                      gpointer user_data,
                      GObject *weak_object)
{	
	int i;
	PurpleConversation *conv;
	telepathy_room_channel *tp_channel = user_data;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "get_contacts_by_id for chatroom error: %s\n",
				error->message);
		return;
	}

	purple_debug_info("telepathy", "Got %u contacts for chatroom (%u failed):\n",
			n_contacts, n_failed);

	conv = purple_find_chat(tp_channel->connection_data->gc,
			tp_channel_get_handle(tp_channel->channel, NULL));
	for (i = 0; i<n_contacts; ++i)
	{
		TpHandle handle = tp_contact_get_handle(contacts[i]);

		purple_debug_info("telepathy", "  %s (%s)\n",
				tp_contact_get_identifier(contacts[i]),
				tp_contact_get_alias(contacts[i]));

		/* Make sure we are not duplicating any contact */
		if (!g_hash_table_lookup(tp_channel->contacts, (gpointer)handle))
		{
			purple_conv_chat_add_user(PURPLE_CONV_CHAT(conv),
					tp_contact_get_alias(contacts[i]), NULL,
					PURPLE_CBFLAGS_NONE, FALSE);

			g_object_ref(contacts[i]);

			g_hash_table_insert(tp_channel->contacts, (gpointer)handle, contacts[i]);
		}
	}

	/* Only after we got the member list are we ready to receive messages */
	tp_cli_channel_type_text_call_list_pending_messages(tp_channel->channel, -1,
			FALSE, chat_list_pending_messages_cb, tp_channel->connection_data,
			NULL, NULL);

}

static void
chat_get_all_members_cb (TpChannel *proxy,
                         const GArray *out_Members,
                         const GArray *out_Local_Pending,
                         const GArray *out_Remote_Pending,
                         const GError *error,
                         gpointer user_data,
                         GObject *weak_object)
{
	telepathy_room_channel *data = user_data;
	int i;

	const TpContactFeature features[] = {
	    TP_CONTACT_FEATURE_ALIAS,
	    TP_CONTACT_FEATURE_PRESENCE
	};

	if (error != NULL)
	{
		purple_debug_error("telepathy", "GetAllMembers error for chatroom: %s\n",
				error->message);
		return;
	}

	purple_debug_info("telepathy", "Got %u members for chatroom\n", out_Members->len);

	for (i = 0; i<out_Members->len; ++i)
	{
		purple_debug_info("telepathy", "  %u\n",
				g_array_index(out_Members, TpHandle, i));
	}

	tp_connection_get_contacts_by_handle(data->connection_data->connection,
			out_Members->len, (TpHandle *)out_Members->data,
			G_N_ELEMENTS (features), features,
			chat_get_contacts_cb, user_data,
			NULL, NULL);

	if (out_Local_Pending->len > 0)
	{
		purple_debug_info("telepathy", "Got %u local pending members\n",
				out_Local_Pending->len);

		for (i = 0; i<out_Local_Pending->len; ++i)
		{
			purple_debug_info("telepathy", "  %u\n",
					g_array_index(out_Local_Pending, TpHandle, i));
		}
	}

	if (out_Remote_Pending->len > 0)
	{
		purple_debug_info("telepathy", "Got %u remote pending members\n",
				out_Remote_Pending->len);

		for (i = 0; i<out_Remote_Pending->len; ++i)
		{
			purple_debug_info("telepathy", "  %u\n",
					g_array_index(out_Remote_Pending, TpHandle, i));
		}
	}
}

static void
room_channel_invalidated_cb (TpProxy *self,
                             guint    domain,
                             gint     code,
                             gchar   *message,
                             gpointer user_data)
{
	telepathy_room_channel *data = user_data;
	telepathy_connection *connection_data = data->connection_data;

	GHashTableIter iter;
	gpointer key, value;

	/* remove the cached TpChannel proxy when the channel closes */
	TpHandle handle = tp_channel_get_handle((TpChannel *)self, NULL);

	purple_debug_info("telepathy", "Chatroom channel with handle %u closed!\n", handle);

	/* Unref all the contacts in the chatroom */
	g_hash_table_iter_init (&iter, data->contacts);
	while (g_hash_table_iter_next (&iter, &key, &value)) 
	{
		g_object_unref(value);
	}

	g_hash_table_destroy(data->contacts);

	g_hash_table_remove(connection_data->room_Channels, (gpointer)handle);
}

static void
chat_members_joined (TpConnection *connection,
                     guint n_contacts,
                     TpContact * const *contacts,
                     guint n_failed,
                     const TpHandle *failed,
                     const GError *error,
                     gpointer user_data,
                     GObject *weak_object)
{
	telepathy_room_channel *tp_channel = user_data;
	telepathy_connection *connection_data = tp_channel->connection_data;
	PurpleConversation *conv;

	int i;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error getting TpContact for members joining"
				" chatroom: %s\n", error->message);
		return;
	}

	if (n_failed > 0)
	{
		purple_debug_error("telepathy", "Failed getting TpContact for %u members joining"
				" chatroom", n_failed);
	}

	conv = purple_find_chat(connection_data->gc,
			tp_channel_get_handle(tp_channel->channel, NULL));

	if (conv == NULL)
	{
		purple_debug_error("telepathy", "There's no conversation to add members to!\n");
	}

	for (i = 0; i<n_contacts; ++i)
	{
		TpContact *contact = contacts[i];

		purple_conv_chat_add_user(PURPLE_CONV_CHAT(conv),
				tp_contact_get_alias(contact), NULL,
				PURPLE_CBFLAGS_NONE, TRUE);

		g_object_ref(contacts[i]);

		g_hash_table_insert(tp_channel->contacts,
				(gpointer)tp_contact_get_handle(contact),
				contact);
	}
}


static void
chat_members_changed_cb (TpChannel *proxy,
                         const gchar *arg_Message,
                         const GArray *arg_Added,
                         const GArray *arg_Removed,
                         const GArray *arg_Local_Pending,
                         const GArray *arg_Remote_Pending,
                         guint arg_Actor,
                         guint arg_Reason,
                         gpointer user_data,
                         GObject *weak_object)
{
	telepathy_room_channel *tp_channel = user_data;
	telepathy_connection *connection_data = tp_channel->connection_data;

	/* Members joined the charoom */
	if (arg_Added->len > 0)
	{
		const TpContactFeature features[] = {
			TP_CONTACT_FEATURE_ALIAS,
			TP_CONTACT_FEATURE_PRESENCE
		};

		purple_debug_info("telepathy", "%u users joined chatroom!\n", arg_Added->len);

		/* Request TpContact objects and then we can add them to the chatroom */
		tp_connection_get_contacts_by_handle(connection_data->connection,
				arg_Added->len, (TpHandle *)arg_Added->data,
				G_N_ELEMENTS (features), features,
				chat_members_joined, tp_channel,
				NULL, NULL);
	}

	/* Members left the chatroom */
	if (arg_Removed->len > 0)
	{
		int i;

		purple_debug_info("telepathy", "%u users left chatroom!\n", arg_Removed->len);

		for (i = 0; i<arg_Removed->len; ++i)
		{
			/* Get the TpContact cached in the channel struct */
			TpHandle handle = g_array_index(arg_Removed, TpHandle, i);
			TpContact *contact = g_hash_table_lookup(tp_channel->contacts,
					(gpointer)handle);

			PurpleConversation *conv;
			const gchar *alias;

			if (contact == NULL)
			{
				purple_debug_error("telepathy", "Handle %u left chatroom but"
						" was not cached!\n", handle);
				continue;
			}

			g_hash_table_remove(tp_channel->contacts, (gpointer)handle);

			alias = tp_contact_get_alias(contact);

			/* Remove the member from the chatroom in libpurple land */
			conv = purple_find_chat(connection_data->gc,
					tp_channel_get_handle(tp_channel->channel, NULL));
			purple_conv_chat_remove_user(PURPLE_CONV_CHAT(conv), alias, NULL);

			g_object_unref(contact);
		}
	}
}

void
handle_room_text_channel (TpChannel *channel,
                          telepathy_connection *data,
                          gboolean invitation)
{
	GError *error = NULL;

	GHashTable *properties = tp_channel_borrow_immutable_properties(channel);

	gchar *who = (gchar *)tp_asv_get_string(properties, TP_IFACE_CHANNEL ".TargetID");

	TpHandle initiator_handle = tp_asv_get_uint32(properties,
			TP_IFACE_CHANNEL ".InitiatorHandle", NULL);

	TpHandle handle = tp_channel_get_handle(channel, NULL);

	telepathy_room_channel *tp_channel;

	tp_channel = g_hash_table_lookup(data->room_Channels, (gpointer)handle);


	/* Is this an invitation we have not yet handled? */
	if (initiator_handle != data->self_handle && !invitation)
	{
		gchar *initiator_id = (gchar *)tp_asv_get_string(properties,
				TP_IFACE_CHANNEL ".InitiatorID");
		
		TpHandle actor;
		TpChannelGroupChangeReason reason;
		const gchar *message;

		/* Create a hash table for prompting the user */
		GHashTable *chat_data = g_hash_table_new(g_str_hash, g_str_equal);

		purple_debug_info("telepathy", "We have been invited by %s\n",
				initiator_id);

		/* Include information about the channel */
		tp_channel = g_new0(telepathy_room_channel, 1);

		tp_channel->channel = channel;
		tp_channel->connection_data = data;
		tp_channel->self_handle = tp_channel_group_get_self_handle(channel);

		/* Get details about the invitation like message */
		tp_channel_group_get_local_pending_info(channel, tp_channel->self_handle,
				&actor, &reason, &message);

		g_hash_table_insert(data->room_Channels, (gpointer)handle, tp_channel);

		/* Fill in the actual information */
		g_hash_table_insert(chat_data, "room", g_strdup(who));
		g_hash_table_insert(chat_data, "tp_channel", tp_channel);

		/* Prompt the user. Depending on his action, telepathy_join_chat() or
		 * telepathy_reject_chat() will be called along with the hash table
		 */
		serv_got_chat_invite(data->gc, who, initiator_id, message, chat_data);

		/* Return to wait for user input. If he accepts the invitation, this function
		 * will be called again with the invitation flag set.
		 */
		return;
	}

	/* if tp_channel exists, then we requested this channel
	 * else it's an incoming request so we must cache it
	 */
	if (tp_channel == NULL)
	{
		purple_debug_info("telepathy", "Saving TpChannel proxy for \"%s\" chatroom (%u)\n",
				who, handle);

		tp_channel = g_new0(telepathy_room_channel, 1);

		tp_channel->channel = channel;
		tp_channel->connection_data = data;
		tp_channel->self_handle = tp_channel_group_get_self_handle(channel);

		g_hash_table_insert(data->room_Channels, (gpointer)handle, tp_channel);
	}

	serv_got_joined_chat(data->gc, handle, who);
	tp_channel->contacts = g_hash_table_new(g_direct_hash, g_direct_equal);

	/* This will notify us of users joining or leaving */
	tp_cli_channel_interface_group_connect_to_members_changed(channel,
			chat_members_changed_cb, tp_channel,
			NULL, NULL, &error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error connecting to MembersChanged signal: %s\n",
				error->message);
	}


	/* Get the members of the chatroom */
	tp_cli_channel_interface_group_call_get_all_members(channel, -1,
			chat_get_all_members_cb, tp_channel,
			NULL, NULL);

	tp_cli_channel_type_text_connect_to_received(channel,
			chat_received_cb, data, NULL, NULL, &error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error connecting to Received signal: %s\n",
				error->message);
	}


	g_signal_connect(channel, "invalidated", G_CALLBACK(room_channel_invalidated_cb), tp_channel);

	tp_cli_channel_type_text_connect_to_send_error(channel,
			chat_send_error_cb, data, NULL, NULL, &error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error connecting to SendError signal: %s\n", error->message);
		g_error_free(error);
	}
}

void
write_message_to_conversation (const gchar *from,
                               guint timestamp,
			       const gchar *msg,
			       gpointer user_data)
{
	telepathy_connection *data = user_data;

	/* escape HTML special characters */
	gchar *escaped_message = g_markup_escape_text(msg, -1);

	/* also change \n to <br> */
	gchar *final_message = purple_strdup_withhtml(escaped_message);
	g_free(escaped_message);

	purple_debug_info("telepathy", "Contact %s says \"%s\" (escaped: \"%s\")\n", from, msg, final_message);

	/* transmit the message to the UI */
	serv_got_im(data->gc, from, final_message, PURPLE_MESSAGE_RECV, timestamp);

	g_free(final_message);
}

void
acknowledge_pending_messages_cb (TpChannel *proxy,
                                 const GError *error,
				 gpointer user_data,
				 GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "AcknowledgePendingMessages error: %s\n", error->message);
	}
}

void
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
		telepathy_connection *data = user_data;
		GArray *message_IDs;

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

		/* this will hold the IDs of message that will be acknowledged */
		message_IDs = g_array_new(FALSE, FALSE, sizeof(guint));

		for (i = 0; i<out_Pending_Messages->len; ++i)
		{
			/* unpack the relevant info from (uuuuus) */
			GValueArray *arr = g_ptr_array_index(out_Pending_Messages, i);
			guint msg_id = g_value_get_uint(g_value_array_get_nth(arr, 0));
			guint timestamp = g_value_get_uint(g_value_array_get_nth(arr, 1));
			guint flags = g_value_get_uint(g_value_array_get_nth(arr, 4));
			gchar *msg = (gchar *)g_value_get_string(g_value_array_get_nth(arr, 5));
			
			/* get the identifier from channel instead of contact since contact might not be ready for offline messages */
			gchar *from = (gchar *)tp_channel_get_identifier(proxy);

			/* drop message if it's not text */
			if ((flags & TP_CHANNEL_TEXT_MESSAGE_FLAG_NON_TEXT_CONTENT) == 0)
				write_message_to_conversation(from, timestamp, msg, user_data);

			/* add the id to the array of acknowledge messages */
			g_array_append_val(message_IDs, msg_id);
		}

		/* acknowledge the messages now */
		tp_cli_channel_type_text_call_acknowledge_pending_messages(proxy, -1, message_IDs,
				acknowledge_pending_messages_cb, user_data, NULL, NULL);

		g_array_free(message_IDs, TRUE);

	}
}

void
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
	telepathy_connection *data = user_data;

	GHashTable *properties = tp_channel_borrow_immutable_properties(proxy);
	gchar *who = (gchar *)tp_asv_get_string(properties, TP_IFACE_CHANNEL ".TargetID");

	telepathy_text_channel *tp_channel = g_hash_table_lookup(data->text_Channels, who);

	if (tp_channel == NULL)
	{
		purple_debug_warning("telepathy", "Received message from %s, but there's no channel struct for the buddy!\n", who);
	}
	else
	{
		GArray *message_IDs;

		/* will this message get caught by ListPendingMessages? */
		if (!tp_channel->received_Pending_Messages)
			return;

		/* drop this message if it's not text */
		if ((arg_Flags & TP_CHANNEL_TEXT_MESSAGE_FLAG_NON_TEXT_CONTENT) == 0)
			write_message_to_conversation(who, arg_Timestamp, arg_Text, user_data);

		/* acknowledge receiving the message */
		message_IDs = g_array_new(FALSE, FALSE, sizeof(guint));

		g_array_append_val(message_IDs, arg_ID);

		tp_cli_channel_type_text_call_acknowledge_pending_messages(proxy, -1, message_IDs,
				acknowledge_pending_messages_cb, user_data, NULL, NULL);

		g_array_free(message_IDs, TRUE);

	}
}

void
send_error_cb (TpChannel *proxy,
               guint arg_Error,
               guint arg_Timestamp,
               guint arg_Type,
               const gchar *arg_Text,
               gpointer user_data,
               GObject *weak_object)
{
	telepathy_connection *data = user_data;

	const gchar *who = tp_channel_get_identifier(proxy);

	const gchar *error_reason = NULL;
	gchar *error_message;
	gchar *error_message2;

	switch (arg_Error)
	{
		case TP_CHANNEL_TEXT_SEND_ERROR_UNKNOWN:
			error_reason = _("Unknown error");
		break;

		case TP_CHANNEL_TEXT_SEND_ERROR_OFFLINE:
			error_reason = _("Contact is offline");
		break;

		case TP_CHANNEL_TEXT_SEND_ERROR_INVALID_CONTACT:
			error_reason = _("Contact is invalid");
		break;

		case TP_CHANNEL_TEXT_SEND_ERROR_PERMISSION_DENIED:
			error_reason = _("Permission denied");
		break;

		case TP_CHANNEL_TEXT_SEND_ERROR_TOO_LONG:
			error_reason = _("The message is too long");
		break;

		case TP_CHANNEL_TEXT_SEND_ERROR_NOT_IMPLEMENTED:
			error_reason = _("Not implemented");
		break;
	}

	error_message = g_strdup_printf(_("There was an error sending your message to %s"), who);
	error_message2 = g_strdup_printf("%s: %s", error_message, error_reason);

	/* display the error in the conversation */
	if (!purple_conv_present_error(who, data->acct, error_message2))
	{
		/* display as a popup if there is no active conversation with the user */
		purple_notify_error(purple_connections_get_handle(),
				_("Error sending message"),
				error_message,
				error_reason);
	}

	g_free(error_message2);
	g_free(error_message);

	purple_debug_error("telepathy", "SendError: %s\n", error_reason);
}

void
send_cb (TpChannel *proxy,
         const GError *error,
         gpointer user_data,
         GObject *weak_object)
{
	if (error != NULL)
	{
		telepathy_connection *data = user_data;

		const gchar *who = tp_channel_get_identifier(proxy);

		gchar *error_message = g_strdup_printf(_("There was an error sending your message to %s"), who);
		gchar *error_message2 = g_strdup_printf("%s: %s", error_message, error->message);

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

void
text_channel_invalidated_cb (TpProxy *self,
                             guint    domain,
                             gint     code,
                             gchar   *message,
                             gpointer user_data)
{
	telepathy_connection *data = user_data;

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

void
chat_state_changed_cb (TpChannel *proxy,
                       guint arg_Contact,
                       guint arg_State,
                       gpointer user_data,
                       GObject *weak_object)
{
	telepathy_connection *data = user_data;

	telepathy_contact *contact_data = g_hash_table_lookup(data->contacts, (gpointer)arg_Contact);

	PurpleTypingState state;
	const gchar *name;

	if (contact_data == NULL)
	{
		purple_debug_warning("telepathy", "Chat state changed for %u who isn't cached!\n", arg_Contact);
		return;
	}

	name = tp_contact_get_identifier(contact_data->contact);

	purple_debug_info("telepathy", "Chat state changed for %s\n", name);

	switch (arg_State)
	{
		case TP_CHANNEL_CHAT_STATE_PAUSED:
			state = PURPLE_TYPED;
		break;

		case TP_CHANNEL_CHAT_STATE_COMPOSING:
			state = PURPLE_TYPING;
		break;

		default:
			state = PURPLE_NOT_TYPING;
		break;
	}

	if (state == PURPLE_NOT_TYPING)
	{
		serv_got_typing_stopped(data->gc, name);
	}
	else
	{
		serv_got_typing(data->gc, name, -1, state);
	}
}

static void
handle_im_text_channel (TpChannel *channel,
                        telepathy_connection *data)
{
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

	tp_cli_channel_interface_chat_state_connect_to_chat_state_changed(channel, chat_state_changed_cb, data, NULL, NULL, &error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error connecting to ChatStateChanged signal: %s\n", error->message);
	}

	tp_cli_channel_type_text_connect_to_received(channel, received_cb, data, NULL, NULL, &error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error connecting to Received signal: %s\n", error->message);
	}

	tp_channel->received_Pending_Messages = FALSE;

	g_signal_connect(channel, "invalidated", G_CALLBACK(text_channel_invalidated_cb), data);

	/* the Clear parameter is deprecated, we need to use AcknowledgePendingMessages */
	tp_cli_channel_type_text_call_list_pending_messages(channel, -1, FALSE, list_pending_messages_cb, data, NULL, NULL);

	/* send pending messages */
	while (tp_channel->pending_Messages != NULL)
	{
		purple_debug_info("telepathy", "Sending pending message \"%s\" to %s\n", (gchar *)tp_channel->pending_Messages->data, who);

		tp_cli_channel_type_text_call_send(channel, -1, TP_CHANNEL_TEXT_MESSAGE_TYPE_NORMAL, tp_channel->pending_Messages->data, send_cb, data, NULL, NULL);

		/* the message was duped */
		g_free(tp_channel->pending_Messages->data);

		tp_channel->pending_Messages = g_list_delete_link(tp_channel->pending_Messages, tp_channel->pending_Messages);
	}

	tp_cli_channel_type_text_connect_to_send_error(channel, send_error_cb, data, NULL, NULL, &error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error connecting to SendError signal: %s\n", error->message);
		g_error_free(error);
	}
}

void
handle_text_channel (TpChannel *channel,
                     telepathy_connection *data)
{
	TpHandle handle;
	TpHandleType handleType;

	handle = tp_channel_get_handle(channel, &handleType);

	switch (handleType)
	{
		case TP_HANDLE_TYPE_CONTACT:
			handle_im_text_channel(channel, data);
		break;

		case TP_HANDLE_TYPE_ROOM:
			handle_room_text_channel(channel, data, FALSE);
		break;

		default:
			return;
	}
}

void
set_chat_state_cb (TpChannel *proxy,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "SetChatState error: %s\n", error->message);
	}
}


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

#include "telepathy_connection.h"

#include <telepathy-glib/interfaces.h>

#include "internal.h"

#include "connection.h"
#include "debug.h"

#include "telepathy_account.h"
#include "telepathy_avatar.h"
#include "telepathy_channel.h"
#include "telepathy_channel_text.h"
#include "telepathy_channel_list.h"
#include "telepathy_contact.h"

void
connection_status_changed (gpointer data,
                           guint arg_Status,
			   guint arg_Reason)
{
	telepathy_account *account_data = data;
	telepathy_connection *connection_data = account_data->connection_data;
	PurpleConnection *gc = purple_account_get_connection(account_data->account);

	if (arg_Status == TP_CONNECTION_STATUS_CONNECTED)
	{
		purple_debug_info("telepathy", "Connected!\n");

		purple_connection_update_progress(gc, _("Connected"),
				1,   /* which connection step this is */
				2);  /* total number of steps */

		purple_connection_set_state(gc, PURPLE_CONNECTED);

		connection_data->text_Channels = g_hash_table_new_full(g_str_hash, g_str_equal,
				g_free, (GDestroyNotify) destroy_text_channel);
		connection_data->room_Channels = g_hash_table_new_full(g_direct_hash, g_direct_equal,
				NULL, (GDestroyNotify) destroy_room_channel);
		connection_data->contacts = g_hash_table_new_full(g_direct_hash, g_direct_equal,
				NULL, (GDestroyNotify) destroy_contact);
		connection_data->groups = g_hash_table_new_full(g_str_hash, g_str_equal,
				g_free, (GDestroyNotify) destroy_group);
		connection_data->lists = g_hash_table_new_full(g_str_hash, g_str_equal,
				g_free, (GDestroyNotify) destroy_group);
		connection_data->buddy_to_be_added = g_hash_table_new_full(g_str_hash, g_str_equal,
				g_free, g_free);
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

		if (reason != NULL && gc)
			purple_connection_error_reason(gc, error, reason);

		if (connection_data)
		{
			purple_connection_set_protocol_data(connection_data->gc, NULL);

			if (connection_data->connection != NULL)
			{
				g_object_unref(connection_data->connection);
				connection_data->connection = NULL;
			}

			if (connection_data->text_Channels != NULL)
			{
				g_hash_table_destroy(connection_data->text_Channels);
				connection_data->text_Channels = NULL;
			}

			if (connection_data->contacts != NULL)
			{
				g_hash_table_destroy(connection_data->contacts);
				connection_data->contacts = NULL;
			}

			if (connection_data->groups != NULL)
			{
				g_hash_table_destroy(connection_data->groups);
				connection_data->groups = NULL;
			}

			if (connection_data->lists != NULL)
			{
				g_hash_table_destroy(connection_data->lists);
				connection_data->lists = NULL;
			}

			g_free(connection_data);
		}

	}
	else if (arg_Status == TP_CONNECTION_STATUS_CONNECTING)
	{
		purple_debug_info("telepathy", "Connecting! Reason: %d\n", arg_Reason);
		purple_connection_set_state(gc, PURPLE_CONNECTING);
		purple_connection_update_progress(gc, _("Connecting"),
				0,   /* which connection step this is */
				2);  /* total number of steps */

	}
}

void
got_connection_object (gchar *connection_object,
                       gpointer data)
{
	GError *error = NULL;

	telepathy_connection *connection_data;
	telepathy_account *account_data = data;

	PurpleAccount *acct = account_data->account;
	PurpleConnection *gc = purple_account_get_connection(acct);

	TpDBusDaemon *daemon = tp_dbus_daemon_dup(&error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error creating dbus daemon: %s\n", error->message);
		g_error_free(error);
		return;
	}

	purple_debug_info("telepathy", "Got connection object!\n");

	connection_data = g_new0(telepathy_connection, 1);

	/* get the connection proxy straight out of the dbus interface */
	connection_data->connection = tp_connection_new(daemon, NULL, connection_object, &error);
	connection_data->gc = gc;
	connection_data->acct = acct;
	connection_data->account_data = account_data;

	account_data->connection_data = connection_data;

	purple_connection_set_protocol_data(gc, connection_data);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error creating TpConnection object: %s\n", error->message);
		g_error_free(error);

		g_object_unref(connection_data->connection);
		connection_data->connection = NULL;
		g_free(connection_data);
		return;
	}

	tp_connection_call_when_ready(connection_data->connection, connection_ready_cb, connection_data);

	if (daemon != NULL)
		g_object_unref(daemon);
}

void
status_changed_cb (TpConnection *proxy,
                   guint arg_Status,
                   guint arg_Reason,
                   gpointer user_data,
                   GObject *weak_object)
{
	telepathy_connection *data = user_data;

	if (arg_Status == TP_CONNECTION_STATUS_CONNECTED)
	{
		purple_debug_info("telepathy", "Connected!\n");

		purple_connection_update_progress(data->gc, _("Connected"),
				1,   /* which connection step this is */
				2);  /* total number of steps */

		purple_connection_set_state(data->gc, PURPLE_CONNECTED);

		data->text_Channels = g_hash_table_new_full(g_str_hash, g_str_equal,
				g_free, (GDestroyNotify) destroy_text_channel);
		data->room_Channels = g_hash_table_new_full(g_direct_hash, g_direct_equal,
				NULL, (GDestroyNotify) destroy_room_channel);
		data->contacts = g_hash_table_new_full(g_direct_hash, g_direct_equal,
				NULL, (GDestroyNotify) destroy_contact);
		data->groups = g_hash_table_new_full(g_str_hash, g_str_equal,
				g_free, (GDestroyNotify) destroy_group);
		data->lists = g_hash_table_new_full(g_str_hash, g_str_equal,
				g_free, (GDestroyNotify) destroy_group);
		data->buddy_to_be_added = g_hash_table_new_full(g_str_hash, g_str_equal,
				g_free, g_free);
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

		if (data)
		{
			purple_connection_set_protocol_data(data->gc, NULL);

			if (reason != NULL && data->gc)
				purple_connection_error_reason(data->gc, error, reason);

			if (data->connection != NULL)
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

			if (data->groups != NULL)
			{
				g_hash_table_destroy(data->groups);
				data->groups = NULL;
			}

			if (data->lists != NULL)
			{
				g_hash_table_destroy(data->lists);
				data->lists = NULL;
			}

			g_free(data);
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

void
set_presence_cb (TpConnection *proxy,
                 const GError *error,
                 gpointer user_data,
                 GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "SetStatus error: %s\n", error->message);
	}
}

void
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
		telepathy_connection *data = user_data;

		purple_debug_info("telepathy", "Connection is ready. Interfaces implemented:\n");

		/* query interfaces */
		g_object_get(connection, "interfaces", &interfaces, NULL);
		for (ptr = interfaces; ptr && *ptr; ptr++)
		{
			purple_debug_info("telepathy", "  %s\n", *ptr);
		}
		g_strfreev(interfaces);


		/* The self handle is useful in a number of possible scenarios,
		 * we need to keep track of it.
		 */
		data->self_handle = tp_connection_get_self_handle(connection);

		purple_debug_info("telepathy", "Self Handle: %u\n", data->self_handle);


		tp_cli_connection_interface_requests_connect_to_new_channels(connection,
				new_channels_cb, user_data,
				NULL, NULL, &error);

		if (error != NULL)
		{
			purple_debug_error("telepathy", "Error while connecting to"
				" NewChannels signal: %s\n", error->message);
			g_error_free(error);
			error = NULL;
			return;
		}

		data->listing_channels = TRUE;
			
		/* query the Channels property of the Requests interface */
		tp_cli_dbus_properties_call_get(connection, -1,
			TP_IFACE_CONNECTION_INTERFACE_REQUESTS, "Channels",
			get_channels_cb, user_data,
			NULL, NULL);


		/* query supported avatar formats */
		tp_cli_dbus_properties_call_get_all(connection, -1,
			TP_IFACE_CONNECTION_INTERFACE_AVATARS,
			get_avatar_properties_cb, user_data,
			NULL, NULL);

		/* this will be fired when an avatar for a buddy has been received */
		tp_cli_connection_interface_avatars_connect_to_avatar_retrieved(connection,
				avatar_retrieved_cb, data,
				NULL, NULL,
				&error);
		
		if (error != NULL)
		{
			purple_debug_error("telepathy", "Error connecting to AvatarRetrieved signal: %s\n", error->message);
			g_error_free(error);
		}

		tp_cli_connection_interface_avatars_connect_to_avatar_updated(connection,
				avatar_updated_cb, data,
				NULL, NULL,
				&error);

		if (error != NULL)
		{
			purple_debug_error("telepathy", "Error connecting to AvatarUpdated signal: %s\n", error->message);
			g_error_free(error);
		}

	}
}

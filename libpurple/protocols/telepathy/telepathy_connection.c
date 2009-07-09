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

#include "telepathy_avatar.h"
#include "telepathy_channel.h"
#include "telepathy_channel_text.h"
#include "telepathy_channel_list.h"
#include "telepathy_contact.h"

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

void
request_connection_cb (TpConnectionManager *proxy,
                       const gchar *out_Bus_Name,
                       const gchar *out_Object_Path,
                       const GError *error,
                       gpointer user_data,
                       GObject *weak_object)
{
	PurpleConnection *gc = user_data;

	if (error != NULL)
	{
		purple_debug_info("telepathy", "RequestConnection error: %s\n", error->message);

		purple_connection_error_reason(gc, PURPLE_CONNECTION_ERROR_OTHER_ERROR, error->message);
	}
	else
	{
		GError *error = NULL;
		telepathy_connection *connection_data;

		TpDBusDaemon *daemon = tp_dbus_daemon_dup(&error);
		
		if (error != NULL)
		{
			purple_debug_error("telepathy", "Error creating dbus daemon: %s\n", error->message);
			g_error_free(error);
			return;
		}

		connection_data = g_new0(telepathy_connection, 1);

		/* get the connection proxy straight out of the dbus interface */
		connection_data->connection = tp_connection_new(daemon, out_Bus_Name, out_Object_Path, &error);
		connection_data->gc = gc;
		connection_data->acct = purple_connection_get_account(gc);
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

		/* this will indicate any connection status change, also providing a reason */
		tp_cli_connection_connect_to_status_changed(connection_data->connection, status_changed_cb, connection_data, NULL, NULL, &error);

		if (error != NULL)
		{
			purple_debug_error("telepathy", "Error conencting to StatusChanged: %s\n", error->message);
			g_error_free(error);

			tp_cli_connection_call_disconnect(connection_data->connection, -1, NULL, NULL, NULL, NULL);
			g_object_unref(connection_data->connection);
			connection_data->connection = NULL;
			g_free(connection_data);
		}
		else
		{
			/* do some magic now :) */
			tp_cli_connection_call_connect(connection_data->connection, -1, connection_connect_cb, connection_data, NULL, NULL);
		}

		if (daemon != NULL)
			g_object_unref(daemon);
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


		/* query supported avatar formats */
		tp_cli_dbus_properties_call_get_all(connection, -1, TP_IFACE_CONNECTION_INTERFACE_AVATARS, get_avatar_properties_cb, user_data, NULL, NULL);

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

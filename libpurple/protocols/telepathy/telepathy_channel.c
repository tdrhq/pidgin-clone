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

#include "telepathy_channel.h"

#include <telepathy-glib/channel-request.h>
#include <telepathy-glib/dbus.h>
#include <telepathy-glib/interfaces.h>

#include "internal.h"

#include "debug.h"

#include "telepathy_avatar.h"
#include "telepathy_channel_list.h"
#include "telepathy_channel_text.h"
#include "telepathy_contact.h"

static void
proceed_cb (TpChannelRequest *proxy,
            const GError *error,
            gpointer user_data,
            GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "Proceed error: %s\n", error->message);
		return;
	}

	purple_debug_info("telepathy", "Proceed succeeded!\n");
}

void
create_channel_cb (TpChannelDispatcher *proxy,
                   const gchar *out_Request,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object)
{
	GError *err = NULL;
	TpDBusDaemon *bus_daemon;
	TpChannelRequest *request;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "CreateChannel error: %s\n", error->message);
		return;
	}

	purple_debug_info("telepathy", "CreateChannel succeeded!\n");

	bus_daemon = tp_dbus_daemon_dup(&err);

	if (err != NULL)
	{
		purple_debug_error("telepathy", "Error dupping dbus daemon: %s\n",
			err->message);
		g_error_free(err);
		g_object_unref(bus_daemon);
		return;
	}

	request = tp_channel_request_new(bus_daemon, out_Request, NULL, &err);

	if (err != NULL)
	{
		purple_debug_error("telepathy", "Error creating ChannelRequest proxy: %s\n",
			err->message);
		g_error_free(err);
		g_object_unref(bus_daemon);
		g_object_unref(request);
		return;
	}

	tp_cli_channel_request_call_proceed(request, -1, proceed_cb, user_data, NULL, NULL);
}

void
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

void
channel_invalidated_cb (TpProxy *self,
                        guint    domain,
                        gint     code,
                        gchar   *message,
                        gpointer user_data)
{
	purple_debug_info("telepathy", "Channel invalidated: %s\n", message);	

	g_object_unref(self);
}

void
handle_new_channel (telepathy_connection *data,
                    const GValueArray *channel_Properties)
{
	char *object_Path = g_value_get_boxed(g_value_array_get_nth((GValueArray *)channel_Properties, 0));
	GHashTable *map = g_value_get_boxed(g_value_array_get_nth((GValueArray *)channel_Properties, 1));

	GError *error = NULL;
	TpConnection *connection = data->connection;

	TpChannel *channel;
	const gchar *channel_Type = g_value_get_string(
		g_hash_table_lookup(map, TP_IFACE_CHANNEL ".ChannelType"));

	if (channel_Type == NULL)
	{
		purple_debug_error("telepathy", "Channel has no type!\n");
		return;
	}

	/* We only listen to contact list channels via the NewChannels signal */
	if (g_strcmp0(channel_Type, TP_IFACE_CHANNEL_TYPE_CONTACT_LIST) != 0)
	{
		purple_debug_info("telepathy", "NewChannels received non contact list channel (%s),"
			" dropping it!\n", channel_Type);
		return;
	}

	channel = tp_channel_new_from_properties(connection, object_Path, map, &error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error while creating TpChannel: %s\n", error->message);
		g_error_free(error);
		return;
	}
	
	purple_debug_info("telepathy", "New channel: %s\n", object_Path);

	tp_channel_call_when_ready(channel, channel_ready_cb, data);

	g_signal_connect(channel, "invalidated", G_CALLBACK (channel_invalidated_cb), data);
}

void
new_channels_cb (TpConnection *proxy,
                 const GPtrArray *arg_Channels,
                 gpointer user_data,
                 GObject *weak_object)
{
	int i;
	telepathy_connection *data = user_data;

	if (data->listing_channels)
		return;

	for (i = 0; i < arg_Channels->len; i++)
	{
		GValueArray *channel = g_ptr_array_index(arg_Channels, i);

		handle_new_channel(user_data, channel);
	}
}

void
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
		telepathy_connection *data = user_data;

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

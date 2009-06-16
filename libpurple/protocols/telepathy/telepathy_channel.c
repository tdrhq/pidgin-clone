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

#include <telepathy-glib/interfaces.h>

#include "internal.h"

#include "debug.h"

#include "telepathy_avatar.h"
#include "telepathy_channel_list.h"
#include "telepathy_channel_text.h"
#include "telepathy_contact.h"

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
	TpChannel *channel = tp_channel_new_from_properties(connection, object_Path, map, &error);

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

	purple_debug_info("telepathy", "NewChannels:\n");

	for (i = 0; i < arg_Channels->len; i++)
	{
		GValueArray *channel = g_ptr_array_index(arg_Channels, i);

		handle_new_channel(user_data, channel);
	}
}

void
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

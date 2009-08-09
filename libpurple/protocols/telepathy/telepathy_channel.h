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

#ifndef _TELEPATHY_CHANNEL_H_
#define _TELEPATHY_CHANNEL_H_

#include <glib.h>

#include <telepathy-glib/channel.h>
#include <telepathy-glib/channel-dispatcher.h>

#include "telepathy_connection.h"

void
create_channel_cb (TpChannelDispatcher *proxy,
                   const gchar *out_Request,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object);

void
channel_ready_cb (TpChannel *channel,
                  const GError *error,
                  gpointer user_data);

void
channel_invalidated_cb (TpProxy *self,
                        guint    domain,
                        gint     code,
                        gchar   *message,
                        gpointer user_data);

void
handle_new_channel (telepathy_connection *data,
                    const GValueArray *channel_Properties);

void
new_channels_cb (TpConnection *proxy,
                 const GPtrArray *arg_Channels,
                 gpointer user_data,
                 GObject *weak_object);

void
ensure_channel_cb (TpConnection *proxy,
                   gboolean out_Yours,
                   const gchar *out_Channel,
                   GHashTable *out_Properties,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object);

void
get_channels_cb (TpProxy *proxy,
                 const GValue *out_Value,
                 const GError *error,
                 gpointer user_data,
                 GObject *weak_object);

#endif /* _TELEPATHY_CHANNEL_H_ */

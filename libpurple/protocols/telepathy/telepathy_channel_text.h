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

#ifndef _TELEPATHY_CHANNEL_TEXT_H_
#define _TELEPATHY_CHANNEL_TEXT_H_

#include <glib.h>

#include <telepathy-glib/channel.h>

#include "telepathy_connection.h"

typedef struct
{
	TpChannel *channel;

	telepathy_connection *connection_data;

	GHashTable *contacts;

	TpHandle self_handle;

} telepathy_room_channel;

void
destroy_room_channel(telepathy_room_channel *tp_channel);

typedef struct
{
	guint msg_id;
	guint timestamp;
	guint sender;
	guint type;
	guint flags;
	gchar *msg;

} telepathy_message;

typedef struct
{
	GArray *messages;
	GArray *handles;

	telepathy_room_channel *channel_data;

} telepathy_scrollback_messages;

typedef struct
{
	telepathy_room_channel *tp_channel;
	gchar *msg;

} telepathy_chat_invitation;

typedef struct
{
	GList *pending_Messages;
	TpChannel *channel;

	/* This flag avoids having a message processed twice via both Received signal and ListPendingMessages */
	gboolean received_Pending_Messages;
} telepathy_text_channel;

void
destroy_text_channel(telepathy_text_channel *tp_channel);

void
handle_room_text_channel (TpChannel *channel,
                          telepathy_connection *data,
                          gboolean invitation);
	
void
chat_send_cb (TpChannel *proxy,
              const GError *error,
              gpointer user_data,
              GObject *weak_object);

void
write_message_to_conversation (const gchar *from,
                               guint timestamp,
			       const gchar *msg,
			       gpointer user_data);

void
acknowledge_pending_messages_cb (TpChannel *proxy,
                                 const GError *error,
				 gpointer user_data,
				 GObject *weak_object);

void
list_pending_messages_cb  (TpChannel *proxy,
                           const GPtrArray *out_Pending_Messages,
                           const GError *error,
                           gpointer user_data,
                           GObject *weak_object);

void
received_cb (TpChannel *proxy,
             guint arg_ID,
             guint arg_Timestamp,
             guint arg_Sender,
             guint arg_Type,
             guint arg_Flags,
             const gchar *arg_Text,
             gpointer user_data,
             GObject *weak_object);

void
send_error_cb (TpChannel *proxy,
               guint arg_Error,
               guint arg_Timestamp,
               guint arg_Type,
               const gchar *arg_Text,
               gpointer user_data,
               GObject *weak_object);

void
send_cb (TpChannel *proxy,
         const GError *error,
         gpointer user_data,
         GObject *weak_object);

void
text_channel_invalidated_cb (TpProxy *self,
                             guint    domain,
                             gint     code,
                             gchar   *message,
                             gpointer user_data);

void
chat_state_changed_cb (TpChannel *proxy,
                       guint arg_Contact,
                       guint arg_State,
                       gpointer user_data,
                       GObject *weak_object);

void
handle_text_channel (TpChannel *channel,
                     telepathy_connection *data);

void
set_chat_state_cb (TpChannel *proxy,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object);

#endif /* _TELEPATHY_CHANNEL_TEXT_H_ */

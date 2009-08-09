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

#ifndef _TELEPATHY_CONNECTION_H_
#define _TELEPATHY_CONNECTION_H_

#include <glib.h>

#include <telepathy-glib/channel.h>
#include <telepathy-glib/connection-manager.h>
#include <telepathy-glib/connection.h>
#include <telepathy-glib/contact.h>

#include "connection.h"

typedef struct
{
	TpConnection *connection;
	PurpleConnection *gc;
	PurpleAccount *acct;

	gpointer account_data;

	TpHandle self_handle;

	/* This flag avoids having a channel processed twice via both NewChannels and quering the Channels property */
	gboolean listing_channels;

	/* This will hold pointers to telepathy_text_channel for buddies that have an active conversation */
	GHashTable *text_Channels;
	
	/* This will map room handles to telepathy_room_channel for active chat rooms */
	GHashTable *room_Channels;
	
	/* This will map contact handles to telepathy_contact */
	GHashTable *contacts;

	/* This will map group name to telepathy_group structs */
	GHashTable *groups;

	/* This will map list name to telepathy_group structs */
	GHashTable *lists;

	/* This will map a group name to a buddy to be added to that group.
	 * It's needed when a new group needs to be created as a request to add a buddy.
	 */
	GHashTable *buddy_to_be_added;
	
} telepathy_connection;

void
connection_status_changed (gpointer account_data,
                           guint status,
			   guint reason);

void
got_connection_object (gchar *connection_object,
                       gpointer account_data);

void
status_changed_cb (TpConnection *proxy,
                   guint arg_Status,
		   guint arg_Reason,
		   gpointer user_data,
		   GObject *weak_object);

void
set_presence_cb (TpConnection *proxy,
                 const GError *error,
                 gpointer user_data,
                 GObject *weak_object);

void
connection_connect_cb (TpConnection *proxy,
                       const GError *error,
                       gpointer user_data,
                       GObject *weak_object);

void
request_connection_cb (TpConnectionManager *proxy,
                       const gchar *out_Bus_Name,
                       const gchar *out_Object_Path,
                       const GError *error,
                       gpointer user_data,
                       GObject *weak_object);

void
connection_ready_cb (TpConnection *connection,
                     const GError *error,
                     gpointer user_data);

#endif /* _TELEPATHY_CONNECTION_H_ */

/**
 * purple - Telepathy Protocol Plugin
 *
 * Copyright (C); 2009, Felix Kerekes <sttwister@soc.pidgin.im>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option); any later version.
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

#ifndef _TELEPATHY_CONTACT_H_
#define _TELEPATHY_CONTACT_H_

#include <telepathy-glib/contact.h>

#include "telepathy_connection.h"

typedef struct
{
	TpContact *contact;

	gboolean requested_avatar;
} telepathy_contact;

void
destroy_contact(telepathy_contact *contact_data);

void
remove_contact_cb (TpConnection *connection,
                   TpHandleType handle_type,
                   guint n_handles,
                   const TpHandle *handles,
                   const gchar * const *ids,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object);

void
add_contact_to_group_cb (TpConnection *connection,
                         TpHandleType handle_type,
                         guint n_handles,
                         const TpHandle *handles,
                         const gchar * const *ids,
                         const GError *error,
                         gpointer user_data,
                         GObject *weak_object);

void
contact_notify_cb (TpContact *contact,
		   GParamSpec *pspec,
		   gpointer user_data);

void
handle_contacts (telepathy_connection *connection_data,
                 guint n_contacts,
		 TpContact * const *contacts,
		 guint n_failed,
		 PurpleGroup *group);

/* this the ContactsReady callback for group channels */
void
group_contacts_ready_cb (TpConnection *connection,
                         guint n_contacts,
                         TpContact * const *contacts,
                         guint n_failed,
                         const TpHandle *failed,
                         const GError *error,
                         gpointer user_data,
                         GObject *weak_object);

/* this the ContactsReady callback for list channels */
void
contacts_ready_cb (TpConnection *connection,
                   guint n_contacts,
                   TpContact * const *contacts,
                   guint n_failed,
                   const TpHandle *failed,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object);

#endif /* _TELEPATHY_CONTACT_H_ */

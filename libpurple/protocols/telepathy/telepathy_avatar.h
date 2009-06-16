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

#ifndef _TELEPATHY_AVATAR_H_
#define _TELEPATHY_AVATAR_H_

#include <telepathy-glib/connection.h>

void
request_avatars_cb (TpConnection *proxy,
                    const GError *error,
                    gpointer user_data,
                    GObject *weak_object);

void
get_known_avatar_tokens_cb (TpConnection *proxy,
                            GHashTable *out_Tokens,
                            const GError *error,
                            gpointer user_data,
                            GObject *weak_object);

void
avatar_retrieved_cb (TpConnection *proxy,
                     guint arg_Contact,
                     const gchar *arg_Token,
                     const GArray *arg_Avatar,
                     const gchar *arg_Type,
                     gpointer user_data,
                     GObject *weak_object);

void
avatar_updated_cb (TpConnection *proxy,
                   guint arg_Contact,
                   const gchar *arg_New_Avatar_Token,
                   gpointer user_data,
                   GObject *weak_object);

void
get_avatar_properties_cb (TpProxy *proxy,
                          GHashTable *out_Properties,
                          const GError *error,
                          gpointer user_data,
                          GObject *weak_object);

void
set_avatar_cb (TpConnection *proxy,
               const gchar *out_Token,
               const GError *error,
               gpointer user_data,
               GObject *weak_object);

void
clear_avatar_cb (TpConnection *proxy,
                 const GError *error,
                 gpointer user_data,
                 GObject *weak_object);


#endif /* _TELEPATHY_AVATAR_H_ */

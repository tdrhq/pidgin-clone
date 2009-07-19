/* purple
 *
 * Purple is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
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
#ifndef PURPLE_SIGNALS_H
#define PURPLE_SIGNALS_H

#include <glib.h>
#include <glib-object.h>

#define PURPLE_TYPE_SIGNAL_HANDLE (purple_signal_handle_get_type())

typedef struct _PurpleGSignalHandle PurpleGSignalHandle;

G_BEGIN_DECLS

#define purple_g_signal_connect(type, name, callback, data) \
	(purple_g_signal_connect_flags((type), (name), 0, (callback), (data))

PurpleGSignalHandle *purple_g_signal_connect_flags(GType type, const gchar *name, GConnectFlags flags, GCallback callback, gpointer data);
void purple_g_signal_disconnect(PurpleGSignalHandle *handle);

GType purple_g_signal_handle_get_gtype(void);
PurpleGSignalHandle *purple_g_signal_handle_new(guint signal_id, gulong hook_id);
PurpleGSignalHandle *purple_g_signal_handle_copy(const PurpleGSignalHandle *handle);
void purple_g_signal_handle_free(PurpleGSignalHandle *handle);

G_END_DECLS

#endif /* PURPLE_SIGNALS_H */

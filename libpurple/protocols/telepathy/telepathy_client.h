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

#ifndef _TELEPATHY_CLIENT_H_
#define _TELEPATHY_CLIENT_H_

#include <glib.h>
#include <gio/gio.h>

#include <telepathy-glib/dbus-properties-mixin.h>

#define TELEPATHY_TYPE_CLIENT         (telepathy_client_get_type ())
#define TELEPATHY_CLIENT(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), TELEPATHY_TYPE_CLIENT, TelepathyClient))
#define TELEPATHY_CLIENT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), TELEPATHY_TYPE_CLIENT, TelepathyClientClass))
#define TELEPATHY_IS_CLIENT(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), TELEPATHY_TYPE_CLIENT))
#define TELEPATHY_IS_CLIENT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), TELEPATHY_TYPE_CLIENT))
#define TELEPATHY_CLIENT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TELEPATHY_TYPE_CLIENT, TelepathyClientClass))

//typedef struct _TelepathyClient      TelepathyClient;
//typedef struct _TelepathyClientClass TelepathyClientClass;

typedef struct 
{
	GObject parent;
	gpointer priv;
} TelepathyClient;

typedef struct 
{
	GObjectClass parent_class;
	TpDBusPropertiesMixinClass dbus_props_class;
} TelepathyClientClass;

GType telepathy_client_get_type (void) G_GNUC_CONST;

TelepathyClient *    telepathy_client_dup_singleton (void);

#endif /* _TELEPATHY_CLIENT_H_ */

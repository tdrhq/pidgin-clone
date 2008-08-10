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
#ifndef PURPLE_OBJECT_H
#define PURPLE_OBJECT_H

#include <glib.h>
#include <glib-object.h>

#define PURPLE_TYPE_OBJECT              (purple_object_get_type())
#define PURPLE_OBJECT(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), PURPLE_TYPE_OBJECT, PurpleObject))
#define PURPLE_OBJECT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), PURPLE_TYPE_OBJECT, PurpleObjectClass))
#define PURPLE_IS_OBJECT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), PURPLE_TYPE_OBJECT))
#define PURPLE_IS_OBJECT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), PURPLE_TYPE_OBJECT))
#define PURPLE_OBJECT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), PURPLE_TYPE_OBJECT, PurpleObjectClass))

typedef struct _PurpleObject        PurpleObject;
typedef struct _PurpleObjectPrivate PurpleObjectPrivate;
typedef struct _PurpleObjectClass   PurpleObjectClass;

struct _PurpleObject
{
    GObject gparent;
    PurpleObjectPrivate *priv;
    void (*_reserved[4])(void);
};

struct _PurpleObjectClass
{
	GObjectClass gparent;

	void (*_purple_reserved[4])(void);
};

G_BEGIN_DECLS

GType purple_object_get_type(void);

void purple_object_set_protocol_data(PurpleObject *pobj, gpointer proto_data);

gpointer purple_object_get_protocol_data(PurpleObject *pobj);

void purple_object_set_ui_data(PurpleObject *pobj, gpointer ui_data);

gpointer purple_object_get_ui_data(PurpleObject *pobj);

int purple_object_get_int(PurpleObject *pobj, const char *prop);

gulong purple_type_connect(GType type, const char *signal, GCallback callback, gpointer data);

gulong purple_type_connect_flags(GType type, const char *signal, GCallback callback,
		gpointer data, GConnectFlags flags);

G_END_DECLS

#endif /* PURPLE_OBJECT_H */

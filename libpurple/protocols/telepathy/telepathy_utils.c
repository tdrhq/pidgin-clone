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

#include "telepathy_utils.h"

#include <telepathy-glib/util.h>

#include "internal.h"

#include "debug.h"

/* TODO: Check for other types of statuses too */
GValueArray *
purple_status_to_telepathy_status(PurpleStatus *status)
{
	GValueArray *presence = g_value_array_new(3);

	PurpleStatusType *type = purple_status_get_type(status);
	const gchar *status_id = purple_status_get_id(status);
	const gchar *presence_message = purple_status_get_attr_string(status, "message");

	if (purple_status_type_get_primitive(type) == PURPLE_STATUS_AVAILABLE)
		g_value_array_append(presence, tp_g_value_slice_new_uint(2)); /* online */
	else
		g_value_array_append(presence, tp_g_value_slice_new_uint(1)); /* offline */

	g_value_array_append(presence, tp_g_value_slice_new_string(status_id));
	g_value_array_append(presence, tp_g_value_slice_new_string(presence_message));

	return presence;
}

void
set_properties_cb (TpProxy *proxy,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_info("telepathy", "SetProperties error: %s\n", error->message);
		return;
	}

	purple_debug_info("telepathy", "SetProperties succeeded!\n");
}

/* transform a telepathy parameter name into a user-friendly one */
gchar*
telepathy_transform_param_name(const gchar* param)
{
	gchar *name;
	int i,len;

	g_return_val_if_fail(param != NULL, NULL);

	name = g_strdup(param);

	/* capitalize first letter */
	name[0] = g_ascii_toupper(name[0]);

	len = strlen(name);
	for (i = 0; i<len; ++i)
	{
	    if (name[i] == '-')
	    {
		name[i] = ' ';
		if (i+1 < len)
		{
		    /* capitalize first letter of each word */
		    name[i+1] = g_ascii_toupper(name[i+1]);
		}
	    }
	}
	return name;
}


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

#ifndef _TELEPATHY_UTILS_H_
#define _TELEPATHY_UTILS_H_

#include <glib.h>

#include "internal.h"

#include "status.h"

#define TELEPATHY_ID "prpl-telepathy"
#define TELEPATHY_DISPLAY_VERSION "1.0"

typedef struct
{
	const gchar *telepathy_name;
	const gchar *dbus_type;
	const gchar *human_name;
} OptionMapping;

static const OptionMapping options[] = {
	{ "server", "s", N_("Server")},
	{ "port", "q", N_("Port")},
	{ "require-encryption", "b", N_("Require Encryption")},
	{ "ident", "s", N_("Ident")},
	{ "fullname", "s", N_("Full Name")},
	{ "stun-server", "s", N_("STUN Server")},
	{ "stun-port", "q", N_("STUN Port")},
	{ NULL, NULL, NULL}
};

typedef struct
{
	const guint status_primitive;
	const gchar* id;
	const gchar* description;
} StatusMapping;

static const StatusMapping statuses[] = 
{
	{ PURPLE_STATUS_AVAILABLE, "available", "Available" },
	{ PURPLE_STATUS_AWAY, "away", N_("Away") },
	{ PURPLE_STATUS_AWAY, "brb", N_("Be right back") },
	{ PURPLE_STATUS_UNAVAILABLE, "dnd", N_("Do not disturb") },
	{ PURPLE_STATUS_UNAVAILABLE, "busy", N_("Busy") },
	{ PURPLE_STATUS_EXTENDED_AWAY, "xa", N_("Extended away") },
	{ PURPLE_STATUS_INVISIBLE, "hidden", NULL },
	{ PURPLE_STATUS_OFFLINE, "offline", NULL },
	{ PURPLE_STATUS_UNSET, "unknown", NULL },
	{ PURPLE_STATUS_UNSET, "error", NULL },
	{ 0, NULL, NULL}
};

gchar*
telepathy_transform_param_name(const gchar* param);

#endif /* _TELEPATHY_UTILS_H_ */

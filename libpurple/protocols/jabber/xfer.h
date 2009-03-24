/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 
#ifndef JABBER_XFER_H
#define JABBER_XFER_H
 
#include <glib.h>

#include "connection.h"
#include "ft.h"
 
PurpleXfer *jabber_xfer_new(PurpleConnection *gc, const char *who);
void jabber_xfer_send(PurpleConnection *gc, const char *who, const char *file);
 
xmlnode *jabber_xfer_create_file_element(const PurpleXfer *xfer);
PurpleXfer *jabber_xfer_create_from_xml(const PurpleAccount *account,
	const xmlnode *file, const gchar *who, gpointer data);
 
#endif /* JABBER_XFER_H */
 

/*
 * gaim-remote
 *
 * Gaim is the legal property of its developers, whose names are too numerous
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _GAIM_SOCKET_H_
#define _GAIM_SOCKET_H_

struct gaim_cui_packet {
	guchar type;
	guchar subtype;
	guint32 length;
	char *data;
};


void cui_send_packet (int fd, struct gaim_cui_packet *p);
gint gaim_connect_to_session(gint session);
gboolean gaim_session_exists(int sess);

struct gaim_cui_packet *cui_packet_new(guchar type, guchar subtype);
void cui_packet_free(struct gaim_cui_packet *p);
void cui_packet_append_string(struct gaim_cui_packet *p, char *str);
void cui_packet_append_char(struct gaim_cui_packet *p, char c);
void cui_packet_append_raw(struct gaim_cui_packet *p, char *str, int len);


#endif /* _GAIM_SOCKET_H_ */

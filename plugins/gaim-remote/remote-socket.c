/*
 * gaim-remote
 *
 * Copyright (C) 2002, Sean Egan <bj91704@binghamton.edu>
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

/* Somewhat inspired by XMMS:
 *  Copyright (C) 1998-2002  Peter Alm, Mikael Alm, Olle Hallnas,
 *                           Thomas Nilsson and 4Front Technologies
 *  Copyright (C) 1999-2002  Haavard Kvaalen 
 */
 
/* This provides code for connecting to a Gaim socket and communicating with
 * it.  It will eventually be made a library once the core and ui are split. */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "gaim.h"
#include "remote-socket.h"

void
gaim_remote_session_send_packet(int fd, GaimRemotePacket *p)
{
	int len = sizeof(p->type) + sizeof(p->subtype) +
	          sizeof(p->length) + p->length;
	char *pack = g_malloc(len);
	char *a = pack;

	memcpy (a, &(p->type), sizeof(p->type));
	a = a + sizeof(p->type);
	memcpy (a, &(p->subtype), sizeof(p->subtype));
	a = a + sizeof(p->subtype);
	memcpy (a, &(p->length), sizeof(p->length));
	a = a + sizeof(p->length);
	memcpy (a, p->data, p->length);
	write(fd, pack, len);
	g_free(pack);
}

void
gaim_remote_packet_append_string(GaimRemotePacket *p, char *str)
{
	int len = p->length + strlen(str);
	char *k = g_malloc(len);

	memcpy(k, p->data, p->length);
	memcpy(k + p->length, str, strlen(str));

	if (p->data)
		g_free(p->data);

	p->data = k;
	p->length = len;
}

void
gaim_remote_packet_append_char(GaimRemotePacket *p, char c)
{
	int len = p->length + sizeof(char);
	char *k = g_malloc(len);

	memcpy(k, p->data, p->length);
	k[p->length] = c;

	if (p->data)
		g_free(p->data);

	p->data = k;
	p->length = len;
}

void
gaim_remote_packet_append_raw(GaimRemotePacket *p, char *str, int len)
{
	int lent = p->length + len;
	char *k = g_malloc(lent);

	memcpy(k, p->data, p->length);
	memcpy(k + p->length, str, len);

	if (p->data)
		g_free(p->data);

	p->data = k;
	p->length = lent;
}

GaimRemotePacket *
gaim_remote_packet_new(guchar type, guchar subtype)
{
	GaimRemotePacket *p = g_new0(GaimRemotePacket, 1);
	p->type = type;
	p->subtype = subtype;
	p->length = 0;
	p->data = NULL;
	return p;
}

void
gaim_remote_packet_free(GaimRemotePacket *p)
{
	if (p->data)
		g_free(p->data);
	g_free(p);
}

GaimRemotePacket *
gaim_remote_session_read_packet(int fd)
{
	GaimRemotePacket *p = g_new0(GaimRemotePacket, 1);
	char *data = NULL;

	if (!(read(fd, &p->type, sizeof(p->type)))) {
		g_free(p);
		return NULL;
	}

	if (!(read(fd, &p->subtype, sizeof(p->subtype)))) {
		g_free(p);
		return NULL;
	}

	if (!(read(fd, &p->length, sizeof(p->length)))) {
		g_free(p);
		return NULL;
	}
	
	if (p->length) {
		data = g_malloc(p->length);

		if (!(read(fd, data, p->length))) {
			g_free(p);
			return NULL;
		}
	}

	p->data = data;

	return p;
}

/* copied directly from xmms_connect_to_session */
int
gaim_remote_session_connect(int session)
{
	gint fd;
	uid_t stored_uid, euid;
	struct sockaddr_un saddr;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) != -1)
	{
		saddr.sun_family = AF_UNIX;
		stored_uid = getuid();
		euid = geteuid();
		setuid(euid);
		sprintf(saddr.sun_path, "%s/gaim_%s.%d",
				g_get_tmp_dir(), g_get_user_name(), session);
		setreuid(stored_uid, euid);

		if (connect(fd, (struct sockaddr *) &saddr, sizeof (saddr)) != -1)
			return fd;
	}

	close(fd);

	return -1;
}

gboolean
gaim_remote_session_exists(int sess)
{
	GaimRemotePacket *pack = NULL;
	int fd = gaim_remote_session_connect(sess);

	if (fd > 0) {
		pack = gaim_remote_packet_new(CUI_TYPE_META, CUI_META_PING);
		gaim_remote_session_send_packet(fd, pack);
		gaim_remote_packet_free(pack);
		close(fd);
		
		return TRUE;
	}

	return FALSE;
}


/*
 * @file yahoo_filexfer.c Yahoo Filetransfer
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
 */

#include "prpl.h"
#include "internal.h"
#include "util.h"
#include "debug.h"
#include "notify.h"
#include "proxy.h"
#include "ft.h"
#include "yahoo.h"
#include "yahoo_filexfer.h"



struct yahoo_xfer_data {
	gchar *host;
	gchar *path;
	int port;
	GaimConnection *gc;
	long expires;
	gboolean started;
	gchar *rxqueue;
	guint rxlen;
};

static void yahoo_xfer_data_free(struct yahoo_xfer_data *xd)
{
	if (xd->host)
		g_free(xd->host);
	if (xd->path)
		g_free(xd->path);
	g_free(xd);
}

static void yahoo_receivefile_connected(gpointer data, gint source, GaimInputCondition condition)
{
	GaimXfer *xfer;
	struct yahoo_xfer_data *xd;
	gchar *buf;

	gaim_debug(GAIM_DEBUG_INFO, "yahoo",
			   "AAA - in yahoo_receivefile_connected\n");
	if (!(xfer = data))
		return;
	if (!(xd = xfer->data))
		return;
	if (source < 0) {
		gaim_xfer_error(GAIM_XFER_RECEIVE, xfer->who, _("Unable to connect."));
		gaim_xfer_cancel_remote(xfer);
		return;
	}

	xfer->fd = source;
	gaim_xfer_start(xfer, source, NULL, 0);

	buf = g_strdup_printf("GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n", xd->path, xd->host);
	write(xfer->fd, buf, strlen(buf));
	g_free(buf);

	return;
}

static int yahoo_send_packet_special(int fd, struct yahoo_packet *pkt, int pad)
{
	int pktlen = yahoo_packet_length(pkt);
	int len = YAHOO_PACKET_HDRLEN + pktlen;
	int ret;

	guchar *data;
	int pos = 0;

	if (fd < 0)
		return -1;

	data = g_malloc0(len + 1);

	memcpy(data + pos, "YMSG", 4); pos += 4;
	pos += yahoo_put16(data + pos, YAHOO_PROTO_VER);
	pos += yahoo_put16(data + pos, 0x0000);
	pos += yahoo_put16(data + pos, pktlen + pad);
	pos += yahoo_put16(data + pos, pkt->service);
	pos += yahoo_put32(data + pos, pkt->status);
	pos += yahoo_put32(data + pos, pkt->id);

	yahoo_packet_write(pkt, data + pos);

	ret = write(fd, data, len);
	g_free(data);

	return ret;
}

static void yahoo_sendfile_connected(gpointer data, gint source, GaimInputCondition condition)
{
	GaimXfer *xfer;
	struct yahoo_xfer_data *xd;
	struct yahoo_packet *pkt;
	gchar *size, *post, *buf;
	int content_length;
	GaimConnection *gc;
	GaimAccount *account;
	struct yahoo_data *yd;
	char *filename;

	gaim_debug(GAIM_DEBUG_INFO, "yahoo",
			   "AAA - in yahoo_sendfile_connected\n");
	if (!(xfer = data))
		return;
	if (!(xd = xfer->data))
		return;

	gc = xd->gc;
	account = gaim_connection_get_account(gc);
	yd = gc->proto_data;



	if (source < 0) {
		gaim_xfer_error(GAIM_XFER_RECEIVE, xfer->who, _("Unable to connect."));
		gaim_xfer_cancel_remote(xfer);
		return;
	}

	xfer->fd = source;
	gaim_xfer_start(xfer, source, NULL, 0);


	pkt = yahoo_packet_new(YAHOO_SERVICE_FILETRANSFER, YAHOO_STATUS_AVAILABLE, yd->session_id);

	size = g_strdup_printf("%d", gaim_xfer_get_size(xfer));

	yahoo_packet_hash(pkt, 0, gaim_connection_get_display_name(gc));
	yahoo_packet_hash(pkt, 5, xfer->who);
	yahoo_packet_hash(pkt, 14, "");
	filename = g_path_get_basename(gaim_xfer_get_local_filename(xfer));
	yahoo_packet_hash(pkt, 27, filename);
	yahoo_packet_hash(pkt, 28, size);

	content_length = YAHOO_PACKET_HDRLEN + yahoo_packet_length(pkt);

	buf = g_strdup_printf("Y=%s; T=%s", yd->cookie_y, yd->cookie_t);

	post = g_strdup_printf("POST /notifyft HTTP/1.0\r\n"
	                       "Content-length: %d\r\n"
	                       "Host: %s:%d\r\n"
	                       "Cookie: %s\r\n"
	                       "\r\n",
	                       content_length + 4 + gaim_xfer_get_size(xfer),
			       gaim_account_get_string(account, "xfer_host", YAHOO_XFER_HOST),
			       gaim_account_get_int(account, "xfer_port", YAHOO_XFER_PORT),
			       buf);
	write(xfer->fd, post, strlen(post));

	yahoo_send_packet_special(xfer->fd, pkt, 8);
	yahoo_packet_free(pkt);

	write(xfer->fd, "29\xc0\x80", 4);

	g_free(size);
	g_free(post);
	g_free(buf);
	g_free(filename);
}

static void yahoo_xfer_init(GaimXfer *xfer)
{
	struct yahoo_xfer_data *xfer_data;
	GaimConnection *gc;
	GaimAccount *account;
	struct yahoo_data *yd;

	xfer_data = xfer->data;
	gc = xfer_data->gc;
	yd = gc->proto_data;
	account = gaim_connection_get_account(gc);

	if (gaim_xfer_get_type(xfer) == GAIM_XFER_SEND) {
		if (gaim_proxy_connect(account, gaim_account_get_string(account, "xfer_host",  YAHOO_XFER_HOST),
		                       gaim_account_get_int(account, "xfer_port", YAHOO_XFER_PORT),
		                       yahoo_sendfile_connected, xfer) == -1)
		{
			gaim_notify_error(gc, NULL, _("File Transfer Aborted"),
			                 _("Unable to establish file descriptor."));
			gaim_xfer_cancel_remote(xfer);
		}
	} else {
		xfer->fd = gaim_proxy_connect(account, xfer_data->host, xfer_data->port,
		                              yahoo_receivefile_connected, xfer);
		if (xfer->fd == -1) {
			gaim_notify_error(gc, NULL, _("File Transfer Aborted"),
			             _("Unable to establish file descriptor."));
			gaim_xfer_cancel_remote(xfer);
		}
	}
}

static void yahoo_xfer_start(GaimXfer *xfer)
{
	/* We don't need to do anything here, do we? */
}

static void yahoo_xfer_end(GaimXfer *xfer)
{
	GaimAccount *account;
	struct yahoo_xfer_data *xfer_data;

	account   = gaim_xfer_get_account(xfer);
	xfer_data = xfer->data;


	if (xfer_data)
		yahoo_xfer_data_free(xfer_data);
	xfer->data = NULL;

}

guint calculate_length(const gchar *l, size_t len)
{
	int i;

	for (i = 0; i < len; i++) {
		if (!g_ascii_isdigit(l[i]))
			continue;
		return strtol(l + i, NULL, 10);
	}
	return 0;
}


ssize_t yahoo_xfer_read(char **buffer, GaimXfer *xfer)
{
	gchar buf[4096];
	ssize_t len;
	gchar *start = NULL;
	gchar *length;
	gchar *end;
	int filelen;
	struct yahoo_xfer_data *xd = xfer->data;

	if (gaim_xfer_get_type(xfer) != GAIM_XFER_RECEIVE) {
		return 0;
	}

	len = read(xfer->fd, buf, sizeof(buf));

	if (len <= 0) {
		if ((gaim_xfer_get_size(xfer) > 0) &&
		   (gaim_xfer_get_bytes_sent(xfer) >= gaim_xfer_get_size(xfer)))
			gaim_xfer_set_completed(xfer, TRUE);
		else
			gaim_xfer_cancel_remote(xfer);
		return 0;
	}


	if (!xd->started) {
		xd->rxqueue = g_realloc(xd->rxqueue, len + xd->rxlen);
		memcpy(xd->rxqueue + xd->rxlen, buf, len);
		xd->rxlen += len;

		length = g_strstr_len(xd->rxqueue, len, "Content-length:");
		if (length) {
			end = g_strstr_len(length, length - xd->rxqueue, "\r\n");
			if (!end)
				return 0;
			if ((filelen = calculate_length(length, len - (length - xd->rxqueue))))
				gaim_xfer_set_size(xfer, filelen);
		}
		start = g_strstr_len(xd->rxqueue, len, "\r\n\r\n");
		if (start)
			start += 4;
		if (!start || start > (xd->rxqueue + len))
			return 0;
		xd->started = TRUE;

		len -= (start - xd->rxqueue);

		*buffer = g_malloc(len);
		memcpy(*buffer, start, len);
		g_free(xd->rxqueue);
		xd->rxqueue = NULL;
		xd->rxlen = 0;
	} else {
		*buffer = g_malloc(len);
		memcpy(*buffer, buf, len);
	}

	return len;
}

ssize_t yahoo_xfer_write(const char *buffer, size_t size, GaimXfer *xfer)
{
	ssize_t len;
	struct yahoo_xfer_data *xd = xfer->data;

	if (!xd)
		return 0;

	if (gaim_xfer_get_type(xfer) != GAIM_XFER_SEND) {
		return 0;
	}

	len = write(xfer->fd, buffer, size);

	if (len == -1) {
		if (gaim_xfer_get_bytes_sent(xfer) >= gaim_xfer_get_size(xfer))
			gaim_xfer_set_completed(xfer, TRUE);
		if ((errno != EAGAIN) && (errno != EINTR))
			gaim_xfer_cancel_remote(xfer);
		return 0;
	}

	if ((gaim_xfer_get_bytes_sent(xfer) + len) >= gaim_xfer_get_size(xfer))
		gaim_xfer_set_completed(xfer, TRUE);

	return len;
}

static void yahoo_xfer_cancel_send(GaimXfer *xfer)
{
	GaimAccount *account;
	struct yahoo_xfer_data *xfer_data;

	xfer_data = xfer->data;
	account   = gaim_xfer_get_account(xfer);

	if (xfer_data)
		yahoo_xfer_data_free(xfer_data);
	xfer->data = NULL;
}

static void yahoo_xfer_cancel_recv(GaimXfer *xfer)
{
	GaimAccount *account;
	struct yahoo_xfer_data *xfer_data;

	account   = gaim_xfer_get_account(xfer);
	xfer_data = xfer->data;

	if (xfer_data)
		yahoo_xfer_data_free(xfer_data);
	xfer->data = NULL;
}

void yahoo_process_filetransfer(GaimConnection *gc, struct yahoo_packet *pkt)
{
	char *from = NULL;
	char *to = NULL;
	char *msg = NULL;
	char *url = NULL;
	long expires = 0;
	GaimXfer *xfer;
	struct yahoo_xfer_data *xfer_data;

	char *service = NULL;

	char *filename = NULL;
	unsigned long filesize = 0L;

	GSList *l;

	for (l = pkt->hash; l; l = l->next) {
		struct yahoo_pair *pair = l->data;

		if (pair->key == 4)
			from = pair->value;
		if (pair->key == 5)
			to = pair->value;
		if (pair->key == 14)
			msg = pair->value;
		if (pair->key == 20)
			url = pair->value;
		if (pair->key == 38)
			expires = strtol(pair->value, NULL, 10);

		if (pair->key == 27)
			filename = pair->value;
		if (pair->key == 28)
			filesize = atol(pair->value);

		if (pair->key == 49)
			service = pair->value;
	}

	if (pkt->service == YAHOO_SERVICE_P2PFILEXFER) {
		if (strcmp("FILEXFER", service) != 0) {
			gaim_debug_misc("yahoo", "unhandled service 0x%02x", pkt->service);
			return;
		}
	}

	if (msg) {
		char *tmp;
		tmp = strchr(msg, '\006');
		if (tmp)
			*tmp = '\0';
	}

	if (!url || !from)
		return;


	/* Setup the Yahoo-specific file transfer data */
	xfer_data = g_new0(struct yahoo_xfer_data, 1);
	xfer_data->gc = gc;
	if (!gaim_url_parse(url, &(xfer_data->host), &(xfer_data->port), &(xfer_data->path))) {
		g_free(xfer_data);
		return;
	}

	gaim_debug_misc("yahoo_filexfer", "Host is %s, port is %d, path is %s, and the full url was %s.\n",
	                xfer_data->host, xfer_data->port, xfer_data->path, url);

	/* Build the file transfer handle. */
	xfer = gaim_xfer_new(gc->account, GAIM_XFER_RECEIVE, from);
	xfer->data = xfer_data;

	/* Set the info about the incoming file. */
	if (filename)
		gaim_xfer_set_filename(xfer, filename);
	else {
		gchar *start, *end;
	 	start = g_strrstr(xfer_data->path, "/");
		if (start)
			start++;
		end = g_strrstr(xfer_data->path, "?");
		if (start && *start && end) {
			filename = g_strndup(start, end - start);
			gaim_xfer_set_filename(xfer, filename);
			g_free(filename);
			filename = NULL;
		}
	}

	gaim_xfer_set_size(xfer, filesize);

	/* Setup our I/O op functions */
	gaim_xfer_set_init_fnc(xfer,        yahoo_xfer_init);
	gaim_xfer_set_start_fnc(xfer,       yahoo_xfer_start);
	gaim_xfer_set_end_fnc(xfer,         yahoo_xfer_end);
	gaim_xfer_set_cancel_send_fnc(xfer, yahoo_xfer_cancel_send);
	gaim_xfer_set_cancel_recv_fnc(xfer, yahoo_xfer_cancel_recv);
	gaim_xfer_set_read_fnc(xfer,        yahoo_xfer_read);
	gaim_xfer_set_write_fnc(xfer,       yahoo_xfer_write);

	/* Now perform the request */
	gaim_xfer_request(xfer);
}

void yahoo_ask_send_file(GaimConnection *gc, const char *who)
{
	GaimXfer *xfer;
	struct yahoo_xfer_data *xfer_data;

	xfer_data = g_new0(struct yahoo_xfer_data, 1);
	xfer_data->gc = gc;


	/* Build the file transfer handle. */
	xfer = gaim_xfer_new(gc->account, GAIM_XFER_SEND, who);
	xfer->data = xfer_data;

	/* Setup our I/O op functions */
	gaim_xfer_set_init_fnc(xfer,        yahoo_xfer_init);
	gaim_xfer_set_start_fnc(xfer,       yahoo_xfer_start);
	gaim_xfer_set_end_fnc(xfer,         yahoo_xfer_end);
	gaim_xfer_set_cancel_send_fnc(xfer, yahoo_xfer_cancel_send);
	gaim_xfer_set_cancel_recv_fnc(xfer, yahoo_xfer_cancel_recv);
	gaim_xfer_set_read_fnc(xfer,        yahoo_xfer_read);
	gaim_xfer_set_write_fnc(xfer,       yahoo_xfer_write);

	/* Now perform the request */
	gaim_xfer_request(xfer);
}

void yahoo_send_file(GaimConnection *gc, const char *who, const char *file)
{
	GaimXfer *xfer;
	struct yahoo_xfer_data *xfer_data;

	if (!who || !file)
		return;

	xfer_data = g_new0(struct yahoo_xfer_data, 1);
	xfer_data->gc = gc;


	/* Build the file transfer handle. */
	xfer = gaim_xfer_new(gc->account, GAIM_XFER_SEND, who);
	xfer->data = xfer_data;

	/* Setup our I/O op functions */
	gaim_xfer_set_init_fnc(xfer,        yahoo_xfer_init);
	gaim_xfer_set_start_fnc(xfer,       yahoo_xfer_start);
	gaim_xfer_set_end_fnc(xfer,         yahoo_xfer_end);
	gaim_xfer_set_cancel_send_fnc(xfer, yahoo_xfer_cancel_send);
	gaim_xfer_set_cancel_recv_fnc(xfer, yahoo_xfer_cancel_recv);
	gaim_xfer_set_read_fnc(xfer,        yahoo_xfer_read);
	gaim_xfer_set_write_fnc(xfer,       yahoo_xfer_write);

	/* Now perform the request */
	gaim_xfer_request_accepted(xfer, file);
}

/*
 * gaim - Jabber Protocol Plugin
 *
 * Copyright (C) 2003, Nathan Walp <faceprint@faceprint.com>
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
#include "internal.h"
#include "debug.h"
#include "ft.h"
#include "util.h"

#include "jabber.h"
#include "iq.h"

typedef struct _JabberOOBXfer {
	char *address;
	int port;
	char *page;

	GString *headers;
	gboolean newline;

	char *iq_id;

	JabberStream *js;

} JabberOOBXfer;

static void jabber_oob_xfer_init(GaimXfer *xfer)
{
	JabberOOBXfer *jox = xfer->data;
	gaim_xfer_start(xfer, -1, jox->address, jox->port);
}

static void jabber_oob_xfer_free(GaimXfer *xfer)
{
	JabberOOBXfer *jox = xfer->data;
	jox->js->oob_file_transfers = g_list_remove(jox->js->oob_file_transfers,
			xfer);

	g_string_free(jox->headers, TRUE);
	g_free(jox->address);
	g_free(jox->page);
	g_free(jox->iq_id);
	g_free(jox);

	xfer->data = NULL;
}

static void jabber_oob_xfer_end(GaimXfer *xfer)
{
	JabberOOBXfer *jox = xfer->data;
	JabberIq *iq;

	iq = jabber_iq_new(jox->js, JABBER_IQ_RESULT);
	xmlnode_set_attrib(iq->node, "to", xfer->who);
	jabber_iq_set_id(iq, jox->iq_id);

	jabber_iq_send(iq);

	jabber_oob_xfer_free(xfer);
}

static void jabber_oob_xfer_start(GaimXfer *xfer)
{
	JabberOOBXfer *jox = xfer->data;

	char *buf = g_strdup_printf("GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n",
			jox->page, jox->address);
	write(xfer->fd, buf, strlen(buf));
	g_free(buf);
}

static ssize_t jabber_oob_xfer_read(char **buffer, GaimXfer *xfer) {
	JabberOOBXfer *jox = xfer->data;
	char test;
	int size;

	if(read(xfer->fd, &test, sizeof(test)) > 0) {
		jox->headers = g_string_append_c(jox->headers, test);
		if(test == '\r')
			return 0;
		if(test == '\n') {
			if(jox->newline) {
				gchar *lenstr = strstr(jox->headers->str, "Content-Length: ");
				if(lenstr) {
					sscanf(lenstr, "Content-Length: %d", &size);
					gaim_xfer_set_size(xfer, size);
				}
				gaim_xfer_set_read_fnc(xfer, NULL);
				return 0;
			} else
				jox->newline = TRUE;
				return 0;
			}
		jox->newline = FALSE;
		return 0;
	} else {
		gaim_debug(GAIM_DEBUG_ERROR, "jabber", "Read error on oob xfer!\n");
		gaim_xfer_cancel_local(xfer);
	}

	return 0;
}

static void jabber_oob_xfer_recv_error(GaimXfer *xfer, const char *code) {
	JabberOOBXfer *jox = xfer->data;
	JabberIq *iq;
	xmlnode *y, *z;

	iq = jabber_iq_new(jox->js, JABBER_IQ_ERROR);
	xmlnode_set_attrib(iq->node, "to", xfer->who);
	jabber_iq_set_id(iq, jox->iq_id);
	y = xmlnode_new_child(iq->node, "error");
	xmlnode_set_attrib(y, "code", code);
	if(!strcmp(code, "406")) {
		z = xmlnode_new_child(y, "not-acceptable");
		xmlnode_set_attrib(y, "type", "modify");
		xmlnode_set_attrib(z, "xmlns", "urn:ietf:params:xml:ns:xmpp-stanzas");
	} else if(!strcmp(code, "404")) {
		z = xmlnode_new_child(y, "not-found");
		xmlnode_set_attrib(y, "type", "cancel");
		xmlnode_set_attrib(z, "xmlns", "urn:ietf:params:xml:ns:xmpp-stanzas");
	}
	jabber_iq_send(iq);

	jabber_oob_xfer_free(xfer);
}

static void jabber_oob_xfer_recv_denied(GaimXfer *xfer) {
	jabber_oob_xfer_recv_error(xfer, "406");
}

static void jabber_oob_xfer_recv_canceled(GaimXfer *xfer) {
	jabber_oob_xfer_recv_error(xfer, "404");
}

void jabber_oob_parse(JabberStream *js, xmlnode *packet) {
	JabberOOBXfer *jox;
	GaimXfer *xfer;
	char *filename;
	char *url;
	xmlnode *querynode, *urlnode;

	if(!(querynode = xmlnode_get_child(packet, "query")))
		return;

	if(!(urlnode = xmlnode_get_child(querynode, "url")))
		return;

	url = xmlnode_get_data(urlnode);

	jox = g_new0(JabberOOBXfer, 1);
	gaim_url_parse(url, &jox->address, &jox->port, &jox->page, NULL, NULL);
	g_free(url);
	jox->js = js;
	jox->headers = g_string_new("");
	jox->iq_id = g_strdup(xmlnode_get_attrib(packet, "id"));

	xfer = gaim_xfer_new(js->gc->account, GAIM_XFER_RECEIVE,
			xmlnode_get_attrib(packet, "from"));
	xfer->data = jox;

	if(!(filename = g_strdup(g_strrstr(jox->page, "/"))))
		filename = g_strdup(jox->page);

	gaim_xfer_set_filename(xfer, filename);

	g_free(filename);

	gaim_xfer_set_init_fnc(xfer,   jabber_oob_xfer_init);
	gaim_xfer_set_end_fnc(xfer,    jabber_oob_xfer_end);
	gaim_xfer_set_request_denied_fnc(xfer, jabber_oob_xfer_recv_denied);
	gaim_xfer_set_cancel_recv_fnc(xfer, jabber_oob_xfer_recv_canceled);
	gaim_xfer_set_read_fnc(xfer,   jabber_oob_xfer_read);
	gaim_xfer_set_start_fnc(xfer,  jabber_oob_xfer_start);

	js->oob_file_transfers = g_list_append(js->oob_file_transfers, xfer);

	gaim_xfer_request(xfer);
}



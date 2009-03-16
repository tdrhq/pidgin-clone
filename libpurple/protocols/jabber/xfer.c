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

#include "xfer.h"
#include "debug.h"
#include "buddy.h"
#include "jingle/jingle.h"
#include "jingle/file-transfer.h"
#include "si.h"

static gboolean
jabber_xfer_support_jingle_ft(const PurpleConnection *gc, const gchar *who)
{
	JabberStream *js = (JabberStream *) gc->proto_data;
	JabberBuddy *jb;

	if (!js) {
		purple_debug_error("jabber",
			"jabber_xfer_support_jingle_ft: could not find stream\n");
		return FALSE;
	}

	jb = jabber_buddy_find(js, who, FALSE);
	if (jb) {
		/* should check for s5b or IBB, probably... */
		return jabber_buddy_has_capability(jb, JINGLE_APP_FT);
	} else {
		return FALSE;
	}
}

PurpleXfer *
jabber_xfer_new(PurpleConnection *gc, const char *who)
{
	if (jabber_xfer_support_jingle_ft(gc, who)) {
		/* call in to the jingle code... */
		return jingle_file_transfer_new_xfer(gc, who);
	} else {
		return jabber_si_new_xfer(gc, who);
	}
}

void 
jabber_xfer_send(PurpleConnection *gc, const char *who, const char *file)
{
	if (jabber_xfer_support_jingle_ft(gc, who)) {
		/* call jingle... */
		jingle_file_transfer_send(gc, who, file);
	} else {
		jabber_si_xfer_send(gc, who, file);
	}
}
 
xmlnode *
jabber_xfer_create_file_element(const PurpleXfer *xfer)
{
	xmlnode *file = xmlnode_new("file");
	gchar buf[32];
	
	xmlnode_set_namespace(file,
			"http://jabber.org/protocol/si/profile/file-transfer");
	xmlnode_set_attrib(file, "name", xfer->filename);
	g_snprintf(buf, sizeof(buf), "%" G_GSIZE_FORMAT, xfer->size);
	xmlnode_set_attrib(file, "size", buf);
	/* maybe later we'll do hash and date attribs */
	/* ... and file transfer thumbnails :) */

	return file;
}

PurpleXfer *
jabber_xfer_create_from_xml(const PurpleAccount *account, const xmlnode *file,
	const gchar *who, gpointer data)
{
	const gchar *filename = xmlnode_get_attrib(file, "name");
	const gchar *filesize_c = xmlnode_get_attrib(file, "size");
	gsize filesize;
	PurpleXfer *xfer = NULL;
	
	if (filename == NULL) {
		purple_debug_error("jabber", "got a file transfer with no file name\n");
		return NULL;
	}
	
	if (filesize_c == NULL) {
		purple_debug_error("jabber", "got a file transfer without a size\n");
		return NULL;
	}
	
	filesize = atoi(filesize_c);
	xfer = purple_xfer_new(account, PURPLE_XFER_RECEIVE, who);
	if (xfer) {
		xfer->data = data;
		purple_xfer_set_filename(xfer, filename);
		if (filesize > 0)
			purple_xfer_set_size(xfer, filesize);
	}

	return xfer;
}
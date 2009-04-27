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
#include "request.h"
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
		return jabber_buddy_has_capability(jb, JINGLE_APP_FT) &&
			(jabber_buddy_has_capability(jb, JINGLE_TRANSPORT_S5B) ||
			 jabber_buddy_has_capability(jb, JINGLE_TRANSPORT_IBB));
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

typedef struct {
	PurpleConnection *gc;
	const gchar *who;
	const gchar *file;
} JabberXferData;

static void
jabber_xfer_send_to_resource(PurpleConnection *gc, const gchar *who, 
	const gchar *file)
{
	purple_debug_info("jabber", "jabber_xfer_send_to_resource: %s\n", who);
	if (jabber_xfer_support_jingle_ft(gc, who)) {
		/* call jingle... */
		jingle_file_transfer_send(gc, who, file);
	} else {
		jabber_si_xfer_send(gc, who, file);
	}
}

static void
jabber_xfer_resource_select_cancel_cb(JabberXferData *data,
	PurpleRequestFields *fields)
{
	g_free(data);
}

static void 
jabber_xfer_resource_select_ok_cb(JabberXferData *data, 
	PurpleRequestFields *fields)
{
	PurpleConnection *gc = data->gc;
	const gchar *who = data->who;
	const gchar *file = data->file;
	PurpleRequestField *field = purple_request_fields_get_field(fields, "resource");
	int selected_id = purple_request_field_choice_get_value(field);
	GList *labels = purple_request_field_choice_get_labels(field);
	const char *selected_label = g_list_nth_data(labels, selected_id);
	gchar *jid = g_strdup_printf("%s/%s", who, selected_label);

	g_free(data);
	jabber_xfer_send_to_resource(gc, jid, file);
	g_free(jid);
}

void 
jabber_xfer_send(PurpleConnection *gc, const char *who, const char *file)
{
	JabberStream *js = (JabberStream *) purple_connection_get_protocol_data(gc);
	JabberBuddy *jb = jabber_buddy_find(js, who, TRUE);
	JabberBuddyResource *jbr = NULL;
	gchar *resource = jabber_get_resource(who);

	/* Let the user select a resource, unless "who" already contains one */
	/* or there's only one resource online */
	if (resource) {
		/* who already contains a resource */
		jabber_xfer_send_to_resource(gc, who, file);
		g_free(resource);
	} else if(!jb || !jb->resources) {
		/* no resources online, we're trying to send to someone
		* whose presence we're not subscribed to, or
		* someone who is offline.  Let's inform the user */
		char *msg;

		if(!jb) {
			msg = g_strdup_printf(_("Unable to send file to %s, invalid JID"), 
				who);
		} else if(jb->subscription & JABBER_SUB_TO) {
			msg = g_strdup_printf(_("Unable to send file to %s, user is not online"), 
				who);
		} else {
			msg = g_strdup_printf(_("Unable to send file to %s, not subscribed to user presence"), 
				who);
		}

		purple_notify_error(js->gc, _("File Send Failed"), _("File Send Failed"), msg);
		g_free(msg);
	} else if(!jb->resources->next) {
		/* only 1 resource online (probably our most common case)
		* so no need to ask who to send to */
		gchar *jid = NULL;

		jbr = jb->resources->data;
		jid = g_strdup_printf("%s/%s", who, jbr->name);
		jabber_xfer_send_to_resource(gc, jid, file);
		g_free(jid);
	} else {
		/* we've got multiple resources, we need to pick one to send to */
		GList *l;
		char *msg = g_strdup_printf(_("Please select the resource of %s to which you would like to send a file"), 
			who);
		PurpleRequestFields *fields = purple_request_fields_new();
		PurpleRequestField *field = 
			purple_request_field_choice_new("resource", _("Resource"), 0);
		PurpleRequestFieldGroup *group = purple_request_field_group_new(NULL);
		JabberXferData *data = g_new0(JabberXferData, 1);

		data->gc = gc;
		data->who = who;
		data->file = g_strdup(file);

		for(l = jb->resources; l; l = l->next) {
			jbr = l->data;

			purple_request_field_choice_add(field, jbr->name);
		}

		purple_request_field_group_add_field(group, field);

		purple_request_fields_add_group(fields, group);

		purple_request_fields(js->gc, _("Select a Resource"), msg, NULL, 
			fields, _("Send File"), G_CALLBACK(jabber_xfer_resource_select_ok_cb), 
			_("Cancel"), G_CALLBACK(jabber_xfer_resource_select_cancel_cb),
			js->gc->account, who, NULL, data);

		g_free(msg);
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
jabber_xfer_create_from_xml(PurpleAccount *account, xmlnode *file,
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
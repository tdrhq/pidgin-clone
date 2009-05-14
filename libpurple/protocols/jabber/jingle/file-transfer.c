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
 * Foundation, Inc., 51 Franklin Street, Fith Floor Boston, MA 02110-1301,  USA
 */

#include "internal.h"

#include "jingle.h"
#include "file-transfer.h"
#include "ibbs.h"
#include "s5b.h"
#include "debug.h"
#include "xmlnode.h"
#include "xfer.h"

#include <string.h>

struct _JingleFTPrivate {
	PurpleXfer *xfer;
	FILE *ibb_fp; /* used to read/write from/to IBB streams */
	gboolean remote_failed_s5b;
};

#define JINGLE_FT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), \
									JINGLE_TYPE_FT, JingleFTPrivate))

#define JINGLE_FT_IBB_THRESHOLD 65536

static void jingle_file_transfer_class_init (JingleFTClass *klass);
static void jingle_file_transfer_init (JingleFT *rtp);
static void jingle_file_transfer_finalize (GObject *object);
static void jingle_file_transfer_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void jingle_file_transfer_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);

static JingleContent *jingle_file_transfer_parse_internal(xmlnode *ft);
static xmlnode *jingle_file_transfer_to_xml_internal(JingleContent *rtp, 
	xmlnode *content, JingleActionType action);
static void jingle_file_transfer_handle_action_internal(JingleContent *content, xmlnode *jingle, JingleActionType action);

static JingleContentClass *parent_class = NULL;

enum {
	PROP_0,
	PROP_XFER,
};

GType
jingle_file_transfer_get_type()
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof(JingleFTClass),
			NULL,
			NULL,
			(GClassInitFunc) jingle_file_transfer_class_init,
			NULL,
			NULL,
			sizeof(JingleFT),
			0,
			(GInstanceInitFunc) jingle_file_transfer_init,
			NULL
		};
		type = g_type_register_static(JINGLE_TYPE_CONTENT, "JingleFT", &info, 0);
	}
	return type;
}

static void
jingle_file_transfer_class_init (JingleFTClass *klass)
{
	GObjectClass *gobject_class = (GObjectClass*)klass;
	parent_class = g_type_class_peek_parent(klass);

	gobject_class->finalize = jingle_file_transfer_finalize;
	gobject_class->set_property = jingle_file_transfer_set_property;
	gobject_class->get_property = jingle_file_transfer_get_property;
	klass->parent_class.to_xml = jingle_file_transfer_to_xml_internal;
	klass->parent_class.parse = jingle_file_transfer_parse_internal;
	klass->parent_class.description_type = JINGLE_APP_FT;
	klass->parent_class.handle_action = 
		jingle_file_transfer_handle_action_internal;

	/* Add properties here... */
	g_object_class_install_property(gobject_class, PROP_XFER,
			g_param_spec_pointer("xfer",
			"PurpleXfer",
			"The PurpleXfer corresponding to this content.",
			G_PARAM_READABLE));
	
	g_type_class_add_private(klass, sizeof(JingleFTPrivate));
}

static void
jingle_file_transfer_init (JingleFT *ft)
{
	ft->priv = JINGLE_FT_GET_PRIVATE(ft);
	memset(ft->priv, 0, sizeof(ft->priv));
}

static void
jingle_file_transfer_finalize (GObject *ft)
{
	JingleFTPrivate *priv = JINGLE_FT_GET_PRIVATE(ft);
	purple_debug_info("jingle-ft","jingle_file_transfer_finalize\n");

	if (priv->ibb_fp) {
		fclose(priv->ibb_fp);
	}
	
	if (priv->xfer) {
		/* remove reference in transfer */
		priv->xfer->data = NULL;
	}
	
	G_OBJECT_CLASS(parent_class)->finalize(ft);
}

static void
jingle_file_transfer_set_property (GObject *object, guint prop_id, 
	const GValue *value, GParamSpec *pspec)
{
	JingleFT *ft;
	g_return_if_fail(JINGLE_IS_FT(object));

	ft = JINGLE_FT(object);

	switch (prop_id) {
		/* properties come here... */
		case PROP_XFER:
			ft->priv->xfer = g_value_get_pointer(value);
			break;
		default:	
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
jingle_file_transfer_get_property (GObject *object, guint prop_id, 
	GValue *value, GParamSpec *pspec)
{
	JingleFT *ft;
	g_return_if_fail(JINGLE_IS_FT(object));
	
	ft = JINGLE_FT(object);

	switch (prop_id) {
		/* properties comes here... */
		case PROP_XFER:
			g_value_set_pointer(value, ft->priv->xfer);
			break;
		default:	
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);	
			break;
	}
}

PurpleXfer *
jingle_file_transfer_get_xfer(JingleContent *content)
{
	PurpleXfer *xfer;
	g_object_get(content, "xfer", &xfer, NULL);
	return xfer;
}

static void
jingle_file_transfer_cancel_remote(JingleContent *content)
{
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
	JingleSession *session = jingle_content_get_session(content);
	
	purple_debug_info("jingle-ft", "cancel remote transfer\n");
	if (xfer) {
		/* TODO: set a reason code */
		jabber_iq_send(jingle_session_to_packet(session, 
			JINGLE_SESSION_TERMINATE));
		purple_xfer_cancel_remote(xfer);
	}

	g_object_unref(session);
	g_object_unref(session);	/* actually close the session */
}

static void
jingle_file_transfer_cancel_local(JingleContent *content)
{
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
	JingleSession *session = jingle_content_get_session(content);
	
	purple_debug_info("jingle-ft", "cancel local trasfer\n");
	if (xfer) {
		/* TODO: set a reason code */
		jabber_iq_send(jingle_session_to_packet(session, 
			JINGLE_SESSION_TERMINATE));
		purple_xfer_cancel_local(xfer);
	}

	g_object_unref(session);
	g_object_unref(session);	/* actually close the the session */
}

static void
jingle_file_transfer_success(JingleContent *content)
{
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
	JingleSession *session = jingle_content_get_session(content);
	
	purple_debug_info("jingle-ft", "transfer received successful!\n");
	if (xfer) {
		purple_xfer_set_completed(xfer, TRUE);
		purple_xfer_end(xfer);
	}

	g_object_unref(session);
}

static void
jingle_file_transfer_ibb_end(JingleContent *content)
{
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
	JingleSession *session = jingle_content_get_session(content);
	
	purple_debug_info("jingle-ft", "ending transfer\n");
	if (xfer) {
		if (jingle_session_is_initiator(session)) {
			jabber_iq_send(jingle_session_terminate_packet(session, "success"));
		}
		purple_xfer_set_completed(xfer, TRUE);
		purple_xfer_end(xfer);
	}

	g_object_unref(session);
	g_object_unref(session); /* actually delete it */
}

static void
jingle_file_transfer_ibb_send_data(JingleContent *content)
{
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
	FILE *fp = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->ibb_fp;
	gsize remaining = purple_xfer_get_bytes_remaining(xfer);
	JingleTransport *transport = jingle_content_get_transport(content);
	gsize block_size = jingle_ibb_get_block_size(JINGLE_IBB(transport));
	gsize packet_size = remaining < block_size ? remaining : block_size;
	gpointer data = g_malloc(packet_size);
	int res;

	purple_debug_info("jingle-ft", 
		"IBB: about to read %" G_GSIZE_FORMAT " bytes from file %p\n",
		packet_size, fp);
	res = fread(data, packet_size, 1, fp);

	if (res == 1) {
		jingle_ibb_send_data(JINGLE_IBB(transport), data, packet_size);
		purple_xfer_set_bytes_sent(xfer,
			purple_xfer_get_bytes_sent(xfer) + packet_size);
		purple_xfer_update_progress(xfer);
	} else {
		jingle_file_transfer_cancel_local(content);
	}
	g_free(data);
	g_object_unref(transport);
}

/* callback functions for IBB */
static void
jingle_file_transfer_ibb_data_sent_callback(JingleContent *content)
{
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
	gsize remaining = purple_xfer_get_bytes_remaining(xfer);

	if (remaining == 0) {
		/* close the session */
		jingle_file_transfer_ibb_end(content);
	} else {
		/* send more... */
		jingle_file_transfer_ibb_send_data(content);
	}
}

static void
jingle_file_transfer_ibb_data_recv_callback(JingleContent *content, 
	gconstpointer data, gsize size)
{
	JingleFT *ft = JINGLE_FT(content);
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(ft)->xfer;
	FILE *fp = JINGLE_FT_GET_PRIVATE(ft)->ibb_fp;

	if (size <= purple_xfer_get_bytes_remaining(xfer)) {
		purple_debug_info("jingle-ft", 
			"about to write %" G_GSIZE_FORMAT " bytes from IBB stream\n",
			size);
		if(!fwrite(data, size, 1, fp)) {
			purple_debug_error("jingle-ft", "error writing to file\n");
			purple_xfer_cancel_remote(xfer);
			return;
		}
		purple_xfer_set_bytes_sent(xfer, 
			purple_xfer_get_bytes_sent(xfer) + size);
		purple_xfer_update_progress(xfer);

		if (purple_xfer_get_bytes_remaining(xfer) == 0) {
			jingle_file_transfer_success(content);
		}
	} else {
		/* sending more than intended */
		purple_debug_error("jingle-ft",
			"IBB file transfer send more data than expected\n");
		jingle_file_transfer_cancel_remote(content);
	}
}

static void
jingle_file_transfer_ibb_error_callback(JingleContent *content)
{
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
	JingleSession *session = jingle_content_get_session(content);
	JabberStream *js = 
		jingle_session_get_js(session);
	PurpleConnection *gc = js->gc;
	PurpleAccount *account = purple_connection_get_account(gc);
	gchar *who = jingle_session_get_remote_jid(session);
	
	purple_debug_error("jingle-ft", 
		"an error occured during IBB file transfer\n");
	purple_xfer_error(purple_xfer_get_type(xfer), account, who,
		_("An error occured on the in-band bytestream transfer\n"));
	purple_xfer_cancel_remote(xfer);
	g_free(who);
	g_object_unref(session);
}

static void
jingle_file_transfer_add_ibb_session_to_transport(JabberStream *js,
	JingleTransport *transport, JingleContent *content, const gchar *jid)
{
	gchar *sid = jabber_get_next_id(js);

	if (JINGLE_IS_IBB(transport)) {
		jingle_ibb_create_session(JINGLE_IBB(transport), content, sid, jid);
		jingle_ibb_set_data_sent_callback(JINGLE_IBB(transport),
			jingle_file_transfer_ibb_data_sent_callback);
		jingle_ibb_set_error_callback(JINGLE_IBB(transport),
			jingle_file_transfer_ibb_error_callback);
	} else {
		purple_debug_error("jingle-ft",
			"trying to setup an IBB session of a non-IBB transport\n");
	}

	g_free(sid);
}

/* callback functions for S5B */
static void
jingle_file_transfer_s5b_connect_callback(JingleContent *content)
{
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
	JingleTransport *transport = jingle_content_get_transport(content);
	JingleS5B *s5b = JINGLE_S5B(transport);
	
	purple_debug_info("jingle-ft", 
		"in jingle_file_transfer_s5b_connect_callback\n");
	purple_debug_info("jingle-ft", "xfer->data: %p\n", xfer->data);
	purple_xfer_start(xfer, jingle_s5b_get_fd(s5b), NULL, 0);
}

static void
jingle_file_transfer_s5b_error_callback(JingleContent *content)
{
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
	JingleSession *session = jingle_content_get_session(content);
	JabberStream *js = 
		jingle_session_get_js(session);
	PurpleConnection *gc = js->gc;
	PurpleAccount *account = purple_connection_get_account(gc);
	gchar *who = jingle_session_get_remote_jid(session);
	
	purple_debug_error("jingle-ft", 
		"an error occured during SOCKS5 file transfer\n");
	purple_xfer_error(purple_xfer_get_type(xfer), account, who,
		_("An error occured on the SOCKS5 transfer\n"));
	purple_xfer_cancel_remote(xfer);
	g_free(who);
	g_object_unref(session);
}


static void
jingle_file_transfer_s5b_connect_failed_callback(JingleContent *content)
{
	/* the S5B transport has failed to connect */
	PurpleXfer *xfer = JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
	JingleSession *session = jingle_content_get_session(content);

	if (JINGLE_FT(content)->priv->remote_failed_s5b &&
		jingle_session_is_initiator(session)) {
		/* if the other end supports IBB, offer a transport replace */
		gchar *who = jingle_session_get_remote_jid(session);
		JabberStream *js = jingle_session_get_js(session);
		JabberBuddy *jb = jabber_buddy_find(js, who, FALSE);
		const gchar *resource = jabber_get_resource(who);
		JabberBuddyResource *jbr = jabber_buddy_find_resource(jb, resource);

		if (jabber_resource_has_capability(jbr, JINGLE_TRANSPORT_IBB)) {
			/* the buddy supports IBB, offer a transport replace */
			JabberIq *transport_replace = NULL;
			JingleTransport *new_transport = 
				jingle_transport_create(JINGLE_TRANSPORT_IBB);
			/* setup IBB stuff for the transport */
			jingle_file_transfer_add_ibb_session_to_transport(
				js, new_transport, content, who);
			/* let the transport be pending until we get a transport-accept */
			jingle_content_set_pending_transport(content, new_transport);

			transport_replace = 
				jingle_session_to_packet(session, JINGLE_TRANSPORT_REPLACE);
			jabber_iq_send(transport_replace);
		} else {
			/* the buddy doesn't support IBB, terminate it */
			JabberStream *js = jingle_session_get_js(session);
			PurpleConnection *gc = js->gc;
			PurpleAccount *account = purple_connection_get_account(gc);
			JabberIq *session_terminate =
				jingle_session_terminate_packet(session, "connectivity-error");

			purple_debug_error("jingle-ft", 
				"could not establish s5b connection, and IBB is not "
				"available\n");
			purple_xfer_error(purple_xfer_get_type(xfer), account, who,
				_("Failed to establish a SOCKS5 connection\n"));
			purple_xfer_cancel_remote(xfer);
			
			jabber_iq_send(session_terminate);
		}
		g_free(who);
	}
	g_object_unref(session);
}




static void
jingle_file_transfer_xfer_init(PurpleXfer *xfer)
{
	if (purple_xfer_get_type(xfer) == PURPLE_XFER_SEND) {
		PurpleAccount *account = purple_xfer_get_account(xfer);
		PurpleConnection *gc = purple_account_get_connection(account);
		JabberStream *js = 
			(JabberStream *) purple_connection_get_protocol_data(gc);
				JingleSession *session;
		const gchar *who = purple_xfer_get_remote_user(xfer);
		JingleContent *content;
		JingleTransport *transport;
		gchar *jid = NULL, *me = NULL, *sid = NULL;
		const gchar *transport_type = NULL;
			
		/* construct JID to send to */
		JabberBuddy *jb = jabber_buddy_find(js, who, FALSE);
		JabberBuddyResource *jbr;
		
		if (!jb) {
			purple_debug_error("jingle-rtp", "Could not find Jabber buddy\n");
			return;
		}
		jbr = jabber_buddy_find_resource(jb, NULL);
		if (!jbr) {
			purple_debug_error("jingle-rtp", "Could not find buddy's resource\n");
		}
		
		if (purple_xfer_get_size(xfer) <= JINGLE_FT_IBB_THRESHOLD &&
			jabber_resource_has_capability(jbr, JINGLE_TRANSPORT_IBB)) {
			purple_debug_info("jingle-ft",
				"the file is less than 64 kB and the receiver supports IBB, "
				"so let's go for that from the start\n");
			transport_type = JINGLE_TRANSPORT_IBB;
		} else if (jabber_resource_has_capability(jbr, JINGLE_TRANSPORT_S5B)) {
			purple_debug_info("jingle-ft", 
				"receiver supports S5B, let's try that first\n");
			transport_type = JINGLE_TRANSPORT_S5B;
		} else if (jabber_resource_has_capability(jbr, JINGLE_TRANSPORT_IBB)) {
			purple_debug_info("jingle-ft",
				"receiver didn't support S5B but IBB, so let's try that\n");
			transport_type = JINGLE_TRANSPORT_IBB;
		} else {
			purple_debug_error("jingle-ft",
				"receiver doesn't support S5B or IBB, bailing out "
				"(this shouldn't happen).");
			return;
		}
		
		if ((strchr(who, '/') == NULL) && jbr && (jbr->name != NULL)) {
			jid = g_strdup_printf("%s/%s", who, jbr->name);
		} else {
			jid = g_strdup(who);
		}

		/* set ourselves as initiator */
		me = g_strdup_printf("%s@%s/%s", js->user->node, js->user->domain, js->user->resource);

		sid = jabber_get_next_id(js);
		session = jingle_session_create(js, sid, me, jid, TRUE);
		g_free(sid);
		g_free(me);
		
		/* add the content */
		transport = jingle_transport_create(transport_type);
		content = jingle_content_create(JINGLE_APP_FT, "initiator", "session",
			"ft-session", "sender", transport);
		jingle_session_add_content(session, content);
		JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer = xfer;

		if (JINGLE_IS_IBB(transport)) {
			jingle_file_transfer_add_ibb_session_to_transport(js, transport, 
				content, jid);
		}	
		g_free(jid);
		purple_debug_info("jingle-ft", "setting content %p on xfer %p\n", 
			content, xfer);
		xfer->data = content;
		
		if (JINGLE_IS_IBB(transport)) {
			/* if it's IBB, send session-intitate directly */
			jabber_iq_send(jingle_session_to_packet(session, 
				JINGLE_SESSION_INITIATE));
		} else if (JINGLE_IS_S5B(transport)) {
			/* generate a new sid to set for the transport */
			sid = jabber_get_next_id(js);
			jingle_s5b_set_sid(JINGLE_S5B(transport), sid);
			g_free(sid);
			jingle_s5b_set_connect_callback(JINGLE_S5B(transport),
				jingle_file_transfer_s5b_connect_callback, content);
			jingle_s5b_set_error_callback(JINGLE_S5B(transport),
				jingle_file_transfer_s5b_error_callback, content);
			jingle_s5b_set_failed_connect_callback(JINGLE_S5B(transport),
				jingle_file_transfer_s5b_connect_failed_callback, content);
			/* start local listen on the S5B transport */
			jingle_s5b_gather_streamhosts(session, JINGLE_S5B(transport));
		}	
	} else if (xfer->data) {
		JingleContent *content = (JingleContent *) xfer->data;
		JingleSession *session = jingle_content_get_session(content);
		JingleTransport *transport = jingle_content_get_transport(content);

		/* ready to receive */
		if (JINGLE_IS_IBB(transport)) {
			/* open file and prepare for IBB */
			/* open file to write to */
			JingleIBB *ibb = JINGLE_IBB(transport);
			const gchar *filename = purple_xfer_get_local_filename(xfer);

			/* send a session-accept immediatly, since it's IBB */
			jabber_iq_send(jingle_session_to_packet(session,
				JINGLE_SESSION_ACCEPT));

			JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->ibb_fp = 
				g_fopen(filename, "wb");
			if (JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->ibb_fp == NULL) {
				purple_debug_error("jabber", 
					"failed to open file %s for writing: %s\n", filename, 
					g_strerror(errno));
				purple_xfer_cancel_remote(xfer);
				jabber_iq_send(jingle_session_to_packet(session,
						JINGLE_SESSION_TERMINATE));
				g_object_unref(transport);
				g_object_unref(session);
				g_object_unref(session);
				return;
			}

			/* setup callbacks */
			jingle_ibb_set_data_received_callback(ibb, 
				jingle_file_transfer_ibb_data_recv_callback);
			jingle_ibb_set_error_callback(ibb,
				jingle_file_transfer_ibb_error_callback);
	
			/* start the transfer */
			purple_xfer_start(xfer, 0, NULL, 0);
		} else if (JINGLE_IS_S5B(transport)) {
			jingle_s5b_set_connect_callback(JINGLE_S5B(transport),
				jingle_file_transfer_s5b_connect_callback, content);
			jingle_s5b_set_error_callback(JINGLE_S5B(transport),
				jingle_file_transfer_s5b_error_callback, content);
			jingle_s5b_set_failed_connect_callback(JINGLE_S5B(transport),
				jingle_file_transfer_s5b_connect_failed_callback, content);
			jingle_s5b_gather_streamhosts(session, JINGLE_S5B(transport));
		}
		g_object_unref(session);
		g_object_unref(transport);
	}
}

static void
jingle_file_transfer_cancel_send(PurpleXfer *xfer)
{
	JingleSession *session = 
		jingle_content_get_session((JingleContent *)xfer->data);
	
	purple_debug_info("jingle-ft", "jingle_file_transfer_cancel_send\n");
	jabber_iq_send(jingle_session_to_packet(session, JINGLE_SESSION_TERMINATE));
	g_object_unref(session);
	g_object_unref(session);
}

static void
jingle_file_transfer_cancel_recv(PurpleXfer *xfer)
{
	JingleSession *session = 
		jingle_content_get_session((JingleContent *)xfer->data);

	purple_debug_info("jingle-ft", "jingle_file_transfer_cancel_recv\n");
	jabber_iq_send(jingle_session_to_packet(session, JINGLE_SESSION_TERMINATE));
	g_object_unref(session);
	g_object_unref(session);
}

static void
jingle_file_transfer_xfer_end(PurpleXfer *xfer)
{
	purple_debug_info("jingle-ft", 
		"jingle_file_transfer_xfer_end xfer->data: %p\n", xfer->data); 
	if (xfer->data) {
		JingleSession *session = 
			jingle_content_get_session((JingleContent *)xfer->data);
		/* the receiver will send a session-terminate when the whole file has
		 been received */
		if (!jingle_session_is_initiator(session)) {
			jabber_iq_send(jingle_session_terminate_packet(session, "success"));
		}
		/* the PurpleXfer will get unref:ed after this has finished, so we
		 disconnect it to avoid a race-condition if we get here before the
		 getting a session-terminate when we are the initiator */
		JINGLE_FT_GET_PRIVATE(JINGLE_FT(xfer->data))->xfer = NULL;
		g_object_unref(session);
		g_object_unref(session);
	}
}

static JingleContent *
jingle_file_transfer_parse_internal(xmlnode *ft)
{
	JingleContent *content = parent_class->parse(ft);
	xmlnode *description = xmlnode_get_child(ft, "description");
	purple_debug_info("jingle-ft", "ft parse\n");
	return content;
}

static xmlnode *
jingle_file_transfer_to_xml_internal(JingleContent *ft, xmlnode *content, 
	JingleActionType action)
{
	xmlnode *node = parent_class->to_xml(ft, content, action);
	xmlnode *description = xmlnode_get_child(node, "description");
	if (description != NULL) {
		xmlnode *offer = xmlnode_new_child(description, "offer");
		const PurpleXfer *xfer = jingle_file_transfer_get_xfer(ft);
		
		/* if xfer is NULL, it means it has disconnected */
		if (xfer)
			xmlnode_insert_child(offer, jabber_xfer_create_file_element(xfer));
	}
	return node;
}

static void
jingle_file_transfer_handle_action_internal(JingleContent *content, 
	xmlnode *xmlcontent, JingleActionType action)
{
	switch (action) {
		case JINGLE_SESSION_ACCEPT: {
			JingleSession *session = jingle_content_get_session(content);
			xmlnode *description = xmlnode_get_child(xmlcontent, "description");
			JingleTransport *transport = jingle_content_get_transport(content);
			PurpleXfer *xfer = 
				JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer;
			
			/* do stuff here, start the transfer, etc... */
			if (JINGLE_IS_IBB(transport)) {
				JingleFT *ft = JINGLE_FT(content);
				/* open the file for reading */
				JINGLE_FT_GET_PRIVATE(ft)->ibb_fp = 
					g_fopen(purple_xfer_get_local_filename(xfer), "rb");
				
				if (JINGLE_FT_GET_PRIVATE(ft)->ibb_fp) {
					/* send first data */
					purple_xfer_start(xfer, 0, NULL, 0);
					purple_xfer_set_bytes_sent(xfer, 0);
					purple_xfer_update_progress(xfer);
					jingle_file_transfer_ibb_send_data(content);
				} else {
					purple_debug_error("jingle-ft", 
						"failed to open file for reading\n");
					jingle_file_transfer_cancel_local(content);
					break;
				}
			} else if (JINGLE_IS_S5B(transport)) {
				/* add the receiver's streamhost (this must be done here since
					parse is not called on the existing transport */
				jingle_s5b_add_streamhosts(JINGLE_S5B(transport),
					xmlnode_get_child(xmlcontent, "transport"));
				/* attempt to connect bytestream */
				jingle_s5b_attempt_connect(session, JINGLE_S5B(transport));
			}
			
			g_object_unref(session);
			g_object_unref(transport);
			break;
		}
		case JINGLE_SESSION_INITIATE: {
			JingleSession *session = jingle_content_get_session(content);
			JingleTransport *transport = jingle_content_get_transport(content);
			xmlnode *description = xmlnode_get_child(xmlcontent, "description");
			JabberStream *js = jingle_session_get_js(session);
			xmlnode *offer = xmlnode_get_child(description, "offer");
			PurpleXfer *xfer = NULL;
			gchar *who = jingle_session_get_remote_jid(session);
	
			if (offer) {
				/* do stuff here... create the PurpleXfer etc... */
				xmlnode *file = xmlnode_get_child(offer, "file");
				PurpleXfer *xfer = NULL;

				if (file)
					xfer = jabber_xfer_create_from_xml(js->gc->account, file, 
						who, content);
				if (xfer) {
					JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer = xfer;
					/* setup callbacks */
					purple_xfer_set_init_fnc(xfer, 
						jingle_file_transfer_xfer_init);
					purple_xfer_set_end_fnc(xfer,
						jingle_file_transfer_xfer_end);
					purple_xfer_set_cancel_recv_fnc(xfer,
						jingle_file_transfer_cancel_recv);
					purple_xfer_request(xfer);
				} else {
					/* TODO: set a reason code */
					jabber_iq_send(jingle_session_to_packet(session,
						JINGLE_SESSION_TERMINATE));
					g_object_unref(session);
					g_object_unref(session);
					g_free(who);
					break;
				}
				
				if (JINGLE_IS_IBB(transport)) {
					JingleIBB *ibb = JINGLE_IBB(transport);
					const gchar *sid = 
						xmlnode_get_attrib(xmlnode_get_child(xmlcontent, 
							"transport"), "sid");
					const gchar *filename = 
						purple_xfer_get_local_filename(xfer);
					jingle_ibb_create_session(ibb, content, sid, who);
				} else if (JINGLE_IS_S5B(transport)) {
					/* set S5B callbacks */
					jingle_s5b_set_connect_callback(JINGLE_S5B(transport),
						jingle_file_transfer_s5b_connect_callback, content);
					jingle_s5b_set_error_callback(JINGLE_S5B(transport),
						jingle_file_transfer_s5b_error_callback, content);
				}
				
				g_object_unref(transport);
			}

			g_object_unref(session);
			g_free(who);
			break;
		}
		case JINGLE_SESSION_TERMINATE: {
			JingleSession *session = jingle_content_get_session(content);
			PurpleXfer *xfer = 
				jingle_file_transfer_get_xfer(content);

			/* do stuff here... close transfer etc... */
			if (xfer) {
				purple_debug_info("jingle", 
					"got session-terminate, ending transfer %p with content %p\n", 
					xfer, xfer->data);
				if (!purple_xfer_is_canceled(xfer))
					purple_xfer_end(xfer);
				JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer = NULL;
				xfer->data = NULL;
			}
	
			g_object_unref(session);
			break;
		}
		case JINGLE_TRANSPORT_INFO: {
			JingleSession *session = jingle_content_get_session(content);
			JingleTransport *transport = jingle_content_get_transport(content);
			xmlnode *xmltransport = xmlnode_get_child(xmlcontent, "transport");
			
			/* we should check for "stream-host" error (in the case of S5B) and
			 offer a transport-replace with IBB */
			if (xmltransport) {
				xmlnode *streamhost_error = 
					xmlnode_get_child(xmltransport, "streamhost-error");

				if (streamhost_error) {
					purple_debug_info("jingle-ft", 
						"got a streamhost-error, remote couldn't connect\n");
					JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->
						remote_failed_s5b = TRUE;
				
					/* if we have already tried connecting to all remote
					 streamhost candidates, or there were none, trigger
					 the replace callback directly */
					if (JINGLE_IS_S5B(transport) &&
						!jingle_s5b_has_remaining_remote_streamhosts(
							JINGLE_S5B(transport))) {
						jingle_file_transfer_s5b_connect_failed_callback(
							content);
					}
				}
			}
			
			g_object_unref(session);
			break;
		}
		case JINGLE_TRANSPORT_ACCEPT: {
			JingleSession *session = jingle_content_get_session(content);
			JingleTransport *transport = jingle_content_get_transport(content);
			PurpleXfer *xfer = 
				jingle_file_transfer_get_xfer(content);

			purple_debug_info("jingle-ft",
				"content %p\n", content);
			purple_debug_info("jingle-ft", 
				"got transport-accept transport %p\n", transport);
			
			/* if we got a streamhost-error before, we should switch over to
			 IBB here */
			if (JINGLE_FT(content)->priv->remote_failed_s5b) {
				jingle_content_accept_transport(content);
				/* open the file, etc... */
				JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->ibb_fp = 
					g_fopen(purple_xfer_get_local_filename(xfer), "rb");
				
				if (JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->ibb_fp) {
					/* send first data */
					purple_xfer_start(xfer, 0, NULL, 0);
					purple_xfer_set_bytes_sent(xfer, 0);
					purple_xfer_update_progress(xfer);
					jingle_file_transfer_ibb_send_data(content);
				} else {
					purple_debug_error("jingle-ft", 
						"failed to open file for reading\n");
					jingle_file_transfer_cancel_local(content);
				}
			} else if (JINGLE_IS_S5B(transport)) {
				JingleS5B *s5b = JINGLE_S5B(transport);
				xmlnode *xmltransport = xmlnode_get_child(xmlcontent, "transport");
				jingle_s5b_handle_transport_accept(s5b, session, xmltransport);
			}
				
			break;
		}
		case JINGLE_TRANSPORT_REPLACE: {
			JingleSession *session = jingle_content_get_session(content);
			xmlnode *xmltransport = xmlnode_get_child(xmlcontent, "transport");
			const gchar *sid = 
				xmlnode_get_attrib(xmlnode_get_child(xmlcontent, 
					"transport"), "sid");
			gchar *who = jingle_session_get_remote_jid(session);
			const PurpleXfer *xfer = JINGLE_FT(content)->priv->xfer;
			const gchar *filename = purple_xfer_get_local_filename(xfer);
			JingleTransport *new_transport = 
				jingle_content_get_pending_transport(content);
			
			/* fallback to IBB etc... */
			if (new_transport && JINGLE_IS_IBB(new_transport)) {
				JingleIBB *ibb = JINGLE_IBB(new_transport);

				jingle_ibb_create_session(ibb, content, sid, who);
				/* immediatly accept the new transport */
				jingle_content_accept_transport(content);

				/* open the file and setup the callbacks */
				JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->ibb_fp = 
					g_fopen(filename, "wb");
				if (JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->ibb_fp == NULL) {
					purple_debug_error("jabber", 
						"failed to open file %s for writing: %s\n", filename, 
						g_strerror(errno));
					purple_xfer_cancel_remote(xfer);
					jabber_iq_send(jingle_session_to_packet(session,
						JINGLE_SESSION_TERMINATE));
					g_object_unref(new_transport);
					g_object_unref(session);
					g_object_unref(session);
					g_free(who);
					return;
				}

				/* setup callbacks */
				jingle_ibb_set_data_received_callback(ibb, 
					jingle_file_transfer_ibb_data_recv_callback);
				jingle_ibb_set_error_callback(ibb,
					jingle_file_transfer_ibb_error_callback);

				/* start the transfer */
				purple_xfer_start(xfer, 0, NULL, 0);

				/* send transport-accept */
				jabber_iq_send(jingle_session_to_packet(session,
                    JINGLE_TRANSPORT_ACCEPT));
			} else {
				/* we don't currently handle other fallback methods than IBB */
				jabber_iq_send(jingle_session_to_packet(session,
					JINGLE_TRANSPORT_REJECT));
			}

			g_object_unref(new_transport);
			g_free(who);
			g_object_unref(session);
			break;
		}
		case JINGLE_TRANSPORT_REJECT: {
			JingleSession *session = jingle_content_get_session(content);
			PurpleXfer *xfer = 
				jingle_file_transfer_get_xfer(content);

			/* do stuff here... close transfer etc... */
			if (xfer) {
				purple_debug_info("jingle", 
					"got transport-reject, ending transfer %p with content %p\n", 
					xfer, xfer->data);
				if (!purple_xfer_is_canceled(xfer))
					purple_xfer_cancel_local(xfer);
				JINGLE_FT_GET_PRIVATE(JINGLE_FT(content))->xfer = NULL;
				xfer->data = NULL;
			}

			jabber_iq_send(jingle_session_to_packet(session, 
				JINGLE_SESSION_TERMINATE));
			g_object_unref(session);
			break;
		}
		default:
			break;
	}
}

PurpleXfer *
jingle_file_transfer_new_xfer(PurpleConnection *gc, const gchar *who)
{
	JabberStream *js = gc->proto_data;
	PurpleXfer *xfer = purple_xfer_new(gc->account, PURPLE_XFER_SEND, who);

	purple_debug_info("jingle-ft", "jingle_file_transfer_new_xfer\n");

	if (xfer)
	{
		purple_xfer_set_init_fnc(xfer, jingle_file_transfer_xfer_init);
		purple_xfer_set_cancel_send_fnc(xfer, jingle_file_transfer_cancel_send);
		purple_xfer_set_end_fnc(xfer, jingle_file_transfer_xfer_end);
	}

	return xfer;
}


void 
jingle_file_transfer_send(PurpleConnection *gc, const char *who, const char *file)
{
	JabberStream *js = gc->proto_data;
	PurpleXfer *xfer = jingle_file_transfer_new_xfer(gc, who);

	purple_debug_info("jingle-ft", "jingle_file_transfer_send\n");
	
	if (file)
		purple_xfer_request_accepted(xfer, file);
	else
		purple_xfer_request(xfer);
}

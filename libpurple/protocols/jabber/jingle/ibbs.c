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
 
#include "internal.h"

#include "../ibb.h"
#include "jingle.h"
#include "session.h"
#include "content.h"
#include "ibbs.h"
#include "debug.h"
#include "xmlnode.h"

struct _JingleIBBPrivate {
	JabberIBBSession *session;
	JingleIBBSentCallback *sent_cb;
	JingleIBBDataCallback *recv_cb;
	JingleIBBErrorCallback *error_cb;
};

#define JINGLE_IBB_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), JINGLE_TYPE_IBB, JingleIBBPrivate))

static void jingle_ibb_class_init (JingleIBBClass *klass);
static void jingle_ibb_init (JingleIBB *ibb);
static void jingle_ibb_finalize (GObject *object);
static void jingle_ibb_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void jingle_ibb_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static JingleTransport *jingle_ibb_parse_internal(xmlnode *ibb);
static xmlnode *jingle_ibb_to_xml_internal(JingleTransport *transport, xmlnode *content, JingleActionType action);

static JingleTransportClass *parent_class = NULL;

enum {
	PROP_0,
	PROP_IBB_SESSION
};

GType
jingle_ibb_get_type()
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof(JingleIBBClass),
			NULL,
			NULL,
			(GClassInitFunc) jingle_ibb_class_init,
			NULL,
			NULL,
			sizeof(JingleIBB),
			0,
			(GInstanceInitFunc) jingle_ibb_init,
			NULL
		};
		type = g_type_register_static(JINGLE_TYPE_TRANSPORT, "JingleIBB", &info, 0);
	}
	return type;
}

static void
jingle_ibb_class_init (JingleIBBClass *klass)
{
	GObjectClass *gobject_class = (GObjectClass*)klass;
	parent_class = g_type_class_peek_parent(klass);

	gobject_class->finalize = jingle_ibb_finalize;
	gobject_class->set_property = jingle_ibb_set_property;
	gobject_class->get_property = jingle_ibb_get_property;
	klass->parent_class.to_xml = jingle_ibb_to_xml_internal;
	klass->parent_class.parse = jingle_ibb_parse_internal;
	klass->parent_class.transport_type = JINGLE_TRANSPORT_IBB;

	g_object_class_install_property(gobject_class, PROP_IBB_SESSION,
			g_param_spec_pointer("ibb-session",
			"IBB session",
			"The IBB session for this transport.",
			G_PARAM_READABLE));

	g_type_class_add_private(klass, sizeof(JingleIBBPrivate));
}

static void
jingle_ibb_init (JingleIBB *ibb)
{	
	ibb->priv = JINGLE_IBB_GET_PRIVATE(ibb);
	memset(ibb->priv, 0, sizeof(ibb->priv));
	
	/* create IBB session */
}

static void
jingle_ibb_finalize (GObject *ibb)
{
	JingleIBBPrivate *priv = JINGLE_IBB_GET_PRIVATE(ibb);
	purple_debug_info("jingle","jingle_ibb_finalize transport: %lx\n",
		ibb);
	
	if (priv->session) {
		/* should manually close the IBB session */
		jabber_ibb_session_destroy(priv->session);
		priv->session = NULL;
	}

	G_OBJECT_CLASS(parent_class)->finalize(ibb);
}

static void
jingle_ibb_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	JingleIBB *ibb;
	g_return_if_fail(JINGLE_IS_IBB(object));

	ibb = JINGLE_IBB(object);

	switch (prop_id) {
		case PROP_IBB_SESSION:
			ibb->priv->session = g_value_get_pointer(value);
			break;
		default:	
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
jingle_ibb_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	JingleIBB *ibb;
	g_return_if_fail(JINGLE_IS_IBB(object));
	
	ibb = JINGLE_IBB(object);

	switch (prop_id) {
		case PROP_IBB_SESSION:
			g_value_set_pointer(value, ibb->priv->session);
			break;
		default:	
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);	
			break;
	}
}

static JingleTransport *
jingle_ibb_parse_internal(xmlnode *ibb)
{
	JingleTransport *transport = parent_class->parse(ibb);
	JingleIBBPrivate *priv = JINGLE_IBB_GET_PRIVATE(transport);
	xmlnode *jingle = xmlnode_get_parent(xmlnode_get_parent(ibb));
	const gchar *who = xmlnode_get_attrib(jingle, "initiator");
	const gchar *sid = xmlnode_get_attrib(ibb, "sid");
	
	return transport;
}

static xmlnode *
jingle_ibb_to_xml_internal(JingleTransport *transport, xmlnode *content, 
	JingleActionType action)
{
	xmlnode *node = parent_class->to_xml(transport, content, action);

	purple_debug_info("jingle", "jingle_ibb_to_xml_internal\n");

	if (action == JINGLE_SESSION_INITIATE || action == JINGLE_TRANSPORT_INFO) {
		JingleIBBPrivate *priv = JINGLE_IBB_GET_PRIVATE(transport);
		if (priv->session) {
			gchar *block_size = 
				g_strdup_printf("%d", 
					jabber_ibb_session_get_block_size(priv->session));
			xmlnode_set_attrib(node, "sid", 
				jabber_ibb_session_get_sid(priv->session));
			xmlnode_set_attrib(node, "block-size", block_size);
			/* we will always send as <iq/> */
			xmlnode_set_attrib(node, "stanza", "iq");
			g_free(block_size);
		} else {
			purple_debug_error("jingle", 
				"trying to send IBB transport before creating IBB session\n");
		}
	}
	return node;
}

static void
jingle_ibb_data_sent_callback(JabberIBBSession *sess)
{
	JingleContent *content = 
		(JingleContent *) jabber_ibb_session_get_user_data(sess);

	if (content) {
		JingleTransport *transport = jingle_content_get_transport(content);

		JingleIBBSentCallback *sent_cb = 
			JINGLE_IBB_GET_PRIVATE(JINGLE_IBB(transport))->sent_cb;

		if (sent_cb) {
			sent_cb(content);
		}
		g_object_unref(transport);
	}
}

static void
jingle_ibb_data_recv_callback(JabberIBBSession *sess, const gpointer data,
	gsize size)
{
	JingleContent *content = 
		(JingleContent *) jabber_ibb_session_get_user_data(sess);
	
	if (content) {
		JingleTransport *transport = jingle_content_get_transport(content);

		JingleIBBDataCallback *recv_cb = 
			JINGLE_IBB_GET_PRIVATE(JINGLE_IBB(transport))->recv_cb;
			
		if (recv_cb) {
			recv_cb(content, data, size);
		}
		g_object_unref(transport);
	}
}

static void
jingle_ibb_error_callback(JabberIBBSession *sess)
{
	JingleContent *content = 
		(JingleContent *) jabber_ibb_session_get_user_data(sess);
	
	if (content) {
		JingleTransport *transport = jingle_content_get_transport(content);

		JingleIBBErrorCallback *error_cb = 
				JINGLE_IBB_GET_PRIVATE(JINGLE_IBB(transport))->error_cb;
			
		if (error_cb) {
			error_cb(content);
		}
		g_object_unref(transport);
	}
}

void
jingle_ibb_create_session(JingleIBB *ibb, JingleContent *content, 
	const gchar *sid, const gchar *who)
{
	JingleSession *sess = jingle_content_get_session(content);
	JabberIBBSession *session = 
		jabber_ibb_session_create(jingle_session_get_js(sess), sid, who,
			content);

	purple_debug_info("jingle-ibb", "creating IBB session with sid: %s,\n"
		" who: %s\n", sid, who);

	/* we will set the IBB session to "open" immediatly, since the Jingle
	 negotiation defines the "open" state */
	jabber_ibb_session_set_state(session, JABBER_IBB_SESSION_OPENED);
	/* set callbacks... */
	jabber_ibb_session_set_data_sent_callback(session, 
		jingle_ibb_data_sent_callback);
	jabber_ibb_session_set_data_received_callback(session,
		jingle_ibb_data_recv_callback);
	jabber_ibb_session_set_error_callback(session, jingle_ibb_error_callback);
	purple_debug_info("jingle-ibb", "setting session %lx on transport %lx\n",
		session, ibb);
	JINGLE_IBB_GET_PRIVATE(ibb)->session = session;

	g_object_unref(sess);
}

void 
jingle_ibb_set_data_sent_callback(JingleIBB *ibb, JingleIBBSentCallback *cb)
{
	JINGLE_IBB_GET_PRIVATE(ibb)->sent_cb = cb;
}

void 
jingle_ibb_set_data_received_callback(JingleIBB *ibb, JingleIBBDataCallback *cb)
{
	JINGLE_IBB_GET_PRIVATE(ibb)->recv_cb = cb;
}

void 
jingle_ibb_set_error_callback(JingleIBB *ibb, JingleIBBErrorCallback *cb)
{
	JINGLE_IBB_GET_PRIVATE(ibb)->error_cb = cb;
}

void
jingle_ibb_send_data(JingleIBB *ibb, gconstpointer data, gsize size)
{
	JabberIBBSession *session = JINGLE_IBB_GET_PRIVATE(ibb)->session;

	if (session) {
		jabber_ibb_session_send_data(session, data, size);
	} else {
		purple_debug_error("jingle", 
			"jingle_ibb_send_data: no IBB session created\n");
	}
}

gsize
jingle_ibb_get_block_size(const JingleIBB *ibb)
{
	return jabber_ibb_session_get_block_size(
		JINGLE_IBB_GET_PRIVATE(ibb)->session);
}
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

#include "jingle.h"
#include "session.h"
#include "content.h"
#include "s5b.h"
#include "debug.h"
#include "network.h"
#include "xmlnode.h"

/* auxillary functions to handle JabberBytestreamsStreamhosts, maybe this
 should be in a separtate module, used by si.c and other places as well */

static JabberBytestreamsStreamhost *
jingle_s5b_streamhost_create(const gchar *jid, const gchar *host, int port,
	const gchar *zeroconf)
{
	JabberBytestreamsStreamhost *sh = g_new0(JabberBytestreamsStreamhost, 1);
	
	if (sh) {
		sh->jid = g_strdup(jid);
		sh->host = g_strdup(host);
		sh->port = port;
		if (zeroconf)
			sh->zeroconf = g_strdup(zeroconf);
	}

	return sh;
}

static JabberBytestreamsStreamhost *
jingle_s5b_streamhost_create_from_xml(xmlnode *streamhost)
{
	JabberBytestreamsStreamhost *sh = NULL;
	const gchar *jid = xmlnode_get_attrib(streamhost, "jid");
	const gchar *host = xmlnode_get_attrib(streamhost, "host");
	const gchar *port = xmlnode_get_attrib(streamhost, "port");
	const gchar *zeroconf = xmlnode_get_attrib(streamhost, "zeroconf");

	if (jid && host) {
		sh = g_new0(JabberBytestreamsStreamhost, 1);
		sh->jid = g_strdup(jid);
		sh->host = g_strdup(host);
		if (port)
			sh->port = atoi(port);
		if (zeroconf)
			sh->zeroconf = g_strdup(zeroconf);
	}
	return sh;
}

static void
jingle_s5b_streamhost_destroy(JabberBytestreamsStreamhost *sh)
{
	g_free(sh->jid);
	g_free(sh->host);
	if (sh->zeroconf)
		g_free(sh->zeroconf);
	g_free(sh);
}

static JabberBytestreamsStreamhost *
jingle_s5b_streamhost_copy(const JabberBytestreamsStreamhost *sh)
{
	JabberBytestreamsStreamhost *new_sh = g_new0(JabberBytestreamsStreamhost, 1);
	
	if (new_sh) {
		new_sh->jid = g_strdup(sh->jid);
		new_sh->host = g_strdup(sh->host);
		new_sh->port = sh->port;
		if (sh->zeroconf)
			new_sh->zeroconf = g_strdup(sh->zeroconf);
	}

	return new_sh;
}

static xmlnode *
jingle_s5b_streamhost_to_xml(const JabberBytestreamsStreamhost *sh)
{
	xmlnode *streamhost = xmlnode_new("streamhost");
	gchar port[10];
	
	if (streamhost) {
		g_snprintf(port, 10, "%d", sh->port);
		xmlnode_set_attrib(streamhost, "jid", sh->jid);
		xmlnode_set_attrib(streamhost, "host", sh->host);
		xmlnode_set_attrib(streamhost, "port", port);
		if (sh->zeroconf)
			xmlnode_set_attrib(streamhost, "zeroconf", sh->zeroconf);
	}
	
	return streamhost;
}


struct _JingleS5BPrivate {
	/* S5B stuff here... */
	gchar *sid;
	guint fd;
	guint local_fd;
	guint remote_fd;
	PurpleProxyConnectData *connect_data;
	PurpleNetworkListenData *listen_data;
	GList *remote_streamhosts;
	GList *local_streamhosts;
};

#define JINGLE_S5B_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), JINGLE_TYPE_S5B, JingleS5BPrivate))

static void jingle_s5b_class_init (JingleS5BClass *klass);
static void jingle_s5b_init (JingleS5B *s5b);
static void jingle_s5b_finalize (GObject *object);
static void jingle_s5b_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void jingle_s5b_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static JingleTransport *jingle_s5b_parse_internal(xmlnode *s5b);
static xmlnode *jingle_s5b_to_xml_internal(JingleTransport *transport, xmlnode *content, JingleActionType action);

static JingleTransportClass *parent_class = NULL;

enum {
	PROP_0,
	PROP_SID
};

GType
jingle_s5b_get_type()
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof(JingleS5BClass),
			NULL,
			NULL,
			(GClassInitFunc) jingle_s5b_class_init,
			NULL,
			NULL,
			sizeof(JingleS5B),
			0,
			(GInstanceInitFunc) jingle_s5b_init,
			NULL
		};
		type = g_type_register_static(JINGLE_TYPE_TRANSPORT, "JingleS5B", &info, 0);
	}
	return type;
}

static void
jingle_s5b_class_init (JingleS5BClass *klass)
{
	GObjectClass *gobject_class = (GObjectClass*)klass;
	parent_class = g_type_class_peek_parent(klass);

	gobject_class->finalize = jingle_s5b_finalize;
	gobject_class->set_property = jingle_s5b_set_property;
	gobject_class->get_property = jingle_s5b_get_property;
	klass->parent_class.to_xml = jingle_s5b_to_xml_internal;
	klass->parent_class.parse = jingle_s5b_parse_internal;
	klass->parent_class.transport_type = JINGLE_TRANSPORT_S5B;

	g_object_class_install_property(gobject_class, PROP_SID,
		g_param_spec_string("sid",
		"Session ID",
		"The unique session ID of the S5B transport.",
		NULL,
		G_PARAM_READWRITE));
	
	g_type_class_add_private(klass, sizeof(JingleS5BPrivate));
}

static void
jingle_s5b_init (JingleS5B *s5b)
{	
	s5b->priv = JINGLE_S5B_GET_PRIVATE(s5b);
	memset(s5b->priv, 0, sizeof(s5b->priv));
	s5b->priv->local_streamhosts = NULL;
	s5b->priv->remote_streamhosts = NULL;
	s5b->priv->fd = 0;
	s5b->priv->local_fd = 0;
	s5b->priv->remote_fd = 0;
	s5b->priv->sid = NULL;
}

static void
jingle_s5b_finalize (GObject *s5b)
{
	JingleS5BPrivate *priv = JINGLE_S5B_GET_PRIVATE(s5b);
	purple_debug_info("jingle-s5b","jingle_s5b_finalize\n");
	
	if (priv->sid)
		g_free(priv->sid);
	
	/* free the local streamhosts */
	while (priv->local_streamhosts) {
		jingle_s5b_streamhost_destroy(
			(JabberBytestreamsStreamhost *)priv->local_streamhosts->data);
		priv->local_streamhosts = g_list_delete_link(priv->local_streamhosts, 
			priv->local_streamhosts);
	}
	
	/* free the remote streamhosts */
	while (priv->local_streamhosts) {
		jingle_s5b_streamhost_destroy(
			(JabberBytestreamsStreamhost *)priv->remote_streamhosts->data);
		priv->remote_streamhosts = g_list_delete_link(priv->remote_streamhosts, 
			priv->remote_streamhosts);
	}
	
	G_OBJECT_CLASS(parent_class)->finalize(s5b);
}

static void
jingle_s5b_set_property (GObject *object, guint prop_id, const GValue *value, 
	GParamSpec *pspec)
{
	JingleS5B *s5b;
	g_return_if_fail(JINGLE_IS_S5B(object));

	s5b = JINGLE_S5B(object);

	switch (prop_id) {
		case PROP_SID:
			g_free(s5b->priv->sid);
			s5b->priv->sid = g_value_dup_string(value);
			break;	
		default:	
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
jingle_s5b_get_property (GObject *object, guint prop_id, GValue *value, 
	GParamSpec *pspec)
{
	JingleS5B *s5b;
	g_return_if_fail(JINGLE_IS_S5B(object));
	
	s5b = JINGLE_S5B(object);

	switch (prop_id) {
		case PROP_SID:
			g_value_set_string(value, s5b->priv->sid);
			break;
		default:	
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);	
			break;
	}
}

const gchar *
jingle_s5b_get_sid(const JingleS5B *s5b)
{
	gchar *sid;
	g_object_get((gpointer) s5b, "sid", &sid, NULL);
	return sid;
}

void
jingle_s5b_set_sid(JingleS5B *s5b, const gchar *sid)
{
	g_object_set(s5b, "sid", sid, NULL);
}

static JingleTransport *
jingle_s5b_parse_internal(xmlnode *s5b)
{
	JingleTransport *transport = parent_class->parse(s5b);
	JingleS5BPrivate *priv = JINGLE_S5B_GET_PRIVATE(transport);
	xmlnode *streamhost;
	
	/* set the sid from the incoming transport */
	jingle_s5b_set_sid(JINGLE_S5B(transport), 
		xmlnode_get_attrib(s5b, "sid"));
	
	for (streamhost = xmlnode_get_child(s5b, "streamhost");
		 streamhost;
		 streamhost = xmlnode_get_next_twin(streamhost)) {
		JabberBytestreamsStreamhost *sh = 
			jingle_s5b_streamhost_create_from_xml(streamhost);
		if (sh) {
			purple_debug_info("jingle-s5b", 
				"adding streamhost jid = %s, host = %s, port = %d to remote "
				"streamhosts\n", sh->jid, sh->host, sh->port);
			priv->remote_streamhosts = 
				g_list_append(priv->remote_streamhosts, sh);
		}
	}
	
	/* should start connection attempts here... */
	
	return transport;
}

static xmlnode *
jingle_s5b_to_xml_internal(JingleTransport *transport, xmlnode *content, 
	JingleActionType action)
{
	xmlnode *node = parent_class->to_xml(transport, content, action);
	const GList *iter;
	JingleS5B *s5b = JINGLE_S5B(transport);
	
	purple_debug_info("jingle", "jingle_s5b_to_xml_internal\n");

	xmlnode_set_attrib(node, "sid", jingle_s5b_get_sid(s5b));
	
	/* always set "mode" to "tcp" */
	xmlnode_set_attrib(node, "mode", "tcp");

	if (action == JINGLE_SESSION_INITIATE || action == JINGLE_SESSION_ACCEPT) {
		for (iter = JINGLE_S5B_GET_PRIVATE(s5b)->local_streamhosts; 
			iter; 
			iter = g_list_next(iter)) {
			JabberBytestreamsStreamhost *sh = 
				(JabberBytestreamsStreamhost *) iter->data;
			xmlnode_insert_child(node, jingle_s5b_streamhost_to_xml(sh));
		}
	}

	return node;
}

typedef struct {
	JingleSession *session;
	JingleS5B *s5b;
} JingleS5BListenData;

static void
jingle_s5b_listen_cb(int sock, gpointer data)
{
	JingleS5B *s5b = ((JingleS5BListenData *) data)->s5b;
	JingleSession *session = ((JingleS5BListenData *) data)->session;
	JabberStream *js = jingle_session_get_js(session);
	const GList *iter;
	
	JINGLE_S5B_GET_PRIVATE(s5b)->listen_data = NULL;
	
	g_free(data);
	
	if (sock > 0) {
		guint local_port = purple_network_get_port_from_fd(sock);
		const gchar *local_ip = purple_network_get_local_system_ip(js->fd);
		const gchar *public_ip = purple_network_get_my_ip(js->fd);
		gchar *jid = g_strdup_printf("%s@%s/%s", js->user->node,
			js->user->domain, js->user->resource);

		purple_debug_info("jingle-s5b", "successfully open port %d locally\n",
			local_port);

		if (!purple_strequal(local_ip, "0.0.0.0")) {
			JabberBytestreamsStreamhost *sh =
				jingle_s5b_streamhost_create(jid, local_ip, local_port, NULL);
			JINGLE_S5B_GET_PRIVATE(s5b)->local_streamhosts =
				g_list_append(JINGLE_S5B_GET_PRIVATE(s5b)->local_streamhosts,
					sh);
		}

		if (!purple_strequal(local_ip, public_ip) && 
			!purple_strequal(public_ip, "0.0.0.0")) {
			JabberBytestreamsStreamhost *sh =
				jingle_s5b_streamhost_create(jid, public_ip, local_port, NULL);
			JINGLE_S5B_GET_PRIVATE(s5b)->local_streamhosts =
				g_list_append(JINGLE_S5B_GET_PRIVATE(s5b)->local_streamhosts,
					sh);	
		}

		JINGLE_S5B_GET_PRIVATE(s5b)->local_fd = sock;
		
		g_free(jid);
	}
	
	/* add bytestream proxies */
	for (iter = js->bs_proxies ; iter ; iter = g_list_next(iter)) {
		JabberBytestreamsStreamhost *sh =
			(JabberBytestreamsStreamhost *) iter->data;
		
		purple_debug_info("jingle-s5b", 
			"found local bytestream proxy jid = %s, host = %s, port %d\n",
			sh->jid ? sh->jid : "(null)", sh->host ? sh->host : "(null)",
			sh->port);
		
		/* is this check really nessesary? si.c does it so... */
		if (sh->jid && sh->host && sh->port > 0) {
			JINGLE_S5B_GET_PRIVATE(s5b)->local_streamhosts =
				g_list_append(JINGLE_S5B_GET_PRIVATE(s5b)->local_streamhosts,
					jingle_s5b_streamhost_copy(sh));		
		}
	}
	/* note: even if we couldn't obtain a local port and no bytestream
	 proxies where found above, we should not revert to IBB just yet, since
	 the other end might have working streamhosts... */
	
	/* if we are the initiator send session-initiate */
	if (jingle_session_is_initiator(session)) {
		jabber_iq_send(jingle_session_to_packet(session, 
			JINGLE_SESSION_INITIATE));
	} else {
		jabber_iq_send(jingle_session_to_packet(session,
			JINGLE_SESSION_ACCEPT));
	}
	g_object_unref(session);
}

void
jingle_s5b_gather_streamhosts(JingleSession *session, JingleS5B *s5b)
{
	JingleS5BListenData *data = g_new0(JingleS5BListenData, 1);
	data->session = session;
	data->s5b = s5b;
	g_object_ref(session);
	JINGLE_S5B_GET_PRIVATE(s5b)->listen_data = 
		purple_network_listen_range(0, 0, SOCK_STREAM, jingle_s5b_listen_cb,
			data);
	
	if (JINGLE_S5B_GET_PRIVATE(s5b)->listen_data == NULL) {
		/* could not listen on a local port, will try to use only proxies,
		 if possible */
		jingle_s5b_listen_cb(-1, data);
	}
}

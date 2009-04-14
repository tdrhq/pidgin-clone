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


#define STREAMHOST_CONNECT_TIMEOUT_MILLIS 200

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

static void
jingle_s5b_streamhost_add_xml_internal(xmlnode *node, 
	const JabberBytestreamsStreamhost *sh)
{
	gchar port[10];

	if (node) {
		g_snprintf(port, 10, "%d", sh->port);
		xmlnode_set_attrib(node, "jid", sh->jid);
		xmlnode_set_attrib(node, "host", sh->host);
		xmlnode_set_attrib(node, "port", port);
		if (sh->zeroconf)
			xmlnode_set_attrib(node, "zeroconf", sh->zeroconf);
	}
}

static xmlnode *
jingle_s5b_streamhost_to_xml(const JabberBytestreamsStreamhost *sh)
{
	xmlnode *streamhost = xmlnode_new("streamhost");
	
	jingle_s5b_streamhost_add_xml_internal(streamhost, sh);
	
	return streamhost;
}

static xmlnode *
jingle_s5b_streamhost_to_xml_used(const JabberBytestreamsStreamhost *sh)
{
	xmlnode *streamhost_used = xmlnode_new("streamhost-used");
	
	jingle_s5b_streamhost_add_xml_internal(streamhost_used, sh);
	
	return streamhost_used;
}

struct _JingleS5BPrivate {
	/* S5B stuff here... */
	gchar *sid;
	guint fd;
	guint local_fd;
	guint remote_fd;
	PurpleProxyConnectData *connect_data;
	PurpleNetworkListenData *listen_data;
	PurpleProxyInfo *ppi;
	guint connect_timeout;
	int watcher;
	char *rxqueue;
	size_t rxlen;
	gsize rxmaxlen;
	GList *remote_streamhosts;
	GList *local_streamhosts;
	GList *remaining_streamhosts; /* pointer to untested remote SHs */
	JabberBytestreamsStreamhost *successfull_remote_streamhost;
	JingleS5BConnectCallback *connect_cb;
	JingleS5BErrorCallback *error_cb;
	JingleContent *connect_content; /* used for the connect callback */
	JingleContent *error_content;  /* used for the error callback */
	gboolean is_connected_to_remote;
	gboolean is_remote_connected;
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
	PROP_SID,
	PROP_FD,
	PROP_CONNECT_CB,
	PROP_ERROR_CB
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
	
	purple_debug_info("jingle-s5b", "install_property\n");
	g_object_class_install_property(gobject_class, PROP_FD,
		g_param_spec_int("fd",
		"File descriptor",
		"The file descriptor for reading/writing data on the stream",
		G_MININT,
		G_MAXINT,
		0,
		G_PARAM_READABLE));
	
	g_type_class_add_private(klass, sizeof(JingleS5BPrivate));
}

static void
jingle_s5b_init (JingleS5B *s5b)
{	
	s5b->priv = JINGLE_S5B_GET_PRIVATE(s5b);
	memset(s5b->priv, 0, sizeof(s5b->priv));
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
	
	if (priv->ppi)
		purple_proxy_info_destroy(priv->ppi);

	if (priv->connect_timeout)
		purple_timeout_remove(priv->connect_timeout);
	
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
		case PROP_FD:
			g_value_set_int(value, s5b->priv->fd);
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

int
jingle_s5b_get_fd(const JingleS5B *s5b)
{
	int fd;
	g_object_get((gpointer) s5b, "fd", &fd, NULL);
	return fd;
}

void jingle_s5b_set_connect_callback(JingleS5B *s5b, 
	JingleS5BConnectCallback *cb, JingleContent *content)
{
	s5b->priv->connect_cb = cb;
	s5b->priv->connect_content = content;
}
	
void jingle_s5b_set_error_callback(JingleS5B *s5b, 
	JingleS5BErrorCallback *cb, JingleContent *content)
{
	s5b->priv->error_cb = cb;
	s5b->priv->error_content = content;
}

void
jingle_s5b_add_streamhosts(JingleS5B *s5b, const xmlnode *transport)
{
	xmlnode *streamhost;
	
	for (streamhost = xmlnode_get_child(transport, "streamhost");
		 streamhost;
		 streamhost = xmlnode_get_next_twin(streamhost)) {
		JabberBytestreamsStreamhost *sh = 
			jingle_s5b_streamhost_create_from_xml(streamhost);
		if (sh) {
			purple_debug_info("jingle-s5b", 
				"adding streamhost jid = %s, host = %s, port = %d to remote "
				"streamhosts\n", sh->jid, sh->host, sh->port);
			s5b->priv->remote_streamhosts = 
				g_list_append(s5b->priv->remote_streamhosts, sh);
		}
	}
}

static JingleTransport *
jingle_s5b_parse_internal(xmlnode *s5b)
{
	JingleTransport *transport = parent_class->parse(s5b);

	/* set the sid from the incoming transport */
	jingle_s5b_set_sid(JINGLE_S5B(transport), 
		xmlnode_get_attrib(s5b, "sid"));
	
	jingle_s5b_add_streamhosts(JINGLE_S5B(transport), s5b);
	
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
	} else if (action == JINGLE_TRANSPORT_ACCEPT) {
		/* should include the chosen streamhost here... */
		xmlnode_insert_child(node, 
			jingle_s5b_streamhost_to_xml_used(
				s5b->priv->successfull_remote_streamhost));
	}

	return node;
}

typedef struct {
	JingleSession *session;
	JingleS5B *s5b;
} JingleS5BConnectData;

static void
jingle_s5b_send_read_again_resp_cb(gpointer data, gint source,
	PurpleInputCondition cond)
{
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;
	int len;
	
	purple_debug_info("jingle-s5b", "in jingle_s5b_send_read_again_cb\n");
	len = write(source, s5b->priv->rxqueue + s5b->priv->rxlen,
		s5b->priv->rxmaxlen - s5b->priv->rxlen);
	if (len < 0 && errno == EAGAIN)
		return;
	else if (len < 0) {
		purple_input_remove(s5b->priv->watcher);
		s5b->priv->watcher = 0;
		g_free(s5b->priv->rxqueue);
		s5b->priv->rxqueue = NULL;
		close(source);
		/* TODO: signal to the content that an error has occurred */
		return;
	}
	s5b->priv->rxlen += len;

	if (s5b->priv->rxlen < s5b->priv->rxmaxlen)
		return;

	purple_input_remove(s5b->priv->watcher);
	s5b->priv->watcher = 0;
	g_free(s5b->priv->rxqueue);
	s5b->priv->rxqueue = NULL;

	/* Before actually starting sending the file, we need to wait until the
	 * recipient sends the IQ result with <streamhost-used/>
	 */
	purple_debug_info("jingle-s5b", "SOCKS5 connection negotiation completed. "
					  "Waiting for IQ result to start file transfer.\n");
	/* set the local fd as connected */
	s5b->priv->local_fd = source;
	s5b->priv->is_remote_connected = TRUE;
	
	g_free(data);
}

static void
jingle_s5b_send_read_again_cb(gpointer data, gint source,
	PurpleInputCondition cond)
{
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;
	JingleSession *session = ((JingleS5BConnectData *) data)->session;
	const gchar *who = jingle_session_get_remote_jid(session);
	JabberID *dstjid = jabber_id_new(who);
	JabberStream *js = jingle_session_get_js(session);
	char buffer[256];
	int len;
	char *dstaddr, *hash;
	const char *host;

	purple_debug_info("jingle-s5b", 
		"in jingle_s5b_send_read_again_cb\n");

	if(s5b->priv->rxlen < 5) {
		purple_debug_info("jingle-s5b", "reading the first 5 bytes\n");
		len = read(source, buffer, 5 - s5b->priv->rxlen);
		if(len < 0 && errno == EAGAIN)
			return;
		else if(len <= 0) {
			purple_input_remove(s5b->priv->watcher);
			s5b->priv->watcher = 0;
			close(source);
			/* TODO: signal the content that an error occured */
			return;
		}
		s5b->priv->rxqueue = 
			g_realloc(s5b->priv->rxqueue, len + s5b->priv->rxlen);
		memcpy(s5b->priv->rxqueue + s5b->priv->rxlen, buffer, len);
		s5b->priv->rxlen += len;
		return;
	} else if(s5b->priv->rxqueue[0] != 0x05 || s5b->priv->rxqueue[1] != 0x01 ||
			s5b->priv->rxqueue[3] != 0x03) {
		purple_debug_info("jingle-s5b", "invalid socks5 stuff\n");
		purple_input_remove(s5b->priv->watcher);
		s5b->priv->watcher = 0;
		close(source);
		/* TODO: signal the content that an error occured */
		return;
	} else if(s5b->priv->rxlen - 5 <  s5b->priv->rxqueue[4] + 2) {
		purple_debug_info("jingle-s5b", "reading umpteen more bytes\n");
		len = read(source, buffer, 
			s5b->priv->rxqueue[4] + 5 + 2 - s5b->priv->rxlen);
		if(len < 0 && errno == EAGAIN)
			return;
		else if(len <= 0) {
			purple_input_remove(s5b->priv->watcher);
			s5b->priv->watcher = 0;
			close(source);
			/* TODO: signal the content that an error occured */
			return;
		}
		s5b->priv->rxqueue = 
			g_realloc(s5b->priv->rxqueue, len + s5b->priv->rxlen);
		memcpy(s5b->priv->rxqueue + s5b->priv->rxlen, buffer, len);
		s5b->priv->rxlen += len;
	}

	if(s5b->priv->rxlen - 5 < s5b->priv->rxqueue[4] + 2)
		return;

	purple_input_remove(s5b->priv->watcher);
	s5b->priv->watcher = 0;

	if(jingle_session_is_initiator(session))
		dstaddr = g_strdup_printf("%s%s@%s/%s%s@%s/%s", s5b->priv->sid, 
			js->user->node, js->user->domain, js->user->resource, 
			dstjid->node, dstjid->domain, dstjid->resource);
	else
		dstaddr = g_strdup_printf("%s%s@%s/%s%s@%s/%s", s5b->priv->sid, 
			dstjid->node, dstjid->domain, dstjid->resource,
			js->user->node, js->user->domain, js->user->resource);

	g_free(dstjid);
		
	/* Per XEP-0065, the 'host' must be SHA1(SID + from JID + to JID) */
	hash = jabber_calculate_data_sha1sum(dstaddr, strlen(dstaddr));
	purple_debug_info("jingle-s5b", "dstaddr: %s\n", dstaddr);
	purple_debug_info("jingle-s5b", "expecting to receive hash %s\n", hash);
	
	if(s5b->priv->rxqueue[4] != 40 || strncmp(hash, s5b->priv->rxqueue+5, 40) ||
			s5b->priv->rxqueue[45] != 0x00 || s5b->priv->rxqueue[46] != 0x00) {
		purple_debug_error("jingle-s5b", 
			"someone connected with the wrong info!\n");
		close(source);
		/* TODO: signal the content that an error occured */
		g_free(hash);
		g_free(dstaddr);
		return;
	}

	g_free(hash);
	g_free(dstaddr);

	g_free(s5b->priv->rxqueue);
	host = purple_network_get_my_ip(js->fd);

	s5b->priv->rxmaxlen = 5 + strlen(host) + 2;
	s5b->priv->rxqueue = g_malloc(s5b->priv->rxmaxlen);
	s5b->priv->rxlen = 0;

	s5b->priv->rxqueue[0] = 0x05;
	s5b->priv->rxqueue[1] = 0x00;
	s5b->priv->rxqueue[2] = 0x00;
	s5b->priv->rxqueue[3] = 0x03;
	s5b->priv->rxqueue[4] = strlen(host);
	memcpy(s5b->priv->rxqueue + 5, host, strlen(host));
	s5b->priv->rxqueue[5+strlen(host)] = 0x00;
	s5b->priv->rxqueue[6+strlen(host)] = 0x00;

	s5b->priv->watcher = purple_input_add(source, PURPLE_INPUT_WRITE,
		jingle_s5b_send_read_again_resp_cb, data);
	jingle_s5b_send_read_again_resp_cb(data, source,
		PURPLE_INPUT_WRITE);
}

static void
jingle_s5b_send_read_response_cb(gpointer data, gint source,
	PurpleInputCondition cond)
{
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;
	int len;
	
	purple_debug_info("jingle-s5b", "in jingle_s5b_send_read_response_cb\n");
	
	len = write(source, s5b->priv->rxqueue + s5b->priv->rxlen, 
		s5b->priv->rxmaxlen - s5b->priv->rxlen);
	if (len < 0 && errno == EAGAIN)
		return;
	else if (len < 0) {
		purple_input_remove(s5b->priv->watcher);
		s5b->priv->watcher = 0;
		g_free(s5b->priv->rxqueue);
		s5b->priv->rxqueue = NULL;
		close(source);
		/* TODO: signal to "surrounding" content that an error has occured */
		return;
	}
	s5b->priv->rxlen += len;

	if (s5b->priv->rxlen < s5b->priv->rxmaxlen)
		return;

	purple_input_remove(s5b->priv->watcher);
	s5b->priv->watcher = 0;

	if (s5b->priv->rxqueue[1] == 0x00) {
		s5b->priv->watcher = purple_input_add(source, PURPLE_INPUT_READ,
			jingle_s5b_send_read_again_cb, data);
		g_free(s5b->priv->rxqueue);
		s5b->priv->rxqueue = NULL;
	} else {
		close(source);
		/* TODO: signal "surrounding" content that an error has occured */
	}
}
	
static void
jingle_s5b_send_read_cb(gpointer data, gint source, PurpleInputCondition cond)
{
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;
	int i;
	int len;
	char buffer[256];

	purple_debug_info("jingle-s5b", "in jingle_s5b_send_read_cb\n");
	
	s5b->priv->local_fd = source;
	
	if(s5b->priv->rxlen < 2) {
		purple_debug_info("jingle-s5b", "reading those first two bytes\n");
		len = read(source, buffer, 2 - s5b->priv->rxlen);
		if(len < 0 && errno == EAGAIN)
			return;
		else if(len <= 0) {
			purple_input_remove(s5b->priv->watcher);
			s5b->priv->watcher = 0;
			close(source);
			/* TODO: signal the "surrounding" content an error has occured */
			return;
		}
		s5b->priv->rxqueue = 
			g_realloc(s5b->priv->rxqueue, len + s5b->priv->rxlen);
		memcpy(s5b->priv->rxqueue + s5b->priv->rxlen, buffer, len);
		s5b->priv->rxlen += len;
		return;
	} else if(s5b->priv->rxlen - 2 < s5b->priv->rxqueue[1]) {
		purple_debug_info("jingle-s5b", "reading the next umpteen bytes\n");
		len = read(source, buffer, s5b->priv->rxqueue[1] + 2 - s5b->priv->rxlen);
		if(len < 0 && errno == EAGAIN)
			return;
		else if(len <= 0) {
			purple_input_remove(s5b->priv->watcher);
			s5b->priv->watcher = 0;
			close(source);
			/* TODO: signal the "surrounding" content an error has occured */
			return;
		}
		s5b->priv->rxqueue = 
			g_realloc(s5b->priv->rxqueue, len + s5b->priv->rxlen);
		memcpy(s5b->priv->rxqueue + s5b->priv->rxlen, buffer, len);
		s5b->priv->rxlen += len;
	}

	if(s5b->priv->rxlen -2 < s5b->priv->rxqueue[1])
		return;

	purple_input_remove(s5b->priv->watcher);
	s5b->priv->watcher = 0;

	purple_debug_info("jingle-s5b", "checking to make sure we're socks FIVE\n");

	if(s5b->priv->rxqueue[0] != 0x05) {
		purple_debug_error("jingle-s5b", "it's not socks FIVE, giving up\n");
		close(source);
		/* TODO: signal to the "surrounding" content that an error has occured */
		return;
	}

	purple_debug_info("jingle-s5b", "going to test %hhu different methods\n", 
		s5b->priv->rxqueue[1]);

	for(i = 0; i < s5b->priv->rxqueue[1]; i++) {
		purple_debug_info("jingle-s5b", "testing %hhu\n", 
			s5b->priv->rxqueue[i+2]);
		if(s5b->priv->rxqueue[i+2] == 0x00) {
			g_free(s5b->priv->rxqueue);
			s5b->priv->rxlen = 0;
			s5b->priv->rxmaxlen = 2;
			s5b->priv->rxqueue = g_malloc(s5b->priv->rxmaxlen);
			s5b->priv->rxqueue[0] = 0x05;
			s5b->priv->rxqueue[1] = 0x00;
			s5b->priv->watcher = purple_input_add(source, PURPLE_INPUT_WRITE,
				jingle_s5b_send_read_response_cb, data);
			jingle_s5b_send_read_response_cb(data, source, PURPLE_INPUT_WRITE);
			s5b->priv->rxqueue = NULL;
			s5b->priv->rxlen = 0;
			return;
		}
	}

	/* question to self: is this called if there was no methods given? */
	g_free(s5b->priv->rxqueue);
	s5b->priv->rxlen = 0;
	s5b->priv->rxmaxlen = 2;
	s5b->priv->rxqueue = g_malloc(s5b->priv->rxmaxlen);
	s5b->priv->rxqueue[0] = 0x05;
	s5b->priv->rxqueue[1] = 0xFF;
	s5b->priv->watcher = purple_input_add(source, PURPLE_INPUT_WRITE,
		jingle_s5b_send_read_response_cb, data);
	jingle_s5b_send_read_response_cb(data,
		source, PURPLE_INPUT_WRITE);
}

static void
jingle_s5b_send_connected_cb(gpointer data, gint source,
		PurpleInputCondition cond)
{
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;
	int acceptfd = accept(source, NULL, 0);
	int flags;
	
	purple_debug_info("jingle-s5b", "in jingle_s5b_send_connected_cb\n");
	
	if(acceptfd == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
		return;
	else if(acceptfd == -1) {
		purple_debug_warning("jingle-s5b", "accept: %s\n", g_strerror(errno));
		/* Don't cancel the ft - allow it to fall to the next streamhost.*/
		return;
	}

	purple_input_remove(s5b->priv->watcher);
	close(source);
	s5b->priv->local_fd = -1;

	flags = fcntl(acceptfd, F_GETFL);
	fcntl(acceptfd, F_SETFL, flags | O_NONBLOCK);
#ifndef _WIN32
	fcntl(acceptfd, F_SETFD, FD_CLOEXEC);
#endif

	s5b->priv->watcher = purple_input_add(acceptfd, PURPLE_INPUT_READ,
					 jingle_s5b_send_read_cb, data);
}

static void
jingle_s5b_listen_cb(int sock, gpointer data)
{
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;
	JingleSession *session = ((JingleS5BConnectData *) data)->session;
	JabberStream *js = jingle_session_get_js(session);
	const GList *iter;
	
	JINGLE_S5B_GET_PRIVATE(s5b)->listen_data = NULL;
	
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

		s5b->priv->local_fd = sock;
		
		/* The listener for the local proxy */
		s5b->priv->watcher = purple_input_add(sock, PURPLE_INPUT_READ,
			jingle_s5b_send_connected_cb, data);
		
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
		/* now we can attempt to connect to the streamhosts the other end
		 handed to us... */
		jingle_s5b_attempt_connect(session, s5b);
		/* Note: this is done in the respective content handler for the
		 initiator, since the initiator needs to wait for session-accept to get
		 streamhost candidates, while as the receiver we need to do it here,
		 since we can't do it before having sent session-accept */
	}
	
	g_object_unref(session);
}

void
jingle_s5b_gather_streamhosts(JingleSession *session, JingleS5B *s5b)
{
	JingleS5BConnectData *data = g_new0(JingleS5BConnectData, 1);
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

static void
jingle_s5b_transport_accept_cb(JabberStream *js, const char *from,
	JabberIqType type, const char *id, xmlnode *packet, gpointer data)
{
	JingleS5BConnectData *cd = (JingleS5BConnectData *) data;
	JingleSession *session = cd->session;
	JingleS5B *s5b = cd->s5b;
	
	purple_debug_info("jingle-s5b", 
		"in jingle_s5b_transport_accept_cb: is connected to remote %d,"
		" remote is connected %d\n", jingle_s5b_is_connected_to_remote(s5b),
		jingle_s5b_remote_is_connected(s5b));
	
	if (type == JABBER_IQ_RESULT) {
		if (!(!jingle_session_is_initiator(session) &&
			jingle_s5b_is_connected_to_remote(s5b)) ||
			!jingle_s5b_remote_is_connected(s5b)) {
			/* unless we are the receiver and the receiver could connect,
				now we shall "surrender" to other side and signal the content
				to start */
			jingle_s5b_surrender(s5b);
			/* start transfer */
			if (s5b->priv->connect_cb && s5b->priv->connect_content) {
				s5b->priv->connect_cb(s5b->priv->connect_content);
			} else {
				/* some error? */
			}
		}
	}
	
	g_free(cd);
}

static void
jingle_s5b_connect_cb(gpointer data, gint source, const gchar *error_message)
{
	JingleS5BConnectData *cd = (JingleS5BConnectData *) data;
	JingleS5B *s5b = cd->s5b;
	JingleSession *session = cd->session;
	JabberIq *result = NULL;
	
	purple_debug_info("jingle-s5b", "Successful in connecting!\n");
	
	s5b->priv->connect_data = NULL;
	s5b->priv->remote_fd = source;
	s5b->priv->is_connected_to_remote = TRUE;
	
	/* cancel timeout if set */
	if (s5b->priv->connect_timeout) {
		purple_timeout_remove(s5b->priv->connect_timeout);
		s5b->priv->connect_timeout = 0;
	}

	/* set the currently tried streamhost as the successfull one */
	s5b->priv->successfull_remote_streamhost =
		(JabberBytestreamsStreamhost *) s5b->priv->remaining_streamhosts->data;

	/* should stop trying to connect */
	jingle_s5b_stop_connection_attempts(s5b);
	
	/* should send transport-accept with streamhost-used */
	result = jingle_session_to_packet(session, JINGLE_TRANSPORT_ACCEPT);
	jabber_iq_set_callback(result, jingle_s5b_transport_accept_cb, cd);
	jabber_iq_send(result);
}
	
static void jingle_s5b_attempt_connect_internal(gpointer data);

static gboolean
jingle_s5b_connect_timeout_cb(gpointer data)
{
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;
	
	purple_debug_info("jingle-s5b", "in jingle_s5b_connect_timeout_cb\n");
	/* cancel timeout */
	purple_timeout_remove(s5b->priv->connect_timeout);
	s5b->priv->connect_timeout = 0;
	
	/* advance streamhost "counter" */
	if (s5b->priv->remaining_streamhosts) {
		s5b->priv->remaining_streamhosts = 
			s5b->priv->remaining_streamhosts->data;
		purple_debug_info("jingle-s5b", "trying next streamhost\n");
		/* if remaining_streamhost is NULL here, this call will result in a
		 streamhost error (and potentially fallback to IBB) */
		jingle_s5b_attempt_connect_internal(data);
	}
	
	return FALSE;
}

static void
jingle_s5b_attempt_connect_internal(gpointer data)
{
	JingleSession *session = ((JingleS5BConnectData *) data)->session;
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;
	
	if (s5b->priv->remaining_streamhosts) {
		JabberStream *js = jingle_session_get_js(session);
		const gchar *who = jingle_session_get_remote_jid(session);
		JabberID *dstjid = jabber_id_new(who);
		JabberBytestreamsStreamhost *sh =
			(JabberBytestreamsStreamhost *) s5b->priv->remaining_streamhosts->data;
		gchar *dstaddr = NULL;
		gchar *hash = NULL;
		
		purple_debug_info("jingle-s5b", 
			"attempting to connect to streamhost: %s, port: %d\n",
			sh->host, sh->port);
		
		if (s5b->priv->ppi)
			purple_proxy_info_destroy(s5b->priv->ppi);
		s5b->priv->ppi = purple_proxy_info_new();
		purple_proxy_info_set_type(s5b->priv->ppi, PURPLE_PROXY_SOCKS5);
		purple_proxy_info_set_host(s5b->priv->ppi, sh->host);
		purple_proxy_info_set_port(s5b->priv->ppi, sh->port);
		
		if(jingle_session_is_initiator(session))
			dstaddr = g_strdup_printf("%s%s@%s/%s%s@%s/%s", s5b->priv->sid, 
				js->user->node, js->user->domain, js->user->resource, 
				dstjid->node, dstjid->domain, dstjid->resource);
		else
			dstaddr = g_strdup_printf("%s%s@%s/%s%s@%s/%s", s5b->priv->sid, 
				dstjid->node, dstjid->domain, dstjid->resource,
				js->user->node, js->user->domain, js->user->resource);

		/* Per XEP-0065, the 'host' must be SHA1(SID + from JID + to JID) */
		purple_debug_info("jingle-s5b", "dstaddr: %s\n", dstaddr);
		hash = jabber_calculate_data_sha1sum(dstaddr, strlen(dstaddr));
		purple_debug_info("jingle-s5b", "connecting with hash: %s\n", hash);
		
		s5b->priv->connect_data = 
			purple_proxy_connect_socks5(NULL, s5b->priv->ppi, hash, 0, 
				jingle_s5b_connect_cb, data);
		g_free(hash);
		g_free(dstaddr);
		
		/* add timeout */
		/* we should add a longer timeout if the next streamhost candidate
		 is a proxy and we have local candidates ourselves, to allow the other
		 end a chance to connect to use before reverting to a proxy */
		s5b->priv->connect_timeout =
			purple_timeout_add(STREAMHOST_CONNECT_TIMEOUT_MILLIS,
				jingle_s5b_connect_timeout_cb, data);
		
		g_free(dstjid);
	} else {
		/* send streamhost error */
		
		g_free(data);
	}
}

void
jingle_s5b_attempt_connect(JingleSession *session, JingleS5B *s5b)
{
	JingleS5BConnectData *data = g_new0(JingleS5BConnectData, 1);
	
	data->session = session;
	data->s5b = s5b;
	s5b->priv->remaining_streamhosts = s5b->priv->remote_streamhosts;
	jingle_s5b_attempt_connect_internal(data);
}

void
jingle_s5b_stop_connection_attempts(JingleS5B *s5b)
{
	purple_debug_info("jingle-s5b", "stop connection attempts\n");
	
	s5b->priv->remaining_streamhosts = NULL;
	
	if (s5b->priv->connect_data) {
		purple_proxy_connect_cancel(s5b->priv->connect_data);
		s5b->priv->connect_data = NULL;
	}
	
	if (s5b->priv->watcher) {
		purple_input_remove(s5b->priv->watcher);
		s5b->priv->watcher = 0;
	}
	
	if (s5b->priv->connect_timeout) {
		purple_timeout_remove(s5b->priv->connect_timeout);
		s5b->priv->connect_timeout = 0;
	}
}

gboolean
jingle_s5b_is_connected_to_remote(const JingleS5B *s5b)
{
	return s5b->priv->is_connected_to_remote;
}

gboolean
jingle_s5b_remote_is_connected(const JingleS5B *s5b)
{
	return s5b->priv->is_remote_connected;
}
	
void
jingle_s5b_surrender(JingleS5B *s5b)
{
	purple_debug_info("jingle-s5b", 
		"in jingle_s5b_surrender, using remote streamhost\n");
	s5b->priv->fd = s5b->priv->remote_fd;
	
	if (s5b->priv->local_fd) {
		close(s5b->priv->local_fd);
		s5b->priv->local_fd = 0;
	}
	
	if (s5b->priv->watcher) {
		purple_input_remove(s5b->priv->watcher);
		s5b->priv->watcher = 0;
	}
	
	if (s5b->priv->connect_timeout) {
		purple_timeout_remove(s5b->priv->connect_timeout);
		s5b->priv->connect_timeout = 0;
	}
}

void
jingle_s5b_take_command(JingleS5B *s5b)
{
	purple_debug_info("jingle-s5b",
		"in jingle_s5b_take_command, using local streamhost\n");
	s5b->priv->fd = s5b->priv->local_fd;
	
	if (s5b->priv->remote_fd) {
		close(s5b->priv->remote_fd);
		s5b->priv->remote_fd = 0;
	}
	
	if (s5b->priv->watcher) {
		purple_input_remove(s5b->priv->watcher);
		s5b->priv->watcher = 0;
	}
	
	if (s5b->priv->connect_timeout) {
		purple_timeout_remove(s5b->priv->connect_timeout);
		s5b->priv->connect_timeout = 0;
	}
}

gboolean
jingle_s5b_streamhost_is_local(JabberStream *js, const gchar *jid)
{
	gchar *me = g_strdup_printf("%s@%s/%s", js->user->node, js->user->domain, 
		js->user->resource);
	gchar *me_bare = jabber_get_bare_jid(me);
	gchar *bare_jid = jabber_get_bare_jid(jid);
	gboolean equal = purple_strequal(bare_jid, me_bare);

	purple_debug_info("jingle-s5b", 
		"jingle_s5b_streamhost_is_local: comparing JIDs %s and %s\n",
		me, jid);
	
	g_free(me);
	g_free(me_bare);
	g_free(bare_jid);

	return equal;
}
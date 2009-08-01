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


#define STREAMHOST_CONNECT_TIMEOUT_MILLIS 2000
#define STREAMHOST_CONNECT_PROXY_TIMEOUT_MILLIS 10000

/* auxillary functions to handle JabberBytestreamsStreamhosts, maybe this
 should be in a separtate module, used by si.c and other places as well */

typedef enum {
	JINGLE_S5B_CANDIDATE_TYPE_DIRECT,
	JINGLE_S5B_CANDIDATE_TYPE_ASSISTED,
	JINGLE_S5B_CANDIDATE_TYPE_TUNNEL,
	JINGLE_S5B_CANDIDATE_TYPE_PROXY,
	JINGLE_S5B_CANDIDATE_TYPE_UNKNOWN
} JingleS5BCandidateType;

typedef struct {
	gchar *cid;
	gchar *host;
	gchar *jid;
	guint16 port;
	guint priority;
	JingleS5BCandidateType type;
} JingleS5BCandidate;

static guint
jingle_s5b_candidate_get_prio(JingleS5BCandidateType type)
{
	guint type_preference;

	switch (type) {
		case JINGLE_S5B_CANDIDATE_TYPE_DIRECT:
			type_preference = 126;
			break;
		case JINGLE_S5B_CANDIDATE_TYPE_ASSISTED:
			type_preference = 120;
			break;
		case JINGLE_S5B_CANDIDATE_TYPE_TUNNEL:
			type_preference = 110;
			break;
		case JINGLE_S5B_CANDIDATE_TYPE_PROXY:
			type_preference = 10;
			break;
		default:
			type_preference = 0;
			break;
	}

	/* we set "local preference" to 0 (see XEP-0260) */
	return 65536 * type_preference;
}

static JingleS5BCandidate *
jingle_s5b_candidate_create(JabberStream *js, const gchar *jid, const gchar *host,
	guint16 port, JingleS5BCandidateType type)  
{
	JingleS5BCandidate *candidate = g_new0(JingleS5BCandidate, 1);
	
	if (candidate) {
		candidate->cid = jabber_get_next_id(js);
		candidate->jid = g_strdup(jid);
		candidate->host = g_strdup(host);
		candidate->port = port;
		candidate->type = type;
		candidate->priority = jingle_s5b_candidate_get_prio(type);
	}

	return candidate;
}

static JingleS5BCandidate *
jingle_s5b_candidate_create_from_xml(xmlnode *candidate)
{
	JingleS5BCandidate *cand = NULL;
	const gchar *cid = xmlnode_get_attrib(candidate, "cid");
	const gchar *jid = xmlnode_get_attrib(candidate, "jid");
	const gchar *host = xmlnode_get_attrib(candidate, "host");
	const gchar *port = xmlnode_get_attrib(candidate, "port");
	const gchar *priority = xmlnode_get_attrib(candidate, "priority");
	const gchar *type = xmlnode_get_attrib(candidate, "type");

	if (cid && host && priority) {
		cand = g_new0(JingleS5BCandidate, 1);
		cand->cid = g_strdup(cid);
		cand->jid = g_strdup(jid);
		cand->host = g_strdup(host);
		if (port)
			cand->port = atoi(port);
		if (priority)
			cand->priority = atoi(priority);

		if (purple_strequal(type, "direct"))
			cand->type = JINGLE_S5B_CANDIDATE_TYPE_DIRECT;
		else if (purple_strequal(type, "assisted"))
			cand->type = JINGLE_S5B_CANDIDATE_TYPE_ASSISTED;
		else if (purple_strequal(type, "tunnel"))
			cand->type = JINGLE_S5B_CANDIDATE_TYPE_TUNNEL;
		else if (purple_strequal(type, "proxy"))
			cand->type = JINGLE_S5B_CANDIDATE_TYPE_PROXY;
		else
			cand->type = JINGLE_S5B_CANDIDATE_TYPE_UNKNOWN;
	}
	return cand;
}

static void
jingle_s5b_candidate_destroy(JingleS5BCandidate *candidate)
{
	g_free(candidate->cid);
	g_free(candidate->jid);
	g_free(candidate->host);
	g_free(candidate);
}

static JingleS5BCandidate *
jingle_s5b_candidate_copy(const JingleS5BCandidate *candidate)
{
	JingleS5BCandidate *new_cand = g_new0(JingleS5BCandidate, 1);
	
	if (new_cand) {
		new_cand->cid = g_strdup(candidate->cid);
		new_cand->jid = g_strdup(candidate->jid);
		new_cand->host = g_strdup(candidate->host);
		new_cand->port = candidate->port;
		new_cand->priority = candidate->priority;
		new_cand->type = candidate->type;
	}

	return new_cand;
}

static void
jingle_s5b_candidate_add_xml_internal(xmlnode *node, 
	const JingleS5BCandidate *cand)
{
	gchar port[10];
	gchar priority[10];

	if (node) {
		g_snprintf(port, 10, "%d", cand->port);
		g_snprintf(priority, 10, "%d", cand->priority);
		xmlnode_set_attrib(node, "cid", cand->cid);
		xmlnode_set_attrib(node, "jid", cand->jid);
		xmlnode_set_attrib(node, "host", cand->host);
		xmlnode_set_attrib(node, "port", port);
		xmlnode_set_attrib(node, "priority", priority);

		switch (cand->type) {
			case JINGLE_S5B_CANDIDATE_TYPE_DIRECT:
				xmlnode_set_attrib(node, "type", "direct");
				break;
			case JINGLE_S5B_CANDIDATE_TYPE_ASSISTED:
				xmlnode_set_attrib(node, "type", "assisted");
				break;
			case JINGLE_S5B_CANDIDATE_TYPE_TUNNEL:
				xmlnode_set_attrib(node, "type", "tunnel");
				break;
			case JINGLE_S5B_CANDIDATE_TYPE_PROXY:
				xmlnode_set_attrib(node, "type", "proxy");
				break;
			default:
				break;
		}
	}
}

static xmlnode *
jingle_s5b_candidate_to_xml(const JingleS5BCandidate *cand)
{
	xmlnode *candidate = xmlnode_new("candidate");
	
	jingle_s5b_candidate_add_xml_internal(candidate, cand);
	
	return candidate;
}

static xmlnode *
jingle_s5b_candidate_to_xml_used(const JingleS5BCandidate *cand)
{
	xmlnode *candidate_used = xmlnode_new("candidate-used");

	xmlnode_set_attrib(candidate_used, "cid", cand->cid);
	
	return candidate_used;
}

struct _JingleS5BPrivate {
	/* S5B stuff here... */
	gchar *sid;
	gint fd;
	gint local_fd;
	gint remote_fd;
	PurpleProxyConnectData *connect_data;
	PurpleNetworkListenData *listen_data;
	PurpleProxyInfo *ppi;
	guint connect_timeout;
	int watcher;
	char *rxqueue;
	size_t rxlen;
	gsize rxmaxlen;
	GList *remote_candidates;
	GList *local_candidates;
	GList *remaining_candidates; /* pointer to untested remote SHs */
	JingleS5BCandidate *successful_remote_candidate;
	JingleS5BCandidate *accepted_candidate;
	JingleS5BConnectCallback *connect_cb;
	JingleS5BErrorCallback *error_cb;
	JingleS5BFailedConnectCallback *failed_cb;
	JingleContent *connect_content; /* used for the connect callback */
	JingleContent *error_content;  /* used for the error callback */
	JingleContent *failed_content; /* used for the failed callback */
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
		-1,
		G_PARAM_READABLE));
	
	g_type_class_add_private(klass, sizeof(JingleS5BPrivate));
}

static void
jingle_s5b_init (JingleS5B *s5b)
{	
	s5b->priv = JINGLE_S5B_GET_PRIVATE(s5b);
	memset(s5b->priv, 0, sizeof(s5b->priv));
	s5b->priv->fd = -1;
	s5b->priv->local_fd = -1;
	s5b->priv->remote_fd = -1;
}

static void
jingle_s5b_finalize (GObject *s5b)
{
	JingleS5BPrivate *priv = JINGLE_S5B_GET_PRIVATE(s5b);
	purple_debug_info("jingle-s5b","jingle_s5b_finalize\n");
	
	if (priv->sid)
		g_free(priv->sid);

	/* remove port mapping */
	if (priv->fd >= 0) {
		purple_network_remove_port_mapping(priv->fd);
		close(priv->fd);
	}

	/* free the local streamhosts */
	while (priv->local_candidates) {
		jingle_s5b_candidate_destroy(
			(JingleS5BCandidate *)priv->local_candidates->data);
		priv->local_candidates = g_list_delete_link(priv->local_candidates, 
			priv->local_candidates);
	}
	
	/* free the remote streamhosts */
	while (priv->remote_candidates) {
		jingle_s5b_candidate_destroy(
			(JingleS5BCandidate *)priv->remote_candidates->data);
		priv->remote_candidates = g_list_delete_link(priv->remote_candidates, 
			priv->remote_candidates);
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

void jingle_s5b_set_failed_connect_callback(JingleS5B *s5b,
	JingleS5BFailedConnectCallback *cb, JingleContent *content)
{
	s5b->priv->failed_cb = cb;
	s5b->priv->failed_content = content;
}

static void
jingle_s5b_add_remote_candidate(JingleS5B *s5b, JingleS5BCandidate *cand)
{
	GList *iter = s5b->priv->remote_candidates;
	
	for (; iter ; iter = g_list_next(iter)) {
		JingleS5BCandidate *curr_cand = (JingleS5BCandidate *) iter->data;

		if (cand->priority > curr_cand->priority)
			break;
	}

	s5b->priv->remote_candidates =
		g_list_insert_before(s5b->priv->remote_candidates, iter, cand);
}

void
jingle_s5b_add_candidates(JingleS5B *s5b, const xmlnode *transport)
{
	xmlnode *candidate;
	
	for (candidate = xmlnode_get_child(transport, "candidate");
		 candidate;
		 candidate = xmlnode_get_next_twin(candidate)) {
		JingleS5BCandidate *cand = 
			jingle_s5b_candidate_create_from_xml(candidate);
		if (cand) {
			purple_debug_info("jingle-s5b", 
				"adding streamhost cid = %s, jid = %s, host = %s, port = %d to remote "
				"candidates\n", cand->cid, cand->jid, cand->host, cand->port);
			jingle_s5b_add_remote_candidate(s5b, cand);
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
	
	jingle_s5b_add_candidates(JINGLE_S5B(transport), s5b);
	
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
		for (iter = JINGLE_S5B_GET_PRIVATE(s5b)->local_candidates; 
			iter; 
			iter = g_list_next(iter)) {
			JingleS5BCandidate *cand = 
				(JingleS5BCandidate *) iter->data;
			xmlnode_insert_child(node, jingle_s5b_candidate_to_xml(cand));
		}
	} else if (action == JINGLE_TRANSPORT_INFO) {
		/* should include the chosen streamhost here... */
		if (s5b->priv->successful_remote_candidate) {
			xmlnode_insert_child(node, 
				jingle_s5b_candidate_to_xml_used(
					s5b->priv->successful_remote_candidate));
		}
	}

	return node;
}

static void
jingle_s5b_surrender(JingleS5B *s5b)
{
	purple_debug_info("jingle-s5b", 
		"in jingle_s5b_surrender, using remote streamhost\n");
	s5b->priv->fd = s5b->priv->remote_fd;
	
	if (s5b->priv->local_fd >= 0) {
		purple_network_remove_port_mapping(s5b->priv->local_fd);
		close(s5b->priv->local_fd);
		s5b->priv->local_fd = -1;
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

static void
jingle_s5b_take_command(JingleS5B *s5b)
{
	purple_debug_info("jingle-s5b",
		"in jingle_s5b_take_command, using local streamhost\n");
	s5b->priv->fd = s5b->priv->local_fd;
	
	if (s5b->priv->remote_fd >= 0) {
		close(s5b->priv->remote_fd);
		s5b->priv->remote_fd = -1;
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

static gboolean
jingle_s5b_is_connected_to_remote(const JingleS5B *s5b)
{
	return s5b->priv->is_connected_to_remote;
}

static gboolean
jingle_s5b_remote_is_connected(const JingleS5B *s5b)
{
	return s5b->priv->is_remote_connected;
}

void
jingle_s5b_stop_connection_attempts(JingleS5B *s5b)
{
	purple_debug_info("jingle-s5b", "stop connection attempts\n");
	
	s5b->priv->remaining_candidates = NULL;

	if (s5b->priv->listen_data) {
		purple_network_listen_cancel(s5b->priv->listen_data);
		s5b->priv->listen_data = NULL;
	}

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

	g_free(data);

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
		if (s5b->priv->error_cb && s5b->priv->error_content)
			s5b->priv->error_cb(s5b->priv->error_content);
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
	 * recipient sends the IQ result with <candidate-used/>
	 */
	purple_debug_info("jingle-s5b", "SOCKS5 connection negotiation completed. "
					  "Waiting for IQ result to start file transfer.\n");
	/* set the local fd as connected */
	s5b->priv->local_fd = source;
	s5b->priv->is_remote_connected = TRUE;
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
	char buffer[42]; /* 40 for DST.ADDR + 2 bytes for port number*/
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
			if (s5b->priv->error_cb && s5b->priv->error_content)
				s5b->priv->error_cb(s5b->priv->error_content);
			g_free(data);
			return;
		}
		s5b->priv->rxqueue = 
			g_realloc(s5b->priv->rxqueue, len + s5b->priv->rxlen);
		memcpy(s5b->priv->rxqueue + s5b->priv->rxlen, buffer, len);
		s5b->priv->rxlen += len;
		return;
	} else if(s5b->priv->rxqueue[0] != 0x05 || s5b->priv->rxqueue[1] != 0x01 ||
			s5b->priv->rxqueue[3] != 0x03 || s5b->priv->rxqueue[4] != 40) {
		purple_debug_info("jingle-s5b", 
			"Invalid socks5 conn req. header[0x%x,0x%x,0x%x,0x%x,0x%x]\n",
			s5b->priv->rxqueue[0], s5b->priv->rxqueue[1], s5b->priv->rxqueue[2],
			s5b->priv->rxqueue[3], s5b->priv->rxqueue[4]);
		purple_input_remove(s5b->priv->watcher);
		s5b->priv->watcher = 0;
		close(source);
		if (s5b->priv->error_cb && s5b->priv->error_content)
			s5b->priv->error_cb(s5b->priv->error_content);
		g_free(data);
		return;
	} else if(s5b->priv->rxlen - 5 <  s5b->priv->rxqueue[4] + 2) {
		purple_debug_info("jingle-s5b", 
			"reading %u bytes for DST.ADDR + port num (trying to read %u now)\n",
			s5b->priv->rxqueue[4] + 2,
			s5b->priv->rxqueue[4] + 2 - (s5b->priv->rxlen - 5));
		len = read(source, buffer,
			s5b->priv->rxqueue[4] + 2 - (s5b->priv->rxlen - 5));

		if(len < 0 && errno == EAGAIN)
			return;
		else if(len <= 0) {
			purple_input_remove(s5b->priv->watcher);
			s5b->priv->watcher = 0;
			close(source);
			if (s5b->priv->error_cb && s5b->priv->error_content)
				s5b->priv->error_cb(s5b->priv->error_content);
			g_free(data);
			return;
		}
		s5b->priv->rxqueue = 
			g_realloc(s5b->priv->rxqueue, len + s5b->priv->rxlen);
		memcpy(s5b->priv->rxqueue + s5b->priv->rxlen, buffer, len);
		s5b->priv->rxlen += len;
	}

	/* Have we not read all of DST.ADDR and the following 2-byte port number? */
	if(s5b->priv->rxlen - 5 < s5b->priv->rxqueue[4] + 2)
		return;

	purple_input_remove(s5b->priv->watcher);
	s5b->priv->watcher = 0;

	dstaddr = g_strdup_printf("%s%s@%s/%s%s@%s/%s", s5b->priv->sid, 
		js->user->node, js->user->domain, js->user->resource, 
		dstjid->node, dstjid->domain, dstjid->resource);

	g_free(dstjid);
		
	/* Per XEP-0065, the 'host' must be SHA1(SID + from JID + to JID) */
	hash = jabber_calculate_data_sha1sum(dstaddr, strlen(dstaddr));
	purple_debug_info("jingle-s5b", "dstaddr: %s\n", dstaddr);
	purple_debug_info("jingle-s5b", "expecting to receive hash %s\n", hash);
	
	if (strncmp(hash, s5b->priv->rxqueue + 5, 40) ||
			s5b->priv->rxqueue[45] != 0x00 || s5b->priv->rxqueue[46] != 0x00) {
		if (s5b->priv->rxqueue[45] != 0x00 || s5b->priv->rxqueue[46] != 0x00)
			purple_debug_error("jingle-s5b", 
				"Got SOCKS5 BS conn with the wrong DST.PORT "
				" (must be 0 - got[0x%x,0x%x]).\n",
			s5b->priv->rxqueue[45], s5b->priv->rxqueue[46]);
		else
			purple_debug_error("jingle-s5b", 
				"Got SOCKS5 BS conn with the wrong DST.ADDR"
				" (expected '%s' - got '%.40s').\n",
			hash, s5b->priv->rxqueue + 5);

		close(source);
		if (s5b->priv->error_cb && s5b->priv->error_content)
			s5b->priv->error_cb(s5b->priv->error_content);
		g_free(data);
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
		if (s5b->priv->error_cb && s5b->priv->error_content)
			s5b->priv->error_cb(s5b->priv->error_content);
		g_free(data);
		return;
	}
	s5b->priv->rxlen += len;

	if (s5b->priv->rxlen < s5b->priv->rxmaxlen) {
		if (s5b->priv->error_cb && s5b->priv->error_content)
			s5b->priv->error_cb(s5b->priv->error_content);
		g_free(data);
		return;
	}

	purple_input_remove(s5b->priv->watcher);
	s5b->priv->watcher = 0;

	/* If we sent a "Success", wait for a response, otherwise give up and cancel */
	if (s5b->priv->rxqueue[1] == 0x00) {
		s5b->priv->watcher = purple_input_add(source, PURPLE_INPUT_READ,
			jingle_s5b_send_read_again_cb, data);
		g_free(s5b->priv->rxqueue);
		s5b->priv->rxqueue = NULL;
		s5b->priv->rxlen = 0;
	} else {
		close(source);
		if (s5b->priv->error_cb && s5b->priv->error_content)
			s5b->priv->error_cb(s5b->priv->error_content);
		g_free(data);
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

	/* Try to read the SOCKS5 header */
	if(s5b->priv->rxlen < 2) {
		purple_debug_info("jingle-s5b", "reading those first two bytes\n");
		len = read(source, buffer, 2 - s5b->priv->rxlen);
		if(len < 0 && errno == EAGAIN)
			return;
		else if(len <= 0) {
			purple_input_remove(s5b->priv->watcher);
			s5b->priv->watcher = 0;
			close(source);
			if (s5b->priv->error_cb && s5b->priv->error_content)
				s5b->priv->error_cb(s5b->priv->error_content);
			g_free(data);
			return;
		}
		s5b->priv->rxqueue = 
			g_realloc(s5b->priv->rxqueue, len + s5b->priv->rxlen);
		memcpy(s5b->priv->rxqueue + s5b->priv->rxlen, buffer, len);
		s5b->priv->rxlen += len;
		return;
	} else if(s5b->priv->rxlen - 2 < s5b->priv->rxqueue[1]) {
		purple_debug_info("jingle-s5b", 
			"reading %u bytes for auth methods (trying to read %u now)\n",
			s5b->priv->rxqueue[1], s5b->priv->rxqueue[1] - (s5b->priv->rxlen - 2));
		len = read(source, buffer, s5b->priv->rxqueue[1] - (s5b->priv->rxlen - 2));

		if(len < 0 && errno == EAGAIN)
			return;
		else if(len <= 0) {
			purple_input_remove(s5b->priv->watcher);
			s5b->priv->watcher = 0;
			close(source);
			if (s5b->priv->error_cb && s5b->priv->error_content)
				s5b->priv->error_cb(s5b->priv->error_content);
			g_free(data);
			return;
		}
		s5b->priv->rxqueue = 
			g_realloc(s5b->priv->rxqueue, len + s5b->priv->rxlen);
		memcpy(s5b->priv->rxqueue + s5b->priv->rxlen, buffer, len);
		s5b->priv->rxlen += len;
	}

	/* Have we not read all the auth. method bytes? */
	if(s5b->priv->rxlen - 2 < s5b->priv->rxqueue[1])
		return;

	purple_input_remove(s5b->priv->watcher);
	s5b->priv->watcher = 0;

	purple_debug_info("jingle-s5b", "checking to make sure we're socks FIVE\n");

	if(s5b->priv->rxqueue[0] != 0x05) {
		purple_debug_error("jingle-s5b", "it's not socks FIVE, giving up\n");
		close(source);
		if (s5b->priv->error_cb && s5b->priv->error_content)
			s5b->priv->error_cb(s5b->priv->error_content);
		g_free(data);
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
			JingleS5BCandidate *cand =
				jingle_s5b_candidate_create(js, jid, local_ip, local_port,
					JINGLE_S5B_CANDIDATE_TYPE_DIRECT);
			JINGLE_S5B_GET_PRIVATE(s5b)->local_candidates =
				g_list_append(JINGLE_S5B_GET_PRIVATE(s5b)->local_candidates,
					cand);
		}

		if (!purple_strequal(local_ip, public_ip) && 
			!purple_strequal(public_ip, "0.0.0.0")) {
			JingleS5BCandidate *cand =
				jingle_s5b_candidate_create(js, jid, public_ip, local_port,
					JINGLE_S5B_CANDIDATE_TYPE_ASSISTED);
			JINGLE_S5B_GET_PRIVATE(s5b)->local_candidates =
				g_list_append(JINGLE_S5B_GET_PRIVATE(s5b)->local_candidates,
					cand);	
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
			JingleS5BCandidate *cand =
				jingle_s5b_candidate_create(js, sh->jid, sh->host, sh->port,
					JINGLE_S5B_CANDIDATE_TYPE_PROXY);
			JINGLE_S5B_GET_PRIVATE(s5b)->local_candidates =
				g_list_append(JINGLE_S5B_GET_PRIVATE(s5b)->local_candidates,
					cand);	
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
jingle_s5b_gather_candidates(JingleSession *session, JingleS5B *s5b)
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

static void jingle_s5b_attempt_connect_internal(gpointer data);

static gboolean
jingle_s5b_candidate_is_local(const JingleS5BCandidate *cand)
{
	return cand->type != JINGLE_S5B_CANDIDATE_TYPE_PROXY;
}

static gboolean
jingle_s5b_has_local_candidates(JabberStream *js, JingleS5B *s5b)
{
	GList *iter = s5b->priv->local_candidates;

	for (; iter; iter = g_list_next(iter)) {
		JingleS5BCandidate *cand =
			(JingleS5BCandidate *) iter->data;
		if (jingle_s5b_candidate_is_local(cand)) {
			return TRUE;
		}
	}
	return FALSE;
}

static gboolean
jingle_s5b_connect_timeout_cb(gpointer data)
{
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;
	
	purple_debug_info("jingle-s5b", "in jingle_s5b_connect_timeout_cb\n");

	/* cancel connect */
	if (s5b->priv->connect_data)
		purple_proxy_connect_cancel(s5b->priv->connect_data);
	s5b->priv->connect_data = NULL;

	s5b->priv->connect_timeout = 0;
	
	/* advance streamhost "counter" */
	if (s5b->priv->remaining_candidates) {
		s5b->priv->remaining_candidates = 
			g_list_next(s5b->priv->remaining_candidates);
		purple_debug_info("jingle-s5b", "trying next streamhost\n");
		/* if remaining_streamhost is NULL here, this call will result in a
		 streamhost error (and potentially fallback to IBB) */
		jingle_s5b_attempt_connect_internal(data);
	}
	
	return FALSE;
}


static void
jingle_s5b_connect_cb(gpointer data, gint source, const gchar *error_message)
{
	JingleS5BConnectData *cd = (JingleS5BConnectData *) data;
	JingleS5B *s5b = cd->s5b;
	JingleSession *session = cd->session;
	JabberIq *result = NULL;
    
	/* reset connect data (prevents the timeout callback to try and cancel the
	the connect if the connection was unsuccessful */
	s5b->priv->connect_data = NULL;
	
	/* cancel timeout if set */
	if (s5b->priv->connect_timeout) {
		purple_timeout_remove(s5b->priv->connect_timeout);
		s5b->priv->connect_timeout = 0;
	}

	if (s5b->priv->ppi) {
		purple_proxy_info_destroy(s5b->priv->ppi);
		s5b->priv->ppi = NULL;
	}
	
	if (source < 0) {
		/* failed to connect */
		/* trigger the a "timeout" to get to the next streamhost */	
		jingle_s5b_connect_timeout_cb(data);
		return;
	}
	
	purple_debug_info("jingle-s5b", "Successful in connecting!\n");
	
	s5b->priv->connect_data = NULL;
	s5b->priv->remote_fd = source;
	s5b->priv->is_connected_to_remote = TRUE;
	
	
	/* set the currently tried streamhost as the successfull one */
	s5b->priv->successful_remote_candidate =
		(JingleS5BCandidate *) s5b->priv->remaining_candidates->data;

	/* should stop trying to connect */
	jingle_s5b_stop_connection_attempts(s5b);
	
	/* should send transport-info with candidate-used */
	result = jingle_session_to_packet(session, JINGLE_TRANSPORT_INFO);
	jabber_iq_send(result);
	g_free(cd);
}
	
static void
jingle_s5b_connect_to_candidate(JingleS5BConnectData *data, 
	const JingleS5BCandidate *cand, gboolean is_local_proxy,
	void (*connect_cb)(gpointer, gint, const gchar *), GSourceFunc timeout_cb)
{
	JingleSession *session = data->session;
	JingleS5B *s5b = data->s5b;
	JabberStream *js = jingle_session_get_js(session);
	const gchar *who = jingle_session_get_remote_jid(session);
	JabberID *dstjid = jabber_id_new(who);
	gchar *dstaddr = NULL;
	gchar *hash = NULL;
	
	purple_debug_info("jingle-s5b", 
		"attempting to connect to streamhost: %s, port: %d\n",
		cand->host, cand->port);

	if (s5b->priv->ppi)
		purple_proxy_info_destroy(s5b->priv->ppi);
	s5b->priv->ppi = purple_proxy_info_new();
	purple_proxy_info_set_type(s5b->priv->ppi, PURPLE_PROXY_SOCKS5);
	purple_proxy_info_set_host(s5b->priv->ppi, cand->host);
	purple_proxy_info_set_port(s5b->priv->ppi, cand->port);
	
	if (is_local_proxy)
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
			connect_cb, data);
	g_free(hash);
	g_free(dstaddr);
	
	/* add timeout */
	/* we should add a longer timeout if the next streamhost candidate
	is a proxy and we have local candidates ourselves, to allow the other
	end a chance to connect to use before reverting to a proxy */
	s5b->priv->connect_timeout =
		purple_timeout_add(STREAMHOST_CONNECT_TIMEOUT_MILLIS, timeout_cb, data);

	g_free(dstjid);
}

static gboolean
jingle_s5b_proxy_timeout_cb(gpointer data)
{
	purple_debug_info("jingle-s5b", "timeout when connecting to proxy\n");
	return FALSE;
}

static void
jingle_s5b_proxy_activate_cb(JabberStream *js, const char *from,
	JabberIqType type, const char *id, xmlnode *packet, gpointer data)
{
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;

	if (type == JABBER_IQ_RESULT) {
		/* we are connected to the proxy, let's start */
		if (s5b->priv->connect_cb && s5b->priv->connect_content) {
			s5b->priv->connect_cb(s5b->priv->connect_content);
		}
	} else {
		/* error */
		purple_debug_error("jingle-s5b", 
			"got an error response to the proxy activation request\n");
	}
	
	g_free(data);
}

static void
jingle_s5b_proxy_connect_cb(gpointer data, gint source, const gchar *error_message)
{
	JabberIq *iq = NULL;
	xmlnode *query = NULL;
	xmlnode *activate = NULL;
	JingleSession *session = ((JingleS5BConnectData *) data)->session;
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;

	purple_debug_info("jingle-s5b", "connect to bytestreams proxy\n");
	/* cancel timeout if set */
	if (s5b->priv->connect_timeout) {
		purple_timeout_remove(s5b->priv->connect_timeout);
		s5b->priv->connect_timeout = 0;
	}

	if (source < 0) {
		/* failed to connect */
		purple_debug_error("jingle-s5b", "failed to connect to our own proxy\n");
		g_free(data);
		return;
	}
	
	purple_debug_info("jingle-s5b", "Successful in connecting to proxy!\n");
	s5b->priv->fd = source;
	
	/* active the streamhost */
	iq = jabber_iq_new_query(jingle_session_get_js(session), JABBER_IQ_SET, 
		"http://jabber.org/protocol/bytestreams");
	xmlnode_set_attrib(iq->node, "to", s5b->priv->accepted_candidate->jid);
	query = xmlnode_get_child(iq->node, "query");
	xmlnode_set_attrib(query, "sid", s5b->priv->sid);
	activate = xmlnode_new_child(query, "activate");
	xmlnode_insert_data(activate, jingle_session_get_remote_jid(session), -1);

	jabber_iq_set_callback(iq, jingle_s5b_proxy_activate_cb, data);
	jabber_iq_send(iq);
}

static void
jingle_s5b_connect_to_proxy(JingleSession *session, JingleS5B *s5b, 
	JingleS5BCandidate *cand)
{
	purple_debug_info("jingle-s5b", "in jingle_s5b_connect_to_proxy\n");
	if (cand) {
		JingleS5BConnectData *data = g_new0(JingleS5BConnectData, 1);

		data->session = session;
		data->s5b = s5b;
		jingle_s5b_connect_to_candidate(data, cand, TRUE, 
			jingle_s5b_proxy_connect_cb, jingle_s5b_proxy_timeout_cb);
	} else {
		purple_debug_error("jingle-s5b", 
			"did not find the local streamhost specified in the "
			"transport-accept message\n");
		/* TODO: should we offer fallback to IBB here? */
	}
}


static void
jingle_s5b_attempt_connect_internal(gpointer data)
{
	JingleSession *session = ((JingleS5BConnectData *) data)->session;
	JingleS5B *s5b = ((JingleS5BConnectData *) data)->s5b;
	
	if (s5b->priv->remaining_candidates) {
		JingleS5BCandidate *cand =
			(JingleS5BCandidate *) s5b->priv->remaining_candidates->data;

		jingle_s5b_connect_to_candidate((JingleS5BConnectData *) data, cand, 
			FALSE, jingle_s5b_connect_cb, jingle_s5b_connect_timeout_cb);
	} else {
		/* send candidate error */
		JabberIq *candidate_error =
			jingle_session_to_packet(session, JINGLE_TRANSPORT_INFO);
		xmlnode *jingle = 
		  xmlnode_get_child(candidate_error->node, "jingle");
		xmlnode *content = 
			xmlnode_get_child(jingle, "content");
		xmlnode *transport = xmlnode_get_child(content, "transport");

		xmlnode_insert_child(transport, xmlnode_new("candidate-error"));
		jabber_iq_send(candidate_error);
		
		/* if the other end could connect to us (they sent a "candidate-used")
		 we should use that */
		if (s5b->priv->accepted_candidate) {
			JingleS5BCandidate *cand = s5b->priv->accepted_candidate;

			jingle_s5b_take_command(s5b);

			if (!jingle_s5b_candidate_is_local(cand)) {
				purple_debug_info("jingle-ft",
					"the remote connected to us through a proxy, "
						"need to connect to the proxy\n");
				jingle_s5b_connect_to_proxy(session, s5b, cand);
			} else {
				/* start transfer */
				if (s5b->priv->connect_cb && s5b->priv->connect_content) {
					s5b->priv->connect_cb(s5b->priv->connect_content);
				} else {
					/* some error? */
				}
			}
		} else {
			/* signal to the content that S5B failed (from our side) */
			if (s5b->priv->failed_cb && s5b->priv->failed_content)
				s5b->priv->failed_cb(s5b->priv->failed_content);
		}
		g_free(data);
	}
}

void
jingle_s5b_attempt_connect(JingleSession *session, JingleS5B *s5b)
{
	JingleS5BConnectData *data = g_new0(JingleS5BConnectData, 1);
	
	data->session = session;
	data->s5b = s5b;
	s5b->priv->remaining_candidates = s5b->priv->remote_candidates;
	jingle_s5b_attempt_connect_internal(data);
}



static JingleS5BCandidate *
jingle_s5b_find_local_candidate(JingleS5B *s5b, const gchar *cid)
{
	const GList *iter = s5b->priv->local_candidates;

	for (; iter ; iter = g_list_next(iter)) {
		JingleS5BCandidate *cand =
			(JingleS5BCandidate *) iter->data;
		if (purple_strequal(cand->cid, cid)) {
			return cand;
		}
	}
	return NULL;
}


void
jingle_s5b_handle_transport_info(JingleS5B *s5b, JingleSession *session, 
	xmlnode *transport)
{
	xmlnode *candidate_used = xmlnode_get_child(transport, "candidate-used");
	xmlnode *candidate_error = xmlnode_get_child(transport, "candidate-error");
	
	if (candidate_used) {
		const gchar *cid = xmlnode_get_attrib(candidate_used, "cid");
		JingleS5BCandidate *cand =
			jingle_s5b_find_local_candidate(s5b, cid);
		JingleS5BCandidate *next_to_try =
			(JingleS5BCandidate *) s5b->priv->remaining_candidates->data;
		
		s5b->priv->accepted_candidate = cand;
		
		purple_debug_info("jingle-ft", "got candidate-used\n");
		/* stop connection attempts, unless there is remaining candidates of
		 higher priority */
		if (!next_to_try || next_to_try->priority < cand->priority) {
			/* we don't have any remaining candidates to try, send a
			 candidate-error */
			/* send candidate error */
			JabberIq *candidate_error =
				jingle_session_to_packet(session, JINGLE_TRANSPORT_INFO);
			xmlnode *jingle = 
				xmlnode_get_child(candidate_error->node, "jingle");
			xmlnode *content = 
				xmlnode_get_child(jingle, "content");
			xmlnode *transport = xmlnode_get_child(content, "transport");

			xmlnode_insert_child(transport, xmlnode_new("candidate-error"));
			jabber_iq_send(candidate_error);
			
			jingle_s5b_stop_connection_attempts(s5b);

			/* if we could not connect to the remote, or if we could and that
			 candidate has a lower priority, we have "won",
			 if the candidates have the same priority, we have won if we are
			 the initiator */
			if (!s5b->priv->remaining_candidates ||
				(s5b->priv->successful_remote_candidate
				 && s5b->priv->successful_remote_candidate->priority < cand->priority) ||
				(s5b->priv->successful_remote_candidate
				 && s5b->priv->successful_remote_candidate->priority == cand->priority
				 && jingle_session_is_initiator(session))) {
				/* we are now the "owner" of the bytestream */
				jingle_s5b_take_command(s5b);

				/* also when receiving a <streamhost-used/> we need to 
				check if that is not one of our local streamhosts, 
				in which case it is a proxy, and we should connect to that */
				if (!jingle_s5b_candidate_is_local(cand)) {
					purple_debug_info("jingle-ft",
						"got transport-accept on a proxy, "
						"need to connect to the proxy\n");
					jingle_s5b_connect_to_proxy(session, s5b, cand);
				} else {
					/* start transfer */
					if (s5b->priv->connect_cb && s5b->priv->connect_content) {
						s5b->priv->connect_cb(s5b->priv->connect_content);
					} else {
						/* some error? */
					}
				}
			} else {
				jingle_s5b_surrender(s5b);
				/* start the transfer */
				if (s5b->priv->connect_cb && s5b->priv->connect_content) {
						s5b->priv->connect_cb(s5b->priv->connect_content);
				} else {
					/* some error? */
				}
			}
		}
	} else if (candidate_error) {
		if (s5b->priv->failed_cb && !s5b->priv->successful_remote_candidate &&
			!s5b->priv->remaining_candidates) {
			s5b->priv->failed_cb(s5b->priv->failed_content);
		} else if (s5b->priv->successful_remote_candidate) {
			jingle_s5b_surrender(s5b);
			/* start the transfer */
			if (s5b->priv->connect_cb && s5b->priv->connect_content) {
				s5b->priv->connect_cb(s5b->priv->connect_content);
			} else {
				/* some error? */
			}
		}
	}
}

gboolean
jingle_s5b_has_remaining_remote_candidates(const JingleS5B *s5b)
{
	return s5b->priv->remaining_candidates != NULL;
}

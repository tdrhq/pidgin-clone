/**
 * @file servconn.c Server connection functions
 *
 * gaim
 *
 * Copyright (C) 2003-2004 Christian Hammond <chipx86@gnupdate.org>
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
#include "msn.h"
#include "servconn.h"
#include "error.h"

static void read_cb(gpointer data, gint source, GaimInputCondition cond);

typedef struct
{
	char *command;
	MsnMessage *msg;

} MsnQueueEntry;

static gboolean
process_message(MsnServConn *servconn, MsnMessage *msg)
{
	MsnServConnMsgCb cb;

	cb = g_hash_table_lookup(servconn->msg_types,
							 msn_message_get_content_type(msg));

	if (cb == NULL) {
		gaim_debug(GAIM_DEBUG_WARNING, "msn",
				   "Unhandled content-type '%s': %s\n",
				   msn_message_get_content_type(msg),
				   msn_message_get_body(msg));

		return FALSE;
	}

	cb(servconn, msg);

	return TRUE;
}

static gboolean
process_single_line(MsnServConn *servconn, char *str)
{
	MsnSession *session = servconn->session;
	MsnServConnCommandCb cb;
	GSList *l, *l_next = NULL;
	gboolean result;
	size_t param_count = 0;
	char *command, *param_start;
	char **params = NULL;

	command = str;

	/**
	 * See how many spaces we have in this.
	 */
	param_start = strchr(command, ' ');

	if (param_start != NULL) {
		params = g_strsplit(param_start + 1, " ", 0);

		for (param_count = 0; params[param_count] != NULL; param_count++)
			;

		*param_start = '\0';
	}

	cb = g_hash_table_lookup(servconn->commands, command);

	if (cb == NULL) {
		cb = g_hash_table_lookup(servconn->commands, "_unknown_");

		if (cb == NULL) {
			if (isdigit(*str))
				msn_error_handle(session, atoi(str));
			else
				gaim_debug(GAIM_DEBUG_WARNING, "msn",
					   "Unhandled command '%s'\n", str);

			if (params != NULL)
				g_strfreev(params);

			return TRUE;
		}
	}

	result = cb(servconn, command, (const char **)params, param_count);

	if (params != NULL)
		g_strfreev(params);

	if (g_list_find(session->servconns, servconn) == NULL)
		return result;

	/* Process all queued messages that are waiting on this command. */
	for (l = servconn->msg_queue; l != NULL; l = l_next) {
		MsnQueueEntry *entry = l->data;
		MsnMessage *msg;

		l_next = l->next;

		if (entry->command == NULL ||
			!g_ascii_strcasecmp(entry->command, command)) {

			MsnUser *sender;

			msg = entry->msg;

			msn_message_ref(msg);

			sender = msn_message_get_sender(msg);

			servconn->msg_passport = g_strdup(msn_user_get_passport(sender));
			servconn->msg_friendly = g_strdup(msn_user_get_name(sender));

			process_message(servconn, msg);

			g_free(servconn->msg_passport);
			g_free(servconn->msg_friendly);

			msn_servconn_unqueue_message(servconn, entry->msg);

			msn_message_destroy(msg);
			entry->msg = NULL;
		}
	}

	return result;
}

static gboolean
process_multi_line(MsnServConn *servconn, char *buffer)
{
	char msg_str[MSN_BUF_LEN];
	gboolean result = TRUE;

	if (servconn->multiline_type == MSN_MULTILINE_MSG) {
		MsnMessage *msg;
		size_t header_len;

		g_snprintf(msg_str, sizeof(msg_str),
				   "MSG %s %s %d\r\n",
				   servconn->msg_passport, servconn->msg_friendly,
				   servconn->multiline_len);

		header_len = strlen(msg_str);

		memcpy(msg_str + header_len, buffer, servconn->multiline_len);

		gaim_debug(GAIM_DEBUG_MISC, "msn",
				   "Message: {%s}\n", buffer);

		msg = msn_message_new_from_str(servconn->session, msg_str);

		result = process_message(servconn, msg);

		msn_message_destroy(msg);
	}
	else if (servconn->multiline_type == MSN_MULTILINE_IPG) {
		g_snprintf(msg_str, sizeof(msg_str),
				   "IPG %d\r\n%s",
				   servconn->multiline_len, buffer);

		gaim_debug(GAIM_DEBUG_MISC, "msn",
				   "Incoming Page: {%s}\n", buffer);
	}
	else if (servconn->multiline_type == MSN_MULTILINE_NOT) {
		g_snprintf(msg_str, sizeof(msg_str),
				   "NOT %d\r\n%s",
				   servconn->multiline_len, buffer);

		gaim_debug(GAIM_DEBUG_MISC, "msn",
				   "Notification: {%s}\n", buffer);
	}

	return result;
}

static void
connect_cb(gpointer data, gint source, GaimInputCondition cond)
{
	MsnServConn *servconn = data;

	gaim_debug_info("msn", "In servconn's connect_cb\n");

	servconn->fd = source;

	if (source > 0)
	{
		/* Someone wants to know we connected. */
		servconn->connect_cb(servconn);
		servconn->inpa = gaim_input_add(servconn->fd, GAIM_INPUT_READ,
										read_cb, data);
	}
	else
	{
		GaimConnection *gc = servconn->session->account->gc;
		gaim_connection_error(gc, _("Unable to connect."));
	}
}

MsnServConn *
msn_servconn_new(MsnSession *session)
{
	MsnServConn *servconn;

	g_return_val_if_fail(session != NULL, NULL);

	servconn = g_new0(MsnServConn, 1);

	servconn->session = session;

	if (session->http_method)
	{
		servconn->http_data = g_new0(MsnHttpMethodData, 1);
		servconn->http_data->virgin = TRUE;
	}

	servconn->commands = g_hash_table_new_full(g_str_hash, g_str_equal,
											   g_free, NULL);

	servconn->msg_types = g_hash_table_new_full(g_str_hash, g_str_equal,
												g_free, NULL);

	session->servconns = g_list_append(session->servconns, servconn);

	return servconn;
}

gboolean
msn_servconn_connect(MsnServConn *servconn, const char *host, int port)
{
	MsnSession *session;
	int i;

	g_return_val_if_fail(servconn != NULL, FALSE);
	g_return_val_if_fail(host     != NULL, FALSE);
	g_return_val_if_fail(port      > 0,    FALSE);

	session = servconn->session;

	if (servconn->connected)
		msn_servconn_disconnect(servconn);

	if (session->http_method)
	{
		servconn->http_data->gateway_ip = g_strdup(host);
		host = "gateway.messenger.hotmail.com";
		port = 80;
	}

	i = gaim_proxy_connect(session->account, host, port, connect_cb,
						   servconn);

	if (i == 0)
		servconn->connected = TRUE;

	return servconn->connected;
}

void
msn_servconn_disconnect(MsnServConn *servconn)
{
	MsnSession *session;

	g_return_if_fail(servconn != NULL);
	g_return_if_fail(servconn->connected);

	session = servconn->session;

	if (servconn->inpa)
		gaim_input_remove(servconn->inpa);

	close(servconn->fd);

	servconn->connected = FALSE;

	if (servconn->http_data != NULL)
	{
		if (servconn->http_data->session_id != NULL)
			g_free(servconn->http_data->session_id);

		if (servconn->http_data->old_gateway_ip != NULL)
			g_free(servconn->http_data->old_gateway_ip);

		if (servconn->http_data->gateway_ip != NULL)
			g_free(servconn->http_data->gateway_ip);

		if (servconn->http_data->timer)
			gaim_timeout_remove(servconn->http_data->timer);

		g_free(servconn->http_data);
	}

	if (servconn->rxqueue != NULL)
		g_free(servconn->rxqueue);

	while (servconn->txqueue != NULL) {
		g_free(servconn->txqueue->data);

		servconn->txqueue = g_slist_remove(servconn->txqueue,
										   servconn->txqueue->data);
	}

	while (servconn->msg_queue != NULL) {
		MsnQueueEntry *entry = servconn->msg_queue->data;

		msn_servconn_unqueue_message(servconn, entry->msg);
	}

	if (servconn->disconnect_cb)
		servconn->disconnect_cb(servconn);
}

void
msn_servconn_destroy(MsnServConn *servconn)
{
	MsnSession *session;

	g_return_if_fail(servconn != NULL);

	session = servconn->session;

	session->servconns = g_list_remove(session->servconns, servconn);

	if (servconn->connected)
		msn_servconn_disconnect(servconn);

#if 0
	/* shx: not used */
	if (servconn->host != NULL)
		g_free(servconn->host);
#endif

	g_free(servconn);
}

#if 0
/* shx: this isn't used */

void
msn_servconn_set_server(MsnServConn *servconn, const char *server, int port)
{
	g_return_if_fail(servconn != NULL);
	g_return_if_fail(server != NULL);
	g_return_if_fail(port > 0);

	if (servconn->host != NULL)
		g_free(servconn->host);

	servconn->host = g_strdup(server);
	servconn->port = port;
}

const char *
msn_servconn_get_server(const MsnServConn *servconn)
{
	g_return_val_if_fail(servconn != NULL, NULL);

	return servconn->host;
}

int
msn_servconn_get_port(const MsnServConn *servconn)
{
	g_return_val_if_fail(servconn != NULL, 0);

	return servconn->port;
}
#endif

void
msn_servconn_set_connect_cb(MsnServConn *servconn,
							gboolean (*connect_cb)(MsnServConn *servconn))
{
	g_return_if_fail(servconn != NULL);

	servconn->connect_cb = connect_cb;
}

void
msn_servconn_set_disconnect_cb(MsnServConn *servconn,
							   void (*disconnect_cb)(MsnServConn
													 *servconn))
{
	g_return_if_fail(servconn != NULL);

	servconn->disconnect_cb = disconnect_cb;
}

size_t
msn_servconn_write(MsnServConn *servconn, const char *buf, size_t size)
{
	g_return_val_if_fail(servconn != NULL, 0);

	gaim_debug(GAIM_DEBUG_MISC, "msn", "C: %s%s", buf,
			   (*(buf + size - 1) == '\n' ? "" : "\n"));

	if (servconn->session->http_method)
		return msn_http_servconn_write(servconn, buf, size,
									   servconn->http_data->server_type);
	else
		return write(servconn->fd, buf, size);
}

gboolean
msn_servconn_send_command(MsnServConn *servconn, const char *command,
						  const char *params)
{
	char buf[MSN_BUF_LEN];

	g_return_val_if_fail(servconn != NULL, FALSE);
	g_return_val_if_fail(command != NULL, FALSE);

	if (params == NULL)
		g_snprintf(buf, sizeof(buf), "%s %u\r\n", command,
				   servconn->session->trId++);
	else
		g_snprintf(buf, sizeof(buf), "%s %u %s\r\n",
				   command, servconn->session->trId++, params);

	return (msn_servconn_write(servconn, buf, strlen(buf)) > 0);
}

void
msn_servconn_queue_message(MsnServConn *servconn, const char *command,
						   MsnMessage *msg)
{
	MsnQueueEntry *entry;

	g_return_if_fail(servconn != NULL);
	g_return_if_fail(msg != NULL);

	entry          = g_new0(MsnQueueEntry, 1);
	entry->msg     = msg;
	entry->command = (command == NULL ? NULL : g_strdup(command));

	servconn->msg_queue = g_slist_append(servconn->msg_queue, entry);

	msn_message_ref(msg);
}

void
msn_servconn_unqueue_message(MsnServConn *servconn, MsnMessage *msg)
{
	MsnQueueEntry *entry = NULL;
	GSList *l;

	g_return_if_fail(servconn != NULL);
	g_return_if_fail(msg != NULL);

	for (l = servconn->msg_queue; l != NULL; l = l->next)
	{
		entry = l->data;

		if (entry->msg == msg)
			break;

		entry = NULL;
	}

	g_return_if_fail(entry != NULL);

	msn_message_unref(msg);

	servconn->msg_queue = g_slist_remove(servconn->msg_queue, entry);

	if (entry->command != NULL)
		g_free(entry->command);

	g_free(entry);
}

void
msn_servconn_register_command(MsnServConn *servconn, const char *command,
							  MsnServConnCommandCb cb)
{
	char *command_up;

	g_return_if_fail(servconn != NULL);
	g_return_if_fail(command != NULL);
	g_return_if_fail(cb != NULL);

	command_up = g_ascii_strup(command, -1);

	g_hash_table_insert(servconn->commands, command_up, cb);
}

void
msn_servconn_register_msg_type(MsnServConn *servconn,
							   const char *content_type,
							   MsnServConnMsgCb cb)
{
	g_return_if_fail(servconn != NULL);
	g_return_if_fail(content_type != NULL);
	g_return_if_fail(cb != NULL);

	g_hash_table_insert(servconn->msg_types, g_strdup(content_type), cb);
}

static void
failed_io(MsnServConn *servconn)
{
	GaimConnection *gc =
		gaim_account_get_connection(servconn->session->account);

	gaim_connection_error(gc, _("IO Error."));

	msn_servconn_disconnect(servconn);
}

static void
read_cb(gpointer data, gint source, GaimInputCondition cond)
{
	MsnServConn *servconn = (MsnServConn *)data;
	MsnSession *session = servconn->session;
	char buf[MSN_BUF_LEN];
	gboolean cont = TRUE;
	int len;

	len = read(servconn->fd, buf, sizeof(buf));

	if (len <= 0)
	{
		failed_io(servconn);

		return;
	}

	servconn->rxqueue = g_realloc(servconn->rxqueue, len + servconn->rxlen);
	memcpy(servconn->rxqueue + servconn->rxlen, buf, len);
	servconn->rxlen += len;

	if (session->http_method)
	{
		char *result_msg = NULL;
		size_t result_len = 0;
		gboolean error;
		char *tmp;

		tmp = g_strndup(servconn->rxqueue, servconn->rxlen);

		if (!msn_http_servconn_parse_data(servconn, tmp,
										  servconn->rxlen, &result_msg,
										  &result_len, &error))
		{
			g_free(tmp);
			return;
		}

		g_free(tmp);

		if (error)
		{
			gaim_connection_error(
				gaim_account_get_connection(session->account),
				_("Received HTTP error. Please report this."));

			return;
		}

		if (servconn->http_data->session_id != NULL &&
			!strcmp(servconn->http_data->session_id, "close"))
		{
			msn_servconn_destroy(servconn);

			return;
		}

#if 0
		if (strcmp(servconn->http_data->gateway_ip,
				   msn_servconn_get_server(servconn)) != 0)
		{
			int i;

			/* Evil hackery. I promise to remove it, even though I can't. */

			servconn->connected = FALSE;

			if (servconn->inpa)
				gaim_input_remove(servconn->inpa);

			close(servconn->fd);

			i = gaim_proxy_connect(session->account, servconn->host,
								   servconn->port, servconn->login_cb,
								   servconn);

			if (i == 0)
				servconn->connected = TRUE;
		}
#endif

		g_free(servconn->rxqueue);
		servconn->rxqueue = result_msg;
		servconn->rxlen   = result_len;
	}

	while (cont)
	{
		if (servconn->parsing_multiline)
		{
			char *msg;

			if (servconn->rxlen == 0)
				break;

			if (servconn->multiline_len > servconn->rxlen)
				break;

			msg = servconn->rxqueue;
			servconn->rxlen -= servconn->multiline_len;

			if (servconn->rxlen) {
				servconn->rxqueue = g_memdup(msg + servconn->multiline_len,
											 servconn->rxlen);
			}
			else {
				servconn->rxqueue = NULL;
				msg = g_realloc(msg, servconn->multiline_len + 1);
			}

			msg[servconn->multiline_len] = '\0';
			servconn->parsing_multiline = FALSE;

			process_multi_line(servconn, msg);

			if (g_list_find(session->servconns, servconn) != NULL) {
				servconn->multiline_len = 0;

				if (servconn->msg_passport != NULL)
					g_free(servconn->msg_passport);

				if (servconn->msg_friendly != NULL)
					g_free(servconn->msg_friendly);
			}
			else
				cont = 0;

			g_free(msg);
		}
		else {
			char *end = servconn->rxqueue;
			char *cmd;
			int cmdlen, i;

			if (!servconn->rxlen)
				return;

			for (i = 0; i < servconn->rxlen - 1; end++, i++) {
				if (*end == '\r' && end[1] == '\n')
					break;
			}

			if (i == servconn->rxlen - 1)
				return;

			cmdlen = end - servconn->rxqueue + 2;
			cmd = servconn->rxqueue;
			servconn->rxlen -= cmdlen;

			if (servconn->rxlen)
				servconn->rxqueue = g_memdup(cmd + cmdlen, servconn->rxlen);
			else {
				servconn->rxqueue = NULL;
				cmd = g_realloc(cmd, cmdlen + 1);
			}

			cmd[cmdlen] = '\0';

			gaim_debug(GAIM_DEBUG_MISC, "msn", "S: %s", cmd);

			g_strchomp(cmd);

			cont = process_single_line(servconn, cmd);

			g_free(cmd);
		}
	}
}

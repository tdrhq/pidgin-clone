/**
 * @file notification.c Notification server functions
 *
 * gaim
 *
 * Copyright (C) 2003 Christian Hammond <chipx86@gnupdate.org>
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
#include "notification.h"
#include "state.h"
#include "error.h"
#include "utils.h"

typedef struct
{
	GaimConnection *gc;
	MsnUser *user;

} MsnPermitAdd;

static GHashTable *notification_commands  = NULL;
static GHashTable *notification_msg_types = NULL;

/**************************************************************************
 * Utility functions
 **************************************************************************/
static gboolean
add_buddy(MsnServConn *servconn, MsnUser *user)
{
	MsnSession *session = servconn->session;
	GaimAccount *account = session->account;
	GaimConnection *gc = gaim_account_get_connection(account);
	GaimBuddy *b;
	MsnGroup *group = NULL;
	GaimGroup *g = NULL;
	GList *l, *l2;
	GSList *sl;
	GSList *buddies;

	buddies = gaim_find_buddies(account, msn_user_get_passport(user));

	for (l = msn_user_get_group_ids(user); l != NULL; l = l->next)
	{
		int group_id = GPOINTER_TO_INT(l->data);

		if (group_id > -1)
			group = msn_groups_find_with_id(session->groups, group_id);

		if (group == NULL)
		{
			gaim_debug(GAIM_DEBUG_WARNING, "msn",
					   "Group ID %d for user %s was not defined.\n",
					   group_id, msn_user_get_passport(user));

			/* Find a group that we can stick this guy into. Lamer. */
			l2 = msn_groups_get_list(session->groups);

			if (l2 != NULL)
			{
				group = l2->data;

				msn_user_add_group_id(user, msn_group_get_id(group));
			}
		}

		if (group == NULL ||
			(g = gaim_find_group(msn_group_get_name(group))) == NULL)
		{
			gaim_debug(GAIM_DEBUG_ERROR, "msn",
					   "Group '%s' appears in server-side "
					   "buddy list, but not here!",
					   msn_group_get_name(group));
		}

		if (group != NULL)
			msn_group_add_user(group, user);

		b = NULL;

		for (sl = buddies; sl != NULL; sl = sl->next)
		{
			b = (GaimBuddy *)sl->data;

			if (gaim_find_buddys_group(b) == g)
				break;

			b = NULL;
		}

		if (b == NULL)
		{
			const char *passport, *friendly;

			passport = msn_user_get_passport(user);

			b = gaim_buddy_new(account, passport, NULL);

			b->proto_data = user;

			gaim_blist_add_buddy(b, NULL, g, NULL);

			if ((friendly = msn_user_get_name(user)) != NULL)
				serv_got_alias(gc, passport, friendly);
		}
		else
			b->proto_data = user;
	}

	g_slist_free(buddies);

	serv_got_alias(gc, (char *)msn_user_get_passport(user),
				   (char *)msn_user_get_name(user));

	return TRUE;
}

static size_t
msn_ssl_read(GaimSslConnection *gsc, char **dest_buffer)
{
	size_t size = 0, s;
	char *buffer = NULL;
	char temp_buf[4096];

	while ((s = gaim_ssl_read(gsc, temp_buf, sizeof(temp_buf))) > 0)
	{
		char *new_buffer = g_new(char, size + s + 1);

		if (buffer != NULL)
		{
			strncpy(new_buffer, buffer, size);

			g_free(buffer);
		}

		buffer = new_buffer;

		strncpy(buffer + size, temp_buf, s);

		buffer[size + s] = '\0';

		size += s;
	}

	*dest_buffer = buffer;

	return size;
}

/**************************************************************************
 * Callbacks
 **************************************************************************/
static void
msn_accept_add_cb(MsnPermitAdd *pa)
{
	if (g_list_find(gaim_connections_get_all(), pa->gc) != NULL) {
		MsnSession *session = pa->gc->proto_data;
		char outparams[MSN_BUF_LEN];

		g_snprintf(outparams, sizeof(outparams), "AL %s %s",
				   msn_user_get_passport(pa->user),
				   msn_url_encode(msn_user_get_name(pa->user)));

		if (msn_servconn_send_command(session->notification_conn,
									  "ADD", outparams) <= 0) {
			gaim_connection_error(pa->gc, _("Write error"));
			return;
		}

		gaim_privacy_permit_add(pa->gc->account,
								msn_user_get_passport(pa->user), TRUE);
		show_got_added(pa->gc, NULL, msn_user_get_passport(pa->user),
					   msn_user_get_name(pa->user), NULL);
	}

	msn_user_destroy(pa->user);
	g_free(pa);
}

static void
msn_cancel_add_cb(MsnPermitAdd *pa)
{
	if (g_list_find(gaim_connections_get_all(), pa->gc) != NULL) {
		MsnSession *session = pa->gc->proto_data;
		char outparams[MSN_BUF_LEN];

		g_snprintf(outparams, sizeof(outparams), "BL %s %s",
				   msn_user_get_passport(pa->user),
				   msn_url_encode(msn_user_get_name(pa->user)));

		if (msn_servconn_send_command(session->notification_conn,
									  "ADD", outparams) <= 0) {
			gaim_connection_error(pa->gc, _("Write error"));
			return;
		}

		gaim_privacy_deny_add(pa->gc->account,
							  msn_user_get_passport(pa->user), TRUE);
	}

	msn_user_destroy(pa->user);
	g_free(pa);
}

/**************************************************************************
 * Catch-all commands
 **************************************************************************/
static gboolean
blank_cmd(MsnServConn *servconn, const char *command, const char **params,
		  size_t param_count)
{
	return TRUE;
}

static gboolean
unknown_cmd(MsnServConn *servconn, const char *command, const char **params,
			size_t param_count)
{
	char buf[MSN_BUF_LEN];

	if (isdigit(*command)) {
		int errnum = atoi(command);

		if (errnum == 225) {
			/*
			 * Ignore this. It happens as a result of moving a buddy from
			 * one group that isn't on the server to another that is.
			 * The user doesn't care if the old group was there or not.
			 */
			return TRUE;
		}

		g_snprintf(buf, sizeof(buf), _("MSN Error: %s\n"),
				   msn_error_get_text(errnum));
	}
	else {
		g_snprintf(buf, sizeof(buf), "MSN Error: Unable to parse message\n");
	}

	gaim_notify_error(servconn->session->account->gc, NULL, buf, NULL);

	return TRUE;
}


/**************************************************************************
 * Login
 **************************************************************************/


static gboolean
cvr_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	GaimAccount *account = servconn->session->account;
	GaimConnection *gc = gaim_account_get_connection(account);
	char outparams[MSN_BUF_LEN];

	g_snprintf(outparams, sizeof(outparams),
			   "TWN I %s", gaim_account_get_username(account));

	if (!msn_servconn_send_command(servconn, "USR", outparams))
	{
		gaim_connection_error(gc, _("Unable to request USR\n"));

		return FALSE;
	}

	return TRUE;
}

static gboolean
inf_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	GaimAccount *account = servconn->session->account;
	GaimConnection *gc = gaim_account_get_connection(account);
	char outparams[MSN_BUF_LEN];

	if (strcmp(params[1], "MD5")) {
		gaim_connection_error(gc, _("Unable to login using MD5"));

	return FALSE;
	}

	g_snprintf(outparams, sizeof(outparams), "MD5 I %s",
			   gaim_account_get_username(account));

	if (!msn_servconn_send_command(servconn, "USR", outparams)) {
		gaim_connection_error(gc, _("Unable to send USR"));

		return FALSE;
	}

	gaim_connection_update_progress(gc, _("Requesting to send password"),
									5, MSN_CONNECT_STEPS);

	return TRUE;
}

static void
login_connect_cb(gpointer data, GaimSslConnection *gsc,
				 GaimInputCondition cond)
{
	MsnServConn *servconn = (MsnServConn *)data;
	MsnSession *session = servconn->session;
	GaimConnection *gc = gaim_account_get_connection(session->account);
	char *username, *password;
	char *request_str;
	char *buffer = NULL;
	size_t s;

	username =
		g_strdup(msn_url_encode(gaim_account_get_username(session->account)));
	password =
		g_strdup(msn_url_encode(gaim_account_get_password(session->account)));

	request_str = g_strdup_printf(
		"GET %s HTTP/1.1\r\n"
		"Authorization: Passport1.4 OrgVerb=GET,OrgURL=%s,sign-in=%s,pwd=%s,"
		"lc=%s,id=%s,tw=%s,fs=%s,ct=%s,kpp=%s,kv=%s,ver=%s,tpf=%s\r\n"
		"User-Agent: MSMSGS\r\n"
		"Host: %s\r\n"
		"Connection: Keep-Alive\r\n"
		"Cache-Control: no-cache\r\n"
		"\r\n",
		session->ssl_login_path,
		(char *)g_hash_table_lookup(session->ssl_challenge_data, "ru"),
		username, password,
		(char *)g_hash_table_lookup(session->ssl_challenge_data, "lc"),
		(char *)g_hash_table_lookup(session->ssl_challenge_data, "id"),
		(char *)g_hash_table_lookup(session->ssl_challenge_data, "tw"),
		(char *)g_hash_table_lookup(session->ssl_challenge_data, "fs"),
		(char *)g_hash_table_lookup(session->ssl_challenge_data, "ct"),
		(char *)g_hash_table_lookup(session->ssl_challenge_data, "kpp"),
		(char *)g_hash_table_lookup(session->ssl_challenge_data, "kv"),
		(char *)g_hash_table_lookup(session->ssl_challenge_data, "ver"),
		(char *)g_hash_table_lookup(session->ssl_challenge_data, "tpf"),
		session->ssl_login_host);

	gaim_debug(GAIM_DEBUG_MISC, "msn", "Sending: {%s}\n", request_str);

	g_free(username);
	g_free(password);

	if ((s = gaim_ssl_write(gsc, request_str, strlen(request_str))) <= 0)
	{
		g_free(request_str);
		gaim_connection_error(gc, _("Unable to write to MSN Nexus server."));

		return;
	}

	g_free(request_str);

	if ((s = msn_ssl_read(gsc, &buffer)) <= 0)
	{
		gaim_connection_error(gc, _("Unable to read from MSN Nexus server."));

		if (buffer != NULL)
			g_free(buffer);

		return;
	}

	gaim_ssl_close(gsc);

	gaim_debug(GAIM_DEBUG_MISC, "msn", "ssl buffer: {%s}", buffer);

	if (strstr(buffer, "HTTP/1.1 302") != NULL)
	{
		/* Redirect. */
		char *location, *c;

		if ((location = strstr(buffer, "Location: ")) == NULL)
		{
			gaim_connection_error(gc,
				_("MSN Nexus server returned invalid redirect information."));

			g_free(buffer);

			return;
		}

		location = strchr(location, ' ') + 1;

		if ((c = strchr(location, '\r')) != NULL)
			*c = '\0';

		/* Skip the http:// */
		if ((c = strchr(location, '/')) != NULL)
			location = c + 2;

		if ((c = strchr(location, '/')) != NULL)
		{
			session->ssl_login_path = g_strdup(c);

			*c = '\0';
		}

		session->ssl_login_host = g_strdup(location);

		session->ssl_conn = gaim_ssl_connect(session->account,
											 session->ssl_login_host,
											 GAIM_SSL_DEFAULT_PORT,
											 login_connect_cb, servconn);
	}
	else if (strstr(buffer, "HTTP/1.1 401 Unauthorized") != NULL)
	{
		char *error;

		if ((error = strstr(buffer, "WWW-Authenticate")) != NULL)
		{
			if ((error = strstr(buffer, "cbtxt=")) != NULL)
				error += strlen("cbtxt=");

			error = msn_url_decode(error);
		}


		if (error == NULL)
		{
			gaim_connection_error(gc,
				_("Unknown error when attempting to authorize with "
				  "MSN login server."));
		}
		else
			gaim_connection_error(gc, error);
	}
	else
	{
		char *base, *c;
		char outparams[MSN_BUF_LEN];

		g_free(session->ssl_login_host);
		g_free(session->ssl_login_path);
		g_hash_table_destroy(session->ssl_challenge_data);

		session->ssl_login_host = NULL;
		session->ssl_login_path = NULL;
		session->ssl_challenge_data = NULL;

#if 0
		/* All your base are belong to us. */
		base = buffer;

		/* For great cookie! */
		while ((base = strstr(base, "Set-Cookie: ")) != NULL)
		{
			base += strlen("Set-Cookie: ");

			c = strchr(base, ';');

			session->login_cookies =
				g_list_append(session->login_cookies,
							  g_strndup(base, c - base));
		}
#endif

		if ((base = strstr(buffer, "Authentication-Info: ")) == NULL)
		{
			gaim_debug(GAIM_DEBUG_ERROR, "msn",
					   "Authentication information was not found. This did "
					   "not just happen, but if it did, you're screwed. "
					   "Report this.\n");

			return;
		}

		base  = strstr(base, "from-PP='");
		base += strlen("from-PP='");
		c     = strchr(base, '\'');

		session->ssl_login_params = g_strndup(base, c - base);

		g_snprintf(outparams, sizeof(outparams),
				   "TWN S %s", session->ssl_login_params);

		g_free(session->ssl_login_params);
		session->ssl_login_params = NULL;

		if (!msn_servconn_send_command(session->notification_conn, "USR",
									   outparams))
		{
			gaim_connection_error(gc, _("Unable to request USR\n"));
		}
	}

	g_free(buffer);
}

static void
nexus_connect_cb(gpointer data, GaimSslConnection *gsc,
				 GaimInputCondition cond)
{
	MsnServConn *servconn = (MsnServConn *)data;
	MsnSession *session = servconn->session;
	GaimConnection *gc = gaim_account_get_connection(session->account);
	char *request_str;
	char *da_login;
	char *base, *c;
	char *buffer = NULL;
	size_t s;

	request_str = g_strdup_printf("GET /rdr/pprdr.asp\r\n\r\n");

	if ((s = gaim_ssl_write(gsc, request_str, strlen(request_str))) <= 0)
	{
		g_free(request_str);
		gaim_connection_error(gc, _("Unable to write to MSN Nexus server."));
		return;
	}

	g_free(request_str);

	if (session->ssl_url != NULL)
	{
		g_free(session->ssl_url);
		session->ssl_url = NULL;
	}

	/* Get the PassportURLs line. */
	if ((s = msn_ssl_read(gsc, &buffer)) <= 0)
	{
		gaim_connection_error(gc, _("Unable to read from MSN Nexus server."));

		if (buffer != NULL)
			g_free(buffer);

		return;
	}

	if ((base = strstr(buffer, "PassportURLs")) == NULL)
	{
		gaim_connection_error(gc,
				_("MSN Nexus server returned invalid information."));

		g_free(buffer);

		return;
	}

	if ((da_login = strstr(base, "DALogin=")) != NULL)
	{
		if ((da_login = strchr(da_login, '=')) != NULL)
			da_login++;

		if ((c = strchr(da_login, ',')) != NULL)
			*c = '\0';

		if ((c = strchr(da_login, '/')) != NULL)
		{
			session->ssl_login_path = g_strdup(c);

			*c = '\0';
		}

		session->ssl_login_host = g_strdup(da_login);
	}

	g_free(buffer);

	gaim_ssl_close(gsc);

	/* Now begin the connection to the login server. */
	session->ssl_conn = gaim_ssl_connect(session->account,
										 session->ssl_login_host,
										 GAIM_SSL_DEFAULT_PORT,
										 login_connect_cb, servconn);
}

static gboolean
usr_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	GaimAccount *account = session->account;
	GaimConnection *gc = gaim_account_get_connection(account);
	char outparams[MSN_BUF_LEN];

	/*
	 * We're either getting the passport connect info (if we're on
	 * MSNP8 or higher), or a challenge request (MSNP7 and lower).
	 *
	 * Let's find out.
	 */
	if (!g_ascii_strcasecmp(params[1], "OK"))
	{
		const char *friendly = msn_url_decode(params[3]);

		/* OK */

		gaim_connection_set_display_name(gc, friendly);

		session->syncing_lists = TRUE;

		if (!msn_servconn_send_command(servconn, "SYN", "0"))
		{
			gaim_connection_error(gc, _("Unable to write"));

			return FALSE;
		}

		gaim_connection_update_progress(gc, _("Retrieving buddy list"),
										7, MSN_CONNECT_STEPS);
	}
	else if (!g_ascii_strcasecmp(params[1], "TWN"))
	{
		/* Passport authentication */
		char *challenge_data;
		char *key, *value = NULL;
		char *c;

		if (session->ssl_challenge_data != NULL)
			g_hash_table_destroy(session->ssl_challenge_data);

		session->ssl_challenge_data =
			g_hash_table_new_full(g_str_hash, g_str_equal,
								  g_free, g_free);

		/* Parse the challenge data. */

		challenge_data = g_strdup(params[3]);

		for (c = challenge_data, key = challenge_data; *c != '\0'; c++)
		{
			if (*c == '=')
			{
				*c = '\0';

				value = c + 1;
			}
			else if (*c == ',')
			{
				*c = '\0';

				g_hash_table_insert(session->ssl_challenge_data,
									g_strdup(key), g_strdup(value));

				key = c + 1;
			}
		}

		g_free(challenge_data);

#if 0
		passport_str = g_strdup(msn_url_decode(params[3]));

		for (c = passport_str; *c != '\0'; c++)
		{
			if (*c == ',')
				*c = '&';
		}

		session->ssl_url = passport_str;
#endif

		session->ssl_conn = gaim_ssl_connect(session->account,
											 "nexus.passport.com",
											 GAIM_SSL_DEFAULT_PORT,
											 nexus_connect_cb, servconn);

		if (session->ssl_conn == NULL)
		{
			gaim_connection_error(gc,
				_("Unable to connect to passport server"));

			return FALSE;
		}

		gaim_connection_update_progress(gc, _("Password sent"),
										6, MSN_CONNECT_STEPS);
	}
	else if (!g_ascii_strcasecmp(params[1], "MD5"))
	{
		/* Challenge */
		const char *challenge = params[3];
		char buf[MSN_BUF_LEN];
		md5_state_t st;
		md5_byte_t di[16];
		int i;

		g_snprintf(buf, sizeof(buf), "%s%s", challenge,
				   gaim_account_get_password(account));

		md5_init(&st);
		md5_append(&st, (const md5_byte_t *)buf, strlen(buf));
		md5_finish(&st, di);

		g_snprintf(outparams, sizeof(outparams), "MD5 S ");

		for (i = 0; i < 16; i++) {
			g_snprintf(buf, sizeof(buf), "%02x", di[i]);
			strcat(outparams, buf);
		}

		if (!msn_servconn_send_command(servconn, "USR", outparams)) {
			gaim_connection_error(gc, _("Unable to send password"));

			return FALSE;
		}

		gaim_connection_update_progress(gc, _("Password sent"),
										6, MSN_CONNECT_STEPS);
	}

	return TRUE;
}

static gboolean
ver_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	GaimAccount *account = session->account;
	GaimConnection *gc = gaim_account_get_connection(account);
	gboolean protocol_supported = FALSE;
	char outparams[MSN_BUF_LEN];
	char proto_str[8];
	size_t i;

	g_snprintf(proto_str, sizeof(proto_str), "MSNP%d", session->protocol_ver);

	for (i = 1; i < param_count; i++)
	{
		if (!strcmp(params[i], proto_str))
		{
			protocol_supported = TRUE;
			break;
		}
	}

	if (!protocol_supported) {
		gaim_connection_error(gc, _("Protocol not supported"));

		return FALSE;
	}

	if (session->protocol_ver >= 8)
	{
		g_snprintf(outparams, sizeof(outparams),
				   "0x0409 winnt 5.1 i386 MSNMSGR 6.0.0602 MSMSGS %s",
				   gaim_account_get_username(account));

		if (!msn_servconn_send_command(servconn, "CVR", outparams))
		{
			gaim_connection_error(gc, _("Unable to request CVR\n"));

			return FALSE;
		}
	}
	else
	{
		if (!msn_servconn_send_command(servconn, "INF", NULL))
		{
			gaim_connection_error(gc, _("Unable to request INF\n"));

			return FALSE;
		}
	}

	return TRUE;
}

/**************************************************************************
 * Log out
 **************************************************************************/
static gboolean
out_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	GaimConnection *gc = servconn->session->account->gc;

	if (!g_ascii_strcasecmp(params[0], "OTH")) {
		gaim_connection_error(gc,
							_("You have been disconnected. You have "
							  "signed on from another location."));
	}
	else if (!g_ascii_strcasecmp(params[0], "SSD")) {
		gaim_connection_error(gc,
							_("You have been disconnected. The MSN servers "
							  "are going down temporarily."));
	}

	return FALSE;
}

/**************************************************************************
 * Messages
 **************************************************************************/
static gboolean
msg_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	gaim_debug(GAIM_DEBUG_INFO, "msn", "Found message. Parsing.\n");

	servconn->parsing_multiline = TRUE;
	servconn->multiline_type    = MSN_MULTILINE_MSG;
	servconn->multiline_len     = atoi(params[2]);

	servconn->msg_passport = g_strdup(params[0]);
	servconn->msg_friendly = g_strdup(params[1]);

	return TRUE;
}

/**************************************************************************
 * Challenges
 **************************************************************************/
static gboolean
chl_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	GaimConnection *gc = session->account->gc;
	char buf[MSN_BUF_LEN];
	char buf2[3];
	const char *challenge_resp;
	md5_state_t st;
	md5_byte_t di[16];
	int i;

	md5_init(&st);
	md5_append(&st, (const md5_byte_t *)params[1], strlen(params[1]));

	if (session->protocol_ver >= 8)
	{
		challenge_resp = "VT6PX?UQTM4WM%YR";
	}
	else
	{
		challenge_resp = "Q1P7W2E4J9R8U3S5";
	}

	md5_append(&st, (const md5_byte_t *)challenge_resp,
			   strlen(challenge_resp));
	md5_finish(&st, di);

	g_snprintf(buf, sizeof(buf),
			   "QRY %u %s 32\r\n",
			   servconn->session->trId++,
			   (session->protocol_ver >= 8
				? "PROD0038W!61ZTF9"
				: "msmsgs@msnmsgr.com"));

	for (i = 0; i < 16; i++) {
		g_snprintf(buf2, sizeof(buf2), "%02x", di[i]);
		strcat(buf, buf2);
	}

	if (msn_servconn_write(servconn, buf, strlen(buf)) <= 0) {
		gaim_connection_error(gc, _("Unable to write to server"));
	}

	return TRUE;
}

/**************************************************************************
 * Buddy Lists
 **************************************************************************/
static gboolean
add_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	MsnUser *user;
	GaimAccount *account = session->account;
	GaimConnection *gc = gaim_account_get_connection(account);
	MsnPermitAdd *pa;
	GSList *sl;
	const char *list, *passport, *group_id = NULL;
	char *friend;
	char msg[MSN_BUF_LEN];

	list     = params[1];
	passport = params[3];
	friend   = msn_url_decode(params[4]);

	if (param_count >= 6)
		group_id = params[5];

	if (!g_ascii_strcasecmp(list, "FL")) {
		user = msn_user_new(session, passport, NULL);

		if (group_id != NULL)
			msn_user_add_group_id(user, atoi(group_id));

		add_buddy(servconn, user);

		return TRUE;
	}
	else if (g_ascii_strcasecmp(list, "RL"))
		return TRUE;

	for (sl = gc->account->permit; sl != NULL; sl = sl->next) {
		if (!gaim_utf8_strcasecmp(sl->data, passport))
			return TRUE;
	}

	user = msn_user_new(session, passport, friend);

	pa       = g_new0(MsnPermitAdd, 1);
	pa->user = user;
	pa->gc   = gc;

	g_snprintf(msg, sizeof(msg),
			   _("The user %s (%s) wants to add %s to his or her buddy list."),
			   passport, friend, gaim_account_get_username(account));

	gaim_request_action(gc, NULL, msg, NULL, 0, pa, 2,
						_("Authorize"), G_CALLBACK(msn_accept_add_cb),
						_("Deny"), G_CALLBACK(msn_cancel_add_cb));

	return TRUE;
}

static gboolean
adg_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnGroup *group;
	MsnSession *session = servconn->session;
	gint group_id;
	char *group_name;

	group_id = atoi(params[3]);

	group_name = msn_url_decode(params[2]);

	group = msn_group_new(session, group_id, group_name);

	msn_groups_add(session->groups, group);

	return TRUE;
}

static gboolean
blp_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	GaimConnection *gc = servconn->session->account->gc;
	const char *list_name;

	if (servconn->session->protocol_ver >= 8)
		list_name = params[0];
	else
		list_name = params[2];

	if (!g_ascii_strcasecmp(list_name, "AL")) {
		/*
		 * If the current setting is AL, messages from users who
		 * are not in BL will be delivered.
		 *
		 * In other words, deny some.
		 */
		gc->account->perm_deny = DENY_SOME;
	}
	else {
		/* If the current setting is BL, only messages from people
		 * who are in the AL will be delivered.
		 *
		 * In other words, permit some.
		 */
		gc->account->perm_deny = PERMIT_SOME;
	}

	return TRUE;
}

static gboolean
bpr_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	GaimConnection *gc = session->account->gc;
	const char *passport, *type, *value;
	GaimBuddy *b;
	MsnUser *user;

	if (param_count == 4)
	{
		passport = params[1];
		type     = params[2];
		value    = params[3];
	}
	else if (param_count == 2)
	{
		passport = msn_user_get_passport(session->last_user_added);
		type     = params[0];
		value    = params[1];
	}
	else
		return TRUE;

	user = msn_users_find_with_passport(session->users, passport);

	if (value != NULL) {
		if (!strcmp(type, "MOB")) {
			if (!strcmp(value, "Y")) {
				user->mobile = TRUE;

				if ((b = gaim_find_buddy(gc->account, passport)) != NULL) {
					if (GAIM_BUDDY_IS_ONLINE(b)) {
						serv_got_update(gc, (char *)passport,
										1, 0, 0, 0, b->uc);
					}
				}
			}
		}
		else if (!strcmp(type, "PHH"))
			msn_user_set_home_phone(user, msn_url_decode(value));
		else if (!strcmp(type, "PHW"))
			msn_user_set_work_phone(user, msn_url_decode(value));
		else if (!strcmp(type, "PHM"))
			msn_user_set_mobile_phone(user, msn_url_decode(value));
	}

	return TRUE;
}

static gboolean
fln_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	GaimConnection *gc = servconn->session->account->gc;

	serv_got_update(gc, (char *)params[0], 0, 0, 0, 0, 0);

	return TRUE;
}

static gboolean
iln_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	GaimConnection *gc = session->account->gc;
	MsnUser *user;
	MsnObject *msnobj;
	int status = 0;
	const char *state, *passport, *friend;
	GaimBuddy *b;

	state    = params[1];
	passport = params[2];
	friend   = msn_url_decode(params[3]);

	user = msn_users_find_with_passport(session->users, passport);

	serv_got_alias(gc, (char *)passport, (char *)friend);

	msn_user_set_name(user, friend);

	if (session->protocol_ver >= 9 && param_count == 6)
	{
		msnobj = msn_object_new_from_string(msn_url_decode(params[5]));
		msn_user_set_object(user, msnobj);
	}

	if ((b = gaim_find_buddy(gc->account, passport)) != NULL)
		status |= ((((b->uc) >> 1) & 0xF0) << 1);

	if (!g_ascii_strcasecmp(state, "BSY"))
		status |= UC_UNAVAILABLE | (MSN_BUSY << 1);
	else if (!g_ascii_strcasecmp(state, "IDL"))
		status |= UC_UNAVAILABLE | (MSN_IDLE << 1);
	else if (!g_ascii_strcasecmp(state, "BRB"))
		status |= UC_UNAVAILABLE | (MSN_BRB << 1);
	else if (!g_ascii_strcasecmp(state, "AWY"))
		status |= UC_UNAVAILABLE | (MSN_AWAY << 1);
	else if (!g_ascii_strcasecmp(state, "PHN"))
		status |= UC_UNAVAILABLE | (MSN_PHONE << 1);
	else if (!g_ascii_strcasecmp(state, "LUN"))
		status |= UC_UNAVAILABLE | (MSN_LUNCH << 1);

	serv_got_update(gc, (char *)passport, 1, 0, 0, 0, status);

	return TRUE;
}

static gboolean
ipg_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	/* GaimConnection *gc = servconn->session->account->gc; */

	servconn->parsing_multiline = TRUE;
	servconn->multiline_type    = MSN_MULTILINE_IPG;
	servconn->multiline_len     = atoi(params[0]);

	servconn->msg_passport = NULL;
	servconn->msg_friendly = NULL;

	return TRUE;
}

static gboolean
lsg_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	MsnGroup *group;
	GaimGroup *g;
	const char *name;
	int num_groups, group_id;

	if (session->protocol_ver >= 8)
	{
		group_id = atoi(params[0]);
		name = msn_url_decode(params[1]);
	}
	else
	{
		num_groups = atoi(params[3]);
		group_id   = atoi(params[4]);
		name       = msn_url_decode(params[5]);

		if (num_groups == 0)
			return TRUE;

		if (!strcmp(name, "~"))
			name = _("Buddies");
	}

	group = msn_group_new(session, group_id, name);

	msn_groups_add(session->groups, group);

	if ((g = gaim_find_group(name)) == NULL) {
		g = gaim_group_new(name);
		gaim_blist_add_group(g, NULL);
	}

	return TRUE;
}

static gboolean
lst_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	GaimAccount *account = session->account;
	GaimConnection *gc = gaim_account_get_connection(account);
	const char *passport = NULL;
	const char *friend = NULL;

	if (session->protocol_ver >= 8)
	{
		const char *group_nums;
		int list_op;

		passport   = params[0];
		friend     = msn_url_decode(params[1]);
		list_op    = atoi(params[2]);
		group_nums = params[3];

		if (list_op & MSN_LIST_FL_OP)
		{
			MsnUser *user;
			char **c;
			char **tokens;

			user = msn_user_new(session, passport, friend);

			tokens = g_strsplit(group_nums, ",", -1);

			gaim_debug_misc("msn", "Fetching group IDs from '%s'\n",
							group_nums);
			for (c = tokens; *c != NULL; c++)
			{
				gaim_debug_misc("msn", "Appending group ID %d\n", atoi(*c));
				msn_user_add_group_id(user, atoi(*c));
			}

			g_strfreev(tokens);

			session->lists.forward =
				g_slist_append(session->lists.forward, user);

			session->last_user_added = user;
		}

		if (list_op & MSN_LIST_AL_OP)
		{
			/* These are users who are allowed to see our status. */

			if (g_slist_find_custom(account->deny, passport,
									(GCompareFunc)strcmp))
			{
				gaim_privacy_deny_remove(gc->account, passport, TRUE);
			}

			gaim_privacy_permit_add(account, passport, TRUE);
		}

		if (list_op & MSN_LIST_BL_OP)
		{
			/* These are users who are not allowed to see our status. */
			gaim_privacy_deny_add(account, passport, TRUE);
		}

		if (list_op & MSN_LIST_RL_OP)
		{
			/* These are users who have us on their contact list. */

			gboolean new_entry = TRUE;

			if (g_slist_find_custom(account->permit, passport,
									(GCompareFunc)g_ascii_strcasecmp) ||
				g_slist_find_custom(account->deny, passport,
									(GCompareFunc)g_ascii_strcasecmp))
			{
				new_entry = FALSE;
			}

			if (new_entry)
			{
				MsnPermitAdd *pa;
				char msg[MSN_BUF_LEN];

				pa       = g_new0(MsnPermitAdd, 1);
				pa->user = msn_user_new(session, passport, friend);
				pa->gc   = gc;

				g_snprintf(msg, sizeof(msg),
						   _("The user %s (%s) wants to add you to their "
							 "buddy list."),
						   msn_user_get_passport(pa->user),
						   msn_user_get_name(pa->user));

				gaim_request_action(gc, NULL, msg, NULL, 0, pa, 2,
									_("Authorize"),
									G_CALLBACK(msn_accept_add_cb),
									_("Deny"),
									G_CALLBACK(msn_cancel_add_cb));
			}
		}

		session->num_users++;

		if (session->num_users == session->total_users)
		{
			if (!msn_servconn_send_command(servconn, "CHG", "NLN"))
			{
				gaim_connection_error(gc, _("Unable to write"));

				return FALSE;
			}

			gaim_connection_set_state(gc, GAIM_CONNECTED);
			serv_finish_login(gc);

			if (session->lists.allow == NULL)
				session->lists.allow = g_slist_copy(account->permit);
			else
				session->lists.allow = g_slist_concat(session->lists.allow,
													  account->permit);

			if (session->lists.block == NULL)
				session->lists.block = g_slist_copy(account->permit);
			else
				session->lists.block = g_slist_concat(session->lists.block,
													  account->deny);

			while (session->lists.forward != NULL)
			{
				MsnUser *user = session->lists.forward->data;
				GSList *buddies;
				GSList *sl;

				session->lists.forward =
					g_slist_remove(session->lists.forward, user);

				add_buddy(servconn, user);

				buddies = gaim_find_buddies(account,
											msn_user_get_passport(user));

				/* Find all occurrences of this buddy in the wrong place. */
				for (sl = buddies; sl != NULL; sl = sl->next)
				{
					GaimBuddy *b = sl->data;

					if (b->proto_data == NULL)
					{
						gaim_debug_warning("msn",
							"Deleting misplaced user %s (%s) during sync "
							"with server.\n",
							b->name, gaim_find_buddys_group(b)->name);

						gaim_blist_remove_buddy(b);
					}
				}

				g_slist_free(buddies);
			}

			session->syncing_lists = FALSE;
			session->lists_synced  = TRUE;
		}
	}
	else
	{
		const char *list_name;
		int user_num;
		int num_users;

		list_name = params[1];
		user_num  = atoi(params[3]);
		num_users = atoi(params[4]);

		if (g_ascii_strcasecmp(list_name, "RL") &&
			user_num == 0 && num_users == 0)
		{
			return TRUE; /* There are no users on this list. */
		}

		if (num_users > 0)
		{
			passport  = params[5];
			friend    = msn_url_decode(params[6]);
		}

		if (session->syncing_lists && session->lists_synced)
			return TRUE;

		if (!g_ascii_strcasecmp(list_name, "FL") && user_num != 0)
		{
			/* These are users on our contact list. */
			MsnUser *user;

			user = msn_user_new(session, passport, friend);

			if (param_count == 8)
				msn_user_add_group_id(user, atoi(params[7]));

			session->lists.forward =
				g_slist_append(session->lists.forward, user);
		}
		else if (!g_ascii_strcasecmp(list_name, "AL") && user_num != 0)
		{
			/* These are users who are allowed to see our status. */
			if (g_slist_find_custom(gc->account->deny, passport,
									(GCompareFunc)strcmp))
			{
				gaim_debug(GAIM_DEBUG_INFO, "msn",
						   "Moving user from deny list to permit: %s (%s)\n",
						   passport, friend);

				gaim_privacy_deny_remove(gc->account, passport, TRUE);
			}

			gaim_privacy_permit_add(gc->account, passport, TRUE);
		}
		else if (!g_ascii_strcasecmp(list_name, "BL") && user_num != 0)
		{
			/* These are users who are not allowed to see our status. */
			gaim_privacy_deny_add(gc->account, passport, TRUE);
		}
		else if (!g_ascii_strcasecmp(list_name, "RL"))
		{
			/* These are users who have us on their contact list. */
			if (user_num > 0)
			{
				gboolean new_entry = TRUE;

				if (g_slist_find_custom(gc->account->permit, passport,
										(GCompareFunc)g_ascii_strcasecmp))
				{
					new_entry = FALSE;
				}

				if (g_slist_find_custom(gc->account->deny, passport,
										(GCompareFunc)g_ascii_strcasecmp))
				{
					new_entry = FALSE;
				}

				if (new_entry)
				{
					MsnPermitAdd *pa;
					char msg[MSN_BUF_LEN];

					gaim_debug(GAIM_DEBUG_WARNING, "msn",
							   "Unresolved MSN RL entry: %s\n", passport);

					pa       = g_new0(MsnPermitAdd, 1);
					pa->user = msn_user_new(session, passport, friend);
					pa->gc   = gc;

					g_snprintf(msg, sizeof(msg),
							   _("The user %s (%s) wants to add you to their "
								 "buddy list."),
							   msn_user_get_passport(pa->user),
							   msn_user_get_name(pa->user));

					gaim_request_action(gc, NULL, msg, NULL, 0, pa, 2,
										_("Authorize"),
										G_CALLBACK(msn_accept_add_cb),
										_("Deny"),
										G_CALLBACK(msn_cancel_add_cb));
				}
			}

			if (user_num != num_users)
				return TRUE; /* This isn't the last one in the RL. */

			/* Now we're at the last one, so we can do final work. */
			if (!session->lists_synced)
			{
				if (!msn_servconn_send_command(servconn, "CHG", "NLN"))
				{
					gaim_connection_error(gc, _("Unable to write"));

					return FALSE;
				}

				gaim_connection_set_state(gc, GAIM_CONNECTED);
				serv_finish_login(gc);
			}

			if (session->lists.allow == NULL)
				session->lists.allow = g_slist_copy(gc->account->permit);
			else
				session->lists.allow = g_slist_concat(session->lists.allow,
													  gc->account->permit);

			if (session->lists.block == NULL)
				session->lists.block = g_slist_copy(gc->account->deny);
			else
				session->lists.block = g_slist_concat(session->lists.block,
													  gc->account->deny);

			while (session->lists.forward != NULL)
			{
				MsnUser *user = session->lists.forward->data;

				session->lists.forward =
					g_slist_remove(session->lists.forward, user);

				add_buddy(servconn, user);
			}

			session->syncing_lists = FALSE;
			session->lists_synced  = TRUE;
		}
	}

	return TRUE;
}

static gboolean
nln_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	GaimConnection *gc = session->account->gc;
	MsnUser *user;
	MsnObject *msnobj;
	const char *state;
	const char *passport;
	const char *friend;
	int status = 0;

	state    = params[0];
	passport = params[1];
	friend   = msn_url_decode(params[2]);

	user = msn_users_find_with_passport(session->users, passport);

	serv_got_alias(gc, (char *)passport, (char *)friend);

	msn_user_set_name(user, friend);

	if (session->protocol_ver >= 9 && param_count == 5)
	{
		msnobj = msn_object_new_from_string(msn_url_decode(params[4]));
		msn_user_set_object(user, msnobj);
	}

	if (!g_ascii_strcasecmp(state, "BSY"))
		status |= UC_UNAVAILABLE | (MSN_BUSY << 1);
	else if (!g_ascii_strcasecmp(state, "IDL"))
		status |= UC_UNAVAILABLE | (MSN_IDLE << 1);
	else if (!g_ascii_strcasecmp(state, "BRB"))
		status |= UC_UNAVAILABLE | (MSN_BRB << 1);
	else if (!g_ascii_strcasecmp(state, "AWY"))
		status |= UC_UNAVAILABLE | (MSN_AWAY << 1);
	else if (!g_ascii_strcasecmp(state, "PHN"))
		status |= UC_UNAVAILABLE | (MSN_PHONE << 1);
	else if (!g_ascii_strcasecmp(state, "LUN"))
		status |= UC_UNAVAILABLE | (MSN_LUNCH << 1);

	serv_got_update(gc, (char *)passport, 1, 0, 0, 0, status);

	return TRUE;
}

static gboolean
not_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	/* GaimConnection *gc = servconn->session->account->gc; */

	servconn->parsing_multiline = TRUE;
	servconn->multiline_type    = MSN_MULTILINE_NOT;
	servconn->multiline_len     = atoi(params[0]);

	servconn->msg_passport = NULL;
	servconn->msg_friendly = NULL;

	return TRUE;
}

static gboolean
prp_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	const char *type, *value;

	type  = params[2];
	value = params[3];

	if (param_count == 4) {
		if (!strcmp(type, "PHH"))
			msn_user_set_home_phone(session->user, msn_url_decode(value));
		else if (!strcmp(type, "PHW"))
			msn_user_set_work_phone(session->user, msn_url_decode(value));
		else if (!strcmp(type, "PHM"))
			msn_user_set_mobile_phone(session->user, msn_url_decode(value));
	}

	return TRUE;
}

static gboolean
rea_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	GaimConnection *gc = session->account->gc;
	char *friend;

	friend = msn_url_decode(params[3]);

	gaim_connection_set_display_name(gc, friend);

	return TRUE;
}

static gboolean
reg_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	MsnGroup *group;
	int group_id;
	char *group_name;

	group_id = atoi(params[2]);

	group_name = msn_url_decode(params[3]);

	group = msn_groups_find_with_id(session->groups, group_id);

	gaim_debug(GAIM_DEBUG_INFO, "msn", "Renamed group %s to %s\n",
			   msn_group_get_name(group), group_name);

	if (group != NULL)
		msn_group_set_name(group, group_name);

	return TRUE;
}

static gboolean
rem_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	const char *passport = params[3];

	if (param_count == 5)
	{
		MsnUser *user;
		int group_id = atoi(params[4]);

		user = msn_users_find_with_passport(session->users, passport);

		msn_user_remove_group_id(user, group_id);
	}

	/* I hate this. */
	if (session->moving_buddy) {
		MsnGroup *group, *old_group;
		GaimConnection *gc = session->account->gc;
		const char *friendly;
		char outparams[MSN_BUF_LEN];

		group = msn_groups_find_with_name(session->groups,
										  session->dest_group_name);

		old_group = session->old_group;

		session->moving_buddy = FALSE;
		session->old_group    = NULL;

		if (group == NULL) {
			gaim_debug(GAIM_DEBUG_ERROR, "msn",
					   "Still don't have a group ID for %s while moving %s!\n",
					   session->dest_group_name, passport);

			g_free(session->dest_group_name);
			session->dest_group_name = NULL;

			return TRUE;
		}

		g_free(session->dest_group_name);
		session->dest_group_name = NULL;

		if ((friendly = msn_user_get_name(session->moving_user)) == NULL)
			friendly = passport;

		g_snprintf(outparams, sizeof(outparams), "FL %s %s %d",
				   passport, friendly, msn_group_get_id(group));

		if (!msn_servconn_send_command(session->notification_conn,
									   "ADD", outparams)) {
			gaim_connection_error(gc, _("Write error"));
			return FALSE;
		}

		if (old_group != NULL)
			msn_group_remove_user(old_group, session->moving_user);

		msn_user_unref(session->moving_user);

		session->moving_user = NULL;

		if (old_group != NULL &&
			msn_users_get_count(msn_group_get_users(old_group)) <= 0) {

			g_snprintf(outparams, sizeof(outparams), "%d",
					   msn_group_get_id(old_group));

			if (!msn_servconn_send_command(session->notification_conn,
										   "RMG", outparams)) {

				gaim_connection_error(gc, _("Write error"));
				return FALSE;
			}
		}
	}

	return TRUE;
}

static gboolean
rmg_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	MsnGroup *group;

	group = msn_groups_find_with_id(session->groups, atoi(params[2]));

	if (group != NULL)
		msn_groups_remove(session->groups, group);

	return TRUE;
}

static gboolean
syn_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;

	if (session->protocol_ver >= 8)
	{
		session->total_users  = atoi(params[2]);
		session->total_groups = atoi(params[3]);
	}

	return TRUE;
}

/**************************************************************************
 * Misc commands
 **************************************************************************/
static gboolean
url_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	GaimAccount *account = session->account;
	const char *rru;
	const char *url;
	md5_state_t st;
	md5_byte_t di[16];
	FILE *fd;
	char buf[2048];
	char buf2[3];
	char sendbuf[64];
	int i;

	rru = params[1];
	url = params[2];

	g_snprintf(buf, sizeof(buf), "%s%lu%s",
			   session->passport_info.mspauth,
			   time(NULL) - session->passport_info.sl,
			   gaim_account_get_password(account));

	md5_init(&st);
	md5_append(&st, (const md5_byte_t *)buf, strlen(buf));
	md5_finish(&st, di);

	memset(sendbuf, 0, sizeof(sendbuf));

	for (i = 0; i < 16; i++) {
		g_snprintf(buf2, sizeof(buf2), "%02x", di[i]);
		strcat(sendbuf, buf2);
	}

	if (session->passport_info.file != NULL) {
		unlink(session->passport_info.file);
		g_free(session->passport_info.file);
	}

	if ((fd = gaim_mkstemp(&session->passport_info.file)) == NULL) {
		gaim_debug(GAIM_DEBUG_ERROR, "msn",
				   "Error opening temp passport file: %s\n",
				   strerror(errno));
	}
	else {
		fputs("<html>\n"
			  "<head>\n"
			  "<noscript>\n"
			  "<meta http-equiv=\"Refresh\" content=\"0; "
			        "url=http://www.hotmail.com\">\n"
			  "</noscript>\n"
			  "</head>\n\n",
			  fd);

		fprintf(fd, "<body onload=\"document.pform.submit(); \">\n");
		fprintf(fd, "<form name=\"pform\" action=\"%s\" method=\"POST\">\n\n",
				url);
		fprintf(fd, "<input type=\"hidden\" name=\"mode\" value=\"ttl\">\n");
		fprintf(fd, "<input type=\"hidden\" name=\"login\" value=\"%s\">\n",
				gaim_account_get_username(account));
		fprintf(fd, "<input type=\"hidden\" name=\"username\" value=\"%s\">\n",
				gaim_account_get_username(account));
		fprintf(fd, "<input type=\"hidden\" name=\"sid\" value=\"%s\">\n",
				session->passport_info.sid);
		fprintf(fd, "<input type=\"hidden\" name=\"kv\" value=\"%s\">\n",
				session->passport_info.kv);
		fprintf(fd, "<input type=\"hidden\" name=\"id\" value=\"2\">\n");
		fprintf(fd, "<input type=\"hidden\" name=\"sl\" value=\"%ld\">\n",
				time(NULL) - session->passport_info.sl);
		fprintf(fd, "<input type=\"hidden\" name=\"rru\" value=\"%s\">\n",
				rru);
		fprintf(fd, "<input type=\"hidden\" name=\"auth\" value=\"%s\">\n",
				session->passport_info.mspauth);
		fprintf(fd, "<input type=\"hidden\" name=\"creds\" value=\"%s\">\n",
				sendbuf); /* TODO Digest me (huh? -- ChipX86) */
		fprintf(fd, "<input type=\"hidden\" name=\"svc\" value=\"mail\">\n");
		fprintf(fd, "<input type=\"hiden\" name=\"js\" value=\"yes\">\n");
		fprintf(fd, "</form></body>\n");
		fprintf(fd, "</html>\n");

		if (fclose(fd)) {
			gaim_debug(GAIM_DEBUG_ERROR, "msn",
					   "Error closing temp passport file: %s\n",
					   strerror(errno));

			unlink(session->passport_info.file);
			g_free(session->passport_info.file);
		}
		else {
			/*
			 * Renaming file with .html extension, so that the
			 * win32 open_url will work.
			 */
			char *tmp;

			if ((tmp = g_strdup_printf("%s.html",
					session->passport_info.file)) != NULL) {

				if (rename(session->passport_info.file, tmp) == 0) {
					g_free(session->passport_info.file);
					session->passport_info.file = tmp;
				}
				else
					g_free(tmp);
			}
		}
	}

	return TRUE;
}
/**************************************************************************
 * Switchboards
 **************************************************************************/
static gboolean
rng_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	MsnSwitchBoard *swboard;
	MsnUser *user;
	const char *session_id;
	char *host, *c;
	int port;

	session_id = params[0];

	host = g_strdup(params[1]);

	if ((c = strchr(host, ':')) != NULL) {
		*c = '\0';
		port = atoi(c + 1);
	}
	else
		port = 1863;

	swboard = msn_switchboard_new(session);

	user = msn_user_new(session, params[4], NULL);

	msn_switchboard_set_invited(swboard, TRUE);
	msn_switchboard_set_session_id(swboard, params[0]);
	msn_switchboard_set_auth_key(swboard, params[3]);
	msn_switchboard_set_user(swboard, user);

	if (!msn_switchboard_connect(swboard, host, port)) {
		gaim_debug(GAIM_DEBUG_ERROR, "msn",
				   "Unable to connect to switchboard on %s, port %d\n",
				   host, port);

		g_free(host);

		return FALSE;
	}

	g_free(host);

	return TRUE;
}

static gboolean
xfr_cmd(MsnServConn *servconn, const char *command, const char **params,
		size_t param_count)
{
	MsnSession *session = servconn->session;
	MsnSwitchBoard *swboard;
	GaimConnection *gc = session->account->gc;
	char *host;
	char *c;
	int port;

	if (strcmp(params[1], "SB") && strcmp(params[1], "NS")) {
		gaim_connection_error(gc, _("Got invalid XFR"));
		
		return FALSE;
	}

	host = g_strdup(params[2]);

	if ((c = strchr(host, ':')) != NULL) {
		*c = '\0';
		port = atoi(c + 1);
	}
	else
		port = 1863;

	if (!strcmp(params[1], "SB")) {
		swboard = msn_session_find_unused_switch(session);

		if (swboard == NULL) {
			gaim_debug(GAIM_DEBUG_ERROR, "msn",
					   "Received an XFR SB request when there's no unused "
					   "switchboards!\n");
			return FALSE;
		}

		msn_switchboard_set_auth_key(swboard, params[4]);

		if (!msn_switchboard_connect(swboard, host, port)) {
			gaim_debug(GAIM_DEBUG_ERROR, "msn",
					   "Unable to connect to switchboard on %s, port %d\n",
					   host, port);

			g_free(host);

			return FALSE;
		}
	}
	else if (!strcmp(params[1], "NS")) {
		msn_servconn_destroy(session->notification_conn);

		session->notification_conn = msn_notification_new(session, host, port);

		if (!msn_servconn_connect(session->notification_conn)) {
			gaim_connection_error(gc, _("Unable to transfer to "
									  "notification server"));

			return FALSE;
		}
	}

	g_free(host);

	return TRUE;
}

/**************************************************************************
 * Message Types
 **************************************************************************/
static gboolean
profile_msg(MsnServConn *servconn, MsnMessage *msg)
{
	MsnSession *session = servconn->session;
	const char *value;

	if (strcmp(servconn->msg_passport, "Hotmail")) {
		/* This isn't an official message. */
		return TRUE;
	}

	if ((value = msn_message_get_attr(msg, "kv")) != NULL)
		session->passport_info.kv = g_strdup(value);

	if ((value = msn_message_get_attr(msg, "sid")) != NULL)
		session->passport_info.sid = g_strdup(value);

	if ((value = msn_message_get_attr(msg, "MSPAuth")) != NULL)
		session->passport_info.mspauth = g_strdup(value);

	if ((value = msn_message_get_attr(msg, "ClientIP")) != NULL)
		session->passport_info.client_ip = g_strdup(value);

	if ((value = msn_message_get_attr(msg, "ClientPort")) != NULL)
		session->passport_info.client_port = ntohs(atoi(value));

	return TRUE;
}

static gboolean
initial_email_msg(MsnServConn *servconn, MsnMessage *msg)
{
	MsnSession *session = servconn->session;
	GaimConnection *gc = session->account->gc;
	GHashTable *table;
	const char *unread;

	if (strcmp(servconn->msg_passport, "Hotmail")) {
		/* This isn't an official message. */
		return TRUE;
	}

	if (!gaim_account_get_check_mail(session->account))
		return TRUE;

	if (session->passport_info.file == NULL) {
		msn_servconn_send_command(servconn, "URL", "INBOX");

		msn_servconn_queue_message(servconn, "URL", msg);

		return TRUE;
	}

	table = msn_message_get_hashtable_from_body(msg);

	unread = g_hash_table_lookup(table, "Inbox-Unread");

	if (unread != NULL) {
		const char *passport = msn_user_get_passport(session->user);
		const char *url = session->passport_info.file;

		gaim_notify_emails(gc, atoi(unread), FALSE, NULL, NULL,
						   &passport, &url, NULL, NULL);
	}

	g_hash_table_destroy(table);

	return TRUE;
}

static gboolean
email_msg(MsnServConn *servconn, MsnMessage *msg)
{
	MsnSession *session = servconn->session;
	GaimConnection *gc = session->account->gc;
	GHashTable *table;
	const char *from, *subject;

	if (strcmp(servconn->msg_passport, "Hotmail")) {
		/* This isn't an official message. */
		return TRUE;
	}

	if (!gaim_account_get_check_mail(session->account))
		return TRUE;

	if (session->passport_info.file == NULL) {
		msn_servconn_send_command(servconn, "URL", "INBOX");

		msn_servconn_queue_message(servconn, "URL", msg);

		return TRUE;
	}

	table = msn_message_get_hashtable_from_body(msg);

	from    = g_hash_table_lookup(table, "From");
	subject = g_hash_table_lookup(table, "Subject");

	gaim_notify_email(gc, subject, from, msn_user_get_passport(session->user),
					  session->passport_info.file, NULL, NULL);

	g_hash_table_destroy(table);

	return TRUE;
}

static gboolean
system_msg(MsnServConn *servconn, MsnMessage *msg)
{
	GHashTable *table;
	const char *type_s;

	if (strcmp(servconn->msg_passport, "Hotmail")) {
		/* This isn't an official message. */
		return TRUE;
	}

	table = msn_message_get_hashtable_from_body(msg);

	if ((type_s = g_hash_table_lookup(table, "Type")) != NULL) {
		int type = atoi(type_s);
		char buf[MSN_BUF_LEN];
		int minutes;

		switch (type) {
			case 1:
				minutes = atoi(g_hash_table_lookup(table, "Arg1"));
				g_snprintf(buf, sizeof(buf), ngettext(
						   "The MSN server will shut down for maintenance "
						   "in %d minute. You will automatically be "
						   "signed out at that time.  Please finish any "
						   "conversations in progress.\n\nAfter the "
						   "maintenance has been completed, you will be "
						   "able to successfully sign in.",
						   "The MSN server will shut down for maintenance "
						   "in %d minutes. You will automatically be "
						   "signed out at that time.  Please finish any "
						   "conversations in progress.\n\nAfter the "
						   "maintenance has been completed, you will be "
						   "able to successfully sign in.", minutes),
						   minutes);
			default:
				break;
		}

		if (*buf != '\0')
			gaim_notify_info(servconn->session->account->gc, NULL, buf, NULL);
	}

	g_hash_table_destroy(table);

	return TRUE;
}

static gboolean
connect_cb(gpointer data, gint source, GaimInputCondition cond)
{
	MsnServConn *notification = data;
	MsnSession *session = notification->session;
	GaimAccount *account = session->account;
	GaimConnection *gc = gaim_account_get_connection(account);
	char proto_vers[256];
	size_t i;

	if (source == -1) {
		gaim_connection_error(session->account->gc, _("Unable to connect"));
		return FALSE;
	}

	if (notification->fd != source)
		notification->fd = source;

	proto_vers[0] = '\0';

	for (i = 7; i <= session->protocol_ver; i++)
	{
		char old_buf[256];

		strcpy(old_buf, proto_vers);

		g_snprintf(proto_vers, sizeof(proto_vers), "MSNP%d %s", i, old_buf);
	}

	strncat(proto_vers, "CVR0", sizeof(proto_vers));

	if (!msn_servconn_send_command(notification, "VER", proto_vers))
	{
		gaim_connection_error(gc, _("Unable to write to server"));
		return FALSE;
	}

	session->user = msn_user_new(session,
								 gaim_account_get_username(account), NULL);

	gaim_connection_update_progress(gc, _("Syncing with server"),
									4, MSN_CONNECT_STEPS);

	return TRUE;
}

static void
failed_read_cb(gpointer data, gint source, GaimInputCondition cond)
{
	MsnServConn *notification = data;
	GaimConnection *gc;

	gc = notification->session->account->gc;

	gaim_connection_error(gc, _("Error reading from server"));
}

MsnServConn *
msn_notification_new(MsnSession *session, const char *server, int port)
{
	MsnServConn *notification;

	notification = msn_servconn_new(session);

	msn_servconn_set_server(notification, server, port);
	msn_servconn_set_connect_cb(notification, connect_cb);
	msn_servconn_set_failed_read_cb(notification, failed_read_cb);

	if (notification_commands == NULL) {
		/* Register the command callbacks. */
		msn_servconn_register_command(notification, "ADD",       add_cmd);
		msn_servconn_register_command(notification, "ADG",       adg_cmd);
		msn_servconn_register_command(notification, "BLP",       blp_cmd);
		msn_servconn_register_command(notification, "BPR",       bpr_cmd);
		msn_servconn_register_command(notification, "CHG",       blank_cmd);
		msn_servconn_register_command(notification, "CHL",       chl_cmd);
		msn_servconn_register_command(notification, "CVR",       cvr_cmd);
		msn_servconn_register_command(notification, "FLN",       fln_cmd);
		msn_servconn_register_command(notification, "GTC",       blank_cmd);
		msn_servconn_register_command(notification, "ILN",       iln_cmd);
		msn_servconn_register_command(notification, "INF",       inf_cmd);
		msn_servconn_register_command(notification, "IPG",       ipg_cmd);
		msn_servconn_register_command(notification, "LSG",       lsg_cmd);
		msn_servconn_register_command(notification, "LST",       lst_cmd);
		msn_servconn_register_command(notification, "MSG",       msg_cmd);
		msn_servconn_register_command(notification, "NLN",       nln_cmd);
		msn_servconn_register_command(notification, "NOT",       not_cmd);
		msn_servconn_register_command(notification, "OUT",       out_cmd);
		msn_servconn_register_command(notification, "PRP",       prp_cmd);
		msn_servconn_register_command(notification, "QNG",       blank_cmd);
		msn_servconn_register_command(notification, "QRY",       blank_cmd);
		msn_servconn_register_command(notification, "REA",       rea_cmd);
		msn_servconn_register_command(notification, "REG",       reg_cmd);
		msn_servconn_register_command(notification, "REM",       rem_cmd);
		msn_servconn_register_command(notification, "RMG",       rmg_cmd);
		msn_servconn_register_command(notification, "RNG",       rng_cmd);
		msn_servconn_register_command(notification, "SYN",       syn_cmd);
		msn_servconn_register_command(notification, "URL",       url_cmd);
		msn_servconn_register_command(notification, "USR",       usr_cmd);
		msn_servconn_register_command(notification, "VER",       ver_cmd);
		msn_servconn_register_command(notification, "XFR",       xfr_cmd);
		msn_servconn_register_command(notification, "_unknown_", unknown_cmd);

		/* Register the message type callbacks. */
		msn_servconn_register_msg_type(notification, "text/x-msmsgsprofile",
									   profile_msg);
		msn_servconn_register_msg_type(notification,
									   "text/x-msmsgsinitialemailnotification",
									   initial_email_msg);
		msn_servconn_register_msg_type(notification,
									   "text/x-msmsgsemailnotification",
									   email_msg);
		msn_servconn_register_msg_type(notification,
									   "application/x-msmsgssystemmessage",
									   system_msg);

		/* Save these for future use. */
		notification_commands  = notification->commands;
		notification_msg_types = notification->msg_types;
	}
	else {
		g_hash_table_destroy(notification->commands);
		g_hash_table_destroy(notification->msg_types);

		notification->commands  = notification_commands;
		notification->msg_types = notification_msg_types;
	}

	return notification;
}

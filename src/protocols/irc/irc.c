/**
 * @file irc.c
 *
 * gaim
 *
 * Copyright (C) 2003, Robbert Haarman <gaim@inglorion.net>
 * Copyright (C) 2003, Ethan Blanton <eblanton@cs.purdue.edu>
 * Copyright (C) 2000-2003, Rob Flynn <rob@tgflinux.com>
 * Copyright (C) 1998-1999, Mark Spencer <markster@marko.net>
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

#include "internal.h"

#include "plugin.h"
#include "accountopt.h"
#include "multi.h"
#include "prpl.h"
#include "conversation.h"
#include "notify.h"
#include "debug.h"
#include "blist.h"
#include "util.h"
#include "irc.h"

static void irc_buddy_append(char *name, struct irc_buddy *ib, GString *string);

static const char *irc_blist_icon(GaimAccount *a, GaimBuddy *b);
static void irc_blist_emblems(GaimBuddy *b, char **se, char **sw, char **nw, char **ne);
static GList *irc_away_states(GaimConnection *gc);
static GList *irc_actions(GaimConnection *gc);
/* static GList *irc_chat_info(GaimConnection *gc); */
static void irc_login(GaimAccount *account);
static void irc_login_cb(gpointer data, gint source, GaimInputCondition cond);
static void irc_close(GaimConnection *gc);
static int irc_im_send(GaimConnection *gc, const char *who, const char *what, GaimConvImFlags flags);
static int irc_chat_send(GaimConnection *gc, int id, const char *what);
static void irc_chat_join (GaimConnection *gc, GHashTable *data);
static void irc_input_cb(gpointer data, gint source, GaimInputCondition cond);

static guint irc_nick_hash(const char *nick);
static gboolean irc_nick_equal(const char *nick1, const char *nick2);
static void irc_buddy_free(struct irc_buddy *ib);

static GaimPlugin *_irc_plugin = NULL;

static const char *status_chars = "@+%&";

static void irc_view_motd(GaimConnection *gc)
{
	struct irc_conn *irc;
	char *title;

	if (gc == NULL || gc->proto_data == NULL) {
		gaim_debug(GAIM_DEBUG_ERROR, "irc", "got MOTD request for NULL gc\n");
		return;
	}
	irc = gc->proto_data;
	if (irc->motd == NULL) {
		gaim_notify_error(gc, _("Error displaying MOTD"), _("No MOTD available"),
				  _("There is no MOTD associated with this connection."));
		return;
	}
	title = g_strdup_printf(_("MOTD for %s"), irc->server);
	gaim_notify_formatted(gc, title, title, NULL, irc->motd->str, NULL, NULL);
}

int irc_send(struct irc_conn *irc, const char *buf)
{
	if (irc->fd < 0)
		return -1;

	/* gaim_debug(GAIM_DEBUG_MISC, "irc", "sent: %s", buf); */
	return write(irc->fd, buf, strlen(buf));
}

/* XXX I don't like messing directly with these buddies */
gboolean irc_blist_timeout(struct irc_conn *irc)
{
	GString *string = g_string_sized_new(512);
	char *list, *buf;

	g_hash_table_foreach(irc->buddies, (GHFunc)irc_buddy_append, (gpointer)string);

	list = g_string_free(string, FALSE);
	if (!list || !strlen(list)) {
		g_free(list);
		return TRUE;
	}

	buf = irc_format(irc, "vn", "ISON", list);
	g_free(list);
	irc_send(irc, buf);
	g_free(buf);

	return TRUE;
}

static void irc_buddy_append(char *name, struct irc_buddy *ib, GString *string)
{
	ib->flag = FALSE;
	g_string_append_printf(string, "%s ", name);
}

static const char *irc_blist_icon(GaimAccount *a, GaimBuddy *b)
{
	return "irc";
}

static void irc_blist_emblems(GaimBuddy *b, char **se, char **sw, char **nw, char **ne)
{
	if (b->present == GAIM_BUDDY_OFFLINE)
		*se = "offline";
}

static GList *irc_away_states(GaimConnection *gc)
{
	return g_list_append(NULL, (gpointer)GAIM_AWAY_CUSTOM);
}

static GList *irc_actions(GaimConnection *gc)
{
	struct proto_actions_menu *pam;
	GList *list = NULL;

	pam = g_new0(struct proto_actions_menu, 1);
	pam->label = _("View MOTD");
	pam->callback = irc_view_motd;
	pam->gc = gc;
	list = g_list_append(list, pam);

	return list;
}

static GList *irc_buddy_menu(GaimConnection *gc, const char *who)
{
	GList *m = NULL;
	struct proto_buddy_menu *pbm;
	
	pbm = g_new0(struct proto_buddy_menu, 1);
	pbm->label = _("Send File");
	pbm->callback = irc_dccsend_send_ask;
	pbm->gc = gc;
	m = g_list_append(m, pbm);
	
	return m;
}

static GList *irc_chat_join_info(GaimConnection *gc)
{
	GList *m = NULL;
	struct proto_chat_entry *pce;

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Channel:");
	pce->identifier = "channel";
	m = g_list_append(m, pce);

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Password:");
	pce->identifier = "password";
	pce->secret = TRUE;
	m = g_list_append(m, pce);

	return m;
}

static void irc_login(GaimAccount *account)
{
	GaimConnection *gc;
	struct irc_conn *irc;
	char *buf, **userparts;
	const char *username = gaim_account_get_username(account);
	int err;

	gc = gaim_account_get_connection(account);

	if (strpbrk(username, " \t\v\r\n") != NULL) {
		gaim_connection_error(gc, _("IRC nicks may not contain whitespace"));
		return;
	}

	gc->proto_data = irc = g_new0(struct irc_conn, 1);
	irc->account = account;

	userparts = g_strsplit(username, "@", 2);
	gaim_connection_set_display_name(gc, userparts[0]);
	irc->server = g_strdup(userparts[1]);
	g_strfreev(userparts);

	irc->buddies = g_hash_table_new_full((GHashFunc)irc_nick_hash, (GEqualFunc)irc_nick_equal, 
					     NULL, (GDestroyNotify)irc_buddy_free);
	irc->cmds = g_hash_table_new(g_str_hash, g_str_equal);
	irc_cmd_table_build(irc);
	irc->msgs = g_hash_table_new(g_str_hash, g_str_equal);
	irc_msg_table_build(irc);

	buf = g_strdup_printf(_("Signon: %s"), username);
	gaim_connection_update_progress(gc, buf, 1, 2);
	g_free(buf);

	err = gaim_proxy_connect(account, irc->server, 
				 gaim_account_get_int(account, "port", IRC_DEFAULT_PORT),
				 irc_login_cb, gc);

	if (err || !account->gc) {
		gaim_connection_error(gc, _("Couldn't create socket"));
		return;
	}
}

static void irc_login_cb(gpointer data, gint source, GaimInputCondition cond)
{
	GaimConnection *gc = data;
	struct irc_conn *irc = gc->proto_data;
	char hostname[256];
	char *buf;
	const char *username;
	GList *connections = gaim_connections_get_all();

	if (source < 0)
		return;

	if (!g_list_find(connections, gc)) {
		close(source);
		return;
	}

	irc->fd = source;

	if (gc->account->password && *gc->account->password) {
		buf = irc_format(irc, "vv", "PASS", gc->account->password);
		if (irc_send(irc, buf) < 0) {
			gaim_connection_error(gc, "Error sending password");
			return;
		}
		g_free(buf);
	}

	gethostname(hostname, sizeof(hostname));
	hostname[sizeof(hostname) - 1] = '\0';
	username = gaim_account_get_string(irc->account, "username", "");
	buf = irc_format(irc, "vvvv:", "USER", strlen(username) ? username : g_get_user_name(), hostname, irc->server,
			      gc->account->alias && *gc->account->alias ? gc->account->alias : IRC_DEFAULT_ALIAS);
	if (irc_send(irc, buf) < 0) {
		gaim_connection_error(gc, "Error registering with server");
		return;
	}
	g_free(buf);
	buf = irc_format(irc, "vn", "NICK", gaim_connection_get_display_name(gc));
	if (irc_send(irc, buf) < 0) {
		gaim_connection_error(gc, "Error sending nickname");
		return;
	}
	g_free(buf);

	gc->inpa = gaim_input_add(irc->fd, GAIM_INPUT_READ, irc_input_cb, gc);
}

static void irc_close(GaimConnection *gc)
{
	struct irc_conn *irc = gc->proto_data;

	if (irc == NULL)
		return;

	irc_cmd_quit(irc, "quit", NULL, NULL);

	if (gc->inpa)
		gaim_timeout_remove(gc->inpa);

	g_free(irc->inbuf);
	close(irc->fd);
	if (irc->timer)
		gaim_timeout_remove(irc->timer);
	g_hash_table_destroy(irc->cmds);
	g_hash_table_destroy(irc->msgs);
	if (irc->motd)
		g_string_free(irc->motd, TRUE);
	g_free(irc->server);
	g_free(irc);
}

static int irc_im_send(GaimConnection *gc, const char *who, const char *what, GaimConvImFlags flags)
{
	struct irc_conn *irc = gc->proto_data;
	const char *args[2];

	if (strchr(status_chars, *who) != NULL)
		args[0] = who + 1;
	else
		args[0] = who;
	args[1] = what;

	if (*what == '/') {
		return irc_parse_cmd(irc, who, what + 1);
	}

	irc_cmd_privmsg(irc, "msg", NULL, args);
	return 1;
}

static void irc_get_info(GaimConnection *gc, const char *who)
{
	struct irc_conn *irc = gc->proto_data;
	const char *args[1];
	args[0] = who;
	irc_cmd_whois(irc, "whois", NULL, args);
}

static void irc_set_away(GaimConnection *gc, const char *state, const char *msg)
{
	struct irc_conn *irc = gc->proto_data;
	const char *args[1];

	if (gc->away) {
		g_free(gc->away);
		gc->away = NULL;
	}

	if (msg)
		gc->away = g_strdup(msg);

	args[0] = msg;
	irc_cmd_away(irc, "away", NULL, args);
}

static void irc_add_buddy(GaimConnection *gc, const char *who, GaimGroup *group)
{
	struct irc_conn *irc = (struct irc_conn *)gc->proto_data;
	struct irc_buddy *ib = g_new0(struct irc_buddy, 1);
	ib->name = g_strdup(who);
	g_hash_table_insert(irc->buddies, ib->name, ib);
}

static void irc_remove_buddy(GaimConnection *gc, const char *who, const char *group)
{
	struct irc_conn *irc = (struct irc_conn *)gc->proto_data;
	g_hash_table_remove(irc->buddies, who);
}


static void irc_input_cb(gpointer data, gint source, GaimInputCondition cond)
{
	GaimConnection *gc = data;
	struct irc_conn *irc = gc->proto_data;
	char *cur, *end;
	int len;

	if (irc->inbuflen < irc->inbufused + IRC_INITIAL_BUFSIZE) {
		irc->inbuflen += IRC_INITIAL_BUFSIZE;
		irc->inbuf = g_realloc(irc->inbuf, irc->inbuflen);
	}

	if ((len = read(irc->fd, irc->inbuf + irc->inbufused, IRC_INITIAL_BUFSIZE - 1)) < 0) {
		gaim_connection_error(gc, _("Read error"));
		return;
	} else if (len == 0) {
		/* Remote closed the connection, probably */
		return;
	}

	irc->inbufused += len;
	irc->inbuf[irc->inbufused] = '\0';

	cur = irc->inbuf;
	while (cur < irc->inbuf + irc->inbufused &&
	       ((end = strstr(cur, "\r\n")) || (end = strstr(cur, "\n")))) {
		int step = (*end == '\r' ? 2 : 1);
		*end = '\0';
		irc_parse_msg(irc, cur);
		cur = end + step;
	}
	if (cur != irc->inbuf + irc->inbufused) { /* leftover */
		irc->inbufused -= (cur - irc->inbuf);
		memmove(irc->inbuf, cur, irc->inbufused);
	} else {
		irc->inbufused = 0;
	}
}

static void irc_chat_join (GaimConnection *gc, GHashTable *data)
{
	struct irc_conn *irc = gc->proto_data;
	const char *args[2];

	args[0] = g_hash_table_lookup(data, "channel");
	args[1] = g_hash_table_lookup(data, "password");
	irc_cmd_join(irc, "join", NULL, args);
}

static void irc_chat_invite(GaimConnection *gc, int id, const char *message, const char *name) 
{
	struct irc_conn *irc = gc->proto_data;
	GaimConversation *convo = gaim_find_chat(gc, id);
	const char *args[2];

	if (!convo) {
		gaim_debug(GAIM_DEBUG_ERROR, "irc", "Got chat invite request for bogus chat\n");
		return;
	}
	args[0] = name;
	args[1] = gaim_conversation_get_name(convo);
	irc_cmd_invite(irc, "invite", gaim_conversation_get_name(convo), args);
}


static void irc_chat_leave (GaimConnection *gc, int id)
{
	struct irc_conn *irc = gc->proto_data;
	GaimConversation *convo = gaim_find_chat(gc, id);
	const char *args[2];

	if (!convo)
		return;

	args[0] = gaim_conversation_get_name(convo);
	args[1] = NULL;
	irc_cmd_part(irc, "part", gaim_conversation_get_name(convo), args);
	serv_got_chat_left(gc, id);
}

static int irc_chat_send(GaimConnection *gc, int id, const char *what)
{
	struct irc_conn *irc = gc->proto_data;
	GaimConversation *convo = gaim_find_chat(gc, id);
	const char *args[2];
	char *tmp;

	if (!convo) {
		gaim_debug(GAIM_DEBUG_ERROR, "irc", "chat send on nonexistent chat\n");
		return -EINVAL;
	}

	if (*what == '/') {
		return irc_parse_cmd(irc, convo->name, what + 1);
	}

	args[0] = convo->name;
	args[1] = what;

	irc_cmd_privmsg(irc, "msg", NULL, args);

	tmp = gaim_escape_html(what);
	serv_got_chat_in(gc, id, gaim_connection_get_display_name(gc), 0, tmp, time(NULL));
	g_free(tmp);
	return 0;
}

static guint irc_nick_hash(const char *nick)
{
	char *lc;
	guint bucket;

	lc = g_utf8_strdown(nick, -1);
	bucket = g_str_hash(lc);
	g_free(lc);

	return bucket;
}

static gboolean irc_nick_equal(const char *nick1, const char *nick2)
{
	return (gaim_utf8_strcasecmp(nick1, nick2) == 0);
}

static void irc_buddy_free(struct irc_buddy *ib)
{
	g_free(ib->name);
	g_free(ib);
}

static GaimRoomlist *irc_roomlist_get_list(GaimConnection *gc)
{
	struct irc_conn *irc;
	GList *fields = NULL;
	GaimRoomlistField *f;

	irc = gc->proto_data;

	if (irc->roomlist)
		gaim_roomlist_unref(irc->roomlist);

	irc->roomlist = gaim_roomlist_new(gaim_connection_get_account(gc));

	f = gaim_roomlist_field_new(GAIM_ROOMLIST_FIELD_STRING, "", "channel", TRUE);
	fields = g_list_append(fields, f);

	f = gaim_roomlist_field_new(GAIM_ROOMLIST_FIELD_INT, _("Users"), "users", FALSE);
	fields = g_list_append(fields, f);

	f = gaim_roomlist_field_new(GAIM_ROOMLIST_FIELD_STRING, _("Topic"), "topic", FALSE);
	fields = g_list_append(fields, f);

	gaim_roomlist_set_fields(irc->roomlist, fields);

	irc_cmd_list(irc, "LIST", NULL, NULL);

	return irc->roomlist;
}

static void irc_roomlist_cancel(GaimRoomlist *list)
{
	GaimConnection *gc = gaim_account_get_connection(list->account);
	struct irc_conn *irc;

	if (gc == NULL)
		return;

	irc = gc->proto_data;

	gaim_roomlist_set_in_progress(list, FALSE);

	if (irc->roomlist == list) {
		irc->roomlist = NULL;
		gaim_roomlist_unref(list);
	}
}

static GaimPluginProtocolInfo prpl_info =
{
	OPT_PROTO_CHAT_TOPIC | OPT_PROTO_PASSWORD_OPTIONAL,
	NULL,
	NULL,
	NULL,
	irc_blist_icon,
	irc_blist_emblems,
	NULL,
	NULL,
	irc_away_states,
	irc_actions,
	irc_buddy_menu,
	irc_chat_join_info,
	irc_login,
	irc_close,
	irc_im_send,
	NULL,
	NULL,
	irc_get_info,
	irc_set_away,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	irc_add_buddy,
	NULL,
	irc_remove_buddy,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	irc_chat_join,
	irc_chat_invite,
	irc_chat_leave,
	NULL,
	irc_chat_send,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL, /*irc_convo_closed,*/
	NULL, /* normalize */
	NULL, /* set buddy icon */
	NULL, /* remove group */
	NULL, /* get_cb_real_name */
	NULL,
	NULL,
	irc_roomlist_get_list,
	irc_roomlist_cancel,
	NULL
};


static GaimPluginInfo info =
{
	2,                                                /**< api_version    */
	GAIM_PLUGIN_PROTOCOL,                             /**< type           */
	NULL,                                             /**< ui_requirement */
	0,                                                /**< flags          */
	NULL,                                             /**< dependencies   */
	GAIM_PRIORITY_DEFAULT,                            /**< priority       */

	"prpl-irc",                                       /**< id             */
	"IRC",                                            /**< name           */
	VERSION,                                          /**< version        */
	N_("IRC Protocol Plugin"),                        /**  summary        */
	N_("The IRC Protocol Plugin that Sucks Less"),    /**  description    */
	NULL,                                             /**< author         */
	GAIM_WEBSITE,                                     /**< homepage       */

	NULL,                                             /**< load           */
	NULL,                                             /**< unload         */
	NULL,                                             /**< destroy        */

	NULL,                                             /**< ui_info        */
	&prpl_info                                        /**< extra_info     */
};

static void _init_plugin(GaimPlugin *plugin)
{
	GaimAccountUserSplit *split;
	GaimAccountOption *option;

	split = gaim_account_user_split_new(_("Server"), IRC_DEFAULT_SERVER, '@');
	prpl_info.user_splits = g_list_append(prpl_info.user_splits, split);

	option = gaim_account_option_int_new(_("Port"), "port", IRC_DEFAULT_PORT);
	prpl_info.protocol_options = g_list_append(prpl_info.protocol_options, option);

	option = gaim_account_option_string_new(_("Encoding"), "encoding", IRC_DEFAULT_CHARSET);
	prpl_info.protocol_options = g_list_append(prpl_info.protocol_options, option);

	option = gaim_account_option_string_new(_("Username"), "username", "");
	prpl_info.protocol_options = g_list_append(prpl_info.protocol_options, option);

	_irc_plugin = plugin;
}

GAIM_INIT_PLUGIN(irc, _init_plugin, info);

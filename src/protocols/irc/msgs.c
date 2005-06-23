/**
 * @file msgs.c
 * 
 * gaim
 *
 * Copyright (C) 2003, Ethan Blanton <eblanton@cs.purdue.edu>
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

#include "conversation.h"
#include "blist.h"
#include "notify.h"
#include "util.h"
#include "debug.h"
#include "irc.h"

#include <stdio.h>

static char *irc_mask_nick(const char *mask);
static char *irc_mask_userhost(const char *mask);
static void irc_chat_remove_buddy(GaimConversation *convo, char *data[2]);
static void irc_buddy_status(char *name, struct irc_buddy *ib, struct irc_conn *irc);

static char *irc_mask_nick(const char *mask)
{
	char *end, *buf;

	end = strchr(mask, '!');
	if (!end)
		buf = g_strdup(mask);
	else
		buf = g_strndup(mask, end - mask);

	return buf;
}

static char *irc_mask_userhost(const char *mask)
{
	return g_strdup(strchr(mask, '!') + 1);
}

static void irc_chat_remove_buddy(GaimConversation *convo, char *data[2])
{
	char *escaped, *message;

	escaped = g_markup_escape_text(data[1], -1);
	message = g_strdup_printf("quit: %s", escaped);

	if (gaim_conv_chat_find_user(GAIM_CONV_CHAT(convo), data[0]))
		gaim_conv_chat_remove_user(GAIM_CONV_CHAT(convo), data[0], message);

	g_free(escaped);
	g_free(message);
}

void irc_msg_default(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	gaim_debug(GAIM_DEBUG_INFO, "irc", "Unrecognized message: %s\n", args[0]);
}

void irc_msg_away(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc;
	char *msg;

	if (!args || !args[1])
		return;

	if (irc->whois.nick && !gaim_utf8_strcasecmp(irc->whois.nick, args[1])) {
		/* We're doing a whois, show this in the whois dialog */
		irc_msg_whois(irc, name, from, args);
		return;
	}

	gc = gaim_account_get_connection(irc->account);
	if (gc) {
		msg = g_markup_escape_text(args[2], -1);
		serv_got_im(gc, args[1], msg, GAIM_CONV_IM_AUTO_RESP, time(NULL));
		g_free(msg);
	}
}

void irc_msg_badmode(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	char *escaped;

	if (!args || !args[1] || !gc)
		return;

	escaped = g_markup_escape_text(args[1], -1);
	gaim_notify_error(gc, NULL, _("Bad mode"), escaped);
	g_free(escaped);
}

void irc_msg_banned(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	char *buf, *escaped;

	if (!args || !args[1] || !gc)
		return;

	escaped = g_markup_escape_text(args[1], -1);
	buf = g_strdup_printf(_("You are banned from %s."), escaped);
	gaim_notify_error(gc, _("Banned"), _("Banned"), buf);
	g_free(escaped);
	g_free(buf);
}

void irc_msg_banfull(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConversation *convo;
	char *buf, *nick;

	if (!args || !args[0] || !args[1] || !args[2])
		return;

	convo = gaim_find_conversation_with_account(args[1], irc->account);
	if (!convo)
		return;

	nick = g_markup_escape_text(args[2], -1);
	buf = g_strdup_printf(_("Cannot ban %s: banlist is full"), nick);
	g_free(nick);
	gaim_conv_chat_write(GAIM_CONV_CHAT(convo), "", buf,
			     GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG,
			     time(NULL));
	g_free(buf);
}

void irc_msg_chanmode(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConversation *convo;
	char *buf, *escaped;

	if (!args || !args[1] || !args[2])
		return;

	convo = gaim_find_conversation_with_account(args[1], irc->account);
	if (!convo)	/* XXX punt on channels we are not in for now */
		return;

	escaped = (args[3] != NULL) ? g_markup_escape_text(args[3], -1) : NULL;
	buf = g_strdup_printf("mode for %s: %s %s", args[1], args[2], escaped ? escaped : "");
	gaim_conv_chat_write(GAIM_CONV_CHAT(convo), "", buf, GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG, time(NULL));
	g_free(escaped);
	g_free(buf);

	return;
}

void irc_msg_whois(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	if (!irc->whois.nick) {
		gaim_debug(GAIM_DEBUG_WARNING, "irc", "Unexpected WHOIS reply for %s\n", args[1]);
		return;
	}

	if (gaim_utf8_strcasecmp(irc->whois.nick, args[1])) {
		gaim_debug(GAIM_DEBUG_WARNING, "irc", "Got WHOIS reply for %s while waiting for %s\n", args[1], irc->whois.nick);
		return;
	}

	if (!strcmp(name, "301")) {
		irc->whois.away = g_strdup(args[2]);
	} else if (!strcmp(name, "311")) {
		irc->whois.userhost = g_strdup_printf("%s@%s", args[2], args[3]);
		irc->whois.name = g_strdup(args[5]);
	} else if (!strcmp(name, "312")) {
		irc->whois.server = g_strdup(args[2]);
		irc->whois.serverinfo = g_strdup(args[3]);
	} else if (!strcmp(name, "313")) {
		irc->whois.ircop = 1;
	} else if (!strcmp(name, "317")) {
		irc->whois.idle = atoi(args[2]);
		if (args[3])
			irc->whois.signon = (time_t)atoi(args[3]);
	} else if (!strcmp(name, "319")) {
		irc->whois.channels = g_strdup(args[2]);
	} else if (!strcmp(name, "320")) {
		irc->whois.identified = 1;
	}
}

void irc_msg_endwhois(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc;
	GString *info;
	char buffer[256];
	char *str, *tmp;

	if (!irc->whois.nick) {
		gaim_debug(GAIM_DEBUG_WARNING, "irc", "Unexpected End of WHOIS for %s\n", args[1]);
		return;
	}
	if (gaim_utf8_strcasecmp(irc->whois.nick, args[1])) {
		gaim_debug(GAIM_DEBUG_WARNING, "irc", "Received end of WHOIS for %s, expecting %s\n", args[1], irc->whois.nick);
		return;
	}

	info = g_string_new("");
	tmp = g_markup_escape_text(args[1], -1);
	g_string_append_printf(info, _("<b>%s:</b> %s"), _("Nick"), tmp);
	g_free(tmp);
	g_string_append_printf(info, "%s%s<br>",
			       irc->whois.ircop ? _(" <i>(ircop)</i>") : "",
			       irc->whois.identified ? _(" <i>(identified)</i>") : "");
	if (irc->whois.away) {
		tmp = g_markup_escape_text(irc->whois.away, strlen(irc->whois.away));
		g_free(irc->whois.away);
		g_string_append_printf(info, _("<b>%s:</b> %s<br>"), _("Away"), tmp);
		g_free(tmp);
	}
	if (irc->whois.userhost) {
		tmp = g_markup_escape_text(irc->whois.name, strlen(irc->whois.name));
		g_free(irc->whois.name);
		g_string_append_printf(info, _("<b>%s:</b> %s<br>"), _("Username"), irc->whois.userhost);
		g_string_append_printf(info, _("<b>%s:</b> %s<br>"), _("Realname"), tmp);
		g_free(irc->whois.userhost);
		g_free(tmp);
	}
	if (irc->whois.server) {
		g_string_append_printf(info, _("<b>%s:</b> %s"), _("Server"), irc->whois.server);
		g_string_append_printf(info, " (%s)<br>", irc->whois.serverinfo);
		g_free(irc->whois.server);
		g_free(irc->whois.serverinfo);
	}
	if (irc->whois.channels) {
		g_string_append_printf(info, _("<b>%s:</b> %s<br>"), _("Currently on"), irc->whois.channels);
		g_free(irc->whois.channels);
	}
	if (irc->whois.idle) {
		gchar *timex = gaim_str_seconds_to_string(irc->whois.idle);
		g_string_append_printf(info, _("<b>Idle for:</b> %s<br>"), timex);
		g_free(timex);
		g_string_append_printf(info, _("<b>%s:</b> %s"), _("Online since"), ctime(&irc->whois.signon));
	}
	if (!strcmp(irc->whois.nick, "Paco-Paco")) {
		g_string_append_printf(info, _("<br><b>Defining adjective:</b> Glorious<br>"));
	}

	gc = gaim_account_get_connection(irc->account);
	str = g_string_free(info, FALSE);

	g_snprintf(buffer, sizeof(buffer),
			   _("Buddy Information for %s"), irc->whois.nick);
	gaim_notify_userinfo(gc, irc->whois.nick, NULL, buffer, NULL, str, NULL, NULL);

	g_free(irc->whois.nick);
	g_free(str);
	memset(&irc->whois, 0, sizeof(irc->whois));
}

void irc_msg_list(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	if (!irc->roomlist)
		return;

	if (!strcmp(name, "321")) {
		gaim_roomlist_set_in_progress(irc->roomlist, TRUE);
		return;
	}

	if (!strcmp(name, "323")) {
		gaim_roomlist_set_in_progress(irc->roomlist, FALSE);
		gaim_roomlist_unref(irc->roomlist);
		irc->roomlist = NULL;
	}

	if (!strcmp(name, "322")) {
		GaimRoomlistRoom *room;

		if (!args[0] || !args[1] || !args[2] || !args[3])
			return;

		room = gaim_roomlist_room_new(GAIM_ROOMLIST_ROOMTYPE_ROOM, args[1], NULL);
		gaim_roomlist_room_add_field(irc->roomlist, room, args[1]);
		gaim_roomlist_room_add_field(irc->roomlist, room, GINT_TO_POINTER(strtol(args[2], NULL, 10)));
		gaim_roomlist_room_add_field(irc->roomlist, room, args[3]);
		gaim_roomlist_room_add(irc->roomlist, room);
	}
}

void irc_msg_topic(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	char *chan, *topic, *msg, *nick, *tmp, *tmp2;
	GaimConversation *convo;

	if (!strcmp(name, "topic")) {
		chan = args[0];
		topic = irc_mirc2txt (args[1]);
	} else {
		chan = args[1];
		topic = irc_mirc2txt (args[2]);
	}

	convo = gaim_find_conversation_with_account(chan, irc->account);
	if (!convo) {
		gaim_debug(GAIM_DEBUG_ERROR, "irc", "Got a topic for %s, which doesn't exist\n", chan);
	}

	/* If this is an interactive update, print it out */
	tmp = gaim_escape_html(topic);
	tmp2 = gaim_markup_linkify(tmp);
	g_free(tmp);
	if (!strcmp(name, "topic")) {
		nick = irc_mask_nick(from);
		gaim_conv_chat_set_topic(GAIM_CONV_CHAT(convo), nick, topic);
		msg = g_strdup_printf(_("%s has changed the topic to: %s"), nick, tmp2);
		g_free(nick);
		gaim_conv_chat_write(GAIM_CONV_CHAT(convo), from, msg, GAIM_MESSAGE_SYSTEM, time(NULL));
		g_free(msg);
	} else {
		msg = g_strdup_printf(_("The topic for %s is: %s"), chan, tmp2);
		gaim_conv_chat_set_topic(GAIM_CONV_CHAT(convo), NULL, topic);
		gaim_conv_chat_write(GAIM_CONV_CHAT(convo), "", msg, GAIM_MESSAGE_SYSTEM, time(NULL));
		g_free(msg);
	}
	g_free(tmp2);
	g_free(topic);
}

void irc_msg_unknown(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	char *buf, *escaped;

	if (!args || !args[1] || !gc)
		return;

	escaped = g_markup_escape_text(args[1], -1);
	buf = g_strdup_printf(_("Unknown message '%s'"), escaped);
	gaim_notify_error(gc, _("Unknown message"), buf, _("Gaim has sent a message the IRC server did not understand."));
	g_free(escaped);
	g_free(buf);
}

void irc_msg_names(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	char *names, *cur, *end, *tmp, *msg;
	GaimConversation *convo;

	if (!strcmp(name, "366")) {
		convo = gaim_find_conversation_with_account(irc->nameconv ? irc->nameconv : args[1], irc->account);
		if (!convo) {
			gaim_debug(GAIM_DEBUG_ERROR, "irc", "Got a NAMES list for %s, which doesn't exist\n", args[2]);
			g_string_free(irc->names, TRUE);
			irc->names = NULL;
			g_free(irc->nameconv);
			irc->nameconv = NULL;
			return;
		}

		names = cur = g_string_free(irc->names, FALSE);
		irc->names = NULL;
		if (irc->nameconv) {
			msg = g_strdup_printf(_("Users on %s: %s"), args[1], names ? names : "");
			if (gaim_conversation_get_type(convo) == GAIM_CONV_CHAT)
				gaim_conv_chat_write(GAIM_CONV_CHAT(convo), "", msg, GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG, time(NULL));
			else
				gaim_conv_im_write(GAIM_CONV_IM(convo), "", msg, GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG, time(NULL));
			g_free(msg);
			g_free(irc->nameconv);
			irc->nameconv = NULL;
		} else {
			GList *users = NULL;
			GList *flags = NULL;

			while (*cur) {
				GaimConvChatBuddyFlags f = GAIM_CBFLAGS_NONE;
				end = strchr(cur, ' ');
				if (!end)
					end = cur + strlen(cur);
				if (*cur == '@') {
					f = GAIM_CBFLAGS_OP;
					cur++;
				} else if (*cur == '%') {
					f = GAIM_CBFLAGS_HALFOP;
					cur++;
				} else if(*cur == '+') {
					f = GAIM_CBFLAGS_VOICE;
					cur++;
				}
				tmp = g_strndup(cur, end - cur);
				users = g_list_append(users, tmp);
				flags = g_list_append(flags, GINT_TO_POINTER(f));
				cur = end;
				if (*cur)
					cur++;
			}

			if (users != NULL) {
				GList *l;

				gaim_conv_chat_add_users(GAIM_CONV_CHAT(convo), users, flags);

				for (l = users; l != NULL; l = l->next)
					g_free(l->data);

				g_list_free(users);
				g_list_free(flags);
			}
		}
		g_free(names);
	} else {
		if (!irc->names)
			irc->names = g_string_new("");

		irc->names = g_string_append(irc->names, args[3]);
	}
}

void irc_msg_motd(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc;
	char *escaped;
	if (!strcmp(name, "375")) {
		gc = gaim_account_get_connection(irc->account);
		if (gc)
			gaim_connection_set_display_name(gc, args[0]);
	}

	if (!irc->motd)
		irc->motd = g_string_new("");

	escaped = g_markup_escape_text(args[1], -1);
	g_string_append_printf(irc->motd, "%s<br>", escaped);
	g_free(escaped);
}

void irc_msg_endmotd(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc;

	gc = gaim_account_get_connection(irc->account);
	if (!gc)
		return;

	gaim_connection_set_state(gc, GAIM_CONNECTED);
	serv_finish_login (gc);

	irc_blist_timeout(irc);
	if (!irc->timer)
		irc->timer = gaim_timeout_add(45000, (GSourceFunc)irc_blist_timeout, (gpointer)irc);
}

void irc_msg_time(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc;
	char *time;

	gc = gaim_account_get_connection(irc->account);
	if (gc == NULL || args == NULL || args[2] == NULL)
		return;

	time = g_markup_escape_text(args[2], -1);
	gaim_notify_message(gc, GAIM_NOTIFY_MSG_INFO, _("Time Response"),
			    _("The IRC server's local time is:"),
			    time, NULL, NULL);
	g_free(time);
}

void irc_msg_nochan(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	char *escaped;

	if (gc == NULL || args == NULL || args[1] == NULL)
		return;

	escaped = g_markup_escape_text(args[1], -1);
	gaim_notify_error(gc, NULL, _("No such channel"), escaped);
	g_free(escaped);
}

void irc_msg_nonick(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc;
	GaimConversation *convo;
	char *nick;

	convo = gaim_find_conversation_with_account(args[1], irc->account);
	if (convo) {
		if (gaim_conversation_get_type(convo) == GAIM_CONV_CHAT) /* does this happen? */
			gaim_conv_chat_write(GAIM_CONV_CHAT(convo), args[1], _("no such channel"),
					GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG, time(NULL));
		else
			gaim_conv_im_write(GAIM_CONV_IM(convo), args[1], _("User is not logged in"),
				      GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG, time(NULL));
	} else {
		if ((gc = gaim_account_get_connection(irc->account)) == NULL)
			return;
		nick = g_markup_escape_text (args[1], -1);
		gaim_notify_error(gc, NULL, _("No such nick or channel"), nick);
		g_free (nick);
	}

	if (irc->whois.nick && !gaim_utf8_strcasecmp(irc->whois.nick, args[1])) {
		g_free(irc->whois.nick);
		irc->whois.nick = NULL;
	}
}

void irc_msg_nosend(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc;
	GaimConversation *convo;

	convo = gaim_find_conversation_with_account(args[1], irc->account);
	if (convo) {
		gaim_conv_chat_write(GAIM_CONV_CHAT(convo), args[1], args[2], GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG, time(NULL));
	} else {
		if ((gc = gaim_account_get_connection(irc->account)) == NULL)
			return;
		gaim_notify_error(gc, NULL, _("Could not send"), args[2]);
	}
}

void irc_msg_notinchan(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConversation *convo = gaim_find_conversation_with_account(args[1], irc->account);

	gaim_debug(GAIM_DEBUG_INFO, "irc", "We're apparently not in %s, but tried to use it\n", args[1]);
	if (convo) {
		/*g_slist_remove(irc->gc->buddy_chats, convo);
		  gaim_conversation_set_account(convo, NULL);*/
		gaim_conv_chat_write(GAIM_CONV_CHAT(convo), args[1], args[2], GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG, time(NULL));
	}
}

void irc_msg_notop(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConversation *convo;

	if (!args || !args[1] || !args[2])
		return;

	convo = gaim_find_conversation_with_account(args[1], irc->account);
	if (!convo)
		return;

	gaim_conv_chat_write(GAIM_CONV_CHAT(convo), "", args[2], GAIM_MESSAGE_SYSTEM, time(NULL));
}

void irc_msg_invite(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	GHashTable *components = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
	char *nick = irc_mask_nick(from);

	if (!args || !args[1] || !gc) {
		g_free(nick);
		g_hash_table_destroy(components);
		return;
	}

	g_hash_table_insert(components, strdup("channel"), strdup(args[1]));

	serv_got_chat_invite(gc, args[1], nick, NULL, components);
	g_free(nick);
}

void irc_msg_inviteonly(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	char *buf;

	if (!args || !args[1] || !gc)
		return;

	buf = g_strdup_printf(_("Joining %s requires an invitation."), args[1]);
	gaim_notify_error(gc, _("Invitation only"), _("Invitation only"), buf);
	g_free(buf);
}

void irc_msg_ison(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	char **nicks;
	struct irc_buddy *ib;
	int i;

	if (!args || !args[1])
		return;

	nicks = g_strsplit(args[1], " ", -1);

	for (i = 0; nicks[i]; i++) {
		if ((ib = g_hash_table_lookup(irc->buddies, (gconstpointer)nicks[i])) == NULL) {
			continue;
		}
		ib->flag = TRUE;
	}

	g_strfreev(nicks);

	g_hash_table_foreach(irc->buddies, (GHFunc)irc_buddy_status, (gpointer)irc);
}

static void irc_buddy_status(char *name, struct irc_buddy *ib, struct irc_conn *irc)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	GaimBuddy *buddy = gaim_find_buddy(irc->account, name);

	if (!gc || !buddy)
		return;

	if (ib->online && !ib->flag) {
		serv_got_update(gc, buddy->name, FALSE, 0, 0, 0, 0);
		ib->online = FALSE;
	}

	if (!ib->online && ib->flag) {
		serv_got_update(gc, buddy->name, TRUE, 0, 0, 0, 0);
		ib->online = TRUE;
	}
}

void irc_msg_join(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	GaimConversation *convo;
	char *nick = irc_mask_nick(from), *userhost;
	struct irc_buddy *ib;
	static int id = 1;

	if (!gc) {
		g_free(nick);
		return;
	}

	if (!gaim_utf8_strcasecmp(nick, gaim_connection_get_display_name(gc))) {
		/* We are joining a channel for the first time */
		serv_got_joined_chat(gc, id++, args[0]);
		g_free(nick);
		return;
	}

	convo = gaim_find_conversation_with_account(args[0], irc->account);
	if (convo == NULL) {
		gaim_debug(GAIM_DEBUG_ERROR, "irc", "JOIN for %s failed\n", args[0]);
		g_free(nick);
		return;
	}

	userhost = irc_mask_userhost(from);
	gaim_conv_chat_add_user(GAIM_CONV_CHAT(convo), nick, userhost, GAIM_CBFLAGS_NONE, TRUE);

	if ((ib = g_hash_table_lookup(irc->buddies, nick)) != NULL) {
		ib->flag = TRUE;
		irc_buddy_status(nick, ib, irc);
	}

	g_free(userhost);
	g_free(nick);
}

void irc_msg_kick(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	GaimConversation *convo = gaim_find_conversation_with_account(args[0], irc->account);
	char *nick = irc_mask_nick(from), *buf, *reason;

	if (!gc) {
		g_free(nick);
		return;
	}

	if (!convo) {
		gaim_debug(GAIM_DEBUG_ERROR, "irc", "Recieved a KICK for unknown channel %s\n", args[0]);
		g_free(nick);
		return;
	}

	reason = g_markup_escape_text(args[2], -1);
	if (!gaim_utf8_strcasecmp(gaim_connection_get_display_name(gc), args[1])) {
		buf = g_strdup_printf(_("You have been kicked by %s: (%s)"), nick, reason);
		gaim_conv_chat_write(GAIM_CONV_CHAT(convo), args[0], buf, GAIM_MESSAGE_SYSTEM, time(NULL));
		g_free(buf);
		serv_got_chat_left(gc, gaim_conv_chat_get_id(GAIM_CONV_CHAT(convo)));
	} else {
		buf = g_strdup_printf(_("Kicked by %s (%s)"), nick, reason);
		gaim_conv_chat_remove_user(GAIM_CONV_CHAT(convo), args[1], buf);
		g_free(buf);
	}

	g_free(reason);
	g_free(nick);
	return;
}

void irc_msg_mode(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConversation *convo;
	char *nick = irc_mask_nick(from), *buf;

	if (*args[0] == '#' || *args[0] == '&') {	/* Channel	*/
		char *escaped;
		convo = gaim_find_conversation_with_account(args[0], irc->account);
		if (!convo) {
			gaim_debug(GAIM_DEBUG_ERROR, "irc", "MODE received for %s, which we are not in\n", args[0]);
			g_free(nick);
			return;
		}
		escaped = (args[2] != NULL) ? g_markup_escape_text(args[2], -1) : NULL;
		buf = g_strdup_printf(_("mode (%s %s) by %s"), args[1], escaped ? escaped : "", nick);
		gaim_conv_chat_write(GAIM_CONV_CHAT(convo), args[0], buf, GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG, time(NULL));
		g_free(escaped);
		g_free(buf);
		if(args[2]) {
			GaimConvChatBuddyFlags newflag, flags;
			char *mcur, *cur, *end, *user;
			gboolean add = FALSE;
			mcur = args[1];
			cur = args[2];
			while (*cur && *mcur) {
				if ((*mcur == '+') || (*mcur == '-')) {
					add = (*mcur == '+') ? TRUE : FALSE;
					mcur++;
					continue;
				}
				end = strchr(cur, ' ');
				if (!end)
					end = cur + strlen(cur);
				user = g_strndup(cur, end - cur);
				flags = gaim_conv_chat_user_get_flags(GAIM_CONV_CHAT(convo), user);
				newflag = GAIM_CBFLAGS_NONE;
				if (*mcur == 'o')
					newflag = GAIM_CBFLAGS_OP;
				else if (*mcur =='h')
					newflag = GAIM_CBFLAGS_HALFOP;
				else if (*mcur == 'v')
					newflag = GAIM_CBFLAGS_VOICE;
				if (newflag) {
					if (add)
						flags |= newflag;
					else
						flags &= ~newflag;
					gaim_conv_chat_user_set_flags(GAIM_CONV_CHAT(convo), user, flags);
				}
				g_free(user);
				cur = end;
				if (*cur)
					cur++;
				if (*mcur)
					mcur++;
			}
		}
	} else {					/* User		*/
	}
	g_free(nick);
}

void irc_msg_nick(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	GaimConversation *conv;
	GSList *chats;
	char *nick = irc_mask_nick(from);

	if (!gc) {
		g_free(nick);
		return;
	}
	chats = gc->buddy_chats;

	if (!gaim_utf8_strcasecmp(nick, gaim_connection_get_display_name(gc))) {
		gaim_connection_set_display_name(gc, args[0]);
	}

	while (chats) {
		GaimConvChat *chat = GAIM_CONV_CHAT(chats->data);
		/* This is ugly ... */
		if (gaim_conv_chat_find_user(chat, nick))
			gaim_conv_chat_rename_user(chat, nick, args[0]);
		chats = chats->next;
	}

	conv = gaim_find_conversation_with_account(nick, irc->account);
	if (conv != NULL)
		gaim_conversation_set_name(conv, args[0]);

	g_free(nick);
}

void irc_msg_badnick(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	char *escaped, *msg;

	if (gc && gaim_connection_get_state(gc) == GAIM_CONNECTED) {
		escaped = g_markup_escape_text(args[1], -1);
		msg = g_strdup_printf(_("Invalid nickname '%s'"), escaped);
		gaim_notify_error(irc->account, _("Invalid nickname"),
				  msg,
				  _("Your selected account name was rejected by the server.  It probably contains invalid characters."));
		g_free(escaped);
		g_free(msg);
	} else {
		gaim_connection_error(gc,
				      _("Your selected account name was rejected by the server.  It probably contains invalid characters."));
	}
}

void irc_msg_nickused(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	char *newnick, *buf, *end;

	if (!args || !args[1])
		return;

	newnick = strdup(args[1]);
	end = newnick + strlen(newnick) - 1;
	/* try three fallbacks */
	if (*end == 2) *end = '3';
	else if (*end == 1) *end = '2';
	else *end = '1';

	buf = irc_format(irc, "vn", "NICK", newnick);
	irc_send(irc, buf);
	g_free(buf);
	g_free(newnick);
}

void irc_msg_notice(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	char *newargs[2];

	newargs[0] = " notice ";	/* The spaces are magic, leave 'em in! */
	newargs[1] = args[1];
	irc_msg_privmsg(irc, name, from, newargs);
}

void irc_msg_nochangenick(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	char *msg;

	if (!args || !args[2] || !gc)
		return;

	msg = g_markup_escape_text(args[2], -1);
	gaim_notify_error(gc, _("Cannot change nick"), _("Could not change nick"), msg);
	g_free(msg);
}

void irc_msg_part(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	GaimConversation *convo;
	char *nick, *msg, *escaped;

	if (!args || !args[0] || !gc)
		return;

	convo = gaim_find_conversation_with_account(args[0], irc->account);
	if (!convo) {
		gaim_debug(GAIM_DEBUG_INFO, "irc", "Got a PART on %s, which doesn't exist -- probably closed\n", args[0]);
		return;
	}

	escaped = (args[1] && *args[1]) ? g_markup_escape_text(args[1], -1) : NULL;
	nick = irc_mask_nick(from);
	if (!gaim_utf8_strcasecmp(nick, gaim_connection_get_display_name(gc))) {
		msg = g_strdup_printf(_("You have parted the channel%s%s"),
                                      (args[1] && *args[1]) ? ": " : "",
									  (escaped && *escaped) ? escaped : "");
		gaim_conv_chat_write(GAIM_CONV_CHAT(convo), args[0], msg, GAIM_MESSAGE_SYSTEM, time(NULL));
		g_free(msg);
		serv_got_chat_left(gc, gaim_conv_chat_get_id(GAIM_CONV_CHAT(convo)));
	} else {
		gaim_conv_chat_remove_user(GAIM_CONV_CHAT(convo), nick, escaped);
	}
	g_free(escaped);
	g_free(nick);
}

void irc_msg_ping(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	char *buf;
	if (!args || !args[0])
		return;

	buf = irc_format(irc, "v:", "PONG", args[0]);
	irc_send(irc, buf);
	g_free(buf);
}

void irc_msg_pong(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConversation *convo;
	GaimConnection *gc;
	char **parts, *msg;
	time_t oldstamp;

	if (!args || !args[1])
		return;

	parts = g_strsplit(args[1], " ", 2);

	if (!parts[0] || !parts[1]) {
		g_strfreev(parts);
		return;
	}

	if (sscanf(parts[1], "%lu", &oldstamp) != 1) {
		msg = g_strdup(_("Error: invalid PONG from server"));
	} else {
		msg = g_strdup_printf(_("PING reply -- Lag: %lu seconds"), time(NULL) - oldstamp);
	}

	convo = gaim_find_conversation_with_account(parts[0], irc->account);
	g_strfreev(parts);
	if (convo) {
		if (gaim_conversation_get_type (convo) == GAIM_CONV_CHAT)
			gaim_conv_chat_write(GAIM_CONV_CHAT(convo), "PONG", msg, GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG, time(NULL));
		else
			gaim_conv_im_write(GAIM_CONV_IM(convo), "PONG", msg, GAIM_MESSAGE_SYSTEM|GAIM_MESSAGE_NO_LOG, time(NULL));
	} else {
		gc = gaim_account_get_connection(irc->account);
		if (!gc) {
			g_free(msg);
			return;
		}
		gaim_notify_info(gc, NULL, "PONG", msg);
	}
	g_free(msg);
}

void irc_msg_privmsg(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	GaimConversation *convo;
	char *nick = irc_mask_nick(from), *tmp, *msg;
	int notice = 0;

	if (!args || !args[0] || !args[1] || !gc) {
		g_free(nick);
		return;
	}

	notice = !strcmp(args[0], " notice ");
	tmp = irc_parse_ctcp(irc, nick, args[0], args[1], notice);
	if (!tmp) {
		g_free(nick);
		return;
	}

	msg = gaim_escape_html(tmp);
	g_free(tmp);

	tmp = irc_mirc2html(msg);
	g_free(msg);
	msg = tmp;
	if (notice) {
		tmp = g_strdup_printf("(notice) %s", msg);
		g_free(msg);
		msg = tmp;
	}

	if (!gaim_utf8_strcasecmp(args[0], gaim_connection_get_display_name(gc))) {
		serv_got_im(gc, nick, msg, 0, time(NULL));
	} else if (notice) {
		serv_got_im(gc, nick, msg, 0, time(NULL));
	} else {
		convo = gaim_find_conversation_with_account(args[0], irc->account);
		if (convo)
			serv_got_chat_in(gc, gaim_conv_chat_get_id(GAIM_CONV_CHAT(convo)), nick, 0, msg, time(NULL));
		else
			gaim_debug(GAIM_DEBUG_ERROR, "irc", "Got a PRIVMSG on %s, which does not exist\n", args[0]);
	}
	g_free(msg);
	g_free(nick);
}

void irc_msg_regonly(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	char *msg, *reason;

	if (!args || !args[1] || !args[2] || !gc)
		return;

	msg = g_strdup_printf(_("Cannot join %s:"), args[1]);
	reason = g_markup_escape_text(args[2], -1);
	gaim_notify_error(gc, _("Cannot join channel"), msg, reason);
	g_free(msg);
	g_free(reason);
}

void irc_msg_quit(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	struct irc_buddy *ib;
	char *data[2];

	if (!args || !args[0] || !gc)
		return;

	data[0] = irc_mask_nick(from);
	data[1] = args[0];
	/* XXX this should have an API, I shouldn't grab this directly */
	g_slist_foreach(gc->buddy_chats, (GFunc)irc_chat_remove_buddy, data);

	if ((ib = g_hash_table_lookup(irc->buddies, data[0])) != NULL) {
		ib->flag = FALSE;
		irc_buddy_status(data[0], ib, irc);
	}
	g_free(data[0]);

	return;
}

void irc_msg_wallops(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	GaimConnection *gc = gaim_account_get_connection(irc->account);
	char *nick, *msg, *wallop;

	if (!args || !args[0] || !gc)
		return;

	nick = irc_mask_nick(from);
	msg = g_strdup_printf (_("Wallops from %s"), nick);
	g_free(nick);
	wallop = g_markup_escape_text(args[0], strlen(args[0]));
	gaim_notify_info(gc, NULL, msg, wallop);
	g_free(msg);
	g_free(wallop);
}

void irc_msg_ignore(struct irc_conn *irc, const char *name, const char *from, char **args)
{
	return;
}

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "gaim.h"
#include "prpl.h"
#include "proxy.h"
#include "md5.h"

#include "pixmaps/protocols/msn/msn_online.xpm"
#include "pixmaps/protocols/msn/msn_away.xpm"
#include "pixmaps/protocols/msn/msn_occ.xpm"

/* MSN Smiley Faces */
#include "pixmaps/protocols/msn/msn_angry.xpm"
#include "pixmaps/protocols/msn/msn_bat.xpm"
#include "pixmaps/protocols/msn/msn_beer.xpm"
#include "pixmaps/protocols/msn/msn_boy.xpm"
#include "pixmaps/protocols/msn/msn_brheart.xpm"
#include "pixmaps/protocols/msn/msn_cake.xpm"
#include "pixmaps/protocols/msn/msn_cat.xpm"
#include "pixmaps/protocols/msn/msn_coffee.xpm"
#include "pixmaps/protocols/msn/msn_cry.xpm"
#include "pixmaps/protocols/msn/msn_deadflower.xpm"
#include "pixmaps/protocols/msn/msn_drink.xpm"
#include "pixmaps/protocols/msn/msn_email.xpm"
#include "pixmaps/protocols/msn/msn_flower.xpm"
#include "pixmaps/protocols/msn/msn_gift.xpm"
#include "pixmaps/protocols/msn/msn_girl.xpm"
#include "pixmaps/protocols/msn/msn_handcuffs.xpm"
#include "pixmaps/protocols/msn/msn_heart.xpm"
#include "pixmaps/protocols/msn/msn_hot.xpm"
#include "pixmaps/protocols/msn/msn_icon.xpm"
#include "pixmaps/protocols/msn/msn_idea.xpm"
#include "pixmaps/protocols/msn/msn_kiss.xpm"
#include "pixmaps/protocols/msn/msn_laugh.xpm"
#include "pixmaps/protocols/msn/msn_neutral.xpm"
#include "pixmaps/protocols/msn/msn_note.xpm"
#include "pixmaps/protocols/msn/msn_ooooh.xpm"
#include "pixmaps/protocols/msn/msn_phone.xpm"
#include "pixmaps/protocols/msn/msn_photo.xpm"
#include "pixmaps/protocols/msn/msn_question.xpm"
#include "pixmaps/protocols/msn/msn_run.xpm"
#include "pixmaps/protocols/msn/msn_runback.xpm"
#include "pixmaps/protocols/msn/msn_sad.xpm"
#include "pixmaps/protocols/msn/msn_sleep.xpm"
#include "pixmaps/protocols/msn/msn_smiley.xpm"
#include "pixmaps/protocols/msn/msn_star.xpm"
#include "pixmaps/protocols/msn/msn_tongue.xpm"
#include "pixmaps/protocols/msn/msn_thumbdown.xpm"
#include "pixmaps/protocols/msn/msn_thumbup.xpm"
#include "pixmaps/protocols/msn/msn_weird.xpm"
#include "pixmaps/protocols/msn/msn_embarrassed.xpm"
#include "pixmaps/protocols/msn/msn_angel.xpm"
#include "pixmaps/protocols/msn/msn_film.xpm"
#include "pixmaps/protocols/msn/msn_dog.xpm"
#include "pixmaps/protocols/msn/msn_clock.xpm"
#include "pixmaps/protocols/msn/msn_sun.xpm"
#include "pixmaps/protocols/msn/msn_rainbow.xpm"
#include "pixmaps/protocols/msn/msn_devil.xpm"
#include "pixmaps/protocols/msn/msn_wink.xpm"

#define MSN_BUF_LEN 8192
#define MIME_HEADER	"MIME-Version: 1.0\r\n" \
			"Content-Type: text/plain; charset=UTF-8\r\n" \
			"User-Agent: Gaim/" VERSION "\r\n" \
			"X-MMS-IM-Format: FN=Arial; EF=; CO=0; PF=0\r\n\r\n"

#define HOTMAIL_URL "http://www.hotmail.com/cgi-bin/folders"
#define PASSPORT_URL "http://lc1.law13.hotmail.passport.com/cgi-bin/dologin?login="

#define MSN_ONLINE  1
#define MSN_BUSY    2
#define MSN_IDLE    3
#define MSN_BRB     4
#define MSN_AWAY    5
#define MSN_PHONE   6
#define MSN_LUNCH   7
#define MSN_OFFLINE 8
#define MSN_HIDDEN  9

#define USEROPT_HOTMAIL 0

#define USEROPT_MSNSERVER 3
#define MSN_SERVER "messenger.hotmail.com"
#define USEROPT_MSNPORT 4
#define MSN_PORT 1863

#define MSN_TYPING_RECV_TIMEOUT 6
#define MSN_TYPING_SEND_TIMEOUT	4

			

struct msn_data {
	int fd;
	int trId;
	int inpa;

	char *rxqueue;
	int rxlen;
	gboolean msg;
	char *msguser;
	int msglen;

	GSList *switches;
	GSList *fl;
	GSList *permit;
	GSList *deny;

	char *kv;
	char *sid;
	char *mspauth;
	unsigned long sl;
	char *passport;

};

struct msn_switchboard {
	struct gaim_connection *gc;
	struct conversation *chat;
	int fd;
	int inpa;

	char *rxqueue;
	int rxlen;
	gboolean msg;
	char *msguser;
	int msglen;

	char *sessid;
	char *auth;
	int trId;
	int total;
	char *user;
	GSList *txqueue;
};

struct msn_buddy {
	char *user;
	char *friend;
};

static void msn_login_callback(gpointer, gint, GaimInputCondition);
static void msn_login_xfr_connect(gpointer, gint, GaimInputCondition);

#define GET_NEXT(tmp)	while (*(tmp) && !isspace(*(tmp))) \
				(tmp)++; \
			*(tmp)++ = 0; \
			while (*(tmp) && isspace(*(tmp))) \
				(tmp)++;


static char *msn_normalize(const char *s)
{
	static char buf[BUF_LEN];

	g_return_val_if_fail(s != NULL, NULL);

	g_snprintf(buf, sizeof(buf), "%s%s", s, strchr(s, '@') ? "" : "@hotmail.com");

	return buf;
}

static int msn_write(int fd, void *data, int len)
{
	debug_printf("MSN C: %s", data);
	return write(fd, data, len);
}

static char *url_decode(const char *msg)
{
	static char buf[MSN_BUF_LEN];
	int i, j = 0;

	bzero(buf, sizeof(buf));
	for (i = 0; i < strlen(msg); i++) {
		char hex[3];
		if (msg[i] != '%') {
			buf[j++] = msg[i];
			continue;
		}
		strncpy(hex, msg + ++i, 2); hex[2] = 0;
		/* i is pointing to the start of the number */
		i++; /* now it's at the end and at the start of the for loop
			will be at the next character */
		buf[j++] = strtol(hex, NULL, 16);
	}
	buf[j] = 0;

	return buf;
}

static char *url_encode(const char *msg)
{
	static char buf[MSN_BUF_LEN];
	int i, j = 0;

	bzero(buf, sizeof(buf));
	for (i = 0; i < strlen(msg); i++) {
		if (isalnum(msg[i]))
			buf[j++] = msg[i];
		else {
			sprintf(buf + j, "%%%02x", (unsigned char)msg[i]);
			j += 3;
		}
	}
	buf[j] = 0;

	return buf;
}

static char *handle_errcode(char *buf, gboolean show)
{
	int errcode;
	static char msg[MSN_BUF_LEN];

	buf[4] = 0;
	errcode = atoi(buf);

	switch (errcode) {
		case 200:
			g_snprintf(msg, sizeof(msg), _("Syntax Error (probably a Gaim bug)"));
			break;
		case 201:
			g_snprintf(msg, sizeof(msg), _("Invalid Parameter (probably a Gaim bug)"));
			break;
		case 205:
			g_snprintf(msg, sizeof(msg), _("Invalid User"));
			break;
		case 206:
			g_snprintf(msg, sizeof(msg), _("Fully Qualified Domain Name missing"));
			break;
		case 207:
			g_snprintf(msg, sizeof(msg), _("Already Login"));
			break;
		case 208:
			g_snprintf(msg, sizeof(msg), _("Invalid Username"));
			break;
		case 209:
			g_snprintf(msg, sizeof(msg), _("Invalid Friendly Name"));
			break;
		case 210:
			g_snprintf(msg, sizeof(msg), _("List Full"));
			break;
		case 215:
			g_snprintf(msg, sizeof(msg), _("Already there"));
			break;
		case 216:
			g_snprintf(msg, sizeof(msg), _("Not on list"));
			break;
		case 217:
			g_snprintf(msg, sizeof(msg), _("User is offline"));
			break;
		case 218:
			g_snprintf(msg, sizeof(msg), _("Already in the mode"));
			break;
		case 219:
			g_snprintf(msg, sizeof(msg), _("Already in opposite list"));
			break;
		case 280:
			g_snprintf(msg, sizeof(msg), _("Switchboard failed"));
			break;
		case 281:
			g_snprintf(msg, sizeof(msg), _("Notify Transfer failed"));
			break;

		case 300:
			g_snprintf(msg, sizeof(msg), _("Required fields missing"));
			break;
		case 302:
			g_snprintf(msg, sizeof(msg), _("Not logged in"));
			break;

		case 500:
			g_snprintf(msg, sizeof(msg), _("Internal server error"));
			break;
		case 501:
			g_snprintf(msg, sizeof(msg), _("Database server error"));
			break;
		case 510:
			g_snprintf(msg, sizeof(msg), _("File operation error"));
			break;
		case 520:
			g_snprintf(msg, sizeof(msg), _("Memory allocation error"));
			break;

		case 600:
			g_snprintf(msg, sizeof(msg), _("Server busy"));
			break;
		case 601:
			g_snprintf(msg, sizeof(msg), _("Server unavailable"));
			break;
		case 602:
			g_snprintf(msg, sizeof(msg), _("Peer Notification server down"));
			break;
		case 603:
			g_snprintf(msg, sizeof(msg), _("Database connect error"));
			break;
		case 604:
			g_snprintf(msg, sizeof(msg), _("Server is going down (abandon ship)"));
			break;

		case 707:
			g_snprintf(msg, sizeof(msg), _("Error creating connection"));
			break;
		case 711:
			g_snprintf(msg, sizeof(msg), _("Unable to write"));
			break;
		case 712:
			g_snprintf(msg, sizeof(msg), _("Session overload"));
			break;
		case 713:
			g_snprintf(msg, sizeof(msg), _("User is too active"));
			break;
		case 714:
			g_snprintf(msg, sizeof(msg), _("Too many sessions"));
			break;
		case 715:
			g_snprintf(msg, sizeof(msg), _("Not expected"));
			break;
		case 717:
			g_snprintf(msg, sizeof(msg), _("Bad friend file"));
			break;

		case 911:
			g_snprintf(msg, sizeof(msg), _("Authentication failed"));
			break;
		case 913:
			g_snprintf(msg, sizeof(msg), _("Not allowed when offline"));
			break;
	        case 920:
			g_snprintf(msg, sizeof(msg), _("Not accepting new users"));
			break;
	        case 924:
			g_snprintf(msg, sizeof(msg), _("User unverified"));
			break;
		default:
			g_snprintf(msg, sizeof(msg), _("Unknown Error Code"));
			break;
	}

	if (show)
		do_error_dialog(msg, NULL, GAIM_ERROR);

	return msg;
}

static void handle_hotmail(struct gaim_connection *gc, char *data)
{
	char login_url[2048];
	char buf[MSN_BUF_LEN];
	struct msn_data *md = gc->proto_data;

	if (strchr(gc->username, '@') != strstr(gc->username, "@hotmail.com"))
		/* We can only get Hotmail notification from hotmail users */
		return;

	if (!md->passport) {
		g_snprintf(buf, sizeof(buf), "URL %d INBOX\r\n", ++md->trId);

		if (msn_write(md->fd, buf, strlen(buf)) < 0) {
			return;
		}
	} else {
		g_snprintf(login_url, sizeof(login_url), "%s", md->passport);

		if (strstr(data, "Content-Type: text/x-msmsgsinitialemailnotification;")) {
			char *x = strstr(data, "Inbox-Unread:");
			if (!x) return;
			x += strlen("Inbox-Unread: ");
			connection_has_mail(gc, atoi(x), NULL, NULL, login_url);
		} else if (strstr(data, "Content-Type: text/x-msmsgsemailnotification;")) {
			char *from = strstr(data, "From:");
			char *subject = strstr(data, "Subject:");
			char *x;
			if (!from || !subject) {
				connection_has_mail(gc, 1, NULL, NULL, login_url);
				return;
			}
			from += strlen("From: ");
			x = strstr(from, "\r\n"); *x = 0;
			subject += strlen("Subject: ");
			x = strstr(subject, "\r\n"); *x = 0;
			from = utf8_to_str(from);
			connection_has_mail(gc, -1, from, subject, login_url);
			g_free(from);
		}
	}
}

static struct msn_switchboard *msn_find_switch(struct gaim_connection *gc, char *id)
{
	struct msn_data *md = gc->proto_data;
	GSList *m = md->switches;

	while (m) {
		struct msn_switchboard *ms = m->data;
		m = m->next;
		if ((ms->total <= 1) && !g_strcasecmp(ms->user, id))
			return ms;
	}

	return NULL;
}

static struct msn_switchboard *msn_find_switch_by_id(struct gaim_connection *gc, int id)
{
	struct msn_data *md = gc->proto_data;
	GSList *m = md->switches;

	while (m) {
		struct msn_switchboard *ms = m->data;
		m = m->next;
		if (ms->chat && (ms->chat->id == id))
			return ms;
	}

	return NULL;
}

static struct msn_switchboard *msn_find_writable_switch(struct gaim_connection *gc)
{
	struct msn_data *md = gc->proto_data;
	GSList *m = md->switches;

	while (m) {
		struct msn_switchboard *ms = m->data;
		m = m->next;
		if (ms->txqueue)
			return ms;
	}

	return NULL;
}

static void msn_kill_switch(struct msn_switchboard *ms)
{
	struct gaim_connection *gc = ms->gc;
	struct msn_data *md = gc->proto_data;

	if (ms->inpa)
		gaim_input_remove(ms->inpa);
	close(ms->fd);
	g_free(ms->rxqueue);
	if (ms->msg)
		g_free(ms->msguser);
	if (ms->sessid)
		g_free(ms->sessid);
	g_free(ms->auth);
	if (ms->user)
		g_free(ms->user);
	while (ms->txqueue) {
		g_free(ms->txqueue->data);
		ms->txqueue = g_slist_remove(ms->txqueue, ms->txqueue->data);
	}
	if (ms->chat)
		serv_got_chat_left(gc, ms->chat->id);

	md->switches = g_slist_remove(md->switches, ms);

	g_free(ms);
}

static int msn_process_switch(struct msn_switchboard *ms, char *buf)
{
	struct gaim_connection *gc = ms->gc;
	char sendbuf[MSN_BUF_LEN];
	static int id = 0;

	if (!g_strncasecmp(buf, "ACK", 3)) {
	} else if (!g_strncasecmp(buf, "ANS", 3)) {
		if (ms->chat)
			add_chat_buddy(ms->chat, gc->username);
	} else if (!g_strncasecmp(buf, "BYE", 3)) {
		if (ms->chat) {
			char *user, *tmp = buf;
			GET_NEXT(tmp);
			user = tmp;
			remove_chat_buddy(ms->chat, user, NULL);
		} else {
			msn_kill_switch(ms);
			return 0;
		}
	} else if (!g_strncasecmp(buf, "CAL", 3)) {
	} else if (!g_strncasecmp(buf, "IRO", 3)) {
		char *tot, *user, *tmp = buf;

		GET_NEXT(tmp);
		GET_NEXT(tmp);
		GET_NEXT(tmp);
		tot = tmp;
		GET_NEXT(tmp);
		ms->total = atoi(tot);
		user = tmp;
		GET_NEXT(tmp);

		if (ms->total > 1) {
			if (!ms->chat)
				ms->chat = serv_got_joined_chat(gc, ++id, "MSN Chat");
			add_chat_buddy(ms->chat, user);
		} 
	} else if (!g_strncasecmp(buf, "JOI", 3)) {
		char *user, *tmp = buf;
		GET_NEXT(tmp);
		user = tmp;
		GET_NEXT(tmp);

		if (ms->total == 1) {
			ms->chat = serv_got_joined_chat(gc, ++id, "MSN Chat");
			add_chat_buddy(ms->chat, ms->user);
			add_chat_buddy(ms->chat, gc->username);
			g_free(ms->user);
			ms->user = NULL;
		}
		if (ms->chat)
			add_chat_buddy(ms->chat, user);
		ms->total++;
		while (ms->txqueue) {
			char *send = add_cr(ms->txqueue->data);
			char *utf8 = str_to_utf8(send);
			g_free(send);
			g_snprintf(sendbuf, sizeof(sendbuf), "MSG %d N %d\r\n%s%s", ++ms->trId,
					strlen(MIME_HEADER) + strlen(utf8),
					MIME_HEADER, utf8);
			g_free(utf8);
			g_free(ms->txqueue->data);
			ms->txqueue = g_slist_remove(ms->txqueue, ms->txqueue->data);
			if (msn_write(ms->fd, sendbuf, strlen(sendbuf)) < 0) {
				msn_kill_switch(ms);
				return 0;
			}
			debug_printf("\n");
		}
	} else if (!g_strncasecmp(buf, "MSG", 3)) {
		char *user, *tmp = buf;
		int length;

		GET_NEXT(tmp);
		user = tmp;

		GET_NEXT(tmp);

		GET_NEXT(tmp);
		length = atoi(tmp);

		ms->msg = TRUE;
		ms->msguser = g_strdup(user);
		ms->msglen = length;
	} else if (!g_strncasecmp(buf, "NAK", 3)) {
		do_error_dialog(_("An MSN message may not have been received."), NULL, GAIM_ERROR);
	} else if (!g_strncasecmp(buf, "NLN", 3)) {
	} else if (!g_strncasecmp(buf, "OUT", 3)) {
		if (ms->chat)
			serv_got_chat_left(gc, ms->chat->id);
		msn_kill_switch(ms);
		return 0;
	} else if (!g_strncasecmp(buf, "USR", 3)) {
		/* good, we got USR, now we need to find out who we want to talk to */
		struct msn_switchboard *ms = msn_find_writable_switch(gc);

		if (!ms)
			return 0;

		g_snprintf(sendbuf, sizeof(sendbuf), "CAL %d %s\r\n", ++ms->trId, ms->user);
		if (msn_write(ms->fd, sendbuf, strlen(sendbuf)) < 0) {
			msn_kill_switch(ms);
			return 0;
		}
	} else if (isdigit(*buf)) {
		handle_errcode(buf, TRUE);
	} else {
		debug_printf("Unhandled message!\n");
	}

	return 1;
}

static char *msn_parse_format(char *mime)
{
	char *cur;
	GString *ret = g_string_new(NULL);
	guint colorbuf;
	char *colors = (char *)(&colorbuf);
	

	cur = strstr(mime, "FN=");
	if (cur && (*(cur = cur + 3) != ';')) {
		ret = g_string_append(ret, "<FONT FACE=\"");
		while (*cur && *cur != ';') {
			ret = g_string_append_c(ret, *cur);
			cur++;
		}
		ret = g_string_append(ret, "\">");
	}
	
	cur = strstr(mime, "EF=");
	if (cur && (*(cur = cur + 3) != ';')) {
		while (*cur && *cur != ';') {
			ret = g_string_append_c(ret, '<');
			ret = g_string_append_c(ret, *cur);
			ret = g_string_append_c(ret, '>');
			cur++;
		}
	}
	
	cur = strstr(mime, "CO=");
	if (cur && (*(cur = cur + 3) != ';')) {
		if (sscanf (cur, "%x;", &colorbuf) == 1) {
			char tag[MSN_BUF_LEN];
			g_snprintf(tag, sizeof(tag), "<FONT COLOR=\"#%02hhx%02hhx%02hhx\">", colors[0], colors[1], colors[2]);
			ret = g_string_append(ret, tag);
		}
	}
	
	cur = url_decode(ret->str);
	g_string_free(ret, TRUE);
	return cur;
}
			
	
static void msn_process_switch_msg(struct msn_switchboard *ms, char *msg)
{
	char *content, *agent, *format, *utf;
	char *message = NULL;
	int flags = 0;

	agent = strstr(msg, "User-Agent: ");
	if (agent) {
		if (!g_strncasecmp(agent, "User-Agent: Gaim", strlen("User-Agent: Gaim")))
			flags |= IM_FLAG_GAIMUSER;
	}

	format = strstr(msg, "X-MMS-IM-Format: ");
	if (format) {
		format = msn_parse_format(format);
	} else {
		format = NULL;
	}

	content = strstr(msg, "Content-Type: ");
	if (!content)
		return;
	if (!g_strncasecmp(content, "Content-Type: text/x-msmsgscontrol\r\n",
			   strlen(  "Content-Type: text/x-msmsgscontrol\r\n"))) {
		if (strstr(content,"TypingUser: ") && !ms->chat) {
			serv_got_typing(ms->gc, ms->msguser, MSN_TYPING_RECV_TIMEOUT);
			return;
		} 

	} else if (!g_strncasecmp(content, "Content-Type: text/plain",
				  strlen("Content-Type: text/plain"))) {
		
	
		
		char *skiphead;
		skiphead = strstr(msg, "\r\n\r\n");
		if (!skiphead || !skiphead[4]) {
			return;
		}
		skiphead += 4;
		utf = utf8_to_str(skiphead);
		strip_linefeed(utf);
		
		if (format) { 
			int len = strlen(utf) + strlen(format) + 1;
			message = g_malloc(len);
			g_snprintf(message, len, "%s%s", format, utf);
		} else {
			message = utf;
		}
		
		if (ms->chat)
			serv_got_chat_in(ms->gc, ms->chat->id, ms->msguser, flags, message, time(NULL));
		else
			serv_got_im(ms->gc, ms->msguser, message, flags, time(NULL), -1);

		g_free(message);
		if (format) {
			g_free(utf);
		}
	}
}

static void msn_switchboard_callback(gpointer data, gint source, GaimInputCondition cond)
{
	struct msn_switchboard *ms = data;
	char buf[MSN_BUF_LEN];
	int cont = 1;
	int len;
	
	/* This is really stupid and I hate to put this here. */
	if (ms->fd != source)
		ms->fd = source;

	len = read(ms->fd, buf, sizeof(buf));

	if (len <= 0) {
		msn_kill_switch(ms);
		return;
	}

	ms->rxqueue = g_realloc(ms->rxqueue, len + ms->rxlen);
	memcpy(ms->rxqueue + ms->rxlen, buf, len);
	ms->rxlen += len;

	while (cont) {
		if (!ms->rxlen)
			return;

		if (ms->msg) {
			char *msg;
			if (ms->msglen > ms->rxlen)
				return;
			msg = ms->rxqueue;
			ms->rxlen -= ms->msglen;
			if (ms->rxlen) {
				ms->rxqueue = g_memdup(msg + ms->msglen, ms->rxlen);
			} else {
				ms->rxqueue = NULL;
				msg = g_realloc(msg, ms->msglen + 1);
			}
			msg[ms->msglen] = 0;
			ms->msglen = 0;
			ms->msg = FALSE;

			msn_process_switch_msg(ms, msg);

			g_free(ms->msguser);
			g_free(msg);
		} else {
			char *end = ms->rxqueue;
			int cmdlen;
			char *cmd;
			int i = 0;

			while (i + 1 < ms->rxlen) {
				if (*end == '\r' && end[1] == '\n')
					break;
				end++; i++;
			}
			if (i + 1 == ms->rxlen)
				return;

			cmdlen = end - ms->rxqueue + 2;
			cmd = ms->rxqueue;
			ms->rxlen -= cmdlen;
			if (ms->rxlen) {
				ms->rxqueue = g_memdup(cmd + cmdlen, ms->rxlen);
			} else {
				ms->rxqueue = NULL;
				cmd = g_realloc(cmd, cmdlen + 1);
			}
			cmd[cmdlen] = 0;

			debug_printf("MSN S: %s", cmd);
			g_strchomp(cmd);
			cont = msn_process_switch(ms, cmd);

			g_free(cmd);
		}
	}
}

static void msn_rng_connect(gpointer data, gint source, GaimInputCondition cond)
{
	struct msn_switchboard *ms = data;
	struct gaim_connection *gc = ms->gc;
	struct msn_data *md;
	char buf[MSN_BUF_LEN];

	if (source == -1 || !g_slist_find(connections, gc)) {
		close(source);
		g_free(ms->sessid);
		g_free(ms->auth);
		g_free(ms);
		return;
	}

	md = gc->proto_data;

	if (ms->fd != source)
		ms->fd = source;

	g_snprintf(buf, sizeof(buf), "ANS %d %s %s %s\r\n", ++ms->trId, gc->username, ms->auth, ms->sessid);
	if (msn_write(ms->fd, buf, strlen(buf)) < 0) {
		close(ms->fd);
		g_free(ms->sessid);
		g_free(ms->auth);
		g_free(ms);
		return;
	}

	md->switches = g_slist_append(md->switches, ms);
	ms->inpa = gaim_input_add(ms->fd, GAIM_INPUT_READ, msn_switchboard_callback, ms);
}

static void msn_ss_xfr_connect(gpointer data, gint source, GaimInputCondition cond)
{
	struct msn_switchboard *ms = data;
	struct gaim_connection *gc = ms->gc;
	char buf[MSN_BUF_LEN];

	if (source == -1 || !g_slist_find(connections, gc)) {
		close(source);
		if (g_slist_find(connections, gc)) {
			msn_kill_switch(ms);
			do_error_dialog(_("Gaim was unable to send an MSN message"),
					_("Gaim encountered an error communicating with the "
					  "MSN switchboard server.  Please try again later."), GAIM_ERROR);
		}
		return;
	}

	if (ms->fd != source)
		ms->fd = source;

	g_snprintf(buf, sizeof(buf), "USR %d %s %s\r\n", ++ms->trId, gc->username, ms->auth);
	if (msn_write(ms->fd, buf, strlen(buf)) < 0) {
		g_free(ms->auth);
		g_free(ms);
		return;
	}

	ms->inpa = gaim_input_add(ms->fd, GAIM_INPUT_READ, msn_switchboard_callback, ms);
}

struct msn_add_permit {
	struct gaim_connection *gc;
	char *user;
	char *friend;
};

static void msn_accept_add(gpointer w, struct msn_add_permit *map)
{
	struct msn_data *md = map->gc->proto_data;
	char buf[MSN_BUF_LEN];
	char *srvfriend;
	
	srvfriend = str_to_utf8(map->friend);
	g_snprintf(buf, sizeof(buf), "ADD %d AL %s %s\r\n", ++md->trId, map->user, url_encode(srvfriend));
	g_free(srvfriend);
	
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(map->gc, "Write error");
		signoff(map->gc);
		return;
	}
	map->gc->permit = g_slist_append(map->gc->permit, map->user);
	build_allow_list(); /* er. right. we'll need to have a thing for this in CUI too */
	show_got_added(map->gc, NULL, map->user, map->friend, NULL);
	*(map->user) = 0;
}

static void msn_cancel_add(gpointer w, struct msn_add_permit *map)
{
	struct msn_data *md = map->gc->proto_data;
	char buf[MSN_BUF_LEN];
	char *srvfriend = str_to_utf8(map->friend);

	if (*(map->user)) {
		g_snprintf(buf, sizeof(buf), "ADD %d BL %s %s\r\n", ++md->trId, map->user, url_encode(srvfriend));
		if (msn_write(md->fd, buf, strlen(buf)) < 0) {
			hide_login_progress(map->gc, "Write error");
			signoff(map->gc);
			return;
		}
		map->gc->deny = g_slist_append(map->gc->deny, map->user);
		build_block_list();
	}
	
	g_free(srvfriend);
	g_free(map->user);
	g_free(map->friend);
	g_free(map);
}

static int msn_process_main(struct gaim_connection *gc, char *buf)
{
	struct msn_data *md = gc->proto_data;
	char sendbuf[MSN_BUF_LEN];

	if (!g_strncasecmp(buf, "ADD", 3)) {
		char *list, *user, *friend, *tmp = buf;
		struct msn_add_permit *ap;
		GSList *perm = gc->permit;
		char msg[MSN_BUF_LEN];

		GET_NEXT(tmp);
		GET_NEXT(tmp);
		list = tmp;

		GET_NEXT(tmp);
		GET_NEXT(tmp);
		user = tmp;

		GET_NEXT(tmp);
		friend = url_decode(tmp);

		if (g_strcasecmp(list, "RL"))
			return 1;

		while (perm) {
			if (!g_strcasecmp(perm->data, user))
				return 1;
			perm = perm->next;
		}

		ap = g_new0(struct msn_add_permit, 1);
		ap->user = g_strdup(user);
		ap->friend = utf8_to_str(friend);
		ap->gc = gc;

		g_snprintf(msg, sizeof(msg), _("The user %s (%s) wants to add %s to his or her buddy list."),
				ap->user, ap->friend, ap->gc->username);

		do_ask_dialog(msg, ap, msn_accept_add, msn_cancel_add);
	} else if (!g_strncasecmp(buf, "BLP", 3)) {
		char *type, *tmp = buf;

		GET_NEXT(tmp);
		GET_NEXT(tmp);
		GET_NEXT(tmp);
		type = tmp;

		if (!g_strcasecmp(type, "AL")) {
			/* If the current setting is AL, messages
			 * from users who are not in BL will be delivered
			 *
			 * In other words, deny some */
			gc->permdeny = DENY_SOME;
		} else {
			/* If the current
			 * setting is BL, only messages from people who are in the AL will be
			 * delivered.
			 * 
			 * In other words, permit some */
			gc->permdeny = PERMIT_SOME;
		}
	} else if (!g_strncasecmp(buf, "BPR", 3)) {
	} else if (!g_strncasecmp(buf, "CHG", 3)) {
	} else if (!g_strncasecmp(buf, "CHL", 3)) {
		char *hash = buf;
		char buf2[MSN_BUF_LEN];
		md5_state_t st;
		md5_byte_t di[16];
		int i;

		GET_NEXT(hash);
		GET_NEXT(hash);

		md5_init(&st);
		md5_append(&st, (const md5_byte_t *)hash, strlen(hash));
		md5_append(&st, (const md5_byte_t *)"Q1P7W2E4J9R8U3S5", strlen("Q1P7W2E4J9R8U3S5"));
		md5_finish(&st, di);

		g_snprintf(sendbuf, sizeof(sendbuf), "QRY %d msmsgs@msnmsgr.com 32\r\n", ++md->trId);
		for (i = 0; i < 16; i++) {
			g_snprintf(buf2, sizeof(buf2), "%02x", di[i]);
			strcat(sendbuf, buf2);
		}

		if (msn_write(md->fd, sendbuf, strlen(sendbuf)) < 0) {
			hide_login_progress(gc, "Unable to write to server");
			signoff(gc);
		}

		debug_printf("\n");
	} else if (!g_strncasecmp(buf, "FLN", 3)) {
		char *usr = buf;

		GET_NEXT(usr);
		serv_got_update(gc, usr, 0, 0, 0, 0, 0, 0);
	} else if (!g_strncasecmp(buf, "GTC", 3)) {
	} else if (!g_strncasecmp(buf, "INF", 3)) {
	} else if (!g_strncasecmp(buf, "ILN", 3)) {
		char *state, *user, *friend, *tmp = buf;
		struct buddy *b;
		int status = 0;

		GET_NEXT(tmp);

		GET_NEXT(tmp);
		state = tmp;

		GET_NEXT(tmp);
		user = tmp;

		GET_NEXT(tmp);
		friend = url_decode(tmp);

		if ((b = find_buddy(gc, user)) != NULL) {
			if (b->proto_data)
				g_free(b->proto_data);
			b->proto_data = utf8_to_str(friend);
		}

		if (!g_strcasecmp(state, "BSY")) {
			status |= UC_UNAVAILABLE | (MSN_BUSY << 1);
		} else if (!g_strcasecmp(state, "IDL")) {
			status |= UC_UNAVAILABLE | (MSN_IDLE << 1);
		} else if (!g_strcasecmp(state, "BRB")) {
			status |= UC_UNAVAILABLE | (MSN_BRB << 1);
		} else if (!g_strcasecmp(state, "AWY")) {
			status |= UC_UNAVAILABLE | (MSN_AWAY << 1);
		} else if (!g_strcasecmp(state, "PHN")) {
			status |= UC_UNAVAILABLE | (MSN_PHONE << 1);
		} else if (!g_strcasecmp(state, "LUN")) {
			status |= UC_UNAVAILABLE | (MSN_LUNCH << 1);
		}

		serv_got_update(gc, user, 1, 0, 0, 0, status, 0);
	} else if (!g_strncasecmp(buf, "LST", 3)) {
		char *which, *who, *friend, *tmp = buf;
		struct msn_add_permit *ap; /* for any as yet undealt with buddies who've added you to their buddy list when you were off-line.  How dare they! */
		GSList *perm = gc->permit; /* current permit list */
		GSList *denyl = gc->deny;
		char msg[MSN_BUF_LEN];
		int new = 1;
		int pos, tot;

		GET_NEXT(tmp);
		GET_NEXT(tmp);
		which = tmp;

		GET_NEXT(tmp);
		GET_NEXT(tmp);
		pos = strtol(tmp, NULL, 10);
		GET_NEXT(tmp);
		tot = strtol(tmp, NULL, 10);
		GET_NEXT(tmp);
		who = tmp;

		GET_NEXT(tmp);
		friend = url_decode(tmp);

		if (!g_strcasecmp(which, "FL") && pos) {
			struct msn_buddy *b = g_new0(struct msn_buddy, 1);
			b->user = g_strdup(who);
			b->friend = utf8_to_str(friend);
			md->fl = g_slist_append(md->fl, b);
		} else if (!g_strcasecmp(which, "AL") && pos) {
			char *dupl;
			if ((dupl = (char *)g_slist_find_custom(gc->deny, who, 
							(GCompareFunc)strcmp))) {
				debug_printf("moving from deny to permit: %s", who);
				gc->deny = g_slist_remove(gc->deny, dupl);
			}
			gc->permit = g_slist_append(gc->permit, g_strdup(who));
		} else if (!g_strcasecmp(which, "BL") && pos) {
			gc->deny = g_slist_append(gc->deny, g_strdup(who));
		} else if (!g_strcasecmp(which, "RL")) {
		    if (pos) {
			while(perm) {
				if(!g_strcasecmp(perm->data, who))
					new = 0;
				perm = perm->next;
			}
			while(denyl) {
			  if(!g_strcasecmp(denyl->data, who))
			    new = 0;
			  denyl = denyl->next;
			}
			if(new) {
				debug_printf("Unresolved MSN RL entry\n");
				ap = g_new0(struct msn_add_permit, 1);
				ap->user = g_strdup(who);
				ap->friend = utf8_to_str(friend);
				ap->gc = gc;
                         
		                g_snprintf(msg, sizeof(msg), _("The user %s (%s) wants to add you to their buddy list"),ap->user, ap->friend);
				do_ask_dialog(msg, ap, msn_accept_add, msn_cancel_add);
			}
		    }
			
			if (pos != tot) 
				return 1; /* this isn't the last one in the RL, so return. */

			g_snprintf(sendbuf, sizeof(sendbuf), "CHG %d NLN\r\n", ++md->trId);
			if (msn_write(md->fd, sendbuf, strlen(sendbuf)) < 0) {
				hide_login_progress(gc, "Unable to write");
				signoff(gc);
				return 0;
			}

			account_online(gc);
			serv_finish_login(gc);

			md->permit = g_slist_copy(gc->permit);
			md->deny = g_slist_copy(gc->deny);

			if (bud_list_cache_exists(gc))
				do_import(gc, NULL);
			else {
				g_snprintf(sendbuf, sizeof(sendbuf), "BLP %d AL\r\n", ++md->trId);
				if (msn_write(md->fd, sendbuf, strlen(sendbuf)) < 0) {
					hide_login_progress(gc, "Unable to write");
					signoff(gc);
					return 0;
				}
			}
			while (md->fl) {
				struct msn_buddy *mb = md->fl->data;
				struct buddy *b;
				md->fl = g_slist_remove(md->fl, mb);
				if (!(b = find_buddy(gc, mb->user)))
					add_buddy(gc, _("Buddies"), mb->user, mb->friend);
				else if (!g_strcasecmp(b->name, b->show)) {
					g_snprintf(b->show, sizeof(b->show), "%s", mb->friend);
					handle_buddy_rename(b, b->name);
				}
				g_free(mb->user);
				g_free(mb->friend);
				g_free(mb);
			}
		}
	} else if (!g_strncasecmp(buf, "MSG", 3)) {
		char *user, *tmp = buf;
		int length;

		GET_NEXT(tmp);
		user = tmp;

		GET_NEXT(tmp);

		GET_NEXT(tmp);
		length = atoi(tmp);

		md->msg = TRUE;
		md->msguser = g_strdup(user);
		md->msglen = length;
	} else if (!g_strncasecmp(buf, "NLN", 3)) {
		char *state, *user, *friend, *tmp = buf;
		struct buddy *b;
		int status = 0;

		GET_NEXT(tmp);
		state = tmp;

		GET_NEXT(tmp);
		user = tmp;

		GET_NEXT(tmp);
		friend = url_decode(tmp);

		if ((b = find_buddy(gc, user)) != NULL) {
			if (b->proto_data)
				g_free(b->proto_data);
			b->proto_data = utf8_to_str(friend);
		}

		if (!g_strcasecmp(state, "BSY")) {
			status |= UC_UNAVAILABLE | (MSN_BUSY << 1);
		} else if (!g_strcasecmp(state, "IDL")) {
			status |= UC_UNAVAILABLE | (MSN_IDLE << 1);
		} else if (!g_strcasecmp(state, "BRB")) {
			status |= UC_UNAVAILABLE | (MSN_BRB << 1);
		} else if (!g_strcasecmp(state, "AWY")) {
			status |= UC_UNAVAILABLE | (MSN_AWAY << 1);
		} else if (!g_strcasecmp(state, "PHN")) {
			status |= UC_UNAVAILABLE | (MSN_PHONE << 1);
		} else if (!g_strcasecmp(state, "LUN")) {
			status |= UC_UNAVAILABLE | (MSN_LUNCH << 1);
		}

		serv_got_update(gc, user, 1, 0, 0, 0, status, 0);
	} else if (!g_strncasecmp(buf, "OUT", 3)) {
		char *tmp = buf;

		GET_NEXT(tmp);
		if (!g_strncasecmp(tmp, "OTH", 3)) {
			hide_login_progress(gc, _("You have been disconnected. You have "
						  "signed on from another location."));
			signoff(gc);
			return 0;
		}
	} else if (!g_strncasecmp(buf, "PRP", 3)) {
	} else if (!g_strncasecmp(buf, "QNG", 3)) {
	} else if (!g_strncasecmp(buf, "QRY", 3)) {
	} else if (!g_strncasecmp(buf, "REA", 3)) {
		char *friend, *tmp = buf;

		GET_NEXT(tmp);
		GET_NEXT(tmp);
		GET_NEXT(tmp);
		GET_NEXT(tmp);

		friend = url_decode(tmp);
		friend = utf8_to_str(friend);

		g_snprintf(gc->displayname, sizeof(gc->displayname), "%s", friend);
		g_free(friend);
	} else if (!g_strncasecmp(buf, "REM", 3)) {
	} else if (!g_strncasecmp(buf, "RNG", 3)) {
		struct msn_switchboard *ms;
		char *sessid, *ssaddr, *auth, *user;
		int port, i = 0;
		char *tmp = buf;

		GET_NEXT(tmp);
		sessid = tmp;

		GET_NEXT(tmp);
		ssaddr = tmp;

		GET_NEXT(tmp);

		GET_NEXT(tmp);
		auth = tmp;

		GET_NEXT(tmp);
		user = tmp;
		GET_NEXT(tmp);

		while (ssaddr[i] && ssaddr[i] != ':') i++;
		if (ssaddr[i] == ':') {
			char *x = &ssaddr[i + 1];
			ssaddr[i] = 0;
			port = atoi(x);
		} else
			port = 1863;

		ms = g_new0(struct msn_switchboard, 1);
		ms->fd = proxy_connect(ssaddr, port, msn_rng_connect, ms);
		if (ms->fd < 0) {
			g_free(ms);
			return 1;
		}
		ms->user = g_strdup(user);
		ms->sessid = g_strdup(sessid);
		ms->auth = g_strdup(auth);
		ms->gc = gc;
	} else if (!g_strncasecmp(buf, "URL", 3)) {
		char *tmp = buf;
		FILE *fd;
		md5_state_t st;
		md5_byte_t di[16];
		int i;
		char buf2[64];
		char sendbuf[64];
		char hippy[2048];
		char *rru;
		char *passport;

		GET_NEXT(tmp);
		GET_NEXT(tmp);
		rru = tmp;
		GET_NEXT(tmp);
		passport = tmp;
		
		snprintf(hippy, sizeof(hippy), "%s%lu%s", md->mspauth, time(NULL) - md->sl, gc->password);

		md5_init(&st);
		md5_append(&st, (const md5_byte_t *)hippy, strlen(hippy));
		md5_finish(&st, di);

		bzero(sendbuf, sizeof(sendbuf));
		for (i = 0; i < 16; i++) {
			g_snprintf(buf2, sizeof(buf2), "%02x", di[i]);
			strcat(sendbuf, buf2);
		}

		if (md->passport) {
			unlink(md->passport);
			g_free(md->passport);
		}

		fd = gaim_mkstemp(&(md->passport));
		fprintf(fd, "<html>\n");
		fprintf(fd, "<head>\n");
		fprintf(fd, "<noscript>\n");
		fprintf(fd, "<meta http-equiv=Refresh content=\"0; url=http://www.hotmail.com\">\n");
		fprintf(fd, "</noscript>\n");
		fprintf(fd, "</head>\n\n");
		
		fprintf(fd, "<body onload=\"document.pform.submit(); \">\n");
		fprintf(fd, "<form name=\"pform\" action=\"%s\" method=\"POST\">\n\n", passport);
		fprintf(fd, "<input type=\"hidden\" name=\"mode\" value=\"ttl\">\n");
		fprintf(fd, "<input type=\"hidden\" name=\"login\" value=\"%s\">\n", gc->username);
		fprintf(fd, "<input type=\"hidden\" name=\"username\" value=\"%s\">\n", gc->username);
		fprintf(fd, "<input type=\"hidden\" name=\"sid\" value=\"%s\">\n", md->sid);
		fprintf(fd, "<input type=\"hidden\" name=\"kv\" value=\"%s\">\n", md->kv);
		fprintf(fd, "<input type=\"hidden\" name=\"id\" value=\"2\">\n");
		fprintf(fd, "<input type=\"hidden\" name=\"sl\" value=\"%ld\">\n", time(NULL) - md->sl);
		fprintf(fd, "<input type=\"hidden\" name=\"rru\" value=\"%s\">\n", rru);
		fprintf(fd, "<input type=\"hidden\" name=\"auth\" value=\"%s\">\n", md->mspauth);
		fprintf(fd, "<input type=\"hidden\" name=\"creds\" value=\"%s\">\n", sendbuf); // Digest me
		fprintf(fd, "<input type=\"hidden\" name=\"svc\" value=\"mail\">\n");
		fprintf(fd, "<input type=\"hidden\" name=\"js\" value=\"yes\">\n");
		fprintf(fd, "</form></body>\n");
		fprintf(fd, "</html>\n");
		fclose(fd);

	} else if (!g_strncasecmp(buf, "SYN", 3)) {
	} else if (!g_strncasecmp(buf, "USR", 3)) {
	} else if (!g_strncasecmp(buf, "XFR", 3)) {
		char *host = strstr(buf, "SB");
		int port;
		int i = 0;
		gboolean switchboard = TRUE;
		char *tmp;

		if (!host) {
			host = strstr(buf, "NS");
			if (!host) {
				hide_login_progress(gc, "Got invalid XFR\n");
				signoff(gc);
				return 0;
			}
			switchboard = FALSE;
		}

		GET_NEXT(host);
		while (host[i] && host[i] != ':') i++;
		if (host[i] == ':') {
			tmp = &host[i + 1];
			host[i] = 0;
			while (isdigit(*tmp)) tmp++;
			*tmp++ = 0;
			port = atoi(&host[i + 1]);
		} else {
			port = 1863;
			tmp = host;
			GET_NEXT(tmp);
		}

		if (switchboard) {
			struct msn_switchboard *ms = msn_find_writable_switch(gc);
			if (!ms)
				return 1;

			GET_NEXT(tmp);

			ms->fd = proxy_connect(host, port, msn_ss_xfr_connect, ms);
			if (ms->fd < 0) {
				msn_kill_switch(ms);
				return 1;
			}
			ms->auth = g_strdup(tmp);
		} else {
			close(md->fd);
			gaim_input_remove(md->inpa);
			md->inpa = 0;
			md->fd = proxy_connect(host, port, msn_login_xfr_connect, gc);
			if (md->fd < 0) {
				hide_login_progress(gc, "Error transfering");
				signoff(gc);
				return 0;
			}
		}
	} else if (isdigit(*buf)) {
		handle_errcode(buf, TRUE);
	} else {
		debug_printf("Unhandled message!\n");
	}

	return 1;
}

static void msn_process_main_msg(struct gaim_connection *gc, char *msg)
{
	struct msn_data *md = gc->proto_data;
	char *skiphead, *utf;
	char *content;

	content = strstr(msg, "Content-Type: ");

	if ((content) && (!g_strncasecmp(content, "Content-Type: text/x-msmsgsprofile",
				strlen("Content-Type: text/x-msmsgsprofile")))) {

		char *kv,*sid,*mspauth;

		kv = strstr(msg, "kv: ");
		sid = strstr(msg, "sid: ");
		mspauth = strstr(msg, "MSPAuth: ");

		if (kv) {
			char *tmp;

			kv += strlen("kv: ");
			tmp = strstr(kv, "\r\n"); *tmp = 0;
			md->kv = g_strdup(kv);
		}

		if (sid) {
			char *tmp;

			sid += strlen("sid: ");
			tmp = strstr(sid, "\r\n"); *tmp = 0;
			md->sid = g_strdup(sid);
		}

		if (mspauth) {
			char *tmp;

			mspauth += strlen("MSPAuth: ");
			tmp = strstr(mspauth, "\r\n"); *tmp = 0;
			md->mspauth = g_strdup(mspauth);
		}

	}



	if (!g_strcasecmp(md->msguser, "hotmail")) {
		handle_hotmail(gc, msg);
		return;
	}


	skiphead = strstr(msg, "\r\n\r\n");
	if (!skiphead || !skiphead[4])
		return;
	skiphead += 4;
	utf = utf8_to_str(skiphead);
	strip_linefeed(utf);

	serv_got_im(gc, md->msguser, utf, 0, time(NULL), -1);

	g_free(utf);
}

static void msn_callback(gpointer data, gint source, GaimInputCondition cond)
{
	struct gaim_connection *gc = data;
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];
	int cont = 1;
	int len;

	len = read(md->fd, buf, sizeof(buf));

	if (len <= 0) {
		hide_login_progress_error(gc, _("Error reading from server"));
		signoff(gc);
		return;
	}

	md->rxqueue = g_realloc(md->rxqueue, len + md->rxlen);
	memcpy(md->rxqueue + md->rxlen, buf, len);
	md->rxlen += len;

	while (cont) {
		if (!md->rxlen)
			return;

		if (md->msg) {
			char *msg;
			if (md->msglen > md->rxlen)
				return;
			msg = md->rxqueue;
			md->rxlen -= md->msglen;
			if (md->rxlen) {
				md->rxqueue = g_memdup(msg + md->msglen, md->rxlen);
			} else {
				md->rxqueue = NULL;
				msg = g_realloc(msg, md->msglen + 1);
			}
			msg[md->msglen] = 0;
			md->msglen = 0;
			md->msg = FALSE;

			msn_process_main_msg(gc, msg);

			g_free(md->msguser);
			g_free(msg);
		} else {
			char *end = md->rxqueue;
			int cmdlen;
			char *cmd;
			int i = 0;

			while (i + 1 < md->rxlen) {
				if (*end == '\r' && end[1] == '\n')
					break;
				end++; i++;
			}
			if (i + 1 == md->rxlen)
				return;

			cmdlen = end - md->rxqueue + 2;
			cmd = md->rxqueue;
			md->rxlen -= cmdlen;
			if (md->rxlen) {
				md->rxqueue = g_memdup(cmd + cmdlen, md->rxlen);
			} else {
				md->rxqueue = NULL;
				cmd = g_realloc(cmd, cmdlen + 1);
			}
			cmd[cmdlen] = 0;

			debug_printf("MSN S: %s", cmd);
			g_strchomp(cmd);
			cont = msn_process_main(gc, cmd);

			g_free(cmd);
		}
	}
}

static void msn_login_xfr_connect(gpointer data, gint source, GaimInputCondition cond)
{
	struct gaim_connection *gc = data;
	struct msn_data *md;
	char buf[MSN_BUF_LEN];

	if (!g_slist_find(connections, gc)) {
		close(source);
		return;
	}

	md = gc->proto_data;

	if (md->fd != source)
		md->fd = source;

	if (md->fd == -1) {
		hide_login_progress(gc, "Unable to connect to Notification Server");
		signoff(gc);
		return;
	}

	g_snprintf(buf, sizeof(buf), "VER %d MSNP5\r\n", ++md->trId);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Unable to talk to Notification Server");
		signoff(gc);
		return;
	}

	md->inpa = gaim_input_add(md->fd, GAIM_INPUT_READ, msn_login_callback, gc);
}

static int msn_process_login(struct gaim_connection *gc, char *buf)
{
	struct msn_data *md = gc->proto_data;
	char sendbuf[MSN_BUF_LEN];

	if (!g_strncasecmp(buf, "VER", 3)) {
		/* we got VER, check to see that MSNP5 is in the list, then send INF */
		if (!strstr(buf, "MSNP5")) {
			hide_login_progress(gc, "Protocol not supported");
			signoff(gc);
			return 0;
		}

		g_snprintf(sendbuf, sizeof(sendbuf), "INF %d\r\n", ++md->trId);
		if (msn_write(md->fd, sendbuf, strlen(sendbuf)) < 0) {
			hide_login_progress(gc, "Unable to request INF\n");
			signoff(gc);
			return 0;
		}
	} else if (!g_strncasecmp(buf, "INF", 3)) {
		/* check to make sure we can use md5 */
		if (!strstr(buf, "MD5")) {
			hide_login_progress(gc, "Unable to login using MD5");
			signoff(gc);
			return 0;
		}

		g_snprintf(sendbuf, sizeof(sendbuf), "USR %d MD5 I %s\r\n", ++md->trId, gc->username);
		if (msn_write(md->fd, sendbuf, strlen(sendbuf)) < 0) {
			hide_login_progress(gc, "Unable to send USR\n");
			signoff(gc);
			return 0;
		}

		set_login_progress(gc, 3, _("Requesting to send password"));
	} else if (!g_strncasecmp(buf, "USR", 3)) {
		char *resp, *friend, *tmp = buf;

		GET_NEXT(tmp);
		GET_NEXT(tmp);
		resp = tmp;
		GET_NEXT(tmp);
		GET_NEXT(tmp);
		friend = url_decode(tmp);
		GET_NEXT(tmp);

		/* so here, we're either getting the challenge or the OK */
		if (!g_strcasecmp(resp, "OK")) {
			friend = utf8_to_str(friend);
			g_snprintf(gc->displayname, sizeof(gc->displayname), "%s", friend);
			g_free(friend);

			g_snprintf(sendbuf, sizeof(sendbuf), "SYN %d 0\r\n", ++md->trId);
			if (msn_write(md->fd, sendbuf, strlen(sendbuf)) < 0) {
				hide_login_progress(gc, "Unable to write");
				signoff(gc);
				return 0;
			}

			gaim_input_remove(md->inpa);
			md->inpa = gaim_input_add(md->fd, GAIM_INPUT_READ, msn_callback, gc);
			return 0;
		} else if (!g_strcasecmp(resp, "MD5")) {
			char buf2[MSN_BUF_LEN];
			md5_state_t st;
			md5_byte_t di[16];
			int i;

			friend = utf8_to_str(friend);
			g_snprintf(buf2, sizeof(buf2), "%s%s", friend, gc->password);
			g_free(friend);

			md5_init(&st);
			md5_append(&st, (const md5_byte_t *)buf2, strlen(buf2));
			md5_finish(&st, di);

			g_snprintf(sendbuf, sizeof(sendbuf), "USR %d MD5 S ", ++md->trId);
			for (i = 0; i < 16; i++) {
				g_snprintf(buf2, sizeof(buf2), "%02x", di[i]);
				strcat(sendbuf, buf2);
			}
			strcat(sendbuf, "\r\n");

			if (msn_write(md->fd, sendbuf, strlen(sendbuf)) < 0) {
				hide_login_progress(gc, _("Unable to send password"));
				signoff(gc);
				return 0;
			}

			set_login_progress(gc, 4, _("Password sent"));
		}
	} else if (!g_strncasecmp(buf, "XFR", 3)) {
		char *host = strstr(buf, "NS");
		int port;
		int i = 0;

		if (!host) {
			hide_login_progress(gc, "Got invalid XFR\n");
			signoff(gc);
			return 0;
		}

		GET_NEXT(host);
		while (host[i] && host[i] != ':') i++;
		if (host[i] == ':') {
			char *x = &host[i + 1];
			host[i] = 0;
			port = atoi(x);
		} else
			port = 1863;

		close(md->fd);
		gaim_input_remove(md->inpa);
		md->inpa = 0;
		md->fd = 0;
		md->fd = proxy_connect(host, port, msn_login_xfr_connect, gc);
		md->sl = time(NULL);
		if (md->fd < 0) {
			hide_login_progress(gc, "Unable to transfer");
			signoff(gc);
		}
		return 0;
	} else {
		if (isdigit(*buf))
			hide_login_progress(gc, handle_errcode(buf, FALSE));
		else
			hide_login_progress(gc, "Unable to parse message");
		signoff(gc);
		return 0;
	}

	return 1;
}

static void msn_login_callback(gpointer data, gint source, GaimInputCondition cond)
{
	struct gaim_connection *gc = data;
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];
	int cont = 1;
	int len;

	len = read(md->fd, buf, sizeof(buf));

	if (len <= 0) {
		hide_login_progress(gc, _("Error reading from server"));
		signoff(gc);
		return;
	}

	md->rxqueue = g_realloc(md->rxqueue, len + md->rxlen);
	memcpy(md->rxqueue + md->rxlen, buf, len);
	md->rxlen += len;

	while (cont) {
		char *end = md->rxqueue;
		int cmdlen;
		char *cmd;
		int i = 0;

		if (!md->rxlen)
			return;

		while (i + 1 < md->rxlen) {
			if (*end == '\r' && end[1] == '\n')
				break;
			end++; i++;
		}
		if (i + 1 == md->rxlen)
			return;

		cmdlen = end - md->rxqueue + 2;
		cmd = md->rxqueue;
		md->rxlen -= cmdlen;
		if (md->rxlen) {
			md->rxqueue = g_memdup(cmd + cmdlen, md->rxlen);
		} else {
			md->rxqueue = NULL;
			cmd = g_realloc(cmd, cmdlen + 1);
		}
		cmd[cmdlen] = 0;

		debug_printf("MSN S: %s", cmd);
		g_strchomp(cmd);
		cont = msn_process_login(gc, cmd);

		g_free(cmd);
	}
}

static void msn_login_connect(gpointer data, gint source, GaimInputCondition cond)
{
	struct gaim_connection *gc = data;
	struct msn_data *md;
	char buf[1024];

	if (!g_slist_find(connections, gc)) {
		close(source);
		return;
	}

	md = gc->proto_data;

	if (md->fd != source)
		md->fd = source;

	if (md->fd == -1) {
		hide_login_progress(gc, _("Unable to connect"));
		signoff(gc);
		return;
	}

	g_snprintf(buf, sizeof(buf), "VER %d MSNP5\r\n", ++md->trId);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, _("Unable to write to server"));
		signoff(gc);
		return;
	}

	md->inpa = gaim_input_add(md->fd, GAIM_INPUT_READ, msn_login_callback, gc);
	set_login_progress(gc, 2,_("Synching with server"));
}

static void msn_login(struct aim_user *user)
{
	struct gaim_connection *gc = new_gaim_conn(user);
	struct msn_data *md = gc->proto_data = g_new0(struct msn_data, 1);

	set_login_progress(gc, 1, _("Connecting"));

	g_snprintf(gc->username, sizeof(gc->username), "%s", msn_normalize(gc->username));
	
	md->fd = proxy_connect(user->proto_opt[USEROPT_MSNSERVER][0] ? user->proto_opt[USEROPT_MSNSERVER] : MSN_SERVER, 
			       user->proto_opt[USEROPT_MSNPORT][0] ? atoi(user->proto_opt[USEROPT_MSNPORT]) : MSN_PORT,
			       msn_login_connect, gc);
	if (md->fd < 0) {
		hide_login_progress(gc, _("Unable to connect"));
		signoff(gc);
	}
}

static void msn_close(struct gaim_connection *gc)
{
	struct msn_data *md = gc->proto_data;
	close(md->fd);
	if (md->inpa)
		gaim_input_remove(md->inpa);
	g_free(md->rxqueue);
	if (md->msg)
		g_free(md->msguser);
	if (md->passport) {
		unlink(md->passport);
      		g_free(md->passport);
	}
	while (md->switches)
		msn_kill_switch(md->switches->data);
	while (md->fl) {
		struct msn_buddy *tmp = md->fl->data;
		md->fl = g_slist_remove(md->fl, tmp);
		g_free(tmp->user);
		g_free(tmp->friend);
		g_free(tmp);
	}
	g_slist_free(md->permit);
	g_slist_free(md->deny);
	g_free(md);
}

static int msn_send_typing(struct gaim_connection *gc, char *who, int typing) {
	struct msn_switchboard *ms = msn_find_switch(gc, who);
	char header[MSN_BUF_LEN] =   "MIME-Version: 1.0\r\n"
				     "Content-Type: text/x-msmsgscontrol\r\n" 
				     "TypingUser: ";
	char buf [MSN_BUF_LEN];
	if (!ms || !typing)
		return 0;
	g_snprintf(buf, sizeof(buf), "MSG %d N %d\r\n%s%s\r\n\r\n\r\n",
		   ++ms->trId,
		   strlen(header) + strlen("\r\n\r\n\r\n") + strlen(gc->username),
		   header, gc->username);
	if (msn_write(ms->fd, buf, strlen(buf)) < 0)
	           msn_kill_switch(ms);
	return MSN_TYPING_SEND_TIMEOUT;
}

static int msn_send_im(struct gaim_connection *gc, char *who, char *message, int len, int flags)
{
	struct msn_data *md = gc->proto_data;
	struct msn_switchboard *ms = msn_find_switch(gc, who);
	char buf[MSN_BUF_LEN];

	if (ms) {
		char *utf8, *send;

		if (ms->txqueue) {
			debug_printf("appending to queue\n");
			ms->txqueue = g_slist_append(ms->txqueue, g_strdup(message));
			return 1;
		}

		send = add_cr(message);
		utf8 = str_to_utf8(send);
		g_free(send);
		g_snprintf(buf, sizeof(buf), "MSG %d N %d\r\n%s%s", ++ms->trId,
				strlen(MIME_HEADER) + strlen(utf8),
				MIME_HEADER, utf8);
		g_free(utf8);
		if (msn_write(ms->fd, buf, strlen(buf)) < 0)
			msn_kill_switch(ms);
		debug_printf("\n");
	} else if (strcmp(who, gc->username)) {
		g_snprintf(buf, MSN_BUF_LEN, "XFR %d SB\r\n", ++md->trId);
		if (msn_write(md->fd, buf, strlen(buf)) < 0) {
			hide_login_progress(gc, "Write error");
			signoff(gc);
			return 1;
		}

		ms = g_new0(struct msn_switchboard, 1);
		md->switches = g_slist_append(md->switches, ms);
		ms->user = g_strdup(who);
		ms->txqueue = g_slist_append(ms->txqueue, g_strdup(message));
		ms->gc = gc;
		ms->fd = -1;
	} else
		/* in msn you can't send messages to yourself, so we'll fake like we received it ;) */
		serv_got_im(gc, who, message, flags | IM_FLAG_GAIMUSER, time(NULL), -1);
	return 1;
}

static int msn_chat_send(struct gaim_connection *gc, int id, char *message)
{
	struct msn_switchboard *ms = msn_find_switch_by_id(gc, id);
	char buf[MSN_BUF_LEN];
	char *utf8, *send;

	if (!ms)
		return -EINVAL;

	send = add_cr(message);
	utf8 = str_to_utf8(send);
	g_free(send);
	g_snprintf(buf, sizeof(buf), "MSG %d N %d\r\n%s%s", ++ms->trId,
			strlen(MIME_HEADER) + strlen(utf8),
			MIME_HEADER, utf8);
	g_free(utf8);
	if (msn_write(ms->fd, buf, strlen(buf)) < 0) {
		msn_kill_switch(ms);
		return 0;
	}
	debug_printf("\n");
	serv_got_chat_in(gc, id, gc->username, 0, message, time(NULL));
	return 0;
}

static void msn_chat_invite(struct gaim_connection *gc, int id, const char *msg, const char *who)
{
	struct msn_switchboard *ms = msn_find_switch_by_id(gc, id);
	char buf[MSN_BUF_LEN];

	if (!ms)
		return;

	g_snprintf(buf, sizeof(buf), "CAL %d %s\r\n", ++ms->trId, who);
	if (msn_write(ms->fd, buf, strlen(buf)) < 0)
		msn_kill_switch(ms);
}

static void msn_chat_leave(struct gaim_connection *gc, int id)
{
	struct msn_switchboard *ms = msn_find_switch_by_id(gc, id);
	char buf[MSN_BUF_LEN];

	if (!ms)
		return;

	g_snprintf(buf, sizeof(buf), "OUT\r\n");
	if (msn_write(ms->fd, buf, strlen(buf)) < 0)
		msn_kill_switch(ms);
}

static GList *msn_away_states(struct gaim_connection *gc)
{
	GList *m = NULL;

	m = g_list_append(m, _("Available"));
	m = g_list_append(m, _("Away From Computer"));
	m = g_list_append(m, _("Be Right Back"));
	m = g_list_append(m, _("Busy"));
	m = g_list_append(m, _("On The Phone"));
	m = g_list_append(m, _("Out To Lunch"));
	m = g_list_append(m, _("Hidden"));

	return m;
}

static void msn_set_away(struct gaim_connection *gc, char *state, char *msg)
{
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];
	char *away;

	gc->away = NULL;

	if (msg) {
		gc->away = "";
		away = "AWY";
	} else if (state) {
		gc->away = "";

		if (!strcmp(state, _("Away From Computer")))
			away = "AWY";
		else if (!strcmp(state, _("Be Right Back")))
			away = "BRB";
		else if (!strcmp(state, _("Busy")))
			away = "BSY";
		else if (!strcmp(state, _("On The Phone")))
			away = "PHN";
		else if (!strcmp(state, _("Out To Lunch")))
			away = "LUN";
		else if (!strcmp(state, _("Hidden")))
			away = "HDN";
		else {
			gc->away = NULL;
			away = "NLN";
		}
	} else if (gc->is_idle)
		away = "IDL";
	else
		away = "NLN";

	g_snprintf(buf, sizeof(buf), "CHG %d %s\r\n", ++md->trId, away);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
}

static void msn_set_idle(struct gaim_connection *gc, int idle)
{
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];

	if (gc->away)
		return;
	if (idle)
		g_snprintf(buf, sizeof(buf), "CHG %d IDL\r\n", ++md->trId);
	else
		g_snprintf(buf, sizeof(buf), "CHG %d NLN\r\n", ++md->trId);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
}

static char **msn_list_icon(int uc)
{
	if (uc == 0)
		return msn_online_xpm;
	
	uc >>= 1;
	
	if (uc == 2 || uc == 6)
		return msn_occ_xpm;
	
	return msn_away_xpm;
}

static char *msn_get_away_text(int s)
{
	switch (s) {
		case MSN_BUSY :
			return "Busy";
		case MSN_BRB :
			return "Be right back";
		case MSN_AWAY :
			return "Away from the computer";
		case MSN_PHONE :
			return "On the phone";
		case MSN_LUNCH :
			return "Out to lunch";
		case MSN_IDLE :
			return "Idle";
		default:
			return "Available";
	}
}

static void msn_reset_friend(struct gaim_connection *gc, char *who)
{
	struct buddy *b = find_buddy(gc, who);
	if (!b || !b->proto_data)
		return;
	g_snprintf(b->show, sizeof(b->show), "%s", (char *)b->proto_data);
	handle_buddy_rename(b, b->name);
}

static GList *msn_buddy_menu(struct gaim_connection *gc, char *who)
{
	GList *m = NULL;
	struct proto_buddy_menu *pbm;
	struct buddy *b = find_buddy(gc, who);
	static char buf[MSN_BUF_LEN];

	pbm = g_new0(struct proto_buddy_menu, 1);
	pbm->label = _("Reset friendly name");
	pbm->callback = msn_reset_friend;
	pbm->gc = gc;
	m = g_list_append(m, pbm);

	if (!b || !(b->uc >> 1))
		return m;

	pbm = g_new0(struct proto_buddy_menu, 1);
	g_snprintf(buf, sizeof(buf), _("Status: %s"), msn_get_away_text(b->uc >> 1));
	pbm->label = buf;
	pbm->callback = NULL;
	pbm->gc = gc;
	m = g_list_append(m, pbm);

	return m;
}

static void msn_add_buddy(struct gaim_connection *gc, const char *name)
{
	struct msn_data *md = gc->proto_data;
	char *who = msn_normalize(name);
	char buf[MSN_BUF_LEN];
	GSList *l = md->fl;

	if (who[0] == '@')
		/* how did this happen? */
		return;

	if (strchr(who, ' '))
		/* This is a broken blist entry. */
		return;

	while (l) {
		struct msn_buddy *b = l->data;
		if (!g_strcasecmp(who, b->user))
			break;
		l = l->next;
	}
	if (l)
		return;

	g_snprintf(buf, sizeof(buf), "ADD %d FL %s %s\r\n", ++md->trId, who, who);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
}

static void msn_rem_buddy(struct gaim_connection *gc, char *who, char *group)
{
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];

	g_snprintf(buf, sizeof(buf), "REM %d FL %s\r\n", ++md->trId, who);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
}

static void msn_act_id(gpointer data, char *entry)
{
	struct gaim_connection *gc = data;
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];
	char *alias;

	if (!entry || *entry == '\0') 
		alias = g_strdup("");
	else
		alias = str_to_utf8(entry);
	
	if (strlen(alias) >= BUDDY_ALIAS_MAXLEN) {
		do_error_dialog(_("New MSN friendly name too long."), NULL, GAIM_ERROR);
		return;
	}
	
	g_snprintf(buf, sizeof(buf), "REA %d %s %s\r\n", ++md->trId, gc->username, url_encode(alias));
	g_free(alias);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
}

static void msn_do_action(struct gaim_connection *gc, char *act)
{
	if (!strcmp(act, _("Set Friendly Name"))) {
		do_prompt_dialog(_("Set Friendly Name:"), gc->displayname, gc, msn_act_id, NULL);
	} else if (!strcmp(act, _("Reset All Friendly Names"))) {
		GSList *g = gc->groups;
		while (g) {
			GSList *m = ((struct group *)g->data)->members;
			while (m) {
				struct buddy *b = m->data;
				if (b->present)
					msn_reset_friend(gc, b->name);
				m = m->next;
			}
			g = g->next;
		}
	}
}

static GList *msn_actions()
{
	GList *m = NULL;

	m = g_list_append(m, _("Set Friendly Name"));
	m = g_list_append(m, "Reset All Friendly Names");

	return m;
}

static void msn_convo_closed(struct gaim_connection *gc, char *who)
{
	struct msn_switchboard *ms = msn_find_switch(gc, who);

	if (ms)
		msn_kill_switch(ms);
}

static void msn_keepalive(struct gaim_connection *gc)
{
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];

	g_snprintf(buf, sizeof(buf), "PNG\r\n");
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
}

static void msn_set_permit_deny(struct gaim_connection *gc)
{
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];
	GSList *s, *t = NULL;

	if (gc->permdeny == PERMIT_ALL || gc->permdeny == DENY_SOME)
		g_snprintf(buf, sizeof(buf), "BLP %d AL\r\n", ++md->trId);
	else
		g_snprintf(buf, sizeof(buf), "BLP %d BL\r\n", ++md->trId);

	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}

	/* this is safe because we'll always come here after we've gotten the list off the server,
	 * and data is never removed. So if the lengths are equal we don't know about anyone locally
	 * and so there's no sense in going through them all. */
	if (g_slist_length(gc->permit) == g_slist_length(md->permit)) {
		g_slist_free(md->permit);
		md->permit = NULL;
	}
	if (g_slist_length(gc->deny) == g_slist_length(md->deny)) {
		g_slist_free(md->deny);
		md->deny = NULL;
	}
	if (!md->permit && !md->deny)
		return;

	if (md->permit) {
		s = g_slist_nth(gc->permit, g_slist_length(md->permit));
		while (s) {
			char *who = s->data;
			char *dupl;
			s = s->next;
			if (!strchr(who, '@')) {
				t = g_slist_append(t, who);
				continue;
			}
			if ((dupl = (char *)g_slist_find(md->deny, who))) {
				t = g_slist_append(t, who);
				continue;
			}
			g_snprintf(buf, sizeof(buf), "ADD %d AL %s %s\r\n", ++md->trId, who, who);
			if (msn_write(md->fd, buf, strlen(buf)) < 0) {
				hide_login_progress(gc, "Write error");
				signoff(gc);
				return;
			}
		}
		while (t) {
			char *who = t->data;
			gc->permit = g_slist_remove(gc->permit, who);
			g_free(who);
			t = t->next;
		}
		if (t)
			g_slist_free(t);
	t = NULL;
	g_slist_free(md->permit);
	md->permit = NULL;
	}
	
	if (md->deny) {
		s = g_slist_nth(gc->deny, g_slist_length(md->deny));
		while (s) {
			char *who = s->data;
			char *dupl;
			s = s->next;
			if (!strchr(who, '@')) {
				t = g_slist_append(t, who);
				continue;
			}
			if ((dupl = (char *)g_slist_find(md->deny, who))) {
				t = g_slist_append(t, who);
				continue;
			}
			g_snprintf(buf, sizeof(buf), "ADD %d BL %s %s\r\n", ++md->trId, who, who);
			if (msn_write(md->fd, buf, strlen(buf)) < 0) {
				hide_login_progress(gc, "Write error");
				signoff(gc);
				return;
			}
		}
		while (t) {
			char *who = t->data;
			gc->deny = g_slist_remove(gc->deny, who);
			g_free(who);
			t = t->next;
		}
		if (t)
			g_slist_free(t);
	g_slist_free(md->deny);
	md->deny = NULL;
	}
}

static void msn_add_permit(struct gaim_connection *gc, char *who)
{
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];
	char *dupl;

	if (!strchr(who, '@')) {
		g_snprintf(buf, sizeof(buf), 
			   _("An MSN screenname must be in the form \"user@server.com\".  "
			     "Perhaps you meant %s@hotmail.com.  No changes were made to your "
			     "allow list."), who);
		do_error_dialog(_("Invalid MSN screenname"), buf, GAIM_ERROR);
		gc->permit = g_slist_remove(gc->permit, who);
		g_free(who);
		return;
	}

	if ((dupl = (char *)g_slist_find_custom(gc->deny, who, 
					(GCompareFunc)strcmp))) {
		debug_printf("MSN: Moving %s from BL to AL\n", who);
		gc->deny = g_slist_remove(gc->deny, dupl);
		g_snprintf(buf, sizeof(buf), "REM %d BL %s\r\n", ++md->trId, who);
			if (msn_write(md->fd, buf, strlen(buf)) < 0) {
				hide_login_progress(gc, "Write error");
				signoff(gc);
				return;
			}
	}
	g_snprintf(buf, sizeof(buf), "ADD %d AL %s %s\r\n", ++md->trId, who, who);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
}

static void msn_rem_permit(struct gaim_connection *gc, char *who)
{
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];

	g_snprintf(buf, sizeof(buf), "REM %d AL %s\r\n", ++md->trId, who);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
	
	g_slist_append(gc->deny, who);
	g_snprintf(buf, sizeof(buf), "ADD %d BL %s %s\r\n", ++md->trId, who, who);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
}

static void msn_add_deny(struct gaim_connection *gc, char *who)
{
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];
	char *dupl;
	
	if (!strchr(who, '@')) {
		g_snprintf(buf, sizeof(buf), 
			   _("An MSN screenname must be in the form \"user@server.com\".  "
			     "Perhaps you meant %s@hotmail.com.  No changes were made to your "
			     "block list."), who);
		do_error_dialog(_("Invalid MSN screenname"), buf, GAIM_ERROR);
		gc->deny = g_slist_remove(gc->deny, who);
		g_free(who);
		return;
	}

	if ((dupl = (char *)g_slist_find_custom(gc->permit, who, 
					(GCompareFunc)strcmp))) {
		debug_printf("MSN: Moving %s from AL to BL\n", who);
		gc->permit = g_slist_remove(gc->permit, dupl);
		g_snprintf(buf, sizeof(buf), "REM %d AL %s\r\n", ++md->trId, who);
		if (msn_write(md->fd, buf, strlen(buf)) < 0) {
			hide_login_progress(gc, "Write error");
			signoff(gc);
			return;
		}
	}
		

	g_snprintf(buf, sizeof(buf), "ADD %d BL %s %s\r\n", ++md->trId, who, who);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
}

static void msn_rem_deny(struct gaim_connection *gc, char *who)
{
	struct msn_data *md = gc->proto_data;
	char buf[MSN_BUF_LEN];

	g_snprintf(buf, sizeof(buf), "REM %d BL %s\r\n", ++md->trId, who);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
	
	g_slist_append(gc->permit, who);
	g_snprintf(buf, sizeof(buf), "ADD %d AL %s %s\r\n", ++md->trId, who, who);
	if (msn_write(md->fd, buf, strlen(buf)) < 0) {
		hide_login_progress(gc, "Write error");
		signoff(gc);
		return;
	}
}

static void msn_buddy_free(struct buddy *b)
{
	if (b->proto_data)
		g_free(b->proto_data);
}

GSList *msn_smiley_list() 
{ 
	GSList *smilies = NULL;

	smilies = add_smiley(smilies, "(a)", msn_angel, 1);
	smilies = add_smiley(smilies, "(A)", msn_angel, 0);
	smilies = add_smiley(smilies, ":-@", msn_angry, 1);
	smilies = add_smiley(smilies, ":@", msn_angry, 0);
	smilies = add_smiley(smilies, ":-[", msn_bat, 1);
	smilies = add_smiley(smilies, ":[", msn_bat, 0);
	smilies = add_smiley(smilies, "(B)", msn_beer, 1);
	smilies = add_smiley(smilies, "(b)", msn_beer, 0);
	smilies = add_smiley(smilies, "(Z)", msn_boy, 1);
	smilies = add_smiley(smilies, "(z)", msn_boy, 0);
	smilies = add_smiley(smilies, "(U)", msn_brheart, 1);
	smilies = add_smiley(smilies, "(u)", msn_brheart, 0);
	smilies = add_smiley(smilies, "(@)", msn_cat, 1);
	smilies = add_smiley(smilies, "(^)", msn_cake, 1);
	smilies = add_smiley(smilies, "(o)", msn_clock, 1);
	smilies = add_smiley(smilies, "(O)", msn_clock, 0);
	smilies = add_smiley(smilies, "(C)", msn_coffee, 1);
	smilies = add_smiley(smilies, "(c)", msn_coffee, 0);
	smilies = add_smiley(smilies, ":'(", msn_cry, 1);
	smilies = add_smiley(smilies, ":`(", msn_cry, 0);
	smilies = add_smiley(smilies, "(W)", msn_deadflower, 1);
	smilies = add_smiley(smilies, "(w)", msn_deadflower, 0);
	smilies = add_smiley(smilies, "(6)", msn_devil, 1);
	smilies = add_smiley(smilies, "(&)", msn_dog, 1);
	smilies = add_smiley(smilies, "(D)", msn_drink, 1);
	smilies = add_smiley(smilies, "(d)", msn_drink, 0);
	smilies = add_smiley(smilies, "(E)", msn_email, 1);
	smilies = add_smiley(smilies, "(e)", msn_email, 0);
	smilies = add_smiley(smilies, "(~)", msn_film, 1);
	smilies = add_smiley(smilies, "(F)", msn_flower, 1);
	smilies = add_smiley(smilies, "(f)", msn_flower, 0);
	smilies = add_smiley(smilies, "(G)", msn_gift, 1);
	smilies = add_smiley(smilies, "(g)", msn_gift, 0);
	smilies = add_smiley(smilies, "(X)", msn_girl, 1);
	smilies = add_smiley(smilies, "(x)", msn_girl, 0);
	smilies = add_smiley(smilies, "(%)", msn_handcuffs, 1);
	smilies = add_smiley(smilies, "(L)", msn_heart, 1);
	smilies = add_smiley(smilies, "(l)", msn_heart, 0);
	smilies = add_smiley(smilies, "(H)", msn_hot, 1);
	smilies = add_smiley(smilies, "(h)", msn_hot, 0);
	smilies = add_smiley(smilies, "(M)", msn_icon, 1);
	smilies = add_smiley(smilies, "(m)", msn_icon, 0);
	smilies = add_smiley(smilies, "(I)", msn_idea, 1);
	smilies = add_smiley(smilies, "(i)", msn_idea, 0);
	smilies = add_smiley(smilies, "(K)", msn_kiss, 1);
	smilies = add_smiley(smilies, "(k)", msn_kiss, 0);
	smilies = add_smiley(smilies, ":-D", msn_laugh, 1);
	smilies = add_smiley(smilies, ":D", msn_laugh, 0);
	smilies = add_smiley(smilies, ":-d", msn_laugh, 0);
	smilies = add_smiley(smilies, ":d", msn_laugh, 0);
	smilies = add_smiley(smilies, ":->", msn_laugh, 0);
	smilies = add_smiley(smilies, ":>", msn_laugh, 0);
	smilies = add_smiley(smilies, ":-|", msn_neutral, 1);
	smilies = add_smiley(smilies, ":|", msn_neutral, 0);
	smilies = add_smiley(smilies, "(8)", msn_note, 1);
	smilies = add_smiley(smilies, ":-O", msn_ooooh, 1);
	smilies = add_smiley(smilies, ":O", msn_ooooh, 0);
	smilies = add_smiley(smilies, ":-o", msn_ooooh, 0);
	smilies = add_smiley(smilies, ":o", msn_ooooh, 0);
	smilies = add_smiley(smilies, "(T)", msn_phone, 1);
	smilies = add_smiley(smilies, "(t)", msn_phone, 0);
	smilies = add_smiley(smilies, "(P)", msn_photo, 1);
	smilies = add_smiley(smilies, "(p)", msn_photo, 0);
	smilies = add_smiley(smilies, "(?)", msn_question, 1);
	smilies = add_smiley(smilies, "(r)", msn_rainbow, 1);
	smilies = add_smiley(smilies, "(R)", msn_rainbow, 0);
	smilies = add_smiley(smilies, "({)", msn_run, 1);
	smilies = add_smiley(smilies, "(})", msn_runback, 1);
	smilies = add_smiley(smilies, ":-(", msn_sad, 1);
	smilies = add_smiley(smilies, ":(", msn_sad, 0);
	smilies = add_smiley(smilies, ":-<", msn_sad, 0);
	smilies = add_smiley(smilies, "(S)", msn_sleep, 1);
	smilies = add_smiley(smilies, "(s)", msn_sleep, 0);
	smilies = add_smiley(smilies, ":-)", msn_smiley, 1);
	smilies = add_smiley(smilies, ":)", msn_smiley, 0);
	smilies = add_smiley(smilies, "(*)", msn_star, 1);
	smilies = add_smiley(smilies, "(#)", msn_sun, 1);
	smilies = add_smiley(smilies, "(N)", msn_thumbdown, 1);
	smilies = add_smiley(smilies, "(n)", msn_thumbdown, 0);
	smilies = add_smiley(smilies, "(Y)", msn_thumbup, 1);
	smilies = add_smiley(smilies, "(y)", msn_thumbup, 0);
	smilies = add_smiley(smilies, ":-P", msn_tongue, 1);
	smilies = add_smiley(smilies, ":P", msn_tongue, 0);
	smilies = add_smiley(smilies, ":-p", msn_tongue, 0);
	smilies = add_smiley(smilies, ":p", msn_tongue, 0);
	smilies = add_smiley(smilies, ":-S", msn_weird, 1);
	smilies = add_smiley(smilies, ":S", msn_weird, 0);
	smilies = add_smiley(smilies, ":-s", msn_weird, 0);
	smilies = add_smiley(smilies, ":s", msn_weird, 0);
	smilies = add_smiley(smilies, ";-)", msn_wink, 1);
	smilies = add_smiley(smilies, ";)", msn_wink, 0);
	smilies = add_smiley(smilies, ":S-", msn_wink, 0);
	smilies = add_smiley(smilies, ":-$", msn_embarrassed, 1);
	smilies = add_smiley(smilies, ":$", msn_embarrassed, 0);
	
	return smilies;
}

static struct prpl *my_protocol = NULL;

void msn_init(struct prpl *ret)
{
	struct proto_user_opt *puo;
	ret->protocol = PROTO_MSN;
	ret->options = OPT_PROTO_MAIL_CHECK;
	ret->name = g_strdup("MSN");
	ret->list_icon = msn_list_icon;
	ret->buddy_menu = msn_buddy_menu;
	ret->login = msn_login;
	ret->close = msn_close;
	ret->send_im = msn_send_im;
	ret->send_typing = msn_send_typing;
	ret->away_states = msn_away_states;
	ret->set_away = msn_set_away;
	ret->set_idle = msn_set_idle;
	ret->add_buddy = msn_add_buddy;
	ret->remove_buddy = msn_rem_buddy;
	ret->chat_send = msn_chat_send;
	ret->chat_invite = msn_chat_invite;
	ret->chat_leave = msn_chat_leave;
	ret->normalize = msn_normalize;
	ret->do_action = msn_do_action;
	ret->actions = msn_actions;
	ret->convo_closed = msn_convo_closed;
	ret->keepalive = msn_keepalive;
	ret->set_permit_deny = msn_set_permit_deny;
	ret->add_permit = msn_add_permit;
	ret->rem_permit = msn_rem_permit;
	ret->add_deny = msn_add_deny;
	ret->rem_deny = msn_rem_deny;
	ret->buddy_free = msn_buddy_free;
	ret->smiley_list = msn_smiley_list;

	puo = g_new0(struct proto_user_opt, 1);
	puo->label = g_strdup("Server:");
	puo->def = g_strdup(MSN_SERVER);
	puo->pos = USEROPT_MSNSERVER;
	ret->user_opts = g_list_append(ret->user_opts, puo);

	puo = g_new0(struct proto_user_opt, 1);
	puo->label = g_strdup("Port:");
	puo->def = g_strdup("1863");
	puo->pos = USEROPT_MSNPORT;
	ret->user_opts = g_list_append(ret->user_opts, puo);

	my_protocol = ret;
}

#ifndef STATIC

void *gaim_prpl_init(struct prpl *prpl)
{
	msn_init(prpl);
	prpl->plug->desc.api_version = PLUGIN_API_VERSION;
}

void gaim_plugin_remove()
{
	struct prpl *p = find_prpl(PROTO_MSN);
	if (p == my_protocol)
		unload_protocol(p);
}

char *name()
{
	return "MSN";
}

char *description()
{
	return PRPL_DESC("MSN");
}

#endif

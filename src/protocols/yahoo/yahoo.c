/*
 * gaim
 *
 * Some code copyright (C) 1998-1999, Mark Spencer <markster@marko.net>
 * libfaim code copyright 1998, 1999 Adam Fritzler <afritz@auk.cx>
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
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <ctype.h>
#include "multi.h"
#include "prpl.h"
#include "gaim.h"
#include "proxy.h"

extern char *yahoo_crypt(char *, char *);

#include "pixmaps/status-away.xpm"
#include "pixmaps/status-here.xpm"
#include "pixmaps/status-idle.xpm"

#define YAHOO_DEBUG

#define USEROPT_MAIL 0

#define USEROPT_PAGERHOST 3
#define YAHOO_PAGER_HOST "cs.yahoo.com"
#define USEROPT_PAGERPORT 4
#define YAHOO_PAGER_PORT 5050

/* I just made these values up.  Anyone know what they should be?
 * -SeanEgan */
#define YAHOO_TYPING_RECV_TIMEOUT 10
#define YAHOO_TYPING_SEND_TIMEOUT 8

enum yahoo_service { /* these are easier to see in hex */
	YAHOO_SERVICE_LOGON = 1,
	YAHOO_SERVICE_LOGOFF,
	YAHOO_SERVICE_ISAWAY,
	YAHOO_SERVICE_ISBACK,
	YAHOO_SERVICE_IDLE, /* 5 (placemarker) */
	YAHOO_SERVICE_MESSAGE,
	YAHOO_SERVICE_IDACT,
	YAHOO_SERVICE_IDDEACT,
	YAHOO_SERVICE_MAILSTAT,
	YAHOO_SERVICE_USERSTAT, /* 0xa */
	YAHOO_SERVICE_NEWMAIL,
	YAHOO_SERVICE_CHATINVITE,
	YAHOO_SERVICE_CALENDAR,
	YAHOO_SERVICE_NEWPERSONALMAIL,
	YAHOO_SERVICE_NEWCONTACT,
	YAHOO_SERVICE_ADDIDENT, /* 0x10 */
	YAHOO_SERVICE_ADDIGNORE,
	YAHOO_SERVICE_PING,
	YAHOO_SERVICE_GROUPRENAME,
	YAHOO_SERVICE_SYSMESSAGE = 0x14,
	YAHOO_SERVICE_PASSTHROUGH2 = 0x16,
	YAHOO_SERVICE_CONFINVITE = 0x18,
	YAHOO_SERVICE_CONFLOGON,
	YAHOO_SERVICE_CONFDECLINE,
	YAHOO_SERVICE_CONFLOGOFF,
	YAHOO_SERVICE_CONFADDINVITE,
	YAHOO_SERVICE_CONFMSG,
	YAHOO_SERVICE_CHATLOGON,
	YAHOO_SERVICE_CHATLOGOFF,
	YAHOO_SERVICE_CHATMSG = 0x20,
	YAHOO_SERVICE_GAMELOGON = 0x28,
	YAHOO_SERVICE_GAMELOGOFF,
	YAHOO_SERVICE_GAMEMSG = 0x2a,
	YAHOO_SERVICE_FILETRANSFER = 0x46,
	YAHOO_SERVICE_TYPING = 0x4B, /* may have other uses too */
	YAHOO_SERVICE_LIST = 0x55,
	YAHOO_SERVICE_ADDBUDDY = 0x83,
	YAHOO_SERVICE_REMBUDDY = 0x84
};

enum yahoo_status {
	YAHOO_STATUS_AVAILABLE = 0,
	YAHOO_STATUS_BRB,
	YAHOO_STATUS_BUSY,
	YAHOO_STATUS_NOTATHOME,
	YAHOO_STATUS_NOTATDESK,
	YAHOO_STATUS_NOTINOFFICE,
	YAHOO_STATUS_ONPHONE,
	YAHOO_STATUS_ONVACATION,
	YAHOO_STATUS_OUTTOLUNCH,
	YAHOO_STATUS_STEPPEDOUT,
	YAHOO_STATUS_INVISIBLE = 12,
	YAHOO_STATUS_CUSTOM = 99,
	YAHOO_STATUS_IDLE = 999,
	YAHOO_STATUS_OFFLINE = 0x5a55aa56, /* don't ask */
	YAHOO_STATUS_TYPING = 0x16
};

struct yahoo_data {
	int fd;
	guchar *rxqueue;
	int rxlen;
	GHashTable *hash;
	int current_status;
	gboolean logged_in;
};

struct yahoo_pair {
	int key;
	char *value;
};

struct yahoo_packet {
	guint16 service;
	guint32 status;
	guint32 id;
	GSList *hash;
};

static char *yahoo_name() {
	return "Yahoo";
}

#define YAHOO_PACKET_HDRLEN (4 + 2 + 2 + 2 + 2 + 4 + 4)

static struct yahoo_packet *yahoo_packet_new(enum yahoo_service service, enum yahoo_status status, int id)
{
	struct yahoo_packet *pkt = g_new0(struct yahoo_packet, 1);

	pkt->service = service;
	pkt->status = status;
	pkt->id = id;

	return pkt;
}

static void yahoo_packet_hash(struct yahoo_packet *pkt, int key, char *value)
{
	struct yahoo_pair *pair = g_new0(struct yahoo_pair, 1);
	pair->key = key;
	pair->value = g_strdup(value);
	pkt->hash = g_slist_append(pkt->hash, pair);
}

static int yahoo_packet_length(struct yahoo_packet *pkt)
{
	GSList *l;

	int len = 0;

	l = pkt->hash;
	while (l) {
		struct yahoo_pair *pair = l->data;
		int tmp = pair->key;
		do {
			tmp /= 10;
			len++;
		} while (tmp);
		len += 2;
		len += strlen(pair->value);
		len += 2;
		l = l->next;
	}

	return len;
}

/* sometimes i wish prpls could #include things from other prpls. then i could just
 * use the routines from libfaim and not have to admit to knowing how they work. */
#define yahoo_put16(buf, data) ( \
		(*(buf) = (u_char)((data)>>8)&0xff), \
		(*((buf)+1) = (u_char)(data)&0xff),  \
		2)
#define yahoo_get16(buf) ((((*(buf))<<8)&0xff00) + ((*((buf)+1)) & 0xff))
#define yahoo_put32(buf, data) ( \
		(*((buf)) = (u_char)((data)>>24)&0xff), \
		(*((buf)+1) = (u_char)((data)>>16)&0xff), \
		(*((buf)+2) = (u_char)((data)>>8)&0xff), \
		(*((buf)+3) = (u_char)(data)&0xff), \
		4)
#define yahoo_get32(buf) ((((*(buf))<<24)&0xff000000) + \
		(((*((buf)+1))<<16)&0x00ff0000) + \
		(((*((buf)+2))<< 8)&0x0000ff00) + \
		(((*((buf)+3)    )&0x000000ff)))

static void yahoo_packet_read(struct yahoo_packet *pkt, guchar *data, int len)
{
	int pos = 0;

	while (pos + 1 < len) {
		char key[64], *value = NULL;
		int accept;
		int x;

		struct yahoo_pair *pair = g_new0(struct yahoo_pair, 1);

		x = 0;
		while (pos + 1 < len) {
			if (data[pos] == 0xc0 && data[pos + 1] == 0x80)
				break;
			key[x++] = data[pos++];
		}
		key[x] = 0;
		pos += 2;
		pair->key = strtol(key, NULL, 10);
		accept = x; /* if x is 0 there was no key, so don't accept it */

		if (accept)
			value = g_malloc(len - pos + 1);
		x = 0;
		while (pos + 1 < len) {
			if (data[pos] == 0xc0 && data[pos + 1] == 0x80)
				break;
			if (accept)
				value[x++] = data[pos++];
		}
		if (accept)
			value[x] = 0;
		pos += 2;
		if (accept) {
			pair->value = g_strdup(value);
			g_free(value);
			pkt->hash = g_slist_append(pkt->hash, pair);
			debug_printf("Key: %d  \tValue: %s\n", pair->key, pair->value);
		} else {
			g_free(pair);
		}
	}
}

static void yahoo_packet_write(struct yahoo_packet *pkt, guchar *data)
{
	GSList *l = pkt->hash;
	int pos = 0;

	while (l) {
		struct yahoo_pair *pair = l->data;
		guchar buf[100];

		g_snprintf(buf, sizeof(buf), "%d", pair->key);
		strcpy(data + pos, buf);
		pos += strlen(buf);
		data[pos++] = 0xc0;
		data[pos++] = 0x80;

		strcpy(data + pos, pair->value);
		pos += strlen(pair->value);
		data[pos++] = 0xc0;
		data[pos++] = 0x80;

		l = l->next;
	}
}

static void yahoo_packet_dump(guchar *data, int len)
{
#ifdef YAHOO_DEBUG
	int i;
	for (i = 0; i + 1 < len; i += 2) {
		if ((i % 16 == 0) && i)
			debug_printf("\n");
		debug_printf("%02x", data[i]);
		debug_printf("%02x ", data[i+1]);
	}
	if (i < len)
		debug_printf("%02x", data[i]);
	debug_printf("\n");
	for (i = 0; i < len; i++) {
		if ((i % 16 == 0) && i)
			debug_printf("\n");
		if (isprint(data[i]))
			debug_printf("%c ", data[i]);
		else
			debug_printf(". ");
	}
	debug_printf("\n");
#endif
}

static int yahoo_send_packet(struct yahoo_data *yd, struct yahoo_packet *pkt)
{
	int pktlen = yahoo_packet_length(pkt);
	int len = YAHOO_PACKET_HDRLEN + pktlen;
	int ret;

	guchar *data;
	int pos = 0;

	if (yd->fd < 0)
		return -1;

	data = g_malloc0(len + 1);

	memcpy(data + pos, "YMSG", 4); pos += 4;
	pos += yahoo_put16(data + pos, 0x0600);
	pos += yahoo_put16(data + pos, 0x0000);
	pos += yahoo_put16(data + pos, pktlen);
	pos += yahoo_put16(data + pos, pkt->service);
	pos += yahoo_put32(data + pos, pkt->status);
	pos += yahoo_put32(data + pos, pkt->id);

	yahoo_packet_write(pkt, data + pos);

	yahoo_packet_dump(data, len);
	ret = write(yd->fd, data, len);

	g_free(data);

	return ret;
}

static void yahoo_packet_free(struct yahoo_packet *pkt)
{
	while (pkt->hash) {
		struct yahoo_pair *pair = pkt->hash->data;
		g_free(pair->value);
		g_free(pair);
		pkt->hash = g_slist_remove(pkt->hash, pair);
	}
	g_free(pkt);
}

static void yahoo_process_status(struct gaim_connection *gc, struct yahoo_packet *pkt)
{
	struct yahoo_data *yd = gc->proto_data;
	GSList *l = pkt->hash;
	struct yahoo_packet *newpkt;
	char *name = NULL;
	int state = 0;
	char *msg = NULL;

	while (l) {
		struct yahoo_pair *pair = l->data;

		switch (pair->key) {
		case 0: /* we won't actually do anything with this */
			break;
		case 1: /* we don't get the full buddy list here. */
			if (!yd->logged_in) {
				account_online(gc);
				serv_finish_login(gc);
				g_snprintf(gc->displayname, sizeof(gc->displayname), "%s", pair->value);
				do_import(gc, NULL);
				yd->logged_in = TRUE;

				/* this requests the list. i have a feeling that this is very evil */
				newpkt = yahoo_packet_new(YAHOO_SERVICE_LIST, YAHOO_STATUS_OFFLINE, 0);
				yahoo_send_packet(yd, newpkt);
				yahoo_packet_free(newpkt);
			}
			break;
		case 8: /* how many online buddies we have */
			break;
		case 7: /* the current buddy */
			name = pair->value;
			break;
		case 10: /* state */
			state = strtol(pair->value, NULL, 10);
			break;
		case 19: /* custom message */
			msg = pair->value;
			break;
		case 11: /* i didn't know what this was in the old protocol either */
			break;
		case 17: /* in chat? */
			break;
		case 13: /* in pager? */
			if (pkt->service == YAHOO_SERVICE_LOGOFF ||
			    strtol(pair->value, NULL, 10) == 0) {
				serv_got_update(gc, name, 0, 0, 0, 0, 0, 0);
				break;
			}
			if (state == YAHOO_STATUS_AVAILABLE)
				serv_got_update(gc, name, 1, 0, 0, 0, 0, 0);
			else
				serv_got_update(gc, name, 1, 0, 0, 0, (state << 1) | UC_UNAVAILABLE, 0);
			if (state == YAHOO_STATUS_CUSTOM) {
				gpointer val = g_hash_table_lookup(yd->hash, name);
				if (val) {
					g_free(val);
					g_hash_table_insert(yd->hash, name,
							msg ? g_strdup(msg) : g_malloc0(1));
				} else
					g_hash_table_insert(yd->hash, g_strdup(name),
							msg ? g_strdup(msg) : g_malloc0(1));
			}
			break;
		case 60: /* no clue */
			 break;
		case 16: /* Custom error message */
			do_error_dialog(pair->value, "Gaim -- Yahoo! Error");
			break;
		default:
			debug_printf("unknown status key %d\n", pair->key);
			break;
		}

		l = l->next;
	}
}

static void yahoo_process_list(struct gaim_connection *gc, struct yahoo_packet *pkt)
{
	GSList *l = pkt->hash;
	gboolean export = FALSE;

	while (l) {
		char **lines;
		char **split;
		char **buddies;
		char **tmp, **bud;

		struct yahoo_pair *pair = l->data;
		l = l->next;

		if (pair->key != 87)
			continue;

		lines = g_strsplit(pair->value, "\n", -1);
		for (tmp = lines; *tmp; tmp++) {
			split = g_strsplit(*tmp, ":", 2);
			if (!split)
				continue;
			if (!split[0] || !split[1]) {
				g_strfreev(split);
				continue;
			}
			buddies = g_strsplit(split[1], ",", -1);
			for (bud = buddies; bud && *bud; bud++)
				if (!find_buddy(gc, *bud)) {
					add_buddy(gc, split[0], *bud, *bud);
					export = TRUE;
				}
			g_strfreev(buddies);
			g_strfreev(split);
		}
		g_strfreev(lines);
	}

	if (export)
		do_export(gc);
}

static void yahoo_process_typing(struct gaim_connection *gc, struct yahoo_packet *pkt)
{
	char *msg = NULL;
	char *from = NULL;
	time_t tm = time(0);
	GSList *l = pkt->hash;

	while (l) {
		struct yahoo_pair *pair = l->data;
		if (pair->key == 4)
			from = pair->value;
		if (pair->key == 49)
			msg = pair->value;
		l = l->next;
	}
	serv_got_typing(gc, from, YAHOO_TYPING_RECV_TIMEOUT);
}

static void yahoo_process_message(struct gaim_connection *gc, struct yahoo_packet *pkt)
{
	char *msg = NULL;
	char *from = NULL;
	time_t tm = time(NULL);
	GSList *l = pkt->hash;

	while (l) {
		struct yahoo_pair *pair = l->data;
		if (pair->key == 4)
			from = pair->value;
		if (pair->key == 14)
			msg = pair->value;
		if (pair->key == 15)
			tm = strtol(pair->value, NULL, 10);
		l = l->next;
	}

	if (pkt->status <= 1) {
		char *m;
		int i, j;
		strip_linefeed(msg);
		m = msg;
		for (i = 0, j = 0; m[i]; i++) {
			if (m[i] == 033) {
				while (m[i] && (m[i] != 'm'))
					i++;
				if (!m[i])
					i--;
				continue;
			}
			msg[j++] = m[i];
		}
		msg[j] = 0;
		serv_got_im(gc, from, msg, 0, tm, -1);
	} else if (pkt->status == 2) {
		do_error_dialog(_("Your message did not get sent."), _("Gaim - Error"));
	}
}


static void yahoo_process_contact(struct gaim_connection *gc, struct yahoo_packet *pkt)
{
	struct yahoo_data *yd = gc->proto_data;
	char *id = NULL;
	char *who = NULL;
	char *msg = NULL;
	char *name = NULL;
	int state = YAHOO_STATUS_AVAILABLE;
	int online = FALSE;

	GSList *l = pkt->hash;

	while (l) {
		struct yahoo_pair *pair = l->data;
		if (pair->key == 1)
			id = pair->value;
		else if (pair->key == 3)
			who = pair->value;
		else if (pair->key == 14)
			msg = pair->value;
		else if (pair->key == 7)
			name = pair->value;
		else if (pair->key == 10)
			state = strtol(pair->value, NULL, 10);
		else if (pair->key == 13)
			online = strtol(pair->value, NULL, 10);
		l = l->next;
	}

	if (id)
		show_got_added(gc, id, who, NULL, msg);
	if (name) {
		if (state == YAHOO_STATUS_AVAILABLE)
			serv_got_update(gc, name, 1, 0, 0, 0, 0, 0);
		else if (state == YAHOO_STATUS_IDLE)
			serv_got_update(gc, name, 1, 0, 0, time(NULL) - 600, (state << 1), 0);
		else
			serv_got_update(gc, name, 1, 0, 0, 0, (state << 1) | UC_UNAVAILABLE, 0);
		if (state == YAHOO_STATUS_CUSTOM) {
			gpointer val = g_hash_table_lookup(yd->hash, name);
			if (val) {
				g_free(val);
				g_hash_table_insert(yd->hash, name,
						msg ? g_strdup(msg) : g_malloc0(1));
			} else
				g_hash_table_insert(yd->hash, g_strdup(name),
						msg ? g_strdup(msg) : g_malloc0(1));
		}
	}
}

static void yahoo_process_mail(struct gaim_connection *gc, struct yahoo_packet *pkt)
{
	char *who = NULL;
	char *email = NULL;
	char *subj = NULL;
	int count = 0;
	GSList *l = pkt->hash;

	while (l) {
		struct yahoo_pair *pair = l->data;
		if (pair->key == 9)
			count = strtol(pair->value, NULL, 10);
		else if (pair->key == 43)
			who = pair->value;
		else if (pair->key == 42)
			email = pair->value;
		else if (pair->key == 18)
			subj = pair->value;
		l = l->next;
	}

	if (who && email && subj) {
		char *from = g_strdup_printf("%s (%s)", who, email);
		connection_has_mail(gc, -1, from, subj, "http://mail.yahoo.com/");
		g_free(from);
	} else
		connection_has_mail(gc, count, NULL, NULL, "http://mail.yahoo.com/");
}

static void yahoo_packet_process(struct gaim_connection *gc, struct yahoo_packet *pkt)
{
	switch (pkt->service)
	{
	case YAHOO_SERVICE_LOGON:
	case YAHOO_SERVICE_LOGOFF:
	case YAHOO_SERVICE_ISAWAY:
	case YAHOO_SERVICE_ISBACK:
		yahoo_process_status(gc, pkt);
		break;
	case YAHOO_SERVICE_TYPING:
		yahoo_process_typing(gc, pkt);
		break;
	case YAHOO_SERVICE_MESSAGE:
	case YAHOO_SERVICE_GAMEMSG:
		yahoo_process_message(gc, pkt);
		break;
	case YAHOO_SERVICE_NEWMAIL:
		yahoo_process_mail(gc, pkt);
		break;
	case YAHOO_SERVICE_NEWCONTACT:
		yahoo_process_contact(gc, pkt);
		break;
	case YAHOO_SERVICE_LIST:
		yahoo_process_list(gc, pkt);
		break;
	default:
		debug_printf("unhandled service 0x%02x\n", pkt->service);
		break;
	}
}

static void yahoo_pending(gpointer data, gint source, GaimInputCondition cond)
{
	struct gaim_connection *gc = data;
	struct yahoo_data *yd = gc->proto_data;
	char buf[1024];
	int len;

	len = read(yd->fd, buf, sizeof(buf));

	if (len <= 0) {
		hide_login_progress(gc, "Unable to read");
		signoff(gc);
		return;
	}

	yd->rxqueue = g_realloc(yd->rxqueue, len + yd->rxlen);
	memcpy(yd->rxqueue + yd->rxlen, buf, len);
	yd->rxlen += len;

	while (1) {
		struct yahoo_packet *pkt;
		int pos = 0;
		int pktlen;

		if (yd->rxlen < YAHOO_PACKET_HDRLEN)
			return;

		pos += 4; /* YMSG */
		pos += 2;
		pos += 2;

		pktlen = yahoo_get16(yd->rxqueue + pos); pos += 2;
		debug_printf("%d bytes to read, rxlen is %d\n", pktlen, yd->rxlen);

		if (yd->rxlen < (YAHOO_PACKET_HDRLEN + pktlen))
			return;

		yahoo_packet_dump(yd->rxqueue, YAHOO_PACKET_HDRLEN + pktlen);

		pkt = yahoo_packet_new(0, 0, 0);

		pkt->service = yahoo_get16(yd->rxqueue + pos); pos += 2;
		debug_printf("Yahoo Service: 0x%02x Status: %d\n", pkt->service, pkt->status);
		pkt->status = yahoo_get32(yd->rxqueue + pos); pos += 4;
		pkt->id = yahoo_get32(yd->rxqueue + pos); pos += 4;

		yahoo_packet_read(pkt, yd->rxqueue + pos, pktlen);

		yd->rxlen -= YAHOO_PACKET_HDRLEN + pktlen;
		if (yd->rxlen) {
			char *tmp = g_memdup(yd->rxqueue + YAHOO_PACKET_HDRLEN + pktlen, yd->rxlen);
			g_free(yd->rxqueue);
			yd->rxqueue = tmp;
		} else {
			g_free(yd->rxqueue);
			yd->rxqueue = NULL;
		}

		yahoo_packet_process(gc, pkt);

		yahoo_packet_free(pkt);
	}
}

static void yahoo_got_connected(gpointer data, gint source, GaimInputCondition cond)
{
	struct gaim_connection *gc = data;
	struct yahoo_data *yd;
	struct yahoo_packet *pkt;

	if (!g_slist_find(connections, gc)) {
		close(source);
		return;
	}

	if (source < 0) {
		hide_login_progress(gc, "Unable to connect");
		signoff(gc);
		return;
	}

	yd = gc->proto_data;
	yd->fd = source;

	pkt = yahoo_packet_new(YAHOO_SERVICE_LOGON, YAHOO_STATUS_AVAILABLE, 0);

	yahoo_packet_hash(pkt, 0, gc->username);
	yahoo_packet_hash(pkt, 1, gc->username);
	yahoo_packet_hash(pkt, 6, yahoo_crypt(gc->password, "$1$_2S43d5f$"));

	yahoo_send_packet(yd, pkt);

	yahoo_packet_free(pkt);

	gc->inpa = gaim_input_add(yd->fd, GAIM_INPUT_READ, yahoo_pending, gc);
}

static void yahoo_login(struct aim_user *user) {
	struct gaim_connection *gc = new_gaim_conn(user);
	struct yahoo_data *yd = gc->proto_data = g_new0(struct yahoo_data, 1);

	set_login_progress(gc, 1, "Connecting");

	yd->fd = -1;
	yd->hash = g_hash_table_new(g_str_hash, g_str_equal);

	if (!g_strncasecmp(user->proto_opt[USEROPT_PAGERHOST], "scs.yahoo.com", strlen("scs.yahoo.com"))) {
		/* As of this morning, Yahoo is no longer supporting its server at scs.yahoo.com
		 * I don't like to edit the preferences in a prpl, but we'll keep this here
		 * for a while until everybody's happy again. -5 Feb 2002*/
		debug_printf("Setting new Yahoo! server.\n");
		g_snprintf(user->proto_opt[USEROPT_PAGERHOST], strlen("cs.yahoo.com") + 1, "cs.yahoo.com");
		save_prefs();
	}
	    
       	if (proxy_connect(user->proto_opt[USEROPT_PAGERHOST][0] ?
				user->proto_opt[USEROPT_PAGERHOST] : YAHOO_PAGER_HOST,
			   user->proto_opt[USEROPT_PAGERPORT][0] ?
				atoi(user->proto_opt[USEROPT_PAGERPORT]) : YAHOO_PAGER_PORT,
			   yahoo_got_connected, gc) < 0) {
		hide_login_progress(gc, "Connection problem");
		signoff(gc);
		return;
	}

}

static gboolean yahoo_destroy_hash(gpointer key, gpointer val, gpointer data)
{
	g_free(key);
	g_free(val);
	return TRUE;
}

static void yahoo_close(struct gaim_connection *gc) {
	struct yahoo_data *yd = (struct yahoo_data *)gc->proto_data;
	g_hash_table_foreach_remove(yd->hash, yahoo_destroy_hash, NULL);
	g_hash_table_destroy(yd->hash);
	if (yd->fd >= 0)
		close(yd->fd);
	if (yd->rxqueue)
		g_free(yd->rxqueue);
	yd->rxlen = 0;
	if (gc->inpa)
		gaim_input_remove(gc->inpa);
	g_free(yd);
}

static char **yahoo_list_icon(int uc)
{
	if ((uc >> 1) == YAHOO_STATUS_IDLE)
		return status_idle_xpm;
	else if (uc == 0)
		return status_here_xpm;
	return status_away_xpm;
}

static char *yahoo_get_status_string(enum yahoo_status a)
{
	switch (a) {
	case YAHOO_STATUS_BRB:
		return "Be Right Back";
	case YAHOO_STATUS_BUSY:
		return "Busy";
	case YAHOO_STATUS_NOTATHOME:
		return "Not At Home";
	case YAHOO_STATUS_NOTATDESK:
		return "Not At Desk";
	case YAHOO_STATUS_NOTINOFFICE:
		return "Not In Office";
	case YAHOO_STATUS_ONPHONE:
		return "On Phone";
	case YAHOO_STATUS_ONVACATION:
		return "On Vacation";
	case YAHOO_STATUS_OUTTOLUNCH:
		return "Out To Lunch";
	case YAHOO_STATUS_STEPPEDOUT:
		return "Stepped Out";
	case YAHOO_STATUS_INVISIBLE:
		return "Invisible";
	default:
		return "Online";
	}
}

static GList *yahoo_buddy_menu(struct gaim_connection *gc, char *who)
{
	GList *m = NULL;
	struct proto_buddy_menu *pbm;
	struct yahoo_data *yd = (struct yahoo_data *)gc->proto_data;
	struct buddy *b = find_buddy(gc, who); /* this should never be null. if it is,
						  segfault and get the bug report. */
	static char buf[1024];

	if (!(b->uc & UC_UNAVAILABLE))
		return NULL;
    if (b->uc >> 1 == YAHOO_STATUS_IDLE)
        return NULL;

	pbm = g_new0(struct proto_buddy_menu, 1);
	if ((b->uc >> 1) != YAHOO_STATUS_CUSTOM)
		g_snprintf(buf, sizeof buf, "Status: %s", yahoo_get_status_string(b->uc >> 1));
	else
		g_snprintf(buf, sizeof buf, "Custom Status: %s",
			   (char *)g_hash_table_lookup(yd->hash, b->name));
	pbm->label = buf;
	pbm->callback = NULL;
	pbm->gc = gc;
	m = g_list_append(m, pbm);

	return m;
}

static GList *yahoo_user_opts()
{
	GList *m = NULL;
	struct proto_user_opt *puo;

	puo = g_new0(struct proto_user_opt, 1);
	puo->label = "Pager Host:";
	puo->def = YAHOO_PAGER_HOST;
	puo->pos = USEROPT_PAGERHOST;
	m = g_list_append(m, puo);

	puo = g_new0(struct proto_user_opt, 1);
	puo->label = "Pager Port:";
	puo->def = "5050";
	puo->pos = USEROPT_PAGERPORT;
	m = g_list_append(m, puo);

	return m;
}

static void yahoo_act_id(gpointer data, char *entry)
{
	struct gaim_connection *gc = data;
	struct yahoo_data *yd = gc->proto_data;

	struct yahoo_packet *pkt = yahoo_packet_new(YAHOO_SERVICE_IDACT, YAHOO_STATUS_AVAILABLE, 0);
	yahoo_packet_hash(pkt, 3, entry);
	yahoo_send_packet(yd, pkt);
	yahoo_packet_free(pkt);

	g_snprintf(gc->displayname, sizeof(gc->displayname), "%s", entry);
}

static void yahoo_do_action(struct gaim_connection *gc, char *act)
{
	if (!strcmp(act, "Activate ID")) {
		do_prompt_dialog("Activate which ID:", gc->displayname, gc, yahoo_act_id, NULL);
	}
}

static GList *yahoo_actions() {
	GList *m = NULL;

	m = g_list_append(m, "Activate ID");

	return m;
}

static int yahoo_send_im(struct gaim_connection *gc, char *who, char *what, int flags)
{
	struct yahoo_data *yd = gc->proto_data;
	struct yahoo_packet *pkt = yahoo_packet_new(YAHOO_SERVICE_MESSAGE, YAHOO_STATUS_OFFLINE, 0);

	yahoo_packet_hash(pkt, 1, gc->displayname);
	yahoo_packet_hash(pkt, 5, who);
	yahoo_packet_hash(pkt, 14, what);

	yahoo_send_packet(yd, pkt);

	yahoo_packet_free(pkt);

	return 1;
}

int yahoo_send_typing(struct gaim_connection *gc, char *who)
{
	struct yahoo_data *yd = gc->proto_data;
	struct yahoo_packet *pkt = yahoo_packet_new(YAHOO_SERVICE_TYPING, YAHOO_STATUS_TYPING, 0);//x6431de4f);

	yahoo_packet_hash(pkt, 49, "TYPING");
	yahoo_packet_hash(pkt, 1, gc->displayname);
	yahoo_packet_hash(pkt, 14, " ");
	yahoo_packet_hash(pkt, 13, "1");
	yahoo_packet_hash(pkt, 5, who);
	yahoo_packet_hash(pkt, 1002, "1");

	yahoo_send_packet(yd, pkt);

	yahoo_packet_free(pkt);

	return YAHOO_TYPING_SEND_TIMEOUT;
}

static void yahoo_set_away(struct gaim_connection *gc, char *state, char *msg)
{
	struct yahoo_data *yd = (struct yahoo_data *)gc->proto_data;
	struct yahoo_packet *pkt;
	int service;
	char s[4];

	gc->away = NULL;

	if (msg) {
		yd->current_status = YAHOO_STATUS_CUSTOM;
		gc->away = "";
	} else if (state) {
		gc->away = "";
		if (!strcmp(state, "Available")) {
			yd->current_status = YAHOO_STATUS_AVAILABLE;
			gc->away = NULL;
		} else if (!strcmp(state, "Be Right Back")) {
			yd->current_status = YAHOO_STATUS_BRB;
		} else if (!strcmp(state, "Busy")) {
			yd->current_status = YAHOO_STATUS_BUSY;
		} else if (!strcmp(state, "Not At Home")) {
			yd->current_status = YAHOO_STATUS_NOTATHOME;
		} else if (!strcmp(state, "Not At Desk")) {
			yd->current_status = YAHOO_STATUS_NOTATDESK;
		} else if (!strcmp(state, "Not In Office")) {
			yd->current_status = YAHOO_STATUS_NOTINOFFICE;
		} else if (!strcmp(state, "On Phone")) {
			yd->current_status = YAHOO_STATUS_ONPHONE;
		} else if (!strcmp(state, "On Vacation")) {
			yd->current_status = YAHOO_STATUS_ONVACATION;
		} else if (!strcmp(state, "Out To Lunch")) {
			yd->current_status = YAHOO_STATUS_OUTTOLUNCH;
		} else if (!strcmp(state, "Stepped Out")) {
			yd->current_status = YAHOO_STATUS_STEPPEDOUT;
		} else if (!strcmp(state, "Invisible")) {
			yd->current_status = YAHOO_STATUS_INVISIBLE;
		} else if (!strcmp(state, GAIM_AWAY_CUSTOM)) {
			if (gc->is_idle) {
				yd->current_status = YAHOO_STATUS_IDLE;
			} else {
				yd->current_status = YAHOO_STATUS_AVAILABLE;
			}
			gc->away = NULL;
		}
	} else if (gc->is_idle) {
		yd->current_status = YAHOO_STATUS_IDLE;
	} else {
		yd->current_status = YAHOO_STATUS_AVAILABLE;
	}

	if (yd->current_status == YAHOO_STATUS_AVAILABLE)
		service = YAHOO_SERVICE_ISBACK;
	else
		service = YAHOO_SERVICE_ISAWAY;
	pkt = yahoo_packet_new(service, yd->current_status, 0);
	g_snprintf(s, sizeof(s), "%d", yd->current_status);
	yahoo_packet_hash(pkt, 10, s);
	if (yd->current_status == YAHOO_STATUS_CUSTOM)
		yahoo_packet_hash(pkt, 19, msg);

	yahoo_send_packet(yd, pkt);
	yahoo_packet_free(pkt);
}

static void yahoo_set_idle(struct gaim_connection *gc, int idle)
{
	struct yahoo_data *yd = gc->proto_data;
	struct yahoo_packet *pkt = NULL;

	if (idle && yd->current_status == YAHOO_STATUS_AVAILABLE) {
		pkt = yahoo_packet_new(YAHOO_SERVICE_ISAWAY, YAHOO_STATUS_IDLE, 0);
		yd->current_status = YAHOO_STATUS_IDLE;
	} else if (!idle && yd->current_status == YAHOO_STATUS_IDLE) {
		pkt = yahoo_packet_new(YAHOO_SERVICE_ISAWAY, YAHOO_STATUS_AVAILABLE, 0);
		yd->current_status = YAHOO_STATUS_AVAILABLE;
	}

	if (pkt) {
		char buf[4];
		g_snprintf(buf, sizeof(buf), "%d", yd->current_status);
		yahoo_packet_hash(pkt, 10, buf);
		yahoo_send_packet(yd, pkt);
		yahoo_packet_free(pkt);
	}
}

static GList *yahoo_away_states(struct gaim_connection *gc)
{
	GList *m = NULL;

	m = g_list_append(m, "Available");
	m = g_list_append(m, "Be Right Back");
	m = g_list_append(m, "Busy");
	m = g_list_append(m, "Not At Home");
	m = g_list_append(m, "Not At Desk");
	m = g_list_append(m, "Not In Office");
	m = g_list_append(m, "On Phone");
	m = g_list_append(m, "On Vacation");
	m = g_list_append(m, "Out To Lunch");
	m = g_list_append(m, "Stepped Out");
	m = g_list_append(m, "Invisible");
	m = g_list_append(m, GAIM_AWAY_CUSTOM);

	return m;
}

static void yahoo_keepalive(struct gaim_connection *gc)
{
	struct yahoo_data *yd = gc->proto_data;
	struct yahoo_packet *pkt = yahoo_packet_new(YAHOO_SERVICE_PING, YAHOO_STATUS_AVAILABLE, 0);
	yahoo_send_packet(yd, pkt);
	yahoo_packet_free(pkt);
}

static void yahoo_add_buddy(struct gaim_connection *gc, char *who)
{
	struct yahoo_data *yd = (struct yahoo_data *)gc->proto_data;
	struct yahoo_packet *pkt;
	struct group *g;
	char *group = NULL;

	if (!yd->logged_in)
		return;

	g = find_group_by_buddy(gc, who);
	if (g)
		group = g->name;
	else
		group = "Buddies";

	pkt = yahoo_packet_new(YAHOO_SERVICE_ADDBUDDY, YAHOO_STATUS_AVAILABLE, 0);
	yahoo_packet_hash(pkt, 1, gc->displayname);
	yahoo_packet_hash(pkt, 7, who);
	yahoo_packet_hash(pkt, 65, group);
	yahoo_send_packet(yd, pkt);
	yahoo_packet_free(pkt);
}

static void yahoo_remove_buddy(struct gaim_connection *gc, char *who, char *group)
{
	struct yahoo_data *yd = (struct yahoo_data *)gc->proto_data;

	struct yahoo_packet *pkt = yahoo_packet_new(YAHOO_SERVICE_REMBUDDY, YAHOO_STATUS_AVAILABLE, 0);
	yahoo_packet_hash(pkt, 1, gc->displayname);
	yahoo_packet_hash(pkt, 7, who);
	yahoo_packet_hash(pkt, 65, group);
	yahoo_send_packet(yd, pkt);
	yahoo_packet_free(pkt);
}

static struct prpl *my_protocol = NULL;

void yahoo_init(struct prpl *ret) {
	ret->protocol = PROTO_YAHOO;
	ret->options = OPT_PROTO_MAIL_CHECK;
	ret->name = yahoo_name;
	ret->user_opts = yahoo_user_opts;
	ret->login = yahoo_login;
	ret->close = yahoo_close;
	ret->buddy_menu = yahoo_buddy_menu;
	ret->list_icon = yahoo_list_icon;
	ret->actions = yahoo_actions;
	ret->do_action = yahoo_do_action;
	ret->send_im = yahoo_send_im;
	ret->away_states = yahoo_away_states;
	ret->set_away = yahoo_set_away;
	ret->set_idle = yahoo_set_idle;
	ret->keepalive = yahoo_keepalive;
	ret->add_buddy = yahoo_add_buddy;
	ret->remove_buddy = yahoo_remove_buddy;
	ret->send_typing = yahoo_send_typing;

	my_protocol = ret;
}

#ifndef STATIC

char *gaim_plugin_init(GModule *handle)
{
	load_protocol(yahoo_init, sizeof(struct prpl));
	return NULL;
}

void gaim_plugin_remove()
{
	struct prpl *p = find_prpl(PROTO_YAHOO);
	if (p == my_protocol)
		unload_protocol(p);
}

char *name()
{
	return "Yahoo";
}

char *description()
{
	return PRPL_DESC("Yahoo");
}

#endif

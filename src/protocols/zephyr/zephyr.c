/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */
/*
 * gaim
 *
 * Copyright (C) 1998-2001, Mark Spencer <markster@marko.net>
 * Some code borrowed from GtkZephyr, by
 * 	Jag/Sean Dilda <agrajag@linuxpower.org>/<smdilda@unity.ncsu.edu>
 * 	http://gtkzephyr.linuxpower.org/
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

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "gaim.h"
#include "prpl.h"
#include "zephyr/zephyr.h"

extern Code_t ZGetLocations(ZLocations_t *, int *);
extern Code_t ZSetLocation(char *);
extern Code_t ZUnsetLocation();

typedef struct _zframe zframe;
typedef struct _zephyr_triple zephyr_triple;

/* struct I need for zephyr_to_html */
struct _zframe {
	/* true for everything but @color, since inside the parens of that one is
	* the color. */
	gboolean has_closer;
	/* </i>, </font>, </b>, etc. */
	char *closing;
	/* text including the opening html thingie. */
	GString *text;
	struct _zframe *enclosing;
};

struct _zephyr_triple {
	char *class;
	char *instance;
	char *recipient;
	char *name;
	gboolean open;
	int id;
};

static char *zephyr_name()
{
	return "Zephyr";
}

#define z_call(func)		if (func != ZERR_NONE)\
					return;
#define z_call_r(func)		if (func != ZERR_NONE)\
					return TRUE;
#define z_call_s(func, err)	if (func != ZERR_NONE) {\
					hide_login_progress(zgc, err);\
					signoff(zgc);\
					return;\
				}

static char *zephyr_normalize(const char *);

/* this is so bad, and if Zephyr weren't so fucked up to begin with I
 * wouldn't do this. but it is so i will. */
static guint32 nottimer = 0;
static guint32 loctimer = 0;
struct gaim_connection *zgc = NULL;
static GList *pending_zloc_names = NULL;
static GSList *subscrips = NULL;
static int last_id = 0;

/* just for debugging
static void handle_unknown(ZNotice_t notice)
{
	g_print("z_packet: %s\n", notice.z_packet);
	g_print("z_version: %s\n", notice.z_version);
	g_print("z_kind: %d\n", notice.z_kind);
	g_print("z_class: %s\n", notice.z_class);
	g_print("z_class_inst: %s\n", notice.z_class_inst);
	g_print("z_opcode: %s\n", notice.z_opcode);
	g_print("z_sender: %s\n", notice.z_sender);
	g_print("z_recipient: %s\n", notice.z_recipient);
	g_print("z_message: %s\n", notice.z_message);
	g_print("z_message_len: %d\n", notice.z_message_len);
	g_print("\n");
}
*/

static zephyr_triple *new_triple(const char *c, const char *i, const char *r)
{
	zephyr_triple *zt;
	zt = g_new0(zephyr_triple, 1);
	zt->class = g_strdup(c);
	zt->instance = g_strdup(i);
	zt->recipient = g_strdup(r);
	zt->name = g_strdup_printf("%s,%s,%s", c, i, r);
	zt->id = ++last_id;
	zt->open = FALSE;
	return zt;
}

static void free_triple(zephyr_triple *zt)
{
	g_free(zt->class);
	g_free(zt->instance);
	g_free(zt->recipient);
	g_free(zt->name);
	g_free(zt);
}

/* returns true if zt1 is a subset of zt2, i.e. zt2 has the same thing or
 * wildcards in each field of zt1. */
static gboolean triple_subset(zephyr_triple *zt1, zephyr_triple *zt2)
{
	if (g_strcasecmp(zt2->class, zt1->class) &&
			g_strcasecmp(zt2->class, "*")) {
		return FALSE;
	}
	if (g_strcasecmp(zt2->instance, zt1->instance) &&
			g_strcasecmp(zt2->instance, "*")) {
		return FALSE;
	}
	if (g_strcasecmp(zt2->recipient, zt1->recipient) &&
			g_strcasecmp(zt2->recipient, "*")) {
		return FALSE;
	}
	return TRUE;
}

static zephyr_triple *find_sub_by_triple(zephyr_triple *zt)
{
	zephyr_triple *curr_t;
	GSList *curr = subscrips;
	while (curr) {
		curr_t = curr->data;
		if (triple_subset(zt, curr_t))
			return curr_t;
		curr = curr->next;
	}
	return NULL;
}

static zephyr_triple *find_sub_by_id(int id)
{
	zephyr_triple *zt;
	GSList *curr = subscrips;
	while (curr) {
		zt = curr->data;
		if (zt->id == id)
			return zt;
		curr = curr->next;
	}
	return NULL;
}

/* utility macros that are useful for zephyr_to_html */

#define IS_OPENER(c) ((c == '{') || (c == '[') || (c == '(') || (c == '<'))
#define IS_CLOSER(c) ((c == '}') || (c == ']') || (c == ')') || (c == '>'))

/* this parses zephyr formatting and converts it to html. For example, if
 * you pass in "@{@color(blue)@i(hello)}" you should get out
 * "<font color=blue><i>hello</i></font>". */
static char *zephyr_to_html(char *message)
{
	int len, cnt;
	zframe *frames, *curr;
	char *ret;

	frames = g_new(zframe, 1);
	frames->text = g_string_new("");
	frames->enclosing = NULL;
	frames->closing = "";
	frames->has_closer = FALSE;

	len = strlen(message);
	cnt = 0;
	while (cnt <= len) {
		if (message[cnt] == '@') {
			zframe *new_f;
			char *buf;
			int end;
			for (end=1; (cnt+end) <= len &&
					!IS_OPENER(message[cnt+end]); end++);
			buf = g_new0(char, end);
			if (end) {
				g_snprintf(buf, end, "%s", message+cnt+1);
			}
			if (!g_strcasecmp(buf, "italic") ||
					!g_strcasecmp(buf, "i")) {
				new_f = g_new(zframe, 1);
				new_f->enclosing = frames;
				new_f->text = g_string_new("<i>");
				new_f->closing = "</i>";
				new_f->has_closer = TRUE;
				frames = new_f;
				cnt += end+1; /* cnt points to char after opener */
			} else if (!g_strcasecmp(buf, "bold")
					|| !g_strcasecmp(buf, "b")) {
				new_f = g_new(zframe, 1);
				new_f->enclosing = frames;
				new_f->text = g_string_new("<b>");
				new_f->closing = "</b>";
				new_f->has_closer = TRUE;
				frames = new_f;
				cnt += end+1;
			} else if (!g_strcasecmp(buf, "color")) {
				cnt += end+1;
				new_f = g_new(zframe, 1);
				new_f->enclosing = frames;
				new_f->text = g_string_new("<font color=");
				for (; (cnt <= len) && !IS_CLOSER(message[cnt]); cnt++) {
					g_string_append_c(new_f->text, message[cnt]);
				}
				cnt++; /* point to char after closer */
				g_string_append_c(new_f->text, '>');
				new_f->closing = "</font>";
				new_f->has_closer = FALSE;
				frames = new_f;
			} else if (!g_strcasecmp(buf, "")) {
				new_f = g_new(zframe, 1);
				new_f->enclosing = frames;
				new_f->text = g_string_new("");
				new_f->closing = "";
				new_f->has_closer = TRUE;
				frames = new_f;
				cnt += end+1; /* cnt points to char after opener */
			} else {
				if ((cnt+end) > len) {
					g_string_append_c(frames->text, '@');
					cnt++;
				} else {
					/* unrecognized thingie. act like it's not there, but we
					 * still need to take care of the corresponding closer,
					 * make a frame that does nothing. */
					new_f = g_new(zframe, 1);
					new_f->enclosing = frames;
					new_f->text = g_string_new("");
					new_f->closing = "";
					new_f->has_closer = TRUE;
					frames = new_f;
					cnt += end+1; /* cnt points to char after opener */
				}
			}
		} else if (IS_CLOSER(message[cnt])) {
			zframe *popped;
			gboolean last_had_closer;
			if (frames->enclosing) {
				do {
					popped = frames;
					frames = frames->enclosing;
					g_string_append(frames->text, popped->text->str);
					g_string_append(frames->text, popped->closing);
					g_string_free(popped->text, TRUE);
					last_had_closer = popped->has_closer;
					g_free(popped);
				} while (frames && frames->enclosing && !last_had_closer);
			} else {
				g_string_append_c(frames->text, message[cnt]);
			}
			cnt++;
		} else if (message[cnt] == '\n') {
			g_string_append(frames->text, "<br>");
			cnt++;
		} else {
			g_string_append_c(frames->text, message[cnt++]);
		}
	}
	/* go through all the stuff that they didn't close */
	while (frames->enclosing) {
		curr = frames;
		g_string_append(frames->enclosing->text, frames->text->str);
		g_string_append(frames->enclosing->text, frames->closing);
		g_string_free(frames->text, TRUE);
		frames = frames->enclosing;
		g_free(curr);
	}
	ret = frames->text->str;
	g_string_free(frames->text, FALSE);
	g_free(frames);
	return ret;
}

static gboolean pending_zloc(char *who)
{
	GList *curr;
	for (curr = pending_zloc_names; curr != NULL; curr = curr->next) {
		if (!g_strcasecmp(who, (char*)curr->data)) {
			g_free((char*)curr->data);
			pending_zloc_names = g_list_remove(pending_zloc_names, curr->data);
			return TRUE;
		}
	}
	return FALSE;
}

static void handle_message(ZNotice_t notice, struct sockaddr_in from)
{
	if (!g_strcasecmp(notice.z_class, LOGIN_CLASS)) {
		/* well, we'll be updating in 2 seconds anyway, might as well ignore this. */
	} else if (!g_strcasecmp(notice.z_class, LOCATE_CLASS)) {
		if (!g_strcasecmp(notice.z_opcode, LOCATE_LOCATE)) {
			int nlocs;
			char *user;
			struct buddy *b;

			if (ZParseLocations(&notice, NULL, &nlocs, &user) != ZERR_NONE)
				return;
			if ((b = find_buddy(zgc, user)) == NULL) {
				char *e = strchr(user, '@');
				if (e) *e = '\0';
				b = find_buddy(zgc, user);
			}
			if (!b) {
				free(user);
				return;
			}
			if (pending_zloc(b->name)) {
				ZLocations_t locs;
				int one = 1;
				GString *str = g_string_new("");
				g_string_sprintfa(str, "<b>User:</b> %s<br>"
								"<b>Alias:</b> %s<br>",
								b->name, b->show);
				if (!nlocs) {
					g_string_sprintfa(str, "<br>Hidden or not logged-in");
				}
				for (; nlocs > 0; nlocs--) {
					ZGetLocations(&locs, &one);
					g_string_sprintfa(str, "<br>At %s since %s", locs.host,
									locs.time);
				}
				g_show_info_text(str->str, NULL);
				g_string_free(str, TRUE);
			} else
				serv_got_update(zgc, b->name, nlocs, 0, 0, 0, 0, 0);

			free(user);
		}
	} else {
		char *buf, *buf2;
		char *ptr = notice.z_message + strlen(notice.z_message) + 1;
		int len = notice.z_message_len - (ptr - notice.z_message);
		int away;
		if (len > 0) {
			buf = g_malloc(len + 1);
			g_snprintf(buf, len + 1, "%s", ptr);
			g_strchomp(buf);
			buf2 = zephyr_to_html(buf);
			g_free(buf);
			if (!g_strcasecmp(notice.z_class, "MESSAGE") &&
					!g_strcasecmp(notice.z_class_inst, "PERSONAL")) {
				if (!g_strcasecmp(notice.z_message, "Automated reply:"))
					away = TRUE;
				else
					away = FALSE;
				len = MAX(BUF_LONG, strlen(buf2));
				buf = g_malloc(len + 1);
				g_snprintf(buf, len + 1, "%s", buf2);
				serv_got_im(zgc, notice.z_sender, buf, 0, time((time_t)NULL));
				g_free(buf);
			} else {
				zephyr_triple *zt1, *zt2;
				zt1 = new_triple(notice.z_class, notice.z_class_inst,
								notice.z_recipient);
				zt2 = find_sub_by_triple(zt1);
				if (!zt2) {
					/* we shouldn't be subscribed to this message. ignore. */
				} else {
					len = MAX(BUF_LONG, strlen(buf2));
					buf = g_malloc(len + 1);
					g_snprintf(buf, len + 1, "%s", buf2);
					if (!zt2->open) {
						zt2->open = TRUE;
						serv_got_joined_chat(zgc, zt2->id, zt2->name);
					}
					serv_got_chat_in(zgc, zt2->id, notice.z_sender, FALSE,
								buf, time((time_t)NULL));
					g_free(buf);
				}
				free_triple(zt1);
			}
			g_free(buf2);
		}
	}
}

static gint check_notify(gpointer data)
{
	while (ZPending()) {
		ZNotice_t notice;
		struct sockaddr_in from;
		z_call_r(ZReceiveNotice(&notice, &from));

		switch (notice.z_kind) {
		case UNSAFE:
		case UNACKED:
		case ACKED:
			handle_message(notice, from);
			break;
		default:
			/* we'll just ignore things for now */
			debug_printf("ZEPHYR: Unhandled Notice\n");
			break;
		}

		ZFreeNotice(&notice);
	}

	return TRUE;
}

static gint check_loc(gpointer data)
{
	GSList *gr, *m;
	ZAsyncLocateData_t ald;

	ald.user = NULL;
	memset(&(ald.uid), 0, sizeof(ZUnique_Id_t));
	ald.version = NULL;

	gr = zgc->groups;
	while (gr) {
		struct group *g = gr->data;
		m = g->members;
		while (m) {
			struct buddy *b = m->data;
			char *chk;
			chk = zephyr_normalize(b->name);
			/* doesn't matter if this fails or not; we'll just move on to the next one */
			ZRequestLocations(chk, &ald, UNACKED, ZAUTH);
			free(ald.user);
			free(ald.version);
			m = m->next;
		}
		gr = gr->next;
	}

	return TRUE;
}

static char *get_exposure_level()
{
	char *exposure = ZGetVariable("exposure");

	if (!exposure)
		return EXPOSE_REALMVIS;
	if (!g_strcasecmp(exposure, EXPOSE_NONE))
		return EXPOSE_NONE;
	if (!g_strcasecmp(exposure, EXPOSE_OPSTAFF))
		return EXPOSE_OPSTAFF;
	if (!g_strcasecmp(exposure, EXPOSE_REALMANN))
		return EXPOSE_REALMANN;
	if (!g_strcasecmp(exposure, EXPOSE_NETVIS))
		return EXPOSE_NETVIS;
	if (!g_strcasecmp(exposure, EXPOSE_NETANN))
		return EXPOSE_NETANN;
	return EXPOSE_REALMVIS;
}

static void strip_comments(char *str)
{
	char *tmp = strchr(str, '#');
	if (tmp)
		*tmp = '\0';
	g_strchug(str);
	g_strchomp(str);
}

static void process_zsubs()
{
	FILE *f;
	gchar *fname;
	gchar buff[BUFSIZ];
	
	fname = g_strdup_printf("%s/.zephyr.subs", g_getenv("HOME"));
	f = fopen(fname, "r");
	if (f) {
		char **triple;
		ZSubscription_t sub;
		char *recip;
		while (fgets(buff, BUFSIZ, f)) {
			strip_comments(buff);
			if (buff[0]) {
				triple = g_strsplit(buff, ",", 3);
				if (triple[0] && triple[1] && triple[2]) {
					sub.zsub_class = triple[0];
					sub.zsub_classinst = triple[1];
					if (!g_strcasecmp(triple[2], "%me%")) {
						recip = g_strdup_printf("%s@%s", g_getenv("USER"),
										ZGetRealm());
					} else if (!g_strcasecmp(triple[2], "*")) {
						/* wildcard */
						recip = g_strdup_printf("@%s", ZGetRealm());
					} else {
						recip = g_strdup(triple[2]);
					}
					sub.zsub_recipient = recip;
					if (ZSubscribeTo(&sub, 1, 0) != ZERR_NONE) {
						debug_printf("Zephyr: Couldn't subscribe to %s, %s, "
									   "%s\n",
										sub.zsub_class,
										sub.zsub_classinst,
										sub.zsub_recipient);
					}
					subscrips = g_slist_append(subscrips,
								new_triple(triple[0], triple[1], recip));
					g_free(recip);
				}
				g_strfreev(triple);
			}
		}
	}
}

static void process_anyone()
{
	FILE *fd;
	gchar buff[BUFSIZ], *filename;

	filename = g_strconcat(g_get_home_dir(), "/.anyone", NULL);
	if ((fd = fopen(filename, "r")) != NULL) {
		while (fgets(buff, BUFSIZ, fd)) {
			strip_comments(buff);
			if (buff[0])
				add_buddy(zgc, "Anyone", buff, buff);
		}
		fclose(fd);
	}
	g_free(filename);
}

static void zephyr_login(struct aim_user *user)
{
	ZSubscription_t sub;

	if (zgc) {
		do_error_dialog("Already logged in with Zephyr", "Zephyr");
		return;
	}

	zgc = new_gaim_conn(user);

	z_call_s(ZInitialize(), "Couldn't initialize zephyr");
	z_call_s(ZOpenPort(NULL), "Couldn't open port");
	z_call_s(ZSetLocation(get_exposure_level()), "Couldn't set location");

	sub.zsub_class = "MESSAGE";
	sub.zsub_classinst = "PERSONAL";
	sub.zsub_recipient = ZGetSender();

	/* we don't care if this fails. i'm lying right now. */
	if (ZSubscribeTo(&sub, 1, 0) != ZERR_NONE) {
		debug_printf("Zephyr: Couldn't subscribe to messages!\n");
	}

	account_online(zgc);
	serv_finish_login(zgc);

	if (bud_list_cache_exists(zgc))
		do_import(NULL, zgc);
	process_anyone();
	process_zsubs();

	nottimer = g_timeout_add(100, check_notify, NULL);
	loctimer = g_timeout_add(2000, check_loc, NULL);
}

static void write_zsubs()
{
	GSList *s = subscrips;
	zephyr_triple *zt;
	FILE *fd;
	char *fname;

	fname = g_strdup_printf("%s/.zephyr.subs", g_get_home_dir());
	fd = fopen(fname, "w");
	
	if (!fd) {
		g_free(fname);
		return;
	}
	
	while (s) {
		zt = s->data;
		fprintf(fd, "%s\n", zt->name);
		s = s->next;
	}
	g_free(fname);
	fclose(fd);
}

static void write_anyone()
{
	GSList *gr, *m;
	struct group *g;
	struct buddy *b;
	char *ptr, *fname;
	FILE *fd;

	fname = g_strdup_printf("%s/.anyone", g_get_home_dir());
	fd = fopen(fname, "w");
	if (!fd) {
		g_free(fname);
		return;
	}

	gr = zgc->groups;
	while (gr) {
		g = gr->data;
		m = g->members;
		while (m) {
			b = m->data;
			if ((ptr = strchr(b->name, '@')) != NULL)
				*ptr = '\0';
			fprintf(fd, "%s\n", b->name);
			if (ptr)
				*ptr = '@';
			m = m->next;
		}
		gr = gr->next;
	}

	fclose(fd);
	g_free(fname);
}

static void zephyr_close(struct gaim_connection *gc)
{
	GList *l;
	GSList *s;
	l = pending_zloc_names;
	while (l) {
		g_free((char*)l->data);
		l = l->next;
	}
	g_list_free(pending_zloc_names);
	
	write_anyone();
	write_zsubs();
	
	s = subscrips;
	while (s) {
		free_triple((zephyr_triple*)s->data);
		s = s->next;
	}
	g_slist_free(subscrips);
	
	if (nottimer)
		g_source_remove(nottimer);
	nottimer = 0;
	if (loctimer)
		g_source_remove(loctimer);
	loctimer = 0;
	zgc = NULL;
	z_call(ZCancelSubscriptions(0));
	z_call(ZUnsetLocation());
	z_call(ZClosePort());
}

static void zephyr_add_buddy(struct gaim_connection *gc, char *buddy) { }
static void zephyr_remove_buddy(struct gaim_connection *gc, char *buddy) { }

static int zephyr_chat_send(struct gaim_connection *gc, int id, char *im)
{
	ZNotice_t notice;
	zephyr_triple *zt;
	char *buf;
	const char *sig;

	zt = find_sub_by_id(id);
	if (!zt)
		/* this should never happen. */
		return -EINVAL;
	
	sig = ZGetVariable("zwrite-signature");
	if (!sig) {
		sig = g_get_real_name();
	}
	buf = g_strdup_printf("%s%c%s", sig, '\0', im);

	bzero((char *)&notice, sizeof(notice));
	notice.z_kind = ACKED;
	notice.z_port = 0;
	notice.z_opcode = "";
	notice.z_class = zt->class;
	notice.z_class_inst = zt->instance;
	if (!g_strcasecmp(zt->recipient, "*"))
		notice.z_recipient = zephyr_normalize("");
	else
		notice.z_recipient = zephyr_normalize(zt->recipient);
	notice.z_sender = 0;
	notice.z_default_format =
		"Class $class, Instance $instance:\n"
		"To: @bold($recipient) at $time $date\n"
		"From: @bold($1) <$sender>\n\n$2";
	notice.z_message_len = strlen(im) + strlen(sig) + 4;
	notice.z_message = buf;
	ZSendNotice(&notice, ZAUTH);
	g_free(buf);
	return 0;
}

static int zephyr_send_im(struct gaim_connection *gc, char *who, char *im, int flags) {
	ZNotice_t notice;
	char *buf;
	const char *sig;

	if (flags & IM_FLAG_AWAY)
		sig = "Automated reply:";
	else {
		sig = ZGetVariable("zwrite-signature");
		if (!sig) {
			sig = g_get_real_name();
		}
	}
	buf = g_strdup_printf("%s%c%s", sig, '\0', im);

	bzero((char *)&notice, sizeof(notice));
	notice.z_kind = ACKED;
	notice.z_port = 0;
	notice.z_opcode = "";
	notice.z_class = "MESSAGE";
	notice.z_class_inst = "PERSONAL";
	notice.z_sender = 0;
	notice.z_recipient = who;
	notice.z_default_format =
		"Class $class, Instance $instance:\n"
		"To: @bold($recipient) at $time $date\n"
		"From: @bold($1) <$sender>\n\n$2";
	notice.z_message_len = strlen(im) + strlen(sig) + 4;
	notice.z_message = buf;
	ZSendNotice(&notice, ZAUTH);
	g_free(buf);
	return 1;
}

static char *zephyr_normalize(const char *orig)
{
	static char buf[80];
	if (strchr(orig, '@')) {
		g_snprintf(buf, 80, "%s", orig);
	} else {
		g_snprintf(buf, 80, "%s@%s", orig, ZGetRealm());
	}
	return buf;
}

static void zephyr_zloc(struct gaim_connection *gc, char *who)
{
	ZAsyncLocateData_t ald;
	
	if (ZRequestLocations(zephyr_normalize(who), &ald, UNACKED, ZAUTH)
					!= ZERR_NONE) {
		return;
	}
	pending_zloc_names = g_list_append(pending_zloc_names,
					g_strdup(zephyr_normalize(who)));
}

static GList *zephyr_buddy_menu(struct gaim_connection *gc, char *who)
{
	GList *m = NULL;
	struct proto_buddy_menu *pbm;

	pbm = g_new0(struct proto_buddy_menu, 1);
	pbm->label = _("ZLocate");
	pbm->callback = zephyr_zloc;
	pbm->gc = gc;
	m = g_list_append(m, pbm);

	return m;
}

static void zephyr_set_away(struct gaim_connection *gc, char *state, char *msg)
{
	if (gc->away)
		g_free(gc->away);
	gc->away = NULL;
	if (!g_strcasecmp(state, "Hidden"))
		ZSetLocation(EXPOSE_OPSTAFF);
	else if (!g_strcasecmp(state, "Online"))
		ZSetLocation(get_exposure_level());
	else /* state is GAIM_AWAY_CUSTOM */ if (msg)
		gc->away = g_strdup(msg);
}

static GList *zephyr_away_states()
{
	GList *m = NULL;

	m = g_list_append(m, "Online");
	m = g_list_append(m, GAIM_AWAY_CUSTOM);
	m = g_list_append(m, "Hidden");

	return m;
}

static GList *zephyr_chat_info(struct gaim_connection *gc) {
	GList *m = NULL;
	struct proto_chat_entry *pce;

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("Class:");
	m = g_list_append(m, NULL);

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("Instance:");
	m = g_list_append(m, NULL);

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("Recipient:");
	m = g_list_append(m, NULL);

	return m;
}

static void zephyr_join_chat(struct gaim_connection *gc, GList *data)
{
	ZSubscription_t sub;
	zephyr_triple *zt1, *zt2;
	const char *classname;
	const char *instname;
	const char *recip;

	if (!data || !data->next || !data->next->next)
		return;

	classname = data->data;
	instname = data->next->data;
	recip = data->next->next->data;
	if (!g_strcasecmp(recip, "%me%"))
		recip = g_getenv("USER");

	zt1 = new_triple(classname, instname, recip);
	zt2 = find_sub_by_triple(zt1);
	if (zt2) {
		free_triple(zt1);
		if (!zt2->open)
			serv_got_joined_chat(gc, zt2->id, zt2->name);
		return;
	}

	sub.zsub_class = zt1->class;
	sub.zsub_classinst = zt1->instance;
	sub.zsub_recipient = zt1->recipient;

	if (ZSubscribeTo(&sub, 1, 0) != ZERR_NONE) {
		free_triple(zt1);
		return;
	}

	subscrips = g_slist_append(subscrips, zt1);
	zt1->open = TRUE;
	serv_got_joined_chat(gc, zt1->id, zt1->name);
}

static void zephyr_chat_leave(struct gaim_connection *gc, int id)
{
	zephyr_triple *zt;
	zt = find_sub_by_id(id);
	if (zt) {
		zt->open = FALSE;
		zt->id = ++last_id;
	}
}

static struct prpl *my_protocol = NULL;

void zephyr_init(struct prpl *ret)
{
	ret->protocol = PROTO_ZEPHYR;
	ret->options = OPT_PROTO_NO_PASSWORD;
	ret->name = zephyr_name;
	ret->login = zephyr_login;
	ret->close = zephyr_close;
	ret->add_buddy = zephyr_add_buddy;
	ret->remove_buddy = zephyr_remove_buddy;
	ret->send_im = zephyr_send_im;
	ret->get_info = zephyr_zloc;
	ret->normalize = zephyr_normalize;
	ret->buddy_menu = zephyr_buddy_menu;
	ret->away_states = zephyr_away_states;
	ret->set_away = zephyr_set_away;
	ret->chat_info = zephyr_chat_info;
	ret->join_chat = zephyr_join_chat;
	ret->chat_send = zephyr_chat_send;
	ret->chat_leave = zephyr_chat_leave;

	my_protocol = ret;
}

#ifndef STATIC

char *gaim_plugin_init(GModule *handle)
{
	load_protocol(zephyr_init, sizeof(struct prpl));
	return NULL;
}

void gaim_plugin_remove()
{
	struct prpl *p = find_prpl(PROTO_ZEPHYR);
	if (p == my_protocol)
		unload_protocol(p);
}

char *name()
{
	return "Zephyr";
}

char *description()
{
	return PRPL_DESC("Zephyr");
}

#endif

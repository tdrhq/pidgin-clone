/*
 * gaim
 *
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
 *
 */

#ifndef _CORE_H_
#define _CORE_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_ICONV
#include <iconv.h>
#endif

#ifdef HAVE_LANGINFO_CODESET
#include <langinfo.h>
#endif

#include <stdio.h>
#include <time.h>
#include <glib.h>
#ifdef GAIM_PLUGINS
#include <gmodule.h>
#endif

#include "multi.h"

/* Really user states are controlled by the PRPLs now. We just use this for event_away */
#define UC_UNAVAILABLE  1

/* This is far too long to be practical, but MSN users are probably used to long aliases */
#define SELF_ALIAS_LEN 400

struct aim_user {
	char username[64];
	char alias[SELF_ALIAS_LEN]; 
	char password[32];
	char user_info[2048];
	int options;
	int protocol;
	/* prpls can use this to save information about the user,
	 * like which server to connect to, etc */
	char proto_opt[7][256];

	/* buddy icon file */
	char iconfile[256];

	struct gaim_connection *gc;
};
	
enum gaim_event {
	event_signon = 0,
	event_signoff,
	event_away,
	event_back,
	event_im_recv,
	event_im_send,
	event_buddy_signon,
	event_buddy_signoff,
	event_buddy_away,
	event_buddy_back,
	event_buddy_idle,
	event_buddy_unidle,
	event_blist_update,
	event_chat_invited,
	event_chat_join,
	event_chat_leave,
	event_chat_buddy_join,
	event_chat_buddy_leave,
	event_chat_recv,
	event_chat_send,
	event_warned,
	event_error,
	event_quit,
	event_new_conversation,
	event_set_info,
	event_draw_menu,
	event_im_displayed_sent,
	event_im_displayed_rcvd,
	event_chat_send_invite,
	event_got_typing,
	event_del_conversation,
	/* any others? it's easy to add... */
};

struct UI {
	GIOChannel *channel;
	guint inpa;
};

#ifdef GAIM_PLUGINS

struct gaim_plugin {
	GModule *handle;
	char *name;
	char *description;
};

struct gaim_callback {
	GModule *handle;
	enum gaim_event event;
	void *function;
	void *data;
};

#endif

#define BUDDY_ALIAS_MAXLEN 388	/* because MSN names can be 387 characters */

struct buddy {
	int edittype; /* XXX CUI: this is really a GUI function and we need to put this in ui.h */
	char name[80];
	char show[BUDDY_ALIAS_MAXLEN];
        int present;
	int evil;
	time_t signon;
	time_t idle;
        int uc;
	guint caps; /* woohoo! */
	void *proto_data; /* what a hack */
	struct gaim_connection *gc; /* the connection it belongs to */
};

struct group {
	int edittype; /* XXX CUI: this is really a GUI function and we need to put this in ui.h */
	char name[80];
	GSList *members;
	struct gaim_connection *gc; /* the connection it belongs to */
};

/* Globals in core.c */
extern GSList *uis;
extern int gaim_session;

/* Globals in plugins.c */
extern GList *plugins;
extern GList *callbacks;

/* Functions in buddy.c */
extern struct buddy *find_buddy(struct gaim_connection *, const char *);
extern struct group *find_group(struct gaim_connection *, const char *);
extern struct group *find_group_by_buddy(struct gaim_connection *, const char *);
extern struct buddy *add_buddy(struct gaim_connection *, const char *, const char *, const char *);
extern void remove_buddy(struct gaim_connection *, struct group *, struct buddy *);
extern struct group *add_group(struct gaim_connection *, const char *);
extern void remove_group(struct gaim_connection *, struct group *);
extern void do_export(struct gaim_connection *);
extern void do_import(struct gaim_connection *, const char *);
extern int bud_list_cache_exists(struct gaim_connection *);
extern void toc_build_config(struct gaim_connection *, char *, int len, gboolean);
extern void parse_toc_buddy_list(struct gaim_connection *, char *);
extern void signoff_blocked(struct gaim_connection *);

/* Functions in core.c */
extern gint UI_write(struct UI *, guchar *, int);
extern void UI_build_write(struct UI *, guchar, guchar, ...);
extern void UI_broadcast(guchar *data, int);
extern void UI_build_broadcast(guchar, guchar, ...);
/* Don't ever use these; when gaim-core is done these will be
 * merged into the core's main() and won't be called directly */
extern int core_main();
extern void core_quit();

/* Functions in gaimrc.c */
extern void load_prefs();
extern void save_prefs();

/* Functions in perl.c */
#ifdef USE_PERL
extern void perl_autoload();
extern void perl_end();
extern int perl_event(enum gaim_event, void *, void *, void *, void *);
extern int perl_load_file(char *);
extern void unload_perl_scripts();
extern void list_perl_scripts();
#endif

/* Functions in plugins.c */
#ifdef GAIM_PLUGINS
extern struct gaim_plugin *load_plugin(const char *);
extern void unload_plugin(struct gaim_plugin *);
extern struct gaim_plugin *reload_plugin(struct gaim_plugin *);
extern void gaim_signal_connect(GModule *, enum gaim_event, void *, void *);
extern void gaim_signal_disconnect(GModule *, enum gaim_event, void *);
extern void gaim_plugin_unload(GModule *);
extern void remove_all_plugins();
#endif
extern int plugin_event(enum gaim_event, void *, void *, void *, void *);
extern char *event_name(enum gaim_event);

/* Functions in server.c */
extern void serv_got_update(struct gaim_connection *, char *, int, int, time_t, time_t, int, guint);
extern void serv_got_im(struct gaim_connection *, char *, char *, guint32, time_t, gint);
extern void serv_got_typing(struct gaim_connection *, char *, int);
extern void serv_got_typing_stopped(struct gaim_connection *, char *);
extern void serv_got_eviled(struct gaim_connection *, char *, int);
extern void serv_got_chat_invite(struct gaim_connection *, char *, char *, char *, GList *);
extern struct conversation *serv_got_joined_chat(struct gaim_connection *, int, char *);
extern void serv_got_chat_left(struct gaim_connection *, int);
extern void serv_got_chat_in(struct gaim_connection *, int, char *, int, char *, time_t);
extern void serv_finish_login();

#endif /* _CORE_H_ */

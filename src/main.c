/*
 * gaim
 *
 * Gaim is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
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

#include "gtkinternal.h"

#include "account.h"
#include "conversation.h"
#include "core.h"
#include "debug.h"
#include "ft.h"
#include "log.h"
#include "notify.h"
#include "prefs.h"
#include "prpl.h"
#include "pounce.h"
#include "sound.h"
#include "status.h"
#include "util.h"

#include "gtkaccount.h"
#include "gtkblist.h"
#include "gtkconn.h"
#include "gtkconv.h"
#include "gtkdebug.h"
#include "gtkft.h"
#include "gtknotify.h"
#include "gtkplugin.h"
#include "gtkpounce.h"
#include "gtkprefs.h"
#include "gtkprivacy.h"
#include "gtkrequest.h"
#include "gtkroomlist.h"
#include "gtksound.h"
#include "gtkutils.h"
#include "stock.h"

#include "ui.h"

#if HAVE_SIGNAL_H
# include <signal.h>
#endif

#include "locale.h"
#include <getopt.h>

#ifdef HAVE_STARTUP_NOTIFICATION
# define SN_API_NOT_YET_FROZEN
# include <libsn/sn-launchee.h>
# include <gdk/gdkx.h>
#endif

extern void load_prefs();
extern void load_pounces();

static GtkWidget *name;
static GtkWidget *pass;

#ifdef HAVE_STARTUP_NOTIFICATION
static SnLauncheeContext *sn_context = NULL;
static SnDisplay *sn_display = NULL;
#endif

GtkWidget *mainwindow = NULL;

int opt_away = 0;
int docklet_count = 0;
char *opt_away_arg = NULL;
char *opt_rcfile_arg = NULL;
int opt_debug = 0;

#if HAVE_SIGNAL_H
/*
 * Lists of signals we wish to catch and those we wish to ignore.
 * Each list terminated with -1
 */
static int catch_sig_list[] = {
	SIGSEGV,
	SIGHUP,
	SIGINT,
	SIGTERM,
	SIGQUIT,
	SIGCHLD,
	-1
};

static int ignore_sig_list[] = {
	SIGPIPE,
	-1
};
#endif

static guint snd_tmout = 0;
static gboolean sound_timeout(gpointer data)
{
	gaim_gtk_sound_set_login_mute(FALSE);
	snd_tmout = 0;
	return FALSE;
}

/* we need to do this for Oscar because serv_login only starts the login
 * process, it doesn't end there. gaim_setup will be called later from
 * oscar.c, after the buddy list is made and serv_finish_login is called */
void gaim_setup(GaimConnection *gc)
{
	if (gaim_prefs_get_bool("/gaim/gtk/sound/enabled/login")
			&& gaim_prefs_get_bool("/gaim/gtk/sound/silent_signon")) {
		if(snd_tmout) {
			g_source_remove(snd_tmout);
		}
		gaim_gtk_sound_set_login_mute(TRUE);
		snd_tmout = g_timeout_add(10000, sound_timeout, NULL);
	}
}

static gboolean domiddleclick(GtkWidget *w, GdkEventButton *event, gpointer null)
{
	if (event->button != 2)
		return FALSE;

	gaim_accounts_auto_login(GAIM_GTK_UI);

	return TRUE;
}

static void dologin(GtkWidget *widget, GtkWidget *w)
{
	GaimAccount *account;
	GtkWidget *item;
	const char *password = gtk_entry_get_text(GTK_ENTRY(pass));

	item = gtk_menu_get_active(GTK_MENU(gtk_option_menu_get_menu(GTK_OPTION_MENU(name))));
	account = g_object_get_data(G_OBJECT(item), "account");

	if (!account) {
		gaim_notify_error(NULL, NULL, _("Please create an account."), NULL);
		return;
	}

	gaim_account_set_password(account, (*password != '\0') ? password : NULL);

	gaim_account_connect(account);
}

/* <name> is a comma-separated list of names, or NULL
   if NULL and there is at least one user defined in .gaimrc, try to login.
   if not NULL, parse <name> into separate strings, look up each one in 
   .gaimrc and, if it's there, try to login.
   returns:  0 if successful
            -1 if no user was found that had a saved password
*/
static int dologin_named(char *name)
{
	GaimAccount *account;
	char **names, **n;
	int retval = -1;

	if (name !=NULL) {	/* list of names given */
		names = g_strsplit(name, ",", 32);
		for (n = names; *n != NULL; n++) {
			account = gaim_accounts_find(*n, NULL);
			if (account) {	/* found a user */
				retval = 0;
				gaim_account_connect(account);
			}
		}
		g_strfreev(names);
	} else {		/* no name given, use default */
		account = (GaimAccount *)gaim_accounts_get_all()->data;
		retval = 0;
		gaim_account_connect(account);
	}

	return retval;
}


static void combo_changed(GtkWidget *menu, GaimAccount *account, gpointer data)
{
	if (account && gaim_account_get_remember_password(account)) {
		gtk_entry_set_text(GTK_ENTRY(pass), account->password);
	} else {
		gtk_entry_set_text(GTK_ENTRY(pass), "");
	}
}


static void login_window_closed(GtkWidget *w, GdkEvent *ev, gpointer d)
{
	if(docklet_count) {
#ifdef _WIN32
		wgaim_systray_minimize(mainwindow);
#endif
		gtk_widget_hide(mainwindow);
	} else
		gaim_core_quit();
}

void show_login()
{
	GtkWidget *image;
	GtkWidget *vbox;
	GtkWidget *button;
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *vbox2;

	/* Do we already have a main window opened? If so, bring it back, baby... ribs... yeah */
	if (mainwindow) {
			gtk_window_present(GTK_WINDOW(mainwindow));
			return;
	}

	mainwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_role(GTK_WINDOW(mainwindow), "login");
	gtk_window_set_resizable(GTK_WINDOW(mainwindow), FALSE);
	gtk_window_set_title(GTK_WINDOW(mainwindow), _("Login"));
	gtk_widget_realize(mainwindow);
	gdk_window_set_group(mainwindow->window, mainwindow->window);
	gtk_container_set_border_width(GTK_CONTAINER(mainwindow), 5);
	g_signal_connect(G_OBJECT(mainwindow), "delete_event",
					 G_CALLBACK(login_window_closed), mainwindow);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(mainwindow), vbox);

	image = gtk_image_new_from_stock(GAIM_STOCK_LOGO, gtk_icon_size_from_name(GAIM_ICON_SIZE_LOGO));
	gtk_box_pack_start(GTK_BOX(vbox), image, FALSE, FALSE, 0);

	vbox2 = gtk_vbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(vbox2), 5);

	/* why isn't there a gtk_label_new_with_markup? */
	label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label), _("<b>Screen Name:</b>"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(vbox2), label, FALSE, FALSE, 0);

	name = gaim_gtk_account_option_menu_new(NULL, TRUE, G_CALLBACK(combo_changed), NULL, NULL);

	gtk_box_pack_start(GTK_BOX(vbox2), name, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), vbox2, FALSE, TRUE, 0);

	vbox2 = gtk_vbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(vbox2), 5);

	label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label), _("<b>Password:</b>"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(vbox2), label, FALSE, FALSE, 0);

	pass = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(pass), FALSE);
	g_signal_connect(G_OBJECT(pass), "activate",
					 G_CALLBACK(dologin), mainwindow);
	gtk_box_pack_start(GTK_BOX(vbox2), pass, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), vbox2, FALSE, TRUE, 0);

	/* Now for the button box */
	hbox = gtk_hbox_new(TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 5);

	/* And now for the buttons */
	button = gaim_pixbuf_button_from_stock(_("Accounts"), GAIM_STOCK_ACCOUNTS, GAIM_BUTTON_VERTICAL);
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(gaim_gtk_accounts_window_show), mainwindow);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

	button = gaim_pixbuf_button_from_stock(_("Preferences"), GTK_STOCK_PREFERENCES, GAIM_BUTTON_VERTICAL);
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(gaim_gtk_prefs_show), mainwindow);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

	button = gaim_pixbuf_button_from_stock(_("Sign on"), GAIM_STOCK_SIGN_ON, GAIM_BUTTON_VERTICAL);
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(dologin), mainwindow);
	g_signal_connect(G_OBJECT(button), "button-press-event", G_CALLBACK(domiddleclick), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

	/* Now grab the focus that we need */
	if (gaim_accounts_get_all()) {
		GaimAccount *account = gaim_accounts_get_all()->data;

		if (gaim_account_get_remember_password(account)) {
			combo_changed(NULL, account, NULL);
			gtk_widget_grab_focus(button);
		} else {
			gtk_widget_grab_focus(pass);
		}
	} else {
		gaim_gtk_accounts_window_show();
		gtk_widget_grab_focus(button);
	}

	/* And raise the curtain! */
	gtk_widget_show_all(mainwindow);

}

static void
clean_pid(void)
{
#ifndef _WIN32
	int status;
	pid_t pid;

	do {
		pid = waitpid(-1, &status, WNOHANG);
	} while (pid != 0 && pid != (pid_t)-1);
	if(pid == (pid_t)-1 && errno != ECHILD) {
		char errmsg[BUFSIZ];
		snprintf(errmsg, BUFSIZ, "Warning: waitpid() returned %d", pid);
		perror(errmsg);
	}
#endif
}

#if HAVE_SIGNAL_H
void sighandler(int sig)
{
	switch (sig) {
	case SIGHUP:
		gaim_debug(GAIM_DEBUG_WARNING, "sighandler",
				   "Caught signal %d\n", sig);
		gaim_connections_disconnect_all();
		break;
	case SIGSEGV:
#ifndef DEBUG
		fprintf(stderr, "Gaim has segfaulted and attempted to dump a core file.\n"
			"This is a bug in the software and has happened through\n"
			"no fault of your own.\n\n"
			"It is possible that this bug is already fixed in CVS.\n"
			"If you can reproduce the crash, please notify the gaim\n" 
			"maintainers by reporting a bug at\n"
			GAIM_WEBSITE "bug.php\n\n"
			"Please make sure to specify what you were doing at the time,\n"
			"and post the backtrace from the core file. If you do not know\n"
			"how to get the backtrace, please get instructions at\n"
			GAIM_WEBSITE "gdb.php. If you need further\n"
			"assistance, please IM either RobFlynn or SeanEgn and\n"
			"they can help you.\n");
#else
		fprintf(stderr, "Oh no!  Segmentation fault!\n");
		/*g_on_error_query (g_get_prgname());*/
#endif
		abort();
		break;
	case SIGCHLD:
		clean_pid();
#if HAVE_SIGNAL_H
		signal(SIGCHLD, sighandler);    /* restore signal catching on this one! */
#endif
		break;
	default:
		gaim_debug(GAIM_DEBUG_WARNING, "sighandler",
				   "Caught signal %d\n", sig);
		gaim_connections_disconnect_all();

		gaim_plugins_unload_all();

		if (gtk_main_level())
			gtk_main_quit();
		exit(0);
	}
}
#endif

static int ui_main()
{
#ifndef _WIN32
	GList *icons = NULL;
	GdkPixbuf *icon = NULL;
	char *icon_path;
#endif

	if (current_smiley_theme == NULL) {
		smiley_theme_probe();
		if (smiley_themes) {
			struct smiley_theme *smile = smiley_themes->data;
			load_smiley_theme(smile->path, TRUE);
		}
	}

	gaim_gtk_blist_setup_sort_methods();

#ifndef _WIN32
	/* use the nice PNG icon for all the windows */
	icon_path = g_build_filename(DATADIR, "pixmaps", "gaim", "icons", "online.png", NULL);
	icon = gdk_pixbuf_new_from_file(icon_path, NULL);
	g_free(icon_path);
	if (icon) {
		icons = g_list_append(icons,icon);
		gtk_window_set_default_icon_list(icons);
		g_object_unref(G_OBJECT(icon));
		g_list_free(icons);
	} else {
		gaim_debug(GAIM_DEBUG_ERROR, "ui_main",
				   "Failed to load the default window icon!\n");
	}
#endif

	return 0;
}

static void set_first_user(char *name)
{
	GaimAccount *account;

	account = gaim_accounts_find(name, NULL);

	if (account == NULL) { /* new user */
		account = gaim_account_new(name, GAIM_PROTO_DEFAULT);
		gaim_accounts_add(account);
	}

	/* Place it as the first user. */
	gaim_accounts_reorder(account, 0);
}

static void
debug_init(void)
{
	gaim_debug_set_ui_ops(gaim_gtk_debug_get_ui_ops());
	gaim_gtk_debug_init();
}

static void
gaim_gtk_ui_init(void)
{
	/* Set the UI operation structures. */
	gaim_accounts_set_ui_ops(gaim_gtk_accounts_get_ui_ops());
	gaim_conversations_set_win_ui_ops(gaim_gtk_conversations_get_win_ui_ops());
	gaim_xfers_set_ui_ops(gaim_gtk_xfers_get_ui_ops());
	gaim_blist_set_ui_ops(gaim_gtk_blist_get_ui_ops());
	gaim_notify_set_ui_ops(gaim_gtk_notify_get_ui_ops());
	gaim_privacy_set_ui_ops(gaim_gtk_privacy_get_ui_ops());
	gaim_request_set_ui_ops(gaim_gtk_request_get_ui_ops());
	gaim_sound_set_ui_ops(gaim_gtk_sound_get_ui_ops());
	gaim_connections_set_ui_ops(gaim_gtk_connections_get_ui_ops());

	gaim_gtk_stock_init();
	gaim_gtk_prefs_init();
	gaim_gtk_blist_init();
	gaim_gtk_conversations_init();
	gaim_gtk_pounces_init();
	gaim_gtk_privacy_init();
	gaim_gtk_xfers_init();
	gaim_gtk_roomlist_init();
}

static void
gaim_gtk_quit(void)
{
	/* XXX? */

	/* captain's log, stardate... */
	/* LOG system_log(log_quit, NULL, NULL, OPT_LOG_BUDDY_SIGNON | OPT_LOG_MY_SIGNON); */

#ifdef USE_SM
	/* unplug */
	session_end();
#endif

	/* Save the plugins we have loaded for next time. */
	gaim_gtk_plugins_save();

	/* and end it all... */
	gtk_main_quit();
}

static GaimCoreUiOps core_ops =
{
	gaim_gtk_prefs_init,
	debug_init,
	gaim_gtk_ui_init,
	gaim_gtk_quit
};

static GaimCoreUiOps *
gaim_gtk_core_get_ui_ops(void)
{
	return &core_ops;
}

static void
show_usage(int mode, const char *name)
{
	char *text=NULL;

	switch (mode) {
	case 0:		/* full help text */
		text=g_strdup_printf(_("Gaim %s\n"
		       "Usage: %s [OPTION]...\n\n"
		       "  -a, --acct          display account editor window\n"
		       "  -w, --away[=MESG]   make away on signon (optional argument MESG specifies\n"
		       "                      name of away message to use)\n"
		       "  -l, --login[=NAME]  automatically login (optional argument NAME specifies\n"
		       "                      account(s) to use, seperated by commas)\n"
		       "  -n, --loginwin      don't automatically login; show login window\n"
		       "  -u, --user=NAME     use account NAME\n"
		       "  -f, --file=FILE     use FILE as config\n"
		       "  -d, --debug         print debugging messages to stdout\n"
		       "  -v, --version       display the current version and exit\n"
		       "  -h, --help          display this help and exit\n"), VERSION, name);
		break;
	case 1:		/* short message */
		text=g_strdup_printf(_("Gaim %s. Try `%s -h' for more information.\n"), VERSION, name);
		break;
	}

	if(text) {
		char *text_conv;
		GError *error=NULL;

		/* tries to convert 'text' to users locale */
		text_conv=g_locale_from_utf8(text,-1,NULL,NULL,&error);
		if(text_conv) {
			puts(text_conv);
			g_free(text_conv);
		}
		/* use 'text' as a fallback */
		else {
			g_warning("%s\n", error->message);
			g_error_free(error);
			puts(text);
		}
		g_free(text);
	}
}

#ifdef HAVE_STARTUP_NOTIFICATION
static void
sn_error_trap_push(SnDisplay *display, Display *xdisplay)
{
	gdk_error_trap_push();
}

static void
sn_error_trap_pop(SnDisplay *display, Display *xdisplay)
{
	gdk_error_trap_pop();
}

static void
startup_notification_complete(void)
{
	Display *xdisplay;

	xdisplay = GDK_DISPLAY();
	sn_display = sn_display_new(xdisplay,
								sn_error_trap_push,
								sn_error_trap_pop);
	sn_context =
		sn_launchee_context_new_from_environment(sn_display,
												 DefaultScreen(xdisplay));

	if (sn_context != NULL)
	{
		sn_launchee_context_complete(sn_context);
		sn_launchee_context_unref(sn_context);

		sn_display_unref(sn_display);
	}
}
#endif /* HAVE_STARTUP_NOTIFICATION */

/* FUCKING GET ME A TOWEL! */
#ifdef _WIN32
int gaim_main(HINSTANCE hint, int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	int opt_acct = 0, opt_help = 0, opt_version = 0, opt_login = 0, opt_nologin = 0, dologin_ret = -1;
	char *opt_user_arg = NULL, *opt_login_arg = NULL;
	char *opt_session_arg = NULL;
	char *plugin_search_paths[3];
#if HAVE_SIGNAL_H
	int sig_indx;	/* for setting up signal catching */
	sigset_t sigset;
	void (*prev_sig_disp)();
#endif
	int opt, opt_user = 0;
	int i;
	gboolean gui_check;

	struct option long_options[] = {
		{"acct", no_argument, NULL, 'a'},
		/*{"away", optional_argument, NULL, 'w'}, */
		{"help", no_argument, NULL, 'h'},
		/*{"login", optional_argument, NULL, 'l'}, */
		{"loginwin", no_argument, NULL, 'n'},
		{"user", required_argument, NULL, 'u'},
		{"file", required_argument, NULL, 'f'},
		{"debug", no_argument, NULL, 'd'},
		{"version", no_argument, NULL, 'v'},
		{"session", required_argument, NULL, 's'},
		{0, 0, 0, 0}
	};

#ifdef DEBUG
	opt_debug = 1;
#endif
#ifdef ENABLE_NLS
	bindtextdomain(PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(PACKAGE, "UTF-8");
	textdomain(PACKAGE);
#endif

#if HAVE_SIGNAL_H
	/* Let's not violate any PLA's!!!! */
	/* jseymour: whatever the fsck that means */
	/* Robot101: for some reason things like gdm like to block     *
	 * useful signals like SIGCHLD, so we unblock all the ones we  *
	 * declare a handler for. thanks JSeymour and Vann.            */
	if (sigemptyset(&sigset)) {
		char errmsg[BUFSIZ];
		snprintf(errmsg, BUFSIZ, "Warning: couldn't initialise empty signal set");
		perror(errmsg);
	}
	for(sig_indx = 0; catch_sig_list[sig_indx] != -1; ++sig_indx) {
		if((prev_sig_disp = signal(catch_sig_list[sig_indx], sighandler)) == SIG_ERR) {
			char errmsg[BUFSIZ];
			snprintf(errmsg, BUFSIZ, "Warning: couldn't set signal %d for catching",
				catch_sig_list[sig_indx]);
			perror(errmsg);
		}
		if(sigaddset(&sigset, catch_sig_list[sig_indx])) {
			char errmsg[BUFSIZ];
			snprintf(errmsg, BUFSIZ, "Warning: couldn't include signal %d for unblocking",
				catch_sig_list[sig_indx]);
			perror(errmsg);
		}
	}
	for(sig_indx = 0; ignore_sig_list[sig_indx] != -1; ++sig_indx) {
		if((prev_sig_disp = signal(ignore_sig_list[sig_indx], SIG_IGN)) == SIG_ERR) {
			char errmsg[BUFSIZ];
			snprintf(errmsg, BUFSIZ, "Warning: couldn't set signal %d to ignore",
				ignore_sig_list[sig_indx]);
			perror(errmsg);
		}
	}

	if (sigprocmask(SIG_UNBLOCK, &sigset, NULL)) {
		char errmsg[BUFSIZ];
		snprintf(errmsg, BUFSIZ, "Warning: couldn't unblock signals");
		perror(errmsg);
	}		
#endif

	for (i = 0; i < argc; i++) {
		/* --login option */
		if (strstr(argv[i], "--l") == argv[i]) {
			char *equals;
			opt_login = 1;
			if ((equals = strchr(argv[i], '=')) != NULL) {
				/* --login=NAME */
				opt_login_arg = g_strdup(equals + 1);
				if (strlen(opt_login_arg) == 0) {
					g_free(opt_login_arg);
					opt_login_arg = NULL;
				}
			} else if (i + 1 < argc && argv[i + 1][0] != '-') {
				/* --login NAME */
				opt_login_arg = g_strdup(argv[i + 1]);
				strcpy(argv[i + 1], " ");
			}
			strcpy(argv[i], " ");
		}
		/* -l option */
		else if (strstr(argv[i], "-l") == argv[i]) {
			opt_login = 1;
			if (strlen(argv[i]) > 2) {
				/* -lNAME */
				opt_login_arg = g_strdup(argv[i] + 2);
			} else if (i + 1 < argc && argv[i + 1][0] != '-') {
				/* -l NAME */
				opt_login_arg = g_strdup(argv[i + 1]);
				strcpy(argv[i + 1], " ");
			}
			strcpy(argv[i], " ");
		}
		/* --away option */
		else if (strstr(argv[i], "--aw") == argv[i]) {
			char *equals;
			opt_away = 1;
			if ((equals = strchr(argv[i], '=')) != NULL) {
				/* --away=MESG */
				opt_away_arg = g_strdup(equals + 1);
				if (strlen(opt_away_arg) == 0) {
					g_free(opt_away_arg);
					opt_away_arg = NULL;
				}
			} else if (i + 1 < argc && argv[i + 1][0] != '-') {
				/* --away MESG */
				opt_away_arg = g_strdup(argv[i + 1]);
				strcpy(argv[i + 1], " ");
			}
			strcpy(argv[i], " ");
		}
		/* -w option */
		else if (strstr(argv[i], "-w") == argv[i]) {
			opt_away = 1;
			if (strlen(argv[i]) > 2) {
				/* -wMESG */
				opt_away_arg = g_strdup(argv[i] + 2);
			} else if (i + 1 < argc && argv[i + 1][0] != '-') {
				/* -w MESG */
				opt_away_arg = g_strdup(argv[i + 1]);
				strcpy(argv[i + 1], " ");
			}
			strcpy(argv[i], " ");
		}
	}
	/*
	   if (opt_login) {
	   printf ("--login given with arg %s\n",
	   opt_login_arg ? opt_login_arg : "NULL");
	   exit(0);
	   }
	 */

	gui_check = gtk_init_check(&argc, &argv);

	/* scan command-line options */
	opterr = 1;
	while ((opt = getopt_long(argc, argv,
#ifndef _WIN32
				  "adhu:f:vns:", 
#else
				  "adhu:f:vn", 
#endif
				  long_options, NULL)) != -1) {
		switch (opt) {
		case 'u':	/* set user */
			opt_user = 1;
			opt_user_arg = g_strdup(optarg);
			break;
		case 'a':	/* account editor */
			opt_acct = 1;
			break;
		case 'd':	/* debug */
			opt_debug = 1;
			break;
		case 'f':
			opt_rcfile_arg = g_strdup(optarg);
			break;
		case 's':	/* use existing session ID */
			opt_session_arg = g_strdup(optarg);
			break;
		case 'v':	/* version */
			opt_version = 1;
			break;
		case 'h':	/* help */
			opt_help = 1;
			break;
		case 'n':       /* don't autologin */
			opt_nologin = 1;
			break;
		case '?':
		default:
			show_usage(1, argv[0]);
			return 0;
			break;
		}
	}

	/* show help message */
	if (opt_help) {
		show_usage(0, argv[0]);
		return 0;
	}
	/* show version message */
	if (opt_version) {
		printf("Gaim %s\n",VERSION);
		return 0;
	}

	if (!gui_check) {
		char *display = gdk_get_display();

		g_warning("cannot open display: %s", display ? display : "unset");
		g_free(display);

		return 1;
	}

#ifdef _WIN32
        wgaim_init(hint);
#endif
	gaim_core_set_ui_ops(gaim_gtk_core_get_ui_ops());

	if (!gaim_core_init(GAIM_GTK_UI)) {
		fprintf(stderr,
				"Initialization of the Gaim core failed. Dumping core.\n"
				"Please report this!\n");
		abort();
	}

	plugin_search_paths[0] = LIBDIR;
	plugin_search_paths[1] = gaim_user_dir();
	plugin_search_paths[2] = g_build_filename(gaim_user_dir(), "plugins", NULL);

	gaim_plugins_set_search_paths(sizeof(plugin_search_paths) /
								  sizeof(*plugin_search_paths),
								  plugin_search_paths);

	g_free(plugin_search_paths[2]);

	gaim_plugins_probe(NULL);

	gaim_prefs_load();

	/* we only read ~/.gaimrc (load_prefs()) if there is no accounts.xml
	 * since prefs.xml existed alongside ~/.gaim in 0.64 */
	if (!gaim_accounts_load()) {
		load_prefs();
		gaim_prefs_sync();
	}

	gaim_prefs_rename_old();

	/* load plugins we had when we quit */
	gaim_plugins_load_saved("/gaim/gtk/plugins/loaded");

	gaim_pounces_load();
	gaim_status_load();

	gaim_set_blist(gaim_blist_new());
	gaim_blist_load();

	load_pounces();
	ui_main();

#ifdef USE_SM
	session_init(argv[0], opt_session_arg);
#endif
	if (opt_session_arg != NULL) {
		g_free(opt_session_arg);
		opt_session_arg = NULL;
	};

	/* set the default username */
	if (opt_user_arg != NULL) {
		set_first_user(opt_user_arg);
		g_free(opt_user_arg);
		opt_user_arg = NULL;
	}

	if (gaim_prefs_get_bool("/gaim/gtk/debug/enabled"))
		gaim_gtk_debug_window_show();

	/* deal with --login */
	if (opt_login) {
		dologin_ret = dologin_named(opt_login_arg);
		if (opt_login_arg != NULL) {
			g_free(opt_login_arg);
			opt_login_arg = NULL;
		}
	}

	if (!opt_acct && !opt_nologin)
		gaim_accounts_auto_login(GAIM_GTK_UI);

	if (opt_acct) {
		gaim_gtk_accounts_window_show();
	} else if ((dologin_ret == -1) && !gaim_connections_get_all())
		show_login();

#ifdef HAVE_STARTUP_NOTIFICATION
	startup_notification_complete();
#endif

	gtk_main();
	gaim_sound_shutdown();
#ifdef _WIN32
	wgaim_cleanup();
#endif
	return 0;

}

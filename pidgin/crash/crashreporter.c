
#include <glib.h>
#include <stdio.h>
#include "crashreporter.h"

#include "internal.h"
#include "pidgin.h"
#include <gtk/gtk.h>

#include "core.h"
#include "pidgin/gtkutils.h"
#include "notify.h"

static GtkWidget *window_;

static PurpleCoreUiOps core_ops =
{
	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL
};

static PurpleEventLoopUiOps eventloop_ops =
{
	g_timeout_add,
	g_source_remove,
	NULL,
	g_source_remove,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static gboolean
has_file_extension(const char *filename, const char *ext)
{
	int len, extlen;

	if (filename == NULL || *filename == '\0' || ext == NULL)
		return 0;

	extlen = strlen(ext);
	len = strlen(filename) - extlen;

	if (len < 0)
		return 0;

	return (strncmp(filename + len, ext, extlen) == 0);
}

/**
 * Makes a frame to display content in
 * Derived from pidgin_make_frame
 */
static GtkWidget *
make_frame(GtkWidget *parent, const char *title)
{
	GtkWidget *vbox, *label, *hbox;
	char *labeltitle;

	vbox = gtk_vbox_new(FALSE, PIDGIN_HIG_BOX_SPACE);
	gtk_box_pack_start(GTK_BOX(parent), vbox, FALSE, FALSE, 0);
	gtk_widget_show(vbox);

	label = gtk_label_new(NULL);

	labeltitle = g_strdup_printf("<span weight=\"bold\">%s</span>", title);
	gtk_label_set_markup(GTK_LABEL(label), labeltitle);
	g_free(labeltitle);

	gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
	gtk_widget_show(label);

	hbox = gtk_hbox_new(FALSE, PIDGIN_HIG_BOX_SPACE);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show(hbox);

	label = gtk_label_new("    ");
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_widget_show(label);

	vbox = gtk_vbox_new(FALSE, PIDGIN_HIG_BOX_SPACE);
	gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
	gtk_widget_show(vbox);

	return vbox;
}
#ifndef _WIN32
static gint
uri_command(const char *command, gboolean sync)
{
	gchar *tmp;
	GError *error = NULL;
	gint ret = 0;

	purple_debug_misc("gtknotify", "Executing %s\n", command);

	if (!purple_program_is_valid(command))
	{
		tmp = g_strdup_printf(_("The browser command \"%s\" is invalid."),
							  command ? command : "(none)");
		purple_notify_error(NULL, NULL, _("Unable to open URL"), tmp);
		g_free(tmp);

	}
	else if (sync)
	{
		gint status;

		if (!g_spawn_command_line_sync(command, NULL, NULL, &status, &error))
		{
			tmp = g_strdup_printf(_("Error launching \"%s\": %s"),
										command, error->message);
			purple_notify_error(NULL, NULL, _("Unable to open URL"), tmp);
			g_free(tmp);
			g_error_free(error);
		}
		else
			ret = status;
	}
	else
	{
		if (!g_spawn_command_line_async(command, &error))
		{
			tmp = g_strdup_printf(_("Error launching \"%s\": %s"),
										command, error->message);
			purple_notify_error(NULL, NULL, _("Unable to open URL"), tmp);
			g_free(tmp);
			g_error_free(error);
		}
	}

	return ret;
}
#endif /* _WIN32 */

/**
 * Opens webpage in new browser window
 * Derived from pidgin_notify_uri()
 */
static void *
notify_uri(const char *uri)
{
#ifndef _WIN32
	char *escaped = g_shell_quote(uri);
	char *command = NULL;
	char *remote_command = NULL;
	const char *web_browser;
	int place;

	web_browser = purple_prefs_get_string(PIDGIN_PREFS_ROOT "/browsers/browser");
	place = purple_prefs_get_int(PIDGIN_PREFS_ROOT "/browsers/place");

	/* if they are running gnome, use the gnome web browser */
	if (purple_running_gnome() == TRUE)
	{
		char *tmp = g_find_program_in_path("xdg-open");
		if (tmp == NULL)
			command = g_strdup_printf("gnome-open %s", escaped);
		else
			command = g_strdup_printf("xdg-open %s", escaped);
		g_free(tmp);
	}
	else if (purple_running_osx() == TRUE)
	{
		command = g_strdup_printf("open %s", escaped);
	}
	else if (!strcmp(web_browser, "epiphany") ||
		!strcmp(web_browser, "galeon"))
	{
		if (place == PIDGIN_BROWSER_NEW_WINDOW)
			command = g_strdup_printf("%s -w %s", web_browser, escaped);
		else if (place == PIDGIN_BROWSER_NEW_TAB)
			command = g_strdup_printf("%s -n %s", web_browser, escaped);
		else
			command = g_strdup_printf("%s %s", web_browser, escaped);
	}
	else if (!strcmp(web_browser, "xdg-open"))
	{
		command = g_strdup_printf("xdg-open %s", escaped);
	}
	else if (!strcmp(web_browser, "gnome-open"))
	{
		command = g_strdup_printf("gnome-open %s", escaped);
	}
	else if (!strcmp(web_browser, "kfmclient"))
	{
		command = g_strdup_printf("kfmclient openURL %s", escaped);
		/*
		 * Does Konqueror have options to open in new tab
		 * and/or current window?
		 */
	}
	else if (!strcmp(web_browser, "mozilla") ||
			 !strcmp(web_browser, "mozilla-firebird") ||
			 !strcmp(web_browser, "firefox") ||
			 !strcmp(web_browser, "seamonkey"))
	{
		char *args = "";

		command = g_strdup_printf("%s %s", web_browser, escaped);

		/*
		 * Firefox 0.9 and higher require a "-a firefox" option when
		 * using -remote commands.  This breaks older versions of
		 * mozilla.  So we include this other handly little string
		 * when calling firefox.  If the API for remote calls changes
		 * any more in firefox then firefox should probably be split
		 * apart from mozilla-firebird and mozilla... but this is good
		 * for now.
		 */
		if (!strcmp(web_browser, "firefox"))
			args = "-a firefox";

		if (place == PIDGIN_BROWSER_NEW_WINDOW)
			remote_command = g_strdup_printf("%s %s -remote "
											 "openURL(%s,new-window)",
											 web_browser, args, escaped);
		else if (place == PIDGIN_BROWSER_NEW_TAB)
			remote_command = g_strdup_printf("%s %s -remote "
											 "openURL(%s,new-tab)",
											 web_browser, args, escaped);
		else if (place == PIDGIN_BROWSER_CURRENT)
			remote_command = g_strdup_printf("%s %s -remote "
											 "openURL(%s)",
											 web_browser, args, escaped);
	}
	else if (!strcmp(web_browser, "netscape"))
	{
		command = g_strdup_printf("netscape %s", escaped);

		if (place == PIDGIN_BROWSER_NEW_WINDOW)
		{
			remote_command = g_strdup_printf("netscape -remote "
											 "openURL(%s,new-window)",
											 escaped);
		}
		else if (place == PIDGIN_BROWSER_CURRENT)
		{
			remote_command = g_strdup_printf("netscape -remote "
											 "openURL(%s)", escaped);
		}
	}
	else if (!strcmp(web_browser, "opera"))
	{
		if (place == PIDGIN_BROWSER_NEW_WINDOW)
			command = g_strdup_printf("opera -newwindow %s", escaped);
		else if (place == PIDGIN_BROWSER_NEW_TAB)
			command = g_strdup_printf("opera -newpage %s", escaped);
		else if (place == PIDGIN_BROWSER_CURRENT)
		{
			remote_command = g_strdup_printf("opera -remote "
											 "openURL(%s)", escaped);
			command = g_strdup_printf("opera %s", escaped);
		}
		else
			command = g_strdup_printf("opera %s", escaped);

	}
	else if (!strcmp(web_browser, "custom"))
	{
		const char *web_command;

		web_command = purple_prefs_get_path(PIDGIN_PREFS_ROOT "/browsers/command");

		if (web_command == NULL || *web_command == '\0')
		{
			purple_notify_error(NULL, NULL, _("Unable to open URL"),
							  _("The 'Manual' browser command has been "
								"chosen, but no command has been set."));
			return NULL;
		}

		if (strstr(web_command, "%s"))
			command = purple_strreplace(web_command, "%s", escaped);
		else
		{
			/*
			 * There is no "%s" in the browser command.  Assume the user
			 * wanted the URL tacked on to the end of the command.
			 */
			command = g_strdup_printf("%s %s", web_command, escaped);
		}
	}

	g_free(escaped);

	if (remote_command != NULL)
	{
		/* try the remote command first */
		if (uri_command(remote_command, TRUE) != 0)
			uri_command(command, FALSE);

		g_free(remote_command);

	}
	else
		uri_command(command, FALSE);

	g_free(command);

#else /* !_WIN32 */
	winpidgin_notify_uri(uri);
#endif /* !_WIN32 */

	return NULL;
}

/**
 * Returns a hashtable mapping strings (library name) to PurplePlugin objects.
 * Derived from purple_probe_plugins()
 */
static GHashTable*
probe_plugins () 
{
	gchar *search_path;
	GDir *dir;
	const gchar *file;
	gchar *path;
	PurplePlugin *plugin;
	GList *cur;
	GHashTable *plugins;
	GList *plugin_search_paths = NULL;

	plugins = g_hash_table_new (g_str_hash, g_str_equal);

	search_path = g_build_filename(purple_user_dir(), "plugins", NULL);
	plugin_search_paths = g_list_append(plugin_search_paths, search_path);
	plugin_search_paths = g_list_append(
		plugin_search_paths, g_strdup(LIBDIR));

	/* Probe plugins */
	for (cur = plugin_search_paths; cur != NULL; cur = cur->next)
	{
		search_path = cur->data;

		dir = g_dir_open(search_path, 0, NULL);

		if (dir != NULL)
		{
			while ((file = g_dir_read_name(dir)) != NULL)
			{
				path = g_build_filename(search_path, file, NULL);

				if (G_MODULE_SUFFIX == NULL ||
					has_file_extension(file, G_MODULE_SUFFIX)) {
					plugin = purple_plugin_probe(path);
					g_hash_table_insert(plugins, 
						g_strdup(file), plugin);
				}
				g_free(path);
			}
			g_dir_close(dir);
		}
	}

	return plugins;
}

static void 
crash_info_closed (GtkWidget *widget, gpointer data)
{
	gtk_main_quit ();
}

static void
close_window (GtkWidget *widget, gpointer data)
{
	gtk_main_quit ();
}

static void
report_bug (GtkWidget *widget, gpointer data)
{
	gchar *website = (gchar *) data;
	notify_uri(website);
}

static void
process_crash_info (const char *minidump_file, GHashTable *plugins)
{
	GtkWidget *label, *vbox, *hbox, *frame, *logo, *button;
	GdkPixbuf *pixbuf;
	AtkObject *obj;
	gchar *text, *tmp, *filename;
	gchar *crash_module_name;
	GList *module_names, *iter;

	module_names = pidgin_crash_module_stack(minidump_file);

	crash_module_name = (gchar *)module_names->data;
	purple_debug_info("crash", 
		"crashed module name is %s\n", crash_module_name);

	PurplePlugin *crashed_plugin = 
		g_hash_table_lookup(plugins, crash_module_name);

	/* Construct the display */
	window_ = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (window_), _("Pidgin Crash Reporter"));
	gtk_window_set_default_size(GTK_WINDOW(window_), 500, 200);
	gtk_container_set_border_width(GTK_CONTAINER(window_), 10);

	g_signal_connect(
		G_OBJECT(window_), "destroy", 
		G_CALLBACK(crash_info_closed), NULL);
	gtk_widget_show_all(window_);

	vbox = gtk_vbox_new (FALSE, PIDGIN_HIG_BOX_SPACE);
	gtk_container_add (GTK_CONTAINER(window_), vbox);

	/* Insert Pidgin logo (copied from about dialog)
	 * TODO: This might be superfluous...is this a good idea?
	 */
	/* Generate a logo with a version number */
	logo = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_realize(logo);
	filename = g_build_filename(DATADIR, "pixmaps", "pidgin", "logo.png", NULL);
	pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
	g_free(filename);
#if 0  /* Don't versionize the logo when the logo has the version in it */
	pidgin_logo_versionize(&pixbuf, logo);
#endif
	gtk_widget_destroy(logo);
	logo = gtk_image_new_from_pixbuf(pixbuf);
	gdk_pixbuf_unref(pixbuf);
	/* Insert the logo */
	obj = gtk_widget_get_accessible(logo);
	tmp = g_strconcat(PIDGIN_NAME, " " DISPLAY_VERSION, NULL);
	atk_object_set_description(obj, tmp);
	g_free(tmp);
	gtk_box_pack_start(GTK_BOX(vbox), logo, FALSE, FALSE, 0);


	/* Display intro text */
	/*
	frame = make_frame(vbox, _("Pidgin has crashed"));
	label = gtk_label_new(_("Crash data has been saved (see below)."));
	gtk_box_pack_start(frame, label, FALSE, FALSE, 0);
	*/

	/* Display crashed plugin info */
	if (crashed_plugin && crashed_plugin->info->name) {
		purple_debug_info("crash", 
			"found plugin corresponding to crash\n");

		frame = make_frame(vbox, _("Source"));
		label = gtk_label_new (crashed_plugin->info->name);	
		gtk_box_pack_start(frame, label, FALSE, FALSE, 0);
		gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

		if (crashed_plugin->info->homepage) {
			text = g_strdup_printf("Report bugs at %s",
				crashed_plugin->info->homepage);
			label = gtk_label_new(text);
			gtk_box_pack_start(frame, label, FALSE, FALSE, 0);
			g_free(text);
			gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
		}
	} else {
		purple_debug_info("crash", 
			"did not find plugin corresponding to crash\n");
	}

	/* Module name */
	frame = make_frame(vbox, _("Crashed modules"));

	text = g_strdup_printf("%s (crashed)", crash_module_name);
	label = gtk_label_new(text);
	gtk_box_pack_start(frame, label, FALSE, FALSE, 0);
	g_free(text);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

	text = strdup ("");
	int i = 0;
	for (iter = module_names; iter != NULL; iter = iter->next) {
		tmp = g_strdup_printf("%s%d %s\; ",
			text, i, (gchar *)iter->data); 
		g_free(text);
		text = tmp;
		i++;
	}

	label = gtk_label_new(text);
	gtk_box_pack_start(frame, label, FALSE, FALSE, 0);
	g_free(text);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

	/* Minidump location */
	frame = make_frame(vbox, _("Crash data"));
	label = gtk_label_new (minidump_file);
	gtk_box_pack_start(frame, label, FALSE, FALSE, 0);

	/* Close button */
	hbox = gtk_hbox_new(TRUE, PIDGIN_HIG_BOX_SPACE);
	gtk_box_pack_start(vbox, hbox, FALSE, FALSE, 0);

	button = gtk_button_new_from_stock(GTK_STOCK_HELP);
	gtk_box_pack_start(hbox, button, TRUE, TRUE, 0);

	text = (crashed_plugin && crashed_plugin->info->homepage) ?
		crashed_plugin->info->homepage :
		PURPLE_WEBSITE;

	g_signal_connect(G_OBJECT(button), "clicked",
		G_CALLBACK(report_bug),
		text);

	button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
	gtk_box_pack_start(hbox, button, TRUE, TRUE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
		G_CALLBACK(close_window), NULL);

	/* Show it */
	gtk_widget_show_all(vbox);

	/* Cleanup */
	while(module_names != NULL) {
		g_free(module_names->data);
		module_names = module_names->next;
	}
}


int main (int argc, char **argv) {
	char *search_path;
	GHashTable *plugins;

	/* Initialize GThread before calling any Glib or GTK+ functions. */
	g_thread_init(NULL);

	/* 
	 * Initialize GTK 
	 * TODO: maybe we need to check return value of gtk_init?
	 */
	search_path = g_build_filename(purple_user_dir(), "gtkrc-2.0", NULL);
	gtk_rc_add_default_file(search_path);
	g_free(search_path);
	gtk_init_check(&argc, &argv);

	/* Initialize libpurple.  This is essentially a hail mary attempt to 
	 * load Pidgin into a "safe mode" of sorts where we can read prefs
	 * and figure out the user state.  It's possible of course that the
	 * crash occured in libpurple itself, which negates all this.  On the
	 * plus side, when we get here the dump will exist, so the next step
	 * is to go into a traditional triage mode where we ask the user to
	 * email us the dump or a gdb core, a copy of prefs.xml, etc.
	 *
	 * TODO: Add a crash handler to detect if the crash reporter itself
	 * crashed, and give a simple notification in that case.
	 */

	/* TODO: if user was using a custom libpurple directory, we don't 
	 * know anything about it.  Figure it out! */

	/* We're in a rescue mode of sorts.  We need all the info we can get */
	purple_debug_set_enabled(TRUE);

	purple_core_set_ui_ops(&core_ops);
	purple_eventloop_set_ui_ops(&eventloop_ops);
	purple_core_create(PIDGIN_UI);

	/* Load user prefs.  This is important for debugging! */
	purple_prefs_init();

	/* We want to immediately isolate bugs caused by plugins. */
	if (!g_module_supported()) return;
	plugins = probe_plugins();

	/*
	 * Deal with the minidump from the crash.  Do a simple check and grab
	 * the names of modules involved in the crash.
	 */
	purple_debug_info("crash", "grabbing minidump file\n");

	if (argc <= 0 || argv[1] == NULL) {
		purple_debug_error("crash", "no crash file provided\n");
		return 1;
	}

	const char *minidump_file = argv[1];
	purple_debug_info("crash", "minidump file name is %s\n", minidump_file);

	process_crash_info(minidump_file, plugins);

	/* Start the main loop */
	gtk_main ();

	return 0;
}



#include <glib.h>
#include <stdio.h>
#include "crashreporter.h"

#include "internal.h"
#include "pidgin.h"
#include <gtk/gtk.h>

#include "core.h"

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
 * Returns a hashtable mapping strings (library name) to PurplePlugin objects
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

	GList *module_names = pidgin_crash_module_stack(minidump_file);



	while(module_names != NULL) {
		g_free(module_names->data);
		module_names = module_names->next;
	}

	return 0;
}


#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../core.h"
#include "../eventloop.h"
#include "../util.h"
#include "../plugin.h"
#include "../debug.h"

#define UI_ID "check_mono"

static guint glib_input_add(gint fd, PurpleInputCondition condition, PurpleInputFunction function,
							   gpointer data)
{
	return 0;
}

static PurpleEventLoopUiOps glib_eventloops = 
{
	g_timeout_add,
	g_source_remove,
	glib_input_add,
	g_source_remove,
	NULL
};

static void
init_libpurple()
{
	purple_util_set_user_dir("/dev/null");

	purple_debug_set_enabled(TRUE);
	
	purple_debug_add_allowed_category("mono");
	purple_debug_add_allowed_category("buddyback");
	purple_debug_add_allowed_category("main");
	purple_debug_add_allowed_category("plugins");

	/* Set the uiops for the eventloop. If your client is glib-based, you can safely
	 * copy this verbatim. */
	purple_eventloop_set_ui_ops(&glib_eventloops);

	if (!purple_core_init(UI_ID)) {
		/* Initializing the core failed. Terminate. */
		fprintf(stderr,
				"libpurple initialization failed. Dumping core.\n"
				"Please report this!\n");
		abort();
	}
}

int main()
{
	PurplePlugin *buddyback = NULL, *monoloader = NULL;

	init_libpurple();
	
	monoloader = purple_plugin_probe("../plugins/mono/loader/.libs/mono.so");
	purple_plugin_load(monoloader);
	buddyback = purple_plugin_probe("../plugins/mono/GetBuddyBack.dll");
	
	if (!buddyback) {
		printf("Didn't find test plugin.\n");
		exit(1);
	}
	
	purple_plugin_load(buddyback);
	
	purple_core_quit();

	return 0;
}

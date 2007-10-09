/*
 * Request API Example Plugin
 *
 * Copyright (C) 2007, John Bailey <rekkanoryo@cpw.pidgin.im>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02111-1301, USA.
 *
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <glib.h>

#ifndef G_GNUC_NULL_TERMINATED
# if __GNUC__ >= 4
#  define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
# else
#  define G_GNUC_NULL_TERMINATED
# endif
#endif

/* This is the required definition of PURPLE_PLUGINS as required for a plugin,
 * but we protect it with an #ifndef because config.h may define it for us
 * already and this would cause an unneeded compiler warning. */
#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#define PLUGIN_ID "core-requestexample"
#define PLUGIN_AUTHOR "John Bailey <rekkanoryo@cpw.pidgin.im>"

static PurplePlugin *request_example = NULL;

static void
ok_cancel_ok_cb(gpointer ignored)
{
	purple_notify_info(request_example, "Done", "You Proceeded.",
		"You clicked OK on the previous dialog and proceeded to this one.");
}

ok_cancel_cancel_cb(gpointer ignored)
{
	purple_notify_info(request_example, "Canceled", "You canceled.",
		"You clicked Cancel on the previous dialog and proceeded to this one.");
}

static void
ok_cancel_cb(PurplePluginAction *action)
{
	purple_request_ok_cancel(request_example, "Proceed?", "Proceed?",
		"Proceed to the next dialog?", PURPLE_DEFAULT_ACTION_NONE, NULL,
		NULL, NULL, NULL, G_CALLBACK(ok_cancel_ok_cb),
		G_CALLBACK(ok_cancel_cancel_cb));
}

static void
accept_cancel_accept_cb(gpointer ignored)
{
	purple_notify_info(request_example, "Action Performed", "Accepted",
		"You accepted on the previous dialog.");
}

static void
accept_cancel_cb(PurplePluginAction *action)
{
	purple_request_accept_cancel(request_example, "Action Requested", "Make a choice!",
		"If you accept this action, you will see a dialog.",
		PURPLE_DEFAULT_ACTION_NONE, NULL, NULL, NULL, NULL,
		G_CALLBACK(accept_cancel_accept_cb), NULL);
}

static GList *
plugin_actions(PurplePlugin *plugin, gpointer context)
{
	GList *actions = NULL;

	actions = g_list_prepend(actions,
		purple_plugin_action_new("Demonstrate Accept/Cancel Request", accept_cancel_cb));
	
	actions = g_list_prepend(actions,
		purple_plugin_action_new("Demonstrate OK/Cancel Request", ok_cancel_cb));

	return g_list_reverse(actions);
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
	/* we need a handle for all the notify calls */
	request_example = plugin;

	return TRUE;
}

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,        /* magic number */
	PURPLE_MAJOR_VERSION,       /* purple major */
	PURPLE_MINOR_VERSION,       /* purple minor */
	PURPLE_PLUGIN_STANDARD,     /* plugin type */
	NULL,                       /* UI requirement */
	0,                          /* flags */
	NULL,                       /* dependencies */
	PURPLE_PRIORITY_DEFAULT,    /* priority */

	PLUGIN_ID,                  /* id */
	"Request API Example",      /* name */
	VERSION,                    /* version */
	"Request API Example",      /* summary */
	"Request API Example",      /* description */
	PLUGIN_AUTHOR,              /* author */
	"http://pidgin.im",         /* homepage */

	plugin_load,                /* load */
	NULL,                       /* unload */
	NULL,                       /* destroy */

	NULL,                       /* ui info */
	NULL,                       /* extra info */
	NULL,                       /* prefs info */
	plugin_actions,             /* actions */
	NULL,                       /* reserved */
	NULL,                       /* reserved */
	NULL,                       /* reserved */
	NULL                        /* reserved */
};

static void
init_plugin(PurplePlugin *plugin)
{
}

PURPLE_INIT_PLUGIN(notifyexample, init_plugin, info)

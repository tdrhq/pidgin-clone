/*
 * Plugin Translation Example Plugin
 *
 * Copyright (C) 2007, John Bailey <rekkanoryo@pidgin.im>
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

/* Note that some of the things done in this plugin for the sake of being
 * translatable may not make sense without some basic knowledge of how the GNU
 * autoconf and automake tools work and provide information.  There will be a
 * how-to document on our wiki to aid in this eventually. */

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

/* This is all the preprocessor magic needed to make translation support work;
 * it's essentially stolen from libpurple. */
#ifdef ENABLE_NLS
#  include <locale.h>
#  include <libintl.h>
#  define _(String) ((const char *)dgettext(PACKAGE, String))
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  include <locale.h>
#  define N_(String) (String)
#  ifndef _
#    define _(String) ((const char *)String)
#  endif
#  define ngettext(Singular, Plural, Number) ((Number == 1) ? ((const char *)Singular) : ((const char *)Plural))
#  define dngettext(Domain, Singular, Plural, Number) ((Number == 1) ? ((const char *)Singular) : ((const char *)Plural))
#endif

/* This is the required definition of PURPLE_PLUGINS as required for a plugin,
 * but we protect it with an #ifndef because config.h may define it for us
 * already and this would cause an unneeded compiler warning. */
#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include <notify.h>
#include <plugin.h>
#include <version.h>

#define PLUGIN_ID "core-translationexample"
#define PLUGIN_AUTHOR "John Bailey <rekkanoryo@pidgin.im>"
#define PLUGIN_WEBSITE "http://pidgin.im"

static PurplePlugin *translation_example = NULL;

static void
action_cb(PurplePluginAction *action)
{
	purple_notify_message(translation_example, PURPLE_NOTIFY_MSG_INFO,
		_("Plugin Translation Support Example"),
		_("This is an example of how to use translation support in a plugin."),
		NULL, NULL, NULL);
}

static GList *
plugin_actions(PurplePlugin *plugin, gpointer context)
{
	return g_list_append(NULL, purple_plugin_action_new(_("Show Dialog"),
				action_cb));
}

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_DEFAULT,

	PLUGIN_ID,
	NULL,
	DISPLAY_VERSION, /* Remember not to use this in your own plugins! */

	/* Ordinarily we would use N_("some string") here, but in a third-party
	 * plugin, we do things a bit differently to accomodate actually displaying
	 * the translated messages.  We'll explain this below. */
	NULL, /* see the comment above */
	NULL, /* see the comment above */
	PLUGIN_AUTHOR,
	PLUGIN_WEBSITE,

	NULL,
	NULL,
	NULL,

	NULL,
	NULL,
	NULL,
	plugin_actions,

	NULL,
	NULL,
	NULL,
	NULL
};

static void
init_plugin(PurplePlugin *plugin)
{
	/* The lines below are disabled with #if 0 because we don't want them to
	 * take effect when this plugin is distributed with libpurple--in this case
	 * we want to rely on libpurple/Pidgin's existing translations.  The lines
	 * are shown here, however, because a plugin developer wishing to provide
	 * translation abilities will need to use them to keep his/her plugin from
	 * trying to use libpurple/Pidgin's translation database (which would most
	 * likely not contain all the strings used in the plugin).  Plugin authors
	 * following this example would remove the #if 0 and the #endif, leaving
	 * the remaining lines. */
#if 0
#  ifdef ENABLE_NLS
	bindtextdomain(PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(PACKAGE, "UTF-8");
#  endif
#endif

	/* I (rekkanoryo) have encountered issues during my time developing for the
	 * Purple Plugin Pack where translators have discovered that, although the
	 * name, summary, and description strings are marked as translatable with
	 * N_(), the translated strings do not appear; the best solution that worked
	 * was to actually assign these fields here in init_plugin() with _() to
	 * ensure translation. */
	info.name = _("Plugin Translation Example");
	info.summary = _("Plugin Translation Support Example Plugin");
	info.description = _("This plugin serves as an example to third-party "
		"plugin authors who wish to include translation support in their plugins.");

	translation_example = plugin; /* we need a handle for the notify call above */
}

PURPLE_INIT_PLUGIN(translation_example, init_plugin, info)

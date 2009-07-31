/*
 * Vulture - Win32 libpurple client
 *
 * cmdline.c: Command-line parsing.
 *
 * Copyright (C) 2009, Gregor Dick <gdick@soc.pidgin.im>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

#include <windows.h>
#include <glib.h>

#include "vulture.h"
#include "cmdline.h"


/** Command-line options set by VultureParseCommandLine. */
static gchar *g_szCustomUserDir = NULL;

static GOptionContext *g_lpgopcontext;
static GOptionEntry g_rggopentry[] =
{
	{ "config", 'c', 0, G_OPTION_ARG_STRING, &g_szCustomUserDir, "use DIR for config files", "DIR" },
	{ NULL, 0, 0, 0, NULL, NULL, NULL }
};


/**
 * Parses the command-line, populating appropriate global variables. Call this
 * before querying any of the command-line parameters. Call
 * VultureCommandLineCleanup to free allocated memory.
 */
void VultureParseCommandLine(void)
{
	gchar *szCmdLine;
	LPTSTR szCmdLineT = GetCommandLine();
	GError *lpgerror = NULL;
	gint iArgc;
	gchar **rgszArgv;

	/* Get the command-line in UTF-8. */
	szCmdLine = VultureTCHARToUTF8(szCmdLineT);

	g_shell_parse_argv(szCmdLine, &iArgc, &rgszArgv, &lpgerror);
	
	g_lpgopcontext = g_option_context_new("");
	g_option_context_add_main_entries(g_lpgopcontext, g_rggopentry, NULL);
	g_option_context_parse(g_lpgopcontext, &iArgc, &rgszArgv, &lpgerror);

	g_strfreev(rgszArgv);
	g_free(szCmdLine);
}


/**
 * Frees memory allocated by VultureParseCommandLine. Do not query any
 * command-line parameters after calling this.
 */
void VultureCommandLineCleanup(void)
{
	g_option_context_free(g_lpgopcontext);
}


/**
 * Retrieves the user directory specified on the command-line, or NULL if none
 * set.
 */
gchar* VultureGetCustomUserDir(void)
{
	return g_szCustomUserDir;
}

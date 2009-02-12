/*
 * pidgin
 *
 * Pidgin is the legal property of its developers, whose names are too numerous
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 *
 */

#include "client/linux/handler/exception_handler.h"
#include "internal.h"

extern "C" void pidgin_crash_init ();

using namespace google_breakpad;

// Callback when minidump written.
static bool MinidumpCallback(
	const char *dump_path,
	const char *minidump_id,
	void *context,
	bool succeeded)
{

	printf("%s is dumped\n", minidump_id);
/*
	char *crash_argv[2];
	crash_argv[0] = "crashreporter";
	// Casting like this is bad.  But at the same time, the heap
	// is in a fubar'ed state here so allocating a copy is a really bad idea.
	crash_argv[1] = (char *)minidump_id;
	g_spawn_async(NULL, crash_argv, NULL, G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL, NULL, NULL);
*/
	return true;
}

static ExceptionHandler* handler_process;

void pidgin_crash_init () {
	/* TODO: This is a minor memory leak.  Clean this stuff up, yo */
	handler_process = new ExceptionHandler(".", NULL, MinidumpCallback, NULL, true);
}



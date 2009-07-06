/* PurpleWrapper - A .NET (CLR) wrapper for libpurple
 *
 * Purple is the legal property of its developers, whose names are too numerous
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
 */

/*
 * This file was auto-generated from the libpurple header files to provide a
 * clean interface between .NET/CLR and the unmanaged C library libpurple.
 *
 * This is the second major commit of the code.
 * Next things:
 *  - A few of the .h files have anonymous parameter names (eg: void cat(int, int).
 *    This program will need to assign these parameters names.
 *  - Function pointers inside structs aren't translated correctly into C#.
 *  - Two places there are specific-length arrays (eg: char hostname[256]). The parser
 *    does not detect them as an array.
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PurpleWrapper
{
	public class Core
	{
		/*
		 * gboolean purple_core_init(char * ui)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_core_init(string ui);

		public static bool Init(string ui)
		{
			return purple_core_init(ui);
		}

		/*
		 * void purple_core_quit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_core_quit();

		public static void Quit()
		{
			purple_core_quit();
		}

		/*
		 * gboolean purple_core_quit_cb(gpointer unused)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_core_quit_cb(IntPtr unused);

		public static bool QuitCb(IntPtr unused)
		{
			return purple_core_quit_cb(unused);
		}

		/*
		 * char * purple_core_get_version()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_core_get_version();

		public static string GetVersion()
		{
			return purple_core_get_version();
		}

		/*
		 * char * purple_core_get_ui()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_core_get_ui();

		public static string GetUi()
		{
			return purple_core_get_ui();
		}

		/*
		 * PurpleCore * purple_get_core()
		 * 
		 * Could not generate a wrapper for purple_get_core in file "core.h".
		 * Message: The type could not be resolved (PurpleCore * purple_get_core()).
		 */

		/*
		 * void purple_core_set_ui_ops(PurpleCoreUiOps * ops)
		 * 
		 * Could not generate a wrapper for purple_core_set_ui_ops in file "core.h".
		 * Message: The type could not be resolved (PurpleCoreUiOps * ops).
		 */

		/*
		 * PurpleCoreUiOps * purple_core_get_ui_ops()
		 * 
		 * Could not generate a wrapper for purple_core_get_ui_ops in file "core.h".
		 * Message: The type could not be resolved (PurpleCoreUiOps * purple_core_get_ui_ops()).
		 */

		/*
		 * gboolean purple_core_migrate()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_core_migrate();

		public static bool Migrate()
		{
			return purple_core_migrate();
		}

		/*
		 * gboolean purple_core_ensure_single_instance()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_core_ensure_single_instance();

		public static bool EnsureSingleInstance()
		{
			return purple_core_ensure_single_instance();
		}

		/*
		 * GHashTable* purple_core_get_ui_info()
		 * 
		 * Could not generate a wrapper for purple_core_get_ui_info in file "core.h".
		 * Message: The type could not be resolved (GHashTable* purple_core_get_ui_info()).
		 */

	}
}


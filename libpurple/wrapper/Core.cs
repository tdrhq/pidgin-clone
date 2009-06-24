/* purple
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
 * clean interface between .NET/CLR and the unmanaged C library, libpurple.
 *
 * This code isn't complete, but completely a work in progress. :)
 * Three major things left:
 *  - Resolve the remaining UNKNOWN types.
 *  - Handle translation between delegate and function pointers.
 *  - Fill in the translation between public .NET class calls and private DllImport[] calls.
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
			throw new NotImplementedException();
		}

		/*
		 * void purple_core_quit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_core_quit();

		public static void Quit()
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_core_quit_cb(gpointer unused)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_core_quit_cb(IntPtr unused);

		public static bool QuitCb(IntPtr unused)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_core_get_version()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_core_get_version();

		public static string GetVersion()
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_core_get_ui()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_core_get_ui();

		public static string GetUi()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleCore * purple_get_core()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_get_core();

		public static PurpleCore GetCore()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_core_set_ui_ops(PurpleCoreUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_core_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleCoreUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleCoreUiOps * purple_core_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_core_get_ui_ops();

		public static PurpleCoreUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_core_migrate()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_core_migrate();

		public static bool Migrate()
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_core_ensure_single_instance()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_core_ensure_single_instance();

		public static bool EnsureSingleInstance()
		{
			throw new NotImplementedException();
		}

	}
}


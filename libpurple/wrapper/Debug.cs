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
	public class Debug
	{
		/*
		 * void purple_debug(PurpleDebugLevel level, char * category, char * format, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug(UNKNOWN level, string category, string format, ...);

		public static void Debug(PurpleDebugLevel level, string category, string format, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_debug_misc(char * category, char * format, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug_misc(string category, string format, ...);

		public static void Misc(string category, string format, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_debug_info(char * category, char * format, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug_info(string category, string format, ...);

		public static void Info(string category, string format, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_debug_warning(char * category, char * format, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug_warning(string category, string format, ...);

		public static void Warning(string category, string format, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_debug_error(char * category, char * format, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug_error(string category, string format, ...);

		public static void Error(string category, string format, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_debug_fatal(char * category, char * format, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug_fatal(string category, string format, ...);

		public static void Fatal(string category, string format, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_debug_set_enabled(gboolean enabled)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug_set_enabled(bool enabled);

		public static void SetEnabled(bool enabled)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_debug_is_enabled()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_debug_is_enabled();

		public static bool IsEnabled()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_debug_set_ui_ops(PurpleDebugUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleDebugUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleDebugUiOps * purple_debug_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_debug_get_ui_ops();

		public static PurpleDebugUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_debug_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

	}
}


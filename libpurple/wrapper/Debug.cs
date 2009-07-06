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
	public class Debug
	{
		public enum PurpleDebugLevel
		{
			PURPLE_DEBUG_ALL = 0,
			PURPLE_DEBUG_MISC,
			PURPLE_DEBUG_INFO,
			PURPLE_DEBUG_WARNING,
			PURPLE_DEBUG_ERROR,
			PURPLE_DEBUG_FATAL
		};

		/*
		 * void purple_debug_set_enabled(gboolean enabled)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug_set_enabled(bool enabled);

		public static void SetEnabled(bool enabled)
		{
			purple_debug_set_enabled(enabled);
		}

		/*
		 * gboolean purple_debug_is_enabled()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_debug_is_enabled();

		public static bool IsEnabled()
		{
			return purple_debug_is_enabled();
		}

		/*
		 * void purple_debug_set_ui_ops(PurpleDebugUiOps * ops)
		 * 
		 * Could not generate a wrapper for purple_debug_set_ui_ops in file "debug.h".
		 * Message: The type could not be resolved (PurpleDebugUiOps * ops).
		 */

		/*
		 * PurpleDebugUiOps * purple_debug_get_ui_ops()
		 * 
		 * Could not generate a wrapper for purple_debug_get_ui_ops in file "debug.h".
		 * Message: The type could not be resolved (PurpleDebugUiOps * purple_debug_get_ui_ops()).
		 */

		/*
		 * void purple_debug_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_debug_init();

		public static void Init()
		{
			purple_debug_init();
		}

	}
}


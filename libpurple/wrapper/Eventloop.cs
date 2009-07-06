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
	public class Eventloop
	{
		public enum PurpleInputCondition
		{
			PURPLE_INPUT_READ = 1 << 0,
			PURPLE_INPUT_WRITE = 1 << 1
		};

		/*
		 * guint purple_timeout_add(guint interval, GSourceFunc function, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_timeout_add(uint interval, IntPtr function, IntPtr data);

		public static uint TimeoutAdd(uint interval, /* libgobject */ IntPtr function, IntPtr data)
		{
			/* Unable to process function, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * guint purple_timeout_add_seconds(guint interval, GSourceFunc function, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_timeout_add_seconds(uint interval, IntPtr function, IntPtr data);

		public static uint TimeoutAddSeconds(uint interval, /* libgobject */ IntPtr function, IntPtr data)
		{
			/* Unable to process function, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_timeout_remove(guint handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_timeout_remove(uint handle);

		public static bool TimeoutRemove(uint handle)
		{
			return purple_timeout_remove(handle);
		}

		/*
		 * gboolean purple_input_remove(guint handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_input_remove(uint handle);

		public static bool InputRemove(uint handle)
		{
			return purple_input_remove(handle);
		}

		/*
		 * int purple_input_get_error(int fd, int * error)
		 * 
		 * Could not generate a wrapper for purple_input_get_error in file "eventloop.h".
		 * Message: The type could not be resolved (int * error).
		 */

		/*
		 * void purple_eventloop_set_ui_ops(PurpleEventLoopUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_eventloop_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleEventLoopUiOps ops)
		{
			purple_eventloop_set_ui_ops(ops.Reference);
		}

		/*
		 * PurpleEventLoopUiOps * purple_eventloop_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_eventloop_get_ui_ops();

		public static PurpleEventLoopUiOps GetUiOps()
		{
			return new PurpleEventLoopUiOps(purple_eventloop_get_ui_ops());
		}

	}
}


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
	public class Eventloop
	{
		/*
		 * guint purple_timeout_add(guint interval, GSourceFunc function, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_timeout_add(uint interval, UNKNOWN function, IntPtr data);

		public static uint TimeoutAdd(uint interval, GSourceFunc function, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_timeout_add_seconds(guint interval, GSourceFunc function, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_timeout_add_seconds(uint interval, UNKNOWN function, IntPtr data);

		public static uint TimeoutAddSeconds(uint interval, GSourceFunc function, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_timeout_remove(guint handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_timeout_remove(uint handle);

		public static bool TimeoutRemove(uint handle)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_input_add(int fd, PurpleInputCondition cond, PurpleInputFunction func, gpointer user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_input_add(int fd, UNKNOWN cond, UNKNOWN func, IntPtr user_data);

		public static uint InputAdd(int fd, PurpleInputCondition cond, PurpleInputFunction func, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_input_remove(guint handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_input_remove(uint handle);

		public static bool InputRemove(uint handle)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_input_get_error(int fd, int * error)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_input_get_error(int fd, IntPtr error);

		public static int InputGetError(int fd, int error)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_eventloop_set_ui_ops(PurpleEventLoopUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_eventloop_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleEventLoopUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleEventLoopUiOps * purple_eventloop_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_eventloop_get_ui_ops();

		public static PurpleEventLoopUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

	}
}


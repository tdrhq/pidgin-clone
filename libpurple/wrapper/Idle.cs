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
	public class Idle
	{
		/*
		 * void purple_idle_touch()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_idle_touch();

		public static void Touch()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_idle_set(time_t time)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_idle_set(UNKNOWN time);

		public static void Set(time_t time)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_idle_set_ui_ops(PurpleIdleUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_idle_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleIdleUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleIdleUiOps * purple_idle_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_idle_get_ui_ops();

		public static PurpleIdleUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_idle_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_idle_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_idle_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_idle_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

	}
}


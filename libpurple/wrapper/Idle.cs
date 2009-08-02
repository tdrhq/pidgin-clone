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

	/*
	 * File: idle.h
	 */
	public class Idle
	{

		/*
		 * Function Name: purple_idle_touch
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_idle_touch()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_idle_touch();

		public static void Touch()
		{
			purple_idle_touch();
		}

		/*
		 * Function Name: purple_idle_set
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_idle_set(time_t time)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_idle_set(ulong time);

		public static void Set(DateTime time)
		{
			ulong _PurpleWrapper_param0 = (ulong)(time - new DateTime(1970, 1, 1)).TotalSeconds;
			purple_idle_set(_PurpleWrapper_param0);
		}

		/*
		 * Function Name: purple_idle_set_ui_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_idle_set_ui_ops(PurpleIdleUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_idle_set_ui_ops(IntPtr ops);

		public static void SetUiOps(/* Resolved as PointerToUnknownStruct to PurpleIdleUiOps */ IntPtr ops)
		{
			/* Unable to process ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_idle_get_ui_ops
		 * Function Type: PurpleIdleUiOps *
		 * Function RawType: PurpleIdleUiOps
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleIdleUiOps * purple_idle_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_idle_get_ui_ops();

		public static /* Resolved as PointerToUnknownStruct to PurpleIdleUiOps */ IntPtr GetUiOps()
		{
			/* Unable to process the return value of purple_idle_get_ui_ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_idle_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_idle_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_idle_init();

		public static void Init()
		{
			purple_idle_init();
		}

		/*
		 * Function Name: purple_idle_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_idle_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_idle_uninit();

		public static void Uninit()
		{
			purple_idle_uninit();
		}

	}
}


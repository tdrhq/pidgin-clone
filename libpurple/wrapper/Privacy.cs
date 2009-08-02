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
	 * File: privacy.h
	 */
	public class Privacy
	{
		public enum PurplePrivacyType
		{
			PURPLE_PRIVACY_ALLOW_ALL = 1,
			PURPLE_PRIVACY_DENY_ALL,
			PURPLE_PRIVACY_ALLOW_USERS,
			PURPLE_PRIVACY_DENY_USERS,
			PURPLE_PRIVACY_ALLOW_BUDDYLIST
		};


		/*
		 * Function Name: purple_privacy_check
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_privacy_check(PurpleAccount * account, char * who)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_privacy_check(IntPtr account, string who);

		public static bool Check(PurpleAccount account, string who)
		{
			return purple_privacy_check(account.Reference, who);
		}

		/*
		 * Function Name: purple_privacy_set_ui_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_privacy_set_ui_ops(PurplePrivacyUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_privacy_set_ui_ops(IntPtr ops);

		public static void SetUiOps(/* Resolved as PointerToUnknownStruct to PurplePrivacyUiOps */ IntPtr ops)
		{
			/* Unable to process ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_privacy_get_ui_ops
		 * Function Type: PurplePrivacyUiOps *
		 * Function RawType: PurplePrivacyUiOps
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePrivacyUiOps * purple_privacy_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_privacy_get_ui_ops();

		public static /* Resolved as PointerToUnknownStruct to PurplePrivacyUiOps */ IntPtr GetUiOps()
		{
			/* Unable to process the return value of purple_privacy_get_ui_ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_privacy_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_privacy_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_privacy_init();

		public static void Init()
		{
			purple_privacy_init();
		}

	}
}


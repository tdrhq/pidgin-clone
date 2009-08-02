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
	 * File: accountopt.h
	 */
	public class Accountopt
	{

		/*
		 * Function Name: purple_account_option_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_option_destroy(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_option_destroy(IntPtr option);

		public static void AccountOptionDestroy(/* Resolved as PointerToUnknownStruct to PurpleAccountOption */ IntPtr option)
		{
			/* Unable to process option, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_option_set_masked
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_option_set_masked(PurpleAccountOption * option, gboolean masked)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_option_set_masked(IntPtr option, bool masked);

		public static void AccountOptionSetMasked(/* Resolved as PointerToUnknownStruct to PurpleAccountOption */ IntPtr option, bool masked)
		{
			/* Unable to process option, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_option_set_list
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_option_set_list(PurpleAccountOption * option, GList * values)
		 * 
		 * Could not generate a wrapper for purple_account_option_set_list in file "accountopt.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_account_option_get_type
		 * Function Type: PurplePrefType
		 * Function RawType: PurplePrefType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurplePrefType purple_account_option_get_type(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern Prefs.PurplePrefType purple_account_option_get_type(IntPtr option);

		public static Prefs.PurplePrefType AccountOptionGetType(/* Resolved as PointerToUnknownStruct to PurpleAccountOption */ IntPtr option)
		{
			/* Unable to process option, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_option_get_text
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_account_option_get_text(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_option_get_text(IntPtr option);

		public static string AccountOptionGetText(/* Resolved as PointerToUnknownStruct to PurpleAccountOption */ IntPtr option)
		{
			/* Unable to process option, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_option_get_setting
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_account_option_get_setting(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_option_get_setting(IntPtr option);

		public static string AccountOptionGetSetting(/* Resolved as PointerToUnknownStruct to PurpleAccountOption */ IntPtr option)
		{
			/* Unable to process option, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_option_get_default_bool
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_account_option_get_default_bool(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_option_get_default_bool(IntPtr option);

		public static bool AccountOptionGetDefaultBool(/* Resolved as PointerToUnknownStruct to PurpleAccountOption */ IntPtr option)
		{
			/* Unable to process option, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_option_get_default_int
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * int purple_account_option_get_default_int(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_account_option_get_default_int(IntPtr option);

		public static int AccountOptionGetDefaultInt(/* Resolved as PointerToUnknownStruct to PurpleAccountOption */ IntPtr option)
		{
			/* Unable to process option, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_option_get_masked
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_account_option_get_masked(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_option_get_masked(IntPtr option);

		public static bool AccountOptionGetMasked(/* Resolved as PointerToUnknownStruct to PurpleAccountOption */ IntPtr option)
		{
			/* Unable to process option, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_option_get_list
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_account_option_get_list(PurpleAccountOption * option)
		 * 
		 * Could not generate a wrapper for purple_account_option_get_list in file "accountopt.h".
		 * Message: Unknown inner-type of argument 0 of purple_account_option_get_list
		 */

		/*
		 * Function Name: purple_account_user_split_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_user_split_destroy(PurpleAccountUserSplit * split)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_user_split_destroy(IntPtr split);

		public static void AccountUserSplitDestroy(/* Resolved as PointerToUnknownStruct to PurpleAccountUserSplit */ IntPtr split)
		{
			/* Unable to process split, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_user_split_get_text
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_account_user_split_get_text(PurpleAccountUserSplit * split)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_user_split_get_text(IntPtr split);

		public static string AccountUserSplitGetText(/* Resolved as PointerToUnknownStruct to PurpleAccountUserSplit */ IntPtr split)
		{
			/* Unable to process split, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_user_split_get_separator
		 * Function Type: char
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char purple_account_user_split_get_separator(PurpleAccountUserSplit * split)
		 */
		[DllImport("libpurple.dll")]
		private static extern char purple_account_user_split_get_separator(IntPtr split);

		public static char AccountUserSplitGetSeparator(/* Resolved as PointerToUnknownStruct to PurpleAccountUserSplit */ IntPtr split)
		{
			/* Unable to process split, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_user_split_get_reverse
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_account_user_split_get_reverse(PurpleAccountUserSplit * split)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_user_split_get_reverse(IntPtr split);

		public static bool AccountUserSplitGetReverse(/* Resolved as PointerToUnknownStruct to PurpleAccountUserSplit */ IntPtr split)
		{
			/* Unable to process split, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_user_split_set_reverse
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_user_split_set_reverse(PurpleAccountUserSplit * split, gboolean reverse)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_user_split_set_reverse(IntPtr split, bool reverse);

		public static void AccountUserSplitSetReverse(/* Resolved as PointerToUnknownStruct to PurpleAccountUserSplit */ IntPtr split, bool reverse)
		{
			/* Unable to process split, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


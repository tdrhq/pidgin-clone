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
	public class Accountopt
	{
		/*
		 * PurpleAccountOption * purple_account_option_new(PurplePrefType type, char * text, char * pref_name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_option_new(UNKNOWN type, string text, string pref_name);

		public static PurpleAccountOption AccountOptionNew(PurplePrefType type, string text, string pref_name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccountOption * purple_account_option_bool_new(char * text, char * pref_name, gboolean default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_option_bool_new(string text, string pref_name, bool default_value);

		public static PurpleAccountOption AccountOptionBoolNew(string text, string pref_name, bool default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccountOption * purple_account_option_int_new(char * text, char * pref_name, int default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_option_int_new(string text, string pref_name, int default_value);

		public static PurpleAccountOption AccountOptionIntNew(string text, string pref_name, int default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccountOption * purple_account_option_string_new(char * text, char * pref_name, char * default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_option_string_new(string text, string pref_name, string default_value);

		public static PurpleAccountOption AccountOptionStringNew(string text, string pref_name, string default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccountOption * purple_account_option_list_new(char * text, char * pref_name, GList * list)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_option_list_new(string text, string pref_name, IntPtr list);

		public static PurpleAccountOption AccountOptionListNew(string text, string pref_name, GList list)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_option_destroy(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_option_destroy(IntPtr option);

		public static void AccountOptionDestroy(PurpleAccountOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_option_set_default_bool(PurpleAccountOption * option, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_option_set_default_bool(IntPtr option, bool value);

		public static void AccountOptionSetDefaultBool(PurpleAccountOption option, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_option_set_default_int(PurpleAccountOption * option, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_option_set_default_int(IntPtr option, int value);

		public static void AccountOptionSetDefaultInt(PurpleAccountOption option, int value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_option_set_default_string(PurpleAccountOption * option, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_option_set_default_string(IntPtr option, string value);

		public static void AccountOptionSetDefaultString(PurpleAccountOption option, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_option_set_masked(PurpleAccountOption * option, gboolean masked)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_option_set_masked(IntPtr option, bool masked);

		public static void AccountOptionSetMasked(PurpleAccountOption option, bool masked)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_option_set_list(PurpleAccountOption * option, GList * values)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_option_set_list(IntPtr option, IntPtr values);

		public static void AccountOptionSetList(PurpleAccountOption option, GList values)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_option_add_list_item(PurpleAccountOption * option, char * key, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_option_add_list_item(IntPtr option, string key, string value);

		public static void AccountOptionAddListItem(PurpleAccountOption option, string key, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePrefType purple_account_option_get_type(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_account_option_get_type(IntPtr option);

		public static PurplePrefType AccountOptionGetType(PurpleAccountOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_option_get_text(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_option_get_text(IntPtr option);

		public static string AccountOptionGetText(PurpleAccountOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_option_get_setting(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_option_get_setting(IntPtr option);

		public static string AccountOptionGetSetting(PurpleAccountOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_option_get_default_bool(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_option_get_default_bool(IntPtr option);

		public static bool AccountOptionGetDefaultBool(PurpleAccountOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_account_option_get_default_int(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_account_option_get_default_int(IntPtr option);

		public static int AccountOptionGetDefaultInt(PurpleAccountOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_option_get_default_string(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_option_get_default_string(IntPtr option);

		public static string AccountOptionGetDefaultString(PurpleAccountOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_option_get_default_list_value(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_option_get_default_list_value(IntPtr option);

		public static string AccountOptionGetDefaultListValue(PurpleAccountOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_option_get_masked(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_option_get_masked(IntPtr option);

		public static bool AccountOptionGetMasked(PurpleAccountOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_account_option_get_list(PurpleAccountOption * option)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_option_get_list(IntPtr option);

		public static GList AccountOptionGetList(PurpleAccountOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccountUserSplit * purple_account_user_split_new(char * text, char * default_value, char sep)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_user_split_new(string text, string default_value, char sep);

		public static PurpleAccountUserSplit AccountUserSplitNew(string text, string default_value, char sep)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_user_split_destroy(PurpleAccountUserSplit * split)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_user_split_destroy(IntPtr split);

		public static void AccountUserSplitDestroy(PurpleAccountUserSplit split)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_user_split_get_text(PurpleAccountUserSplit * split)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_user_split_get_text(IntPtr split);

		public static string AccountUserSplitGetText(PurpleAccountUserSplit split)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_user_split_get_default_value(PurpleAccountUserSplit * split)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_user_split_get_default_value(IntPtr split);

		public static string AccountUserSplitGetDefaultValue(PurpleAccountUserSplit split)
		{
			throw new NotImplementedException();
		}

		/*
		 * char purple_account_user_split_get_separator(PurpleAccountUserSplit * split)
		 */
		[DllImport("libpurple.dll")]
		private static extern char purple_account_user_split_get_separator(IntPtr split);

		public static char AccountUserSplitGetSeparator(PurpleAccountUserSplit split)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_user_split_get_reverse(PurpleAccountUserSplit * split)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_user_split_get_reverse(IntPtr split);

		public static bool AccountUserSplitGetReverse(PurpleAccountUserSplit split)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_user_split_set_reverse(PurpleAccountUserSplit * split, gboolean reverse)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_user_split_set_reverse(IntPtr split, bool reverse);

		public static void AccountUserSplitSetReverse(PurpleAccountUserSplit split, bool reverse)
		{
			throw new NotImplementedException();
		}

	}
}


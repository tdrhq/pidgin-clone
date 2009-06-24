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
	public class Desktopitem
	{
		/*
		 * GType purple_desktop_item_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_desktop_item_get_type();

		public static GType DesktopItemGetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleDesktopItem * purple_desktop_item_new_from_file(char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_desktop_item_new_from_file(string filename);

		public static PurpleDesktopItem DesktopItemNewFromFile(string filename)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleDesktopItemType purple_desktop_item_get_entry_type(PurpleDesktopItem * item)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_desktop_item_get_entry_type(IntPtr item);

		public static PurpleDesktopItemType DesktopItemGetEntryType(PurpleDesktopItem item)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_desktop_item_get_string(PurpleDesktopItem * item, char * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_desktop_item_get_string(IntPtr item, string attr);

		public static string DesktopItemGetString(PurpleDesktopItem item, string attr)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleDesktopItem * purple_desktop_item_copy(PurpleDesktopItem * item)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_desktop_item_copy(IntPtr item);

		public static PurpleDesktopItem DesktopItemCopy(PurpleDesktopItem item)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_desktop_item_unref(PurpleDesktopItem * item)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_desktop_item_unref(IntPtr item);

		public static void DesktopItemUnref(PurpleDesktopItem item)
		{
			throw new NotImplementedException();
		}

	}
}


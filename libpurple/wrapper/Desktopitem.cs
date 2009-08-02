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
	 * File: desktopitem.h
	 */
	public class Desktopitem
	{
		public enum PurpleDesktopItemType
		{
			PURPLE_DESKTOP_ITEM_TYPE_NULL = 0,
			PURPLE_DESKTOP_ITEM_TYPE_OTHER,
			PURPLE_DESKTOP_ITEM_TYPE_APPLICATION,
			PURPLE_DESKTOP_ITEM_TYPE_LINK,
			PURPLE_DESKTOP_ITEM_TYPE_FSDEVICE,
			PURPLE_DESKTOP_ITEM_TYPE_MIME_TYPE,
			PURPLE_DESKTOP_ITEM_TYPE_DIRECTORY,
			PURPLE_DESKTOP_ITEM_TYPE_SERVICE,
			PURPLE_DESKTOP_ITEM_TYPE_SERVICE_TYPE
		};


		/*
		 * Function Name: purple_desktop_item_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_desktop_item_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_desktop_item_get_type();

		public static /* libobject */ IntPtr DesktopItemGetType()
		{
			/* Unable to process the return value of purple_desktop_item_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_desktop_item_new_from_file
		 * Function Type: PurpleDesktopItem *
		 * Function RawType: PurpleDesktopItem
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleDesktopItem * purple_desktop_item_new_from_file(char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_desktop_item_new_from_file(string filename);

		public static /* Resolved as PointerToUnknownStruct to PurpleDesktopItem */ IntPtr DesktopItemNewFromFile(string filename)
		{
			/* Unable to process the return value of purple_desktop_item_new_from_file, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_desktop_item_get_entry_type
		 * Function Type: PurpleDesktopItemType
		 * Function RawType: PurpleDesktopItemType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleDesktopItemType purple_desktop_item_get_entry_type(PurpleDesktopItem * item)
		 */
		[DllImport("libpurple.dll")]
		private static extern Desktopitem.PurpleDesktopItemType purple_desktop_item_get_entry_type(IntPtr item);

		public static Desktopitem.PurpleDesktopItemType DesktopItemGetEntryType(/* Resolved as PointerToUnknownStruct to PurpleDesktopItem */ IntPtr item)
		{
			/* Unable to process item, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_desktop_item_copy
		 * Function Type: PurpleDesktopItem *
		 * Function RawType: PurpleDesktopItem
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleDesktopItem * purple_desktop_item_copy(PurpleDesktopItem * item)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_desktop_item_copy(IntPtr item);

		public static /* Resolved as PointerToUnknownStruct to PurpleDesktopItem */ IntPtr DesktopItemCopy(/* Resolved as PointerToUnknownStruct to PurpleDesktopItem */ IntPtr item)
		{
			/* Unable to process item, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_desktop_item_unref
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_desktop_item_unref(PurpleDesktopItem * item)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_desktop_item_unref(IntPtr item);

		public static void DesktopItemUnref(/* Resolved as PointerToUnknownStruct to PurpleDesktopItem */ IntPtr item)
		{
			/* Unable to process item, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


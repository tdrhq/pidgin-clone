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
		 * GType purple_desktop_item_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_desktop_item_get_type();

		public static /* libgobject */ IntPtr DesktopItemGetType()
		{
			/* Unable to process purple_desktop_item_get_type, a GObjectObject. */
			
		}

		/*
		 * PurpleDesktopItem * purple_desktop_item_new_from_file(char * filename)
		 * 
		 * Could not generate a wrapper for purple_desktop_item_new_from_file in file "desktopitem.h".
		 * Message: The type could not be resolved (PurpleDesktopItem * purple_desktop_item_new_from_file(char * filename)).
		 */

		/*
		 * PurpleDesktopItemType purple_desktop_item_get_entry_type(PurpleDesktopItem * item)
		 * 
		 * Could not generate a wrapper for purple_desktop_item_get_entry_type in file "desktopitem.h".
		 * Message: The type could not be resolved (PurpleDesktopItem * item).
		 */

		/*
		 * PurpleDesktopItem * purple_desktop_item_copy(PurpleDesktopItem * item)
		 * 
		 * Could not generate a wrapper for purple_desktop_item_copy in file "desktopitem.h".
		 * Message: The type could not be resolved (PurpleDesktopItem * purple_desktop_item_copy(PurpleDesktopItem * item)).
		 */

		/*
		 * void purple_desktop_item_unref(PurpleDesktopItem * item)
		 * 
		 * Could not generate a wrapper for purple_desktop_item_unref in file "desktopitem.h".
		 * Message: The type could not be resolved (PurpleDesktopItem * item).
		 */

	}
}


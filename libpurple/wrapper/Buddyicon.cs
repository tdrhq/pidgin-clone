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
	 * File: buddyicon.h
	 */
	public class Buddyicon
	{

		/*
		 * Function Name: purple_buddy_icon_ref
		 * Function Type: PurpleBuddyIcon *
		 * Function RawType: PurpleBuddyIcon
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleBuddyIcon * purple_buddy_icon_ref(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icon_ref(IntPtr icon);

		public static /* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr BuddyIconRef(/* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon)
		{
			/* Unable to process icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icon_unref
		 * Function Type: PurpleBuddyIcon *
		 * Function RawType: PurpleBuddyIcon
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleBuddyIcon * purple_buddy_icon_unref(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icon_unref(IntPtr icon);

		public static /* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr BuddyIconUnref(/* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon)
		{
			/* Unable to process icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icon_update
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_buddy_icon_update(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icon_update(IntPtr icon);

		public static void BuddyIconUpdate(/* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon)
		{
			/* Unable to process icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icon_get_account
		 * Function Type: PurpleAccount *
		 * Function RawType: PurpleAccount
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleAccount * purple_buddy_icon_get_account(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icon_get_account(IntPtr icon);

		public static PurpleAccount BuddyIconGetAccount(/* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon)
		{
			/* Unable to process icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icon_get_username
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_buddy_icon_get_username(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icon_get_username(IntPtr icon);

		public static string BuddyIconGetUsername(/* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon)
		{
			/* Unable to process icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icon_get_checksum
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_buddy_icon_get_checksum(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icon_get_checksum(IntPtr icon);

		public static string BuddyIconGetChecksum(/* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon)
		{
			/* Unable to process icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icon_get_data
		 * Function Type: gconstpointer
		 * Function RawType: gconstpointer
		 * Function Category: VoidPointer
		 */
		/*
		 * gconstpointer purple_buddy_icon_get_data(PurpleBuddyIcon * icon, size_t * len)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icon_get_data(IntPtr icon, IntPtr len);

		public static IntPtr BuddyIconGetData(/* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon, /* Resolved as PointerToUnknownStruct to size_t */ IntPtr len)
		{
			/* Unable to process icon, a PointerToUnknownStruct. */
			/* Unable to process len, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icon_get_extension
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_buddy_icon_get_extension(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icon_get_extension(IntPtr icon);

		public static string BuddyIconGetExtension(/* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon)
		{
			/* Unable to process icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icon_get_full_path
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_buddy_icon_get_full_path(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icon_get_full_path(IntPtr icon);

		public static string BuddyIconGetFullPath(/* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon)
		{
			/* Unable to process icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icons_get_checksum_for_user
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_buddy_icons_get_checksum_for_user(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icons_get_checksum_for_user(IntPtr buddy);

		public static string BuddyIconsGetChecksumForUser(PurpleBuddy buddy)
		{
			return purple_buddy_icons_get_checksum_for_user(buddy.Reference);
		}

		/*
		 * Function Name: purple_buddy_icons_find
		 * Function Type: PurpleBuddyIcon *
		 * Function RawType: PurpleBuddyIcon
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleBuddyIcon * purple_buddy_icons_find(PurpleAccount * account, char * username)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icons_find(IntPtr account, string username);

		public static /* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr BuddyIconsFind(PurpleAccount account, string username)
		{
			/* Unable to process the return value of purple_buddy_icons_find, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icons_find_account_icon
		 * Function Type: PurpleStoredImage *
		 * Function RawType: PurpleStoredImage
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStoredImage * purple_buddy_icons_find_account_icon(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icons_find_account_icon(IntPtr account);

		public static /* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr BuddyIconsFindAccountIcon(PurpleAccount account)
		{
			/* Unable to process the return value of purple_buddy_icons_find_account_icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icons_get_account_icon_timestamp
		 * Function Type: time_t
		 * Function RawType: time_t
		 * Function Category: DateTime
		 */
		/*
		 * time_t purple_buddy_icons_get_account_icon_timestamp(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_buddy_icons_get_account_icon_timestamp(IntPtr account);

		public static DateTime BuddyIconsGetAccountIconTimestamp(PurpleAccount account)
		{
			/* Unable to process the return value of purple_buddy_icons_get_account_icon_timestamp, a DateTime. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icons_node_has_custom_icon
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_buddy_icons_node_has_custom_icon(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_buddy_icons_node_has_custom_icon(IntPtr node);

		public static bool BuddyIconsNodeHasCustomIcon(PurpleBlistNode node)
		{
			return purple_buddy_icons_node_has_custom_icon(node.Reference);
		}

		/*
		 * Function Name: purple_buddy_icons_node_find_custom_icon
		 * Function Type: PurpleStoredImage *
		 * Function RawType: PurpleStoredImage
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStoredImage * purple_buddy_icons_node_find_custom_icon(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icons_node_find_custom_icon(IntPtr node);

		public static /* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr BuddyIconsNodeFindCustomIcon(PurpleBlistNode node)
		{
			/* Unable to process the return value of purple_buddy_icons_node_find_custom_icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icons_has_custom_icon
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_buddy_icons_has_custom_icon(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_buddy_icons_has_custom_icon(IntPtr contact);

		public static bool BuddyIconsHasCustomIcon(PurpleContact contact)
		{
			return purple_buddy_icons_has_custom_icon(contact.Reference);
		}

		/*
		 * Function Name: purple_buddy_icons_find_custom_icon
		 * Function Type: PurpleStoredImage *
		 * Function RawType: PurpleStoredImage
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStoredImage * purple_buddy_icons_find_custom_icon(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icons_find_custom_icon(IntPtr contact);

		public static /* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr BuddyIconsFindCustomIcon(PurpleContact contact)
		{
			/* Unable to process the return value of purple_buddy_icons_find_custom_icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_buddy_icons_set_caching
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_buddy_icons_set_caching(gboolean caching)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icons_set_caching(bool caching);

		public static void BuddyIconsSetCaching(bool caching)
		{
			purple_buddy_icons_set_caching(caching);
		}

		/*
		 * Function Name: purple_buddy_icons_is_caching
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_buddy_icons_is_caching()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_buddy_icons_is_caching();

		public static bool BuddyIconsIsCaching()
		{
			return purple_buddy_icons_is_caching();
		}

		/*
		 * Function Name: purple_buddy_icons_set_cache_dir
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_buddy_icons_set_cache_dir(char * cache_dir)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icons_set_cache_dir(string cache_dir);

		public static void BuddyIconsSetCacheDir(string cache_dir)
		{
			purple_buddy_icons_set_cache_dir(cache_dir);
		}

		/*
		 * Function Name: purple_buddy_icons_get_cache_dir
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_buddy_icons_get_cache_dir()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icons_get_cache_dir();

		public static string BuddyIconsGetCacheDir()
		{
			return purple_buddy_icons_get_cache_dir();
		}

		/*
		 * Function Name: purple_buddy_icons_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_buddy_icons_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icons_get_handle();

		public static IntPtr BuddyIconsGetHandle()
		{
			return purple_buddy_icons_get_handle();
		}

		/*
		 * Function Name: purple_buddy_icons_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_buddy_icons_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icons_init();

		public static void BuddyIconsInit()
		{
			purple_buddy_icons_init();
		}

		/*
		 * Function Name: purple_buddy_icons_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_buddy_icons_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icons_uninit();

		public static void BuddyIconsUninit()
		{
			purple_buddy_icons_uninit();
		}

		/*
		 * Function Name: purple_buddy_icon_get_scale_size
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_buddy_icon_get_scale_size(PurpleBuddyIconSpec * spec, int * width, int * height)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icon_get_scale_size(IntPtr spec, IntPtr width, IntPtr height);

		public static void BuddyIconGetScaleSize(PurpleBuddyIconSpec spec, /* Resolved as PointerToUnknownStruct to int */ IntPtr width, /* Resolved as PointerToUnknownStruct to int */ IntPtr height)
		{
			/* Unable to process width, a PointerToUnknownStruct. */
			/* Unable to process height, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


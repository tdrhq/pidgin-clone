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
	public class Buddyicon
	{
		/*
		 * PurpleBuddyIcon * purple_buddy_icon_new(PurpleAccount * account, char * username, void * icon_data, size_t icon_len, char * checksum)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icon_new(IntPtr account, string username, IntPtr icon_data, UNKNOWN icon_len, string checksum);

		public static PurpleBuddyIcon BuddyIconNew(PurpleAccount account, string username, IntPtr icon_data, size_t icon_len, string checksum)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBuddyIcon * purple_buddy_icon_ref(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icon_ref(IntPtr icon);

		public static PurpleBuddyIcon BuddyIconRef(PurpleBuddyIcon icon)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBuddyIcon * purple_buddy_icon_unref(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icon_unref(IntPtr icon);

		public static PurpleBuddyIcon BuddyIconUnref(PurpleBuddyIcon icon)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_icon_update(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icon_update(IntPtr icon);

		public static void BuddyIconUpdate(PurpleBuddyIcon icon)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_icon_set_data(PurpleBuddyIcon * icon, guchar * data, size_t len, char * checksum)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icon_set_data(IntPtr icon, IntPtr data, UNKNOWN len, string checksum);

		public static void BuddyIconSetData(PurpleBuddyIcon icon, guchar data, size_t len, string checksum)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_buddy_icon_get_account(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icon_get_account(IntPtr icon);

		public static PurpleAccount BuddyIconGetAccount(PurpleBuddyIcon icon)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_icon_get_username(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icon_get_username(IntPtr icon);

		public static string BuddyIconGetUsername(PurpleBuddyIcon icon)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_icon_get_checksum(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icon_get_checksum(IntPtr icon);

		public static string BuddyIconGetChecksum(PurpleBuddyIcon icon)
		{
			throw new NotImplementedException();
		}

		/*
		 * gconstpointer purple_buddy_icon_get_data(PurpleBuddyIcon * icon, size_t * len)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_buddy_icon_get_data(IntPtr icon, IntPtr len);

		public static gconstpointer BuddyIconGetData(PurpleBuddyIcon icon, size_t len)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_icon_get_extension(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icon_get_extension(IntPtr icon);

		public static string BuddyIconGetExtension(PurpleBuddyIcon icon)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_icon_get_full_path(PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icon_get_full_path(IntPtr icon);

		public static string BuddyIconGetFullPath(PurpleBuddyIcon icon)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_icons_set_for_user(PurpleAccount * account, char * username, void * icon_data, size_t icon_len, char * checksum)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icons_set_for_user(IntPtr account, string username, IntPtr icon_data, UNKNOWN icon_len, string checksum);

		public static void BuddyIconsSetForUser(PurpleAccount account, string username, IntPtr icon_data, size_t icon_len, string checksum)
		{
			throw new NotImplementedException();
		}

		/*
		 * time_t purple_buddy_icons_get_account_icon_timestamp(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_buddy_icons_get_account_icon_timestamp(IntPtr account);

		public static time_t BuddyIconsGetAccountIconTimestamp(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_buddy_icons_node_has_custom_icon(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_buddy_icons_node_has_custom_icon(IntPtr node);

		public static bool BuddyIconsNodeHasCustomIcon(PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_buddy_icons_has_custom_icon(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_buddy_icons_has_custom_icon(IntPtr contact);

		public static bool BuddyIconsHasCustomIcon(PurpleContact contact)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_icons_set_caching(gboolean caching)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icons_set_caching(bool caching);

		public static void BuddyIconsSetCaching(bool caching)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_buddy_icons_is_caching()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_buddy_icons_is_caching();

		public static bool BuddyIconsIsCaching()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_icons_set_cache_dir(char * cache_dir)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icons_set_cache_dir(string cache_dir);

		public static void BuddyIconsSetCacheDir(string cache_dir)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_icons_get_cache_dir()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icons_get_cache_dir();

		public static string BuddyIconsGetCacheDir()
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_buddy_icons_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_icons_get_handle();

		public static IntPtr BuddyIconsGetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_icons_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icons_init();

		public static void BuddyIconsInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_icons_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icons_uninit();

		public static void BuddyIconsUninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_icon_get_scale_size(PurpleBuddyIconSpec * spec, int * width, int * height)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icon_get_scale_size(IntPtr spec, IntPtr width, IntPtr height);

		public static void BuddyIconGetScaleSize(PurpleBuddyIconSpec spec, int width, int height)
		{
			throw new NotImplementedException();
		}

	}
}


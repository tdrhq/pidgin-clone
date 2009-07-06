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
	public class Buddyicon
	{
		/*
		 * PurpleBuddyIcon * purple_buddy_icon_ref(PurpleBuddyIcon * icon)
		 * 
		 * Could not generate a wrapper for purple_buddy_icon_ref in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * purple_buddy_icon_ref(PurpleBuddyIcon * icon)).
		 */

		/*
		 * PurpleBuddyIcon * purple_buddy_icon_unref(PurpleBuddyIcon * icon)
		 * 
		 * Could not generate a wrapper for purple_buddy_icon_unref in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * purple_buddy_icon_unref(PurpleBuddyIcon * icon)).
		 */

		/*
		 * void purple_buddy_icon_update(PurpleBuddyIcon * icon)
		 * 
		 * Could not generate a wrapper for purple_buddy_icon_update in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * icon).
		 */

		/*
		 * PurpleAccount * purple_buddy_icon_get_account(PurpleBuddyIcon * icon)
		 * 
		 * Could not generate a wrapper for purple_buddy_icon_get_account in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * icon).
		 */

		/*
		 * char * purple_buddy_icon_get_username(PurpleBuddyIcon * icon)
		 * 
		 * Could not generate a wrapper for purple_buddy_icon_get_username in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * icon).
		 */

		/*
		 * char * purple_buddy_icon_get_checksum(PurpleBuddyIcon * icon)
		 * 
		 * Could not generate a wrapper for purple_buddy_icon_get_checksum in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * icon).
		 */

		/*
		 * gconstpointer purple_buddy_icon_get_data(PurpleBuddyIcon * icon, size_t * len)
		 * 
		 * Could not generate a wrapper for purple_buddy_icon_get_data in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * icon).
		 */

		/*
		 * char * purple_buddy_icon_get_extension(PurpleBuddyIcon * icon)
		 * 
		 * Could not generate a wrapper for purple_buddy_icon_get_extension in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * icon).
		 */

		/*
		 * char * purple_buddy_icon_get_full_path(PurpleBuddyIcon * icon)
		 * 
		 * Could not generate a wrapper for purple_buddy_icon_get_full_path in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * icon).
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
		 * PurpleBuddyIcon * purple_buddy_icons_find(PurpleAccount * account, char * username)
		 * 
		 * Could not generate a wrapper for purple_buddy_icons_find in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * purple_buddy_icons_find(PurpleAccount * account, char * username)).
		 */

		/*
		 * PurpleStoredImage * purple_buddy_icons_find_account_icon(PurpleAccount * account)
		 * 
		 * Could not generate a wrapper for purple_buddy_icons_find_account_icon in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleStoredImage * purple_buddy_icons_find_account_icon(PurpleAccount * account)).
		 */

		/*
		 * time_t purple_buddy_icons_get_account_icon_timestamp(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_buddy_icons_get_account_icon_timestamp(IntPtr account);

		public static DateTime BuddyIconsGetAccountIconTimestamp(PurpleAccount account)
		{
			/* Unable to process purple_buddy_icons_get_account_icon_timestamp, a DateTime. */
			
		}

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
		 * PurpleStoredImage * purple_buddy_icons_node_find_custom_icon(PurpleBlistNode * node)
		 * 
		 * Could not generate a wrapper for purple_buddy_icons_node_find_custom_icon in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleStoredImage * purple_buddy_icons_node_find_custom_icon(PurpleBlistNode * node)).
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
		 * PurpleStoredImage * purple_buddy_icons_find_custom_icon(PurpleContact * contact)
		 * 
		 * Could not generate a wrapper for purple_buddy_icons_find_custom_icon in file "buddyicon.h".
		 * Message: The type could not be resolved (PurpleStoredImage * purple_buddy_icons_find_custom_icon(PurpleContact * contact)).
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
		 * gboolean purple_buddy_icons_is_caching()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_buddy_icons_is_caching();

		public static bool BuddyIconsIsCaching()
		{
			return purple_buddy_icons_is_caching();
		}

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
		 * char * purple_buddy_icons_get_cache_dir()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_icons_get_cache_dir();

		public static string BuddyIconsGetCacheDir()
		{
			return purple_buddy_icons_get_cache_dir();
		}

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
		 * void purple_buddy_icons_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_icons_init();

		public static void BuddyIconsInit()
		{
			purple_buddy_icons_init();
		}

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
		 * void purple_buddy_icon_get_scale_size(PurpleBuddyIconSpec * spec, int * width, int * height)
		 * 
		 * Could not generate a wrapper for purple_buddy_icon_get_scale_size in file "buddyicon.h".
		 * Message: The type could not be resolved (int * width).
		 */

	}
}


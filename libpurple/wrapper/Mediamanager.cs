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
	public class Mediamanager
	{
		/*
		 * GType purple_media_manager_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_manager_get_type();

		public static GType MediaManagerGetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMediaManager * purple_media_manager_get()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_manager_get();

		public static PurpleMediaManager MediaManagerGet()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMedia * purple_media_manager_create_media(PurpleMediaManager * manager, PurpleAccount * account, char * conference_type, char * remote_user, gboolean initiator)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_manager_create_media(IntPtr manager, IntPtr account, string conference_type, string remote_user, bool initiator);

		public static PurpleMedia MediaManagerCreateMedia(PurpleMediaManager manager, PurpleAccount account, string conference_type, string remote_user, bool initiator)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_media_manager_get_media(PurpleMediaManager * manager)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_manager_get_media(IntPtr manager);

		public static GList MediaManagerGetMedia(PurpleMediaManager manager)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_media_manager_get_media_by_account(PurpleMediaManager * manager, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_manager_get_media_by_account(IntPtr manager, IntPtr account);

		public static GList MediaManagerGetMediaByAccount(PurpleMediaManager manager, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_manager_remove_media(PurpleMediaManager * manager, PurpleMedia * media)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_manager_remove_media(IntPtr manager, IntPtr media);

		public static void MediaManagerRemoveMedia(PurpleMediaManager manager, PurpleMedia media)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_media_manager_create_output_window(PurpleMediaManager * manager, PurpleMedia * media, gchar * session_id, gchar * participant)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_manager_create_output_window(IntPtr manager, IntPtr media, string session_id, string participant);

		public static bool MediaManagerCreateOutputWindow(PurpleMediaManager manager, PurpleMedia media, string session_id, string participant)
		{
			throw new NotImplementedException();
		}

		/*
		 * gulong purple_media_manager_set_output_window(PurpleMediaManager * manager, PurpleMedia * media, gchar * session_id, gchar * participant, gulong window_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_media_manager_set_output_window(IntPtr manager, IntPtr media, string session_id, string participant, ulong window_id);

		public static ulong MediaManagerSetOutputWindow(PurpleMediaManager manager, PurpleMedia media, string session_id, string participant, ulong window_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_media_manager_remove_output_window(PurpleMediaManager * manager, gulong output_window_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_manager_remove_output_window(IntPtr manager, ulong output_window_id);

		public static bool MediaManagerRemoveOutputWindow(PurpleMediaManager manager, ulong output_window_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_manager_remove_output_windows(PurpleMediaManager * manager, PurpleMedia * media, gchar * session_id, gchar * participant)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_manager_remove_output_windows(IntPtr manager, IntPtr media, string session_id, string participant);

		public static void MediaManagerRemoveOutputWindows(PurpleMediaManager manager, PurpleMedia media, string session_id, string participant)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_manager_set_ui_caps(PurpleMediaManager * manager, PurpleMediaCaps caps)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_manager_set_ui_caps(IntPtr manager, UNKNOWN caps);

		public static void MediaManagerSetUiCaps(PurpleMediaManager manager, PurpleMediaCaps caps)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMediaCaps purple_media_manager_get_ui_caps(PurpleMediaManager * manager)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_manager_get_ui_caps(IntPtr manager);

		public static PurpleMediaCaps MediaManagerGetUiCaps(PurpleMediaManager manager)
		{
			throw new NotImplementedException();
		}

	}
}


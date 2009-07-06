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
	public class Notify
	{
		public enum PurpleNotifyType
		{
			PURPLE_NOTIFY_MESSAGE = 0,
			PURPLE_NOTIFY_EMAIL,
			PURPLE_NOTIFY_EMAILS,
			PURPLE_NOTIFY_FORMATTED,
			PURPLE_NOTIFY_SEARCHRESULTS,
			PURPLE_NOTIFY_USERINFO,
			PURPLE_NOTIFY_URI
		};

		public enum PurpleNotifyMsgType
		{
			PURPLE_NOTIFY_MSG_ERROR = 0,
			PURPLE_NOTIFY_MSG_WARNING,
			PURPLE_NOTIFY_MSG_INFO
		};

		public enum PurpleNotifySearchButtonType
		{
			PURPLE_NOTIFY_BUTTON_LABELED = 0,
			PURPLE_NOTIFY_BUTTON_CONTINUE = 1,
			PURPLE_NOTIFY_BUTTON_ADD,
			PURPLE_NOTIFY_BUTTON_INFO,
			PURPLE_NOTIFY_BUTTON_IM,
			PURPLE_NOTIFY_BUTTON_JOIN,
			PURPLE_NOTIFY_BUTTON_INVITE
		};

		public enum PurpleNotifyUserInfoEntryType
		{
			PURPLE_NOTIFY_USER_INFO_ENTRY_PAIR = 0,
			PURPLE_NOTIFY_USER_INFO_ENTRY_SECTION_BREAK,
			PURPLE_NOTIFY_USER_INFO_ENTRY_SECTION_HEADER
		};

		/*
		 * void purple_notify_searchresults_free(PurpleNotifySearchResults * results)
		 * 
		 * Could not generate a wrapper for purple_notify_searchresults_free in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifySearchResults * results).
		 */

		/*
		 * PurpleNotifySearchResults * purple_notify_searchresults_new()
		 * 
		 * Could not generate a wrapper for purple_notify_searchresults_new in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifySearchResults * purple_notify_searchresults_new()).
		 */

		/*
		 * PurpleNotifySearchColumn * purple_notify_searchresults_column_new(char * title)
		 * 
		 * Could not generate a wrapper for purple_notify_searchresults_column_new in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifySearchColumn * purple_notify_searchresults_column_new(char * title)).
		 */

		/*
		 * guint purple_notify_searchresults_get_rows_count(PurpleNotifySearchResults * results)
		 * 
		 * Could not generate a wrapper for purple_notify_searchresults_get_rows_count in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifySearchResults * results).
		 */

		/*
		 * guint purple_notify_searchresults_get_columns_count(PurpleNotifySearchResults * results)
		 * 
		 * Could not generate a wrapper for purple_notify_searchresults_get_columns_count in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifySearchResults * results).
		 */

		/*
		 * PurpleNotifyUserInfo * purple_notify_user_info_new()
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_new in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * purple_notify_user_info_new()).
		 */

		/*
		 * void purple_notify_user_info_destroy(PurpleNotifyUserInfo * user_info)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_destroy in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * user_info).
		 */

		/*
		 * GList * purple_notify_user_info_get_entries(PurpleNotifyUserInfo * user_info)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_get_entries in file "notify.h".
		 * Message: The type could not be resolved (GList * purple_notify_user_info_get_entries(PurpleNotifyUserInfo * user_info)).
		 */

		/*
		 * char * purple_notify_user_info_get_text_with_newline(PurpleNotifyUserInfo * user_info, char * newline)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_get_text_with_newline in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * user_info).
		 */

		/*
		 * void purple_notify_user_info_add_pair(PurpleNotifyUserInfo * user_info, char * label, char * value)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_add_pair in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * user_info).
		 */

		/*
		 * void purple_notify_user_info_prepend_pair(PurpleNotifyUserInfo * user_info, char * label, char * value)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_prepend_pair in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * user_info).
		 */

		/*
		 * void purple_notify_user_info_remove_entry(PurpleNotifyUserInfo * user_info, PurpleNotifyUserInfoEntry * user_info_entry)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_remove_entry in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * user_info).
		 */

		/*
		 * PurpleNotifyUserInfoEntry * purple_notify_user_info_entry_new(char * label, char * value)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_entry_new in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfoEntry * purple_notify_user_info_entry_new(char * label, char * value)).
		 */

		/*
		 * void purple_notify_user_info_add_section_break(PurpleNotifyUserInfo * user_info)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_add_section_break in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * user_info).
		 */

		/*
		 * void purple_notify_user_info_prepend_section_break(PurpleNotifyUserInfo * user_info)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_prepend_section_break in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * user_info).
		 */

		/*
		 * void purple_notify_user_info_add_section_header(PurpleNotifyUserInfo * user_info, char * label)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_add_section_header in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * user_info).
		 */

		/*
		 * void purple_notify_user_info_prepend_section_header(PurpleNotifyUserInfo * user_info, char * label)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_prepend_section_header in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * user_info).
		 */

		/*
		 * void purple_notify_user_info_remove_last_item(PurpleNotifyUserInfo * user_info)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_remove_last_item in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfo * user_info).
		 */

		/*
		 * gchar * purple_notify_user_info_entry_get_label(PurpleNotifyUserInfoEntry * user_info_entry)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_entry_get_label in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfoEntry * user_info_entry).
		 */

		/*
		 * void purple_notify_user_info_entry_set_label(PurpleNotifyUserInfoEntry * user_info_entry, char * label)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_entry_set_label in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfoEntry * user_info_entry).
		 */

		/*
		 * gchar * purple_notify_user_info_entry_get_value(PurpleNotifyUserInfoEntry * user_info_entry)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_entry_get_value in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfoEntry * user_info_entry).
		 */

		/*
		 * void purple_notify_user_info_entry_set_value(PurpleNotifyUserInfoEntry * user_info_entry, char * value)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_entry_set_value in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfoEntry * user_info_entry).
		 */

		/*
		 * PurpleNotifyUserInfoEntryType purple_notify_user_info_entry_get_type(PurpleNotifyUserInfoEntry * user_info_entry)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_entry_get_type in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUserInfoEntry * user_info_entry).
		 */

		/*
		 * void * purple_notify_uri(void * handle, char * uri)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_uri(IntPtr handle, string uri);

		public static IntPtr Uri(IntPtr handle, string uri)
		{
			return purple_notify_uri(handle, uri);
		}

		/*
		 * void purple_notify_close(PurpleNotifyType type, void * ui_handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_close(Notify.PurpleNotifyType type, IntPtr ui_handle);

		public static void Close(Notify.PurpleNotifyType type, IntPtr ui_handle)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_close_with_handle(void * handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_close_with_handle(IntPtr handle);

		public static void CloseWithHandle(IntPtr handle)
		{
			purple_notify_close_with_handle(handle);
		}

		/*
		 * void purple_notify_set_ui_ops(PurpleNotifyUiOps * ops)
		 * 
		 * Could not generate a wrapper for purple_notify_set_ui_ops in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUiOps * ops).
		 */

		/*
		 * PurpleNotifyUiOps * purple_notify_get_ui_ops()
		 * 
		 * Could not generate a wrapper for purple_notify_get_ui_ops in file "notify.h".
		 * Message: The type could not be resolved (PurpleNotifyUiOps * purple_notify_get_ui_ops()).
		 */

		/*
		 * void * purple_notify_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_notify_get_handle();
		}

		/*
		 * void purple_notify_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_init();

		public static void Init()
		{
			purple_notify_init();
		}

		/*
		 * void purple_notify_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_uninit();

		public static void Uninit()
		{
			purple_notify_uninit();
		}

	}
}


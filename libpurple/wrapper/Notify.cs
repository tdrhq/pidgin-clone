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
	 * File: notify.h
	 */
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

		public delegate void PurpleNotifyCloseCallback(IntPtr user_data);
		public delegate void PurpleNotifySearchResultsCallback(IntPtr c, IntPtr row, IntPtr user_data);

		/*
		 * Function Name: purple_notify_searchresults_free
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_searchresults_free(PurpleNotifySearchResults * results)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_searchresults_free(IntPtr results);

		public static void SearchresultsFree(/* Resolved as PointerToUnknownStruct to PurpleNotifySearchResults */ IntPtr results)
		{
			/* Unable to process results, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_searchresults_new
		 * Function Type: PurpleNotifySearchResults *
		 * Function RawType: PurpleNotifySearchResults
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleNotifySearchResults * purple_notify_searchresults_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_searchresults_new();

		public static /* Resolved as PointerToUnknownStruct to PurpleNotifySearchResults */ IntPtr SearchresultsNew()
		{
			/* Unable to process the return value of purple_notify_searchresults_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_searchresults_column_new
		 * Function Type: PurpleNotifySearchColumn *
		 * Function RawType: PurpleNotifySearchColumn
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleNotifySearchColumn * purple_notify_searchresults_column_new(char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_searchresults_column_new(string title);

		public static /* Resolved as PointerToUnknownStruct to PurpleNotifySearchColumn */ IntPtr SearchresultsColumnNew(string title)
		{
			/* Unable to process the return value of purple_notify_searchresults_column_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_searchresults_get_rows_count
		 * Function Type: guint
		 * Function RawType: guint
		 * Function Category: Native
		 */
		/*
		 * guint purple_notify_searchresults_get_rows_count(PurpleNotifySearchResults * results)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_notify_searchresults_get_rows_count(IntPtr results);

		public static uint SearchresultsGetRowsCount(/* Resolved as PointerToUnknownStruct to PurpleNotifySearchResults */ IntPtr results)
		{
			/* Unable to process results, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_searchresults_get_columns_count
		 * Function Type: guint
		 * Function RawType: guint
		 * Function Category: Native
		 */
		/*
		 * guint purple_notify_searchresults_get_columns_count(PurpleNotifySearchResults * results)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_notify_searchresults_get_columns_count(IntPtr results);

		public static uint SearchresultsGetColumnsCount(/* Resolved as PointerToUnknownStruct to PurpleNotifySearchResults */ IntPtr results)
		{
			/* Unable to process results, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_new
		 * Function Type: PurpleNotifyUserInfo *
		 * Function RawType: PurpleNotifyUserInfo
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleNotifyUserInfo * purple_notify_user_info_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_user_info_new();

		public static /* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr UserInfoNew()
		{
			/* Unable to process the return value of purple_notify_user_info_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_destroy(PurpleNotifyUserInfo * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_destroy(IntPtr user_info);

		public static void UserInfoDestroy(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr user_info)
		{
			/* Unable to process user_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_get_entries
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_notify_user_info_get_entries(PurpleNotifyUserInfo * user_info)
		 * 
		 * Could not generate a wrapper for purple_notify_user_info_get_entries in file "notify.h".
		 * Message: Unknown inner-type of argument 0 of purple_notify_user_info_get_entries
		 */

		/*
		 * Function Name: purple_notify_user_info_get_text_with_newline
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_notify_user_info_get_text_with_newline(PurpleNotifyUserInfo * user_info, char * newline)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_notify_user_info_get_text_with_newline(IntPtr user_info, string newline);

		public static string UserInfoGetTextWithNewline(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr user_info, string newline)
		{
			/* Unable to process user_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_add_pair
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_add_pair(PurpleNotifyUserInfo * user_info, char * label, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_add_pair(IntPtr user_info, string label, string value);

		public static void UserInfoAddPair(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr user_info, string label, string value)
		{
			/* Unable to process user_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_prepend_pair
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_prepend_pair(PurpleNotifyUserInfo * user_info, char * label, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_prepend_pair(IntPtr user_info, string label, string value);

		public static void UserInfoPrependPair(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr user_info, string label, string value)
		{
			/* Unable to process user_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_remove_entry
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_remove_entry(PurpleNotifyUserInfo * user_info, PurpleNotifyUserInfoEntry * user_info_entry)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_remove_entry(IntPtr user_info, IntPtr user_info_entry);

		public static void UserInfoRemoveEntry(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr user_info, /* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfoEntry */ IntPtr user_info_entry)
		{
			/* Unable to process user_info, a PointerToUnknownStruct. */
			/* Unable to process user_info_entry, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_entry_new
		 * Function Type: PurpleNotifyUserInfoEntry *
		 * Function RawType: PurpleNotifyUserInfoEntry
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleNotifyUserInfoEntry * purple_notify_user_info_entry_new(char * label, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_user_info_entry_new(string label, string value);

		public static /* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfoEntry */ IntPtr UserInfoEntryNew(string label, string value)
		{
			/* Unable to process the return value of purple_notify_user_info_entry_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_add_section_break
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_add_section_break(PurpleNotifyUserInfo * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_add_section_break(IntPtr user_info);

		public static void UserInfoAddSectionBreak(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr user_info)
		{
			/* Unable to process user_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_prepend_section_break
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_prepend_section_break(PurpleNotifyUserInfo * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_prepend_section_break(IntPtr user_info);

		public static void UserInfoPrependSectionBreak(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr user_info)
		{
			/* Unable to process user_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_add_section_header
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_add_section_header(PurpleNotifyUserInfo * user_info, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_add_section_header(IntPtr user_info, string label);

		public static void UserInfoAddSectionHeader(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr user_info, string label)
		{
			/* Unable to process user_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_prepend_section_header
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_prepend_section_header(PurpleNotifyUserInfo * user_info, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_prepend_section_header(IntPtr user_info, string label);

		public static void UserInfoPrependSectionHeader(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr user_info, string label)
		{
			/* Unable to process user_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_remove_last_item
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_remove_last_item(PurpleNotifyUserInfo * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_remove_last_item(IntPtr user_info);

		public static void UserInfoRemoveLastItem(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfo */ IntPtr user_info)
		{
			/* Unable to process user_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_entry_get_label
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_notify_user_info_entry_get_label(PurpleNotifyUserInfoEntry * user_info_entry)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_notify_user_info_entry_get_label(IntPtr user_info_entry);

		public static string UserInfoEntryGetLabel(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfoEntry */ IntPtr user_info_entry)
		{
			/* Unable to process user_info_entry, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_entry_set_label
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_entry_set_label(PurpleNotifyUserInfoEntry * user_info_entry, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_entry_set_label(IntPtr user_info_entry, string label);

		public static void UserInfoEntrySetLabel(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfoEntry */ IntPtr user_info_entry, string label)
		{
			/* Unable to process user_info_entry, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_entry_get_value
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_notify_user_info_entry_get_value(PurpleNotifyUserInfoEntry * user_info_entry)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_notify_user_info_entry_get_value(IntPtr user_info_entry);

		public static string UserInfoEntryGetValue(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfoEntry */ IntPtr user_info_entry)
		{
			/* Unable to process user_info_entry, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_entry_set_value
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_user_info_entry_set_value(PurpleNotifyUserInfoEntry * user_info_entry, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_entry_set_value(IntPtr user_info_entry, string value);

		public static void UserInfoEntrySetValue(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfoEntry */ IntPtr user_info_entry, string value)
		{
			/* Unable to process user_info_entry, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_user_info_entry_get_type
		 * Function Type: PurpleNotifyUserInfoEntryType
		 * Function RawType: PurpleNotifyUserInfoEntryType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleNotifyUserInfoEntryType purple_notify_user_info_entry_get_type(PurpleNotifyUserInfoEntry * user_info_entry)
		 */
		[DllImport("libpurple.dll")]
		private static extern Notify.PurpleNotifyUserInfoEntryType purple_notify_user_info_entry_get_type(IntPtr user_info_entry);

		public static Notify.PurpleNotifyUserInfoEntryType UserInfoEntryGetType(/* Resolved as PointerToUnknownStruct to PurpleNotifyUserInfoEntry */ IntPtr user_info_entry)
		{
			/* Unable to process user_info_entry, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_uri
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
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
		 * Function Name: purple_notify_close
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_notify_close_with_handle
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_notify_set_ui_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_notify_set_ui_ops(PurpleNotifyUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_set_ui_ops(IntPtr ops);

		public static void SetUiOps(/* Resolved as PointerToUnknownStruct to PurpleNotifyUiOps */ IntPtr ops)
		{
			/* Unable to process ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_get_ui_ops
		 * Function Type: PurpleNotifyUiOps *
		 * Function RawType: PurpleNotifyUiOps
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleNotifyUiOps * purple_notify_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_get_ui_ops();

		public static /* Resolved as PointerToUnknownStruct to PurpleNotifyUiOps */ IntPtr GetUiOps()
		{
			/* Unable to process the return value of purple_notify_get_ui_ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_notify_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
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
		 * Function Name: purple_notify_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_notify_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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


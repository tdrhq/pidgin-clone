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
	public class Notify
	{
		/*
		 * void * purple_notify_searchresults(PurpleConnection * gc, char * title, char * primary, char * secondary, PurpleNotifySearchResults * results, PurpleNotifyCloseCallback cb, gpointer user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_searchresults(IntPtr gc, string title, string primary, string secondary, IntPtr results, UNKNOWN cb, IntPtr user_data);

		public static IntPtr Searchresults(PurpleConnection gc, string title, string primary, string secondary, PurpleNotifySearchResults results, PurpleNotifyCloseCallback cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_searchresults_free(PurpleNotifySearchResults * results)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_searchresults_free(IntPtr results);

		public static void SearchresultsFree(PurpleNotifySearchResults results)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_searchresults_new_rows(PurpleConnection * gc, PurpleNotifySearchResults * results, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_searchresults_new_rows(IntPtr gc, IntPtr results, IntPtr data);

		public static void SearchresultsNewRows(PurpleConnection gc, PurpleNotifySearchResults results, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_searchresults_button_add(PurpleNotifySearchResults * results, PurpleNotifySearchButtonType type, PurpleNotifySearchResultsCallback cb)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_searchresults_button_add(IntPtr results, UNKNOWN type, UNKNOWN cb);

		public static void SearchresultsButtonAdd(PurpleNotifySearchResults results, PurpleNotifySearchButtonType type, PurpleNotifySearchResultsCallback cb)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_searchresults_button_add_labeled(PurpleNotifySearchResults * results, char * label, PurpleNotifySearchResultsCallback cb)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_searchresults_button_add_labeled(IntPtr results, string label, UNKNOWN cb);

		public static void SearchresultsButtonAddLabeled(PurpleNotifySearchResults results, string label, PurpleNotifySearchResultsCallback cb)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleNotifySearchResults * purple_notify_searchresults_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_searchresults_new();

		public static PurpleNotifySearchResults SearchresultsNew()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleNotifySearchColumn * purple_notify_searchresults_column_new(char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_searchresults_column_new(string title);

		public static PurpleNotifySearchColumn SearchresultsColumnNew(string title)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_searchresults_column_add(PurpleNotifySearchResults * results, PurpleNotifySearchColumn * column)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_searchresults_column_add(IntPtr results, IntPtr column);

		public static void SearchresultsColumnAdd(PurpleNotifySearchResults results, PurpleNotifySearchColumn column)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_searchresults_row_add(PurpleNotifySearchResults * results, GList * row)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_searchresults_row_add(IntPtr results, IntPtr row);

		public static void SearchresultsRowAdd(PurpleNotifySearchResults results, GList row)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_notify_searchresults_get_rows_count(PurpleNotifySearchResults * results)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_notify_searchresults_get_rows_count(IntPtr results);

		public static uint SearchresultsGetRowsCount(PurpleNotifySearchResults results)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_notify_searchresults_get_columns_count(PurpleNotifySearchResults * results)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_notify_searchresults_get_columns_count(IntPtr results);

		public static uint SearchresultsGetColumnsCount(PurpleNotifySearchResults results)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_notify_searchresults_row_get(PurpleNotifySearchResults * results, unsigned int)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_searchresults_row_get(IntPtr results, UNKNOWN int);

		public static GList SearchresultsRowGet(PurpleNotifySearchResults results, unsigned int)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_notify_searchresults_column_get_title(PurpleNotifySearchResults * results, unsigned int)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_notify_searchresults_column_get_title(IntPtr results, UNKNOWN int);

		public static string SearchresultsColumnGetTitle(PurpleNotifySearchResults results, unsigned int)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_notify_message(void * handle, PurpleNotifyMsgType type, char * title, char * primary, char * secondary, PurpleNotifyCloseCallback cb, gpointer user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_message(IntPtr handle, UNKNOWN type, string title, string primary, string secondary, UNKNOWN cb, IntPtr user_data);

		public static IntPtr Message(IntPtr handle, PurpleNotifyMsgType type, string title, string primary, string secondary, PurpleNotifyCloseCallback cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_notify_email(void * handle, char * subject, char * from, char * to, char * url, PurpleNotifyCloseCallback cb, gpointer user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_email(IntPtr handle, string subject, string from, string to, string url, UNKNOWN cb, IntPtr user_data);

		public static IntPtr Email(IntPtr handle, string subject, string from, string to, string url, PurpleNotifyCloseCallback cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_notify_emails(void * handle, size_t count, gboolean detailed, char ** subjects, char ** froms, char ** tos, char ** urls, PurpleNotifyCloseCallback cb, gpointer user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_emails(IntPtr handle, UNKNOWN count, bool detailed, IntPtr subjects, IntPtr froms, IntPtr tos, IntPtr urls, UNKNOWN cb, IntPtr user_data);

		public static IntPtr Emails(IntPtr handle, size_t count, bool detailed, char subjects, char froms, char tos, char urls, PurpleNotifyCloseCallback cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_notify_formatted(void * handle, char * title, char * primary, char * secondary, char * text, PurpleNotifyCloseCallback cb, gpointer user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_formatted(IntPtr handle, string title, string primary, string secondary, string text, UNKNOWN cb, IntPtr user_data);

		public static IntPtr Formatted(IntPtr handle, string title, string primary, string secondary, string text, PurpleNotifyCloseCallback cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_notify_userinfo(PurpleConnection * gc, char * who, PurpleNotifyUserInfo * user_info, PurpleNotifyCloseCallback cb, gpointer user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_userinfo(IntPtr gc, string who, IntPtr user_info, UNKNOWN cb, IntPtr user_data);

		public static IntPtr Userinfo(PurpleConnection gc, string who, PurpleNotifyUserInfo user_info, PurpleNotifyCloseCallback cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleNotifyUserInfo * purple_notify_user_info_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_user_info_new();

		public static PurpleNotifyUserInfo UserInfoNew()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_destroy(PurpleNotifyUserInfo * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_destroy(IntPtr user_info);

		public static void UserInfoDestroy(PurpleNotifyUserInfo user_info)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_notify_user_info_get_entries(PurpleNotifyUserInfo * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_user_info_get_entries(IntPtr user_info);

		public static GList UserInfoGetEntries(PurpleNotifyUserInfo user_info)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_notify_user_info_get_text_with_newline(PurpleNotifyUserInfo * user_info, char * newline)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_notify_user_info_get_text_with_newline(IntPtr user_info, string newline);

		public static string UserInfoGetTextWithNewline(PurpleNotifyUserInfo user_info, string newline)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_add_pair(PurpleNotifyUserInfo * user_info, char * label, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_add_pair(IntPtr user_info, string label, string value);

		public static void UserInfoAddPair(PurpleNotifyUserInfo user_info, string label, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_prepend_pair(PurpleNotifyUserInfo * user_info, char * label, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_prepend_pair(IntPtr user_info, string label, string value);

		public static void UserInfoPrependPair(PurpleNotifyUserInfo user_info, string label, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_remove_entry(PurpleNotifyUserInfo * user_info, PurpleNotifyUserInfoEntry * user_info_entry)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_remove_entry(IntPtr user_info, IntPtr user_info_entry);

		public static void UserInfoRemoveEntry(PurpleNotifyUserInfo user_info, PurpleNotifyUserInfoEntry user_info_entry)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleNotifyUserInfoEntry * purple_notify_user_info_entry_new(char * label, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_user_info_entry_new(string label, string value);

		public static PurpleNotifyUserInfoEntry UserInfoEntryNew(string label, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_add_section_break(PurpleNotifyUserInfo * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_add_section_break(IntPtr user_info);

		public static void UserInfoAddSectionBreak(PurpleNotifyUserInfo user_info)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_prepend_section_break(PurpleNotifyUserInfo * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_prepend_section_break(IntPtr user_info);

		public static void UserInfoPrependSectionBreak(PurpleNotifyUserInfo user_info)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_add_section_header(PurpleNotifyUserInfo * user_info, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_add_section_header(IntPtr user_info, string label);

		public static void UserInfoAddSectionHeader(PurpleNotifyUserInfo user_info, string label)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_prepend_section_header(PurpleNotifyUserInfo * user_info, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_prepend_section_header(IntPtr user_info, string label);

		public static void UserInfoPrependSectionHeader(PurpleNotifyUserInfo user_info, string label)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_remove_last_item(PurpleNotifyUserInfo * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_remove_last_item(IntPtr user_info);

		public static void UserInfoRemoveLastItem(PurpleNotifyUserInfo user_info)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_notify_user_info_entry_get_label(PurpleNotifyUserInfoEntry * user_info_entry)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_notify_user_info_entry_get_label(IntPtr user_info_entry);

		public static string UserInfoEntryGetLabel(PurpleNotifyUserInfoEntry user_info_entry)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_entry_set_label(PurpleNotifyUserInfoEntry * user_info_entry, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_entry_set_label(IntPtr user_info_entry, string label);

		public static void UserInfoEntrySetLabel(PurpleNotifyUserInfoEntry user_info_entry, string label)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_notify_user_info_entry_get_value(PurpleNotifyUserInfoEntry * user_info_entry)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_notify_user_info_entry_get_value(IntPtr user_info_entry);

		public static string UserInfoEntryGetValue(PurpleNotifyUserInfoEntry user_info_entry)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_entry_set_value(PurpleNotifyUserInfoEntry * user_info_entry, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_entry_set_value(IntPtr user_info_entry, string value);

		public static void UserInfoEntrySetValue(PurpleNotifyUserInfoEntry user_info_entry, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleNotifyUserInfoEntryType purple_notify_user_info_entry_get_type(PurpleNotifyUserInfoEntry * user_info_entry)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_notify_user_info_entry_get_type(IntPtr user_info_entry);

		public static PurpleNotifyUserInfoEntryType UserInfoEntryGetType(PurpleNotifyUserInfoEntry user_info_entry)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_user_info_entry_set_type(PurpleNotifyUserInfoEntry * user_info_entry, PurpleNotifyUserInfoEntryType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_user_info_entry_set_type(IntPtr user_info_entry, UNKNOWN type);

		public static void UserInfoEntrySetType(PurpleNotifyUserInfoEntry user_info_entry, PurpleNotifyUserInfoEntryType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_notify_uri(void * handle, char * uri)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_uri(IntPtr handle, string uri);

		public static IntPtr Uri(IntPtr handle, string uri)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_close(PurpleNotifyType type, void * ui_handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_close(UNKNOWN type, IntPtr ui_handle);

		public static void Close(PurpleNotifyType type, IntPtr ui_handle)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_close_with_handle(void * handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_close_with_handle(IntPtr handle);

		public static void CloseWithHandle(IntPtr handle)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_set_ui_ops(PurpleNotifyUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleNotifyUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleNotifyUiOps * purple_notify_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_get_ui_ops();

		public static PurpleNotifyUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_notify_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_notify_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_notify_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_notify_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

	}
}


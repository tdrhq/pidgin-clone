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
	public class Roomlist
	{
		/*
		 * void purple_roomlist_show_with_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_show_with_account(IntPtr account);

		public static void ShowWithAccount(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRoomlist * purple_roomlist_new(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_roomlist_new(IntPtr account);

		public static PurpleRoomlist New(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_roomlist_ref(PurpleRoomlist * list)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_ref(IntPtr list);

		public static void Ref(PurpleRoomlist list)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_roomlist_unref(PurpleRoomlist * list)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_unref(IntPtr list);

		public static void Unref(PurpleRoomlist list)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_roomlist_set_fields(PurpleRoomlist * list, GList * fields)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_set_fields(IntPtr list, IntPtr fields);

		public static void SetFields(PurpleRoomlist list, GList fields)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_roomlist_set_in_progress(PurpleRoomlist * list, gboolean in_progress)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_set_in_progress(IntPtr list, bool in_progress);

		public static void SetInProgress(PurpleRoomlist list, bool in_progress)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_roomlist_get_in_progress(PurpleRoomlist * list)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_roomlist_get_in_progress(IntPtr list);

		public static bool GetInProgress(PurpleRoomlist list)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_roomlist_room_add(PurpleRoomlist * list, PurpleRoomlistRoom * room)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_room_add(IntPtr list, IntPtr room);

		public static void RoomAdd(PurpleRoomlist list, PurpleRoomlistRoom room)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRoomlist * purple_roomlist_get_list(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_roomlist_get_list(IntPtr gc);

		public static PurpleRoomlist GetList(PurpleConnection gc)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_roomlist_cancel_get_list(PurpleRoomlist * list)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_cancel_get_list(IntPtr list);

		public static void CancelGetList(PurpleRoomlist list)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_roomlist_expand_category(PurpleRoomlist * list, PurpleRoomlistRoom * category)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_expand_category(IntPtr list, IntPtr category);

		public static void ExpandCategory(PurpleRoomlist list, PurpleRoomlistRoom category)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRoomlistRoom * purple_roomlist_room_new(PurpleRoomlistRoomType type, gchar * name, PurpleRoomlistRoom * parent)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_roomlist_room_new(UNKNOWN type, string name, IntPtr parent);

		public static PurpleRoomlistRoom RoomNew(PurpleRoomlistRoomType type, string name, PurpleRoomlistRoom parent)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_roomlist_room_add_field(PurpleRoomlist * list, PurpleRoomlistRoom * room, gconstpointer field)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_room_add_field(IntPtr list, IntPtr room, UNKNOWN field);

		public static void RoomAddField(PurpleRoomlist list, PurpleRoomlistRoom room, gconstpointer field)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_roomlist_room_join(PurpleRoomlist * list, PurpleRoomlistRoom * room)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_room_join(IntPtr list, IntPtr room);

		public static void RoomJoin(PurpleRoomlist list, PurpleRoomlistRoom room)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRoomlistRoomType purple_roomlist_room_get_type(PurpleRoomlistRoom * room)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_roomlist_room_get_type(IntPtr room);

		public static PurpleRoomlistRoomType RoomGetType(PurpleRoomlistRoom room)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRoomlistField * purple_roomlist_field_new(PurpleRoomlistFieldType type, gchar * label, gchar * name, gboolean hidden)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_roomlist_field_new(UNKNOWN type, string label, string name, bool hidden);

		public static PurpleRoomlistField FieldNew(PurpleRoomlistFieldType type, string label, string name, bool hidden)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRoomlistFieldType purple_roomlist_field_get_type(PurpleRoomlistField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_roomlist_field_get_type(IntPtr field);

		public static PurpleRoomlistFieldType FieldGetType(PurpleRoomlistField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_roomlist_field_get_hidden(PurpleRoomlistField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_roomlist_field_get_hidden(IntPtr field);

		public static bool FieldGetHidden(PurpleRoomlistField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_roomlist_set_ui_ops(PurpleRoomlistUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleRoomlistUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRoomlistUiOps * purple_roomlist_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_roomlist_get_ui_ops();

		public static PurpleRoomlistUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

	}
}


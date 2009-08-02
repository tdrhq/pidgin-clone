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
	 * File: roomlist.h
	 */
	public class Roomlist
	{
		public enum PurpleRoomlistRoomType
		{
			PURPLE_ROOMLIST_ROOMTYPE_CATEGORY = 0x01,
			PURPLE_ROOMLIST_ROOMTYPE_ROOM = 0x02
		};

		public enum PurpleRoomlistFieldType
		{
			PURPLE_ROOMLIST_FIELD_BOOL,
			PURPLE_ROOMLIST_FIELD_INT,
			PURPLE_ROOMLIST_FIELD_STRING
		};


		/*
		 * Function Name: purple_roomlist_show_with_account
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_show_with_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_show_with_account(IntPtr account);

		public static void ShowWithAccount(PurpleAccount account)
		{
			purple_roomlist_show_with_account(account.Reference);
		}

		/*
		 * Function Name: purple_roomlist_new
		 * Function Type: PurpleRoomlist *
		 * Function RawType: PurpleRoomlist
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleRoomlist * purple_roomlist_new(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_roomlist_new(IntPtr account);

		public static PurpleRoomlist New(PurpleAccount account)
		{
			return new PurpleRoomlist(purple_roomlist_new(account.Reference));
		}

		/*
		 * Function Name: purple_roomlist_ref
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_ref(PurpleRoomlist * list)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_ref(IntPtr list);

		public static void Ref(PurpleRoomlist list)
		{
			purple_roomlist_ref(list.Reference);
		}

		/*
		 * Function Name: purple_roomlist_unref
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_unref(PurpleRoomlist * list)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_unref(IntPtr list);

		public static void Unref(PurpleRoomlist list)
		{
			purple_roomlist_unref(list.Reference);
		}

		/*
		 * Function Name: purple_roomlist_set_fields
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_set_fields(PurpleRoomlist * list, GList * fields)
		 * 
		 * Could not generate a wrapper for purple_roomlist_set_fields in file "roomlist.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_roomlist_set_in_progress
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_set_in_progress(PurpleRoomlist * list, gboolean in_progress)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_set_in_progress(IntPtr list, bool in_progress);

		public static void SetInProgress(PurpleRoomlist list, bool in_progress)
		{
			purple_roomlist_set_in_progress(list.Reference, in_progress);
		}

		/*
		 * Function Name: purple_roomlist_get_in_progress
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_roomlist_get_in_progress(PurpleRoomlist * list)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_roomlist_get_in_progress(IntPtr list);

		public static bool GetInProgress(PurpleRoomlist list)
		{
			return purple_roomlist_get_in_progress(list.Reference);
		}

		/*
		 * Function Name: purple_roomlist_room_add
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_room_add(PurpleRoomlist * list, PurpleRoomlistRoom * room)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_room_add(IntPtr list, IntPtr room);

		public static void RoomAdd(PurpleRoomlist list, PurpleRoomlistRoom room)
		{
			purple_roomlist_room_add(list.Reference, room.Reference);
		}

		/*
		 * Function Name: purple_roomlist_get_list
		 * Function Type: PurpleRoomlist *
		 * Function RawType: PurpleRoomlist
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleRoomlist * purple_roomlist_get_list(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_roomlist_get_list(IntPtr gc);

		public static PurpleRoomlist GetList(PurpleConnection gc)
		{
			return new PurpleRoomlist(purple_roomlist_get_list(gc.Reference));
		}

		/*
		 * Function Name: purple_roomlist_cancel_get_list
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_cancel_get_list(PurpleRoomlist * list)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_cancel_get_list(IntPtr list);

		public static void CancelGetList(PurpleRoomlist list)
		{
			purple_roomlist_cancel_get_list(list.Reference);
		}

		/*
		 * Function Name: purple_roomlist_expand_category
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_expand_category(PurpleRoomlist * list, PurpleRoomlistRoom * category)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_expand_category(IntPtr list, IntPtr category);

		public static void ExpandCategory(PurpleRoomlist list, PurpleRoomlistRoom category)
		{
			purple_roomlist_expand_category(list.Reference, category.Reference);
		}

		/*
		 * Function Name: purple_roomlist_get_fields
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_roomlist_get_fields(PurpleRoomlist * roomlist)
		 * 
		 * Could not generate a wrapper for purple_roomlist_get_fields in file "roomlist.h".
		 * Message: Unknown inner-type of argument 0 of purple_roomlist_get_fields
		 */

		/*
		 * Function Name: purple_roomlist_room_add_field
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_room_add_field(PurpleRoomlist * list, PurpleRoomlistRoom * room, gconstpointer field)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_room_add_field(IntPtr list, IntPtr room, IntPtr field);

		public static void RoomAddField(PurpleRoomlist list, PurpleRoomlistRoom room, IntPtr field)
		{
			purple_roomlist_room_add_field(list.Reference, room.Reference, field);
		}

		/*
		 * Function Name: purple_roomlist_room_join
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_room_join(PurpleRoomlist * list, PurpleRoomlistRoom * room)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_room_join(IntPtr list, IntPtr room);

		public static void RoomJoin(PurpleRoomlist list, PurpleRoomlistRoom room)
		{
			purple_roomlist_room_join(list.Reference, room.Reference);
		}

		/*
		 * Function Name: purple_roomlist_room_get_type
		 * Function Type: PurpleRoomlistRoomType
		 * Function RawType: PurpleRoomlistRoomType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleRoomlistRoomType purple_roomlist_room_get_type(PurpleRoomlistRoom * room)
		 */
		[DllImport("libpurple.dll")]
		private static extern Roomlist.PurpleRoomlistRoomType purple_roomlist_room_get_type(IntPtr room);

		public static Roomlist.PurpleRoomlistRoomType RoomGetType(PurpleRoomlistRoom room)
		{
			/* Unable to process the return value of purple_roomlist_room_get_type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_roomlist_room_get_name
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_roomlist_room_get_name(PurpleRoomlistRoom * room)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_roomlist_room_get_name(IntPtr room);

		public static string RoomGetName(PurpleRoomlistRoom room)
		{
			return purple_roomlist_room_get_name(room.Reference);
		}

		/*
		 * Function Name: purple_roomlist_room_get_parent
		 * Function Type: PurpleRoomlistRoom *
		 * Function RawType: PurpleRoomlistRoom
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleRoomlistRoom * purple_roomlist_room_get_parent(PurpleRoomlistRoom * room)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_roomlist_room_get_parent(IntPtr room);

		public static PurpleRoomlistRoom RoomGetParent(PurpleRoomlistRoom room)
		{
			return new PurpleRoomlistRoom(purple_roomlist_room_get_parent(room.Reference));
		}

		/*
		 * Function Name: purple_roomlist_room_get_fields
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_roomlist_room_get_fields(PurpleRoomlistRoom * room)
		 * 
		 * Could not generate a wrapper for purple_roomlist_room_get_fields in file "roomlist.h".
		 * Message: Unknown inner-type of argument 0 of purple_roomlist_room_get_fields
		 */

		/*
		 * Function Name: purple_roomlist_field_get_type
		 * Function Type: PurpleRoomlistFieldType
		 * Function RawType: PurpleRoomlistFieldType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleRoomlistFieldType purple_roomlist_field_get_type(PurpleRoomlistField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern Roomlist.PurpleRoomlistFieldType purple_roomlist_field_get_type(IntPtr field);

		public static Roomlist.PurpleRoomlistFieldType FieldGetType(PurpleRoomlistField field)
		{
			/* Unable to process the return value of purple_roomlist_field_get_type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_roomlist_field_get_label
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_roomlist_field_get_label(PurpleRoomlistField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_roomlist_field_get_label(IntPtr field);

		public static string FieldGetLabel(PurpleRoomlistField field)
		{
			return purple_roomlist_field_get_label(field.Reference);
		}

		/*
		 * Function Name: purple_roomlist_field_get_hidden
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_roomlist_field_get_hidden(PurpleRoomlistField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_roomlist_field_get_hidden(IntPtr field);

		public static bool FieldGetHidden(PurpleRoomlistField field)
		{
			return purple_roomlist_field_get_hidden(field.Reference);
		}

		/*
		 * Function Name: purple_roomlist_set_ui_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_roomlist_set_ui_ops(PurpleRoomlistUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_roomlist_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleRoomlistUiOps ops)
		{
			purple_roomlist_set_ui_ops(ops.Reference);
		}

		/*
		 * Function Name: purple_roomlist_get_ui_ops
		 * Function Type: PurpleRoomlistUiOps *
		 * Function RawType: PurpleRoomlistUiOps
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleRoomlistUiOps * purple_roomlist_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_roomlist_get_ui_ops();

		public static PurpleRoomlistUiOps GetUiOps()
		{
			return new PurpleRoomlistUiOps(purple_roomlist_get_ui_ops());
		}

	}
}


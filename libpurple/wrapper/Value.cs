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
	 * File: value.h
	 */
	public class Value
	{
		public enum PurpleType
		{
			PURPLE_TYPE_UNKNOWN = 0,
			PURPLE_TYPE_SUBTYPE,
			PURPLE_TYPE_CHAR,
			PURPLE_TYPE_UCHAR,
			PURPLE_TYPE_BOOLEAN,
			PURPLE_TYPE_SHORT,
			PURPLE_TYPE_USHORT,
			PURPLE_TYPE_INT,
			PURPLE_TYPE_UINT,
			PURPLE_TYPE_LONG,
			PURPLE_TYPE_ULONG,
			PURPLE_TYPE_INT64,
			PURPLE_TYPE_UINT64,
			PURPLE_TYPE_STRING,
			PURPLE_TYPE_OBJECT,
			PURPLE_TYPE_POINTER,
			PURPLE_TYPE_ENUM,
			PURPLE_TYPE_BOXED
		};

		public enum PurpleSubType
		{
			PURPLE_SUBTYPE_UNKNOWN = 0,
			PURPLE_SUBTYPE_ACCOUNT,
			PURPLE_SUBTYPE_BLIST,
			PURPLE_SUBTYPE_BLIST_BUDDY,
			PURPLE_SUBTYPE_BLIST_GROUP,
			PURPLE_SUBTYPE_BLIST_CHAT,
			PURPLE_SUBTYPE_BUDDY_ICON,
			PURPLE_SUBTYPE_CONNECTION,
			PURPLE_SUBTYPE_CONVERSATION,
			PURPLE_SUBTYPE_PLUGIN,
			PURPLE_SUBTYPE_BLIST_NODE,
			PURPLE_SUBTYPE_CIPHER,
			PURPLE_SUBTYPE_STATUS,
			PURPLE_SUBTYPE_LOG,
			PURPLE_SUBTYPE_XFER,
			PURPLE_SUBTYPE_SAVEDSTATUS,
			PURPLE_SUBTYPE_XMLNODE,
			PURPLE_SUBTYPE_USERINFO,
			PURPLE_SUBTYPE_STORED_IMAGE,
			PURPLE_SUBTYPE_CERTIFICATEPOOL
		};


		/*
		 * Function Name: purple_value_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_destroy(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_destroy(IntPtr value);

		public static void Destroy(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_dup
		 * Function Type: PurpleValue *
		 * Function RawType: PurpleValue
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleValue * purple_value_dup(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_value_dup(IntPtr value);

		public static /* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr Dup(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_type
		 * Function Type: PurpleType
		 * Function RawType: PurpleType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleType purple_value_get_type(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern Value.PurpleType purple_value_get_type(IntPtr value);

		public static Value.PurpleType GetType(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_subtype
		 * Function Type: unsigned int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * unsigned int purple_value_get_subtype(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_value_get_subtype(IntPtr value);

		public static uint GetSubtype(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_specific_type
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_value_get_specific_type(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_value_get_specific_type(IntPtr value);

		public static string GetSpecificType(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_is_outgoing
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_value_is_outgoing(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_value_is_outgoing(IntPtr value);

		public static bool IsOutgoing(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_char
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_char(PurpleValue * value, char data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_char(IntPtr value, char data);

		public static void SetChar(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, char data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_uchar
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_uchar(PurpleValue * value, unsigned char data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_uchar(IntPtr value, byte data);

		public static void SetUchar(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, byte data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_boolean
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_boolean(PurpleValue * value, gboolean data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_boolean(IntPtr value, bool data);

		public static void SetBoolean(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, bool data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_short
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_short(PurpleValue * value, short data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_short(IntPtr value, short data);

		public static void SetShort(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, short data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_ushort
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_ushort(PurpleValue * value, unsigned short data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_ushort(IntPtr value, ushort data);

		public static void SetUshort(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, ushort data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_int
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_int(PurpleValue * value, int data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_int(IntPtr value, int data);

		public static void SetInt(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, int data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_uint
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_uint(PurpleValue * value, unsigned int data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_uint(IntPtr value, uint data);

		public static void SetUint(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, uint data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_long
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_long(PurpleValue * value, long data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_long(IntPtr value, long data);

		public static void SetLong(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, long data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_ulong
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_ulong(PurpleValue * value, unsigned long data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_ulong(IntPtr value, ulong data);

		public static void SetUlong(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, ulong data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_int64
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_int64(PurpleValue * value, gint64 data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_int64(IntPtr value, long data);

		public static void SetInt64(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, long data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_uint64
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_uint64(PurpleValue * value, guint64 data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_uint64(IntPtr value, ulong data);

		public static void SetUint64(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, ulong data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_string
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_string(PurpleValue * value, char * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_string(IntPtr value, string data);

		public static void SetString(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, string data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_object
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_object(PurpleValue * value, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_object(IntPtr value, IntPtr data);

		public static void SetObject(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, IntPtr data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_pointer
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_pointer(PurpleValue * value, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_pointer(IntPtr value, IntPtr data);

		public static void SetPointer(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, IntPtr data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_enum
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_enum(PurpleValue * value, int data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_enum(IntPtr value, int data);

		public static void SetEnum(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, int data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_set_boxed
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_value_set_boxed(PurpleValue * value, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_boxed(IntPtr value, IntPtr data);

		public static void SetBoxed(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value, IntPtr data)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_char
		 * Function Type: char
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char purple_value_get_char(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern char purple_value_get_char(IntPtr value);

		public static char GetChar(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_uchar
		 * Function Type: unsigned char
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * unsigned char purple_value_get_uchar(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern byte purple_value_get_uchar(IntPtr value);

		public static byte GetUchar(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_boolean
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_value_get_boolean(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_value_get_boolean(IntPtr value);

		public static bool GetBoolean(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_short
		 * Function Type: short
		 * Function RawType: short
		 * Function Category: Native
		 */
		/*
		 * short purple_value_get_short(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern short purple_value_get_short(IntPtr value);

		public static short GetShort(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_ushort
		 * Function Type: unsigned short
		 * Function RawType: short
		 * Function Category: Native
		 */
		/*
		 * unsigned short purple_value_get_ushort(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern ushort purple_value_get_ushort(IntPtr value);

		public static ushort GetUshort(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_int
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * int purple_value_get_int(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_value_get_int(IntPtr value);

		public static int GetInt(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_uint
		 * Function Type: unsigned int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * unsigned int purple_value_get_uint(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_value_get_uint(IntPtr value);

		public static uint GetUint(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_long
		 * Function Type: long
		 * Function RawType: long
		 * Function Category: Native
		 */
		/*
		 * long purple_value_get_long(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern long purple_value_get_long(IntPtr value);

		public static long GetLong(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_ulong
		 * Function Type: unsigned long
		 * Function RawType: long
		 * Function Category: Native
		 */
		/*
		 * unsigned long purple_value_get_ulong(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_value_get_ulong(IntPtr value);

		public static ulong GetUlong(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_int64
		 * Function Type: gint64
		 * Function RawType: gint64
		 * Function Category: Native
		 */
		/*
		 * gint64 purple_value_get_int64(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern long purple_value_get_int64(IntPtr value);

		public static long GetInt64(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_uint64
		 * Function Type: guint64
		 * Function RawType: guint64
		 * Function Category: Native
		 */
		/*
		 * guint64 purple_value_get_uint64(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_value_get_uint64(IntPtr value);

		public static ulong GetUint64(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_string
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_value_get_string(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_value_get_string(IntPtr value);

		public static string GetString(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_object
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_value_get_object(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_value_get_object(IntPtr value);

		public static IntPtr GetObject(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_pointer
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_value_get_pointer(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_value_get_pointer(IntPtr value);

		public static IntPtr GetPointer(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_enum
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * int purple_value_get_enum(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_value_get_enum(IntPtr value);

		public static int GetEnum(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_value_get_boxed
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_value_get_boxed(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_value_get_boxed(IntPtr value);

		public static IntPtr GetBoxed(/* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr value)
		{
			/* Unable to process value, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


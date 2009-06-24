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
	public class Value
	{
		/*
		 * PurpleValue * purple_value_new(PurpleType type, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_value_new(UNKNOWN type, ...);

		public static PurpleValue New(PurpleType type, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleValue * purple_value_new_outgoing(PurpleType type, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_value_new_outgoing(UNKNOWN type, ...);

		public static PurpleValue NewOutgoing(PurpleType type, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_destroy(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_destroy(IntPtr value);

		public static void Destroy(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleValue * purple_value_dup(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_value_dup(IntPtr value);

		public static PurpleValue Dup(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleType purple_value_get_type(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_value_get_type(IntPtr value);

		public static PurpleType GetType(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_value_get_subtype(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_value_get_subtype(IntPtr value);

		public static int GetSubtype(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_value_get_specific_type(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_value_get_specific_type(IntPtr value);

		public static string GetSpecificType(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_value_is_outgoing(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_value_is_outgoing(IntPtr value);

		public static bool IsOutgoing(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_char(PurpleValue * value, char data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_char(IntPtr value, char data);

		public static void SetChar(PurpleValue value, char data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_uchar(PurpleValue * value, unsigned char)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_uchar(IntPtr value, UNKNOWN char);

		public static void SetUchar(PurpleValue value, unsigned char)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_boolean(PurpleValue * value, gboolean data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_boolean(IntPtr value, bool data);

		public static void SetBoolean(PurpleValue value, bool data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_short(PurpleValue * value, short data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_short(IntPtr value, short data);

		public static void SetShort(PurpleValue value, short data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_ushort(PurpleValue * value, unsigned short)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_ushort(IntPtr value, UNKNOWN short);

		public static void SetUshort(PurpleValue value, unsigned short)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_int(PurpleValue * value, int data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_int(IntPtr value, int data);

		public static void SetInt(PurpleValue value, int data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_uint(PurpleValue * value, unsigned int)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_uint(IntPtr value, UNKNOWN int);

		public static void SetUint(PurpleValue value, unsigned int)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_long(PurpleValue * value, long data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_long(IntPtr value, long data);

		public static void SetLong(PurpleValue value, long data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_ulong(PurpleValue * value, unsigned long)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_ulong(IntPtr value, UNKNOWN long);

		public static void SetUlong(PurpleValue value, unsigned long)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_int64(PurpleValue * value, gint64 data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_int64(IntPtr value, long data);

		public static void SetInt64(PurpleValue value, long data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_uint64(PurpleValue * value, guint64 data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_uint64(IntPtr value, ulong data);

		public static void SetUint64(PurpleValue value, ulong data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_string(PurpleValue * value, char * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_string(IntPtr value, string data);

		public static void SetString(PurpleValue value, string data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_object(PurpleValue * value, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_object(IntPtr value, IntPtr data);

		public static void SetObject(PurpleValue value, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_pointer(PurpleValue * value, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_pointer(IntPtr value, IntPtr data);

		public static void SetPointer(PurpleValue value, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_enum(PurpleValue * value, int data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_enum(IntPtr value, int data);

		public static void SetEnum(PurpleValue value, int data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_value_set_boxed(PurpleValue * value, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_value_set_boxed(IntPtr value, IntPtr data);

		public static void SetBoxed(PurpleValue value, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * char purple_value_get_char(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern char purple_value_get_char(IntPtr value);

		public static char GetChar(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * char purple_value_get_uchar(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern char purple_value_get_uchar(IntPtr value);

		public static char GetUchar(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_value_get_boolean(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_value_get_boolean(IntPtr value);

		public static bool GetBoolean(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * short purple_value_get_short(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern short purple_value_get_short(IntPtr value);

		public static short GetShort(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * short purple_value_get_ushort(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern short purple_value_get_ushort(IntPtr value);

		public static short GetUshort(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_value_get_int(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_value_get_int(IntPtr value);

		public static int GetInt(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_value_get_uint(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_value_get_uint(IntPtr value);

		public static int GetUint(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * long purple_value_get_long(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern long purple_value_get_long(IntPtr value);

		public static long GetLong(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * long purple_value_get_ulong(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern long purple_value_get_ulong(IntPtr value);

		public static long GetUlong(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * gint64 purple_value_get_int64(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern long purple_value_get_int64(IntPtr value);

		public static long GetInt64(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint64 purple_value_get_uint64(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_value_get_uint64(IntPtr value);

		public static ulong GetUint64(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_value_get_string(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_value_get_string(IntPtr value);

		public static string GetString(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_value_get_object(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_value_get_object(IntPtr value);

		public static IntPtr GetObject(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_value_get_pointer(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_value_get_pointer(IntPtr value);

		public static IntPtr GetPointer(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_value_get_enum(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_value_get_enum(IntPtr value);

		public static int GetEnum(PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_value_get_boxed(PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_value_get_boxed(IntPtr value);

		public static IntPtr GetBoxed(PurpleValue value)
		{
			throw new NotImplementedException();
		}

	}
}


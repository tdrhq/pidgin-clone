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
	 * File: stringref.h
	 */
	public class Stringref
	{

		/*
		 * Function Name: purple_stringref_new
		 * Function Type: PurpleStringref *
		 * Function RawType: PurpleStringref
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStringref * purple_stringref_new(char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_stringref_new(string value);

		public static /* Resolved as PointerToUnknownStruct to PurpleStringref */ IntPtr New(string value)
		{
			/* Unable to process the return value of purple_stringref_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_stringref_new_noref
		 * Function Type: PurpleStringref *
		 * Function RawType: PurpleStringref
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStringref * purple_stringref_new_noref(char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_stringref_new_noref(string value);

		public static /* Resolved as PointerToUnknownStruct to PurpleStringref */ IntPtr NewNoref(string value)
		{
			/* Unable to process the return value of purple_stringref_new_noref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_stringref_ref
		 * Function Type: PurpleStringref *
		 * Function RawType: PurpleStringref
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStringref * purple_stringref_ref(PurpleStringref * stringref)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_stringref_ref(IntPtr stringref);

		public static /* Resolved as PointerToUnknownStruct to PurpleStringref */ IntPtr Ref(/* Resolved as PointerToUnknownStruct to PurpleStringref */ IntPtr stringref)
		{
			/* Unable to process stringref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_stringref_unref
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_stringref_unref(PurpleStringref * stringref)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_stringref_unref(IntPtr stringref);

		public static void Unref(/* Resolved as PointerToUnknownStruct to PurpleStringref */ IntPtr stringref)
		{
			/* Unable to process stringref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_stringref_value
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_stringref_value(PurpleStringref * stringref)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_stringref_value(IntPtr stringref);

		public static string Value(/* Resolved as PointerToUnknownStruct to PurpleStringref */ IntPtr stringref)
		{
			/* Unable to process stringref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_stringref_cmp
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * int purple_stringref_cmp(PurpleStringref * s1, PurpleStringref * s2)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_stringref_cmp(IntPtr s1, IntPtr s2);

		public static int Cmp(/* Resolved as PointerToUnknownStruct to PurpleStringref */ IntPtr s1, /* Resolved as PointerToUnknownStruct to PurpleStringref */ IntPtr s2)
		{
			/* Unable to process s1, a PointerToUnknownStruct. */
			/* Unable to process s2, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_stringref_len
		 * Function Type: size_t
		 * Function RawType: size_t
		 * Function Category: Native
		 */
		/*
		 * size_t purple_stringref_len(PurpleStringref * stringref)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_stringref_len(IntPtr stringref);

		public static ulong Len(/* Resolved as PointerToUnknownStruct to PurpleStringref */ IntPtr stringref)
		{
			/* Unable to process stringref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


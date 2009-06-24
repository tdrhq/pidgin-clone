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
	public class Stringref
	{
		/*
		 * PurpleStringref * purple_stringref_new(char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_stringref_new(string value);

		public static PurpleStringref New(string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStringref * purple_stringref_new_noref(char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_stringref_new_noref(string value);

		public static PurpleStringref NewNoref(string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStringref * purple_stringref_printf(char * format, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_stringref_printf(string format, ...);

		public static PurpleStringref Printf(string format, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStringref * purple_stringref_ref(PurpleStringref * stringref)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_stringref_ref(IntPtr stringref);

		public static PurpleStringref Ref(PurpleStringref stringref)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_stringref_unref(PurpleStringref * stringref)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_stringref_unref(IntPtr stringref);

		public static void Unref(PurpleStringref stringref)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_stringref_value(PurpleStringref * stringref)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_stringref_value(IntPtr stringref);

		public static string Value(PurpleStringref stringref)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_stringref_cmp(PurpleStringref * s1, PurpleStringref * s2)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_stringref_cmp(IntPtr s1, IntPtr s2);

		public static int Cmp(PurpleStringref s1, PurpleStringref s2)
		{
			throw new NotImplementedException();
		}

		/*
		 * size_t purple_stringref_len(PurpleStringref * stringref)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_stringref_len(IntPtr stringref);

		public static size_t Len(PurpleStringref stringref)
		{
			throw new NotImplementedException();
		}

	}
}


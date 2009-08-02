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
	 * File: ntlm.h
	 */
	public class Ntlm
	{

		/*
		 * Function Name: purple_ntlm_gen_type1
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_ntlm_gen_type1(gchar * hostname, gchar * domain)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_ntlm_gen_type1(string hostname, string domain);

		public static string GenType1(string hostname, string domain)
		{
			return purple_ntlm_gen_type1(hostname, domain);
		}

		/*
		 * Function Name: purple_ntlm_parse_type2
		 * Function Type: guint8 *
		 * Function RawType: guint8
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * guint8 * purple_ntlm_parse_type2(gchar * type2, guint32 * flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ntlm_parse_type2(string type2, IntPtr flags);

		public static /* Resolved as PointerToUnknownStruct to guint8 */ IntPtr ParseType2(string type2, /* Resolved as PointerToUnknownStruct to guint32 */ IntPtr flags)
		{
			/* Unable to process flags, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_ntlm_gen_type3
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_ntlm_gen_type3(gchar * username, gchar * passw, gchar * hostname, gchar * domain, guint8 * nonce, guint32 * flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_ntlm_gen_type3(string username, string passw, string hostname, string domain, IntPtr nonce, IntPtr flags);

		public static string GenType3(string username, string passw, string hostname, string domain, /* Resolved as PointerToUnknownStruct to guint8 */ IntPtr nonce, /* Resolved as PointerToUnknownStruct to guint32 */ IntPtr flags)
		{
			/* Unable to process nonce, a PointerToUnknownStruct. */
			/* Unable to process flags, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


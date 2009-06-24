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
	public class Ntlm
	{
		/*
		 * gchar * purple_ntlm_gen_type1(gchar * hostname, gchar * domain)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_ntlm_gen_type1(string hostname, string domain);

		public static string GenType1(string hostname, string domain)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint8 * purple_ntlm_parse_type2(gchar * type2, guint32 * flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ntlm_parse_type2(string type2, IntPtr flags);

		public static guint8 ParseType2(string type2, guint32 flags)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_ntlm_gen_type3(gchar * username, gchar * passw, gchar * hostname, gchar * domain, guint8 * nonce, guint32 * flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_ntlm_gen_type3(string username, string passw, string hostname, string domain, IntPtr nonce, IntPtr flags);

		public static string GenType3(string username, string passw, string hostname, string domain, guint8 nonce, guint32 flags)
		{
			throw new NotImplementedException();
		}

	}
}


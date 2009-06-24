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
	public class Dnssrv
	{
		/*
		 * PurpleSrvQueryData * purple_srv_resolve(char * protocol, char * transport, char * domain, PurpleSrvCallback cb, gpointer extradata)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_srv_resolve(string protocol, string transport, string domain, UNKNOWN cb, IntPtr extradata);

		public static PurpleSrvQueryData SrvResolve(string protocol, string transport, string domain, PurpleSrvCallback cb, IntPtr extradata)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_srv_cancel(PurpleSrvQueryData * query_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_srv_cancel(IntPtr query_data);

		public static void SrvCancel(PurpleSrvQueryData query_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSrvQueryData * purple_txt_resolve(char * owner, char * domain, PurpleTxtCallback cb, gpointer extradata)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_txt_resolve(string owner, string domain, UNKNOWN cb, IntPtr extradata);

		public static PurpleSrvQueryData TxtResolve(string owner, string domain, PurpleTxtCallback cb, IntPtr extradata)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_txt_cancel(PurpleSrvQueryData * query_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_txt_cancel(IntPtr query_data);

		public static void TxtCancel(PurpleSrvQueryData query_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_txt_response_get_content(PurpleTxtResponse * resp)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_txt_response_get_content(IntPtr resp);

		public static string TxtResponseGetContent(PurpleTxtResponse resp)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_txt_response_destroy(PurpleTxtResponse * resp)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_txt_response_destroy(IntPtr resp);

		public static void TxtResponseDestroy(PurpleTxtResponse resp)
		{
			throw new NotImplementedException();
		}

	}
}


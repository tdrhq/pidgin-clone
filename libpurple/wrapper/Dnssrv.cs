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
	 * File: dnssrv.h
	 */
	public class Dnssrv
	{
		public delegate void PurpleSrvCallback(IntPtr resp, int results, IntPtr data);
		public delegate void PurpleTxtCallback(IntPtr responses, IntPtr data);

		/*
		 * Function Name: purple_srv_resolve
		 * Function Type: PurpleSrvQueryData *
		 * Function RawType: PurpleSrvQueryData
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSrvQueryData * purple_srv_resolve(char * protocol, char * transport, char * domain, PurpleSrvCallback cb, gpointer extradata)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_srv_resolve(string protocol, string transport, string domain, IntPtr cb, IntPtr extradata);

		public static /* Resolved as PointerToUnknownStruct to PurpleSrvQueryData */ IntPtr SrvResolve(string protocol, string transport, string domain, Dnssrv.PurpleSrvCallback cb, IntPtr extradata)
		{
			/* Unable to process cb, a KnownFunctionPointer. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_srv_cancel
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_srv_cancel(PurpleSrvQueryData * query_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_srv_cancel(IntPtr query_data);

		public static void SrvCancel(/* Resolved as PointerToUnknownStruct to PurpleSrvQueryData */ IntPtr query_data)
		{
			/* Unable to process query_data, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_txt_resolve
		 * Function Type: PurpleSrvQueryData *
		 * Function RawType: PurpleSrvQueryData
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSrvQueryData * purple_txt_resolve(char * owner, char * domain, PurpleTxtCallback cb, gpointer extradata)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_txt_resolve(string owner, string domain, IntPtr cb, IntPtr extradata);

		public static /* Resolved as PointerToUnknownStruct to PurpleSrvQueryData */ IntPtr TxtResolve(string owner, string domain, Dnssrv.PurpleTxtCallback cb, IntPtr extradata)
		{
			/* Unable to process cb, a KnownFunctionPointer. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_txt_cancel
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_txt_cancel(PurpleSrvQueryData * query_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_txt_cancel(IntPtr query_data);

		public static void TxtCancel(/* Resolved as PointerToUnknownStruct to PurpleSrvQueryData */ IntPtr query_data)
		{
			/* Unable to process query_data, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_txt_response_get_content
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_txt_response_get_content(PurpleTxtResponse * resp)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_txt_response_get_content(IntPtr resp);

		public static string TxtResponseGetContent(/* Resolved as PointerToUnknownStruct to PurpleTxtResponse */ IntPtr resp)
		{
			/* Unable to process resp, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_txt_response_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_txt_response_destroy(PurpleTxtResponse * resp)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_txt_response_destroy(IntPtr resp);

		public static void TxtResponseDestroy(/* Resolved as PointerToUnknownStruct to PurpleTxtResponse */ IntPtr resp)
		{
			/* Unable to process resp, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


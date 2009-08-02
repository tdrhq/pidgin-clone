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
	 * File: dnsquery.h
	 */
	public class Dnsquery
	{
		public delegate void PurpleDnsQueryConnectFunction(IntPtr hosts, IntPtr data, string error_message);
		public delegate void PurpleDnsQueryResolvedCallback(IntPtr query_data, IntPtr hosts);
		public delegate void PurpleDnsQueryFailedCallback(IntPtr query_data, string error_message);

		/*
		 * Function Name: purple_dnsquery_a
		 * Function Type: PurpleDnsQueryData *
		 * Function RawType: PurpleDnsQueryData
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleDnsQueryData * purple_dnsquery_a(char * hostname, int port, PurpleDnsQueryConnectFunction callback, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_dnsquery_a(string hostname, int port, IntPtr callback, IntPtr data);

		public static /* Resolved as PointerToUnknownStruct to PurpleDnsQueryData */ IntPtr A(string hostname, int port, Dnsquery.PurpleDnsQueryConnectFunction callback, IntPtr data)
		{
			/* Unable to process callback, a KnownFunctionPointer. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_dnsquery_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_dnsquery_destroy(PurpleDnsQueryData * query_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_dnsquery_destroy(IntPtr query_data);

		public static void Destroy(/* Resolved as PointerToUnknownStruct to PurpleDnsQueryData */ IntPtr query_data)
		{
			/* Unable to process query_data, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_dnsquery_set_ui_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_dnsquery_set_ui_ops(PurpleDnsQueryUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_dnsquery_set_ui_ops(IntPtr ops);

		public static void SetUiOps(/* Resolved as PointerToUnknownStruct to PurpleDnsQueryUiOps */ IntPtr ops)
		{
			/* Unable to process ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_dnsquery_get_ui_ops
		 * Function Type: PurpleDnsQueryUiOps *
		 * Function RawType: PurpleDnsQueryUiOps
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleDnsQueryUiOps * purple_dnsquery_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_dnsquery_get_ui_ops();

		public static /* Resolved as PointerToUnknownStruct to PurpleDnsQueryUiOps */ IntPtr GetUiOps()
		{
			/* Unable to process the return value of purple_dnsquery_get_ui_ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_dnsquery_get_host
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_dnsquery_get_host(PurpleDnsQueryData * query_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_dnsquery_get_host(IntPtr query_data);

		public static string GetHost(/* Resolved as PointerToUnknownStruct to PurpleDnsQueryData */ IntPtr query_data)
		{
			/* Unable to process query_data, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_dnsquery_get_port
		 * Function Type: unsigned short
		 * Function RawType: short
		 * Function Category: Native
		 */
		/*
		 * unsigned short purple_dnsquery_get_port(PurpleDnsQueryData * query_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern ushort purple_dnsquery_get_port(IntPtr query_data);

		public static ushort GetPort(/* Resolved as PointerToUnknownStruct to PurpleDnsQueryData */ IntPtr query_data)
		{
			/* Unable to process query_data, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_dnsquery_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_dnsquery_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_dnsquery_init();

		public static void Init()
		{
			purple_dnsquery_init();
		}

		/*
		 * Function Name: purple_dnsquery_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_dnsquery_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_dnsquery_uninit();

		public static void Uninit()
		{
			purple_dnsquery_uninit();
		}

	}
}


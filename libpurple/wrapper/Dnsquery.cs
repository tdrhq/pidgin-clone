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
	public class Dnsquery
	{
		/*
		 * PurpleDnsQueryData * purple_dnsquery_a(char * hostname, int port, PurpleDnsQueryConnectFunction callback, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_dnsquery_a(string hostname, int port, UNKNOWN callback, IntPtr data);

		public static PurpleDnsQueryData A(string hostname, int port, PurpleDnsQueryConnectFunction callback, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_dnsquery_destroy(PurpleDnsQueryData * query_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_dnsquery_destroy(IntPtr query_data);

		public static void Destroy(PurpleDnsQueryData query_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_dnsquery_set_ui_ops(PurpleDnsQueryUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_dnsquery_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleDnsQueryUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleDnsQueryUiOps * purple_dnsquery_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_dnsquery_get_ui_ops();

		public static PurpleDnsQueryUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_dnsquery_get_host(PurpleDnsQueryData * query_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_dnsquery_get_host(IntPtr query_data);

		public static string GetHost(PurpleDnsQueryData query_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * short purple_dnsquery_get_port(PurpleDnsQueryData * query_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern short purple_dnsquery_get_port(IntPtr query_data);

		public static short GetPort(PurpleDnsQueryData query_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_dnsquery_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_dnsquery_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_dnsquery_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_dnsquery_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

	}
}


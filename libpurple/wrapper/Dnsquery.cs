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
	public class Dnsquery
	{
		/*
		 * PurpleDnsQueryData * purple_dnsquery_a(char * hostname, int port, PurpleDnsQueryConnectFunction callback, gpointer data)
		 * 
		 * Could not generate a wrapper for purple_dnsquery_a in file "dnsquery.h".
		 * Message: The type could not be resolved (PurpleDnsQueryConnectFunction callback).
		 */

		/*
		 * void purple_dnsquery_destroy(PurpleDnsQueryData * query_data)
		 * 
		 * Could not generate a wrapper for purple_dnsquery_destroy in file "dnsquery.h".
		 * Message: The type could not be resolved (PurpleDnsQueryData * query_data).
		 */

		/*
		 * void purple_dnsquery_set_ui_ops(PurpleDnsQueryUiOps * ops)
		 * 
		 * Could not generate a wrapper for purple_dnsquery_set_ui_ops in file "dnsquery.h".
		 * Message: The type could not be resolved (PurpleDnsQueryUiOps * ops).
		 */

		/*
		 * PurpleDnsQueryUiOps * purple_dnsquery_get_ui_ops()
		 * 
		 * Could not generate a wrapper for purple_dnsquery_get_ui_ops in file "dnsquery.h".
		 * Message: The type could not be resolved (PurpleDnsQueryUiOps * purple_dnsquery_get_ui_ops()).
		 */

		/*
		 * char * purple_dnsquery_get_host(PurpleDnsQueryData * query_data)
		 * 
		 * Could not generate a wrapper for purple_dnsquery_get_host in file "dnsquery.h".
		 * Message: The type could not be resolved (PurpleDnsQueryData * query_data).
		 */

		/*
		 * unsigned short purple_dnsquery_get_port(PurpleDnsQueryData * query_data)
		 * 
		 * Could not generate a wrapper for purple_dnsquery_get_port in file "dnsquery.h".
		 * Message: The type could not be resolved (PurpleDnsQueryData * query_data).
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


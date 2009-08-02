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
	 * File: upnp.h
	 */
	public class Upnp
	{
		public delegate void PurpleUPnPCallback(bool success, IntPtr data);

		/*
		 * Function Name: purple_upnp_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_upnp_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_upnp_init();

		public static void Init()
		{
			purple_upnp_init();
		}

		/*
		 * Function Name: purple_upnp_discover
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_upnp_discover(PurpleUPnPCallback cb, gpointer cb_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_upnp_discover(IntPtr cb, IntPtr cb_data);

		public static void Discover(Upnp.PurpleUPnPCallback cb, IntPtr cb_data)
		{
			/* Unable to process cb, a KnownFunctionPointer. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_upnp_get_control_info
		 * Function Type: PurpleUPnPControlInfo*
		 * Function RawType: PurpleUPnPControlInfo*
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleUPnPControlInfo* purple_upnp_get_control_info()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_upnp_get_control_info();

		public static /* Resolved as PointerToUnknownStruct to PurpleUPnPControlInfo* */ IntPtr GetControlInfo()
		{
			/* Unable to process the return value of purple_upnp_get_control_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_upnp_get_public_ip
		 * Function Type: gchar*
		 * Function RawType: gchar*
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * gchar* purple_upnp_get_public_ip()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_upnp_get_public_ip();

		public static /* Resolved as PointerToUnknownStruct to gchar* */ IntPtr GetPublicIp()
		{
			/* Unable to process the return value of purple_upnp_get_public_ip, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_upnp_cancel_port_mapping
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_upnp_cancel_port_mapping(UPnPMappingAddRemove * mapping_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_upnp_cancel_port_mapping(IntPtr mapping_data);

		public static void CancelPortMapping(/* Resolved as PointerToUnknownStruct to UPnPMappingAddRemove */ IntPtr mapping_data)
		{
			/* Unable to process mapping_data, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


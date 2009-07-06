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
	public class Upnp
	{
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
		 * void purple_upnp_discover(PurpleUPnPCallback cb, gpointer cb_data)
		 * 
		 * Could not generate a wrapper for purple_upnp_discover in file "upnp.h".
		 * Message: The type could not be resolved (PurpleUPnPCallback cb).
		 */

		/*
		 * PurpleUPnPControlInfo* purple_upnp_get_control_info()
		 * 
		 * Could not generate a wrapper for purple_upnp_get_control_info in file "upnp.h".
		 * Message: The type could not be resolved (PurpleUPnPControlInfo* purple_upnp_get_control_info()).
		 */

		/*
		 * gchar* purple_upnp_get_public_ip()
		 * 
		 * Could not generate a wrapper for purple_upnp_get_public_ip in file "upnp.h".
		 * Message: The type could not be resolved (gchar* purple_upnp_get_public_ip()).
		 */

		/*
		 * void purple_upnp_cancel_port_mapping(UPnPMappingAddRemove * mapping_data)
		 * 
		 * Could not generate a wrapper for purple_upnp_cancel_port_mapping in file "upnp.h".
		 * Message: The type could not be resolved (UPnPMappingAddRemove * mapping_data).
		 */

	}
}


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
	public class Upnp
	{
		/*
		 * void purple_upnp_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_upnp_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_upnp_discover(PurpleUPnPCallback cb, gpointer cb_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_upnp_discover(UNKNOWN cb, IntPtr cb_data);

		public static void Discover(PurpleUPnPCallback cb, IntPtr cb_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_upnp_cancel_port_mapping(UPnPMappingAddRemove * mapping_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_upnp_cancel_port_mapping(IntPtr mapping_data);

		public static void CancelPortMapping(UPnPMappingAddRemove mapping_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * UPnPMappingAddRemove * purple_upnp_set_port_mapping(unsigned short,  , PurpleUPnPCallback cb, gpointer cb_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_upnp_set_port_mapping(UNKNOWN short, UNKNOWN , UNKNOWN cb, IntPtr cb_data);

		public static UPnPMappingAddRemove SetPortMapping(unsigned short,  , PurpleUPnPCallback cb, IntPtr cb_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * UPnPMappingAddRemove * purple_upnp_remove_port_mapping(unsigned short,  , PurpleUPnPCallback cb, gpointer cb_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_upnp_remove_port_mapping(UNKNOWN short, UNKNOWN , UNKNOWN cb, IntPtr cb_data);

		public static UPnPMappingAddRemove RemovePortMapping(unsigned short,  , PurpleUPnPCallback cb, IntPtr cb_data)
		{
			throw new NotImplementedException();
		}

	}
}


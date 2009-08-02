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
	 * File: network.h
	 */
	public class Network
	{
		public delegate void PurpleNetworkListenCallback(int listenfd, IntPtr data);

		/*
		 * Function Name: purple_network_ip_atoi
		 * Function Type: unsigned char *
		 * Function RawType: char
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * unsigned char * purple_network_ip_atoi(char * ip)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_network_ip_atoi(string ip);

		public static /* Resolved as PointerToUnknownStruct to char */ IntPtr IpAtoi(string ip)
		{
			/* Unable to process the return value of purple_network_ip_atoi, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_network_set_public_ip
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_network_set_public_ip(char * ip)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_set_public_ip(string ip);

		public static void SetPublicIp(string ip)
		{
			purple_network_set_public_ip(ip);
		}

		/*
		 * Function Name: purple_network_get_public_ip
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_network_get_public_ip()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_public_ip();

		public static string GetPublicIp()
		{
			return purple_network_get_public_ip();
		}

		/*
		 * Function Name: purple_network_get_local_system_ip
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_network_get_local_system_ip(int fd)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_local_system_ip(int fd);

		public static string GetLocalSystemIp(int fd)
		{
			return purple_network_get_local_system_ip(fd);
		}

		/*
		 * Function Name: purple_network_get_my_ip
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_network_get_my_ip(int fd)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_my_ip(int fd);

		public static string GetMyIp(int fd)
		{
			return purple_network_get_my_ip(fd);
		}

		/*
		 * Function Name: purple_network_listen_map_external
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_network_listen_map_external(gboolean map_external)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_listen_map_external(bool map_external);

		public static void ListenMapExternal(bool map_external)
		{
			purple_network_listen_map_external(map_external);
		}

		/*
		 * Function Name: purple_network_listen_cancel
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_network_listen_cancel(PurpleNetworkListenData * listen_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_listen_cancel(IntPtr listen_data);

		public static void ListenCancel(/* Resolved as PointerToUnknownStruct to PurpleNetworkListenData */ IntPtr listen_data)
		{
			/* Unable to process listen_data, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_network_get_port_from_fd
		 * Function Type: unsigned short
		 * Function RawType: short
		 * Function Category: Native
		 */
		/*
		 * unsigned short purple_network_get_port_from_fd(int fd)
		 */
		[DllImport("libpurple.dll")]
		private static extern ushort purple_network_get_port_from_fd(int fd);

		public static ushort GetPortFromFd(int fd)
		{
			return purple_network_get_port_from_fd(fd);
		}

		/*
		 * Function Name: purple_network_is_available
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_network_is_available()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_network_is_available();

		public static bool IsAvailable()
		{
			return purple_network_is_available();
		}

		/*
		 * Function Name: purple_network_force_online
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_network_force_online()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_force_online();

		public static void ForceOnline()
		{
			purple_network_force_online();
		}

		/*
		 * Function Name: purple_network_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_network_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_network_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_network_get_handle();
		}

		/*
		 * Function Name: purple_network_set_stun_server
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_network_set_stun_server(gchar * stun_server)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_set_stun_server(string stun_server);

		public static void SetStunServer(string stun_server)
		{
			purple_network_set_stun_server(stun_server);
		}

		/*
		 * Function Name: purple_network_get_stun_ip
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_network_get_stun_ip()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_stun_ip();

		public static string GetStunIp()
		{
			return purple_network_get_stun_ip();
		}

		/*
		 * Function Name: purple_network_set_turn_server
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_network_set_turn_server(gchar * stun_server)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_set_turn_server(string stun_server);

		public static void SetTurnServer(string stun_server)
		{
			purple_network_set_turn_server(stun_server);
		}

		/*
		 * Function Name: purple_network_get_turn_ip
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_network_get_turn_ip()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_turn_ip();

		public static string GetTurnIp()
		{
			return purple_network_get_turn_ip();
		}

		/*
		 * Function Name: purple_network_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_network_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_init();

		public static void Init()
		{
			purple_network_init();
		}

		/*
		 * Function Name: purple_network_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_network_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_uninit();

		public static void Uninit()
		{
			purple_network_uninit();
		}

	}
}


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
	public class Network
	{
		/*
		 * unsigned char * purple_network_ip_atoi(char * ip)
		 * 
		 * Could not generate a wrapper for purple_network_ip_atoi in file "network.h".
		 * Message: The type could not be resolved (unsigned char * purple_network_ip_atoi(char * ip)).
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
		 * char * purple_network_get_public_ip()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_public_ip();

		public static string GetPublicIp()
		{
			return purple_network_get_public_ip();
		}

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
		 * char * purple_network_get_my_ip(int fd)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_my_ip(int fd);

		public static string GetMyIp(int fd)
		{
			return purple_network_get_my_ip(fd);
		}

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
		 * void purple_network_listen_cancel(PurpleNetworkListenData * listen_data)
		 * 
		 * Could not generate a wrapper for purple_network_listen_cancel in file "network.h".
		 * Message: The type could not be resolved (PurpleNetworkListenData * listen_data).
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
		 * gboolean purple_network_is_available()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_network_is_available();

		public static bool IsAvailable()
		{
			return purple_network_is_available();
		}

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
		 * void * purple_network_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_network_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_network_get_handle();
		}

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
		 * gchar * purple_network_get_stun_ip()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_stun_ip();

		public static string GetStunIp()
		{
			return purple_network_get_stun_ip();
		}

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
		 * gchar * purple_network_get_turn_ip()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_turn_ip();

		public static string GetTurnIp()
		{
			return purple_network_get_turn_ip();
		}

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


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
	public class Network
	{
		/*
		 * char * purple_network_ip_atoi(char * ip)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_ip_atoi(string ip);

		public static string IpAtoi(string ip)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_network_set_public_ip(char * ip)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_set_public_ip(string ip);

		public static void SetPublicIp(string ip)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_network_get_public_ip()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_public_ip();

		public static string GetPublicIp()
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_network_get_local_system_ip(int fd)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_local_system_ip(int fd);

		public static string GetLocalSystemIp(int fd)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_network_get_my_ip(int fd)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_my_ip(int fd);

		public static string GetMyIp(int fd)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_network_listen_map_external(gboolean map_external)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_listen_map_external(bool map_external);

		public static void ListenMapExternal(bool map_external)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleNetworkListenData * purple_network_listen(unsigned short, int socket_type, PurpleNetworkListenCallback cb, gpointer cb_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_network_listen(UNKNOWN short, int socket_type, UNKNOWN cb, IntPtr cb_data);

		public static PurpleNetworkListenData Listen(unsigned short, int socket_type, PurpleNetworkListenCallback cb, IntPtr cb_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleNetworkListenData * purple_network_listen_range(unsigned short, unsigned short, int socket_type, PurpleNetworkListenCallback cb, gpointer cb_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_network_listen_range(UNKNOWN short, UNKNOWN short, int socket_type, UNKNOWN cb, IntPtr cb_data);

		public static PurpleNetworkListenData ListenRange(unsigned short, unsigned short, int socket_type, PurpleNetworkListenCallback cb, IntPtr cb_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_network_listen_cancel(PurpleNetworkListenData * listen_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_listen_cancel(IntPtr listen_data);

		public static void ListenCancel(PurpleNetworkListenData listen_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * short purple_network_get_port_from_fd(int fd)
		 */
		[DllImport("libpurple.dll")]
		private static extern short purple_network_get_port_from_fd(int fd);

		public static short GetPortFromFd(int fd)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_network_is_available()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_network_is_available();

		public static bool IsAvailable()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_network_force_online()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_force_online();

		public static void ForceOnline()
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_network_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_network_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_network_set_stun_server(gchar * stun_server)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_set_stun_server(string stun_server);

		public static void SetStunServer(string stun_server)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_network_get_stun_ip()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_stun_ip();

		public static string GetStunIp()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_network_set_turn_server(gchar * stun_server)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_set_turn_server(string stun_server);

		public static void SetTurnServer(string stun_server)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_network_get_turn_ip()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_network_get_turn_ip();

		public static string GetTurnIp()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_network_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_network_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_network_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

	}
}


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
	public class Proxy
	{
		/*
		 * PurpleProxyInfo * purple_proxy_info_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_proxy_info_new();

		public static PurpleProxyInfo InfoNew()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_proxy_info_destroy(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_destroy(IntPtr info);

		public static void InfoDestroy(PurpleProxyInfo info)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_proxy_info_set_type(PurpleProxyInfo * info, PurpleProxyType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_set_type(IntPtr info, UNKNOWN type);

		public static void InfoSetType(PurpleProxyInfo info, PurpleProxyType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_proxy_info_set_host(PurpleProxyInfo * info, char * host)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_set_host(IntPtr info, string host);

		public static void InfoSetHost(PurpleProxyInfo info, string host)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_proxy_info_set_port(PurpleProxyInfo * info, int port)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_set_port(IntPtr info, int port);

		public static void InfoSetPort(PurpleProxyInfo info, int port)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_proxy_info_set_username(PurpleProxyInfo * info, char * username)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_set_username(IntPtr info, string username);

		public static void InfoSetUsername(PurpleProxyInfo info, string username)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_proxy_info_set_password(PurpleProxyInfo * info, char * password)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_set_password(IntPtr info, string password);

		public static void InfoSetPassword(PurpleProxyInfo info, string password)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleProxyType purple_proxy_info_get_type(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_proxy_info_get_type(IntPtr info);

		public static PurpleProxyType InfoGetType(PurpleProxyInfo info)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_proxy_info_get_host(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_proxy_info_get_host(IntPtr info);

		public static string InfoGetHost(PurpleProxyInfo info)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_proxy_info_get_port(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_proxy_info_get_port(IntPtr info);

		public static int InfoGetPort(PurpleProxyInfo info)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_proxy_info_get_username(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_proxy_info_get_username(IntPtr info);

		public static string InfoGetUsername(PurpleProxyInfo info)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_proxy_info_get_password(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_proxy_info_get_password(IntPtr info);

		public static string InfoGetPassword(PurpleProxyInfo info)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleProxyInfo * purple_global_proxy_get_info()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_global_proxy_get_info();

		public static PurpleProxyInfo GlobalProxyGetInfo()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_global_proxy_set_info(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_global_proxy_set_info(IntPtr info);

		public static void GlobalProxySetInfo(PurpleProxyInfo info)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_proxy_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_proxy_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_proxy_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_proxy_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleProxyInfo * purple_proxy_get_setup(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_proxy_get_setup(IntPtr account);

		public static PurpleProxyInfo GetSetup(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleProxyConnectData * purple_proxy_connect(void * handle, PurpleAccount * account, char * host, int port, PurpleProxyConnectFunction connect_cb, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_proxy_connect(IntPtr handle, IntPtr account, string host, int port, UNKNOWN connect_cb, IntPtr data);

		public static PurpleProxyConnectData Connect(IntPtr handle, PurpleAccount account, string host, int port, PurpleProxyConnectFunction connect_cb, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleProxyConnectData * purple_proxy_connect_udp(void * handle, PurpleAccount * account, char * host, int port, PurpleProxyConnectFunction connect_cb, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_proxy_connect_udp(IntPtr handle, IntPtr account, string host, int port, UNKNOWN connect_cb, IntPtr data);

		public static PurpleProxyConnectData ConnectUdp(IntPtr handle, PurpleAccount account, string host, int port, PurpleProxyConnectFunction connect_cb, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleProxyConnectData * purple_proxy_connect_socks5(void * handle, PurpleProxyInfo * gpi, char * host, int port, PurpleProxyConnectFunction connect_cb, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_proxy_connect_socks5(IntPtr handle, IntPtr gpi, string host, int port, UNKNOWN connect_cb, IntPtr data);

		public static PurpleProxyConnectData ConnectSocks5(IntPtr handle, PurpleProxyInfo gpi, string host, int port, PurpleProxyConnectFunction connect_cb, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_proxy_connect_cancel(PurpleProxyConnectData * connect_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_connect_cancel(IntPtr connect_data);

		public static void ConnectCancel(PurpleProxyConnectData connect_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_proxy_connect_cancel_with_handle(void * handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_connect_cancel_with_handle(IntPtr handle);

		public static void ConnectCancelWithHandle(IntPtr handle)
		{
			throw new NotImplementedException();
		}

	}
}


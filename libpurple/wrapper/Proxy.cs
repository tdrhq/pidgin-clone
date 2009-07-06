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
	public class Proxy
	{
		public enum PurpleProxyType
		{
			PURPLE_PROXY_USE_GLOBAL = -1,
			PURPLE_PROXY_NONE = 0,
			PURPLE_PROXY_HTTP,
			PURPLE_PROXY_SOCKS4,
			PURPLE_PROXY_SOCKS5,
			PURPLE_PROXY_USE_ENVVAR
		};

		/*
		 * PurpleProxyInfo * purple_proxy_info_new()
		 * 
		 * Could not generate a wrapper for purple_proxy_info_new in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * purple_proxy_info_new()).
		 */

		/*
		 * void purple_proxy_info_destroy(PurpleProxyInfo * info)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_destroy in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * void purple_proxy_info_set_type(PurpleProxyInfo * info, PurpleProxyType type)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_set_type in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * void purple_proxy_info_set_host(PurpleProxyInfo * info, char * host)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_set_host in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * void purple_proxy_info_set_port(PurpleProxyInfo * info, int port)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_set_port in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * void purple_proxy_info_set_username(PurpleProxyInfo * info, char * username)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_set_username in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * void purple_proxy_info_set_password(PurpleProxyInfo * info, char * password)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_set_password in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * PurpleProxyType purple_proxy_info_get_type(PurpleProxyInfo * info)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_get_type in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * char * purple_proxy_info_get_host(PurpleProxyInfo * info)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_get_host in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * int purple_proxy_info_get_port(PurpleProxyInfo * info)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_get_port in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * char * purple_proxy_info_get_username(PurpleProxyInfo * info)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_get_username in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * char * purple_proxy_info_get_password(PurpleProxyInfo * info)
		 * 
		 * Could not generate a wrapper for purple_proxy_info_get_password in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * PurpleProxyInfo * purple_global_proxy_get_info()
		 * 
		 * Could not generate a wrapper for purple_global_proxy_get_info in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * purple_global_proxy_get_info()).
		 */

		/*
		 * void purple_global_proxy_set_info(PurpleProxyInfo * info)
		 * 
		 * Could not generate a wrapper for purple_global_proxy_set_info in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * info).
		 */

		/*
		 * void * purple_proxy_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_proxy_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_proxy_get_handle();
		}

		/*
		 * void purple_proxy_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_init();

		public static void Init()
		{
			purple_proxy_init();
		}

		/*
		 * void purple_proxy_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_uninit();

		public static void Uninit()
		{
			purple_proxy_uninit();
		}

		/*
		 * PurpleProxyInfo * purple_proxy_get_setup(PurpleAccount * account)
		 * 
		 * Could not generate a wrapper for purple_proxy_get_setup in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyInfo * purple_proxy_get_setup(PurpleAccount * account)).
		 */

		/*
		 * void purple_proxy_connect_cancel(PurpleProxyConnectData * connect_data)
		 * 
		 * Could not generate a wrapper for purple_proxy_connect_cancel in file "proxy.h".
		 * Message: The type could not be resolved (PurpleProxyConnectData * connect_data).
		 */

		/*
		 * void purple_proxy_connect_cancel_with_handle(void * handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_connect_cancel_with_handle(IntPtr handle);

		public static void ConnectCancelWithHandle(IntPtr handle)
		{
			purple_proxy_connect_cancel_with_handle(handle);
		}

	}
}


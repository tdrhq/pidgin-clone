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
	 * File: proxy.h
	 */
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

		public delegate void PurpleProxyConnectFunction(IntPtr data, int source, string error_message);

		/*
		 * Function Name: purple_proxy_info_new
		 * Function Type: PurpleProxyInfo *
		 * Function RawType: PurpleProxyInfo
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleProxyInfo * purple_proxy_info_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_proxy_info_new();

		public static /* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr InfoNew()
		{
			/* Unable to process the return value of purple_proxy_info_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_proxy_info_destroy(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_destroy(IntPtr info);

		public static void InfoDestroy(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_set_type
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_proxy_info_set_type(PurpleProxyInfo * info, PurpleProxyType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_set_type(IntPtr info, Proxy.PurpleProxyType type);

		public static void InfoSetType(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info, Proxy.PurpleProxyType type)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_set_host
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_proxy_info_set_host(PurpleProxyInfo * info, char * host)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_set_host(IntPtr info, string host);

		public static void InfoSetHost(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info, string host)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_set_port
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_proxy_info_set_port(PurpleProxyInfo * info, int port)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_set_port(IntPtr info, int port);

		public static void InfoSetPort(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info, int port)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_set_username
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_proxy_info_set_username(PurpleProxyInfo * info, char * username)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_set_username(IntPtr info, string username);

		public static void InfoSetUsername(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info, string username)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_set_password
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_proxy_info_set_password(PurpleProxyInfo * info, char * password)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_info_set_password(IntPtr info, string password);

		public static void InfoSetPassword(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info, string password)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_get_type
		 * Function Type: PurpleProxyType
		 * Function RawType: PurpleProxyType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleProxyType purple_proxy_info_get_type(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern Proxy.PurpleProxyType purple_proxy_info_get_type(IntPtr info);

		public static Proxy.PurpleProxyType InfoGetType(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_get_host
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_proxy_info_get_host(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_proxy_info_get_host(IntPtr info);

		public static string InfoGetHost(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_get_port
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * int purple_proxy_info_get_port(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_proxy_info_get_port(IntPtr info);

		public static int InfoGetPort(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_get_username
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_proxy_info_get_username(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_proxy_info_get_username(IntPtr info);

		public static string InfoGetUsername(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_info_get_password
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_proxy_info_get_password(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_proxy_info_get_password(IntPtr info);

		public static string InfoGetPassword(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_global_proxy_get_info
		 * Function Type: PurpleProxyInfo *
		 * Function RawType: PurpleProxyInfo
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleProxyInfo * purple_global_proxy_get_info()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_global_proxy_get_info();

		public static /* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr GlobalProxyGetInfo()
		{
			/* Unable to process the return value of purple_global_proxy_get_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_global_proxy_set_info
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_global_proxy_set_info(PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_global_proxy_set_info(IntPtr info);

		public static void GlobalProxySetInfo(/* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
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
		 * Function Name: purple_proxy_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_proxy_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_proxy_get_setup
		 * Function Type: PurpleProxyInfo *
		 * Function RawType: PurpleProxyInfo
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleProxyInfo * purple_proxy_get_setup(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_proxy_get_setup(IntPtr account);

		public static /* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr GetSetup(PurpleAccount account)
		{
			/* Unable to process the return value of purple_proxy_get_setup, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_connect_cancel
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_proxy_connect_cancel(PurpleProxyConnectData * connect_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_proxy_connect_cancel(IntPtr connect_data);

		public static void ConnectCancel(/* Resolved as PointerToUnknownStruct to PurpleProxyConnectData */ IntPtr connect_data)
		{
			/* Unable to process connect_data, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_proxy_connect_cancel_with_handle
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
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


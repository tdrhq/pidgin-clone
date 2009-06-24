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
	public class Sslconn
	{
		/*
		 * gboolean purple_ssl_is_supported()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_ssl_is_supported();

		public static bool SslIsSupported()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSslConnection * purple_ssl_connect(PurpleAccount * account, char * host, int port, PurpleSslInputFunction func, PurpleSslErrorFunction error_func, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ssl_connect(IntPtr account, string host, int port, UNKNOWN func, UNKNOWN error_func, IntPtr data);

		public static PurpleSslConnection SslConnect(PurpleAccount account, string host, int port, PurpleSslInputFunction func, PurpleSslErrorFunction error_func, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSslConnection * purple_ssl_connect_fd(PurpleAccount * account, int fd, PurpleSslInputFunction func, PurpleSslErrorFunction error_func, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ssl_connect_fd(IntPtr account, int fd, UNKNOWN func, UNKNOWN error_func, IntPtr data);

		public static PurpleSslConnection SslConnectFd(PurpleAccount account, int fd, PurpleSslInputFunction func, PurpleSslErrorFunction error_func, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSslConnection * purple_ssl_connect_with_host_fd(PurpleAccount * account, int fd, PurpleSslInputFunction func, PurpleSslErrorFunction error_func, char * host, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ssl_connect_with_host_fd(IntPtr account, int fd, UNKNOWN func, UNKNOWN error_func, string host, IntPtr data);

		public static PurpleSslConnection SslConnectWithHostFd(PurpleAccount account, int fd, PurpleSslInputFunction func, PurpleSslErrorFunction error_func, string host, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_ssl_input_add(PurpleSslConnection * gsc, PurpleSslInputFunction func, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ssl_input_add(IntPtr gsc, UNKNOWN func, IntPtr data);

		public static void SslInputAdd(PurpleSslConnection gsc, PurpleSslInputFunction func, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_ssl_close(PurpleSslConnection * gsc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ssl_close(IntPtr gsc);

		public static void SslClose(PurpleSslConnection gsc)
		{
			throw new NotImplementedException();
		}

		/*
		 * size_t purple_ssl_read(PurpleSslConnection * gsc, void * buffer, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_ssl_read(IntPtr gsc, IntPtr buffer, UNKNOWN len);

		public static size_t SslRead(PurpleSslConnection gsc, IntPtr buffer, size_t len)
		{
			throw new NotImplementedException();
		}

		/*
		 * size_t purple_ssl_write(PurpleSslConnection * gsc, void * buffer, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_ssl_write(IntPtr gsc, IntPtr buffer, UNKNOWN len);

		public static size_t SslWrite(PurpleSslConnection gsc, IntPtr buffer, size_t len)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_ssl_set_ops(PurpleSslOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ssl_set_ops(IntPtr ops);

		public static void SslSetOps(PurpleSslOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSslOps * purple_ssl_get_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ssl_get_ops();

		public static PurpleSslOps SslGetOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_ssl_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ssl_init();

		public static void SslInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_ssl_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ssl_uninit();

		public static void SslUninit()
		{
			throw new NotImplementedException();
		}

	}
}


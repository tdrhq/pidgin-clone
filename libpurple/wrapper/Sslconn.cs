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
	public class Sslconn
	{
		public enum PurpleSslErrorType
		{
			PURPLE_SSL_HANDSHAKE_FAILED = 1,
			PURPLE_SSL_CONNECT_FAILED = 2,
			PURPLE_SSL_CERTIFICATE_INVALID = 3
		};

		/*
		 * gboolean purple_ssl_is_supported()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_ssl_is_supported();

		public static bool SslIsSupported()
		{
			return purple_ssl_is_supported();
		}

		/*
		 * gchar * purple_ssl_strerror(PurpleSslErrorType error)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_ssl_strerror(Sslconn.PurpleSslErrorType error);

		public static string SslStrerror(Sslconn.PurpleSslErrorType error)
		{
			/* Unable to process error, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * void purple_ssl_close(PurpleSslConnection * gsc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ssl_close(IntPtr gsc);

		public static void SslClose(PurpleSslConnection gsc)
		{
			purple_ssl_close(gsc.Reference);
		}

		/*
		 * size_t purple_ssl_read(PurpleSslConnection * gsc, void * buffer, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_ssl_read(IntPtr gsc, IntPtr buffer, ulong len);

		public static ulong SslRead(PurpleSslConnection gsc, IntPtr buffer, ulong len)
		{
			return purple_ssl_read(gsc.Reference, buffer, len);
		}

		/*
		 * size_t purple_ssl_write(PurpleSslConnection * gsc, void * buffer, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_ssl_write(IntPtr gsc, IntPtr buffer, ulong len);

		public static ulong SslWrite(PurpleSslConnection gsc, IntPtr buffer, ulong len)
		{
			return purple_ssl_write(gsc.Reference, buffer, len);
		}

		/*
		 * GList * purple_ssl_get_peer_certificates(PurpleSslConnection * gsc)
		 * 
		 * Could not generate a wrapper for purple_ssl_get_peer_certificates in file "sslconn.h".
		 * Message: The type could not be resolved (GList * purple_ssl_get_peer_certificates(PurpleSslConnection * gsc)).
		 */

		/*
		 * void purple_ssl_set_ops(PurpleSslOps * ops)
		 * 
		 * Could not generate a wrapper for purple_ssl_set_ops in file "sslconn.h".
		 * Message: The type could not be resolved (PurpleSslOps * ops).
		 */

		/*
		 * PurpleSslOps * purple_ssl_get_ops()
		 * 
		 * Could not generate a wrapper for purple_ssl_get_ops in file "sslconn.h".
		 * Message: The type could not be resolved (PurpleSslOps * purple_ssl_get_ops()).
		 */

		/*
		 * void purple_ssl_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ssl_init();

		public static void SslInit()
		{
			purple_ssl_init();
		}

		/*
		 * void purple_ssl_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ssl_uninit();

		public static void SslUninit()
		{
			purple_ssl_uninit();
		}

	}
}


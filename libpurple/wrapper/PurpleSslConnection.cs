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
	 * File: sslconn.h
	 * Structure: PurpleSslConnection
	 */
	public class PurpleSslConnection : UnmanagedWrapper<_PurpleSslConnection>
	{
		public PurpleSslConnection()
		{
		}

		public PurpleSslConnection(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleSslConnection)Marshal.PtrToStructure(this.Reference, typeof(_PurpleSslConnection));
		}

		/*
		 * Argument Name: host
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string host
		{
			get
			{
				return this.Data.host;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.host = value;
			}
		}

		/*
		 * Argument Name: port
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public int port
		{
			get
			{
				return this.Data.port;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.port = value;
			}
		}

		/*
		 * Argument Name: connect_cb_data
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr connect_cb_data
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		/*
		 * Argument Name: connect_cb
		 * Argument Type: PurpleSslInputFunction
		 * Argument Category: KnownFunctionPointer
		 */
		public Sslconn.PurpleSslInputFunction connect_cb
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		/*
		 * Argument Name: error_cb
		 * Argument Type: PurpleSslErrorFunction
		 * Argument Category: KnownFunctionPointer
		 */
		public Sslconn.PurpleSslErrorFunction error_cb
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		/*
		 * Argument Name: recv_cb_data
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr recv_cb_data
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		/*
		 * Argument Name: recv_cb
		 * Argument Type: PurpleSslInputFunction
		 * Argument Category: KnownFunctionPointer
		 */
		public Sslconn.PurpleSslInputFunction recv_cb
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		/*
		 * Argument Name: fd
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public int fd
		{
			get
			{
				return this.Data.fd;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.fd = value;
			}
		}

		/*
		 * Argument Name: inpa
		 * Argument Type: guint
		 * Argument Category: Native
		 */
		public uint inpa
		{
			get
			{
				return this.Data.inpa;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.inpa = value;
			}
		}

		/*
		 * Argument Name: connect_data
		 * Argument Type: PurpleProxyConnectData *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to PurpleProxyConnectData */ IntPtr connect_data
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		/*
		 * Argument Name: private_data
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr private_data
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		/*
		 * Argument Name: verifier
		 * Argument Type: PurpleCertificateVerifier *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleCertificateVerifier verifier
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurpleSslConnection
	{
		/*
		 * char * host
		 */
		public string host;

		/*
		 * int port
		 */
		public int port;

		/*
		 * void * connect_cb_data
		 */
		public IntPtr connect_cb_data;

		/*
		 * PurpleSslInputFunction connect_cb
		 */
		public IntPtr connect_cb;

		/*
		 * PurpleSslErrorFunction error_cb
		 */
		public IntPtr error_cb;

		/*
		 * void * recv_cb_data
		 */
		public IntPtr recv_cb_data;

		/*
		 * PurpleSslInputFunction recv_cb
		 */
		public IntPtr recv_cb;

		/*
		 * int fd
		 */
		public int fd;

		/*
		 * guint inpa
		 */
		public uint inpa;

		/*
		 * PurpleProxyConnectData * connect_data
		 */
		public IntPtr connect_data;

		/*
		 * void * private_data
		 */
		public IntPtr private_data;

		/*
		 * PurpleCertificateVerifier * verifier
		 */
		public IntPtr verifier;

	}

}


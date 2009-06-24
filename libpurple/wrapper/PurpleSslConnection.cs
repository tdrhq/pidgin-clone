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
	public class PurpleSslConnection : UnmanagedWrapper<_PurpleSslConnection>
	{
		public PurpleSslConnection()
		{
		}

		public PurpleSslConnection(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleSslConnection)Marshal.PtrToStructure(this.Reference, typeof(_PurpleSslConnection));
		}

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

		public PurpleSslInputFunction connect_cb
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

		public PurpleSslErrorFunction error_cb
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

		public PurpleSslInputFunction recv_cb
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

		public PurpleProxyConnectData connect_data
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
	class _PurpleSslConnection
	{
		/*
		 * char * host
		 */
		string host;

		/*
		 * int port
		 */
		int port;

		/*
		 * void * connect_cb_data
		 */
		IntPtr connect_cb_data;

		/*
		 * PurpleSslInputFunction connect_cb
		 */
		UNKNOWN connect_cb;

		/*
		 * PurpleSslErrorFunction error_cb
		 */
		UNKNOWN error_cb;

		/*
		 * void * recv_cb_data
		 */
		IntPtr recv_cb_data;

		/*
		 * PurpleSslInputFunction recv_cb
		 */
		UNKNOWN recv_cb;

		/*
		 * int fd
		 */
		int fd;

		/*
		 * guint inpa
		 */
		uint inpa;

		/*
		 * PurpleProxyConnectData * connect_data
		 */
		IntPtr connect_data;

		/*
		 * void * private_data
		 */
		IntPtr private_data;

		/*
		 * PurpleCertificateVerifier * verifier
		 */
		IntPtr verifier;

	}
}


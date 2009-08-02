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
	 * File: certificate.h
	 * Structure: PurpleCertificateVerifier
	 */
	public class PurpleCertificateVerifier : UnmanagedWrapper<_PurpleCertificateVerifier>
	{
		public PurpleCertificateVerifier()
		{
		}

		public PurpleCertificateVerifier(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleCertificateVerifier)Marshal.PtrToStructure(this.Reference, typeof(_PurpleCertificateVerifier));
		}

		/*
		 * Argument Name: scheme_name
		 * Argument Type: gchar *
		 * Argument Category: Native
		 */
		public string scheme_name
		{
			get
			{
				return this.Data.scheme_name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.scheme_name = value;
			}
		}

		/*
		 * Argument Name: name
		 * Argument Type: gchar *
		 * Argument Category: Native
		 */
		public string name
		{
			get
			{
				return this.Data.name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.name = value;
			}
		}

		/*
		 * Argument Name: start_verification
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr start_verification
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
		 * Argument Name: destroy_request
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr destroy_request
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
		 * Argument Name: _purple_reserved1
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr _purple_reserved1
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
		 * Argument Name: _purple_reserved2
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr _purple_reserved2
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
		 * Argument Name: _purple_reserved3
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr _purple_reserved3
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
		 * Argument Name: _purple_reserved4
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr _purple_reserved4
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
	public class _PurpleCertificateVerifier
	{
		/*
		 * gchar * scheme_name
		 */
		public string scheme_name;

		/*
		 * gchar * name
		 */
		public string name;

		/*
		 * void (*start_verification)(PurpleCertificateVerificationRequest * vrq)
		 */
		public IntPtr start_verification;

		/*
		 * void (*destroy_request)(PurpleCertificateVerificationRequest * vrq)
		 */
		public IntPtr destroy_request;

		/*
		 * void (*_purple_reserved1)()
		 */
		public IntPtr _purple_reserved1;

		/*
		 * void (*_purple_reserved2)()
		 */
		public IntPtr _purple_reserved2;

		/*
		 * void (*_purple_reserved3)()
		 */
		public IntPtr _purple_reserved3;

		/*
		 * void (*_purple_reserved4)()
		 */
		public IntPtr _purple_reserved4;

	}

}


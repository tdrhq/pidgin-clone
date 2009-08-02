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
	 * Structure: PurpleCertificateVerificationRequest
	 */
	public class PurpleCertificateVerificationRequest : UnmanagedWrapper<_PurpleCertificateVerificationRequest>
	{
		public PurpleCertificateVerificationRequest()
		{
		}

		public PurpleCertificateVerificationRequest(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleCertificateVerificationRequest)Marshal.PtrToStructure(this.Reference, typeof(_PurpleCertificateVerificationRequest));
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

		/*
		 * Argument Name: scheme
		 * Argument Type: PurpleCertificateScheme *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleCertificateScheme scheme
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
		 * Argument Name: subject_name
		 * Argument Type: gchar *
		 * Argument Category: Native
		 */
		public string subject_name
		{
			get
			{
				return this.Data.subject_name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.subject_name = value;
			}
		}

		/*
		 * Argument Name: cert_chain
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurpleCertificateVerificationRequest.cert_chain in file "certificate.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Argument Name: data
		 * Argument Type: gpointer
		 * Argument Category: VoidPointer
		 */
		public IntPtr data
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
		 * Argument Name: cb
		 * Argument Type: PurpleCertificateVerifiedCallback
		 * Argument Category: KnownFunctionPointer
		 */
		public Certificate.PurpleCertificateVerifiedCallback cb
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
		 * Argument Name: cb_data
		 * Argument Type: gpointer
		 * Argument Category: VoidPointer
		 */
		public IntPtr cb_data
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
	public class _PurpleCertificateVerificationRequest
	{
		/*
		 * PurpleCertificateVerifier * verifier
		 */
		public IntPtr verifier;

		/*
		 * PurpleCertificateScheme * scheme
		 */
		public IntPtr scheme;

		/*
		 * gchar * subject_name
		 */
		public string subject_name;

		/*
		 * GList * cert_chain
		 */
		public IntPtr cert_chain;

		/*
		 * gpointer data
		 */
		public IntPtr data;

		/*
		 * PurpleCertificateVerifiedCallback cb
		 */
		public IntPtr cb;

		/*
		 * gpointer cb_data
		 */
		public IntPtr cb_data;

	}

}


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
	 * Structure: PurpleCertificateScheme
	 */
	public class PurpleCertificateScheme : UnmanagedWrapper<_PurpleCertificateScheme>
	{
		public PurpleCertificateScheme()
		{
		}

		public PurpleCertificateScheme(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleCertificateScheme)Marshal.PtrToStructure(this.Reference, typeof(_PurpleCertificateScheme));
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
		 * Argument Name: fullname
		 * Argument Type: gchar *
		 * Argument Category: Native
		 */
		public string fullname
		{
			get
			{
				return this.Data.fullname;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.fullname = value;
			}
		}

		/*
		 * Argument Name: import_certificate
		 * Argument Type: PurpleCertificate *
		 * Argument Category: PointerToKnownStruct
		 */
		public IntPtr import_certificate
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
		 * Argument Name: export_certificate
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr export_certificate
		{
			get
			{
				return this.Data.export_certificate;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.export_certificate = value;
			}
		}

		/*
		 * Argument Name: copy_certificate
		 * Argument Type: PurpleCertificate *
		 * Argument Category: PointerToKnownStruct
		 */
		public IntPtr copy_certificate
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
		 * Argument Name: destroy_certificate
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr destroy_certificate
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
		 * Argument Name: signed_by
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr signed_by
		{
			get
			{
				return this.Data.signed_by;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.signed_by = value;
			}
		}

		/*
		 * Argument Name: get_fingerprint_sha1
		 * Argument Type: GByteArray *
		 * Argument Category: PointerToUnknownStruct
		 */
		public IntPtr get_fingerprint_sha1
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
		 * Argument Name: get_unique_id
		 * Argument Type: gchar *
		 * Argument Category: Native
		 */
		public IntPtr get_unique_id
		{
			get
			{
				return this.Data.get_unique_id;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.get_unique_id = value;
			}
		}

		/*
		 * Argument Name: get_issuer_unique_id
		 * Argument Type: gchar *
		 * Argument Category: Native
		 */
		public IntPtr get_issuer_unique_id
		{
			get
			{
				return this.Data.get_issuer_unique_id;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.get_issuer_unique_id = value;
			}
		}

		/*
		 * Argument Name: get_subject_name
		 * Argument Type: gchar *
		 * Argument Category: Native
		 */
		public IntPtr get_subject_name
		{
			get
			{
				return this.Data.get_subject_name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.get_subject_name = value;
			}
		}

		/*
		 * Argument Name: check_subject_name
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr check_subject_name
		{
			get
			{
				return this.Data.check_subject_name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.check_subject_name = value;
			}
		}

		/*
		 * Argument Name: get_times
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr get_times
		{
			get
			{
				return this.Data.get_times;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.get_times = value;
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
	public class _PurpleCertificateScheme
	{
		/*
		 * gchar * name
		 */
		public string name;

		/*
		 * gchar * fullname
		 */
		public string fullname;

		/*
		 * PurpleCertificate * (*import_certificate)(gchar * filename)
		 */
		public IntPtr import_certificate;

		/*
		 * gboolean (*export_certificate)(gchar * filename, PurpleCertificate * crt)
		 */
		public IntPtr export_certificate;

		/*
		 * PurpleCertificate * (*copy_certificate)(PurpleCertificate * crt)
		 */
		public IntPtr copy_certificate;

		/*
		 * void (*destroy_certificate)(PurpleCertificate * crt)
		 */
		public IntPtr destroy_certificate;

		/*
		 * gboolean (*signed_by)(PurpleCertificate * crt, PurpleCertificate * issuer)
		 */
		public IntPtr signed_by;

		/*
		 * GByteArray * (*get_fingerprint_sha1)(PurpleCertificate * crt)
		 */
		public IntPtr get_fingerprint_sha1;

		/*
		 * gchar * (*get_unique_id)(PurpleCertificate * crt)
		 */
		public IntPtr get_unique_id;

		/*
		 * gchar * (*get_issuer_unique_id)(PurpleCertificate * crt)
		 */
		public IntPtr get_issuer_unique_id;

		/*
		 * gchar * (*get_subject_name)(PurpleCertificate * crt)
		 */
		public IntPtr get_subject_name;

		/*
		 * gboolean (*check_subject_name)(PurpleCertificate * crt, gchar * name)
		 */
		public IntPtr check_subject_name;

		/*
		 * gboolean (*get_times)(PurpleCertificate * crt, time_t * activation, time_t * expiration)
		 */
		public IntPtr get_times;

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


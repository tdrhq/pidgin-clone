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
	public class PurpleCertificateScheme : UnmanagedWrapper<_PurpleCertificateScheme>
	{
		public PurpleCertificateScheme()
		{
		}

		public PurpleCertificateScheme(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleCertificateScheme)Marshal.PtrToStructure(this.Reference, typeof(_PurpleCertificateScheme));
		}

		public string  name
		{
			get
			{
				return this.Data. name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data. name = value;
			}
		}

		public string  fullname
		{
			get
			{
				return this.Data. fullname;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data. fullname = value;
			}
		}

		public PurpleCertificate  (* import_certificate)(const gchar * filename)
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

		public bool (* export_certificate)(const gchar *filename, PurpleCertificate *crt)
		{
			get
			{
				return this.Data.(* export_certificate)(const gchar *filename, PurpleCertificate *crt);
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.(* export_certificate)(const gchar *filename, PurpleCertificate *crt) = value;
			}
		}

		public PurpleCertificate  (* copy_certificate)(PurpleCertificate *crt)
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

		public void (* destroy_certificate)(PurpleCertificate * crt)
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

		public bool signed_by
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

		public GByteArray  (* get_fingerprint_sha1)(PurpleCertificate *crt)
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

		public string  (* get_unique_id)(PurpleCertificate *crt)
		{
			get
			{
				return this.Data. (* get_unique_id)(PurpleCertificate *crt);
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data. (* get_unique_id)(PurpleCertificate *crt) = value;
			}
		}

		public string  (* get_issuer_unique_id)(PurpleCertificate *crt)
		{
			get
			{
				return this.Data. (* get_issuer_unique_id)(PurpleCertificate *crt);
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data. (* get_issuer_unique_id)(PurpleCertificate *crt) = value;
			}
		}

		public string  (* get_subject_name)(PurpleCertificate *crt)
		{
			get
			{
				return this.Data. (* get_subject_name)(PurpleCertificate *crt);
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data. (* get_subject_name)(PurpleCertificate *crt) = value;
			}
		}

		public bool (* check_subject_name)(PurpleCertificate *crt, const gchar *name)
		{
			get
			{
				return this.Data.(* check_subject_name)(PurpleCertificate *crt, const gchar *name);
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.(* check_subject_name)(PurpleCertificate *crt, const gchar *name) = value;
			}
		}

		public bool (* get_times)(PurpleCertificate *crt, time_t *activation, time_t *expiration)
		{
			get
			{
				return this.Data.(* get_times)(PurpleCertificate *crt, time_t *activation, time_t *expiration);
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.(* get_times)(PurpleCertificate *crt, time_t *activation, time_t *expiration) = value;
			}
		}

		public void _purple_reserved1
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

		public void _purple_reserved2
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

		public void _purple_reserved3
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

		public void _purple_reserved4
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
	class _PurpleCertificateScheme
	{
		/*
		 * gchar *  name
		 */
		string  name;

		/*
		 * gchar *  fullname
		 */
		string  fullname;

		/*
		 * PurpleCertificate *  (* import_certificate)(const gchar * filename)
		 */
		IntPtr  (* import_certificate)(const gchar * filename);

		/*
		 * gboolean (* export_certificate)(const gchar *filename, PurpleCertificate *crt)
		 */
		bool (* export_certificate)(const gchar *filename, PurpleCertificate *crt);

		/*
		 * PurpleCertificate *  (* copy_certificate)(PurpleCertificate *crt)
		 */
		IntPtr  (* copy_certificate)(PurpleCertificate *crt);

		/*
		 * void (* destroy_certificate)(PurpleCertificate * crt)
		 */
		void (* destroy_certificate)(PurpleCertificate * crt);

		/*
		 * gboolean (*signed_by)(PurpleCertificate * crt, PurpleCertificate * issuer)
		 */
		bool signed_by;

		/*
		 * GByteArray *  (* get_fingerprint_sha1)(PurpleCertificate *crt)
		 */
		IntPtr  (* get_fingerprint_sha1)(PurpleCertificate *crt);

		/*
		 * gchar *  (* get_unique_id)(PurpleCertificate *crt)
		 */
		string  (* get_unique_id)(PurpleCertificate *crt);

		/*
		 * gchar *  (* get_issuer_unique_id)(PurpleCertificate *crt)
		 */
		string  (* get_issuer_unique_id)(PurpleCertificate *crt);

		/*
		 * gchar *  (* get_subject_name)(PurpleCertificate *crt)
		 */
		string  (* get_subject_name)(PurpleCertificate *crt);

		/*
		 * gboolean (* check_subject_name)(PurpleCertificate *crt, const gchar *name)
		 */
		bool (* check_subject_name)(PurpleCertificate *crt, const gchar *name);

		/*
		 * gboolean (* get_times)(PurpleCertificate *crt, time_t *activation, time_t *expiration)
		 */
		bool (* get_times)(PurpleCertificate *crt, time_t *activation, time_t *expiration);

		/*
		 * void (*_purple_reserved1)()
		 */
		void _purple_reserved1;

		/*
		 * void (*_purple_reserved2)()
		 */
		void _purple_reserved2;

		/*
		 * void (*_purple_reserved3)()
		 */
		void _purple_reserved3;

		/*
		 * void (*_purple_reserved4)()
		 */
		void _purple_reserved4;

	}
}


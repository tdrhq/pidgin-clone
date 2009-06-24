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
	public class PurpleCertificatePool : UnmanagedWrapper<_PurpleCertificatePool>
	{
		public PurpleCertificatePool()
		{
		}

		public PurpleCertificatePool(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleCertificatePool)Marshal.PtrToStructure(this.Reference, typeof(_PurpleCertificatePool));
		}

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

		public bool (* init)(void)
		{
			get
			{
				return this.Data.(* init)(void);
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.(* init)(void) = value;
			}
		}

		public void (* uninit)(void)
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

		public bool (* cert_in_pool)(const gchar *id)
		{
			get
			{
				return this.Data.(* cert_in_pool)(const gchar *id);
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.(* cert_in_pool)(const gchar *id) = value;
			}
		}

		public PurpleCertificate  (* get_cert)(const gchar *id)
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

		public bool (* put_cert)(const gchar *id, PurpleCertificate *crt)
		{
			get
			{
				return this.Data.(* put_cert)(const gchar *id, PurpleCertificate *crt);
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.(* put_cert)(const gchar *id, PurpleCertificate *crt) = value;
			}
		}

		public bool (* delete_cert)(const gchar *id)
		{
			get
			{
				return this.Data.(* delete_cert)(const gchar *id);
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.(* delete_cert)(const gchar *id) = value;
			}
		}

		public GList  (* get_idlist)(void)
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
	class _PurpleCertificatePool
	{
		/*
		 * gchar * scheme_name
		 */
		string scheme_name;

		/*
		 * gchar * name
		 */
		string name;

		/*
		 * gchar * fullname
		 */
		string fullname;

		/*
		 * gpointer data
		 */
		IntPtr data;

		/*
		 * gboolean (* init)(void)
		 */
		bool (* init)(void);

		/*
		 * void (* uninit)(void)
		 */
		void (* uninit)(void);

		/*
		 * gboolean (* cert_in_pool)(const gchar *id)
		 */
		bool (* cert_in_pool)(const gchar *id);

		/*
		 * PurpleCertificate *  (* get_cert)(const gchar *id)
		 */
		IntPtr  (* get_cert)(const gchar *id);

		/*
		 * gboolean (* put_cert)(const gchar *id, PurpleCertificate *crt)
		 */
		bool (* put_cert)(const gchar *id, PurpleCertificate *crt);

		/*
		 * gboolean (* delete_cert)(const gchar *id)
		 */
		bool (* delete_cert)(const gchar *id);

		/*
		 * GList *  (* get_idlist)(void)
		 */
		IntPtr  (* get_idlist)(void);

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


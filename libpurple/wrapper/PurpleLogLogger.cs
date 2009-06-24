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
	public class PurpleLogLogger : UnmanagedWrapper<_PurpleLogLogger>
	{
		public PurpleLogLogger()
		{
		}

		public PurpleLogLogger(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleLogLogger)Marshal.PtrToStructure(this.Reference, typeof(_PurpleLogLogger));
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

		public string id
		{
			get
			{
				return this.Data.id;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.id = value;
			}
		}

		public void create
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

		public gsize write
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

		public void finalize
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

		public GList list
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

		public string read
		{
			get
			{
				return this.Data.read;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.read = value;
			}
		}

		public int size
		{
			get
			{
				return this.Data.size;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.size = value;
			}
		}

		public int total_size
		{
			get
			{
				return this.Data.total_size;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.total_size = value;
			}
		}

		public GList list_syslog
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

		public void get_log_sets
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

		public bool remove
		{
			get
			{
				return this.Data.remove;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.remove = value;
			}
		}

		public bool is_deletable
		{
			get
			{
				return this.Data.is_deletable;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.is_deletable = value;
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
	class _PurpleLogLogger
	{
		/*
		 * char * name
		 */
		string name;

		/*
		 * char * id
		 */
		string id;

		/*
		 * void (*create)(PurpleLog * log)
		 */
		void create;

		/*
		 * gsize (*write)(PurpleLog * log, PurpleMessageFlags type, char * from, time_t time, char * message)
		 */
		UNKNOWN write;

		/*
		 * void (*finalize)(PurpleLog * log)
		 */
		void finalize;

		/*
		 * GList * (*list)(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		IntPtr list;

		/*
		 * char * (*read)(PurpleLog * log, PurpleLogReadFlags * flags)
		 */
		string read;

		/*
		 * int (*size)(PurpleLog * log)
		 */
		int size;

		/*
		 * int (*total_size)(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		int total_size;

		/*
		 * GList * (*list_syslog)(PurpleAccount * account)
		 */
		IntPtr list_syslog;

		/*
		 * void (*get_log_sets)(PurpleLogSetCallback cb, GHashTable * sets)
		 */
		void get_log_sets;

		/*
		 * gboolean (*remove)(PurpleLog * log)
		 */
		bool remove;

		/*
		 * gboolean (*is_deletable)(PurpleLog * log)
		 */
		bool is_deletable;

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


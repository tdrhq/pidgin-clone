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

		public IntPtr create
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

		public IntPtr write
		{
			get
			{
				return this.Data.write;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.write = value;
			}
		}

		public IntPtr finalize
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

		public IntPtr list
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

		public IntPtr read
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

		public IntPtr size
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

		public IntPtr total_size
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

		public IntPtr list_syslog
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

		public IntPtr get_log_sets
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

		public IntPtr remove
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

		public IntPtr is_deletable
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
	class _PurpleLogLogger
	{
		/*
		 * char * name
		 */
		IntPtr name;

		/*
		 * char * id
		 */
		IntPtr id;

		/*
		 * void (*create)(PurpleLog * log)
		 */
		IntPtr create;

		/*
		 * gsize (*write)(PurpleLog * log, PurpleMessageFlags type, char * from, time_t time, char * message)
		 */
		IntPtr write;

		/*
		 * void (*finalize)(PurpleLog * log)
		 */
		IntPtr finalize;

		/*
		 * GList * (*list)(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		IntPtr list;

		/*
		 * char * (*read)(PurpleLog * log, PurpleLogReadFlags * flags)
		 */
		IntPtr read;

		/*
		 * int (*size)(PurpleLog * log)
		 */
		IntPtr size;

		/*
		 * int (*total_size)(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		IntPtr total_size;

		/*
		 * GList * (*list_syslog)(PurpleAccount * account)
		 */
		IntPtr list_syslog;

		/*
		 * void (*get_log_sets)(PurpleLogSetCallback cb, GHashTable * sets)
		 */
		IntPtr get_log_sets;

		/*
		 * gboolean (*remove)(PurpleLog * log)
		 */
		IntPtr remove;

		/*
		 * gboolean (*is_deletable)(PurpleLog * log)
		 */
		IntPtr is_deletable;

		/*
		 * void (*_purple_reserved1)()
		 */
		IntPtr _purple_reserved1;

		/*
		 * void (*_purple_reserved2)()
		 */
		IntPtr _purple_reserved2;

		/*
		 * void (*_purple_reserved3)()
		 */
		IntPtr _purple_reserved3;

		/*
		 * void (*_purple_reserved4)()
		 */
		IntPtr _purple_reserved4;

	}

}


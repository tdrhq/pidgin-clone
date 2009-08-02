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
	 * File: log.h
	 * Structure: PurpleLogLogger
	 */
	public class PurpleLogLogger : UnmanagedWrapper<_PurpleLogLogger>
	{
		public PurpleLogLogger()
		{
		}

		public PurpleLogLogger(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleLogLogger)Marshal.PtrToStructure(this.Reference, typeof(_PurpleLogLogger));
		}

		/*
		 * Argument Name: name
		 * Argument Type: char *
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
		 * Argument Name: id
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: create
		 * Argument Type: void
		 * Argument Category: Void
		 */
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

		/*
		 * Argument Name: write
		 * Argument Type: gsize
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: finalize
		 * Argument Type: void
		 * Argument Category: Void
		 */
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

		/*
		 * Argument Name: list
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
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

		/*
		 * Argument Name: read
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: size
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: total_size
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: list_syslog
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
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

		/*
		 * Argument Name: get_log_sets
		 * Argument Type: void
		 * Argument Category: Void
		 */
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

		/*
		 * Argument Name: remove
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: is_deletable
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
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
	public class _PurpleLogLogger
	{
		/*
		 * char * name
		 */
		public string name;

		/*
		 * char * id
		 */
		public string id;

		/*
		 * void (*create)(PurpleLog * log)
		 */
		public IntPtr create;

		/*
		 * gsize (*write)(PurpleLog * log, PurpleMessageFlags type, char * from, time_t time, char * message)
		 */
		public IntPtr write;

		/*
		 * void (*finalize)(PurpleLog * log)
		 */
		public IntPtr finalize;

		/*
		 * GList * (*list)(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		public IntPtr list;

		/*
		 * char * (*read)(PurpleLog * log, PurpleLogReadFlags * flags)
		 */
		public IntPtr read;

		/*
		 * int (*size)(PurpleLog * log)
		 */
		public IntPtr size;

		/*
		 * int (*total_size)(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		public IntPtr total_size;

		/*
		 * GList * (*list_syslog)(PurpleAccount * account)
		 */
		public IntPtr list_syslog;

		/*
		 * void (*get_log_sets)(PurpleLogSetCallback cb, GHashTable * sets)
		 */
		public IntPtr get_log_sets;

		/*
		 * gboolean (*remove)(PurpleLog * log)
		 */
		public IntPtr remove;

		/*
		 * gboolean (*is_deletable)(PurpleLog * log)
		 */
		public IntPtr is_deletable;

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


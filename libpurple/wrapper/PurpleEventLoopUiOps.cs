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
	 * File: eventloop.h
	 * Structure: PurpleEventLoopUiOps
	 */
	public class PurpleEventLoopUiOps : UnmanagedWrapper<_PurpleEventLoopUiOps>
	{
		public PurpleEventLoopUiOps()
		{
		}

		public PurpleEventLoopUiOps(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleEventLoopUiOps)Marshal.PtrToStructure(this.Reference, typeof(_PurpleEventLoopUiOps));
		}

		/*
		 * Argument Name: timeout_add
		 * Argument Type: guint
		 * Argument Category: Native
		 */
		public IntPtr timeout_add
		{
			get
			{
				return this.Data.timeout_add;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.timeout_add = value;
			}
		}

		/*
		 * Argument Name: timeout_remove
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr timeout_remove
		{
			get
			{
				return this.Data.timeout_remove;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.timeout_remove = value;
			}
		}

		/*
		 * Argument Name: input_add
		 * Argument Type: guint
		 * Argument Category: Native
		 */
		public IntPtr input_add
		{
			get
			{
				return this.Data.input_add;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.input_add = value;
			}
		}

		/*
		 * Argument Name: input_remove
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr input_remove
		{
			get
			{
				return this.Data.input_remove;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.input_remove = value;
			}
		}

		/*
		 * Argument Name: input_get_error
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public IntPtr input_get_error
		{
			get
			{
				return this.Data.input_get_error;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.input_get_error = value;
			}
		}

		/*
		 * Argument Name: timeout_add_seconds
		 * Argument Type: guint
		 * Argument Category: Native
		 */
		public IntPtr timeout_add_seconds
		{
			get
			{
				return this.Data.timeout_add_seconds;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.timeout_add_seconds = value;
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
	public class _PurpleEventLoopUiOps
	{
		/*
		 * guint (*timeout_add)(guint interval, GSourceFunc function, gpointer data)
		 */
		public IntPtr timeout_add;

		/*
		 * gboolean (*timeout_remove)(guint handle)
		 */
		public IntPtr timeout_remove;

		/*
		 * guint (*input_add)(int fd, PurpleInputCondition cond, PurpleInputFunction func, gpointer user_data)
		 */
		public IntPtr input_add;

		/*
		 * gboolean (*input_remove)(guint handle)
		 */
		public IntPtr input_remove;

		/*
		 * int (*input_get_error)(int fd, int * error)
		 */
		public IntPtr input_get_error;

		/*
		 * guint (*timeout_add_seconds)(guint interval, GSourceFunc function, gpointer data)
		 */
		public IntPtr timeout_add_seconds;

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


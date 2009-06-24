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
	public class PurpleEventLoopUiOps : UnmanagedWrapper<_PurpleEventLoopUiOps>
	{
		public PurpleEventLoopUiOps()
		{
		}

		public PurpleEventLoopUiOps(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleEventLoopUiOps)Marshal.PtrToStructure(this.Reference, typeof(_PurpleEventLoopUiOps));
		}

		public uint timeout_add
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

		public bool timeout_remove
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

		public uint input_add
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

		public bool input_remove
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

		public int input_get_error
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

		public uint timeout_add_seconds
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
	class _PurpleEventLoopUiOps
	{
		/*
		 * guint (*timeout_add)(guint interval, GSourceFunc function, gpointer data)
		 */
		uint timeout_add;

		/*
		 * gboolean (*timeout_remove)(guint handle)
		 */
		bool timeout_remove;

		/*
		 * guint (*input_add)(int fd, PurpleInputCondition cond, PurpleInputFunction func, gpointer user_data)
		 */
		uint input_add;

		/*
		 * gboolean (*input_remove)(guint handle)
		 */
		bool input_remove;

		/*
		 * int (*input_get_error)(int fd, int * error)
		 */
		int input_get_error;

		/*
		 * guint (*timeout_add_seconds)(guint interval, GSourceFunc function, gpointer data)
		 */
		uint timeout_add_seconds;

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


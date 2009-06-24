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
	public class PurpleBuddyIconSpec : UnmanagedWrapper<_PurpleBuddyIconSpec>
	{
		public PurpleBuddyIconSpec()
		{
		}

		public PurpleBuddyIconSpec(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleBuddyIconSpec)Marshal.PtrToStructure(this.Reference, typeof(_PurpleBuddyIconSpec));
		}

		public string format
		{
			get
			{
				return this.Data.format;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.format = value;
			}
		}

		public int min_width
		{
			get
			{
				return this.Data.min_width;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.min_width = value;
			}
		}

		public int min_height
		{
			get
			{
				return this.Data.min_height;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.min_height = value;
			}
		}

		public int max_width
		{
			get
			{
				return this.Data.max_width;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.max_width = value;
			}
		}

		public int max_height
		{
			get
			{
				return this.Data.max_height;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.max_height = value;
			}
		}

		public size_t max_filesize
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

		public PurpleIconScaleRules scale_rules
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
	class _PurpleBuddyIconSpec
	{
		/*
		 * char * format
		 */
		string format;

		/*
		 * int min_width
		 */
		int min_width;

		/*
		 * int min_height
		 */
		int min_height;

		/*
		 * int max_width
		 */
		int max_width;

		/*
		 * int max_height
		 */
		int max_height;

		/*
		 * size_t max_filesize
		 */
		UNKNOWN max_filesize;

		/*
		 * PurpleIconScaleRules scale_rules
		 */
		UNKNOWN scale_rules;

	}
}


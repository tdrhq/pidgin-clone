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
	 * File: prpl.h
	 * Structure: PurpleBuddyIconSpec
	 */
	public class PurpleBuddyIconSpec : UnmanagedWrapper<_PurpleBuddyIconSpec>
	{
		public PurpleBuddyIconSpec()
		{
		}

		public PurpleBuddyIconSpec(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleBuddyIconSpec)Marshal.PtrToStructure(this.Reference, typeof(_PurpleBuddyIconSpec));
		}

		/*
		 * Argument Name: format
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: min_width
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: min_height
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: max_width
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: max_height
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: max_filesize
		 * Argument Type: size_t
		 * Argument Category: Native
		 */
		public ulong max_filesize
		{
			get
			{
				return this.Data.max_filesize;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.max_filesize = value;
			}
		}

		/*
		 * Argument Name: scale_rules
		 * Argument Type: PurpleIconScaleRules
		 * Argument Category: KnownEnum
		 */
		public Prpl.PurpleIconScaleRules scale_rules
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
	public class _PurpleBuddyIconSpec
	{
		/*
		 * char * format
		 */
		public string format;

		/*
		 * int min_width
		 */
		public int min_width;

		/*
		 * int min_height
		 */
		public int min_height;

		/*
		 * int max_width
		 */
		public int max_width;

		/*
		 * int max_height
		 */
		public int max_height;

		/*
		 * size_t max_filesize
		 */
		public ulong max_filesize;

		/*
		 * PurpleIconScaleRules scale_rules
		 */
		public Prpl.PurpleIconScaleRules scale_rules;

	}

}


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
	 * Structure: proto_chat_entry
	 */
	public class proto_chat_entry : UnmanagedWrapper<_proto_chat_entry>
	{
		public proto_chat_entry()
		{
		}

		public proto_chat_entry(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_proto_chat_entry)Marshal.PtrToStructure(this.Reference, typeof(_proto_chat_entry));
		}

		/*
		 * Argument Name: label
		 * Argument Type: const char *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to char */ IntPtr label
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
		 * Argument Name: identifier
		 * Argument Type: const char *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to char */ IntPtr identifier
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
		 * Argument Name: required
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool required
		{
			get
			{
				return this.Data.required;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.required = value;
			}
		}

		/*
		 * Argument Name: is_int
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool is_int
		{
			get
			{
				return this.Data.is_int;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.is_int = value;
			}
		}

		/*
		 * Argument Name: min
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public int min
		{
			get
			{
				return this.Data.min;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.min = value;
			}
		}

		/*
		 * Argument Name: max
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public int max
		{
			get
			{
				return this.Data.max;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.max = value;
			}
		}

		/*
		 * Argument Name: secret
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool secret
		{
			get
			{
				return this.Data.secret;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.secret = value;
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _proto_chat_entry
	{
		/*
		 * const char * label
		 */
		public IntPtr label;

		/*
		 * const char * identifier
		 */
		public IntPtr identifier;

		/*
		 * gboolean required
		 */
		public bool required;

		/*
		 * gboolean is_int
		 */
		public bool is_int;

		/*
		 * int min
		 */
		public int min;

		/*
		 * int max
		 */
		public int max;

		/*
		 * gboolean secret
		 */
		public bool secret;

	}

}


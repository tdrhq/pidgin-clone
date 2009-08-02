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
	 * File: whiteboard.h
	 * Structure: PurpleWhiteboard
	 */
	public class PurpleWhiteboard : UnmanagedWrapper<_PurpleWhiteboard>
	{
		public PurpleWhiteboard()
		{
		}

		public PurpleWhiteboard(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleWhiteboard)Marshal.PtrToStructure(this.Reference, typeof(_PurpleWhiteboard));
		}

		/*
		 * Argument Name: state
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public int state
		{
			get
			{
				return this.Data.state;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.state = value;
			}
		}

		/*
		 * Argument Name: account
		 * Argument Type: PurpleAccount *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleAccount account
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
		 * Argument Name: who
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string who
		{
			get
			{
				return this.Data.who;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.who = value;
			}
		}

		/*
		 * Argument Name: ui_data
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr ui_data
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
		 * Argument Name: proto_data
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr proto_data
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
		 * Argument Name: prpl_ops
		 * Argument Type: PurpleWhiteboardPrplOps *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleWhiteboardPrplOps prpl_ops
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
		 * Argument Name: draw_list
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurpleWhiteboard.draw_list in file "whiteboard.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurpleWhiteboard
	{
		/*
		 * int state
		 */
		public int state;

		/*
		 * PurpleAccount * account
		 */
		public IntPtr account;

		/*
		 * char * who
		 */
		public string who;

		/*
		 * void * ui_data
		 */
		public IntPtr ui_data;

		/*
		 * void * proto_data
		 */
		public IntPtr proto_data;

		/*
		 * PurpleWhiteboardPrplOps * prpl_ops
		 */
		public IntPtr prpl_ops;

		/*
		 * GList * draw_list
		 */
		public IntPtr draw_list;

	}

}


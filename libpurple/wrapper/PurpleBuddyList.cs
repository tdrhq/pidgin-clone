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
	 * File: blist.h
	 * Structure: PurpleBuddyList
	 */
	public class PurpleBuddyList : UnmanagedWrapper<_PurpleBuddyList>
	{
		public PurpleBuddyList()
		{
		}

		public PurpleBuddyList(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleBuddyList)Marshal.PtrToStructure(this.Reference, typeof(_PurpleBuddyList));
		}

		/*
		 * Argument Name: root
		 * Argument Type: PurpleBlistNode *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleBlistNode root
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
		 * Argument Name: buddies
		 * Argument Type: GHashTable *
		 * Argument Category: PointerToGHashTable
		 */
		/*
		 * Could not generate a wrapper for PurpleBuddyList.buddies in file "blist.h".
		 * Message: Unable to provide a type for HashTable as no function information was provided in code.
		 */

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

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurpleBuddyList
	{
		/*
		 * PurpleBlistNode * root
		 */
		public IntPtr root;

		/*
		 * GHashTable * buddies
		 */
		public IntPtr buddies;

		/*
		 * void * ui_data
		 */
		public IntPtr ui_data;

	}

}

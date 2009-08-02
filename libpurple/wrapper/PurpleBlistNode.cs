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
	 * Structure: PurpleBlistNode
	 */
	public class PurpleBlistNode : UnmanagedWrapper<_PurpleBlistNode>
	{
		public PurpleBlistNode()
		{
		}

		public PurpleBlistNode(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleBlistNode)Marshal.PtrToStructure(this.Reference, typeof(_PurpleBlistNode));
		}

		/*
		 * Argument Name: type
		 * Argument Type: PurpleBlistNodeType
		 * Argument Category: KnownEnum
		 */
		public Blist.PurpleBlistNodeType type
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
		 * Argument Name: prev
		 * Argument Type: PurpleBlistNode *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleBlistNode prev
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
		 * Argument Name: next
		 * Argument Type: PurpleBlistNode *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleBlistNode next
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
		 * Argument Name: parent
		 * Argument Type: PurpleBlistNode *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleBlistNode parent
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
		 * Argument Name: child
		 * Argument Type: PurpleBlistNode *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleBlistNode child
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
		 * Argument Name: settings
		 * Argument Type: GHashTable *
		 * Argument Category: PointerToGHashTable
		 */
		/*
		 * Could not generate a wrapper for PurpleBlistNode.settings in file "blist.h".
		 * Message: Unable to provide a type for HashTable as no function information was provided in code.
		 */

		/*
		 * Argument Name: ui_data
		 * Argument Type: void          *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to void */ IntPtr ui_data
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
		 * Argument Name: flags
		 * Argument Type: PurpleBlistNodeFlags
		 * Argument Category: KnownEnum
		 */
		public Blist.PurpleBlistNodeFlags flags
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
	public class _PurpleBlistNode
	{
		/*
		 * PurpleBlistNodeType type
		 */
		public Blist.PurpleBlistNodeType type;

		/*
		 * PurpleBlistNode * prev
		 */
		public IntPtr prev;

		/*
		 * PurpleBlistNode * next
		 */
		public IntPtr next;

		/*
		 * PurpleBlistNode * parent
		 */
		public IntPtr parent;

		/*
		 * PurpleBlistNode * child
		 */
		public IntPtr child;

		/*
		 * GHashTable * settings
		 */
		public IntPtr settings;

		/*
		 * void          * ui_data
		 */
		public IntPtr ui_data;

		/*
		 * PurpleBlistNodeFlags flags
		 */
		public Blist.PurpleBlistNodeFlags flags;

	}

}


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
	 * Structure: PurpleBuddy
	 */
	public class PurpleBuddy : UnmanagedWrapper<_PurpleBuddy>
	{
		public PurpleBuddy()
		{
		}

		public PurpleBuddy(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleBuddy)Marshal.PtrToStructure(this.Reference, typeof(_PurpleBuddy));
		}

		/*
		 * Argument Name: node
		 * Argument Type: PurpleBlistNode
		 * Argument Category: KnownStruct
		 */
		/*
		 * Could not generate a wrapper for PurpleBuddy.node in file "blist.h".
		 * Message: The struct contains an object that resolves to a function that is not a function pointer (PurpleBlistNode node).
		 */

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
		 * Argument Name: alias
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string alias
		{
			get
			{
				return this.Data.alias;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.alias = value;
			}
		}

		/*
		 * Argument Name: server_alias
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string server_alias
		{
			get
			{
				return this.Data.server_alias;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.server_alias = value;
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
		 * Argument Name: icon
		 * Argument Type: PurpleBuddyIcon *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon
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
		 * Argument Name: presence
		 * Argument Type: PurplePresence *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence
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
	public class _PurpleBuddy
	{
		/*
		 * PurpleBlistNode node
		 */
		/*
		 * Could not generate a wrapper for PurpleBuddy.node in file "blist.h".
		 * Message: The struct contains an object that resolves to a function that is not a function pointer (PurpleBlistNode node).
		 */

		/*
		 * char * name
		 */
		public string name;

		/*
		 * char * alias
		 */
		public string alias;

		/*
		 * char * server_alias
		 */
		public string server_alias;

		/*
		 * void * proto_data
		 */
		public IntPtr proto_data;

		/*
		 * PurpleBuddyIcon * icon
		 */
		public IntPtr icon;

		/*
		 * PurpleAccount * account
		 */
		public IntPtr account;

		/*
		 * PurplePresence * presence
		 */
		public IntPtr presence;

	}

}


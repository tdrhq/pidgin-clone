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
	 * File: roomlist.h
	 * Structure: PurpleRoomlistRoom
	 */
	public class PurpleRoomlistRoom : UnmanagedWrapper<_PurpleRoomlistRoom>
	{
		public PurpleRoomlistRoom()
		{
		}

		public PurpleRoomlistRoom(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleRoomlistRoom)Marshal.PtrToStructure(this.Reference, typeof(_PurpleRoomlistRoom));
		}

		/*
		 * Argument Name: type
		 * Argument Type: PurpleRoomlistRoomType
		 * Argument Category: KnownEnum
		 */
		public Roomlist.PurpleRoomlistRoomType type
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
		 * Argument Name: name
		 * Argument Type: gchar *
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
		 * Argument Name: fields
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurpleRoomlistRoom.fields in file "roomlist.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Argument Name: parent
		 * Argument Type: PurpleRoomlistRoom *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleRoomlistRoom parent
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
		 * Argument Name: expanded_once
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool expanded_once
		{
			get
			{
				return this.Data.expanded_once;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.expanded_once = value;
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurpleRoomlistRoom
	{
		/*
		 * PurpleRoomlistRoomType type
		 */
		public Roomlist.PurpleRoomlistRoomType type;

		/*
		 * gchar * name
		 */
		public string name;

		/*
		 * GList * fields
		 */
		public IntPtr fields;

		/*
		 * PurpleRoomlistRoom * parent
		 */
		public IntPtr parent;

		/*
		 * gboolean expanded_once
		 */
		public bool expanded_once;

	}

}

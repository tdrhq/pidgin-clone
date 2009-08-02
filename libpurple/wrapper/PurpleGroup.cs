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
	 * Structure: PurpleGroup
	 */
	public class PurpleGroup : UnmanagedWrapper<_PurpleGroup>
	{
		public PurpleGroup()
		{
		}

		public PurpleGroup(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleGroup)Marshal.PtrToStructure(this.Reference, typeof(_PurpleGroup));
		}

		/*
		 * Argument Name: node
		 * Argument Type: PurpleBlistNode
		 * Argument Category: KnownStruct
		 */
		/*
		 * Could not generate a wrapper for PurpleGroup.node in file "blist.h".
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
		 * Argument Name: totalsize
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public int totalsize
		{
			get
			{
				return this.Data.totalsize;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.totalsize = value;
			}
		}

		/*
		 * Argument Name: currentsize
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public int currentsize
		{
			get
			{
				return this.Data.currentsize;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.currentsize = value;
			}
		}

		/*
		 * Argument Name: online
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public int online
		{
			get
			{
				return this.Data.online;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.online = value;
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurpleGroup
	{
		/*
		 * PurpleBlistNode node
		 */
		/*
		 * Could not generate a wrapper for PurpleGroup.node in file "blist.h".
		 * Message: The struct contains an object that resolves to a function that is not a function pointer (PurpleBlistNode node).
		 */

		/*
		 * char * name
		 */
		public string name;

		/*
		 * int totalsize
		 */
		public int totalsize;

		/*
		 * int currentsize
		 */
		public int currentsize;

		/*
		 * int online
		 */
		public int online;

	}

}


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
	public class PurpleConvChatBuddy : UnmanagedWrapper<_PurpleConvChatBuddy>
	{
		public PurpleConvChatBuddy()
		{
		}

		public PurpleConvChatBuddy(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleConvChatBuddy)Marshal.PtrToStructure(this.Reference, typeof(_PurpleConvChatBuddy));
		}

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

		public string alias_key
		{
			get
			{
				return this.Data.alias_key;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.alias_key = value;
			}
		}

		public bool buddy
		{
			get
			{
				return this.Data.buddy;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.buddy = value;
			}
		}

		public Conversation.PurpleConvChatBuddyFlags flags
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
	class _PurpleConvChatBuddy
	{
		/*
		 * char * name
		 */
		IntPtr name;

		/*
		 * char * alias
		 */
		IntPtr alias;

		/*
		 * char * alias_key
		 */
		IntPtr alias_key;

		/*
		 * gboolean buddy
		 */
		IntPtr buddy;

		/*
		 * PurpleConvChatBuddyFlags flags
		 */
		/* Cannot generate struct for type KnownEnum */

	}

}


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
	 * File: conversation.h
	 * Structure: PurpleConvChat
	 */
	public class PurpleConvChat : UnmanagedWrapper<_PurpleConvChat>
	{
		public PurpleConvChat()
		{
		}

		public PurpleConvChat(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleConvChat)Marshal.PtrToStructure(this.Reference, typeof(_PurpleConvChat));
		}

		/*
		 * Argument Name: conv
		 * Argument Type: PurpleConversation *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleConversation conv
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
		 * Argument Name: in_room
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurpleConvChat.in_room in file "conversation.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Argument Name: ignored
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurpleConvChat.ignored in file "conversation.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Argument Name: who
		 * Argument Type: char  *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to char */ IntPtr who
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
		 * Argument Name: topic
		 * Argument Type: char  *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to char */ IntPtr topic
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
		 * Argument Name: id
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public int id
		{
			get
			{
				return this.Data.id;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.id = value;
			}
		}

		/*
		 * Argument Name: nick
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string nick
		{
			get
			{
				return this.Data.nick;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.nick = value;
			}
		}

		/*
		 * Argument Name: left
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool left
		{
			get
			{
				return this.Data.left;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.left = value;
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurpleConvChat
	{
		/*
		 * PurpleConversation * conv
		 */
		public IntPtr conv;

		/*
		 * GList * in_room
		 */
		public IntPtr in_room;

		/*
		 * GList * ignored
		 */
		public IntPtr ignored;

		/*
		 * char  * who
		 */
		public IntPtr who;

		/*
		 * char  * topic
		 */
		public IntPtr topic;

		/*
		 * int id
		 */
		public int id;

		/*
		 * char * nick
		 */
		public string nick;

		/*
		 * gboolean left
		 */
		public bool left;

	}

}


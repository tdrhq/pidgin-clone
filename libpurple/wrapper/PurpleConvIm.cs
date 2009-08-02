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
	 * Structure: PurpleConvIm
	 */
	public class PurpleConvIm : UnmanagedWrapper<_PurpleConvIm>
	{
		public PurpleConvIm()
		{
		}

		public PurpleConvIm(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleConvIm)Marshal.PtrToStructure(this.Reference, typeof(_PurpleConvIm));
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
		 * Argument Name: typing_state
		 * Argument Type: PurpleTypingState
		 * Argument Category: KnownEnum
		 */
		public Conversation.PurpleTypingState typing_state
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
		 * Argument Name: typing_timeout
		 * Argument Type: guint
		 * Argument Category: Native
		 */
		public uint typing_timeout
		{
			get
			{
				return this.Data.typing_timeout;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.typing_timeout = value;
			}
		}

		/*
		 * Argument Name: type_again
		 * Argument Type: time_t
		 * Argument Category: DateTime
		 */
		public DateTime type_again
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
		 * Argument Name: send_typed_timeout
		 * Argument Type: guint
		 * Argument Category: Native
		 */
		public uint send_typed_timeout
		{
			get
			{
				return this.Data.send_typed_timeout;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.send_typed_timeout = value;
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

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurpleConvIm
	{
		/*
		 * PurpleConversation * conv
		 */
		public IntPtr conv;

		/*
		 * PurpleTypingState typing_state
		 */
		public Conversation.PurpleTypingState typing_state;

		/*
		 * guint typing_timeout
		 */
		public uint typing_timeout;

		/*
		 * time_t type_again
		 */
		public ulong type_again;

		/*
		 * guint send_typed_timeout
		 */
		public uint send_typed_timeout;

		/*
		 * PurpleBuddyIcon * icon
		 */
		public IntPtr icon;

	}

}


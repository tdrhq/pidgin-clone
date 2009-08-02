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
	 * Structure: PurpleConversation
	 */
	public class PurpleConversation : UnmanagedWrapper<_PurpleConversation>
	{
		public PurpleConversation()
		{
		}

		public PurpleConversation(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleConversation)Marshal.PtrToStructure(this.Reference, typeof(_PurpleConversation));
		}

		/*
		 * Argument Name: type
		 * Argument Type: PurpleConversationType
		 * Argument Category: KnownEnum
		 */
		public Conversation.PurpleConversationType type
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
		 * Argument Name: title
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string title
		{
			get
			{
				return this.Data.title;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.title = value;
			}
		}

		/*
		 * Argument Name: logging
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool logging
		{
			get
			{
				return this.Data.logging;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.logging = value;
			}
		}

		/*
		 * Argument Name: logs
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurpleConversation.logs in file "conversation.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Argument Name: {
		PurpleConvIm   *im
		 * Argument Type: union
		 * Argument Category: InnerStruct
		 */
		/*
		 * Could not generate a wrapper for PurpleConversation.{
		PurpleConvIm   *im in file "conversation.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * Argument Name: chat
		 * Argument Type: PurpleConvChat *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleConvChat chat
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
		 * Argument Name: misc
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr misc
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
	public class _PurpleConversation
	{
		/*
		 * PurpleConversationType type
		 */
		public Conversation.PurpleConversationType type;

		/*
		 * PurpleAccount * account
		 */
		public IntPtr account;

		/*
		 * char * name
		 */
		public string name;

		/*
		 * char * title
		 */
		public string title;

		/*
		 * gboolean logging
		 */
		public bool logging;

		/*
		 * GList * logs
		 */
		public IntPtr logs;

		/*
		 * union {
		PurpleConvIm   *im
		 */
		/*
		 * Could not generate a wrapper for PurpleConversation.{
		PurpleConvIm   *im in file "conversation.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * PurpleConvChat * chat
		 */
		public IntPtr chat;

		/*
		 * void * misc
		 */
		public IntPtr misc;

	}

}


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
	 * File: util.h
	 * Structure: PurpleMenuAction
	 */
	public class PurpleMenuAction : UnmanagedWrapper<_PurpleMenuAction>
	{
		public PurpleMenuAction()
		{
		}

		public PurpleMenuAction(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleMenuAction)Marshal.PtrToStructure(this.Reference, typeof(_PurpleMenuAction));
		}

		/*
		 * Argument Name: label
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string label
		{
			get
			{
				return this.Data.label;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.label = value;
			}
		}

		/*
		 * Argument Name: callback
		 * Argument Type: PurpleCallback
		 * Argument Category: KnownFunctionPointer
		 */
		public Signals.PurpleCallback callback
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
		 * Argument Name: data
		 * Argument Type: gpointer
		 * Argument Category: VoidPointer
		 */
		public IntPtr data
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
		 * Argument Name: children
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurpleMenuAction.children in file "util.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurpleMenuAction
	{
		/*
		 * char * label
		 */
		public string label;

		/*
		 * PurpleCallback callback
		 */
		public IntPtr callback;

		/*
		 * gpointer data
		 */
		public IntPtr data;

		/*
		 * GList * children
		 */
		public IntPtr children;

	}

}


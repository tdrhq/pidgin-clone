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
	 * File: pounce.h
	 * Structure: PurplePounce
	 */
	public class PurplePounce : UnmanagedWrapper<_PurplePounce>
	{
		public PurplePounce()
		{
		}

		public PurplePounce(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurplePounce)Marshal.PtrToStructure(this.Reference, typeof(_PurplePounce));
		}

		/*
		 * Argument Name: ui_type
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string ui_type
		{
			get
			{
				return this.Data.ui_type;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.ui_type = value;
			}
		}

		/*
		 * Argument Name: events
		 * Argument Type: PurplePounceEvent
		 * Argument Category: KnownEnum
		 */
		public Pounce.PurplePounceEvent events
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
		 * Argument Name: options
		 * Argument Type: PurplePounceOption
		 * Argument Category: KnownEnum
		 */
		public Pounce.PurplePounceOption options
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
		 * Argument Name: pouncer
		 * Argument Type: PurpleAccount *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleAccount pouncer
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
		 * Argument Name: pouncee
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string pouncee
		{
			get
			{
				return this.Data.pouncee;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.pouncee = value;
			}
		}

		/*
		 * Argument Name: actions
		 * Argument Type: GHashTable *
		 * Argument Category: PointerToGHashTable
		 */
		/*
		 * Could not generate a wrapper for PurplePounce.actions in file "pounce.h".
		 * Message: Unable to provide a type for HashTable as no function information was provided in code.
		 */

		/*
		 * Argument Name: save
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool save
		{
			get
			{
				return this.Data.save;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.save = value;
			}
		}

		/*
		 * Argument Name: data
		 * Argument Type: void *
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

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurplePounce
	{
		/*
		 * char * ui_type
		 */
		public string ui_type;

		/*
		 * PurplePounceEvent events
		 */
		public Pounce.PurplePounceEvent events;

		/*
		 * PurplePounceOption options
		 */
		public Pounce.PurplePounceOption options;

		/*
		 * PurpleAccount * pouncer
		 */
		public IntPtr pouncer;

		/*
		 * char * pouncee
		 */
		public string pouncee;

		/*
		 * GHashTable * actions
		 */
		public IntPtr actions;

		/*
		 * gboolean save
		 */
		public bool save;

		/*
		 * void * data
		 */
		public IntPtr data;

	}

}


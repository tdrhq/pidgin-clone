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
	public class PurpleLogSet : UnmanagedWrapper<_PurpleLogSet>
	{
		public PurpleLogSet()
		{
		}

		public PurpleLogSet(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleLogSet)Marshal.PtrToStructure(this.Reference, typeof(_PurpleLogSet));
		}

		public Log.PurpleLogType type
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

		public string normalized_name
		{
			get
			{
				return this.Data.normalized_name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.normalized_name = value;
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	class _PurpleLogSet
	{
		/*
		 * PurpleLogType type
		 */
		/* Cannot generate struct for type KnownEnum */

		/*
		 * char * name
		 */
		IntPtr name;

		/*
		 * PurpleAccount * account
		 */
		/* Cannot generate struct for type PointerToKnownStruct */

		/*
		 * gboolean buddy
		 */
		IntPtr buddy;

		/*
		 * char * normalized_name
		 */
		IntPtr normalized_name;

	}

}


/* purple
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
 * clean interface between .NET/CLR and the unmanaged C library, libpurple.
 *
 * This code isn't complete, but completely a work in progress. :)
 * Three major things left:
 *  - Resolve the remaining UNKNOWN types.
 *  - Handle translation between delegate and function pointers.
 *  - Fill in the translation between public .NET class calls and private DllImport[] calls.
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PurpleWrapper
{
	public class PurpleGroup : UnmanagedWrapper<_PurpleGroup>
	{
		public PurpleGroup()
		{
		}

		public PurpleGroup(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleGroup)Marshal.PtrToStructure(this.Reference, typeof(_PurpleGroup));
		}

		public PurpleBlistNode node
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
	class _PurpleGroup
	{
		/*
		 * PurpleBlistNode node
		 */
		UNKNOWN node;

		/*
		 * char * name
		 */
		string name;

		/*
		 * int totalsize
		 */
		int totalsize;

		/*
		 * int currentsize
		 */
		int currentsize;

		/*
		 * int online
		 */
		int online;

	}
}


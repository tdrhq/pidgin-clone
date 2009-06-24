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
	public class PurpleSrvResponse : UnmanagedWrapper<_PurpleSrvResponse>
	{
		public PurpleSrvResponse()
		{
		}

		public PurpleSrvResponse(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleSrvResponse)Marshal.PtrToStructure(this.Reference, typeof(_PurpleSrvResponse));
		}

		public char hostname[256]
		{
			get
			{
				return this.Data.hostname[256];
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.hostname[256] = value;
			}
		}

		public int port
		{
			get
			{
				return this.Data.port;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.port = value;
			}
		}

		public int weight
		{
			get
			{
				return this.Data.weight;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.weight = value;
			}
		}

		public int pref
		{
			get
			{
				return this.Data.pref;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.pref = value;
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	class _PurpleSrvResponse
	{
		/*
		 * char hostname[256]
		 */
		char hostname[256];

		/*
		 * int port
		 */
		int port;

		/*
		 * int weight
		 */
		int weight;

		/*
		 * int pref
		 */
		int pref;

	}
}


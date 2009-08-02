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
	 * File: dnssrv.h
	 * Structure: PurpleSrvResponse
	 */
	public class PurpleSrvResponse : UnmanagedWrapper<_PurpleSrvResponse>
	{
		public PurpleSrvResponse()
		{
		}

		public PurpleSrvResponse(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleSrvResponse)Marshal.PtrToStructure(this.Reference, typeof(_PurpleSrvResponse));
		}

		/*
		 * Argument Name: hostname
		 * Argument Type: char
		 * Argument Category: Native
		 */
		public string[] hostname
		{
			get
			{
				return this.Data.hostname;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.hostname = value;
			}
		}

		/*
		 * Argument Name: port
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: weight
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: pref
		 * Argument Type: int
		 * Argument Category: Native
		 */
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
	public class _PurpleSrvResponse
	{
		/*
		 * char hostname
		 */
		public string[] hostname;

		/*
		 * int port
		 */
		public int port;

		/*
		 * int weight
		 */
		public int weight;

		/*
		 * int pref
		 */
		public int pref;

	}

}


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
	 * File: stun.h
	 * Structure: PurpleStunNatDiscovery
	 */
	public class PurpleStunNatDiscovery : UnmanagedWrapper<_PurpleStunNatDiscovery>
	{
		public PurpleStunNatDiscovery()
		{
		}

		public PurpleStunNatDiscovery(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleStunNatDiscovery)Marshal.PtrToStructure(this.Reference, typeof(_PurpleStunNatDiscovery));
		}

		/*
		 * Argument Name: status
		 * Argument Type: PurpleStunStatus
		 * Argument Category: KnownEnum
		 */
		public Stun.PurpleStunStatus status
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
		 * Argument Name: type
		 * Argument Type: PurpleStunNatType
		 * Argument Category: KnownEnum
		 */
		public Stun.PurpleStunNatType type
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
		 * Argument Name: publicip
		 * Argument Type: char
		 * Argument Category: Native
		 */
		public string[] publicip
		{
			get
			{
				return this.Data.publicip;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.publicip = value;
			}
		}

		/*
		 * Argument Name: servername
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string servername
		{
			get
			{
				return this.Data.servername;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.servername = value;
			}
		}

		/*
		 * Argument Name: lookup_time
		 * Argument Type: time_t
		 * Argument Category: DateTime
		 */
		public DateTime lookup_time
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
	public class _PurpleStunNatDiscovery
	{
		/*
		 * PurpleStunStatus status
		 */
		public Stun.PurpleStunStatus status;

		/*
		 * PurpleStunNatType type
		 */
		public Stun.PurpleStunNatType type;

		/*
		 * char publicip
		 */
		public string[] publicip;

		/*
		 * char * servername
		 */
		public string servername;

		/*
		 * time_t lookup_time
		 */
		public ulong lookup_time;

	}

}


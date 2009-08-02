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
	 * File: connection.h
	 * Structure: PurpleConnection
	 */
	public class PurpleConnection : UnmanagedWrapper<_PurpleConnection>
	{
		public PurpleConnection()
		{
		}

		public PurpleConnection(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleConnection)Marshal.PtrToStructure(this.Reference, typeof(_PurpleConnection));
		}

		/*
		 * Argument Name: prpl
		 * Argument Type: PurplePlugin *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurplePlugin prpl
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
		 * Argument Name: flags
		 * Argument Type: PurpleConnectionFlags
		 * Argument Category: KnownEnum
		 */
		public Connection.PurpleConnectionFlags flags
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
		 * Argument Name: state
		 * Argument Type: PurpleConnectionState
		 * Argument Category: KnownEnum
		 */
		public Connection.PurpleConnectionState state
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
		 * Argument Name: password
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string password
		{
			get
			{
				return this.Data.password;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.password = value;
			}
		}

		/*
		 * Argument Name: inpa
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public int inpa
		{
			get
			{
				return this.Data.inpa;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.inpa = value;
			}
		}

		/*
		 * Argument Name: buddy_chats
		 * Argument Type: GSList *
		 * Argument Category: PointerToGSList
		 */
		/*
		 * Could not generate a wrapper for PurpleConnection.buddy_chats in file "connection.h".
		 * Message: Unable to provide a type for GSList as no function information was provided in code.
		 */

		/*
		 * Argument Name: proto_data
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr proto_data
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
		 * Argument Name: display_name
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string display_name
		{
			get
			{
				return this.Data.display_name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.display_name = value;
			}
		}

		/*
		 * Argument Name: keepalive
		 * Argument Type: guint
		 * Argument Category: Native
		 */
		public uint keepalive
		{
			get
			{
				return this.Data.keepalive;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.keepalive = value;
			}
		}

		/*
		 * Argument Name: wants_to_die
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool wants_to_die
		{
			get
			{
				return this.Data.wants_to_die;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.wants_to_die = value;
			}
		}

		/*
		 * Argument Name: disconnect_timeout
		 * Argument Type: guint
		 * Argument Category: Native
		 */
		public uint disconnect_timeout
		{
			get
			{
				return this.Data.disconnect_timeout;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.disconnect_timeout = value;
			}
		}

		/*
		 * Argument Name: last_received
		 * Argument Type: time_t
		 * Argument Category: DateTime
		 */
		public DateTime last_received
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
	public class _PurpleConnection
	{
		/*
		 * PurplePlugin * prpl
		 */
		public IntPtr prpl;

		/*
		 * PurpleConnectionFlags flags
		 */
		public Connection.PurpleConnectionFlags flags;

		/*
		 * PurpleConnectionState state
		 */
		public Connection.PurpleConnectionState state;

		/*
		 * PurpleAccount * account
		 */
		public IntPtr account;

		/*
		 * char * password
		 */
		public string password;

		/*
		 * int inpa
		 */
		public int inpa;

		/*
		 * GSList * buddy_chats
		 */
		public IntPtr buddy_chats;

		/*
		 * void * proto_data
		 */
		public IntPtr proto_data;

		/*
		 * char * display_name
		 */
		public string display_name;

		/*
		 * guint keepalive
		 */
		public uint keepalive;

		/*
		 * gboolean wants_to_die
		 */
		public bool wants_to_die;

		/*
		 * guint disconnect_timeout
		 */
		public uint disconnect_timeout;

		/*
		 * time_t last_received
		 */
		public ulong last_received;

	}

}


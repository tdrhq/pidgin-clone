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
	public class PurpleRoomlistField : UnmanagedWrapper<_PurpleRoomlistField>
	{
		public PurpleRoomlistField()
		{
		}

		public PurpleRoomlistField(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleRoomlistField)Marshal.PtrToStructure(this.Reference, typeof(_PurpleRoomlistField));
		}

		public PurpleRoomlistFieldType type
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

		public bool hidden
		{
			get
			{
				return this.Data.hidden;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.hidden = value;
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	class _PurpleRoomlistField
	{
		/*
		 * PurpleRoomlistFieldType type
		 */
		UNKNOWN type;

		/*
		 * gchar * label
		 */
		string label;

		/*
		 * gchar * name
		 */
		string name;

		/*
		 * gboolean hidden
		 */
		bool hidden;

	}
}


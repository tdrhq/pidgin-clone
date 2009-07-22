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
	public class PurpleCircBuffer : UnmanagedWrapper<_PurpleCircBuffer>
	{
		public PurpleCircBuffer()
		{
		}

		public PurpleCircBuffer(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleCircBuffer)Marshal.PtrToStructure(this.Reference, typeof(_PurpleCircBuffer));
		}

		public string buffer
		{
			get
			{
				return this.Data.buffer;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.buffer = value;
			}
		}

		public ulong growsize
		{
			get
			{
				return this.Data.growsize;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.growsize = value;
			}
		}

		public ulong buflen
		{
			get
			{
				return this.Data.buflen;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.buflen = value;
			}
		}

		public ulong bufused
		{
			get
			{
				return this.Data.bufused;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.bufused = value;
			}
		}

		public string inptr
		{
			get
			{
				return this.Data.inptr;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.inptr = value;
			}
		}

		public string outptr
		{
			get
			{
				return this.Data.outptr;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.outptr = value;
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	class _PurpleCircBuffer
	{
		/*
		 * gchar * buffer
		 */
		IntPtr buffer;

		/*
		 * gsize growsize
		 */
		IntPtr growsize;

		/*
		 * gsize buflen
		 */
		IntPtr buflen;

		/*
		 * gsize bufused
		 */
		IntPtr bufused;

		/*
		 * gchar * inptr
		 */
		IntPtr inptr;

		/*
		 * gchar * outptr
		 */
		IntPtr outptr;

	}

}


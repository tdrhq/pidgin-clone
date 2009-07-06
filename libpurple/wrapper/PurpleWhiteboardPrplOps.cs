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
	public class PurpleWhiteboardPrplOps : UnmanagedWrapper<_PurpleWhiteboardPrplOps>
	{
		public PurpleWhiteboardPrplOps()
		{
		}

		public PurpleWhiteboardPrplOps(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleWhiteboardPrplOps)Marshal.PtrToStructure(this.Reference, typeof(_PurpleWhiteboardPrplOps));
		}

		public void start
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

		public void end
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

		public void get_dimensions
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

		public void set_dimensions
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

		public void get_brush
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

		public void set_brush
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

		public void send_draw_list
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

		public void clear
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

		public void purple_reserved1
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

		public void purple_reserved2
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

		public void purple_reserved3
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

		public void purple_reserved4
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
	class _PurpleWhiteboardPrplOps
	{
		/*
		 * void (*start)(PurpleWhiteboard * wb)
		 */
		void start;

		/*
		 * void (*end)(PurpleWhiteboard * wb)
		 */
		void end;

		/*
		 * void (*get_dimensions)(PurpleWhiteboard * wb, int * width, int * height)
		 */
		void get_dimensions;

		/*
		 * void (*set_dimensions)(PurpleWhiteboard * wb, int width, int height)
		 */
		void set_dimensions;

		/*
		 * void (*get_brush)(PurpleWhiteboard * wb, int * size, int * color)
		 */
		void get_brush;

		/*
		 * void (*set_brush)(PurpleWhiteboard * wb, int size, int color)
		 */
		void set_brush;

		/*
		 * void (*send_draw_list)(PurpleWhiteboard * wb, GList * draw_list)
		 */
		void send_draw_list;

		/*
		 * void (*clear)(PurpleWhiteboard * wb)
		 */
		void clear;

		/*
		 * void (*purple_reserved1)()
		 */
		void purple_reserved1;

		/*
		 * void (*purple_reserved2)()
		 */
		void purple_reserved2;

		/*
		 * void (*purple_reserved3)()
		 */
		void purple_reserved3;

		/*
		 * void (*purple_reserved4)()
		 */
		void purple_reserved4;

	}

}


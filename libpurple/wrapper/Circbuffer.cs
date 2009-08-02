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
	 * File: circbuffer.h
	 */
	public class Circbuffer
	{

		/*
		 * Function Name: purple_circ_buffer_new
		 * Function Type: PurpleCircBuffer *
		 * Function RawType: PurpleCircBuffer
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleCircBuffer * purple_circ_buffer_new(gsize growsize)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_circ_buffer_new(ulong growsize);

		public static PurpleCircBuffer CircBufferNew(ulong growsize)
		{
			return new PurpleCircBuffer(purple_circ_buffer_new(growsize));
		}

		/*
		 * Function Name: purple_circ_buffer_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_circ_buffer_destroy(PurpleCircBuffer * buf)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_circ_buffer_destroy(IntPtr buf);

		public static void CircBufferDestroy(PurpleCircBuffer buf)
		{
			purple_circ_buffer_destroy(buf.Reference);
		}

		/*
		 * Function Name: purple_circ_buffer_append
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_circ_buffer_append(PurpleCircBuffer * buf, gconstpointer src, gsize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_circ_buffer_append(IntPtr buf, IntPtr src, ulong len);

		public static void CircBufferAppend(PurpleCircBuffer buf, IntPtr src, ulong len)
		{
			purple_circ_buffer_append(buf.Reference, src, len);
		}

		/*
		 * Function Name: purple_circ_buffer_get_max_read
		 * Function Type: gsize
		 * Function RawType: gsize
		 * Function Category: Native
		 */
		/*
		 * gsize purple_circ_buffer_get_max_read(PurpleCircBuffer * buf)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_circ_buffer_get_max_read(IntPtr buf);

		public static ulong CircBufferGetMaxRead(PurpleCircBuffer buf)
		{
			return purple_circ_buffer_get_max_read(buf.Reference);
		}

		/*
		 * Function Name: purple_circ_buffer_mark_read
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_circ_buffer_mark_read(PurpleCircBuffer * buf, gsize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_circ_buffer_mark_read(IntPtr buf, ulong len);

		public static bool CircBufferMarkRead(PurpleCircBuffer buf, ulong len)
		{
			return purple_circ_buffer_mark_read(buf.Reference, len);
		}

	}
}


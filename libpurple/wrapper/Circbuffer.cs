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
	public class Circbuffer
	{
		/*
		 * PurpleCircBuffer * purple_circ_buffer_new(gsize growsize)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_circ_buffer_new(UNKNOWN growsize);

		public static PurpleCircBuffer CircBufferNew(gsize growsize)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_circ_buffer_destroy(PurpleCircBuffer * buf)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_circ_buffer_destroy(IntPtr buf);

		public static void CircBufferDestroy(PurpleCircBuffer buf)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_circ_buffer_append(PurpleCircBuffer * buf, gconstpointer src, gsize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_circ_buffer_append(IntPtr buf, UNKNOWN src, UNKNOWN len);

		public static void CircBufferAppend(PurpleCircBuffer buf, gconstpointer src, gsize len)
		{
			throw new NotImplementedException();
		}

		/*
		 * gsize purple_circ_buffer_get_max_read(PurpleCircBuffer * buf)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_circ_buffer_get_max_read(IntPtr buf);

		public static gsize CircBufferGetMaxRead(PurpleCircBuffer buf)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_circ_buffer_mark_read(PurpleCircBuffer * buf, gsize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_circ_buffer_mark_read(IntPtr buf, UNKNOWN len);

		public static bool CircBufferMarkRead(PurpleCircBuffer buf, gsize len)
		{
			throw new NotImplementedException();
		}

	}
}


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
	public class Imgstore
	{
		/*
		 * PurpleStoredImage * purple_imgstore_add(gpointer data, size_t size, char * filename)
		 * 
		 * Could not generate a wrapper for purple_imgstore_add in file "imgstore.h".
		 * Message: The type could not be resolved (PurpleStoredImage * purple_imgstore_add(gpointer data, size_t size, char * filename)).
		 */

		/*
		 * PurpleStoredImage * purple_imgstore_new_from_file(char * path)
		 * 
		 * Could not generate a wrapper for purple_imgstore_new_from_file in file "imgstore.h".
		 * Message: The type could not be resolved (PurpleStoredImage * purple_imgstore_new_from_file(char * path)).
		 */

		/*
		 * int purple_imgstore_add_with_id(gpointer data, size_t size, char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_imgstore_add_with_id(IntPtr data, ulong size, string filename);

		public static int AddWithId(IntPtr data, ulong size, string filename)
		{
			return purple_imgstore_add_with_id(data, size, filename);
		}

		/*
		 * PurpleStoredImage * purple_imgstore_find_by_id(int id)
		 * 
		 * Could not generate a wrapper for purple_imgstore_find_by_id in file "imgstore.h".
		 * Message: The type could not be resolved (PurpleStoredImage * purple_imgstore_find_by_id(int id)).
		 */

		/*
		 * gconstpointer purple_imgstore_get_data(PurpleStoredImage * img)
		 * 
		 * Could not generate a wrapper for purple_imgstore_get_data in file "imgstore.h".
		 * Message: The type could not be resolved (PurpleStoredImage * img).
		 */

		/*
		 * size_t purple_imgstore_get_size(PurpleStoredImage * img)
		 * 
		 * Could not generate a wrapper for purple_imgstore_get_size in file "imgstore.h".
		 * Message: The type could not be resolved (PurpleStoredImage * img).
		 */

		/*
		 * char * purple_imgstore_get_filename(PurpleStoredImage * img)
		 * 
		 * Could not generate a wrapper for purple_imgstore_get_filename in file "imgstore.h".
		 * Message: The type could not be resolved (PurpleStoredImage * img).
		 */

		/*
		 * char * purple_imgstore_get_extension(PurpleStoredImage * img)
		 * 
		 * Could not generate a wrapper for purple_imgstore_get_extension in file "imgstore.h".
		 * Message: The type could not be resolved (PurpleStoredImage * img).
		 */

		/*
		 * PurpleStoredImage * purple_imgstore_ref(PurpleStoredImage * img)
		 * 
		 * Could not generate a wrapper for purple_imgstore_ref in file "imgstore.h".
		 * Message: The type could not be resolved (PurpleStoredImage * purple_imgstore_ref(PurpleStoredImage * img)).
		 */

		/*
		 * PurpleStoredImage * purple_imgstore_unref(PurpleStoredImage * img)
		 * 
		 * Could not generate a wrapper for purple_imgstore_unref in file "imgstore.h".
		 * Message: The type could not be resolved (PurpleStoredImage * purple_imgstore_unref(PurpleStoredImage * img)).
		 */

		/*
		 * void purple_imgstore_ref_by_id(int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_imgstore_ref_by_id(int id);

		public static void RefById(int id)
		{
			purple_imgstore_ref_by_id(id);
		}

		/*
		 * void purple_imgstore_unref_by_id(int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_imgstore_unref_by_id(int id);

		public static void UnrefById(int id)
		{
			purple_imgstore_unref_by_id(id);
		}

		/*
		 * void * purple_imgstore_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_imgstore_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_imgstore_get_handle();
		}

		/*
		 * void purple_imgstore_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_imgstore_init();

		public static void Init()
		{
			purple_imgstore_init();
		}

		/*
		 * void purple_imgstore_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_imgstore_uninit();

		public static void Uninit()
		{
			purple_imgstore_uninit();
		}

	}
}


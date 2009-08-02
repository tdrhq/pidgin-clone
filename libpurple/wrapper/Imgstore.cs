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
	 * File: imgstore.h
	 */
	public class Imgstore
	{

		/*
		 * Function Name: purple_imgstore_add
		 * Function Type: PurpleStoredImage *
		 * Function RawType: PurpleStoredImage
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStoredImage * purple_imgstore_add(gpointer data, size_t size, char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_imgstore_add(IntPtr data, ulong size, string filename);

		public static /* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr Add(IntPtr data, ulong size, string filename)
		{
			/* Unable to process the return value of purple_imgstore_add, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_imgstore_new_from_file
		 * Function Type: PurpleStoredImage *
		 * Function RawType: PurpleStoredImage
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStoredImage * purple_imgstore_new_from_file(char * path)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_imgstore_new_from_file(string path);

		public static /* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr NewFromFile(string path)
		{
			/* Unable to process the return value of purple_imgstore_new_from_file, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_imgstore_add_with_id
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
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
		 * Function Name: purple_imgstore_find_by_id
		 * Function Type: PurpleStoredImage *
		 * Function RawType: PurpleStoredImage
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStoredImage * purple_imgstore_find_by_id(int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_imgstore_find_by_id(int id);

		public static /* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr FindById(int id)
		{
			/* Unable to process the return value of purple_imgstore_find_by_id, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_imgstore_get_data
		 * Function Type: gconstpointer
		 * Function RawType: gconstpointer
		 * Function Category: VoidPointer
		 */
		/*
		 * gconstpointer purple_imgstore_get_data(PurpleStoredImage * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_imgstore_get_data(IntPtr img);

		public static IntPtr GetData(/* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr img)
		{
			/* Unable to process img, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_imgstore_get_size
		 * Function Type: size_t
		 * Function RawType: size_t
		 * Function Category: Native
		 */
		/*
		 * size_t purple_imgstore_get_size(PurpleStoredImage * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_imgstore_get_size(IntPtr img);

		public static ulong GetSize(/* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr img)
		{
			/* Unable to process img, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_imgstore_get_filename
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_imgstore_get_filename(PurpleStoredImage * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_imgstore_get_filename(IntPtr img);

		public static string GetFilename(/* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr img)
		{
			/* Unable to process img, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_imgstore_get_extension
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_imgstore_get_extension(PurpleStoredImage * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_imgstore_get_extension(IntPtr img);

		public static string GetExtension(/* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr img)
		{
			/* Unable to process img, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_imgstore_ref
		 * Function Type: PurpleStoredImage *
		 * Function RawType: PurpleStoredImage
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStoredImage * purple_imgstore_ref(PurpleStoredImage * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_imgstore_ref(IntPtr img);

		public static /* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr Ref(/* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr img)
		{
			/* Unable to process img, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_imgstore_unref
		 * Function Type: PurpleStoredImage *
		 * Function RawType: PurpleStoredImage
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStoredImage * purple_imgstore_unref(PurpleStoredImage * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_imgstore_unref(IntPtr img);

		public static /* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr Unref(/* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr img)
		{
			/* Unable to process img, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_imgstore_ref_by_id
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
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
		 * Function Name: purple_imgstore_unref_by_id
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_imgstore_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
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
		 * Function Name: purple_imgstore_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_imgstore_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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


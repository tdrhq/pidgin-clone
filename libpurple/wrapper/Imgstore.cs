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
	public class Imgstore
	{
		/*
		 * int purple_imgstore_add_with_id(gpointer data, size_t size, char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_imgstore_add_with_id(IntPtr data, UNKNOWN size, string filename);

		public static int AddWithId(IntPtr data, size_t size, string filename)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStoredImage * purple_imgstore_find_by_id(int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_imgstore_find_by_id(int id);

		public static PurpleStoredImage FindById(int id)
		{
			throw new NotImplementedException();
		}

		/*
		 * gconstpointer purple_imgstore_get_data(PurpleStoredImage * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_imgstore_get_data(IntPtr img);

		public static gconstpointer GetData(PurpleStoredImage img)
		{
			throw new NotImplementedException();
		}

		/*
		 * size_t purple_imgstore_get_size(PurpleStoredImage * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_imgstore_get_size(IntPtr img);

		public static size_t GetSize(PurpleStoredImage img)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_imgstore_get_filename(PurpleStoredImage * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_imgstore_get_filename(IntPtr img);

		public static string GetFilename(PurpleStoredImage img)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_imgstore_get_extension(PurpleStoredImage * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_imgstore_get_extension(IntPtr img);

		public static string GetExtension(PurpleStoredImage img)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_imgstore_ref_by_id(int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_imgstore_ref_by_id(int id);

		public static void RefById(int id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_imgstore_unref_by_id(int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_imgstore_unref_by_id(int id);

		public static void UnrefById(int id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_imgstore_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_imgstore_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_imgstore_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_imgstore_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_imgstore_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_imgstore_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

	}
}


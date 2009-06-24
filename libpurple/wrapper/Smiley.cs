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
	public class Smiley
	{
		/*
		 * GType purple_smiley_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_smiley_get_type();

		public static GType GetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_smiley_delete(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_smiley_delete(IntPtr smiley);

		public static void Delete(PurpleSmiley smiley)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_smiley_set_shortcut(PurpleSmiley * smiley, char * shortcut)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_smiley_set_shortcut(IntPtr smiley, string shortcut);

		public static bool SetShortcut(PurpleSmiley smiley, string shortcut)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_smiley_set_data(PurpleSmiley * smiley, guchar * smiley_data, size_t smiley_data_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_smiley_set_data(IntPtr smiley, IntPtr smiley_data, UNKNOWN smiley_data_len);

		public static void SetData(PurpleSmiley smiley, guchar smiley_data, size_t smiley_data_len)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_smiley_get_shortcut(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_smiley_get_shortcut(IntPtr smiley);

		public static string GetShortcut(PurpleSmiley smiley)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_smiley_get_checksum(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_smiley_get_checksum(IntPtr smiley);

		public static string GetChecksum(PurpleSmiley smiley)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStoredImage * purple_smiley_get_stored_image(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_smiley_get_stored_image(IntPtr smiley);

		public static PurpleStoredImage GetStoredImage(PurpleSmiley smiley)
		{
			throw new NotImplementedException();
		}

		/*
		 * gconstpointer purple_smiley_get_data(PurpleSmiley * smiley, size_t * len)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_smiley_get_data(IntPtr smiley, IntPtr len);

		public static gconstpointer GetData(PurpleSmiley smiley, size_t len)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_smiley_get_extension(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_smiley_get_extension(IntPtr smiley);

		public static string GetExtension(PurpleSmiley smiley)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_smiley_get_full_path(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_smiley_get_full_path(IntPtr smiley);

		public static string GetFullPath(PurpleSmiley smiley)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_smileys_get_storing_dir()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_smileys_get_storing_dir();

		public static string SmileysGetStoringDir()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_smileys_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_smileys_init();

		public static void SmileysInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_smileys_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_smileys_uninit();

		public static void SmileysUninit()
		{
			throw new NotImplementedException();
		}

	}
}


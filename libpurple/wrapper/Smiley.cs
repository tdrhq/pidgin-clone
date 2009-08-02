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
	 * File: smiley.h
	 */
	public class Smiley
	{

		/*
		 * Function Name: purple_smiley_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_smiley_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_smiley_get_type();

		public static /* libobject */ IntPtr GetType()
		{
			/* Unable to process the return value of purple_smiley_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smiley_new
		 * Function Type: PurpleSmiley *
		 * Function RawType: PurpleSmiley
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSmiley * purple_smiley_new(PurpleStoredImage * img, char * shortcut)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_smiley_new(IntPtr img, string shortcut);

		public static /* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr New(/* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr img, string shortcut)
		{
			/* Unable to process img, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smiley_new_from_file
		 * Function Type: PurpleSmiley *
		 * Function RawType: PurpleSmiley
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSmiley * purple_smiley_new_from_file(char * shortcut, char * filepath)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_smiley_new_from_file(string shortcut, string filepath);

		public static /* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr NewFromFile(string shortcut, string filepath)
		{
			/* Unable to process the return value of purple_smiley_new_from_file, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smiley_delete
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_smiley_delete(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_smiley_delete(IntPtr smiley);

		public static void Delete(/* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr smiley)
		{
			/* Unable to process smiley, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smiley_set_shortcut
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_smiley_set_shortcut(PurpleSmiley * smiley, char * shortcut)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_smiley_set_shortcut(IntPtr smiley, string shortcut);

		public static bool SetShortcut(/* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr smiley, string shortcut)
		{
			/* Unable to process smiley, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smiley_get_shortcut
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_smiley_get_shortcut(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_smiley_get_shortcut(IntPtr smiley);

		public static string GetShortcut(/* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr smiley)
		{
			/* Unable to process smiley, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smiley_get_checksum
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_smiley_get_checksum(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_smiley_get_checksum(IntPtr smiley);

		public static string GetChecksum(/* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr smiley)
		{
			/* Unable to process smiley, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smiley_get_stored_image
		 * Function Type: PurpleStoredImage *
		 * Function RawType: PurpleStoredImage
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStoredImage * purple_smiley_get_stored_image(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_smiley_get_stored_image(IntPtr smiley);

		public static /* Resolved as PointerToUnknownStruct to PurpleStoredImage */ IntPtr GetStoredImage(/* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr smiley)
		{
			/* Unable to process smiley, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smiley_get_data
		 * Function Type: gconstpointer
		 * Function RawType: gconstpointer
		 * Function Category: VoidPointer
		 */
		/*
		 * gconstpointer purple_smiley_get_data(PurpleSmiley * smiley, size_t * len)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_smiley_get_data(IntPtr smiley, IntPtr len);

		public static IntPtr GetData(/* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr smiley, /* Resolved as PointerToUnknownStruct to size_t */ IntPtr len)
		{
			/* Unable to process smiley, a PointerToUnknownStruct. */
			/* Unable to process len, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smiley_get_extension
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_smiley_get_extension(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_smiley_get_extension(IntPtr smiley);

		public static string GetExtension(/* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr smiley)
		{
			/* Unable to process smiley, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smiley_get_full_path
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_smiley_get_full_path(PurpleSmiley * smiley)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_smiley_get_full_path(IntPtr smiley);

		public static string GetFullPath(/* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr smiley)
		{
			/* Unable to process smiley, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smileys_get_all
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_smileys_get_all()
		 * 
		 * Could not generate a wrapper for purple_smileys_get_all in file "smiley.h".
		 * Message: Unknown inner-type of argument 0 of purple_smileys_get_all
		 */

		/*
		 * Function Name: purple_smileys_find_by_shortcut
		 * Function Type: PurpleSmiley *
		 * Function RawType: PurpleSmiley
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSmiley * purple_smileys_find_by_shortcut(char * shortcut)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_smileys_find_by_shortcut(string shortcut);

		public static /* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr SmileysFindByShortcut(string shortcut)
		{
			/* Unable to process the return value of purple_smileys_find_by_shortcut, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smileys_find_by_checksum
		 * Function Type: PurpleSmiley *
		 * Function RawType: PurpleSmiley
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSmiley * purple_smileys_find_by_checksum(char * checksum)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_smileys_find_by_checksum(string checksum);

		public static /* Resolved as PointerToUnknownStruct to PurpleSmiley */ IntPtr SmileysFindByChecksum(string checksum)
		{
			/* Unable to process the return value of purple_smileys_find_by_checksum, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_smileys_get_storing_dir
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_smileys_get_storing_dir()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_smileys_get_storing_dir();

		public static string SmileysGetStoringDir()
		{
			return purple_smileys_get_storing_dir();
		}

		/*
		 * Function Name: purple_smileys_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_smileys_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_smileys_init();

		public static void SmileysInit()
		{
			purple_smileys_init();
		}

		/*
		 * Function Name: purple_smileys_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_smileys_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_smileys_uninit();

		public static void SmileysUninit()
		{
			purple_smileys_uninit();
		}

	}
}


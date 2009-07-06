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
	public class Smiley
	{
		/*
		 * GType purple_smiley_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_smiley_get_type();

		public static /* libgobject */ IntPtr GetType()
		{
			/* Unable to process purple_smiley_get_type, a GObjectObject. */
			
		}

		/*
		 * PurpleSmiley * purple_smiley_new(PurpleStoredImage * img, char * shortcut)
		 * 
		 * Could not generate a wrapper for purple_smiley_new in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * purple_smiley_new(PurpleStoredImage * img, char * shortcut)).
		 */

		/*
		 * PurpleSmiley * purple_smiley_new_from_file(char * shortcut, char * filepath)
		 * 
		 * Could not generate a wrapper for purple_smiley_new_from_file in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * purple_smiley_new_from_file(char * shortcut, char * filepath)).
		 */

		/*
		 * void purple_smiley_delete(PurpleSmiley * smiley)
		 * 
		 * Could not generate a wrapper for purple_smiley_delete in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * smiley).
		 */

		/*
		 * gboolean purple_smiley_set_shortcut(PurpleSmiley * smiley, char * shortcut)
		 * 
		 * Could not generate a wrapper for purple_smiley_set_shortcut in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * smiley).
		 */

		/*
		 * char * purple_smiley_get_shortcut(PurpleSmiley * smiley)
		 * 
		 * Could not generate a wrapper for purple_smiley_get_shortcut in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * smiley).
		 */

		/*
		 * char * purple_smiley_get_checksum(PurpleSmiley * smiley)
		 * 
		 * Could not generate a wrapper for purple_smiley_get_checksum in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * smiley).
		 */

		/*
		 * PurpleStoredImage * purple_smiley_get_stored_image(PurpleSmiley * smiley)
		 * 
		 * Could not generate a wrapper for purple_smiley_get_stored_image in file "smiley.h".
		 * Message: The type could not be resolved (PurpleStoredImage * purple_smiley_get_stored_image(PurpleSmiley * smiley)).
		 */

		/*
		 * gconstpointer purple_smiley_get_data(PurpleSmiley * smiley, size_t * len)
		 * 
		 * Could not generate a wrapper for purple_smiley_get_data in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * smiley).
		 */

		/*
		 * char * purple_smiley_get_extension(PurpleSmiley * smiley)
		 * 
		 * Could not generate a wrapper for purple_smiley_get_extension in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * smiley).
		 */

		/*
		 * char * purple_smiley_get_full_path(PurpleSmiley * smiley)
		 * 
		 * Could not generate a wrapper for purple_smiley_get_full_path in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * smiley).
		 */

		/*
		 * GList * purple_smileys_get_all()
		 * 
		 * Could not generate a wrapper for purple_smileys_get_all in file "smiley.h".
		 * Message: The type could not be resolved (GList * purple_smileys_get_all()).
		 */

		/*
		 * PurpleSmiley * purple_smileys_find_by_shortcut(char * shortcut)
		 * 
		 * Could not generate a wrapper for purple_smileys_find_by_shortcut in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * purple_smileys_find_by_shortcut(char * shortcut)).
		 */

		/*
		 * PurpleSmiley * purple_smileys_find_by_checksum(char * checksum)
		 * 
		 * Could not generate a wrapper for purple_smileys_find_by_checksum in file "smiley.h".
		 * Message: The type could not be resolved (PurpleSmiley * purple_smileys_find_by_checksum(char * checksum)).
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
		 * void purple_smileys_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_smileys_init();

		public static void SmileysInit()
		{
			purple_smileys_init();
		}

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


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
	public class Accountopt
	{
		/*
		 * void purple_account_option_destroy(PurpleAccountOption * option)
		 * 
		 * Could not generate a wrapper for purple_account_option_destroy in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountOption * option).
		 */

		/*
		 * void purple_account_option_set_masked(PurpleAccountOption * option, gboolean masked)
		 * 
		 * Could not generate a wrapper for purple_account_option_set_masked in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountOption * option).
		 */

		/*
		 * void purple_account_option_set_list(PurpleAccountOption * option, GList * values)
		 * 
		 * Could not generate a wrapper for purple_account_option_set_list in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountOption * option).
		 */

		/*
		 * PurplePrefType purple_account_option_get_type(PurpleAccountOption * option)
		 * 
		 * Could not generate a wrapper for purple_account_option_get_type in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountOption * option).
		 */

		/*
		 * char * purple_account_option_get_text(PurpleAccountOption * option)
		 * 
		 * Could not generate a wrapper for purple_account_option_get_text in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountOption * option).
		 */

		/*
		 * char * purple_account_option_get_setting(PurpleAccountOption * option)
		 * 
		 * Could not generate a wrapper for purple_account_option_get_setting in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountOption * option).
		 */

		/*
		 * gboolean purple_account_option_get_default_bool(PurpleAccountOption * option)
		 * 
		 * Could not generate a wrapper for purple_account_option_get_default_bool in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountOption * option).
		 */

		/*
		 * int purple_account_option_get_default_int(PurpleAccountOption * option)
		 * 
		 * Could not generate a wrapper for purple_account_option_get_default_int in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountOption * option).
		 */

		/*
		 * gboolean purple_account_option_get_masked(PurpleAccountOption * option)
		 * 
		 * Could not generate a wrapper for purple_account_option_get_masked in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountOption * option).
		 */

		/*
		 * GList * purple_account_option_get_list(PurpleAccountOption * option)
		 * 
		 * Could not generate a wrapper for purple_account_option_get_list in file "accountopt.h".
		 * Message: The type could not be resolved (GList * purple_account_option_get_list(PurpleAccountOption * option)).
		 */

		/*
		 * void purple_account_user_split_destroy(PurpleAccountUserSplit * split)
		 * 
		 * Could not generate a wrapper for purple_account_user_split_destroy in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountUserSplit * split).
		 */

		/*
		 * char * purple_account_user_split_get_text(PurpleAccountUserSplit * split)
		 * 
		 * Could not generate a wrapper for purple_account_user_split_get_text in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountUserSplit * split).
		 */

		/*
		 * char purple_account_user_split_get_separator(PurpleAccountUserSplit * split)
		 * 
		 * Could not generate a wrapper for purple_account_user_split_get_separator in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountUserSplit * split).
		 */

		/*
		 * gboolean purple_account_user_split_get_reverse(PurpleAccountUserSplit * split)
		 * 
		 * Could not generate a wrapper for purple_account_user_split_get_reverse in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountUserSplit * split).
		 */

		/*
		 * void purple_account_user_split_set_reverse(PurpleAccountUserSplit * split, gboolean reverse)
		 * 
		 * Could not generate a wrapper for purple_account_user_split_set_reverse in file "accountopt.h".
		 * Message: The type could not be resolved (PurpleAccountUserSplit * split).
		 */

	}
}


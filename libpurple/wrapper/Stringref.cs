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
	public class Stringref
	{
		/*
		 * PurpleStringref * purple_stringref_new(char * value)
		 * 
		 * Could not generate a wrapper for purple_stringref_new in file "stringref.h".
		 * Message: The type could not be resolved (PurpleStringref * purple_stringref_new(char * value)).
		 */

		/*
		 * PurpleStringref * purple_stringref_new_noref(char * value)
		 * 
		 * Could not generate a wrapper for purple_stringref_new_noref in file "stringref.h".
		 * Message: The type could not be resolved (PurpleStringref * purple_stringref_new_noref(char * value)).
		 */

		/*
		 * PurpleStringref * purple_stringref_ref(PurpleStringref * stringref)
		 * 
		 * Could not generate a wrapper for purple_stringref_ref in file "stringref.h".
		 * Message: The type could not be resolved (PurpleStringref * purple_stringref_ref(PurpleStringref * stringref)).
		 */

		/*
		 * void purple_stringref_unref(PurpleStringref * stringref)
		 * 
		 * Could not generate a wrapper for purple_stringref_unref in file "stringref.h".
		 * Message: The type could not be resolved (PurpleStringref * stringref).
		 */

		/*
		 * char * purple_stringref_value(PurpleStringref * stringref)
		 * 
		 * Could not generate a wrapper for purple_stringref_value in file "stringref.h".
		 * Message: The type could not be resolved (PurpleStringref * stringref).
		 */

		/*
		 * int purple_stringref_cmp(PurpleStringref * s1, PurpleStringref * s2)
		 * 
		 * Could not generate a wrapper for purple_stringref_cmp in file "stringref.h".
		 * Message: The type could not be resolved (PurpleStringref * s1).
		 */

		/*
		 * size_t purple_stringref_len(PurpleStringref * stringref)
		 * 
		 * Could not generate a wrapper for purple_stringref_len in file "stringref.h".
		 * Message: The type could not be resolved (PurpleStringref * stringref).
		 */

	}
}


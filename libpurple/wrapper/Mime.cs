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
	public class Mime
	{
		/*
		 * PurpleMimeDocument * purple_mime_document_new()
		 * 
		 * Could not generate a wrapper for purple_mime_document_new in file "mime.h".
		 * Message: The type could not be resolved (PurpleMimeDocument * purple_mime_document_new()).
		 */

		/*
		 * void purple_mime_document_free(PurpleMimeDocument * doc)
		 * 
		 * Could not generate a wrapper for purple_mime_document_free in file "mime.h".
		 * Message: The type could not be resolved (PurpleMimeDocument * doc).
		 */

		/*
		 * PurpleMimeDocument * purple_mime_document_parse(char * buf)
		 * 
		 * Could not generate a wrapper for purple_mime_document_parse in file "mime.h".
		 * Message: The type could not be resolved (PurpleMimeDocument * purple_mime_document_parse(char * buf)).
		 */

		/*
		 * PurpleMimeDocument * purple_mime_document_parsen(char * buf, gsize len)
		 * 
		 * Could not generate a wrapper for purple_mime_document_parsen in file "mime.h".
		 * Message: The type could not be resolved (PurpleMimeDocument * purple_mime_document_parsen(char * buf, gsize len)).
		 */

		/*
		 * void purple_mime_document_write(PurpleMimeDocument * doc, GString * str)
		 * 
		 * Could not generate a wrapper for purple_mime_document_write in file "mime.h".
		 * Message: The type could not be resolved (PurpleMimeDocument * doc).
		 */

		/*
		 * GList * purple_mime_document_get_fields(PurpleMimeDocument * doc)
		 * 
		 * Could not generate a wrapper for purple_mime_document_get_fields in file "mime.h".
		 * Message: The type could not be resolved (GList * purple_mime_document_get_fields(PurpleMimeDocument * doc)).
		 */

		/*
		 * GList * purple_mime_document_get_parts(PurpleMimeDocument * doc)
		 * 
		 * Could not generate a wrapper for purple_mime_document_get_parts in file "mime.h".
		 * Message: The type could not be resolved (GList * purple_mime_document_get_parts(PurpleMimeDocument * doc)).
		 */

		/*
		 * PurpleMimePart * purple_mime_part_new(PurpleMimeDocument * doc)
		 * 
		 * Could not generate a wrapper for purple_mime_part_new in file "mime.h".
		 * Message: The type could not be resolved (PurpleMimePart * purple_mime_part_new(PurpleMimeDocument * doc)).
		 */

		/*
		 * GList * purple_mime_part_get_fields(PurpleMimePart * part)
		 * 
		 * Could not generate a wrapper for purple_mime_part_get_fields in file "mime.h".
		 * Message: The type could not be resolved (GList * purple_mime_part_get_fields(PurpleMimePart * part)).
		 */

		/*
		 * char * purple_mime_part_get_data(PurpleMimePart * part)
		 * 
		 * Could not generate a wrapper for purple_mime_part_get_data in file "mime.h".
		 * Message: The type could not be resolved (PurpleMimePart * part).
		 */

		/*
		 * gsize purple_mime_part_get_length(PurpleMimePart * part)
		 * 
		 * Could not generate a wrapper for purple_mime_part_get_length in file "mime.h".
		 * Message: The type could not be resolved (PurpleMimePart * part).
		 */

		/*
		 * void purple_mime_part_set_data(PurpleMimePart * part, char * data)
		 * 
		 * Could not generate a wrapper for purple_mime_part_set_data in file "mime.h".
		 * Message: The type could not be resolved (PurpleMimePart * part).
		 */

	}
}


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
	 * File: mime.h
	 */
	public class Mime
	{

		/*
		 * Function Name: purple_mime_document_new
		 * Function Type: PurpleMimeDocument *
		 * Function RawType: PurpleMimeDocument
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleMimeDocument * purple_mime_document_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_document_new();

		public static /* Resolved as PointerToUnknownStruct to PurpleMimeDocument */ IntPtr DocumentNew()
		{
			/* Unable to process the return value of purple_mime_document_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_mime_document_free
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_mime_document_free(PurpleMimeDocument * doc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_mime_document_free(IntPtr doc);

		public static void DocumentFree(/* Resolved as PointerToUnknownStruct to PurpleMimeDocument */ IntPtr doc)
		{
			/* Unable to process doc, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_mime_document_parse
		 * Function Type: PurpleMimeDocument *
		 * Function RawType: PurpleMimeDocument
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleMimeDocument * purple_mime_document_parse(char * buf)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_document_parse(string buf);

		public static /* Resolved as PointerToUnknownStruct to PurpleMimeDocument */ IntPtr DocumentParse(string buf)
		{
			/* Unable to process the return value of purple_mime_document_parse, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_mime_document_parsen
		 * Function Type: PurpleMimeDocument *
		 * Function RawType: PurpleMimeDocument
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleMimeDocument * purple_mime_document_parsen(char * buf, gsize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_document_parsen(string buf, ulong len);

		public static /* Resolved as PointerToUnknownStruct to PurpleMimeDocument */ IntPtr DocumentParsen(string buf, ulong len)
		{
			/* Unable to process the return value of purple_mime_document_parsen, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_mime_document_write
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_mime_document_write(PurpleMimeDocument * doc, GString * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_mime_document_write(IntPtr doc, IntPtr str);

		public static void DocumentWrite(/* Resolved as PointerToUnknownStruct to PurpleMimeDocument */ IntPtr doc, /* Resolved as PointerToUnknownStruct to GString */ IntPtr str)
		{
			/* Unable to process doc, a PointerToUnknownStruct. */
			/* Unable to process str, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_mime_document_get_fields
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_mime_document_get_fields(PurpleMimeDocument * doc)
		 * 
		 * Could not generate a wrapper for purple_mime_document_get_fields in file "mime.h".
		 * Message: Unknown inner-type of argument 0 of purple_mime_document_get_fields
		 */

		/*
		 * Function Name: purple_mime_document_get_parts
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_mime_document_get_parts(PurpleMimeDocument * doc)
		 * 
		 * Could not generate a wrapper for purple_mime_document_get_parts in file "mime.h".
		 * Message: Unknown inner-type of argument 0 of purple_mime_document_get_parts
		 */

		/*
		 * Function Name: purple_mime_part_new
		 * Function Type: PurpleMimePart *
		 * Function RawType: PurpleMimePart
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleMimePart * purple_mime_part_new(PurpleMimeDocument * doc)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_part_new(IntPtr doc);

		public static /* Resolved as PointerToUnknownStruct to PurpleMimePart */ IntPtr PartNew(/* Resolved as PointerToUnknownStruct to PurpleMimeDocument */ IntPtr doc)
		{
			/* Unable to process doc, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_mime_part_get_fields
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_mime_part_get_fields(PurpleMimePart * part)
		 * 
		 * Could not generate a wrapper for purple_mime_part_get_fields in file "mime.h".
		 * Message: Unknown inner-type of argument 0 of purple_mime_part_get_fields
		 */

		/*
		 * Function Name: purple_mime_part_get_data
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_mime_part_get_data(PurpleMimePart * part)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_mime_part_get_data(IntPtr part);

		public static string PartGetData(/* Resolved as PointerToUnknownStruct to PurpleMimePart */ IntPtr part)
		{
			/* Unable to process part, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_mime_part_get_length
		 * Function Type: gsize
		 * Function RawType: gsize
		 * Function Category: Native
		 */
		/*
		 * gsize purple_mime_part_get_length(PurpleMimePart * part)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_mime_part_get_length(IntPtr part);

		public static ulong PartGetLength(/* Resolved as PointerToUnknownStruct to PurpleMimePart */ IntPtr part)
		{
			/* Unable to process part, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_mime_part_set_data
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_mime_part_set_data(PurpleMimePart * part, char * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_mime_part_set_data(IntPtr part, string data);

		public static void PartSetData(/* Resolved as PointerToUnknownStruct to PurpleMimePart */ IntPtr part, string data)
		{
			/* Unable to process part, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


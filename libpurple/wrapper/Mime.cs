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
	public class Mime
	{
		/*
		 * PurpleMimeDocument * purple_mime_document_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_document_new();

		public static PurpleMimeDocument DocumentNew()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_mime_document_free(PurpleMimeDocument * doc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_mime_document_free(IntPtr doc);

		public static void DocumentFree(PurpleMimeDocument doc)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMimeDocument * purple_mime_document_parse(char * buf)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_document_parse(string buf);

		public static PurpleMimeDocument DocumentParse(string buf)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMimeDocument * purple_mime_document_parsen(char * buf, gsize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_document_parsen(string buf, UNKNOWN len);

		public static PurpleMimeDocument DocumentParsen(string buf, gsize len)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_mime_document_write(PurpleMimeDocument * doc, GString * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_mime_document_write(IntPtr doc, IntPtr str);

		public static void DocumentWrite(PurpleMimeDocument doc, GString str)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_mime_document_get_fields(PurpleMimeDocument * doc)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_document_get_fields(IntPtr doc);

		public static GList DocumentGetFields(PurpleMimeDocument doc)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_mime_document_get_field(PurpleMimeDocument * doc, char * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_mime_document_get_field(IntPtr doc, string field);

		public static string DocumentGetField(PurpleMimeDocument doc, string field)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_mime_document_set_field(PurpleMimeDocument * doc, char * field, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_mime_document_set_field(IntPtr doc, string field, string value);

		public static void DocumentSetField(PurpleMimeDocument doc, string field, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_mime_document_get_parts(PurpleMimeDocument * doc)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_document_get_parts(IntPtr doc);

		public static GList DocumentGetParts(PurpleMimeDocument doc)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMimePart * purple_mime_part_new(PurpleMimeDocument * doc)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_part_new(IntPtr doc);

		public static PurpleMimePart PartNew(PurpleMimeDocument doc)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_mime_part_get_fields(PurpleMimePart * part)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mime_part_get_fields(IntPtr part);

		public static GList PartGetFields(PurpleMimePart part)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_mime_part_get_field(PurpleMimePart * part, char * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_mime_part_get_field(IntPtr part, string field);

		public static string PartGetField(PurpleMimePart part, string field)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_mime_part_get_field_decoded(PurpleMimePart * part, char * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_mime_part_get_field_decoded(IntPtr part, string field);

		public static string PartGetFieldDecoded(PurpleMimePart part, string field)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_mime_part_set_field(PurpleMimePart * part, char * field, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_mime_part_set_field(IntPtr part, string field, string value);

		public static void PartSetField(PurpleMimePart part, string field, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_mime_part_get_data(PurpleMimePart * part)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_mime_part_get_data(IntPtr part);

		public static string PartGetData(PurpleMimePart part)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_mime_part_get_data_decoded(PurpleMimePart * part, guchar ** data, gsize * len)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_mime_part_get_data_decoded(IntPtr part, IntPtr data, IntPtr len);

		public static void PartGetDataDecoded(PurpleMimePart part, guchar data, gsize len)
		{
			throw new NotImplementedException();
		}

		/*
		 * gsize purple_mime_part_get_length(PurpleMimePart * part)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_mime_part_get_length(IntPtr part);

		public static gsize PartGetLength(PurpleMimePart part)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_mime_part_set_data(PurpleMimePart * part, char * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_mime_part_set_data(IntPtr part, string data);

		public static void PartSetData(PurpleMimePart part, string data)
		{
			throw new NotImplementedException();
		}

	}
}


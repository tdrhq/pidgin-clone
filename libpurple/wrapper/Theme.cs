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
	 * File: theme.h
	 */
	public class Theme
	{

		/*
		 * Function Name: purple_theme_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_theme_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_theme_get_type();

		public static /* libobject */ IntPtr GetType()
		{
			/* Unable to process the return value of purple_theme_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_theme_get_name
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_theme_get_name(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_name(IntPtr theme);

		public static string GetName(PurpleTheme theme)
		{
			return purple_theme_get_name(theme.Reference);
		}

		/*
		 * Function Name: purple_theme_set_name
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_theme_set_name(PurpleTheme * theme, gchar * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_set_name(IntPtr theme, string name);

		public static void SetName(PurpleTheme theme, string name)
		{
			purple_theme_set_name(theme.Reference, name);
		}

		/*
		 * Function Name: purple_theme_get_description
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_theme_get_description(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_description(IntPtr theme);

		public static string GetDescription(PurpleTheme theme)
		{
			return purple_theme_get_description(theme.Reference);
		}

		/*
		 * Function Name: purple_theme_set_description
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_theme_set_description(PurpleTheme * theme, gchar * description)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_set_description(IntPtr theme, string description);

		public static void SetDescription(PurpleTheme theme, string description)
		{
			purple_theme_set_description(theme.Reference, description);
		}

		/*
		 * Function Name: purple_theme_get_author
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_theme_get_author(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_author(IntPtr theme);

		public static string GetAuthor(PurpleTheme theme)
		{
			return purple_theme_get_author(theme.Reference);
		}

		/*
		 * Function Name: purple_theme_set_author
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_theme_set_author(PurpleTheme * theme, gchar * author)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_set_author(IntPtr theme, string author);

		public static void SetAuthor(PurpleTheme theme, string author)
		{
			purple_theme_set_author(theme.Reference, author);
		}

		/*
		 * Function Name: purple_theme_get_type_string
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_theme_get_type_string(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_type_string(IntPtr theme);

		public static string GetTypeString(PurpleTheme theme)
		{
			return purple_theme_get_type_string(theme.Reference);
		}

		/*
		 * Function Name: purple_theme_get_dir
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_theme_get_dir(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_dir(IntPtr theme);

		public static string GetDir(PurpleTheme theme)
		{
			return purple_theme_get_dir(theme.Reference);
		}

		/*
		 * Function Name: purple_theme_set_dir
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_theme_set_dir(PurpleTheme * theme, gchar * dir)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_set_dir(IntPtr theme, string dir);

		public static void SetDir(PurpleTheme theme, string dir)
		{
			purple_theme_set_dir(theme.Reference, dir);
		}

		/*
		 * Function Name: purple_theme_get_image
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_theme_get_image(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_image(IntPtr theme);

		public static string GetImage(PurpleTheme theme)
		{
			return purple_theme_get_image(theme.Reference);
		}

		/*
		 * Function Name: purple_theme_get_image_full
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_theme_get_image_full(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_image_full(IntPtr theme);

		public static string GetImageFull(PurpleTheme theme)
		{
			return purple_theme_get_image_full(theme.Reference);
		}

		/*
		 * Function Name: purple_theme_set_image
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_theme_set_image(PurpleTheme * theme, gchar * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_set_image(IntPtr theme, string img);

		public static void SetImage(PurpleTheme theme, string img)
		{
			purple_theme_set_image(theme.Reference, img);
		}

	}
}


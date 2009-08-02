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
	 * File: pluginpref.h
	 */
	public class Pluginpref
	{
		public enum PurpleStringFormatType
		{
			PURPLE_STRING_FORMAT_TYPE_NONE = 0,
			PURPLE_STRING_FORMAT_TYPE_MULTILINE = 1 << 0,
			PURPLE_STRING_FORMAT_TYPE_HTML = 1 << 1
		};

		public enum PurplePluginPrefType
		{
			PURPLE_PLUGIN_PREF_NONE,
			PURPLE_PLUGIN_PREF_CHOICE,
			PURPLE_PLUGIN_PREF_INFO,
			PURPLE_PLUGIN_PREF_STRING_FORMAT
		};


		/*
		 * Function Name: purple_plugin_pref_frame_new
		 * Function Type: PurplePluginPrefFrame *
		 * Function RawType: PurplePluginPrefFrame
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePluginPrefFrame * purple_plugin_pref_frame_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_frame_new();

		public static /* Resolved as PointerToUnknownStruct to PurplePluginPrefFrame */ IntPtr PluginPrefFrameNew()
		{
			/* Unable to process the return value of purple_plugin_pref_frame_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_frame_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_frame_destroy(PurplePluginPrefFrame * frame)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_frame_destroy(IntPtr frame);

		public static void PluginPrefFrameDestroy(/* Resolved as PointerToUnknownStruct to PurplePluginPrefFrame */ IntPtr frame)
		{
			/* Unable to process frame, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_frame_add
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_frame_add(PurplePluginPrefFrame * frame, PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_frame_add(IntPtr frame, IntPtr pref);

		public static void PluginPrefFrameAdd(/* Resolved as PointerToUnknownStruct to PurplePluginPrefFrame */ IntPtr frame, /* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref)
		{
			/* Unable to process frame, a PointerToUnknownStruct. */
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_frame_get_prefs
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_plugin_pref_frame_get_prefs(PurplePluginPrefFrame * frame)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_frame_get_prefs in file "pluginpref.h".
		 * Message: Unknown inner-type of argument 0 of purple_plugin_pref_frame_get_prefs
		 */

		/*
		 * Function Name: purple_plugin_pref_new
		 * Function Type: PurplePluginPref *
		 * Function RawType: PurplePluginPref
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePluginPref * purple_plugin_pref_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_new();

		public static /* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr PluginPrefNew()
		{
			/* Unable to process the return value of purple_plugin_pref_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_new_with_name
		 * Function Type: PurplePluginPref *
		 * Function RawType: PurplePluginPref
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePluginPref * purple_plugin_pref_new_with_name(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_new_with_name(string name);

		public static /* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr PluginPrefNewWithName(string name)
		{
			/* Unable to process the return value of purple_plugin_pref_new_with_name, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_new_with_label
		 * Function Type: PurplePluginPref *
		 * Function RawType: PurplePluginPref
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePluginPref * purple_plugin_pref_new_with_label(char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_new_with_label(string label);

		public static /* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr PluginPrefNewWithLabel(string label)
		{
			/* Unable to process the return value of purple_plugin_pref_new_with_label, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_new_with_name_and_label
		 * Function Type: PurplePluginPref *
		 * Function RawType: PurplePluginPref
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePluginPref * purple_plugin_pref_new_with_name_and_label(char * name, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_new_with_name_and_label(string name, string label);

		public static /* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr PluginPrefNewWithNameAndLabel(string name, string label)
		{
			/* Unable to process the return value of purple_plugin_pref_new_with_name_and_label, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_destroy(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_destroy(IntPtr pref);

		public static void PluginPrefDestroy(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_set_name
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_set_name(PurplePluginPref * pref, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_name(IntPtr pref, string name);

		public static void PluginPrefSetName(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref, string name)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_get_name
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_plugin_pref_get_name(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_pref_get_name(IntPtr pref);

		public static string PluginPrefGetName(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_set_label
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_set_label(PurplePluginPref * pref, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_label(IntPtr pref, string label);

		public static void PluginPrefSetLabel(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref, string label)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_get_label
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_plugin_pref_get_label(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_pref_get_label(IntPtr pref);

		public static string PluginPrefGetLabel(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_set_bounds
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_set_bounds(PurplePluginPref * pref, int min, int max)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_bounds(IntPtr pref, int min, int max);

		public static void PluginPrefSetBounds(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref, int min, int max)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_get_bounds
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_get_bounds(PurplePluginPref * pref, int * min, int * max)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_get_bounds(IntPtr pref, IntPtr min, IntPtr max);

		public static void PluginPrefGetBounds(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref, /* Resolved as PointerToUnknownStruct to int */ IntPtr min, /* Resolved as PointerToUnknownStruct to int */ IntPtr max)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			/* Unable to process min, a PointerToUnknownStruct. */
			/* Unable to process max, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_set_type
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_set_type(PurplePluginPref * pref, PurplePluginPrefType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_type(IntPtr pref, Pluginpref.PurplePluginPrefType type);

		public static void PluginPrefSetType(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref, Pluginpref.PurplePluginPrefType type)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_get_type
		 * Function Type: PurplePluginPrefType
		 * Function RawType: PurplePluginPrefType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurplePluginPrefType purple_plugin_pref_get_type(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern Pluginpref.PurplePluginPrefType purple_plugin_pref_get_type(IntPtr pref);

		public static Pluginpref.PurplePluginPrefType PluginPrefGetType(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_add_choice
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_add_choice(PurplePluginPref * pref, char * label, gpointer choice)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_add_choice(IntPtr pref, string label, IntPtr choice);

		public static void PluginPrefAddChoice(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref, string label, IntPtr choice)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_get_choices
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_plugin_pref_get_choices(PurplePluginPref * pref)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_get_choices in file "pluginpref.h".
		 * Message: Unknown inner-type of argument 0 of purple_plugin_pref_get_choices
		 */

		/*
		 * Function Name: purple_plugin_pref_set_max_length
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_set_max_length(PurplePluginPref * pref, unsigned int max_length)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_max_length(IntPtr pref, uint max_length);

		public static void PluginPrefSetMaxLength(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref, uint max_length)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_get_max_length
		 * Function Type: unsigned int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * unsigned int purple_plugin_pref_get_max_length(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_plugin_pref_get_max_length(IntPtr pref);

		public static uint PluginPrefGetMaxLength(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_set_masked
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_set_masked(PurplePluginPref * pref, gboolean mask)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_masked(IntPtr pref, bool mask);

		public static void PluginPrefSetMasked(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref, bool mask)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_get_masked
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_plugin_pref_get_masked(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_pref_get_masked(IntPtr pref);

		public static bool PluginPrefGetMasked(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_set_format_type
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_plugin_pref_set_format_type(PurplePluginPref * pref, PurpleStringFormatType format)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_format_type(IntPtr pref, Pluginpref.PurpleStringFormatType format);

		public static void PluginPrefSetFormatType(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref, Pluginpref.PurpleStringFormatType format)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			/* Unable to process format, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_plugin_pref_get_format_type
		 * Function Type: PurpleStringFormatType
		 * Function RawType: PurpleStringFormatType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleStringFormatType purple_plugin_pref_get_format_type(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern Pluginpref.PurpleStringFormatType purple_plugin_pref_get_format_type(IntPtr pref);

		public static Pluginpref.PurpleStringFormatType PluginPrefGetFormatType(/* Resolved as PointerToUnknownStruct to PurplePluginPref */ IntPtr pref)
		{
			/* Unable to process pref, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


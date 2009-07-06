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
		 * PurplePluginPrefFrame * purple_plugin_pref_frame_new()
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_frame_new in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPrefFrame * purple_plugin_pref_frame_new()).
		 */

		/*
		 * void purple_plugin_pref_frame_destroy(PurplePluginPrefFrame * frame)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_frame_destroy in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPrefFrame * frame).
		 */

		/*
		 * void purple_plugin_pref_frame_add(PurplePluginPrefFrame * frame, PurplePluginPref * pref)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_frame_add in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPrefFrame * frame).
		 */

		/*
		 * GList * purple_plugin_pref_frame_get_prefs(PurplePluginPrefFrame * frame)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_frame_get_prefs in file "pluginpref.h".
		 * Message: The type could not be resolved (GList * purple_plugin_pref_frame_get_prefs(PurplePluginPrefFrame * frame)).
		 */

		/*
		 * PurplePluginPref * purple_plugin_pref_new()
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_new in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * purple_plugin_pref_new()).
		 */

		/*
		 * PurplePluginPref * purple_plugin_pref_new_with_name(char * name)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_new_with_name in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * purple_plugin_pref_new_with_name(char * name)).
		 */

		/*
		 * PurplePluginPref * purple_plugin_pref_new_with_label(char * label)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_new_with_label in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * purple_plugin_pref_new_with_label(char * label)).
		 */

		/*
		 * PurplePluginPref * purple_plugin_pref_new_with_name_and_label(char * name, char * label)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_new_with_name_and_label in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * purple_plugin_pref_new_with_name_and_label(char * name, char * label)).
		 */

		/*
		 * void purple_plugin_pref_destroy(PurplePluginPref * pref)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_destroy in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * void purple_plugin_pref_set_name(PurplePluginPref * pref, char * name)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_set_name in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * char * purple_plugin_pref_get_name(PurplePluginPref * pref)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_get_name in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * void purple_plugin_pref_set_label(PurplePluginPref * pref, char * label)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_set_label in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * char * purple_plugin_pref_get_label(PurplePluginPref * pref)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_get_label in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * void purple_plugin_pref_set_bounds(PurplePluginPref * pref, int min, int max)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_set_bounds in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * void purple_plugin_pref_get_bounds(PurplePluginPref * pref, int * min, int * max)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_get_bounds in file "pluginpref.h".
		 * Message: The type could not be resolved (int * min).
		 */

		/*
		 * void purple_plugin_pref_set_type(PurplePluginPref * pref, PurplePluginPrefType type)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_set_type in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * PurplePluginPrefType purple_plugin_pref_get_type(PurplePluginPref * pref)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_get_type in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * void purple_plugin_pref_add_choice(PurplePluginPref * pref, char * label, gpointer choice)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_add_choice in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * GList * purple_plugin_pref_get_choices(PurplePluginPref * pref)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_get_choices in file "pluginpref.h".
		 * Message: The type could not be resolved (GList * purple_plugin_pref_get_choices(PurplePluginPref * pref)).
		 */

		/*
		 * void purple_plugin_pref_set_max_length(PurplePluginPref * pref, unsigned int max_length)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_set_max_length in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * unsigned int purple_plugin_pref_get_max_length(PurplePluginPref * pref)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_get_max_length in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * void purple_plugin_pref_set_masked(PurplePluginPref * pref, gboolean mask)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_set_masked in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * gboolean purple_plugin_pref_get_masked(PurplePluginPref * pref)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_get_masked in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * void purple_plugin_pref_set_format_type(PurplePluginPref * pref, PurpleStringFormatType format)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_set_format_type in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

		/*
		 * PurpleStringFormatType purple_plugin_pref_get_format_type(PurplePluginPref * pref)
		 * 
		 * Could not generate a wrapper for purple_plugin_pref_get_format_type in file "pluginpref.h".
		 * Message: The type could not be resolved (PurplePluginPref * pref).
		 */

	}
}


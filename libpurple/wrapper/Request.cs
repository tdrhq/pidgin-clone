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
	public class Request
	{
		public enum PurpleRequestType
		{
			PURPLE_REQUEST_INPUT = 0,
			PURPLE_REQUEST_CHOICE,
			PURPLE_REQUEST_ACTION,
			PURPLE_REQUEST_FIELDS,
			PURPLE_REQUEST_FILE,
			PURPLE_REQUEST_FOLDER
		};

		public enum PurpleRequestFieldType
		{
			PURPLE_REQUEST_FIELD_NONE,
			PURPLE_REQUEST_FIELD_STRING,
			PURPLE_REQUEST_FIELD_INTEGER,
			PURPLE_REQUEST_FIELD_BOOLEAN,
			PURPLE_REQUEST_FIELD_CHOICE,
			PURPLE_REQUEST_FIELD_LIST,
			PURPLE_REQUEST_FIELD_LABEL,
			PURPLE_REQUEST_FIELD_IMAGE,
			PURPLE_REQUEST_FIELD_ACCOUNT
		};

		/*
		 * PurpleRequestFields * purple_request_fields_new()
		 * 
		 * Could not generate a wrapper for purple_request_fields_new in file "request.h".
		 * Message: The type could not be resolved (PurpleRequestFields * purple_request_fields_new()).
		 */

		/*
		 * void purple_request_fields_destroy(PurpleRequestFields * fields)
		 * 
		 * Could not generate a wrapper for purple_request_fields_destroy in file "request.h".
		 * Message: The type could not be resolved (PurpleRequestFields * fields).
		 */

		/*
		 * GList * purple_request_fields_get_groups(PurpleRequestFields * fields)
		 * 
		 * Could not generate a wrapper for purple_request_fields_get_groups in file "request.h".
		 * Message: The type could not be resolved (GList * purple_request_fields_get_groups(PurpleRequestFields * fields)).
		 */

		/*
		 * GList * purple_request_fields_get_required(PurpleRequestFields * fields)
		 * 
		 * Could not generate a wrapper for purple_request_fields_get_required in file "request.h".
		 * Message: The type could not be resolved (GList * purple_request_fields_get_required(PurpleRequestFields * fields)).
		 */

		/*
		 * PurpleRequestFieldGroup * purple_request_field_group_new(char * title)
		 * 
		 * Could not generate a wrapper for purple_request_field_group_new in file "request.h".
		 * Message: The type could not be resolved (PurpleRequestFieldGroup * purple_request_field_group_new(char * title)).
		 */

		/*
		 * void purple_request_field_group_destroy(PurpleRequestFieldGroup * group)
		 * 
		 * Could not generate a wrapper for purple_request_field_group_destroy in file "request.h".
		 * Message: The type could not be resolved (PurpleRequestFieldGroup * group).
		 */

		/*
		 * void purple_request_field_destroy(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_destroy(IntPtr field);

		public static void FieldDestroy(PurpleRequestField field)
		{
			purple_request_field_destroy(field.Reference);
		}

		/*
		 * void purple_request_field_set_label(PurpleRequestField * field, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_set_label(IntPtr field, string label);

		public static void FieldSetLabel(PurpleRequestField field, string label)
		{
			purple_request_field_set_label(field.Reference, label);
		}

		/*
		 * void purple_request_field_set_visible(PurpleRequestField * field, gboolean visible)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_set_visible(IntPtr field, bool visible);

		public static void FieldSetVisible(PurpleRequestField field, bool visible)
		{
			purple_request_field_set_visible(field.Reference, visible);
		}

		/*
		 * PurpleRequestFieldType purple_request_field_get_type(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern Request.PurpleRequestFieldType purple_request_field_get_type(IntPtr field);

		public static Request.PurpleRequestFieldType FieldGetType(PurpleRequestField field)
		{
			/* Unable to process purple_request_field_get_type, a KnownEnum. */
			
		}

		/*
		 * PurpleRequestFieldGroup * purple_request_field_get_group(PurpleRequestField * field)
		 * 
		 * Could not generate a wrapper for purple_request_field_get_group in file "request.h".
		 * Message: The type could not be resolved (PurpleRequestFieldGroup * purple_request_field_get_group(PurpleRequestField * field)).
		 */

		/*
		 * char * purple_request_field_get_id(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_get_id(IntPtr field);

		public static string FieldGetId(PurpleRequestField field)
		{
			return purple_request_field_get_id(field.Reference);
		}

		/*
		 * char * purple_request_field_get_label(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_get_label(IntPtr field);

		public static string FieldGetLabel(PurpleRequestField field)
		{
			return purple_request_field_get_label(field.Reference);
		}

		/*
		 * gboolean purple_request_field_is_visible(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_is_visible(IntPtr field);

		public static bool FieldIsVisible(PurpleRequestField field)
		{
			return purple_request_field_is_visible(field.Reference);
		}

		/*
		 * char * purple_request_field_get_type_hint(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_get_type_hint(IntPtr field);

		public static string FieldGetTypeHint(PurpleRequestField field)
		{
			return purple_request_field_get_type_hint(field.Reference);
		}

		/*
		 * gboolean purple_request_field_is_required(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_is_required(IntPtr field);

		public static bool FieldIsRequired(PurpleRequestField field)
		{
			return purple_request_field_is_required(field.Reference);
		}

		/*
		 * gpointer purple_request_field_get_ui_data(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_get_ui_data(IntPtr field);

		public static IntPtr FieldGetUiData(PurpleRequestField field)
		{
			return purple_request_field_get_ui_data(field.Reference);
		}

		/*
		 * char * purple_request_field_string_get_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_string_get_value(IntPtr field);

		public static string FieldStringGetValue(PurpleRequestField field)
		{
			return purple_request_field_string_get_value(field.Reference);
		}

		/*
		 * gboolean purple_request_field_string_is_multiline(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_string_is_multiline(IntPtr field);

		public static bool FieldStringIsMultiline(PurpleRequestField field)
		{
			return purple_request_field_string_is_multiline(field.Reference);
		}

		/*
		 * gboolean purple_request_field_string_is_masked(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_string_is_masked(IntPtr field);

		public static bool FieldStringIsMasked(PurpleRequestField field)
		{
			return purple_request_field_string_is_masked(field.Reference);
		}

		/*
		 * gboolean purple_request_field_string_is_editable(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_string_is_editable(IntPtr field);

		public static bool FieldStringIsEditable(PurpleRequestField field)
		{
			return purple_request_field_string_is_editable(field.Reference);
		}

		/*
		 * void purple_request_field_int_set_value(PurpleRequestField * field, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_int_set_value(IntPtr field, int value);

		public static void FieldIntSetValue(PurpleRequestField field, int value)
		{
			purple_request_field_int_set_value(field.Reference, value);
		}

		/*
		 * int purple_request_field_int_get_default_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_field_int_get_default_value(IntPtr field);

		public static int FieldIntGetDefaultValue(PurpleRequestField field)
		{
			return purple_request_field_int_get_default_value(field.Reference);
		}

		/*
		 * int purple_request_field_int_get_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_field_int_get_value(IntPtr field);

		public static int FieldIntGetValue(PurpleRequestField field)
		{
			return purple_request_field_int_get_value(field.Reference);
		}

		/*
		 * gboolean purple_request_field_bool_get_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_bool_get_value(IntPtr field);

		public static bool FieldBoolGetValue(PurpleRequestField field)
		{
			return purple_request_field_bool_get_value(field.Reference);
		}

		/*
		 * void purple_request_field_choice_set_value(PurpleRequestField * field, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_choice_set_value(IntPtr field, int value);

		public static void FieldChoiceSetValue(PurpleRequestField field, int value)
		{
			purple_request_field_choice_set_value(field.Reference, value);
		}

		/*
		 * int purple_request_field_choice_get_default_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_field_choice_get_default_value(IntPtr field);

		public static int FieldChoiceGetDefaultValue(PurpleRequestField field)
		{
			return purple_request_field_choice_get_default_value(field.Reference);
		}

		/*
		 * int purple_request_field_choice_get_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_field_choice_get_value(IntPtr field);

		public static int FieldChoiceGetValue(PurpleRequestField field)
		{
			return purple_request_field_choice_get_value(field.Reference);
		}

		/*
		 * GList * purple_request_field_choice_get_labels(PurpleRequestField * field)
		 * 
		 * Could not generate a wrapper for purple_request_field_choice_get_labels in file "request.h".
		 * Message: The type could not be resolved (GList * purple_request_field_choice_get_labels(PurpleRequestField * field)).
		 */

		/*
		 * PurpleRequestField * purple_request_field_list_new(char * id, char * text)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_list_new(string id, string text);

		public static PurpleRequestField FieldListNew(string id, string text)
		{
			return new PurpleRequestField(purple_request_field_list_new(id, text));
		}

		/*
		 * void purple_request_field_list_clear_selected(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_list_clear_selected(IntPtr field);

		public static void FieldListClearSelected(PurpleRequestField field)
		{
			purple_request_field_list_clear_selected(field.Reference);
		}

		/*
		 * GList * purple_request_field_list_get_items(PurpleRequestField * field)
		 * 
		 * Could not generate a wrapper for purple_request_field_list_get_items in file "request.h".
		 * Message: The type could not be resolved (GList * purple_request_field_list_get_items(PurpleRequestField * field)).
		 */

		/*
		 * void purple_request_field_image_set_scale(PurpleRequestField * field, unsigned int x, unsigned int y)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_image_set_scale(IntPtr field, uint x, uint y);

		public static void FieldImageSetScale(PurpleRequestField field, uint x, uint y)
		{
			purple_request_field_image_set_scale(field.Reference, x, y);
		}

		/*
		 * char * purple_request_field_image_get_buffer(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_image_get_buffer(IntPtr field);

		public static string FieldImageGetBuffer(PurpleRequestField field)
		{
			return purple_request_field_image_get_buffer(field.Reference);
		}

		/*
		 * gsize purple_request_field_image_get_size(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_request_field_image_get_size(IntPtr field);

		public static ulong FieldImageGetSize(PurpleRequestField field)
		{
			return purple_request_field_image_get_size(field.Reference);
		}

		/*
		 * unsigned int purple_request_field_image_get_scale_x(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_request_field_image_get_scale_x(IntPtr field);

		public static uint FieldImageGetScaleX(PurpleRequestField field)
		{
			return purple_request_field_image_get_scale_x(field.Reference);
		}

		/*
		 * unsigned int purple_request_field_image_get_scale_y(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_request_field_image_get_scale_y(IntPtr field);

		public static uint FieldImageGetScaleY(PurpleRequestField field)
		{
			return purple_request_field_image_get_scale_y(field.Reference);
		}

		/*
		 * void purple_request_close(PurpleRequestType type, void * uihandle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_close(Request.PurpleRequestType type, IntPtr uihandle);

		public static void Close(Request.PurpleRequestType type, IntPtr uihandle)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_close_with_handle(void * handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_close_with_handle(IntPtr handle);

		public static void CloseWithHandle(IntPtr handle)
		{
			purple_request_close_with_handle(handle);
		}

		/*
		 * void purple_request_set_ui_ops(PurpleRequestUiOps * ops)
		 * 
		 * Could not generate a wrapper for purple_request_set_ui_ops in file "request.h".
		 * Message: The type could not be resolved (PurpleRequestUiOps * ops).
		 */

		/*
		 * PurpleRequestUiOps * purple_request_get_ui_ops()
		 * 
		 * Could not generate a wrapper for purple_request_get_ui_ops in file "request.h".
		 * Message: The type could not be resolved (PurpleRequestUiOps * purple_request_get_ui_ops()).
		 */

	}
}


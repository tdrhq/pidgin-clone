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
	 * File: request.h
	 */
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

		public delegate void PurpleRequestInputCb(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);
		public delegate void PurpleRequestActionCb(IntPtr _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1);
		public delegate void PurpleRequestChoiceCb(IntPtr _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1);
		public delegate void PurpleRequestFieldsCb(IntPtr _PurpleWrapper_arg0, IntPtr fields);
		public delegate void PurpleRequestFileCb(IntPtr _PurpleWrapper_arg0, string filename);

		/*
		 * Function Name: purple_request_fields_new
		 * Function Type: PurpleRequestFields *
		 * Function RawType: PurpleRequestFields
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleRequestFields * purple_request_fields_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_fields_new();

		public static /* Resolved as PointerToUnknownStruct to PurpleRequestFields */ IntPtr FieldsNew()
		{
			/* Unable to process the return value of purple_request_fields_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_request_fields_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_request_fields_destroy(PurpleRequestFields * fields)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_fields_destroy(IntPtr fields);

		public static void FieldsDestroy(/* Resolved as PointerToUnknownStruct to PurpleRequestFields */ IntPtr fields)
		{
			/* Unable to process fields, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_request_fields_get_groups
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_request_fields_get_groups(PurpleRequestFields * fields)
		 * 
		 * Could not generate a wrapper for purple_request_fields_get_groups in file "request.h".
		 * Message: Unknown inner-type of argument 0 of purple_request_fields_get_groups
		 */

		/*
		 * Function Name: purple_request_fields_get_required
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_request_fields_get_required(PurpleRequestFields * fields)
		 * 
		 * Could not generate a wrapper for purple_request_fields_get_required in file "request.h".
		 * Message: Unknown inner-type of argument 0 of purple_request_fields_get_required
		 */

		/*
		 * Function Name: purple_request_field_group_new
		 * Function Type: PurpleRequestFieldGroup *
		 * Function RawType: PurpleRequestFieldGroup
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleRequestFieldGroup * purple_request_field_group_new(char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_group_new(string title);

		public static /* Resolved as PointerToUnknownStruct to PurpleRequestFieldGroup */ IntPtr FieldGroupNew(string title)
		{
			/* Unable to process the return value of purple_request_field_group_new, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_request_field_group_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_request_field_group_destroy(PurpleRequestFieldGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_group_destroy(IntPtr group);

		public static void FieldGroupDestroy(/* Resolved as PointerToUnknownStruct to PurpleRequestFieldGroup */ IntPtr group)
		{
			/* Unable to process group, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_request_field_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
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
		 * Function Name: purple_request_field_set_label
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_request_field_set_visible
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_request_field_get_type
		 * Function Type: PurpleRequestFieldType
		 * Function RawType: PurpleRequestFieldType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleRequestFieldType purple_request_field_get_type(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern Request.PurpleRequestFieldType purple_request_field_get_type(IntPtr field);

		public static Request.PurpleRequestFieldType FieldGetType(PurpleRequestField field)
		{
			/* Unable to process the return value of purple_request_field_get_type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_request_field_get_group
		 * Function Type: PurpleRequestFieldGroup *
		 * Function RawType: PurpleRequestFieldGroup
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleRequestFieldGroup * purple_request_field_get_group(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_get_group(IntPtr field);

		public static /* Resolved as PointerToUnknownStruct to PurpleRequestFieldGroup */ IntPtr FieldGetGroup(PurpleRequestField field)
		{
			/* Unable to process the return value of purple_request_field_get_group, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_request_field_get_id
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
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
		 * Function Name: purple_request_field_get_label
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_is_visible
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_get_type_hint
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_is_required
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_get_ui_data
		 * Function Type: gpointer
		 * Function RawType: gpointer
		 * Function Category: VoidPointer
		 */
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
		 * Function Name: purple_request_field_string_get_value
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_string_is_multiline
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_string_is_masked
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_string_is_editable
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_int_set_value
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_request_field_int_get_default_value
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_int_get_value
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_bool_get_value
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_choice_set_value
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_request_field_choice_get_default_value
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_choice_get_value
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_choice_get_labels
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_request_field_choice_get_labels(PurpleRequestField * field)
		 * 
		 * Could not generate a wrapper for purple_request_field_choice_get_labels in file "request.h".
		 * Message: Unknown inner-type of argument 0 of purple_request_field_choice_get_labels
		 */

		/*
		 * Function Name: purple_request_field_list_new
		 * Function Type: PurpleRequestField *
		 * Function RawType: PurpleRequestField
		 * Function Category: PointerToKnownStruct
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
		 * Function Name: purple_request_field_list_clear_selected
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_request_field_list_get_items
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_request_field_list_get_items(PurpleRequestField * field)
		 * 
		 * Could not generate a wrapper for purple_request_field_list_get_items in file "request.h".
		 * Message: Unknown inner-type of argument 0 of purple_request_field_list_get_items
		 */

		/*
		 * Function Name: purple_request_field_image_set_scale
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
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
		 * Function Name: purple_request_field_image_get_buffer
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_image_get_size
		 * Function Type: gsize
		 * Function RawType: gsize
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_image_get_scale_x
		 * Function Type: unsigned int
		 * Function RawType: int
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_field_image_get_scale_y
		 * Function Type: unsigned int
		 * Function RawType: int
		 * Function Category: Native
		 */
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
		 * Function Name: purple_request_close
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_request_close_with_handle
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_request_set_ui_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_request_set_ui_ops(PurpleRequestUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_set_ui_ops(IntPtr ops);

		public static void SetUiOps(/* Resolved as PointerToUnknownStruct to PurpleRequestUiOps */ IntPtr ops)
		{
			/* Unable to process ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_request_get_ui_ops
		 * Function Type: PurpleRequestUiOps *
		 * Function RawType: PurpleRequestUiOps
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleRequestUiOps * purple_request_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_get_ui_ops();

		public static /* Resolved as PointerToUnknownStruct to PurpleRequestUiOps */ IntPtr GetUiOps()
		{
			/* Unable to process the return value of purple_request_get_ui_ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


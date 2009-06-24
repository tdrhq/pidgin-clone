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
	public class Request
	{
		/*
		 * PurpleRequestFields * purple_request_fields_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_fields_new();

		public static PurpleRequestFields FieldsNew()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_fields_destroy(PurpleRequestFields * fields)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_fields_destroy(IntPtr fields);

		public static void FieldsDestroy(PurpleRequestFields fields)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_fields_add_group(PurpleRequestFields * fields, PurpleRequestFieldGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_fields_add_group(IntPtr fields, IntPtr group);

		public static void FieldsAddGroup(PurpleRequestFields fields, PurpleRequestFieldGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_request_fields_get_groups(PurpleRequestFields * fields)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_fields_get_groups(IntPtr fields);

		public static GList FieldsGetGroups(PurpleRequestFields fields)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_fields_exists(PurpleRequestFields * fields, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_fields_exists(IntPtr fields, string id);

		public static bool FieldsExists(PurpleRequestFields fields, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_request_fields_get_required(PurpleRequestFields * fields)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_fields_get_required(IntPtr fields);

		public static GList FieldsGetRequired(PurpleRequestFields fields)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_fields_is_field_required(PurpleRequestFields * fields, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_fields_is_field_required(IntPtr fields, string id);

		public static bool FieldsIsFieldRequired(PurpleRequestFields fields, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_fields_all_required_filled(PurpleRequestFields * fields)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_fields_all_required_filled(IntPtr fields);

		public static bool FieldsAllRequiredFilled(PurpleRequestFields fields)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestField * purple_request_fields_get_field(PurpleRequestFields * fields, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_fields_get_field(IntPtr fields, string id);

		public static PurpleRequestField FieldsGetField(PurpleRequestFields fields, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_request_fields_get_string(PurpleRequestFields * fields, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_fields_get_string(IntPtr fields, string id);

		public static string FieldsGetString(PurpleRequestFields fields, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_request_fields_get_integer(PurpleRequestFields * fields, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_fields_get_integer(IntPtr fields, string id);

		public static int FieldsGetInteger(PurpleRequestFields fields, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_fields_get_bool(PurpleRequestFields * fields, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_fields_get_bool(IntPtr fields, string id);

		public static bool FieldsGetBool(PurpleRequestFields fields, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_request_fields_get_choice(PurpleRequestFields * fields, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_fields_get_choice(IntPtr fields, string id);

		public static int FieldsGetChoice(PurpleRequestFields fields, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_request_fields_get_account(PurpleRequestFields * fields, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_fields_get_account(IntPtr fields, string id);

		public static PurpleAccount FieldsGetAccount(PurpleRequestFields fields, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestFieldGroup * purple_request_field_group_new(char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_group_new(string title);

		public static PurpleRequestFieldGroup FieldGroupNew(string title)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_group_destroy(PurpleRequestFieldGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_group_destroy(IntPtr group);

		public static void FieldGroupDestroy(PurpleRequestFieldGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_group_add_field(PurpleRequestFieldGroup * group, PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_group_add_field(IntPtr group, IntPtr field);

		public static void FieldGroupAddField(PurpleRequestFieldGroup group, PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_request_field_group_get_title(PurpleRequestFieldGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_group_get_title(IntPtr group);

		public static string FieldGroupGetTitle(PurpleRequestFieldGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_request_field_group_get_fields(PurpleRequestFieldGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_group_get_fields(IntPtr group);

		public static GList FieldGroupGetFields(PurpleRequestFieldGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestField * purple_request_field_new(char * id, char * text, PurpleRequestFieldType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_new(string id, string text, UNKNOWN type);

		public static PurpleRequestField FieldNew(string id, string text, PurpleRequestFieldType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_destroy(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_destroy(IntPtr field);

		public static void FieldDestroy(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_set_label(PurpleRequestField * field, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_set_label(IntPtr field, string label);

		public static void FieldSetLabel(PurpleRequestField field, string label)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_set_visible(PurpleRequestField * field, gboolean visible)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_set_visible(IntPtr field, bool visible);

		public static void FieldSetVisible(PurpleRequestField field, bool visible)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_set_type_hint(PurpleRequestField * field, char * type_hint)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_set_type_hint(IntPtr field, string type_hint);

		public static void FieldSetTypeHint(PurpleRequestField field, string type_hint)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_set_required(PurpleRequestField * field, gboolean required)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_set_required(IntPtr field, bool required);

		public static void FieldSetRequired(PurpleRequestField field, bool required)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestFieldType purple_request_field_get_type(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_request_field_get_type(IntPtr field);

		public static PurpleRequestFieldType FieldGetType(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestFieldGroup * purple_request_field_get_group(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_get_group(IntPtr field);

		public static PurpleRequestFieldGroup FieldGetGroup(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_request_field_get_id(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_get_id(IntPtr field);

		public static string FieldGetId(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_request_field_get_label(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_get_label(IntPtr field);

		public static string FieldGetLabel(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_field_is_visible(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_is_visible(IntPtr field);

		public static bool FieldIsVisible(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_request_field_get_type_hint(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_get_type_hint(IntPtr field);

		public static string FieldGetTypeHint(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_field_is_required(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_is_required(IntPtr field);

		public static bool FieldIsRequired(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_request_field_get_ui_data(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_get_ui_data(IntPtr field);

		public static IntPtr FieldGetUiData(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_set_ui_data(PurpleRequestField * field, gpointer ui_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_set_ui_data(IntPtr field, IntPtr ui_data);

		public static void FieldSetUiData(PurpleRequestField field, IntPtr ui_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestField * purple_request_field_string_new(char * id, char * text, char * default_value, gboolean multiline)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_string_new(string id, string text, string default_value, bool multiline);

		public static PurpleRequestField FieldStringNew(string id, string text, string default_value, bool multiline)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_string_set_default_value(PurpleRequestField * field, char * default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_string_set_default_value(IntPtr field, string default_value);

		public static void FieldStringSetDefaultValue(PurpleRequestField field, string default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_string_set_value(PurpleRequestField * field, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_string_set_value(IntPtr field, string value);

		public static void FieldStringSetValue(PurpleRequestField field, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_string_set_masked(PurpleRequestField * field, gboolean masked)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_string_set_masked(IntPtr field, bool masked);

		public static void FieldStringSetMasked(PurpleRequestField field, bool masked)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_string_set_editable(PurpleRequestField * field, gboolean editable)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_string_set_editable(IntPtr field, bool editable);

		public static void FieldStringSetEditable(PurpleRequestField field, bool editable)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_request_field_string_get_default_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_string_get_default_value(IntPtr field);

		public static string FieldStringGetDefaultValue(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_request_field_string_get_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_string_get_value(IntPtr field);

		public static string FieldStringGetValue(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_field_string_is_multiline(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_string_is_multiline(IntPtr field);

		public static bool FieldStringIsMultiline(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_field_string_is_masked(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_string_is_masked(IntPtr field);

		public static bool FieldStringIsMasked(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_field_string_is_editable(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_string_is_editable(IntPtr field);

		public static bool FieldStringIsEditable(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestField * purple_request_field_int_new(char * id, char * text, int default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_int_new(string id, string text, int default_value);

		public static PurpleRequestField FieldIntNew(string id, string text, int default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_int_set_default_value(PurpleRequestField * field, int default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_int_set_default_value(IntPtr field, int default_value);

		public static void FieldIntSetDefaultValue(PurpleRequestField field, int default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_int_set_value(PurpleRequestField * field, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_int_set_value(IntPtr field, int value);

		public static void FieldIntSetValue(PurpleRequestField field, int value)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_request_field_int_get_default_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_field_int_get_default_value(IntPtr field);

		public static int FieldIntGetDefaultValue(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_request_field_int_get_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_field_int_get_value(IntPtr field);

		public static int FieldIntGetValue(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestField * purple_request_field_bool_new(char * id, char * text, gboolean default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_bool_new(string id, string text, bool default_value);

		public static PurpleRequestField FieldBoolNew(string id, string text, bool default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_bool_set_default_value(PurpleRequestField * field, gboolean default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_bool_set_default_value(IntPtr field, bool default_value);

		public static void FieldBoolSetDefaultValue(PurpleRequestField field, bool default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_bool_set_value(PurpleRequestField * field, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_bool_set_value(IntPtr field, bool value);

		public static void FieldBoolSetValue(PurpleRequestField field, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_field_bool_get_default_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_bool_get_default_value(IntPtr field);

		public static bool FieldBoolGetDefaultValue(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_field_bool_get_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_bool_get_value(IntPtr field);

		public static bool FieldBoolGetValue(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestField * purple_request_field_choice_new(char * id, char * text, int default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_choice_new(string id, string text, int default_value);

		public static PurpleRequestField FieldChoiceNew(string id, string text, int default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_choice_add(PurpleRequestField * field, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_choice_add(IntPtr field, string label);

		public static void FieldChoiceAdd(PurpleRequestField field, string label)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_choice_set_default_value(PurpleRequestField * field, int default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_choice_set_default_value(IntPtr field, int default_value);

		public static void FieldChoiceSetDefaultValue(PurpleRequestField field, int default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_choice_set_value(PurpleRequestField * field, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_choice_set_value(IntPtr field, int value);

		public static void FieldChoiceSetValue(PurpleRequestField field, int value)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_request_field_choice_get_default_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_field_choice_get_default_value(IntPtr field);

		public static int FieldChoiceGetDefaultValue(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_request_field_choice_get_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_field_choice_get_value(IntPtr field);

		public static int FieldChoiceGetValue(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_request_field_choice_get_labels(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_choice_get_labels(IntPtr field);

		public static GList FieldChoiceGetLabels(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestField * purple_request_field_list_new(char * id, char * text)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_list_new(string id, string text);

		public static PurpleRequestField FieldListNew(string id, string text)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_list_set_multi_select(PurpleRequestField * field, gboolean multi_select)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_list_set_multi_select(IntPtr field, bool multi_select);

		public static void FieldListSetMultiSelect(PurpleRequestField field, bool multi_select)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_field_list_get_multi_select(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_list_get_multi_select(IntPtr field);

		public static bool FieldListGetMultiSelect(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_request_field_list_get_data(PurpleRequestField * field, char * text)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_list_get_data(IntPtr field, string text);

		public static IntPtr FieldListGetData(PurpleRequestField field, string text)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_list_add(PurpleRequestField * field, char * item, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_list_add(IntPtr field, string item, IntPtr data);

		public static void FieldListAdd(PurpleRequestField field, string item, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_list_add_selected(PurpleRequestField * field, char * item)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_list_add_selected(IntPtr field, string item);

		public static void FieldListAddSelected(PurpleRequestField field, string item)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_list_clear_selected(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_list_clear_selected(IntPtr field);

		public static void FieldListClearSelected(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_list_set_selected(PurpleRequestField * field, GList * items)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_list_set_selected(IntPtr field, IntPtr items);

		public static void FieldListSetSelected(PurpleRequestField field, GList items)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_field_list_is_selected(PurpleRequestField * field, char * item)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_list_is_selected(IntPtr field, string item);

		public static bool FieldListIsSelected(PurpleRequestField field, string item)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_request_field_list_get_selected(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_list_get_selected(IntPtr field);

		public static GList FieldListGetSelected(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_request_field_list_get_items(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_list_get_items(IntPtr field);

		public static GList FieldListGetItems(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestField * purple_request_field_label_new(char * id, char * text)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_label_new(string id, string text);

		public static PurpleRequestField FieldLabelNew(string id, string text)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestField * purple_request_field_image_new(char * id, char * text, char * buf, gsize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_image_new(string id, string text, string buf, UNKNOWN size);

		public static PurpleRequestField FieldImageNew(string id, string text, string buf, gsize size)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_image_set_scale(PurpleRequestField * field, unsigned int, unsigned int)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_image_set_scale(IntPtr field, UNKNOWN int, UNKNOWN int);

		public static void FieldImageSetScale(PurpleRequestField field, unsigned int, unsigned int)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_request_field_image_get_buffer(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_request_field_image_get_buffer(IntPtr field);

		public static string FieldImageGetBuffer(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * gsize purple_request_field_image_get_size(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_request_field_image_get_size(IntPtr field);

		public static gsize FieldImageGetSize(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_request_field_image_get_scale_x(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_field_image_get_scale_x(IntPtr field);

		public static int FieldImageGetScaleX(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_request_field_image_get_scale_y(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_request_field_image_get_scale_y(IntPtr field);

		public static int FieldImageGetScaleY(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestField * purple_request_field_account_new(char * id, char * text, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_account_new(string id, string text, IntPtr account);

		public static PurpleRequestField FieldAccountNew(string id, string text, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_account_set_default_value(PurpleRequestField * field, PurpleAccount * default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_account_set_default_value(IntPtr field, IntPtr default_value);

		public static void FieldAccountSetDefaultValue(PurpleRequestField field, PurpleAccount default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_account_set_value(PurpleRequestField * field, PurpleAccount * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_account_set_value(IntPtr field, IntPtr value);

		public static void FieldAccountSetValue(PurpleRequestField field, PurpleAccount value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_account_set_show_all(PurpleRequestField * field, gboolean show_all)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_account_set_show_all(IntPtr field, bool show_all);

		public static void FieldAccountSetShowAll(PurpleRequestField field, bool show_all)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_field_account_set_filter(PurpleRequestField * field, PurpleFilterAccountFunc filter_func)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_field_account_set_filter(IntPtr field, UNKNOWN filter_func);

		public static void FieldAccountSetFilter(PurpleRequestField field, PurpleFilterAccountFunc filter_func)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_request_field_account_get_default_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_account_get_default_value(IntPtr field);

		public static PurpleAccount FieldAccountGetDefaultValue(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_request_field_account_get_value(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_field_account_get_value(IntPtr field);

		public static PurpleAccount FieldAccountGetValue(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_request_field_account_get_show_all(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_request_field_account_get_show_all(IntPtr field);

		public static bool FieldAccountGetShowAll(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleFilterAccountFunc purple_request_field_account_get_filter(PurpleRequestField * field)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_request_field_account_get_filter(IntPtr field);

		public static PurpleFilterAccountFunc FieldAccountGetFilter(PurpleRequestField field)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_request_input(void * handle, char * title, char * primary, char * secondary, char * default_value, gboolean multiline, gboolean masked, gchar * hint, char * ok_text, GCallback ok_cb, char * cancel_text, GCallback cancel_cb, PurpleAccount * account, char * who, PurpleConversation * conv, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_input(IntPtr handle, string title, string primary, string secondary, string default_value, bool multiline, bool masked, string hint, string ok_text, UNKNOWN ok_cb, string cancel_text, UNKNOWN cancel_cb, IntPtr account, string who, IntPtr conv, IntPtr user_data);

		public static IntPtr Input(IntPtr handle, string title, string primary, string secondary, string default_value, bool multiline, bool masked, string hint, string ok_text, GCallback ok_cb, string cancel_text, GCallback cancel_cb, PurpleAccount account, string who, PurpleConversation conv, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_request_choice(void * handle, char * title, char * primary, char * secondary, int default_value, char * ok_text, GCallback ok_cb, char * cancel_text, GCallback cancel_cb, PurpleAccount * account, char * who, PurpleConversation * conv, void * user_data, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_choice(IntPtr handle, string title, string primary, string secondary, int default_value, string ok_text, UNKNOWN ok_cb, string cancel_text, UNKNOWN cancel_cb, IntPtr account, string who, IntPtr conv, IntPtr user_data, ...);

		public static IntPtr Choice(IntPtr handle, string title, string primary, string secondary, int default_value, string ok_text, GCallback ok_cb, string cancel_text, GCallback cancel_cb, PurpleAccount account, string who, PurpleConversation conv, IntPtr user_data, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_request_choice_varg(void * handle, char * title, char * primary, char * secondary, int default_value, char * ok_text, GCallback ok_cb, char * cancel_text, GCallback cancel_cb, PurpleAccount * account, char * who, PurpleConversation * conv, void * user_data, va_list choices)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_choice_varg(IntPtr handle, string title, string primary, string secondary, int default_value, string ok_text, UNKNOWN ok_cb, string cancel_text, UNKNOWN cancel_cb, IntPtr account, string who, IntPtr conv, IntPtr user_data, UNKNOWN choices);

		public static IntPtr ChoiceVarg(IntPtr handle, string title, string primary, string secondary, int default_value, string ok_text, GCallback ok_cb, string cancel_text, GCallback cancel_cb, PurpleAccount account, string who, PurpleConversation conv, IntPtr user_data, va_list choices)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_request_action(void * handle, char * title, char * primary, char * secondary, int default_action, PurpleAccount * account, char * who, PurpleConversation * conv, void * user_data, size_t action_count, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_action(IntPtr handle, string title, string primary, string secondary, int default_action, IntPtr account, string who, IntPtr conv, IntPtr user_data, UNKNOWN action_count, ...);

		public static IntPtr Action(IntPtr handle, string title, string primary, string secondary, int default_action, PurpleAccount account, string who, PurpleConversation conv, IntPtr user_data, size_t action_count, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_request_action_varg(void * handle, char * title, char * primary, char * secondary, int default_action, PurpleAccount * account, char * who, PurpleConversation * conv, void * user_data, size_t action_count, va_list actions)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_action_varg(IntPtr handle, string title, string primary, string secondary, int default_action, IntPtr account, string who, IntPtr conv, IntPtr user_data, UNKNOWN action_count, UNKNOWN actions);

		public static IntPtr ActionVarg(IntPtr handle, string title, string primary, string secondary, int default_action, PurpleAccount account, string who, PurpleConversation conv, IntPtr user_data, size_t action_count, va_list actions)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_request_fields(void * handle, char * title, char * primary, char * secondary, PurpleRequestFields * fields, char * ok_text, GCallback ok_cb, char * cancel_text, GCallback cancel_cb, PurpleAccount * account, char * who, PurpleConversation * conv, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_fields(IntPtr handle, string title, string primary, string secondary, IntPtr fields, string ok_text, UNKNOWN ok_cb, string cancel_text, UNKNOWN cancel_cb, IntPtr account, string who, IntPtr conv, IntPtr user_data);

		public static IntPtr Fields(IntPtr handle, string title, string primary, string secondary, PurpleRequestFields fields, string ok_text, GCallback ok_cb, string cancel_text, GCallback cancel_cb, PurpleAccount account, string who, PurpleConversation conv, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_close(PurpleRequestType type, void * uihandle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_close(UNKNOWN type, IntPtr uihandle);

		public static void Close(PurpleRequestType type, IntPtr uihandle)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_close_with_handle(void * handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_close_with_handle(IntPtr handle);

		public static void CloseWithHandle(IntPtr handle)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_request_file(void * handle, char * title, char * filename, gboolean savedialog, GCallback ok_cb, GCallback cancel_cb, PurpleAccount * account, char * who, PurpleConversation * conv, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_file(IntPtr handle, string title, string filename, bool savedialog, UNKNOWN ok_cb, UNKNOWN cancel_cb, IntPtr account, string who, IntPtr conv, IntPtr user_data);

		public static IntPtr File(IntPtr handle, string title, string filename, bool savedialog, GCallback ok_cb, GCallback cancel_cb, PurpleAccount account, string who, PurpleConversation conv, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_request_folder(void * handle, char * title, char * dirname, GCallback ok_cb, GCallback cancel_cb, PurpleAccount * account, char * who, PurpleConversation * conv, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_folder(IntPtr handle, string title, string dirname, UNKNOWN ok_cb, UNKNOWN cancel_cb, IntPtr account, string who, IntPtr conv, IntPtr user_data);

		public static IntPtr Folder(IntPtr handle, string title, string dirname, GCallback ok_cb, GCallback cancel_cb, PurpleAccount account, string who, PurpleConversation conv, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_request_set_ui_ops(PurpleRequestUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_request_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleRequestUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleRequestUiOps * purple_request_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_request_get_ui_ops();

		public static PurpleRequestUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

	}
}


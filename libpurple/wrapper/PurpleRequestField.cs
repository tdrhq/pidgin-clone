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
	public class PurpleRequestField : UnmanagedWrapper<_PurpleRequestField>
	{
		public PurpleRequestField()
		{
		}

		public PurpleRequestField(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleRequestField)Marshal.PtrToStructure(this.Reference, typeof(_PurpleRequestField));
		}

		public PurpleRequestFieldType type
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public PurpleRequestFieldGroup group
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public string id
		{
			get
			{
				return this.Data.id;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.id = value;
			}
		}

		public string label
		{
			get
			{
				return this.Data.label;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.label = value;
			}
		}

		public string type_hint
		{
			get
			{
				return this.Data.type_hint;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.type_hint = value;
			}
		}

		public bool visible
		{
			get
			{
				return this.Data.visible;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.visible = value;
			}
		}

		public bool required
		{
			get
			{
				return this.Data.required;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.required = value;
			}
		}

		public union {
		struct
		{
			gboolean multiline
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public bool masked
		{
			get
			{
				return this.Data.masked;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.masked = value;
			}
		}

		public bool editable
		{
			get
			{
				return this.Data.editable;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.editable = value;
			}
		}

		public string default_value
		{
			get
			{
				return this.Data.default_value;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.default_value = value;
			}
		}

		public string value
		{
			get
			{
				return this.Data.value;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.value = value;
			}
		}

		public struct {
			int default_value
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public int value
		{
			get
			{
				return this.Data.value;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.value = value;
			}
		}

		public struct {
			gboolean default_value
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public bool value
		{
			get
			{
				return this.Data.value;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.value = value;
			}
		}

		public struct {
			int default_value
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public int value
		{
			get
			{
				return this.Data.value;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.value = value;
			}
		}

		public GList labels
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public struct {
			GList *items
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public GHashTable item_data
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public GList selected
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public GHashTable selected_table
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public bool multiple_selection
		{
			get
			{
				return this.Data.multiple_selection;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.multiple_selection = value;
			}
		}

		public struct {
			PurpleAccount *default_account
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public PurpleAccount account
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public bool show_all
		{
			get
			{
				return this.Data.show_all;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.show_all = value;
			}
		}

		public PurpleFilterAccountFunc filter_func
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public struct {
			unsigned int scale_x
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public unsigned int scale_y
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public const char *buffer
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public gsize size
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public IntPtr ui_data
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	class _PurpleRequestField
	{
		/*
		 * PurpleRequestFieldType type
		 */
		UNKNOWN type;

		/*
		 * PurpleRequestFieldGroup * group
		 */
		IntPtr group;

		/*
		 * char * id
		 */
		string id;

		/*
		 * char * label
		 */
		string label;

		/*
		 * char * type_hint
		 */
		string type_hint;

		/*
		 * gboolean visible
		 */
		bool visible;

		/*
		 * gboolean required
		 */
		bool required;

		/*
		 * union {
		struct
		{
			gboolean multiline
		 */
		UNKNOWN {
		struct
		{
			gboolean multiline;

		/*
		 * gboolean masked
		 */
		bool masked;

		/*
		 * gboolean editable
		 */
		bool editable;

		/*
		 * char * default_value
		 */
		string default_value;

		/*
		 * char * value
		 */
		string value;

		/*
		 * struct {
			int default_value
		 */
		UNKNOWN {
			int default_value;

		/*
		 * int value
		 */
		int value;

		/*
		 * struct {
			gboolean default_value
		 */
		UNKNOWN {
			gboolean default_value;

		/*
		 * gboolean value
		 */
		bool value;

		/*
		 * struct {
			int default_value
		 */
		UNKNOWN {
			int default_value;

		/*
		 * int value
		 */
		int value;

		/*
		 * GList * labels
		 */
		IntPtr labels;

		/*
		 * struct {
			GList *items
		 */
		UNKNOWN {
			GList *items;

		/*
		 * GHashTable * item_data
		 */
		IntPtr item_data;

		/*
		 * GList * selected
		 */
		IntPtr selected;

		/*
		 * GHashTable * selected_table
		 */
		IntPtr selected_table;

		/*
		 * gboolean multiple_selection
		 */
		bool multiple_selection;

		/*
		 * struct {
			PurpleAccount *default_account
		 */
		UNKNOWN {
			PurpleAccount *default_account;

		/*
		 * PurpleAccount * account
		 */
		IntPtr account;

		/*
		 * gboolean show_all
		 */
		bool show_all;

		/*
		 * PurpleFilterAccountFunc filter_func
		 */
		UNKNOWN filter_func;

		/*
		 * struct {
			unsigned int scale_x
		 */
		UNKNOWN {
			unsigned int scale_x;

		/*
		 * unsigned int scale_y
		 */
		UNKNOWN int scale_y;

		/*
		 * const char *buffer
		 */
		UNKNOWN char *buffer;

		/*
		 * gsize size
		 */
		UNKNOWN size;

		/*
		 * void * ui_data
		 */
		IntPtr ui_data;

	}
}


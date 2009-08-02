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
	 * Structure: PurpleRequestField
	 */
	public class PurpleRequestField : UnmanagedWrapper<_PurpleRequestField>
	{
		public PurpleRequestField()
		{
		}

		public PurpleRequestField(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleRequestField)Marshal.PtrToStructure(this.Reference, typeof(_PurpleRequestField));
		}

		/*
		 * Argument Name: type
		 * Argument Type: PurpleRequestFieldType
		 * Argument Category: KnownEnum
		 */
		public Request.PurpleRequestFieldType type
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

		/*
		 * Argument Name: group
		 * Argument Type: PurpleRequestFieldGroup *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to PurpleRequestFieldGroup */ IntPtr group
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

		/*
		 * Argument Name: id
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: label
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: type_hint
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: visible
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: required
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: {
		struct
		{
			gboolean multiline
		 * Argument Type: union
		 * Argument Category: InnerStruct
		 */
		/*
		 * Could not generate a wrapper for PurpleRequestField.{
		struct
		{
			gboolean multiline in file "request.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * Argument Name: masked
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: editable
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: default_value
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: value
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurpleRequestField
	{
		/*
		 * PurpleRequestFieldType type
		 */
		public Request.PurpleRequestFieldType type;

		/*
		 * PurpleRequestFieldGroup * group
		 */
		public IntPtr group;

		/*
		 * char * id
		 */
		public string id;

		/*
		 * char * label
		 */
		public string label;

		/*
		 * char * type_hint
		 */
		public string type_hint;

		/*
		 * gboolean visible
		 */
		public bool visible;

		/*
		 * gboolean required
		 */
		public bool required;

		/*
		 * union {
		struct
		{
			gboolean multiline
		 */
		/*
		 * Could not generate a wrapper for PurpleRequestField.{
		struct
		{
			gboolean multiline in file "request.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * gboolean masked
		 */
		public bool masked;

		/*
		 * gboolean editable
		 */
		public bool editable;

		/*
		 * char * default_value
		 */
		public string default_value;

		/*
		 * char * value
		 */
		public string value;

	}

}


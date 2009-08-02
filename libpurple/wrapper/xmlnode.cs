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
	 * File: xmlnode.h
	 * Structure: xmlnode
	 */
	public class xmlnode : UnmanagedWrapper<_xmlnode>
	{
		public xmlnode()
		{
		}

		public xmlnode(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_xmlnode)Marshal.PtrToStructure(this.Reference, typeof(_xmlnode));
		}

		/*
		 * Argument Name: name
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string name
		{
			get
			{
				return this.Data.name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.name = value;
			}
		}

		/*
		 * Argument Name: xmlns
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string xmlns
		{
			get
			{
				return this.Data.xmlns;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.xmlns = value;
			}
		}

		/*
		 * Argument Name: type
		 * Argument Type: XMLNodeType
		 * Argument Category: KnownEnum
		 */
		public XMLNodeClass.XMLNodeType type
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
		 * Argument Name: data
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string data
		{
			get
			{
				return this.Data.data;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.data = value;
			}
		}

		/*
		 * Argument Name: data_sz
		 * Argument Type: size_t
		 * Argument Category: Native
		 */
		public ulong data_sz
		{
			get
			{
				return this.Data.data_sz;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.data_sz = value;
			}
		}

		/*
		 * Argument Name: xmlnode *parent
		 * Argument Type: struct
		 * Argument Category: InnerStruct
		 */
		/*
		 * Could not generate a wrapper for xmlnode.xmlnode *parent in file "xmlnode.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * Argument Name: xmlnode *child
		 * Argument Type: struct
		 * Argument Category: InnerStruct
		 */
		/*
		 * Could not generate a wrapper for xmlnode.xmlnode *child in file "xmlnode.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * Argument Name: xmlnode *lastchild
		 * Argument Type: struct
		 * Argument Category: InnerStruct
		 */
		/*
		 * Could not generate a wrapper for xmlnode.xmlnode *lastchild in file "xmlnode.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * Argument Name: xmlnode *next
		 * Argument Type: struct
		 * Argument Category: InnerStruct
		 */
		/*
		 * Could not generate a wrapper for xmlnode.xmlnode *next in file "xmlnode.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * Argument Name: prefix
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string prefix
		{
			get
			{
				return this.Data.prefix;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.prefix = value;
			}
		}

		/*
		 * Argument Name: namespace_map
		 * Argument Type: GHashTable *
		 * Argument Category: PointerToGHashTable
		 */
		/*
		 * Could not generate a wrapper for xmlnode.namespace_map in file "xmlnode.h".
		 * Message: Unable to provide a type for HashTable as no function information was provided in code.
		 */

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _xmlnode
	{
		/*
		 * char * name
		 */
		public string name;

		/*
		 * char * xmlns
		 */
		public string xmlns;

		/*
		 * XMLNodeType type
		 */
		public XMLNodeClass.XMLNodeType type;

		/*
		 * char * data
		 */
		public string data;

		/*
		 * size_t data_sz
		 */
		public ulong data_sz;

		/*
		 * struct xmlnode *parent
		 */
		/*
		 * Could not generate a wrapper for xmlnode.xmlnode *parent in file "xmlnode.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * struct xmlnode *child
		 */
		/*
		 * Could not generate a wrapper for xmlnode.xmlnode *child in file "xmlnode.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * struct xmlnode *lastchild
		 */
		/*
		 * Could not generate a wrapper for xmlnode.xmlnode *lastchild in file "xmlnode.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * struct xmlnode *next
		 */
		/*
		 * Could not generate a wrapper for xmlnode.xmlnode *next in file "xmlnode.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * char * prefix
		 */
		public string prefix;

		/*
		 * GHashTable * namespace_map
		 */
		public IntPtr namespace_map;

	}

}


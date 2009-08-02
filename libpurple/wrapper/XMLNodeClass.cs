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
	 */
	public class XMLNodeClass
	{
		public enum XMLNodeType
		{
			XMLNODE_TYPE_TAG,
			XMLNODE_TYPE_ATTRIB,
			XMLNODE_TYPE_DATA
		};


		/*
		 * Function Name: xmlnode_new
		 * Function Type: xmlnode *
		 * Function RawType: xmlnode
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * xmlnode * xmlnode_new(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_new(string name);

		public static xmlnode XmlnodeNew(string name)
		{
			return new xmlnode(xmlnode_new(name));
		}

		/*
		 * Function Name: xmlnode_new_child
		 * Function Type: xmlnode *
		 * Function RawType: xmlnode
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * xmlnode * xmlnode_new_child(xmlnode * parent, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_new_child(IntPtr parent, string name);

		public static xmlnode XmlnodeNewChild(xmlnode parent, string name)
		{
			return new xmlnode(xmlnode_new_child(parent.Reference, name));
		}

		/*
		 * Function Name: xmlnode_insert_child
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void xmlnode_insert_child(xmlnode * parent, xmlnode * child)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_insert_child(IntPtr parent, IntPtr child);

		public static void XmlnodeInsertChild(xmlnode parent, xmlnode child)
		{
			xmlnode_insert_child(parent.Reference, child.Reference);
		}

		/*
		 * Function Name: xmlnode_get_child
		 * Function Type: xmlnode *
		 * Function RawType: xmlnode
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * xmlnode * xmlnode_get_child(xmlnode * parent, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_get_child(IntPtr parent, string name);

		public static xmlnode XmlnodeGetChild(xmlnode parent, string name)
		{
			return new xmlnode(xmlnode_get_child(parent.Reference, name));
		}

		/*
		 * Function Name: xmlnode_get_child_with_namespace
		 * Function Type: xmlnode *
		 * Function RawType: xmlnode
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * xmlnode * xmlnode_get_child_with_namespace(xmlnode * parent, char * name, char * xmlns)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_get_child_with_namespace(IntPtr parent, string name, string xmlns);

		public static xmlnode XmlnodeGetChildWithNamespace(xmlnode parent, string name, string xmlns)
		{
			return new xmlnode(xmlnode_get_child_with_namespace(parent.Reference, name, xmlns));
		}

		/*
		 * Function Name: xmlnode_get_next_twin
		 * Function Type: xmlnode *
		 * Function RawType: xmlnode
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * xmlnode * xmlnode_get_next_twin(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_get_next_twin(IntPtr node);

		public static xmlnode XmlnodeGetNextTwin(xmlnode node)
		{
			return new xmlnode(xmlnode_get_next_twin(node.Reference));
		}

		/*
		 * Function Name: xmlnode_insert_data
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void xmlnode_insert_data(xmlnode * node, char * data, gssize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_insert_data(IntPtr node, string data, long size);

		public static void XmlnodeInsertData(xmlnode node, string data, long size)
		{
			xmlnode_insert_data(node.Reference, data, size);
		}

		/*
		 * Function Name: xmlnode_get_data
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * xmlnode_get_data(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_data(IntPtr node);

		public static string XmlnodeGetData(xmlnode node)
		{
			return xmlnode_get_data(node.Reference);
		}

		/*
		 * Function Name: xmlnode_get_data_unescaped
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * xmlnode_get_data_unescaped(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_data_unescaped(IntPtr node);

		public static string XmlnodeGetDataUnescaped(xmlnode node)
		{
			return xmlnode_get_data_unescaped(node.Reference);
		}

		/*
		 * Function Name: xmlnode_set_attrib
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void xmlnode_set_attrib(xmlnode * node, char * attr, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_attrib(IntPtr node, string attr, string value);

		public static void XmlnodeSetAttrib(xmlnode node, string attr, string value)
		{
			xmlnode_set_attrib(node.Reference, attr, value);
		}

		/*
		 * Function Name: xmlnode_set_attrib_with_prefix
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void xmlnode_set_attrib_with_prefix(xmlnode * node, char * attr, char * prefix, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_attrib_with_prefix(IntPtr node, string attr, string prefix, string value);

		public static void XmlnodeSetAttribWithPrefix(xmlnode node, string attr, string prefix, string value)
		{
			xmlnode_set_attrib_with_prefix(node.Reference, attr, prefix, value);
		}

		/*
		 * Function Name: xmlnode_set_attrib_with_namespace
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void xmlnode_set_attrib_with_namespace(xmlnode * node, char * attr, char * xmlns, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_attrib_with_namespace(IntPtr node, string attr, string xmlns, string value);

		public static void XmlnodeSetAttribWithNamespace(xmlnode node, string attr, string xmlns, string value)
		{
			xmlnode_set_attrib_with_namespace(node.Reference, attr, xmlns, value);
		}

		/*
		 * Function Name: xmlnode_get_attrib
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * xmlnode_get_attrib(xmlnode * node, char * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_attrib(IntPtr node, string attr);

		public static string XmlnodeGetAttrib(xmlnode node, string attr)
		{
			return xmlnode_get_attrib(node.Reference, attr);
		}

		/*
		 * Function Name: xmlnode_get_attrib_with_namespace
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * xmlnode_get_attrib_with_namespace(xmlnode * node, char * attr, char * xmlns)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_attrib_with_namespace(IntPtr node, string attr, string xmlns);

		public static string XmlnodeGetAttribWithNamespace(xmlnode node, string attr, string xmlns)
		{
			return xmlnode_get_attrib_with_namespace(node.Reference, attr, xmlns);
		}

		/*
		 * Function Name: xmlnode_remove_attrib
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void xmlnode_remove_attrib(xmlnode * node, char * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_remove_attrib(IntPtr node, string attr);

		public static void XmlnodeRemoveAttrib(xmlnode node, string attr)
		{
			xmlnode_remove_attrib(node.Reference, attr);
		}

		/*
		 * Function Name: xmlnode_remove_attrib_with_namespace
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void xmlnode_remove_attrib_with_namespace(xmlnode * node, char * attr, char * xmlns)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_remove_attrib_with_namespace(IntPtr node, string attr, string xmlns);

		public static void XmlnodeRemoveAttribWithNamespace(xmlnode node, string attr, string xmlns)
		{
			xmlnode_remove_attrib_with_namespace(node.Reference, attr, xmlns);
		}

		/*
		 * Function Name: xmlnode_set_namespace
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void xmlnode_set_namespace(xmlnode * node, char * xmlns)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_namespace(IntPtr node, string xmlns);

		public static void XmlnodeSetNamespace(xmlnode node, string xmlns)
		{
			xmlnode_set_namespace(node.Reference, xmlns);
		}

		/*
		 * Function Name: xmlnode_get_namespace
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * xmlnode_get_namespace(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_namespace(IntPtr node);

		public static string XmlnodeGetNamespace(xmlnode node)
		{
			return xmlnode_get_namespace(node.Reference);
		}

		/*
		 * Function Name: xmlnode_set_prefix
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void xmlnode_set_prefix(xmlnode * node, char * prefix)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_prefix(IntPtr node, string prefix);

		public static void XmlnodeSetPrefix(xmlnode node, string prefix)
		{
			xmlnode_set_prefix(node.Reference, prefix);
		}

		/*
		 * Function Name: xmlnode_get_prefix
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * xmlnode_get_prefix(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_prefix(IntPtr node);

		public static string XmlnodeGetPrefix(xmlnode node)
		{
			return xmlnode_get_prefix(node.Reference);
		}

		/*
		 * Function Name: xmlnode_get_parent
		 * Function Type: xmlnode *
		 * Function RawType: xmlnode
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * xmlnode * xmlnode_get_parent(xmlnode * child)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_get_parent(IntPtr child);

		public static xmlnode XmlnodeGetParent(xmlnode child)
		{
			return new xmlnode(xmlnode_get_parent(child.Reference));
		}

		/*
		 * Function Name: xmlnode_to_str
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * xmlnode_to_str(xmlnode * node, int * len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_to_str(IntPtr node, IntPtr len);

		public static string XmlnodeToStr(xmlnode node, /* Resolved as PointerToUnknownStruct to int */ IntPtr len)
		{
			/* Unable to process len, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: xmlnode_to_formatted_str
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * xmlnode_to_formatted_str(xmlnode * node, int * len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_to_formatted_str(IntPtr node, IntPtr len);

		public static string XmlnodeToFormattedStr(xmlnode node, /* Resolved as PointerToUnknownStruct to int */ IntPtr len)
		{
			/* Unable to process len, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: xmlnode_from_str
		 * Function Type: xmlnode *
		 * Function RawType: xmlnode
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * xmlnode * xmlnode_from_str(char * str, gssize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_from_str(string str, long size);

		public static xmlnode XmlnodeFromStr(string str, long size)
		{
			return new xmlnode(xmlnode_from_str(str, size));
		}

		/*
		 * Function Name: xmlnode_copy
		 * Function Type: xmlnode *
		 * Function RawType: xmlnode
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * xmlnode * xmlnode_copy(xmlnode * src)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_copy(IntPtr src);

		public static xmlnode XmlnodeCopy(xmlnode src)
		{
			return new xmlnode(xmlnode_copy(src.Reference));
		}

		/*
		 * Function Name: xmlnode_free
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void xmlnode_free(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_free(IntPtr node);

		public static void XmlnodeFree(xmlnode node)
		{
			xmlnode_free(node.Reference);
		}

	}
}


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
	public class Xmlnode
	{
		/*
		 * xmlnode * xmlnode_new(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_new(string name);

		public static xmlnode New(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * xmlnode * xmlnode_new_child(xmlnode * parent, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_new_child(IntPtr parent, string name);

		public static xmlnode NewChild(xmlnode parent, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_insert_child(xmlnode * parent, xmlnode * child)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_insert_child(IntPtr parent, IntPtr child);

		public static void InsertChild(xmlnode parent, xmlnode child)
		{
			throw new NotImplementedException();
		}

		/*
		 * xmlnode * xmlnode_get_child(xmlnode * parent, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_get_child(IntPtr parent, string name);

		public static xmlnode GetChild(xmlnode parent, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * xmlnode * xmlnode_get_child_with_namespace(xmlnode * parent, char * name, char * xmlns)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_get_child_with_namespace(IntPtr parent, string name, string xmlns);

		public static xmlnode GetChildWithNamespace(xmlnode parent, string name, string xmlns)
		{
			throw new NotImplementedException();
		}

		/*
		 * xmlnode * xmlnode_get_next_twin(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_get_next_twin(IntPtr node);

		public static xmlnode GetNextTwin(xmlnode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_insert_data(xmlnode * node, char * data, gssize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_insert_data(IntPtr node, string data, UNKNOWN size);

		public static void InsertData(xmlnode node, string data, gssize size)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * xmlnode_get_data(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_data(IntPtr node);

		public static string GetData(xmlnode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * xmlnode_get_data_unescaped(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_data_unescaped(IntPtr node);

		public static string GetDataUnescaped(xmlnode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_set_attrib(xmlnode * node, char * attr, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_attrib(IntPtr node, string attr, string value);

		public static void SetAttrib(xmlnode node, string attr, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_set_attrib_with_prefix(xmlnode * node, char * attr, char * prefix, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_attrib_with_prefix(IntPtr node, string attr, string prefix, string value);

		public static void SetAttribWithPrefix(xmlnode node, string attr, string prefix, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_set_attrib_with_namespace(xmlnode * node, char * attr, char * xmlns, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_attrib_with_namespace(IntPtr node, string attr, string xmlns, string value);

		public static void SetAttribWithNamespace(xmlnode node, string attr, string xmlns, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_set_attrib_full(xmlnode * node, char * attr, char * xmlns, char * prefix, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_attrib_full(IntPtr node, string attr, string xmlns, string prefix, string value);

		public static void SetAttribFull(xmlnode node, string attr, string xmlns, string prefix, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * xmlnode_get_attrib(xmlnode * node, char * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_attrib(IntPtr node, string attr);

		public static string GetAttrib(xmlnode node, string attr)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * xmlnode_get_attrib_with_namespace(xmlnode * node, char * attr, char * xmlns)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_attrib_with_namespace(IntPtr node, string attr, string xmlns);

		public static string GetAttribWithNamespace(xmlnode node, string attr, string xmlns)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_remove_attrib(xmlnode * node, char * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_remove_attrib(IntPtr node, string attr);

		public static void RemoveAttrib(xmlnode node, string attr)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_remove_attrib_with_namespace(xmlnode * node, char * attr, char * xmlns)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_remove_attrib_with_namespace(IntPtr node, string attr, string xmlns);

		public static void RemoveAttribWithNamespace(xmlnode node, string attr, string xmlns)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_set_namespace(xmlnode * node, char * xmlns)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_namespace(IntPtr node, string xmlns);

		public static void SetNamespace(xmlnode node, string xmlns)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * xmlnode_get_namespace(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_namespace(IntPtr node);

		public static string GetNamespace(xmlnode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_set_prefix(xmlnode * node, char * prefix)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_set_prefix(IntPtr node, string prefix);

		public static void SetPrefix(xmlnode node, string prefix)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * xmlnode_get_prefix(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_get_prefix(IntPtr node);

		public static string GetPrefix(xmlnode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * xmlnode * xmlnode_get_parent(xmlnode * child)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_get_parent(IntPtr child);

		public static xmlnode GetParent(xmlnode child)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * xmlnode_to_str(xmlnode * node, int * len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_to_str(IntPtr node, IntPtr len);

		public static string ToStr(xmlnode node, int len)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * xmlnode_to_formatted_str(xmlnode * node, int * len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string xmlnode_to_formatted_str(IntPtr node, IntPtr len);

		public static string ToFormattedStr(xmlnode node, int len)
		{
			throw new NotImplementedException();
		}

		/*
		 * xmlnode * xmlnode_from_str(char * str, gssize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_from_str(string str, UNKNOWN size);

		public static xmlnode FromStr(string str, gssize size)
		{
			throw new NotImplementedException();
		}

		/*
		 * xmlnode * xmlnode_copy(xmlnode * src)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_copy(IntPtr src);

		public static xmlnode Copy(xmlnode src)
		{
			throw new NotImplementedException();
		}

		/*
		 * void xmlnode_free(xmlnode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void xmlnode_free(IntPtr node);

		public static void Free(xmlnode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * xmlnode * xmlnode_from_file(char * dir, char * filename, char * description, char * process)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr xmlnode_from_file(string dir, string filename, string description, string process);

		public static xmlnode FromFile(string dir, string filename, string description, string process)
		{
			throw new NotImplementedException();
		}

	}
}


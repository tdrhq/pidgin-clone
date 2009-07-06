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
	public class Value
	{
		public enum PurpleType
		{
			PURPLE_TYPE_UNKNOWN = 0,
			PURPLE_TYPE_SUBTYPE,
			PURPLE_TYPE_CHAR,
			PURPLE_TYPE_UCHAR,
			PURPLE_TYPE_BOOLEAN,
			PURPLE_TYPE_SHORT,
			PURPLE_TYPE_USHORT,
			PURPLE_TYPE_INT,
			PURPLE_TYPE_UINT,
			PURPLE_TYPE_LONG,
			PURPLE_TYPE_ULONG,
			PURPLE_TYPE_INT64,
			PURPLE_TYPE_UINT64,
			PURPLE_TYPE_STRING,
			PURPLE_TYPE_OBJECT,
			PURPLE_TYPE_POINTER,
			PURPLE_TYPE_ENUM,
			PURPLE_TYPE_BOXED
		};

		public enum PurpleSubType
		{
			PURPLE_SUBTYPE_UNKNOWN = 0,
			PURPLE_SUBTYPE_ACCOUNT,
			PURPLE_SUBTYPE_BLIST,
			PURPLE_SUBTYPE_BLIST_BUDDY,
			PURPLE_SUBTYPE_BLIST_GROUP,
			PURPLE_SUBTYPE_BLIST_CHAT,
			PURPLE_SUBTYPE_BUDDY_ICON,
			PURPLE_SUBTYPE_CONNECTION,
			PURPLE_SUBTYPE_CONVERSATION,
			PURPLE_SUBTYPE_PLUGIN,
			PURPLE_SUBTYPE_BLIST_NODE,
			PURPLE_SUBTYPE_CIPHER,
			PURPLE_SUBTYPE_STATUS,
			PURPLE_SUBTYPE_LOG,
			PURPLE_SUBTYPE_XFER,
			PURPLE_SUBTYPE_SAVEDSTATUS,
			PURPLE_SUBTYPE_XMLNODE,
			PURPLE_SUBTYPE_USERINFO,
			PURPLE_SUBTYPE_STORED_IMAGE,
			PURPLE_SUBTYPE_CERTIFICATEPOOL
		};

		/*
		 * void purple_value_destroy(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_destroy in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * PurpleValue * purple_value_dup(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_dup in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * purple_value_dup(PurpleValue * value)).
		 */

		/*
		 * PurpleType purple_value_get_type(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_type in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * unsigned int purple_value_get_subtype(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_subtype in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * char * purple_value_get_specific_type(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_specific_type in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * gboolean purple_value_is_outgoing(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_is_outgoing in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_char(PurpleValue * value, char data)
		 * 
		 * Could not generate a wrapper for purple_value_set_char in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_uchar(PurpleValue * value, unsigned char data)
		 * 
		 * Could not generate a wrapper for purple_value_set_uchar in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_boolean(PurpleValue * value, gboolean data)
		 * 
		 * Could not generate a wrapper for purple_value_set_boolean in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_short(PurpleValue * value, short data)
		 * 
		 * Could not generate a wrapper for purple_value_set_short in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_ushort(PurpleValue * value, unsigned short data)
		 * 
		 * Could not generate a wrapper for purple_value_set_ushort in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_int(PurpleValue * value, int data)
		 * 
		 * Could not generate a wrapper for purple_value_set_int in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_uint(PurpleValue * value, unsigned int data)
		 * 
		 * Could not generate a wrapper for purple_value_set_uint in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_long(PurpleValue * value, long data)
		 * 
		 * Could not generate a wrapper for purple_value_set_long in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_ulong(PurpleValue * value, unsigned long data)
		 * 
		 * Could not generate a wrapper for purple_value_set_ulong in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_int64(PurpleValue * value, gint64 data)
		 * 
		 * Could not generate a wrapper for purple_value_set_int64 in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_uint64(PurpleValue * value, guint64 data)
		 * 
		 * Could not generate a wrapper for purple_value_set_uint64 in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_string(PurpleValue * value, char * data)
		 * 
		 * Could not generate a wrapper for purple_value_set_string in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_object(PurpleValue * value, void * data)
		 * 
		 * Could not generate a wrapper for purple_value_set_object in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_pointer(PurpleValue * value, void * data)
		 * 
		 * Could not generate a wrapper for purple_value_set_pointer in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_enum(PurpleValue * value, int data)
		 * 
		 * Could not generate a wrapper for purple_value_set_enum in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void purple_value_set_boxed(PurpleValue * value, void * data)
		 * 
		 * Could not generate a wrapper for purple_value_set_boxed in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * char purple_value_get_char(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_char in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * unsigned char purple_value_get_uchar(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_uchar in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * gboolean purple_value_get_boolean(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_boolean in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * short purple_value_get_short(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_short in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * unsigned short purple_value_get_ushort(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_ushort in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * int purple_value_get_int(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_int in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * unsigned int purple_value_get_uint(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_uint in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * long purple_value_get_long(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_long in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * unsigned long purple_value_get_ulong(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_ulong in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * gint64 purple_value_get_int64(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_int64 in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * guint64 purple_value_get_uint64(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_uint64 in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * char * purple_value_get_string(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_string in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void * purple_value_get_object(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_object in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void * purple_value_get_pointer(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_pointer in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * int purple_value_get_enum(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_enum in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

		/*
		 * void * purple_value_get_boxed(PurpleValue * value)
		 * 
		 * Could not generate a wrapper for purple_value_get_boxed in file "value.h".
		 * Message: The type could not be resolved (PurpleValue * value).
		 */

	}
}


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
	 * File: log.h
	 * Structure: PurpleLog
	 */
	public class PurpleLog : UnmanagedWrapper<_PurpleLog>
	{
		public PurpleLog()
		{
		}

		public PurpleLog(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleLog)Marshal.PtrToStructure(this.Reference, typeof(_PurpleLog));
		}

		/*
		 * Argument Name: type
		 * Argument Type: PurpleLogType
		 * Argument Category: KnownEnum
		 */
		public Log.PurpleLogType type
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
		 * Argument Name: account
		 * Argument Type: PurpleAccount *
		 * Argument Category: PointerToKnownStruct
		 */
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

		/*
		 * Argument Name: conv
		 * Argument Type: PurpleConversation *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleConversation conv
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
		 * Argument Name: time
		 * Argument Type: time_t
		 * Argument Category: DateTime
		 */
		public DateTime time
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
		 * Argument Name: logger
		 * Argument Type: PurpleLogLogger *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleLogLogger logger
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
		 * Argument Name: logger_data
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr logger_data
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
		 * Argument Name: tm *tm
		 * Argument Type: struct
		 * Argument Category: InnerStruct
		 */
		/*
		 * Could not generate a wrapper for PurpleLog.tm *tm in file "log.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

	}


	[StructLayout(LayoutKind.Sequential)]
	public class _PurpleLog
	{
		/*
		 * PurpleLogType type
		 */
		public Log.PurpleLogType type;

		/*
		 * char * name
		 */
		public string name;

		/*
		 * PurpleAccount * account
		 */
		public IntPtr account;

		/*
		 * PurpleConversation * conv
		 */
		public IntPtr conv;

		/*
		 * time_t time
		 */
		public ulong time;

		/*
		 * PurpleLogLogger * logger
		 */
		public IntPtr logger;

		/*
		 * void * logger_data
		 */
		public IntPtr logger_data;

		/*
		 * struct tm *tm
		 */
		/*
		 * Could not generate a wrapper for PurpleLog.tm *tm in file "log.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

	}

}


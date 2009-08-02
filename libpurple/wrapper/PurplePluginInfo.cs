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
	 * File: plugin.h
	 * Structure: PurplePluginInfo
	 */
	public class PurplePluginInfo : UnmanagedWrapper<_PurplePluginInfo>
	{
		public PurplePluginInfo()
		{
		}

		public PurplePluginInfo(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurplePluginInfo)Marshal.PtrToStructure(this.Reference, typeof(_PurplePluginInfo));
		}

		/*
		 * Argument Name: magic
		 * Argument Type: unsigned int
		 * Argument Category: Native
		 */
		public uint magic
		{
			get
			{
				return this.Data.magic;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.magic = value;
			}
		}

		/*
		 * Argument Name: major_version
		 * Argument Type: unsigned int
		 * Argument Category: Native
		 */
		public uint major_version
		{
			get
			{
				return this.Data.major_version;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.major_version = value;
			}
		}

		/*
		 * Argument Name: minor_version
		 * Argument Type: unsigned int
		 * Argument Category: Native
		 */
		public uint minor_version
		{
			get
			{
				return this.Data.minor_version;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.minor_version = value;
			}
		}

		/*
		 * Argument Name: type
		 * Argument Type: PurplePluginType
		 * Argument Category: KnownEnum
		 */
		public Plugin.PurplePluginType type
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
		 * Argument Name: ui_requirement
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string ui_requirement
		{
			get
			{
				return this.Data.ui_requirement;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.ui_requirement = value;
			}
		}

		/*
		 * Argument Name: flags
		 * Argument Type: unsigned long
		 * Argument Category: Native
		 */
		public ulong flags
		{
			get
			{
				return this.Data.flags;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.flags = value;
			}
		}

		/*
		 * Argument Name: dependencies
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurplePluginInfo.dependencies in file "plugin.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Argument Name: priority
		 * Argument Type: PurplePluginPriority
		 * Argument Category: Native
		 */
		public int priority
		{
			get
			{
				return this.Data.priority;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.priority = value;
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
		 * Argument Name: version
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string version
		{
			get
			{
				return this.Data.version;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.version = value;
			}
		}

		/*
		 * Argument Name: summary
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string summary
		{
			get
			{
				return this.Data.summary;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.summary = value;
			}
		}

		/*
		 * Argument Name: description
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string description
		{
			get
			{
				return this.Data.description;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.description = value;
			}
		}

		/*
		 * Argument Name: author
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string author
		{
			get
			{
				return this.Data.author;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.author = value;
			}
		}

		/*
		 * Argument Name: homepage
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string homepage
		{
			get
			{
				return this.Data.homepage;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.homepage = value;
			}
		}

		/*
		 * Argument Name: load
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr load
		{
			get
			{
				return this.Data.load;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.load = value;
			}
		}

		/*
		 * Argument Name: unload
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr unload
		{
			get
			{
				return this.Data.unload;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.unload = value;
			}
		}

		/*
		 * Argument Name: destroy
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr destroy
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
		 * Argument Name: ui_info
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr ui_info
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
		 * Argument Name: extra_info
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr extra_info
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
		 * Argument Name: prefs_info
		 * Argument Type: PurplePluginUiInfo *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurplePluginUiInfo prefs_info
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
		 * Argument Name: actions
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		public IntPtr actions
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
		 * Argument Name: _purple_reserved1
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr _purple_reserved1
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
		 * Argument Name: _purple_reserved2
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr _purple_reserved2
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
		 * Argument Name: _purple_reserved3
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr _purple_reserved3
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
		 * Argument Name: _purple_reserved4
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr _purple_reserved4
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
	public class _PurplePluginInfo
	{
		/*
		 * unsigned int magic
		 */
		public uint magic;

		/*
		 * unsigned int major_version
		 */
		public uint major_version;

		/*
		 * unsigned int minor_version
		 */
		public uint minor_version;

		/*
		 * PurplePluginType type
		 */
		public Plugin.PurplePluginType type;

		/*
		 * char * ui_requirement
		 */
		public string ui_requirement;

		/*
		 * unsigned long flags
		 */
		public ulong flags;

		/*
		 * GList * dependencies
		 */
		public IntPtr dependencies;

		/*
		 * PurplePluginPriority priority
		 */
		public int priority;

		/*
		 * char * id
		 */
		public string id;

		/*
		 * char * name
		 */
		public string name;

		/*
		 * char * version
		 */
		public string version;

		/*
		 * char * summary
		 */
		public string summary;

		/*
		 * char * description
		 */
		public string description;

		/*
		 * char * author
		 */
		public string author;

		/*
		 * char * homepage
		 */
		public string homepage;

		/*
		 * gboolean (*load)(PurplePlugin * plugin)
		 */
		public IntPtr load;

		/*
		 * gboolean (*unload)(PurplePlugin * plugin)
		 */
		public IntPtr unload;

		/*
		 * void (*destroy)(PurplePlugin * plugin)
		 */
		public IntPtr destroy;

		/*
		 * void * ui_info
		 */
		public IntPtr ui_info;

		/*
		 * void * extra_info
		 */
		public IntPtr extra_info;

		/*
		 * PurplePluginUiInfo * prefs_info
		 */
		public IntPtr prefs_info;

		/*
		 * GList * (*actions)(PurplePlugin * plugin, gpointer context)
		 */
		public IntPtr actions;

		/*
		 * void (*_purple_reserved1)()
		 */
		public IntPtr _purple_reserved1;

		/*
		 * void (*_purple_reserved2)()
		 */
		public IntPtr _purple_reserved2;

		/*
		 * void (*_purple_reserved3)()
		 */
		public IntPtr _purple_reserved3;

		/*
		 * void (*_purple_reserved4)()
		 */
		public IntPtr _purple_reserved4;

	}

}


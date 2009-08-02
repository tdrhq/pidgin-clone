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
	 * Structure: PurplePlugin
	 */
	public class PurplePlugin : UnmanagedWrapper<_PurplePlugin>
	{
		public PurplePlugin()
		{
		}

		public PurplePlugin(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurplePlugin)Marshal.PtrToStructure(this.Reference, typeof(_PurplePlugin));
		}

		/*
		 * Argument Name: native_plugin
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool native_plugin
		{
			get
			{
				return this.Data.native_plugin;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.native_plugin = value;
			}
		}

		/*
		 * Argument Name: loaded
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool loaded
		{
			get
			{
				return this.Data.loaded;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.loaded = value;
			}
		}

		/*
		 * Argument Name: handle
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr handle
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
		 * Argument Name: path
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string path
		{
			get
			{
				return this.Data.path;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.path = value;
			}
		}

		/*
		 * Argument Name: info
		 * Argument Type: PurplePluginInfo *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurplePluginInfo info
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
		 * Argument Name: error
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public string error
		{
			get
			{
				return this.Data.error;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.error = value;
			}
		}

		/*
		 * Argument Name: ipc_data
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr ipc_data
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
		 * Argument Name: extra
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr extra
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
		 * Argument Name: unloadable
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public bool unloadable
		{
			get
			{
				return this.Data.unloadable;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.unloadable = value;
			}
		}

		/*
		 * Argument Name: dependent_plugins
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurplePlugin.dependent_plugins in file "plugin.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

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
	public class _PurplePlugin
	{
		/*
		 * gboolean native_plugin
		 */
		public bool native_plugin;

		/*
		 * gboolean loaded
		 */
		public bool loaded;

		/*
		 * void * handle
		 */
		public IntPtr handle;

		/*
		 * char * path
		 */
		public string path;

		/*
		 * PurplePluginInfo * info
		 */
		public IntPtr info;

		/*
		 * char * error
		 */
		public string error;

		/*
		 * void * ipc_data
		 */
		public IntPtr ipc_data;

		/*
		 * void * extra
		 */
		public IntPtr extra;

		/*
		 * gboolean unloadable
		 */
		public bool unloadable;

		/*
		 * GList * dependent_plugins
		 */
		public IntPtr dependent_plugins;

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


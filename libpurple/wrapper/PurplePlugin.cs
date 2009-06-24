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
	public class PurplePlugin : UnmanagedWrapper<_PurplePlugin>
	{
		public PurplePlugin()
		{
		}

		public PurplePlugin(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurplePlugin)Marshal.PtrToStructure(this.Reference, typeof(_PurplePlugin));
		}

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

		public GList dependent_plugins
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

		public void _purple_reserved1
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

		public void _purple_reserved2
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

		public void _purple_reserved3
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

		public void _purple_reserved4
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
	class _PurplePlugin
	{
		/*
		 * gboolean native_plugin
		 */
		bool native_plugin;

		/*
		 * gboolean loaded
		 */
		bool loaded;

		/*
		 * void * handle
		 */
		IntPtr handle;

		/*
		 * char * path
		 */
		string path;

		/*
		 * PurplePluginInfo * info
		 */
		IntPtr info;

		/*
		 * char * error
		 */
		string error;

		/*
		 * void * ipc_data
		 */
		IntPtr ipc_data;

		/*
		 * void * extra
		 */
		IntPtr extra;

		/*
		 * gboolean unloadable
		 */
		bool unloadable;

		/*
		 * GList * dependent_plugins
		 */
		IntPtr dependent_plugins;

		/*
		 * void (*_purple_reserved1)()
		 */
		void _purple_reserved1;

		/*
		 * void (*_purple_reserved2)()
		 */
		void _purple_reserved2;

		/*
		 * void (*_purple_reserved3)()
		 */
		void _purple_reserved3;

		/*
		 * void (*_purple_reserved4)()
		 */
		void _purple_reserved4;

	}
}


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
	public class PurpleAccount : UnmanagedWrapper<_PurpleAccount>
	{
		public PurpleAccount()
		{
		}

		public PurpleAccount(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleAccount)Marshal.PtrToStructure(this.Reference, typeof(_PurpleAccount));
		}

		public string username
		{
			get
			{
				return this.Data.username;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.username = value;
			}
		}

		public string alias
		{
			get
			{
				return this.Data.alias;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.alias = value;
			}
		}

		public string password
		{
			get
			{
				return this.Data.password;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.password = value;
			}
		}

		public string user_info
		{
			get
			{
				return this.Data.user_info;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.user_info = value;
			}
		}

		public string buddy_icon_path
		{
			get
			{
				return this.Data.buddy_icon_path;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.buddy_icon_path = value;
			}
		}

		public bool remember_pass
		{
			get
			{
				return this.Data.remember_pass;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.remember_pass = value;
			}
		}

		public string protocol_id
		{
			get
			{
				return this.Data.protocol_id;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.protocol_id = value;
			}
		}

		public PurpleConnection gc
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

		public bool disconnecting
		{
			get
			{
				return this.Data.disconnecting;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.disconnecting = value;
			}
		}

		public GHashTable settings
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

		public GHashTable ui_settings
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

		public PurpleProxyInfo proxy_info
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

		public GSList permit
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

		public GSList deny
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

		public PurplePrivacyType perm_deny
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

		public GList status_types
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

		public PurplePresence presence
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

		public PurpleLog system_log
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

		public PurpleAccountRegistrationCb registration_cb
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

		public IntPtr registration_cb_user_data
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

		public IntPtr priv
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
	class _PurpleAccount
	{
		/*
		 * char * username
		 */
		string username;

		/*
		 * char * alias
		 */
		string alias;

		/*
		 * char * password
		 */
		string password;

		/*
		 * char * user_info
		 */
		string user_info;

		/*
		 * char * buddy_icon_path
		 */
		string buddy_icon_path;

		/*
		 * gboolean remember_pass
		 */
		bool remember_pass;

		/*
		 * char * protocol_id
		 */
		string protocol_id;

		/*
		 * PurpleConnection * gc
		 */
		IntPtr gc;

		/*
		 * gboolean disconnecting
		 */
		bool disconnecting;

		/*
		 * GHashTable * settings
		 */
		IntPtr settings;

		/*
		 * GHashTable * ui_settings
		 */
		IntPtr ui_settings;

		/*
		 * PurpleProxyInfo * proxy_info
		 */
		IntPtr proxy_info;

		/*
		 * GSList * permit
		 */
		IntPtr permit;

		/*
		 * GSList * deny
		 */
		IntPtr deny;

		/*
		 * PurplePrivacyType perm_deny
		 */
		UNKNOWN perm_deny;

		/*
		 * GList * status_types
		 */
		IntPtr status_types;

		/*
		 * PurplePresence * presence
		 */
		IntPtr presence;

		/*
		 * PurpleLog * system_log
		 */
		IntPtr system_log;

		/*
		 * void * ui_data
		 */
		IntPtr ui_data;

		/*
		 * PurpleAccountRegistrationCb registration_cb
		 */
		UNKNOWN registration_cb;

		/*
		 * void * registration_cb_user_data
		 */
		IntPtr registration_cb_user_data;

		/*
		 * gpointer priv
		 */
		IntPtr priv;

	}
}


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
	 * File: account.h
	 * Structure: PurpleAccount
	 */
	public class PurpleAccount : UnmanagedWrapper<_PurpleAccount>
	{
		public PurpleAccount()
		{
		}

		public PurpleAccount(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleAccount)Marshal.PtrToStructure(this.Reference, typeof(_PurpleAccount));
		}

		/*
		 * Argument Name: username
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: alias
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: password
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: user_info
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: buddy_icon_path
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: remember_pass
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: protocol_id
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: gc
		 * Argument Type: PurpleConnection *
		 * Argument Category: PointerToKnownStruct
		 */
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

		/*
		 * Argument Name: disconnecting
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: settings
		 * Argument Type: GHashTable *
		 * Argument Category: PointerToGHashTable
		 */
		/*
		 * Could not generate a wrapper for PurpleAccount.settings in file "account.h".
		 * Message: Unable to provide a type for HashTable as no function information was provided in code.
		 */

		/*
		 * Argument Name: ui_settings
		 * Argument Type: GHashTable *
		 * Argument Category: PointerToGHashTable
		 */
		/*
		 * Could not generate a wrapper for PurpleAccount.ui_settings in file "account.h".
		 * Message: Unable to provide a type for HashTable as no function information was provided in code.
		 */

		/*
		 * Argument Name: proxy_info
		 * Argument Type: PurpleProxyInfo *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr proxy_info
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
		 * Argument Name: permit
		 * Argument Type: GSList *
		 * Argument Category: PointerToGSList
		 */
		/*
		 * Could not generate a wrapper for PurpleAccount.permit in file "account.h".
		 * Message: Unable to provide a type for GSList as no function information was provided in code.
		 */

		/*
		 * Argument Name: deny
		 * Argument Type: GSList *
		 * Argument Category: PointerToGSList
		 */
		/*
		 * Could not generate a wrapper for PurpleAccount.deny in file "account.h".
		 * Message: Unable to provide a type for GSList as no function information was provided in code.
		 */

		/*
		 * Argument Name: perm_deny
		 * Argument Type: PurplePrivacyType
		 * Argument Category: KnownEnum
		 */
		public Privacy.PurplePrivacyType perm_deny
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
		 * Argument Name: status_types
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurpleAccount.status_types in file "account.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Argument Name: presence
		 * Argument Type: PurplePresence *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence
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
		 * Argument Name: system_log
		 * Argument Type: PurpleLog *
		 * Argument Category: PointerToKnownStruct
		 */
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

		/*
		 * Argument Name: ui_data
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
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

		/*
		 * Argument Name: registration_cb
		 * Argument Type: PurpleAccountRegistrationCb
		 * Argument Category: KnownFunctionPointer
		 */
		public Account.PurpleAccountRegistrationCb registration_cb
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
		 * Argument Name: registration_cb_user_data
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
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

		/*
		 * Argument Name: priv
		 * Argument Type: gpointer
		 * Argument Category: VoidPointer
		 */
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
	public class _PurpleAccount
	{
		/*
		 * char * username
		 */
		public string username;

		/*
		 * char * alias
		 */
		public string alias;

		/*
		 * char * password
		 */
		public string password;

		/*
		 * char * user_info
		 */
		public string user_info;

		/*
		 * char * buddy_icon_path
		 */
		public string buddy_icon_path;

		/*
		 * gboolean remember_pass
		 */
		public bool remember_pass;

		/*
		 * char * protocol_id
		 */
		public string protocol_id;

		/*
		 * PurpleConnection * gc
		 */
		public IntPtr gc;

		/*
		 * gboolean disconnecting
		 */
		public bool disconnecting;

		/*
		 * GHashTable * settings
		 */
		public IntPtr settings;

		/*
		 * GHashTable * ui_settings
		 */
		public IntPtr ui_settings;

		/*
		 * PurpleProxyInfo * proxy_info
		 */
		public IntPtr proxy_info;

		/*
		 * GSList * permit
		 */
		public IntPtr permit;

		/*
		 * GSList * deny
		 */
		public IntPtr deny;

		/*
		 * PurplePrivacyType perm_deny
		 */
		public Privacy.PurplePrivacyType perm_deny;

		/*
		 * GList * status_types
		 */
		public IntPtr status_types;

		/*
		 * PurplePresence * presence
		 */
		public IntPtr presence;

		/*
		 * PurpleLog * system_log
		 */
		public IntPtr system_log;

		/*
		 * void * ui_data
		 */
		public IntPtr ui_data;

		/*
		 * PurpleAccountRegistrationCb registration_cb
		 */
		public IntPtr registration_cb;

		/*
		 * void * registration_cb_user_data
		 */
		public IntPtr registration_cb_user_data;

		/*
		 * gpointer priv
		 */
		public IntPtr priv;

	}

}


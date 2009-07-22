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
	public class PurpleConversationUiOps : UnmanagedWrapper<_PurpleConversationUiOps>
	{
		public PurpleConversationUiOps()
		{
		}

		public PurpleConversationUiOps(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleConversationUiOps)Marshal.PtrToStructure(this.Reference, typeof(_PurpleConversationUiOps));
		}

		public IntPtr create_conversation
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

		public IntPtr destroy_conversation
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

		public IntPtr write_chat
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

		public IntPtr write_im
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

		public IntPtr write_conv
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

		public IntPtr chat_add_users
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

		public IntPtr chat_rename_user
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

		public IntPtr chat_remove_users
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

		public IntPtr chat_update_user
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

		public IntPtr present
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

		public IntPtr has_focus
		{
			get
			{
				return this.Data.has_focus;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.has_focus = value;
			}
		}

		public IntPtr custom_smiley_add
		{
			get
			{
				return this.Data.custom_smiley_add;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.custom_smiley_add = value;
			}
		}

		public IntPtr custom_smiley_write
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

		public IntPtr custom_smiley_close
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

		public IntPtr send_confirm
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
	class _PurpleConversationUiOps
	{
		/*
		 * void (*create_conversation)(PurpleConversation * conv)
		 */
		IntPtr create_conversation;

		/*
		 * void (*destroy_conversation)(PurpleConversation * conv)
		 */
		IntPtr destroy_conversation;

		/*
		 * void (*write_chat)(PurpleConversation * conv, char * who, char * message, PurpleMessageFlags flags, time_t mtime)
		 */
		IntPtr write_chat;

		/*
		 * void (*write_im)(PurpleConversation * conv, char * who, char * message, PurpleMessageFlags flags, time_t mtime)
		 */
		IntPtr write_im;

		/*
		 * void (*write_conv)(PurpleConversation * conv, char * name, char * alias, char * message, PurpleMessageFlags flags, time_t mtime)
		 */
		IntPtr write_conv;

		/*
		 * void (*chat_add_users)(PurpleConversation * conv, GList * cbuddies, gboolean new_arrivals)
		 */
		IntPtr chat_add_users;

		/*
		 * void (*chat_rename_user)(PurpleConversation * conv, char * old_name, char * new_name, char * new_alias)
		 */
		IntPtr chat_rename_user;

		/*
		 * void (*chat_remove_users)(PurpleConversation * conv, GList * users)
		 */
		IntPtr chat_remove_users;

		/*
		 * void (*chat_update_user)(PurpleConversation * conv, char * user)
		 */
		IntPtr chat_update_user;

		/*
		 * void (*present)(PurpleConversation * conv)
		 */
		IntPtr present;

		/*
		 * gboolean (*has_focus)(PurpleConversation * conv)
		 */
		IntPtr has_focus;

		/*
		 * gboolean (*custom_smiley_add)(PurpleConversation * conv, char * smile, gboolean remote)
		 */
		IntPtr custom_smiley_add;

		/*
		 * void (*custom_smiley_write)(PurpleConversation * conv, char * smile, guchar * data, gsize size)
		 */
		IntPtr custom_smiley_write;

		/*
		 * void (*custom_smiley_close)(PurpleConversation * conv, char * smile)
		 */
		IntPtr custom_smiley_close;

		/*
		 * void (*send_confirm)(PurpleConversation * conv, char * message)
		 */
		IntPtr send_confirm;

		/*
		 * void (*_purple_reserved1)()
		 */
		IntPtr _purple_reserved1;

		/*
		 * void (*_purple_reserved2)()
		 */
		IntPtr _purple_reserved2;

		/*
		 * void (*_purple_reserved3)()
		 */
		IntPtr _purple_reserved3;

		/*
		 * void (*_purple_reserved4)()
		 */
		IntPtr _purple_reserved4;

	}

}


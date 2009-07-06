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

		public void create_conversation
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

		public void destroy_conversation
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

		public void write_chat
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

		public void write_im
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

		public void write_conv
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

		public void chat_add_users
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

		public void chat_rename_user
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

		public void chat_remove_users
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

		public void chat_update_user
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

		public void present
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

		public bool has_focus
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

		public bool custom_smiley_add
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

		public void custom_smiley_write
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

		public void custom_smiley_close
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

		public void send_confirm
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

		public void purple_reserved1
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

		public void purple_reserved2
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

		public void purple_reserved3
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

		public void purple_reserved4
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
		void create_conversation;

		/*
		 * void (*destroy_conversation)(PurpleConversation * conv)
		 */
		void destroy_conversation;

		/*
		 * void (*write_chat)(PurpleConversation * conv, char * who, char * message, PurpleMessageFlags flags, time_t mtime)
		 */
		void write_chat;

		/*
		 * void (*write_im)(PurpleConversation * conv, char * who, char * message, PurpleMessageFlags flags, time_t mtime)
		 */
		void write_im;

		/*
		 * void (*write_conv)(PurpleConversation * conv, char * name, char * alias, char * message, PurpleMessageFlags flags, time_t mtime)
		 */
		void write_conv;

		/*
		 * void (*chat_add_users)(PurpleConversation * conv, GList * cbuddies, gboolean new_arrivals)
		 */
		void chat_add_users;

		/*
		 * void (*chat_rename_user)(PurpleConversation * conv, char * old_name, char * new_name, char * new_alias)
		 */
		void chat_rename_user;

		/*
		 * void (*chat_remove_users)(PurpleConversation * conv, GList * users)
		 */
		void chat_remove_users;

		/*
		 * void (*chat_update_user)(PurpleConversation * conv, char * user)
		 */
		void chat_update_user;

		/*
		 * void (*present)(PurpleConversation * conv)
		 */
		void present;

		/*
		 * gboolean (*has_focus)(PurpleConversation * conv)
		 */
		bool has_focus;

		/*
		 * gboolean (*custom_smiley_add)(PurpleConversation * conv, char * smile, gboolean remote)
		 */
		bool custom_smiley_add;

		/*
		 * void (*custom_smiley_write)(PurpleConversation * conv, char * smile, guchar * data, gsize size)
		 */
		void custom_smiley_write;

		/*
		 * void (*custom_smiley_close)(PurpleConversation * conv, char * smile)
		 */
		void custom_smiley_close;

		/*
		 * void (*send_confirm)(PurpleConversation * conv, char * message)
		 */
		void send_confirm;

		/*
		 * void (*purple_reserved1)()
		 */
		void purple_reserved1;

		/*
		 * void (*purple_reserved2)()
		 */
		void purple_reserved2;

		/*
		 * void (*purple_reserved3)()
		 */
		void purple_reserved3;

		/*
		 * void (*purple_reserved4)()
		 */
		void purple_reserved4;

	}

}


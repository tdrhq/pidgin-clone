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
	 * File: prpl.h
	 * Structure: PurplePluginProtocolInfo
	 */
	public class PurplePluginProtocolInfo : UnmanagedWrapper<_PurplePluginProtocolInfo>
	{
		public PurplePluginProtocolInfo()
		{
		}

		public PurplePluginProtocolInfo(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurplePluginProtocolInfo)Marshal.PtrToStructure(this.Reference, typeof(_PurplePluginProtocolInfo));
		}

		/*
		 * Argument Name: options
		 * Argument Type: PurpleProtocolOptions
		 * Argument Category: KnownEnum
		 */
		public Prpl.PurpleProtocolOptions options
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
		 * Argument Name: user_splits
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurplePluginProtocolInfo.user_splits in file "prpl.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Argument Name: protocol_options
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		/*
		 * Could not generate a wrapper for PurplePluginProtocolInfo.protocol_options in file "prpl.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Argument Name: icon_spec
		 * Argument Type: PurpleBuddyIconSpec
		 * Argument Category: KnownStruct
		 */
		/*
		 * Could not generate a wrapper for PurplePluginProtocolInfo.icon_spec in file "prpl.h".
		 * Message: The struct contains an object that resolves to a function that is not a function pointer (PurpleBuddyIconSpec icon_spec).
		 */

		/*
		 * Argument Name: list_icon
		 * Argument Type: const char *
		 * Argument Category: PointerToUnknownStruct
		 */
		public IntPtr list_icon
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
		 * Argument Name: list_emblem
		 * Argument Type: const char *
		 * Argument Category: PointerToUnknownStruct
		 */
		public IntPtr list_emblem
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
		 * Argument Name: status_text
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public IntPtr status_text
		{
			get
			{
				return this.Data.status_text;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.status_text = value;
			}
		}

		/*
		 * Argument Name: tooltip_text
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr tooltip_text
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
		public IntPtr status_types
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
		 * Argument Name: blist_node_menu
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		public IntPtr blist_node_menu
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
		 * Argument Name: chat_info
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		public IntPtr chat_info
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
		 * Argument Name: chat_info_defaults
		 * Argument Type: GHashTable *
		 * Argument Category: PointerToGHashTable
		 */
		public IntPtr chat_info_defaults
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
		 * Argument Name: login
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr login
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
		 * Argument Name: close
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr close
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
		 * Argument Name: send_im
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public IntPtr send_im
		{
			get
			{
				return this.Data.send_im;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.send_im = value;
			}
		}

		/*
		 * Argument Name: set_info
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_info
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
		 * Argument Name: send_typing
		 * Argument Type: unsigned int
		 * Argument Category: Native
		 */
		public IntPtr send_typing
		{
			get
			{
				return this.Data.send_typing;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.send_typing = value;
			}
		}

		/*
		 * Argument Name: get_info
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr get_info
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
		 * Argument Name: set_status
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_status
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
		 * Argument Name: set_idle
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_idle
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
		 * Argument Name: change_passwd
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr change_passwd
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
		 * Argument Name: add_buddy
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr add_buddy
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
		 * Argument Name: add_buddies
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr add_buddies
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
		 * Argument Name: remove_buddy
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr remove_buddy
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
		 * Argument Name: remove_buddies
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr remove_buddies
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
		 * Argument Name: add_permit
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr add_permit
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
		 * Argument Name: add_deny
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr add_deny
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
		 * Argument Name: rem_permit
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr rem_permit
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
		 * Argument Name: rem_deny
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr rem_deny
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
		 * Argument Name: set_permit_deny
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_permit_deny
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
		 * Argument Name: join_chat
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr join_chat
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
		 * Argument Name: reject_chat
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr reject_chat
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
		 * Argument Name: get_chat_name
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public IntPtr get_chat_name
		{
			get
			{
				return this.Data.get_chat_name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.get_chat_name = value;
			}
		}

		/*
		 * Argument Name: chat_invite
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr chat_invite
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
		 * Argument Name: chat_leave
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr chat_leave
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
		 * Argument Name: chat_whisper
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr chat_whisper
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
		 * Argument Name: chat_send
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public IntPtr chat_send
		{
			get
			{
				return this.Data.chat_send;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.chat_send = value;
			}
		}

		/*
		 * Argument Name: keepalive
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr keepalive
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
		 * Argument Name: register_user
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr register_user
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
		 * Argument Name: get_cb_info
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr get_cb_info
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
		 * Argument Name: get_cb_away
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr get_cb_away
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
		 * Argument Name: alias_buddy
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr alias_buddy
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
		 * Argument Name: group_buddy
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr group_buddy
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
		 * Argument Name: rename_group
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr rename_group
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
		 * Argument Name: buddy_free
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr buddy_free
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
		 * Argument Name: convo_closed
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr convo_closed
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
		 * Argument Name: normalize
		 * Argument Type: const char *
		 * Argument Category: PointerToUnknownStruct
		 */
		public IntPtr normalize
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
		 * Argument Name: set_buddy_icon
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_buddy_icon
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
		 * Argument Name: remove_group
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr remove_group
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
		 * Argument Name: get_cb_real_name
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public IntPtr get_cb_real_name
		{
			get
			{
				return this.Data.get_cb_real_name;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.get_cb_real_name = value;
			}
		}

		/*
		 * Argument Name: set_chat_topic
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_chat_topic
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
		 * Argument Name: find_blist_chat
		 * Argument Type: PurpleChat *
		 * Argument Category: PointerToKnownStruct
		 */
		public IntPtr find_blist_chat
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
		 * Argument Name: roomlist_get_list
		 * Argument Type: PurpleRoomlist *
		 * Argument Category: PointerToKnownStruct
		 */
		public IntPtr roomlist_get_list
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
		 * Argument Name: roomlist_cancel
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr roomlist_cancel
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
		 * Argument Name: roomlist_expand_category
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr roomlist_expand_category
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
		 * Argument Name: can_receive_file
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr can_receive_file
		{
			get
			{
				return this.Data.can_receive_file;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.can_receive_file = value;
			}
		}

		/*
		 * Argument Name: send_file
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr send_file
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
		 * Argument Name: new_xfer
		 * Argument Type: PurpleXfer *
		 * Argument Category: PointerToKnownStruct
		 */
		public IntPtr new_xfer
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
		 * Argument Name: offline_message
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr offline_message
		{
			get
			{
				return this.Data.offline_message;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.offline_message = value;
			}
		}

		/*
		 * Argument Name: whiteboard_prpl_ops
		 * Argument Type: PurpleWhiteboardPrplOps *
		 * Argument Category: PointerToKnownStruct
		 */
		public PurpleWhiteboardPrplOps whiteboard_prpl_ops
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
		 * Argument Name: send_raw
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public IntPtr send_raw
		{
			get
			{
				return this.Data.send_raw;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.send_raw = value;
			}
		}

		/*
		 * Argument Name: roomlist_room_serialize
		 * Argument Type: char *
		 * Argument Category: Native
		 */
		public IntPtr roomlist_room_serialize
		{
			get
			{
				return this.Data.roomlist_room_serialize;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.roomlist_room_serialize = value;
			}
		}

		/*
		 * Argument Name: unregister_user
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr unregister_user
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
		 * Argument Name: send_attention
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr send_attention
		{
			get
			{
				return this.Data.send_attention;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.send_attention = value;
			}
		}

		/*
		 * Argument Name: get_attention_types
		 * Argument Type: GList *
		 * Argument Category: PointerToGList
		 */
		public IntPtr get_attention_types
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
		 * Argument Name: struct_size
		 * Argument Type: unsigned long
		 * Argument Category: Native
		 */
		public ulong struct_size
		{
			get
			{
				return this.Data.struct_size;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.struct_size = value;
			}
		}

		/*
		 * Argument Name: get_account_text_table
		 * Argument Type: GHashTable *
		 * Argument Category: PointerToGHashTable
		 */
		public IntPtr get_account_text_table
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
		 * Argument Name: initiate_media
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr initiate_media
		{
			get
			{
				return this.Data.initiate_media;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.initiate_media = value;
			}
		}

		/*
		 * Argument Name: get_media_caps
		 * Argument Type: PurpleMediaCaps
		 * Argument Category: KnownEnum
		 */
		public IntPtr get_media_caps
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
	public class _PurplePluginProtocolInfo
	{
		/*
		 * PurpleProtocolOptions options
		 */
		public Prpl.PurpleProtocolOptions options;

		/*
		 * GList * user_splits
		 */
		public IntPtr user_splits;

		/*
		 * GList * protocol_options
		 */
		public IntPtr protocol_options;

		/*
		 * PurpleBuddyIconSpec icon_spec
		 */
		/*
		 * Could not generate a wrapper for PurplePluginProtocolInfo.icon_spec in file "prpl.h".
		 * Message: The struct contains an object that resolves to a function that is not a function pointer (PurpleBuddyIconSpec icon_spec).
		 */

		/*
		 * const char * (*list_icon)(PurpleAccount * account, PurpleBuddy * buddy)
		 */
		public IntPtr list_icon;

		/*
		 * const char * (*list_emblem)(PurpleBuddy * buddy)
		 */
		public IntPtr list_emblem;

		/*
		 * char * (*status_text)(PurpleBuddy * buddy)
		 */
		public IntPtr status_text;

		/*
		 * void (*tooltip_text)(PurpleBuddy * buddy, PurpleNotifyUserInfo * user_info, gboolean full)
		 */
		public IntPtr tooltip_text;

		/*
		 * GList * (*status_types)(PurpleAccount * account)
		 */
		public IntPtr status_types;

		/*
		 * GList * (*blist_node_menu)(PurpleBlistNode * node)
		 */
		public IntPtr blist_node_menu;

		/*
		 * GList * (*chat_info)(PurpleConnection * )
		 */
		public IntPtr chat_info;

		/*
		 * GHashTable * (*chat_info_defaults)(PurpleConnection * , char * chat_name)
		 */
		public IntPtr chat_info_defaults;

		/*
		 * void (*login)(PurpleAccount * )
		 */
		public IntPtr login;

		/*
		 * void (*close)(PurpleConnection * )
		 */
		public IntPtr close;

		/*
		 * int (*send_im)(PurpleConnection * , char * who, char * message, PurpleMessageFlags flags)
		 */
		public IntPtr send_im;

		/*
		 * void (*set_info)(PurpleConnection * , char * info)
		 */
		public IntPtr set_info;

		/*
		 * unsigned int (*send_typing)(PurpleConnection * , char * name, PurpleTypingState state)
		 */
		public IntPtr send_typing;

		/*
		 * void (*get_info)(PurpleConnection * , char * who)
		 */
		public IntPtr get_info;

		/*
		 * void (*set_status)(PurpleAccount * account, PurpleStatus * status)
		 */
		public IntPtr set_status;

		/*
		 * void (*set_idle)(PurpleConnection * , int idletime)
		 */
		public IntPtr set_idle;

		/*
		 * void (*change_passwd)(PurpleConnection * , char * old_pass, char * new_pass)
		 */
		public IntPtr change_passwd;

		/*
		 * void (*add_buddy)(PurpleConnection * , PurpleBuddy * buddy, PurpleGroup * group)
		 */
		public IntPtr add_buddy;

		/*
		 * void (*add_buddies)(PurpleConnection * , GList * buddies, GList * groups)
		 */
		public IntPtr add_buddies;

		/*
		 * void (*remove_buddy)(PurpleConnection * , PurpleBuddy * buddy, PurpleGroup * group)
		 */
		public IntPtr remove_buddy;

		/*
		 * void (*remove_buddies)(PurpleConnection * , GList * buddies, GList * groups)
		 */
		public IntPtr remove_buddies;

		/*
		 * void (*add_permit)(PurpleConnection * , char * name)
		 */
		public IntPtr add_permit;

		/*
		 * void (*add_deny)(PurpleConnection * , char * name)
		 */
		public IntPtr add_deny;

		/*
		 * void (*rem_permit)(PurpleConnection * , char * name)
		 */
		public IntPtr rem_permit;

		/*
		 * void (*rem_deny)(PurpleConnection * , char * name)
		 */
		public IntPtr rem_deny;

		/*
		 * void (*set_permit_deny)(PurpleConnection * )
		 */
		public IntPtr set_permit_deny;

		/*
		 * void (*join_chat)(PurpleConnection * , GHashTable * components)
		 */
		public IntPtr join_chat;

		/*
		 * void (*reject_chat)(PurpleConnection * , GHashTable * components)
		 */
		public IntPtr reject_chat;

		/*
		 * char * (*get_chat_name)(GHashTable * components)
		 */
		public IntPtr get_chat_name;

		/*
		 * void (*chat_invite)(PurpleConnection * , int id, char * message, char * who)
		 */
		public IntPtr chat_invite;

		/*
		 * void (*chat_leave)(PurpleConnection * , int id)
		 */
		public IntPtr chat_leave;

		/*
		 * void (*chat_whisper)(PurpleConnection * , int id, char * who, char * message)
		 */
		public IntPtr chat_whisper;

		/*
		 * int (*chat_send)(PurpleConnection * , int id, char * message, PurpleMessageFlags flags)
		 */
		public IntPtr chat_send;

		/*
		 * void (*keepalive)(PurpleConnection * )
		 */
		public IntPtr keepalive;

		/*
		 * void (*register_user)(PurpleAccount * )
		 */
		public IntPtr register_user;

		/*
		 * void (*get_cb_info)(PurpleConnection * ,  , char * who)
		 */
		public IntPtr get_cb_info;

		/*
		 * void (*get_cb_away)(PurpleConnection * ,  , char * who)
		 */
		public IntPtr get_cb_away;

		/*
		 * void (*alias_buddy)(PurpleConnection * , char * who, char * alias)
		 */
		public IntPtr alias_buddy;

		/*
		 * void (*group_buddy)(PurpleConnection * , char * who, char * old_group, char * new_group)
		 */
		public IntPtr group_buddy;

		/*
		 * void (*rename_group)(PurpleConnection * , char * old_name, PurpleGroup * group, GList * moved_buddies)
		 */
		public IntPtr rename_group;

		/*
		 * void (*buddy_free)(PurpleBuddy * )
		 */
		public IntPtr buddy_free;

		/*
		 * void (*convo_closed)(PurpleConnection * , char * who)
		 */
		public IntPtr convo_closed;

		/*
		 * const char * (*normalize)(PurpleAccount * , char * who)
		 */
		public IntPtr normalize;

		/*
		 * void (*set_buddy_icon)(PurpleConnection * , PurpleStoredImage * img)
		 */
		public IntPtr set_buddy_icon;

		/*
		 * void (*remove_group)(PurpleConnection * gc, PurpleGroup * group)
		 */
		public IntPtr remove_group;

		/*
		 * char * (*get_cb_real_name)(PurpleConnection * gc, int id, char * who)
		 */
		public IntPtr get_cb_real_name;

		/*
		 * void (*set_chat_topic)(PurpleConnection * gc, int id, char * topic)
		 */
		public IntPtr set_chat_topic;

		/*
		 * PurpleChat * (*find_blist_chat)(PurpleAccount * account, char * name)
		 */
		public IntPtr find_blist_chat;

		/*
		 * PurpleRoomlist * (*roomlist_get_list)(PurpleConnection * gc)
		 */
		public IntPtr roomlist_get_list;

		/*
		 * void (*roomlist_cancel)(PurpleRoomlist * list)
		 */
		public IntPtr roomlist_cancel;

		/*
		 * void (*roomlist_expand_category)(PurpleRoomlist * list, PurpleRoomlistRoom * category)
		 */
		public IntPtr roomlist_expand_category;

		/*
		 * gboolean (*can_receive_file)(PurpleConnection * , char * who)
		 */
		public IntPtr can_receive_file;

		/*
		 * void (*send_file)(PurpleConnection * , char * who, char * filename)
		 */
		public IntPtr send_file;

		/*
		 * PurpleXfer * (*new_xfer)(PurpleConnection * , char * who)
		 */
		public IntPtr new_xfer;

		/*
		 * gboolean (*offline_message)(PurpleBuddy * buddy)
		 */
		public IntPtr offline_message;

		/*
		 * PurpleWhiteboardPrplOps * whiteboard_prpl_ops
		 */
		public IntPtr whiteboard_prpl_ops;

		/*
		 * int (*send_raw)(PurpleConnection * gc, char * buf, int len)
		 */
		public IntPtr send_raw;

		/*
		 * char * (*roomlist_room_serialize)(PurpleRoomlistRoom * room)
		 */
		public IntPtr roomlist_room_serialize;

		/*
		 * void (*unregister_user)(PurpleAccount * , PurpleAccountUnregistrationCb cb, void * user_data)
		 */
		public IntPtr unregister_user;

		/*
		 * gboolean (*send_attention)(PurpleConnection * gc, char * username, guint type)
		 */
		public IntPtr send_attention;

		/*
		 * GList * (*get_attention_types)(PurpleAccount * acct)
		 */
		public IntPtr get_attention_types;

		/*
		 * unsigned long struct_size
		 */
		public ulong struct_size;

		/*
		 * GHashTable * (*get_account_text_table)(PurpleAccount * account)
		 */
		public IntPtr get_account_text_table;

		/*
		 * gboolean (*initiate_media)(PurpleAccount * account, char * who, PurpleMediaSessionType type)
		 */
		public IntPtr initiate_media;

		/*
		 * PurpleMediaCaps (*get_media_caps)(PurpleAccount * account, char * who)
		 */
		public IntPtr get_media_caps;

	}

}


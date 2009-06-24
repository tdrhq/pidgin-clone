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
	public class PurplePluginProtocolInfo : UnmanagedWrapper<_PurplePluginProtocolInfo>
	{
		public PurplePluginProtocolInfo()
		{
		}

		public PurplePluginProtocolInfo(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurplePluginProtocolInfo)Marshal.PtrToStructure(this.Reference, typeof(_PurplePluginProtocolInfo));
		}

		public PurpleProtocolOptions options
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

		public GList user_splits
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

		public GList protocol_options
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

		public PurpleBuddyIconSpec icon_spec
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

		public const char *(*list_icon)(PurpleAccount *account, PurpleBuddy *buddy)
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

		public const char *(*list_emblem)(PurpleBuddy *buddy)
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

		public string status_text
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

		public void tooltip_text
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

		public GList blist_node_menu
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

		public GList chat_info
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

		public GHashTable chat_info_defaults
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

		public void login
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

		public void close
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

		public int send_im
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

		public void set_info
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

		public unsigned int (*send_typing)(PurpleConnection *, const char *name, PurpleTypingState state)
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

		public void get_info
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

		public void set_status
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

		public void set_idle
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

		public void change_passwd
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

		public void add_buddy
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

		public void add_buddies
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

		public void remove_buddy
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

		public void remove_buddies
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

		public void add_permit
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

		public void add_deny
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

		public void rem_permit
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

		public void rem_deny
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

		public void set_permit_deny
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

		public void join_chat
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

		public void reject_chat
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

		public string get_chat_name
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

		public void chat_invite
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

		public void chat_leave
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

		public void chat_whisper
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

		public int chat_send
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

		public void keepalive
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

		public void register_user
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

		public void get_cb_info
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

		public void get_cb_away
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

		public void alias_buddy
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

		public void group_buddy
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

		public void rename_group
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

		public void buddy_free
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

		public void convo_closed
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

		public const char *(*normalize)(const PurpleAccount *, const char *who)
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

		public void set_buddy_icon
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

		public void remove_group
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

		public string get_cb_real_name
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

		public void set_chat_topic
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

		public PurpleChat find_blist_chat
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

		public PurpleRoomlist roomlist_get_list
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

		public void roomlist_cancel
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

		public void roomlist_expand_category
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

		public bool can_receive_file
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

		public void send_file
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

		public PurpleXfer new_xfer
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

		public bool offline_message
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

		public int send_raw
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

		public string roomlist_room_serialize
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

		public void unregister_user
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

		public bool send_attention
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

		public GList get_attention_types
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

		public unsigned long struct_size
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

		public GHashTable get_account_text_table
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

		public bool initiate_media
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

		public PurpleMediaCaps get_media_caps
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
	class _PurplePluginProtocolInfo
	{
		/*
		 * PurpleProtocolOptions options
		 */
		UNKNOWN options;

		/*
		 * GList * user_splits
		 */
		IntPtr user_splits;

		/*
		 * GList * protocol_options
		 */
		IntPtr protocol_options;

		/*
		 * PurpleBuddyIconSpec icon_spec
		 */
		UNKNOWN icon_spec;

		/*
		 * const char *(*list_icon)(PurpleAccount *account, PurpleBuddy *buddy)
		 */
		UNKNOWN char *(*list_icon)(PurpleAccount *account, PurpleBuddy *buddy);

		/*
		 * const char *(*list_emblem)(PurpleBuddy *buddy)
		 */
		UNKNOWN char *(*list_emblem)(PurpleBuddy *buddy);

		/*
		 * char * (*status_text)(PurpleBuddy * buddy)
		 */
		string status_text;

		/*
		 * void (*tooltip_text)(PurpleBuddy * buddy, PurpleNotifyUserInfo * user_info, gboolean full)
		 */
		void tooltip_text;

		/*
		 * GList * (*status_types)(PurpleAccount * account)
		 */
		IntPtr status_types;

		/*
		 * GList * (*blist_node_menu)(PurpleBlistNode * node)
		 */
		IntPtr blist_node_menu;

		/*
		 * GList * (*chat_info)( )
		 */
		IntPtr chat_info;

		/*
		 * GHashTable * (*chat_info_defaults)( , char * chat_name)
		 */
		IntPtr chat_info_defaults;

		/*
		 * void (*login)( )
		 */
		void login;

		/*
		 * void (*close)( )
		 */
		void close;

		/*
		 * int (*send_im)( , char * who, char * message, PurpleMessageFlags flags)
		 */
		int send_im;

		/*
		 * void (*set_info)( , char * info)
		 */
		void set_info;

		/*
		 * unsigned int (*send_typing)(PurpleConnection *, const char *name, PurpleTypingState state)
		 */
		UNKNOWN int (*send_typing)(PurpleConnection *, const char *name, PurpleTypingState state);

		/*
		 * void (*get_info)( , char * who)
		 */
		void get_info;

		/*
		 * void (*set_status)(PurpleAccount * account, PurpleStatus * status)
		 */
		void set_status;

		/*
		 * void (*set_idle)( , int idletime)
		 */
		void set_idle;

		/*
		 * void (*change_passwd)( , char * old_pass, char * new_pass)
		 */
		void change_passwd;

		/*
		 * void (*add_buddy)( , PurpleBuddy * buddy, PurpleGroup * group)
		 */
		void add_buddy;

		/*
		 * void (*add_buddies)( , GList * buddies, GList * groups)
		 */
		void add_buddies;

		/*
		 * void (*remove_buddy)( , PurpleBuddy * buddy, PurpleGroup * group)
		 */
		void remove_buddy;

		/*
		 * void (*remove_buddies)( , GList * buddies, GList * groups)
		 */
		void remove_buddies;

		/*
		 * void (*add_permit)( , char * name)
		 */
		void add_permit;

		/*
		 * void (*add_deny)( , char * name)
		 */
		void add_deny;

		/*
		 * void (*rem_permit)( , char * name)
		 */
		void rem_permit;

		/*
		 * void (*rem_deny)( , char * name)
		 */
		void rem_deny;

		/*
		 * void (*set_permit_deny)( )
		 */
		void set_permit_deny;

		/*
		 * void (*join_chat)( , GHashTable * components)
		 */
		void join_chat;

		/*
		 * void (*reject_chat)( , GHashTable * components)
		 */
		void reject_chat;

		/*
		 * char * (*get_chat_name)(GHashTable * components)
		 */
		string get_chat_name;

		/*
		 * void (*chat_invite)( , int id, char * message, char * who)
		 */
		void chat_invite;

		/*
		 * void (*chat_leave)( , int id)
		 */
		void chat_leave;

		/*
		 * void (*chat_whisper)( , int id, char * who, char * message)
		 */
		void chat_whisper;

		/*
		 * int (*chat_send)( , int id, char * message, PurpleMessageFlags flags)
		 */
		int chat_send;

		/*
		 * void (*keepalive)( )
		 */
		void keepalive;

		/*
		 * void (*register_user)( )
		 */
		void register_user;

		/*
		 * void (*get_cb_info)( ,  , char * who)
		 */
		void get_cb_info;

		/*
		 * void (*get_cb_away)( ,  , char * who)
		 */
		void get_cb_away;

		/*
		 * void (*alias_buddy)( , char * who, char * alias)
		 */
		void alias_buddy;

		/*
		 * void (*group_buddy)( , char * who, char * old_group, char * new_group)
		 */
		void group_buddy;

		/*
		 * void (*rename_group)( , char * old_name, PurpleGroup * group, GList * moved_buddies)
		 */
		void rename_group;

		/*
		 * void (*buddy_free)( )
		 */
		void buddy_free;

		/*
		 * void (*convo_closed)( , char * who)
		 */
		void convo_closed;

		/*
		 * const char *(*normalize)(const PurpleAccount *, const char *who)
		 */
		UNKNOWN char *(*normalize)(const PurpleAccount *, const char *who);

		/*
		 * void (*set_buddy_icon)( , PurpleStoredImage * img)
		 */
		void set_buddy_icon;

		/*
		 * void (*remove_group)(PurpleConnection * gc, PurpleGroup * group)
		 */
		void remove_group;

		/*
		 * char * (*get_cb_real_name)(PurpleConnection * gc, int id, char * who)
		 */
		string get_cb_real_name;

		/*
		 * void (*set_chat_topic)(PurpleConnection * gc, int id, char * topic)
		 */
		void set_chat_topic;

		/*
		 * PurpleChat * (*find_blist_chat)(PurpleAccount * account, char * name)
		 */
		IntPtr find_blist_chat;

		/*
		 * PurpleRoomlist * (*roomlist_get_list)(PurpleConnection * gc)
		 */
		IntPtr roomlist_get_list;

		/*
		 * void (*roomlist_cancel)(PurpleRoomlist * list)
		 */
		void roomlist_cancel;

		/*
		 * void (*roomlist_expand_category)(PurpleRoomlist * list, PurpleRoomlistRoom * category)
		 */
		void roomlist_expand_category;

		/*
		 * gboolean (*can_receive_file)( , char * who)
		 */
		bool can_receive_file;

		/*
		 * void (*send_file)( , char * who, char * filename)
		 */
		void send_file;

		/*
		 * PurpleXfer * (*new_xfer)( , char * who)
		 */
		IntPtr new_xfer;

		/*
		 * gboolean (*offline_message)(PurpleBuddy * buddy)
		 */
		bool offline_message;

		/*
		 * PurpleWhiteboardPrplOps * whiteboard_prpl_ops
		 */
		IntPtr whiteboard_prpl_ops;

		/*
		 * int (*send_raw)(PurpleConnection * gc, char * buf, int len)
		 */
		int send_raw;

		/*
		 * char * (*roomlist_room_serialize)(PurpleRoomlistRoom * room)
		 */
		string roomlist_room_serialize;

		/*
		 * void (*unregister_user)( , PurpleAccountUnregistrationCb cb, void * user_data)
		 */
		void unregister_user;

		/*
		 * gboolean (*send_attention)(PurpleConnection * gc, char * username, guint type)
		 */
		bool send_attention;

		/*
		 * GList * (*get_attention_types)(PurpleAccount * acct)
		 */
		IntPtr get_attention_types;

		/*
		 * unsigned long struct_size
		 */
		UNKNOWN long struct_size;

		/*
		 * GHashTable * (*get_account_text_table)(PurpleAccount * account)
		 */
		IntPtr get_account_text_table;

		/*
		 * gboolean (*initiate_media)(PurpleAccount * account, char * who, PurpleMediaSessionType type)
		 */
		bool initiate_media;

		/*
		 * PurpleMediaCaps (*get_media_caps)(PurpleAccount * account, char * who)
		 */
		UNKNOWN get_media_caps;

	}
}


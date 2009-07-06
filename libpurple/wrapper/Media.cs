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
	public class Media
	{
		public enum PurpleMediaCaps
		{
			PURPLE_MEDIA_CAPS_NONE = 0,
			PURPLE_MEDIA_CAPS_AUDIO = 1,
			PURPLE_MEDIA_CAPS_AUDIO_SINGLE_DIRECTION = 1 << 1,
			PURPLE_MEDIA_CAPS_VIDEO = 1 << 2,
			PURPLE_MEDIA_CAPS_VIDEO_SINGLE_DIRECTION = 1 << 3,
			PURPLE_MEDIA_CAPS_AUDIO_VIDEO = 1 << 4,
			PURPLE_MEDIA_CAPS_MODIFY_SESSION = 1 << 5,
			PURPLE_MEDIA_CAPS_CHANGE_DIRECTION = 1 << 6
		};

		public enum PurpleMediaSessionType
		{
			PURPLE_MEDIA_NONE = 0,
			PURPLE_MEDIA_RECV_AUDIO = 1 << 0,
			PURPLE_MEDIA_SEND_AUDIO = 1 << 1,
			PURPLE_MEDIA_RECV_VIDEO = 1 << 2,
			PURPLE_MEDIA_SEND_VIDEO = 1 << 3,
			PURPLE_MEDIA_AUDIO = PURPLE_MEDIA_RECV_AUDIO | PURPLE_MEDIA_SEND_AUDIO,
			PURPLE_MEDIA_VIDEO = PURPLE_MEDIA_RECV_VIDEO | PURPLE_MEDIA_SEND_VIDEO
		};

		public enum PurpleMediaState
		{
			PURPLE_MEDIA_STATE_NEW = 0,
			PURPLE_MEDIA_STATE_CONNECTED,
			PURPLE_MEDIA_STATE_END
		};

		public enum PurpleMediaInfoType
		{
			PURPLE_MEDIA_INFO_HANGUP = 0,
			PURPLE_MEDIA_INFO_ACCEPT,
			PURPLE_MEDIA_INFO_REJECT,
			PURPLE_MEDIA_INFO_MUTE,
			PURPLE_MEDIA_INFO_UNMUTE,
			PURPLE_MEDIA_INFO_HOLD,
			PURPLE_MEDIA_INFO_UNHOLD
		};

		public enum PurpleMediaCandidateType
		{
			PURPLE_MEDIA_CANDIDATE_TYPE_HOST,
			PURPLE_MEDIA_CANDIDATE_TYPE_SRFLX,
			PURPLE_MEDIA_CANDIDATE_TYPE_PRFLX,
			PURPLE_MEDIA_CANDIDATE_TYPE_RELAY,
			PURPLE_MEDIA_CANDIDATE_TYPE_MULTICAST
		};

		public enum PurpleMediaComponentType
		{
			PURPLE_MEDIA_COMPONENT_NONE = 0,
			PURPLE_MEDIA_COMPONENT_RTP = 1,
			PURPLE_MEDIA_COMPONENT_RTCP = 2
		};

		public enum PurpleMediaNetworkProtocol
		{
			PURPLE_MEDIA_NETWORK_PROTOCOL_UDP,
			PURPLE_MEDIA_NETWORK_PROTOCOL_TCP
		};

		/*
		 * GType purple_media_session_type_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_session_type_get_type();

		public static /* libgobject */ IntPtr SessionTypeGetType()
		{
			/* Unable to process purple_media_session_type_get_type, a GObjectObject. */
			
		}

		/*
		 * GType purple_media_candidate_type_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_candidate_type_get_type();

		public static /* libgobject */ IntPtr CandidateTypeGetType()
		{
			/* Unable to process purple_media_candidate_type_get_type, a GObjectObject. */
			
		}

		/*
		 * GType purple_media_network_protocol_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_network_protocol_get_type();

		public static /* libgobject */ IntPtr NetworkProtocolGetType()
		{
			/* Unable to process purple_media_network_protocol_get_type, a GObjectObject. */
			
		}

		/*
		 * GType purple_media_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_type();

		public static /* libgobject */ IntPtr GetType()
		{
			/* Unable to process purple_media_get_type, a GObjectObject. */
			
		}

		/*
		 * GType purple_media_state_changed_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_state_changed_get_type();

		public static /* libgobject */ IntPtr StateChangedGetType()
		{
			/* Unable to process purple_media_state_changed_get_type, a GObjectObject. */
			
		}

		/*
		 * GType purple_media_info_type_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_info_type_get_type();

		public static /* libgobject */ IntPtr InfoTypeGetType()
		{
			/* Unable to process purple_media_info_type_get_type, a GObjectObject. */
			
		}

		/*
		 * GType purple_media_candidate_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_candidate_get_type();

		public static /* libgobject */ IntPtr CandidateGetType()
		{
			/* Unable to process purple_media_candidate_get_type, a GObjectObject. */
			
		}

		/*
		 * GList * purple_media_candidate_list_copy(GList * candidates)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_list_copy in file "media.h".
		 * Message: The type could not be resolved (GList * purple_media_candidate_list_copy(GList * candidates)).
		 */

		/*
		 * void purple_media_candidate_list_free(GList * candidates)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_list_free in file "media.h".
		 * Message: The type could not be resolved (GList * candidates).
		 */

		/*
		 * gchar * purple_media_candidate_get_foundation(PurpleMediaCandidate * candidate)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_get_foundation in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCandidate * candidate).
		 */

		/*
		 * guint purple_media_candidate_get_component_id(PurpleMediaCandidate * candidate)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_get_component_id in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCandidate * candidate).
		 */

		/*
		 * gchar * purple_media_candidate_get_ip(PurpleMediaCandidate * candidate)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_get_ip in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCandidate * candidate).
		 */

		/*
		 * guint16 purple_media_candidate_get_port(PurpleMediaCandidate * candidate)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_get_port in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCandidate * candidate).
		 */

		/*
		 * gchar * purple_media_candidate_get_base_ip(PurpleMediaCandidate * candidate)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_get_base_ip in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCandidate * candidate).
		 */

		/*
		 * guint16 purple_media_candidate_get_base_port(PurpleMediaCandidate * candidate)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_get_base_port in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCandidate * candidate).
		 */

		/*
		 * guint32 purple_media_candidate_get_priority(PurpleMediaCandidate * candidate)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_get_priority in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCandidate * candidate).
		 */

		/*
		 * gchar * purple_media_candidate_get_username(PurpleMediaCandidate * candidate)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_get_username in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCandidate * candidate).
		 */

		/*
		 * gchar * purple_media_candidate_get_password(PurpleMediaCandidate * candidate)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_get_password in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCandidate * candidate).
		 */

		/*
		 * guint purple_media_candidate_get_ttl(PurpleMediaCandidate * candidate)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_get_ttl in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCandidate * candidate).
		 */

		/*
		 * GType purple_media_codec_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_codec_get_type();

		public static /* libgobject */ IntPtr CodecGetType()
		{
			/* Unable to process purple_media_codec_get_type, a GObjectObject. */
			
		}

		/*
		 * guint purple_media_codec_get_id(PurpleMediaCodec * codec)
		 * 
		 * Could not generate a wrapper for purple_media_codec_get_id in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCodec * codec).
		 */

		/*
		 * gchar * purple_media_codec_get_encoding_name(PurpleMediaCodec * codec)
		 * 
		 * Could not generate a wrapper for purple_media_codec_get_encoding_name in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCodec * codec).
		 */

		/*
		 * guint purple_media_codec_get_clock_rate(PurpleMediaCodec * codec)
		 * 
		 * Could not generate a wrapper for purple_media_codec_get_clock_rate in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCodec * codec).
		 */

		/*
		 * guint purple_media_codec_get_channels(PurpleMediaCodec * codec)
		 * 
		 * Could not generate a wrapper for purple_media_codec_get_channels in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCodec * codec).
		 */

		/*
		 * GList * purple_media_codec_get_optional_parameters(PurpleMediaCodec * codec)
		 * 
		 * Could not generate a wrapper for purple_media_codec_get_optional_parameters in file "media.h".
		 * Message: The type could not be resolved (GList * purple_media_codec_get_optional_parameters(PurpleMediaCodec * codec)).
		 */

		/*
		 * gchar * purple_media_codec_to_string(PurpleMediaCodec * codec)
		 * 
		 * Could not generate a wrapper for purple_media_codec_to_string in file "media.h".
		 * Message: The type could not be resolved (PurpleMediaCodec * codec).
		 */

		/*
		 * GList * purple_media_codec_list_copy(GList * codecs)
		 * 
		 * Could not generate a wrapper for purple_media_codec_list_copy in file "media.h".
		 * Message: The type could not be resolved (GList * purple_media_codec_list_copy(GList * codecs)).
		 */

		/*
		 * void purple_media_codec_list_free(GList * codecs)
		 * 
		 * Could not generate a wrapper for purple_media_codec_list_free in file "media.h".
		 * Message: The type could not be resolved (GList * codecs).
		 */

		/*
		 * GList * purple_media_get_session_ids(PurpleMedia * media)
		 * 
		 * Could not generate a wrapper for purple_media_get_session_ids in file "media.h".
		 * Message: The type could not be resolved (GList * purple_media_get_session_ids(PurpleMedia * media)).
		 */

		/*
		 * PurpleAccount * purple_media_get_account(PurpleMedia * media)
		 * 
		 * Could not generate a wrapper for purple_media_get_account in file "media.h".
		 * Message: The type could not be resolved (PurpleMedia * media).
		 */

		/*
		 * gpointer purple_media_get_prpl_data(PurpleMedia * media)
		 * 
		 * Could not generate a wrapper for purple_media_get_prpl_data in file "media.h".
		 * Message: The type could not be resolved (PurpleMedia * media).
		 */

		/*
		 * void purple_media_set_prpl_data(PurpleMedia * media, gpointer prpl_data)
		 * 
		 * Could not generate a wrapper for purple_media_set_prpl_data in file "media.h".
		 * Message: The type could not be resolved (PurpleMedia * media).
		 */

		/*
		 * PurpleMediaSessionType purple_media_get_session_type(PurpleMedia * media, gchar * sess_id)
		 * 
		 * Could not generate a wrapper for purple_media_get_session_type in file "media.h".
		 * Message: The type could not be resolved (PurpleMedia * media).
		 */

		/*
		 * _PurpleMediaManager * purple_media_get_manager(PurpleMedia * media)
		 * 
		 * Could not generate a wrapper for purple_media_get_manager in file "media.h".
		 * Message: The type could not be resolved (_PurpleMediaManager * purple_media_get_manager(PurpleMedia * media)).
		 */

		/*
		 * GList * purple_media_get_codecs(PurpleMedia * media, gchar * sess_id)
		 * 
		 * Could not generate a wrapper for purple_media_get_codecs in file "media.h".
		 * Message: The type could not be resolved (GList * purple_media_get_codecs(PurpleMedia * media, gchar * sess_id)).
		 */

		/*
		 * gboolean purple_media_set_send_codec(PurpleMedia * media, gchar * sess_id, PurpleMediaCodec * codec)
		 * 
		 * Could not generate a wrapper for purple_media_set_send_codec in file "media.h".
		 * Message: The type could not be resolved (PurpleMedia * media).
		 */

		/*
		 * gboolean purple_media_codecs_ready(PurpleMedia * media, gchar * sess_id)
		 * 
		 * Could not generate a wrapper for purple_media_codecs_ready in file "media.h".
		 * Message: The type could not be resolved (PurpleMedia * media).
		 */

		/*
		 * void purple_media_set_input_volume(PurpleMedia * media, gchar * session_id, double level)
		 * 
		 * Could not generate a wrapper for purple_media_set_input_volume in file "media.h".
		 * Message: The type could not be resolved (PurpleMedia * media).
		 */

		/*
		 * void purple_media_remove_output_windows(PurpleMedia * media)
		 * 
		 * Could not generate a wrapper for purple_media_remove_output_windows in file "media.h".
		 * Message: The type could not be resolved (PurpleMedia * media).
		 */

	}
}


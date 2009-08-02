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
	 * File: media.h
	 */
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
		 * Function Name: purple_media_session_type_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_media_session_type_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_session_type_get_type();

		public static /* libobject */ IntPtr SessionTypeGetType()
		{
			/* Unable to process the return value of purple_media_session_type_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_type_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_media_candidate_type_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_candidate_type_get_type();

		public static /* libobject */ IntPtr CandidateTypeGetType()
		{
			/* Unable to process the return value of purple_media_candidate_type_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_network_protocol_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_media_network_protocol_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_network_protocol_get_type();

		public static /* libobject */ IntPtr NetworkProtocolGetType()
		{
			/* Unable to process the return value of purple_media_network_protocol_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_media_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_type();

		public static /* libobject */ IntPtr GetType()
		{
			/* Unable to process the return value of purple_media_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_state_changed_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_media_state_changed_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_state_changed_get_type();

		public static /* libobject */ IntPtr StateChangedGetType()
		{
			/* Unable to process the return value of purple_media_state_changed_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_info_type_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_media_info_type_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_info_type_get_type();

		public static /* libobject */ IntPtr InfoTypeGetType()
		{
			/* Unable to process the return value of purple_media_info_type_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_media_candidate_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_candidate_get_type();

		public static /* libobject */ IntPtr CandidateGetType()
		{
			/* Unable to process the return value of purple_media_candidate_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_list_copy
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_media_candidate_list_copy(GList * candidates)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_list_copy in file "media.h".
		 * Message: Unknown inner-type of argument 0 of purple_media_candidate_list_copy
		 */

		/*
		 * Function Name: purple_media_candidate_list_free
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_media_candidate_list_free(GList * candidates)
		 * 
		 * Could not generate a wrapper for purple_media_candidate_list_free in file "media.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_media_candidate_get_foundation
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_media_candidate_get_foundation(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_candidate_get_foundation(IntPtr candidate);

		public static string CandidateGetFoundation(/* Resolved as PointerToUnknownStruct to PurpleMediaCandidate */ IntPtr candidate)
		{
			/* Unable to process candidate, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_get_component_id
		 * Function Type: guint
		 * Function RawType: guint
		 * Function Category: Native
		 */
		/*
		 * guint purple_media_candidate_get_component_id(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_candidate_get_component_id(IntPtr candidate);

		public static uint CandidateGetComponentId(/* Resolved as PointerToUnknownStruct to PurpleMediaCandidate */ IntPtr candidate)
		{
			/* Unable to process candidate, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_get_ip
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_media_candidate_get_ip(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_candidate_get_ip(IntPtr candidate);

		public static string CandidateGetIp(/* Resolved as PointerToUnknownStruct to PurpleMediaCandidate */ IntPtr candidate)
		{
			/* Unable to process candidate, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_get_port
		 * Function Type: guint16
		 * Function RawType: guint16
		 * Function Category: Native
		 */
		/*
		 * guint16 purple_media_candidate_get_port(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern ushort purple_media_candidate_get_port(IntPtr candidate);

		public static ushort CandidateGetPort(/* Resolved as PointerToUnknownStruct to PurpleMediaCandidate */ IntPtr candidate)
		{
			/* Unable to process candidate, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_get_base_ip
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_media_candidate_get_base_ip(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_candidate_get_base_ip(IntPtr candidate);

		public static string CandidateGetBaseIp(/* Resolved as PointerToUnknownStruct to PurpleMediaCandidate */ IntPtr candidate)
		{
			/* Unable to process candidate, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_get_base_port
		 * Function Type: guint16
		 * Function RawType: guint16
		 * Function Category: Native
		 */
		/*
		 * guint16 purple_media_candidate_get_base_port(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern ushort purple_media_candidate_get_base_port(IntPtr candidate);

		public static ushort CandidateGetBasePort(/* Resolved as PointerToUnknownStruct to PurpleMediaCandidate */ IntPtr candidate)
		{
			/* Unable to process candidate, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_get_priority
		 * Function Type: guint32
		 * Function RawType: guint32
		 * Function Category: Native
		 */
		/*
		 * guint32 purple_media_candidate_get_priority(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_candidate_get_priority(IntPtr candidate);

		public static uint CandidateGetPriority(/* Resolved as PointerToUnknownStruct to PurpleMediaCandidate */ IntPtr candidate)
		{
			/* Unable to process candidate, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_get_username
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_media_candidate_get_username(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_candidate_get_username(IntPtr candidate);

		public static string CandidateGetUsername(/* Resolved as PointerToUnknownStruct to PurpleMediaCandidate */ IntPtr candidate)
		{
			/* Unable to process candidate, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_get_password
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_media_candidate_get_password(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_candidate_get_password(IntPtr candidate);

		public static string CandidateGetPassword(/* Resolved as PointerToUnknownStruct to PurpleMediaCandidate */ IntPtr candidate)
		{
			/* Unable to process candidate, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_candidate_get_ttl
		 * Function Type: guint
		 * Function RawType: guint
		 * Function Category: Native
		 */
		/*
		 * guint purple_media_candidate_get_ttl(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_candidate_get_ttl(IntPtr candidate);

		public static uint CandidateGetTtl(/* Resolved as PointerToUnknownStruct to PurpleMediaCandidate */ IntPtr candidate)
		{
			/* Unable to process candidate, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_codec_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_media_codec_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_codec_get_type();

		public static /* libobject */ IntPtr CodecGetType()
		{
			/* Unable to process the return value of purple_media_codec_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_codec_get_id
		 * Function Type: guint
		 * Function RawType: guint
		 * Function Category: Native
		 */
		/*
		 * guint purple_media_codec_get_id(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_codec_get_id(IntPtr codec);

		public static uint CodecGetId(/* Resolved as PointerToUnknownStruct to PurpleMediaCodec */ IntPtr codec)
		{
			/* Unable to process codec, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_codec_get_encoding_name
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_media_codec_get_encoding_name(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_codec_get_encoding_name(IntPtr codec);

		public static string CodecGetEncodingName(/* Resolved as PointerToUnknownStruct to PurpleMediaCodec */ IntPtr codec)
		{
			/* Unable to process codec, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_codec_get_clock_rate
		 * Function Type: guint
		 * Function RawType: guint
		 * Function Category: Native
		 */
		/*
		 * guint purple_media_codec_get_clock_rate(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_codec_get_clock_rate(IntPtr codec);

		public static uint CodecGetClockRate(/* Resolved as PointerToUnknownStruct to PurpleMediaCodec */ IntPtr codec)
		{
			/* Unable to process codec, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_codec_get_channels
		 * Function Type: guint
		 * Function RawType: guint
		 * Function Category: Native
		 */
		/*
		 * guint purple_media_codec_get_channels(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_codec_get_channels(IntPtr codec);

		public static uint CodecGetChannels(/* Resolved as PointerToUnknownStruct to PurpleMediaCodec */ IntPtr codec)
		{
			/* Unable to process codec, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_codec_get_optional_parameters
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_media_codec_get_optional_parameters(PurpleMediaCodec * codec)
		 * 
		 * Could not generate a wrapper for purple_media_codec_get_optional_parameters in file "media.h".
		 * Message: Unknown inner-type of argument 0 of purple_media_codec_get_optional_parameters
		 */

		/*
		 * Function Name: purple_media_codec_to_string
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_media_codec_to_string(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_codec_to_string(IntPtr codec);

		public static string CodecToString(/* Resolved as PointerToUnknownStruct to PurpleMediaCodec */ IntPtr codec)
		{
			/* Unable to process codec, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_codec_list_copy
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_media_codec_list_copy(GList * codecs)
		 * 
		 * Could not generate a wrapper for purple_media_codec_list_copy in file "media.h".
		 * Message: Unknown inner-type of argument 0 of purple_media_codec_list_copy
		 */

		/*
		 * Function Name: purple_media_codec_list_free
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_media_codec_list_free(GList * codecs)
		 * 
		 * Could not generate a wrapper for purple_media_codec_list_free in file "media.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_media_get_session_ids
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_media_get_session_ids(PurpleMedia * media)
		 * 
		 * Could not generate a wrapper for purple_media_get_session_ids in file "media.h".
		 * Message: Unknown inner-type of argument 0 of purple_media_get_session_ids
		 */

		/*
		 * Function Name: purple_media_get_account
		 * Function Type: PurpleAccount *
		 * Function RawType: PurpleAccount
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleAccount * purple_media_get_account(PurpleMedia * media)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_account(IntPtr media);

		public static PurpleAccount GetAccount(/* Resolved as PointerToUnknownStruct to PurpleMedia */ IntPtr media)
		{
			/* Unable to process media, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_get_prpl_data
		 * Function Type: gpointer
		 * Function RawType: gpointer
		 * Function Category: VoidPointer
		 */
		/*
		 * gpointer purple_media_get_prpl_data(PurpleMedia * media)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_prpl_data(IntPtr media);

		public static IntPtr GetPrplData(/* Resolved as PointerToUnknownStruct to PurpleMedia */ IntPtr media)
		{
			/* Unable to process media, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_set_prpl_data
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_media_set_prpl_data(PurpleMedia * media, gpointer prpl_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_set_prpl_data(IntPtr media, IntPtr prpl_data);

		public static void SetPrplData(/* Resolved as PointerToUnknownStruct to PurpleMedia */ IntPtr media, IntPtr prpl_data)
		{
			/* Unable to process media, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_get_session_type
		 * Function Type: PurpleMediaSessionType
		 * Function RawType: PurpleMediaSessionType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleMediaSessionType purple_media_get_session_type(PurpleMedia * media, gchar * sess_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern Media.PurpleMediaSessionType purple_media_get_session_type(IntPtr media, string sess_id);

		public static Media.PurpleMediaSessionType GetSessionType(/* Resolved as PointerToUnknownStruct to PurpleMedia */ IntPtr media, string sess_id)
		{
			/* Unable to process media, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_get_manager
		 * Function Type: _PurpleMediaManager *
		 * Function RawType: _PurpleMediaManager
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * _PurpleMediaManager * purple_media_get_manager(PurpleMedia * media)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_manager(IntPtr media);

		public static /* Resolved as PointerToUnknownStruct to _PurpleMediaManager */ IntPtr GetManager(/* Resolved as PointerToUnknownStruct to PurpleMedia */ IntPtr media)
		{
			/* Unable to process media, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_get_codecs
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_media_get_codecs(PurpleMedia * media, gchar * sess_id)
		 * 
		 * Could not generate a wrapper for purple_media_get_codecs in file "media.h".
		 * Message: Unknown inner-type of argument 0 of purple_media_get_codecs
		 */

		/*
		 * Function Name: purple_media_set_send_codec
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_media_set_send_codec(PurpleMedia * media, gchar * sess_id, PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_set_send_codec(IntPtr media, string sess_id, IntPtr codec);

		public static bool SetSendCodec(/* Resolved as PointerToUnknownStruct to PurpleMedia */ IntPtr media, string sess_id, /* Resolved as PointerToUnknownStruct to PurpleMediaCodec */ IntPtr codec)
		{
			/* Unable to process media, a PointerToUnknownStruct. */
			/* Unable to process codec, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_codecs_ready
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_media_codecs_ready(PurpleMedia * media, gchar * sess_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_codecs_ready(IntPtr media, string sess_id);

		public static bool CodecsReady(/* Resolved as PointerToUnknownStruct to PurpleMedia */ IntPtr media, string sess_id)
		{
			/* Unable to process media, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_set_input_volume
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_media_set_input_volume(PurpleMedia * media, gchar * session_id, double level)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_set_input_volume(IntPtr media, string session_id, double level);

		public static void SetInputVolume(/* Resolved as PointerToUnknownStruct to PurpleMedia */ IntPtr media, string session_id, double level)
		{
			/* Unable to process media, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_media_remove_output_windows
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_media_remove_output_windows(PurpleMedia * media)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_remove_output_windows(IntPtr media);

		public static void RemoveOutputWindows(/* Resolved as PointerToUnknownStruct to PurpleMedia */ IntPtr media)
		{
			/* Unable to process media, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


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
	public class Media
	{
		/*
		 * GType purple_media_session_type_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_session_type_get_type();

		public static GType SessionTypeGetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * GType purple_media_candidate_type_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_candidate_type_get_type();

		public static GType CandidateTypeGetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * GType purple_media_network_protocol_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_network_protocol_get_type();

		public static GType NetworkProtocolGetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * GType purple_media_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_get_type();

		public static GType GetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * GType purple_media_state_changed_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_state_changed_get_type();

		public static GType StateChangedGetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * GType purple_media_info_type_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_info_type_get_type();

		public static GType InfoTypeGetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * GType purple_media_candidate_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_candidate_get_type();

		public static GType CandidateGetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMediaCandidate * purple_media_candidate_new(gchar * foundation, guint component_id, PurpleMediaCandidateType type, PurpleMediaNetworkProtocol proto, gchar * ip, guint port)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_candidate_new(string foundation, uint component_id, UNKNOWN type, UNKNOWN proto, string ip, uint port);

		public static PurpleMediaCandidate CandidateNew(string foundation, uint component_id, PurpleMediaCandidateType type, PurpleMediaNetworkProtocol proto, string ip, uint port)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_media_candidate_list_copy(GList * candidates)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_candidate_list_copy(IntPtr candidates);

		public static GList CandidateListCopy(GList candidates)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_candidate_list_free(GList * candidates)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_candidate_list_free(IntPtr candidates);

		public static void CandidateListFree(GList candidates)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_media_candidate_get_foundation(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_candidate_get_foundation(IntPtr candidate);

		public static string CandidateGetFoundation(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_media_candidate_get_component_id(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_candidate_get_component_id(IntPtr candidate);

		public static uint CandidateGetComponentId(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_media_candidate_get_ip(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_candidate_get_ip(IntPtr candidate);

		public static string CandidateGetIp(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint16 purple_media_candidate_get_port(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern ushort purple_media_candidate_get_port(IntPtr candidate);

		public static ushort CandidateGetPort(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_media_candidate_get_base_ip(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_candidate_get_base_ip(IntPtr candidate);

		public static string CandidateGetBaseIp(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint16 purple_media_candidate_get_base_port(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern ushort purple_media_candidate_get_base_port(IntPtr candidate);

		public static ushort CandidateGetBasePort(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMediaNetworkProtocol purple_media_candidate_get_protocol(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_candidate_get_protocol(IntPtr candidate);

		public static PurpleMediaNetworkProtocol CandidateGetProtocol(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint32 purple_media_candidate_get_priority(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_candidate_get_priority(IntPtr candidate);

		public static uint CandidateGetPriority(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMediaCandidateType purple_media_candidate_get_candidate_type(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_candidate_get_candidate_type(IntPtr candidate);

		public static PurpleMediaCandidateType CandidateGetCandidateType(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_media_candidate_get_username(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_candidate_get_username(IntPtr candidate);

		public static string CandidateGetUsername(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_media_candidate_get_password(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_candidate_get_password(IntPtr candidate);

		public static string CandidateGetPassword(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_media_candidate_get_ttl(PurpleMediaCandidate * candidate)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_candidate_get_ttl(IntPtr candidate);

		public static uint CandidateGetTtl(PurpleMediaCandidate candidate)
		{
			throw new NotImplementedException();
		}

		/*
		 * GType purple_media_codec_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_codec_get_type();

		public static GType CodecGetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMediaCodec * purple_media_codec_new(int id, char * encoding_name, PurpleMediaSessionType media_type, guint clock_rate)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_codec_new(int id, string encoding_name, UNKNOWN media_type, uint clock_rate);

		public static PurpleMediaCodec CodecNew(int id, string encoding_name, PurpleMediaSessionType media_type, uint clock_rate)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_media_codec_get_id(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_codec_get_id(IntPtr codec);

		public static uint CodecGetId(PurpleMediaCodec codec)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_media_codec_get_encoding_name(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_codec_get_encoding_name(IntPtr codec);

		public static string CodecGetEncodingName(PurpleMediaCodec codec)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_media_codec_get_clock_rate(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_codec_get_clock_rate(IntPtr codec);

		public static uint CodecGetClockRate(PurpleMediaCodec codec)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_media_codec_get_channels(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_media_codec_get_channels(IntPtr codec);

		public static uint CodecGetChannels(PurpleMediaCodec codec)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_media_codec_get_optional_parameters(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_codec_get_optional_parameters(IntPtr codec);

		public static GList CodecGetOptionalParameters(PurpleMediaCodec codec)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_media_codec_to_string(PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_media_codec_to_string(IntPtr codec);

		public static string CodecToString(PurpleMediaCodec codec)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_codec_add_optional_parameter(PurpleMediaCodec * codec, gchar * name, gchar * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_codec_add_optional_parameter(IntPtr codec, string name, string value);

		public static void CodecAddOptionalParameter(PurpleMediaCodec codec, string name, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_codec_remove_optional_parameter(PurpleMediaCodec * codec, PurpleKeyValuePair * param)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_codec_remove_optional_parameter(IntPtr codec, IntPtr param);

		public static void CodecRemoveOptionalParameter(PurpleMediaCodec codec, PurpleKeyValuePair param)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleKeyValuePair * purple_media_codec_get_optional_parameter(PurpleMediaCodec * codec, gchar * name, gchar * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_codec_get_optional_parameter(IntPtr codec, string name, string value);

		public static PurpleKeyValuePair CodecGetOptionalParameter(PurpleMediaCodec codec, string name, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_media_codec_list_copy(GList * codecs)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_codec_list_copy(IntPtr codecs);

		public static GList CodecListCopy(GList codecs)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_codec_list_free(GList * codecs)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_codec_list_free(IntPtr codecs);

		public static void CodecListFree(GList codecs)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_media_get_session_ids(PurpleMedia * media)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_session_ids(IntPtr media);

		public static GList GetSessionIds(PurpleMedia media)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_media_get_account(PurpleMedia * media)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_account(IntPtr media);

		public static PurpleAccount GetAccount(PurpleMedia media)
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_media_get_prpl_data(PurpleMedia * media)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_prpl_data(IntPtr media);

		public static IntPtr GetPrplData(PurpleMedia media)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_set_prpl_data(PurpleMedia * media, gpointer prpl_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_set_prpl_data(IntPtr media, IntPtr prpl_data);

		public static void SetPrplData(PurpleMedia media, IntPtr prpl_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_error(PurpleMedia * media, gchar * error, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_error(IntPtr media, string error, ...);

		public static void Error(PurpleMedia media, string error, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_end(PurpleMedia * media, gchar * session_id, gchar * participant)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_end(IntPtr media, string session_id, string participant);

		public static void End(PurpleMedia media, string session_id, string participant)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_stream_info(PurpleMedia * media, PurpleMediaInfoType type, gchar * session_id, gchar * participant, gboolean local)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_stream_info(IntPtr media, UNKNOWN type, string session_id, string participant, bool local);

		public static void StreamInfo(PurpleMedia media, PurpleMediaInfoType type, string session_id, string participant, bool local)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_media_add_stream(PurpleMedia * media, gchar * sess_id, gchar * who, PurpleMediaSessionType type, gboolean initiator, gchar * transmitter, guint num_params, GParameter * params)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_add_stream(IntPtr media, string sess_id, string who, UNKNOWN type, bool initiator, string transmitter, uint num_params, IntPtr params);

		public static bool AddStream(PurpleMedia media, string sess_id, string who, PurpleMediaSessionType type, bool initiator, string transmitter, uint num_params, GParameter params)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMediaSessionType purple_media_get_session_type(PurpleMedia * media, gchar * sess_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_media_get_session_type(IntPtr media, string sess_id);

		public static PurpleMediaSessionType GetSessionType(PurpleMedia media, string sess_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * _PurpleMediaManager * purple_media_get_manager(PurpleMedia * media)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_manager(IntPtr media);

		public static _PurpleMediaManager GetManager(PurpleMedia media)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_media_get_codecs(PurpleMedia * media, gchar * sess_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_codecs(IntPtr media, string sess_id);

		public static GList GetCodecs(PurpleMedia media, string sess_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_add_remote_candidates(PurpleMedia * media, gchar * sess_id, gchar * participant, GList * remote_candidates)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_add_remote_candidates(IntPtr media, string sess_id, string participant, IntPtr remote_candidates);

		public static void AddRemoteCandidates(PurpleMedia media, string sess_id, string participant, GList remote_candidates)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_media_get_local_candidates(PurpleMedia * media, gchar * sess_id, gchar * participant)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_local_candidates(IntPtr media, string sess_id, string participant);

		public static GList GetLocalCandidates(PurpleMedia media, string sess_id, string participant)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_media_get_active_local_candidates(PurpleMedia * media, gchar * sess_id, gchar * participant)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_active_local_candidates(IntPtr media, string sess_id, string participant);

		public static GList GetActiveLocalCandidates(PurpleMedia media, string sess_id, string participant)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_media_get_active_remote_candidates(PurpleMedia * media, gchar * sess_id, gchar * participant)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_media_get_active_remote_candidates(IntPtr media, string sess_id, string participant);

		public static GList GetActiveRemoteCandidates(PurpleMedia media, string sess_id, string participant)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_media_set_remote_codecs(PurpleMedia * media, gchar * sess_id, gchar * participant, GList * codecs)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_set_remote_codecs(IntPtr media, string sess_id, string participant, IntPtr codecs);

		public static bool SetRemoteCodecs(PurpleMedia media, string sess_id, string participant, GList codecs)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_media_candidates_prepared(PurpleMedia * media, gchar * session_id, gchar * participant)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_candidates_prepared(IntPtr media, string session_id, string participant);

		public static bool CandidatesPrepared(PurpleMedia media, string session_id, string participant)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_media_set_send_codec(PurpleMedia * media, gchar * sess_id, PurpleMediaCodec * codec)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_set_send_codec(IntPtr media, string sess_id, IntPtr codec);

		public static bool SetSendCodec(PurpleMedia media, string sess_id, PurpleMediaCodec codec)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_media_codecs_ready(PurpleMedia * media, gchar * sess_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_codecs_ready(IntPtr media, string sess_id);

		public static bool CodecsReady(PurpleMedia media, string sess_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_media_is_initiator(PurpleMedia * media, gchar * sess_id, gchar * participant)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_is_initiator(IntPtr media, string sess_id, string participant);

		public static bool IsInitiator(PurpleMedia media, string sess_id, string participant)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_media_accepted(PurpleMedia * media, gchar * sess_id, gchar * participant)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_media_accepted(IntPtr media, string sess_id, string participant);

		public static bool Accepted(PurpleMedia media, string sess_id, string participant)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_set_input_volume(PurpleMedia * media, gchar * session_id, double level)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_set_input_volume(IntPtr media, string session_id, double level);

		public static void SetInputVolume(PurpleMedia media, string session_id, double level)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_set_output_volume(PurpleMedia * media, gchar * session_id, gchar * participant, double level)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_set_output_volume(IntPtr media, string session_id, string participant, double level);

		public static void SetOutputVolume(PurpleMedia media, string session_id, string participant, double level)
		{
			throw new NotImplementedException();
		}

		/*
		 * gulong purple_media_set_output_window(PurpleMedia * media, gchar * session_id, gchar * participant, gulong window_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_media_set_output_window(IntPtr media, string session_id, string participant, ulong window_id);

		public static ulong SetOutputWindow(PurpleMedia media, string session_id, string participant, ulong window_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_media_remove_output_windows(PurpleMedia * media)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_media_remove_output_windows(IntPtr media);

		public static void RemoveOutputWindows(PurpleMedia media)
		{
			throw new NotImplementedException();
		}

	}
}


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
	public class Util
	{
		/*
		 * PurpleMenuAction * purple_menu_action_new(char * label, PurpleCallback callback, gpointer data, GList * children)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_menu_action_new(string label, UNKNOWN callback, IntPtr data, IntPtr children);

		public static PurpleMenuAction MenuActionNew(string label, PurpleCallback callback, IntPtr data, GList children)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_menu_action_free(PurpleMenuAction * act)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_menu_action_free(IntPtr act);

		public static void MenuActionFree(PurpleMenuAction act)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_util_set_current_song(char * title, char * artist, char * album)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_util_set_current_song(string title, string artist, string album);

		public static void SetCurrentSong(string title, string artist, string album)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_util_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_util_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_util_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_util_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_base16_encode(guchar * data, gsize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_base16_encode(IntPtr data, UNKNOWN len);

		public static string Base16Encode(guchar data, gsize len)
		{
			throw new NotImplementedException();
		}

		/*
		 * guchar * purple_base16_decode(char * str, gsize * ret_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_base16_decode(string str, IntPtr ret_len);

		public static guchar Base16Decode(string str, gsize ret_len)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_base16_encode_chunked(guchar * data, gsize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_base16_encode_chunked(IntPtr data, UNKNOWN len);

		public static string Base16EncodeChunked(guchar data, gsize len)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_base64_encode(guchar * data, gsize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_base64_encode(IntPtr data, UNKNOWN len);

		public static string Base64Encode(guchar data, gsize len)
		{
			throw new NotImplementedException();
		}

		/*
		 * guchar * purple_base64_decode(char * str, gsize * ret_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_base64_decode(string str, IntPtr ret_len);

		public static guchar Base64Decode(string str, gsize ret_len)
		{
			throw new NotImplementedException();
		}

		/*
		 * guchar * purple_quotedp_decode(char * str, gsize * ret_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_quotedp_decode(string str, IntPtr ret_len);

		public static guchar QuotedpDecode(string str, gsize ret_len)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_mime_decode_field(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_mime_decode_field(string str);

		public static string MimeDecodeField(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_utf8_strftime(char * format, struct tm)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_strftime(string format, UNKNOWN tm);

		public static string Utf8Strftime(string format, struct tm)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_get_tzoff_str(struct tm, gboolean iso)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_get_tzoff_str(UNKNOWN tm, bool iso);

		public static string GetTzoffStr(struct tm, bool iso)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_date_format_short(struct tm)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_date_format_short(UNKNOWN tm);

		public static string DateFormatShort(struct tm)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_date_format_long(struct tm)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_date_format_long(UNKNOWN tm);

		public static string DateFormatLong(struct tm)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_date_format_full(struct tm)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_date_format_full(UNKNOWN tm);

		public static string DateFormatFull(struct tm)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_time_format(struct tm)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_time_format(UNKNOWN tm);

		public static string TimeFormat(struct tm)
		{
			throw new NotImplementedException();
		}

		/*
		 * time_t purple_time_build(int year, int month, int day, int hour, int min, int sec)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_time_build(int year, int month, int day, int hour, int min, int sec);

		public static time_t TimeBuild(int year, int month, int day, int hour, int min, int sec)
		{
			throw new NotImplementedException();
		}

		/*
		 * time_t purple_str_to_time(char * timestamp, gboolean utc, struct tm, long * tz_off, char ** rest)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_str_to_time(string timestamp, bool utc, UNKNOWN tm, IntPtr tz_off, IntPtr rest);

		public static time_t StrToTime(string timestamp, bool utc, struct tm, long tz_off, char rest)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_markup_find_tag(char * needle, char * haystack, char ** start, char ** end, GData ** attributes)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_markup_find_tag(string needle, string haystack, IntPtr start, IntPtr end, IntPtr attributes);

		public static bool MarkupFindTag(string needle, string haystack, char start, char end, GData attributes)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_markup_extract_info_field(char * str, int len, PurpleNotifyUserInfo * user_info, char * start_token, int skip, char * end_token, char check_value, char * no_value_token, char * display_name, gboolean is_link, char * link_prefix, PurpleInfoFieldFormatCallback format_cb)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_markup_extract_info_field(string str, int len, IntPtr user_info, string start_token, int skip, string end_token, char check_value, string no_value_token, string display_name, bool is_link, string link_prefix, UNKNOWN format_cb);

		public static bool MarkupExtractInfoField(string str, int len, PurpleNotifyUserInfo user_info, string start_token, int skip, string end_token, char check_value, string no_value_token, string display_name, bool is_link, string link_prefix, PurpleInfoFieldFormatCallback format_cb)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_markup_html_to_xhtml(char * html, char ** dest_xhtml, char ** dest_plain)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_markup_html_to_xhtml(string html, IntPtr dest_xhtml, IntPtr dest_plain);

		public static void MarkupHtmlToXhtml(string html, char dest_xhtml, char dest_plain)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_markup_strip_html(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_markup_strip_html(string str);

		public static string MarkupStripHtml(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_markup_linkify(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_markup_linkify(string str);

		public static string MarkupLinkify(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_unescape_html(char * html)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_unescape_html(string html);

		public static string UnescapeHtml(string html)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_markup_slice(char * str, guint x, guint y)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_markup_slice(string str, uint x, uint y);

		public static string MarkupSlice(string str, uint x, uint y)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_markup_get_tag_name(char * tag)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_markup_get_tag_name(string tag);

		public static string MarkupGetTagName(string tag)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_markup_is_rtl(char * html)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_markup_is_rtl(string html);

		public static bool MarkupIsRtl(string html)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_home_dir()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_home_dir();

		public static string HomeDir()
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_user_dir()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_user_dir();

		public static string UserDir()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_util_set_user_dir(char * dir)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_util_set_user_dir(string dir);

		public static void SetUserDir(string dir)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_build_dir(char * path, int mode)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_build_dir(string path, int mode);

		public static int BuildDir(string path, int mode)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_util_write_data_to_file(char * filename, char * data, gssize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_util_write_data_to_file(string filename, string data, UNKNOWN size);

		public static bool WriteDataToFile(string filename, string data, gssize size)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_util_write_data_to_file_absolute(char * filename_full, char * data, gssize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_util_write_data_to_file_absolute(string filename_full, string data, UNKNOWN size);

		public static bool WriteDataToFileAbsolute(string filename_full, string data, gssize size)
		{
			throw new NotImplementedException();
		}

		/*
		 * xmlnode * purple_util_read_xml_from_file(char * filename, char * description)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_util_read_xml_from_file(string filename, string description);

		public static xmlnode ReadXmlFromFile(string filename, string description)
		{
			throw new NotImplementedException();
		}

		/*
		 * FILE * purple_mkstemp(char ** path, gboolean binary)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_mkstemp(IntPtr path, bool binary);

		public static FILE Mkstemp(char path, bool binary)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_util_get_image_checksum(gconstpointer image_data, size_t image_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_util_get_image_checksum(UNKNOWN image_data, UNKNOWN image_len);

		public static string GetImageChecksum(gconstpointer image_data, size_t image_len)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_util_get_image_filename(gconstpointer image_data, size_t image_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_util_get_image_filename(UNKNOWN image_data, UNKNOWN image_len);

		public static string GetImageFilename(gconstpointer image_data, size_t image_len)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_program_is_valid(char * program)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_program_is_valid(string program);

		public static bool ProgramIsValid(string program)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_running_gnome()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_running_gnome();

		public static bool RunningGnome()
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_running_kde()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_running_kde();

		public static bool RunningKde()
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_running_osx()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_running_osx();

		public static bool RunningOsx()
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_fd_get_ip(int fd)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_fd_get_ip(int fd);

		public static string FdGetIp(int fd)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_strequal(gchar * left, gchar * right)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_strequal(string left, string right);

		public static bool Strequal(string left, string right)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_normalize(PurpleAccount * account, char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_normalize(IntPtr account, string str);

		public static string Normalize(PurpleAccount account, string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_normalize_nocase(PurpleAccount * account, char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_normalize_nocase(IntPtr account, string str);

		public static string NormalizeNocase(PurpleAccount account, string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_str_has_prefix(char * s, char * p)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_str_has_prefix(string s, string p);

		public static bool StrHasPrefix(string s, string p)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_str_has_suffix(char * s, char * x)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_str_has_suffix(string s, string x);

		public static bool StrHasSuffix(string s, string x)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_strdup_withhtml(gchar * src)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_strdup_withhtml(string src);

		public static string StrdupWithhtml(string src)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_str_add_cr(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_str_add_cr(string str);

		public static string StrAddCr(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_str_strip_char(char * str, char thechar)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_str_strip_char(string str, char thechar);

		public static void StrStripChar(string str, char thechar)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_util_chrreplace(char * string, char delimiter, char replacement)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_util_chrreplace(string string, char delimiter, char replacement);

		public static void Chrreplace(string string, char delimiter, char replacement)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_strreplace(char * string, char * delimiter, char * replacement)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_strreplace(string string, string delimiter, string replacement);

		public static string Strreplace(string string, string delimiter, string replacement)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_utf8_ncr_encode(char * in)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_ncr_encode(string in);

		public static string Utf8NcrEncode(string in)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_utf8_ncr_decode(char * in)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_ncr_decode(string in);

		public static string Utf8NcrDecode(string in)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_strcasereplace(char * string, char * delimiter, char * replacement)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_strcasereplace(string string, string delimiter, string replacement);

		public static string Strcasereplace(string string, string delimiter, string replacement)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_strcasestr(char * haystack, char * needle)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_strcasestr(string haystack, string needle);

		public static string Strcasestr(string haystack, string needle)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_str_size_to_units(size_t size)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_str_size_to_units(UNKNOWN size);

		public static string StrSizeToUnits(size_t size)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_str_seconds_to_string(guint sec)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_str_seconds_to_string(uint sec);

		public static string StrSecondsToString(uint sec)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_str_binary_to_ascii(unsigned char, guint len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_str_binary_to_ascii(UNKNOWN char, uint len);

		public static string StrBinaryToAscii(unsigned char, uint len)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_got_protocol_handler_uri(char * uri)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_got_protocol_handler_uri(string uri);

		public static void GotProtocolHandlerUri(string uri)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_url_parse(char * url, char ** ret_host, int * ret_port, char ** ret_path, char ** ret_user, char ** ret_passwd)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_url_parse(string url, IntPtr ret_host, IntPtr ret_port, IntPtr ret_path, IntPtr ret_user, IntPtr ret_passwd);

		public static bool UrlParse(string url, char ret_host, int ret_port, char ret_path, char ret_user, char ret_passwd)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleUtilFetchUrlData * purple_util_fetch_url_request(gchar * url, gboolean full, gchar * user_agent, gboolean http11, gchar * request, gboolean include_headers, PurpleUtilFetchUrlCallback callback, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_util_fetch_url_request(string url, bool full, string user_agent, bool http11, string request, bool include_headers, UNKNOWN callback, IntPtr data);

		public static PurpleUtilFetchUrlData FetchUrlRequest(string url, bool full, string user_agent, bool http11, string request, bool include_headers, PurpleUtilFetchUrlCallback callback, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleUtilFetchUrlData * purple_util_fetch_url_request_len(gchar * url, gboolean full, gchar * user_agent, gboolean http11, gchar * request, gboolean include_headers, gssize max_len, PurpleUtilFetchUrlCallback callback, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_util_fetch_url_request_len(string url, bool full, string user_agent, bool http11, string request, bool include_headers, UNKNOWN max_len, UNKNOWN callback, IntPtr data);

		public static PurpleUtilFetchUrlData FetchUrlRequestLen(string url, bool full, string user_agent, bool http11, string request, bool include_headers, gssize max_len, PurpleUtilFetchUrlCallback callback, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleUtilFetchUrlData * purple_util_fetch_url_request_len_with_account(PurpleAccount * account, gchar * url, gboolean full, gchar * user_agent, gboolean http11, gchar * request, gboolean include_headers, gssize max_len, PurpleUtilFetchUrlCallback callback, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_util_fetch_url_request_len_with_account(IntPtr account, string url, bool full, string user_agent, bool http11, string request, bool include_headers, UNKNOWN max_len, UNKNOWN callback, IntPtr data);

		public static PurpleUtilFetchUrlData FetchUrlRequestLenWithAccount(PurpleAccount account, string url, bool full, string user_agent, bool http11, string request, bool include_headers, gssize max_len, PurpleUtilFetchUrlCallback callback, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_util_fetch_url_cancel(PurpleUtilFetchUrlData * url_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_util_fetch_url_cancel(IntPtr url_data);

		public static void FetchUrlCancel(PurpleUtilFetchUrlData url_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_url_decode(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_url_decode(string str);

		public static string UrlDecode(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_url_encode(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_url_encode(string str);

		public static string UrlEncode(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_email_is_valid(char * address)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_email_is_valid(string address);

		public static bool EmailIsValid(string address)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_ip_address_is_valid(char * ip)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_ip_address_is_valid(string ip);

		public static bool IpAddressIsValid(string ip)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_uri_list_extract_uris(gchar * uri_list)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_uri_list_extract_uris(string uri_list);

		public static GList UriListExtractUris(string uri_list)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_uri_list_extract_filenames(gchar * uri_list)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_uri_list_extract_filenames(string uri_list);

		public static GList UriListExtractFilenames(string uri_list)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_utf8_try_convert(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_try_convert(string str);

		public static string Utf8TryConvert(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_utf8_salvage(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_salvage(string str);

		public static string Utf8Salvage(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_utf8_strip_unprintables(gchar * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_strip_unprintables(string str);

		public static string Utf8StripUnprintables(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_gai_strerror(gint errnum)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_gai_strerror(int errnum);

		public static string GaiStrerror(int errnum)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_utf8_strcasecmp(char * a, char * b)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_utf8_strcasecmp(string a, string b);

		public static int Utf8Strcasecmp(string a, string b)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_utf8_has_word(char * haystack, char * needle)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_utf8_has_word(string haystack, string needle);

		public static bool Utf8HasWord(string haystack, string needle)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_print_utf8_to_console(FILE * filestream, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_print_utf8_to_console(IntPtr filestream, string message);

		public static void PrintUtf8ToConsole(FILE filestream, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_message_meify(char * message, gssize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_message_meify(string message, UNKNOWN len);

		public static bool MessageMeify(string message, gssize len)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_text_strip_mnemonic(char * in)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_text_strip_mnemonic(string in);

		public static string TextStripMnemonic(string in)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_unescape_filename(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_unescape_filename(string str);

		public static string UnescapeFilename(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_escape_filename(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_escape_filename(string str);

		public static string EscapeFilename(string str)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * _purple_oscar_convert(char * act, char * protocol)
		 */
		[DllImport("libpurple.dll")]
		private static extern string _purple_oscar_convert(string act, string protocol);

		public static string _purpleOscarConvert(string act, string protocol)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_restore_default_signal_handlers()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_restore_default_signal_handlers();

		public static void RestoreDefaultSignalHandlers()
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_get_host_name()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_get_host_name();

		public static string GetHostName()
		{
			throw new NotImplementedException();
		}

	}
}


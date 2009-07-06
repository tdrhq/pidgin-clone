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
	public class Util
	{
		/*
		 * void purple_menu_action_free(PurpleMenuAction * act)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_menu_action_free(IntPtr act);

		public static void MenuActionFree(PurpleMenuAction act)
		{
			purple_menu_action_free(act.Reference);
		}

		/*
		 * void purple_util_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_util_init();

		public static void Init()
		{
			purple_util_init();
		}

		/*
		 * void purple_util_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_util_uninit();

		public static void Uninit()
		{
			purple_util_uninit();
		}

		/*
		 * gchar * purple_base16_encode(guchar * data, gsize len)
		 * 
		 * Could not generate a wrapper for purple_base16_encode in file "util.h".
		 * Message: The type could not be resolved (guchar * data).
		 */

		/*
		 * guchar * purple_base16_decode(char * str, gsize * ret_len)
		 * 
		 * Could not generate a wrapper for purple_base16_decode in file "util.h".
		 * Message: The type could not be resolved (guchar * purple_base16_decode(char * str, gsize * ret_len)).
		 */

		/*
		 * gchar * purple_base16_encode_chunked(guchar * data, gsize len)
		 * 
		 * Could not generate a wrapper for purple_base16_encode_chunked in file "util.h".
		 * Message: The type could not be resolved (guchar * data).
		 */

		/*
		 * gchar * purple_base64_encode(guchar * data, gsize len)
		 * 
		 * Could not generate a wrapper for purple_base64_encode in file "util.h".
		 * Message: The type could not be resolved (guchar * data).
		 */

		/*
		 * guchar * purple_base64_decode(char * str, gsize * ret_len)
		 * 
		 * Could not generate a wrapper for purple_base64_decode in file "util.h".
		 * Message: The type could not be resolved (guchar * purple_base64_decode(char * str, gsize * ret_len)).
		 */

		/*
		 * guchar * purple_quotedp_decode(char * str, gsize * ret_len)
		 * 
		 * Could not generate a wrapper for purple_quotedp_decode in file "util.h".
		 * Message: The type could not be resolved (guchar * purple_quotedp_decode(char * str, gsize * ret_len)).
		 */

		/*
		 * char * purple_mime_decode_field(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_mime_decode_field(string str);

		public static string MimeDecodeField(string str)
		{
			return purple_mime_decode_field(str);
		}

		/*
		 * char * purple_utf8_strftime(char * format, struct tm)
		 * 
		 * Could not generate a wrapper for purple_utf8_strftime in file "util.h".
		 * Message: The struct contains an inner-struct or innter-union and cannot be automatically wrapped.
		 */

		/*
		 * char * purple_get_tzoff_str(struct tm, gboolean iso)
		 * 
		 * Could not generate a wrapper for purple_get_tzoff_str in file "util.h".
		 * Message: The struct contains an inner-struct or innter-union and cannot be automatically wrapped.
		 */

		/*
		 * char * purple_date_format_short(struct tm)
		 * 
		 * Could not generate a wrapper for purple_date_format_short in file "util.h".
		 * Message: The struct contains an inner-struct or innter-union and cannot be automatically wrapped.
		 */

		/*
		 * char * purple_date_format_long(struct tm)
		 * 
		 * Could not generate a wrapper for purple_date_format_long in file "util.h".
		 * Message: The struct contains an inner-struct or innter-union and cannot be automatically wrapped.
		 */

		/*
		 * char * purple_date_format_full(struct tm)
		 * 
		 * Could not generate a wrapper for purple_date_format_full in file "util.h".
		 * Message: The struct contains an inner-struct or innter-union and cannot be automatically wrapped.
		 */

		/*
		 * char * purple_time_format(struct tm)
		 * 
		 * Could not generate a wrapper for purple_time_format in file "util.h".
		 * Message: The struct contains an inner-struct or innter-union and cannot be automatically wrapped.
		 */

		/*
		 * char * purple_markup_strip_html(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_markup_strip_html(string str);

		public static string MarkupStripHtml(string str)
		{
			return purple_markup_strip_html(str);
		}

		/*
		 * char * purple_markup_linkify(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_markup_linkify(string str);

		public static string MarkupLinkify(string str)
		{
			return purple_markup_linkify(str);
		}

		/*
		 * char * purple_unescape_html(char * html)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_unescape_html(string html);

		public static string UnescapeHtml(string html)
		{
			return purple_unescape_html(html);
		}

		/*
		 * char * purple_markup_slice(char * str, guint x, guint y)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_markup_slice(string str, uint x, uint y);

		public static string MarkupSlice(string str, uint x, uint y)
		{
			return purple_markup_slice(str, x, y);
		}

		/*
		 * char * purple_markup_get_tag_name(char * tag)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_markup_get_tag_name(string tag);

		public static string MarkupGetTagName(string tag)
		{
			return purple_markup_get_tag_name(tag);
		}

		/*
		 * char * purple_markup_unescape_entity(char * text, int * length)
		 * 
		 * Could not generate a wrapper for purple_markup_unescape_entity in file "util.h".
		 * Message: The type could not be resolved (int * length).
		 */

		/*
		 * char * purple_markup_get_css_property(gchar * style, gchar * opt)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_markup_get_css_property(string style, string opt);

		public static string MarkupGetCssProperty(string style, string opt)
		{
			return purple_markup_get_css_property(style, opt);
		}

		/*
		 * gboolean purple_markup_is_rtl(char * html)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_markup_is_rtl(string html);

		public static bool MarkupIsRtl(string html)
		{
			return purple_markup_is_rtl(html);
		}

		/*
		 * gchar * purple_home_dir()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_home_dir();

		public static string HomeDir()
		{
			return purple_home_dir();
		}

		/*
		 * char * purple_user_dir()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_user_dir();

		public static string UserDir()
		{
			return purple_user_dir();
		}

		/*
		 * void purple_util_set_user_dir(char * dir)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_util_set_user_dir(string dir);

		public static void SetUserDir(string dir)
		{
			purple_util_set_user_dir(dir);
		}

		/*
		 * int purple_build_dir(char * path, int mode)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_build_dir(string path, int mode);

		public static int BuildDir(string path, int mode)
		{
			return purple_build_dir(path, mode);
		}

		/*
		 * gboolean purple_util_write_data_to_file_absolute(char * filename_full, char * data, gssize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_util_write_data_to_file_absolute(string filename_full, string data, long size);

		public static bool WriteDataToFileAbsolute(string filename_full, string data, long size)
		{
			return purple_util_write_data_to_file_absolute(filename_full, data, size);
		}

		/*
		 * FILE * purple_mkstemp(char ** path, gboolean binary)
		 * 
		 * Could not generate a wrapper for purple_mkstemp in file "util.h".
		 * Message: The type could not be resolved (FILE * purple_mkstemp(char ** path, gboolean binary)).
		 */

		/*
		 * char * purple_util_get_image_extension(gconstpointer data, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_util_get_image_extension(IntPtr data, ulong len);

		public static string GetImageExtension(IntPtr data, ulong len)
		{
			return purple_util_get_image_extension(data, len);
		}

		/*
		 * char * purple_util_get_image_checksum(gconstpointer image_data, size_t image_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_util_get_image_checksum(IntPtr image_data, ulong image_len);

		public static string GetImageChecksum(IntPtr image_data, ulong image_len)
		{
			return purple_util_get_image_checksum(image_data, image_len);
		}

		/*
		 * char * purple_util_get_image_filename(gconstpointer image_data, size_t image_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_util_get_image_filename(IntPtr image_data, ulong image_len);

		public static string GetImageFilename(IntPtr image_data, ulong image_len)
		{
			return purple_util_get_image_filename(image_data, image_len);
		}

		/*
		 * gboolean purple_program_is_valid(char * program)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_program_is_valid(string program);

		public static bool ProgramIsValid(string program)
		{
			return purple_program_is_valid(program);
		}

		/*
		 * gboolean purple_running_gnome()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_running_gnome();

		public static bool RunningGnome()
		{
			return purple_running_gnome();
		}

		/*
		 * gboolean purple_running_kde()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_running_kde();

		public static bool RunningKde()
		{
			return purple_running_kde();
		}

		/*
		 * gboolean purple_running_osx()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_running_osx();

		public static bool RunningOsx()
		{
			return purple_running_osx();
		}

		/*
		 * char * purple_fd_get_ip(int fd)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_fd_get_ip(int fd);

		public static string FdGetIp(int fd)
		{
			return purple_fd_get_ip(fd);
		}

		/*
		 * gboolean purple_strequal(gchar * left, gchar * right)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_strequal(string left, string right);

		public static bool Strequal(string left, string right)
		{
			return purple_strequal(left, right);
		}

		/*
		 * char * purple_normalize(PurpleAccount * account, char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_normalize(IntPtr account, string str);

		public static string Normalize(PurpleAccount account, string str)
		{
			return purple_normalize(account.Reference, str);
		}

		/*
		 * char * purple_normalize_nocase(PurpleAccount * account, char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_normalize_nocase(IntPtr account, string str);

		public static string NormalizeNocase(PurpleAccount account, string str)
		{
			return purple_normalize_nocase(account.Reference, str);
		}

		/*
		 * gboolean purple_str_has_prefix(char * s, char * p)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_str_has_prefix(string s, string p);

		public static bool StrHasPrefix(string s, string p)
		{
			return purple_str_has_prefix(s, p);
		}

		/*
		 * gboolean purple_str_has_suffix(char * s, char * x)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_str_has_suffix(string s, string x);

		public static bool StrHasSuffix(string s, string x)
		{
			return purple_str_has_suffix(s, x);
		}

		/*
		 * gchar * purple_strdup_withhtml(gchar * src)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_strdup_withhtml(string src);

		public static string StrdupWithhtml(string src)
		{
			return purple_strdup_withhtml(src);
		}

		/*
		 * char * purple_str_add_cr(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_str_add_cr(string str);

		public static string StrAddCr(string str)
		{
			return purple_str_add_cr(str);
		}

		/*
		 * void purple_str_strip_char(char * str, char thechar)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_str_strip_char(string str, char thechar);

		public static void StrStripChar(string str, char thechar)
		{
			purple_str_strip_char(str, thechar);
		}

		/*
		 * char * purple_utf8_ncr_encode(char * in)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_ncr_encode(string in_);

		public static string Utf8NcrEncode(string in_)
		{
			return purple_utf8_ncr_encode(in_);
		}

		/*
		 * char * purple_utf8_ncr_decode(char * in)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_ncr_decode(string in_);

		public static string Utf8NcrDecode(string in_)
		{
			return purple_utf8_ncr_decode(in_);
		}

		/*
		 * char * purple_strcasestr(char * haystack, char * needle)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_strcasestr(string haystack, string needle);

		public static string Strcasestr(string haystack, string needle)
		{
			return purple_strcasestr(haystack, needle);
		}

		/*
		 * char * purple_str_size_to_units(size_t size)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_str_size_to_units(ulong size);

		public static string StrSizeToUnits(ulong size)
		{
			return purple_str_size_to_units(size);
		}

		/*
		 * char * purple_str_seconds_to_string(guint sec)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_str_seconds_to_string(uint sec);

		public static string StrSecondsToString(uint sec)
		{
			return purple_str_seconds_to_string(sec);
		}

		/*
		 * char * purple_str_binary_to_ascii(unsigned char * binary, guint len)
		 * 
		 * Could not generate a wrapper for purple_str_binary_to_ascii in file "util.h".
		 * Message: The type could not be resolved (unsigned char * binary).
		 */

		/*
		 * void purple_got_protocol_handler_uri(char * uri)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_got_protocol_handler_uri(string uri);

		public static void GotProtocolHandlerUri(string uri)
		{
			purple_got_protocol_handler_uri(uri);
		}

		/*
		 * void purple_util_fetch_url_cancel(PurpleUtilFetchUrlData * url_data)
		 * 
		 * Could not generate a wrapper for purple_util_fetch_url_cancel in file "util.h".
		 * Message: The type could not be resolved (PurpleUtilFetchUrlData * url_data).
		 */

		/*
		 * char * purple_url_decode(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_url_decode(string str);

		public static string UrlDecode(string str)
		{
			return purple_url_decode(str);
		}

		/*
		 * char * purple_url_encode(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_url_encode(string str);

		public static string UrlEncode(string str)
		{
			return purple_url_encode(str);
		}

		/*
		 * gboolean purple_email_is_valid(char * address)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_email_is_valid(string address);

		public static bool EmailIsValid(string address)
		{
			return purple_email_is_valid(address);
		}

		/*
		 * gboolean purple_ip_address_is_valid(char * ip)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_ip_address_is_valid(string ip);

		public static bool IpAddressIsValid(string ip)
		{
			return purple_ip_address_is_valid(ip);
		}

		/*
		 * GList * purple_uri_list_extract_uris(gchar * uri_list)
		 * 
		 * Could not generate a wrapper for purple_uri_list_extract_uris in file "util.h".
		 * Message: The type could not be resolved (GList * purple_uri_list_extract_uris(gchar * uri_list)).
		 */

		/*
		 * GList * purple_uri_list_extract_filenames(gchar * uri_list)
		 * 
		 * Could not generate a wrapper for purple_uri_list_extract_filenames in file "util.h".
		 * Message: The type could not be resolved (GList * purple_uri_list_extract_filenames(gchar * uri_list)).
		 */

		/*
		 * gchar * purple_utf8_try_convert(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_try_convert(string str);

		public static string Utf8TryConvert(string str)
		{
			return purple_utf8_try_convert(str);
		}

		/*
		 * gchar * purple_utf8_salvage(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_salvage(string str);

		public static string Utf8Salvage(string str)
		{
			return purple_utf8_salvage(str);
		}

		/*
		 * gchar * purple_utf8_strip_unprintables(gchar * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_utf8_strip_unprintables(string str);

		public static string Utf8StripUnprintables(string str)
		{
			return purple_utf8_strip_unprintables(str);
		}

		/*
		 * gchar * purple_gai_strerror(gint errnum)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_gai_strerror(int errnum);

		public static string GaiStrerror(int errnum)
		{
			return purple_gai_strerror(errnum);
		}

		/*
		 * int purple_utf8_strcasecmp(char * a, char * b)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_utf8_strcasecmp(string a, string b);

		public static int Utf8Strcasecmp(string a, string b)
		{
			return purple_utf8_strcasecmp(a, b);
		}

		/*
		 * gboolean purple_utf8_has_word(char * haystack, char * needle)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_utf8_has_word(string haystack, string needle);

		public static bool Utf8HasWord(string haystack, string needle)
		{
			return purple_utf8_has_word(haystack, needle);
		}

		/*
		 * void purple_print_utf8_to_console(FILE * filestream, char * message)
		 * 
		 * Could not generate a wrapper for purple_print_utf8_to_console in file "util.h".
		 * Message: The type could not be resolved (FILE * filestream).
		 */

		/*
		 * gboolean purple_message_meify(char * message, gssize len)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_message_meify(string message, long len);

		public static bool MessageMeify(string message, long len)
		{
			return purple_message_meify(message, len);
		}

		/*
		 * char * purple_text_strip_mnemonic(char * in)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_text_strip_mnemonic(string in_);

		public static string TextStripMnemonic(string in_)
		{
			return purple_text_strip_mnemonic(in_);
		}

		/*
		 * char * purple_unescape_filename(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_unescape_filename(string str);

		public static string UnescapeFilename(string str)
		{
			return purple_unescape_filename(str);
		}

		/*
		 * char * purple_escape_filename(char * str)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_escape_filename(string str);

		public static string EscapeFilename(string str)
		{
			return purple_escape_filename(str);
		}

		/*
		 * char * purple_oscar_convert(char * act, char * protocol)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_oscar_convert(string act, string protocol);

		public static string OscarConvert(string act, string protocol)
		{
			return purple_oscar_convert(act, protocol);
		}

		/*
		 * void purple_restore_default_signal_handlers()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_restore_default_signal_handlers();

		public static void RestoreDefaultSignalHandlers()
		{
			purple_restore_default_signal_handlers();
		}

		/*
		 * gchar * purple_get_host_name()
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_get_host_name();

		public static string GetHostName()
		{
			return purple_get_host_name();
		}

	}
}


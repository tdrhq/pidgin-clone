/**
 * @file util.h Utility Functions
 * @ingroup core
 *
 * gaim
 *
 * Gaim is the legal property of its developers, whose names are too numerous
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * @todo Rename the functions so that they live somewhere in the gaim
 *       namespace.
 */
#ifndef _GAIM_UTIL_H_
#define _GAIM_UTIL_H_

#include <stdio.h>

#include "account.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/** @name Base16 Functions                                                */
/**************************************************************************/
/*@{*/

/**
 * Converts a string to its base-16 equivalent.
 *
 * @param str The string to convert.
 * @param len The length of the string.
 *
 * @return The base-16 string.
 *
 * @see gaim_base16_decode()
 */
unsigned char *gaim_base16_encode(const unsigned char *str, int len);

/**
 * Converts a string back from its base-16 equivalent.
 *
 * @param str     The string to convert back.
 * @param ret_str The returned, non-base-16 string.
 *
 * @return The length of the returned string.
 *
 * @see gaim_base16_encode()
 */
int gaim_base16_decode(const char *str, unsigned char **ret_str);

/*@}*/


/**************************************************************************/
/** @name Base64 Functions                                                */
/**************************************************************************/
/*@{*/

/**
 * Converts a string to its base-64 equivalent.
 *
 * @param buf The data to convert.
 * @param len The length of the data.
 *
 * @return The base-64 version of @a str.
 *
 * @see gaim_base64_decode()
 */
unsigned char *gaim_base64_encode(const unsigned char *buf, size_t len);

/**
 * Converts a string back from its base-64 equivalent.
 *
 * @param str     The string to convert back.
 * @param ret_str The returned, non-base-64 string.
 * @param ret_len The returned string length.
 *
 * @see gaim_base64_encode()
 */
void gaim_base64_decode(const char *str, char **ret_str, int *ret_len);

/*@}*/

/**************************************************************************/
/** @name Quoted Printable Functions                                      */
/**************************************************************************/
/*@{*/

/**
 * Converts a quoted printable string back to its readable equivalent.
 *
 * @param str     The string to convert back.
 * @param ret_str The returned, readable string.
 * @param ret_len The returned string length.
 */
void gaim_quotedp_decode (const char *str, char **ret_str, int *ret_len);

/*@}*/

/**************************************************************************/
/** @name MIME Functions                                                  */
/**************************************************************************/
/*@{*/

/**
 * Converts a MIME header field string back to its readable equivalent
 * according to RFC 2047.  Basically, a header is plain ASCII and can 
 * contain any number of sections called "encoded-words."  The format 
 * of an encoded word is =?ISO-8859-1?Q?Keld_J=F8rn_Simonsen?=
 * =? designates the beginning of the encoded-word
 * ?= designates the end of the encoded-word
 * ? segments the encoded word into three pieces.  The first piece is 
 *   the character set, the second peice is the encoding, and the 
 *   third piece is the encoded text.
 *
 * @param str The string to convert back.
 *
 * @return The readble string.
 */
char *gaim_mime_decode_field (const char *str);

/*@}*/


/**************************************************************************/
/** @name Date/Time Functions                                             */
/**************************************************************************/
/*@{*/

/**
 * Returns the current local time in hour:minute:second form.
 *
 * The returned string is stored in a static buffer, so the result
 * should be g_strdup()'d if it's intended to be used for long.
 *
 * @return The current local time.
 *
 * @see gaim_date_full()
 */
const char *gaim_date(void);

/**
 * Returns the date and time in human-readable form.
 *
 * The returned string is stored in a static buffer, so the result
 * should be g_strdup()'d if it's intended to be used for long.
 *
 * @return The date and time in human-readable form.
 *
 * @see gaim_date()
 */
const char *gaim_date_full(void);

/**
 * Builds a time_t from the supplied information.
 *
 * @param year  The year.
 * @param month The month.
 * @param day   The day.
 * @param hour  The hour.
 * @param min   The minute.
 * @param sec   The second.
 *
 * @return A time_t.
 */
time_t gaim_time_build(int year, int month, int day, int hour,
					   int min, int sec);

/*@}*/


/**************************************************************************/
/** @name Markup Functions                                                */
/**************************************************************************/
/*@{*/

/**
 * Finds a HTML tag matching the given name.
 *
 * This locates an HTML tag's start and end, and stores its attributes
 * in a GData hash table. The names of the attributes are lower-cased
 * in the hash table, and the name of the tag is case insensitive.
 *
 * @param needle	the name of the tag
 * @param haystack	the null-delimited string to search in
 * @param start		a pointer to the start of the tag if found
 * @param end		a pointer to the end of the tag if found
 * @param attributes	the attributes, if the tag was found
 * @return TRUE if the tag was found
 */
gboolean gaim_markup_find_tag(const char *needle, const char *haystack,
							  const char **start, const char **end,
							  GData **attributes);

/**
 * Extracts a field of data from HTML.
 *
 * This is a scary function. See protocols/msn/msn.c and
 * protocols/yahoo/yahoo.c for example usage.
 *
 * @param str            The string to parse.
 * @param len            The size of str.
 * @param dest           The destination GString to append the new
 *                       field info to.
 * @param start_token    The beginning token.
 * @param skip           The number of characters to skip after the
 *                       start token.
 * @param end_token      The ending token.
 * @param check_value    The value that the last character must meet.
 * @param no_value_token The token indicating no value is given.
 * @param display_name   The short descriptive name to display for this token.
 * @param is_link        TRUE if this should be a link, or FALSE otherwise.
 * @param link_prefix    The prefix for the link.
 *
 * @return TRUE if successful, or FALSE otherwise.
 */
gboolean gaim_markup_extract_info_field(const char *str, int len, GString *dest,
                                        const char *start_token, int skip,
                                        const char *end_token, char check_value,
                                        const char *no_value_token,
                                        const char *display_name, gboolean is_link,
                                        const char *link_prefix);

/**
 * Converts HTML markup to XHTML.
 *
 * @param html       The HTML markup.
 * @param dest_xhtml The destination XHTML output.
 * @param dest_plain The destination plain-text output.
 */
void gaim_markup_html_to_xhtml(const char *html, char **dest_xhtml,
							   char **dest_plain);

/**
 * Strips HTML tags from a string.
 *
 * @param str The string to strip HTML from.
 *
 * @return The new string without HTML. This must be freed.
 */
char *gaim_markup_strip_html(const char *str);

/**
 * Adds the necessary HTML code to turn URIs into HTML links in a string.
 *
 * @param str The string to linkify.
 *
 * @return The linkified text.
 */
char *gaim_markup_linkify(const char *str);

/**
 * Escapes HTML special characters to be displayed literally.
 * For example '&' is replaced by "&amp;" and so on
 *
 * @param html The string in which to escape any HTML special characters
 *
 * @return the text with HTML special characters escaped
 */
char *gaim_escape_html(const char *html);

/*@}*/


/**************************************************************************/
/** @name Path/Filename Functions                                         */
/**************************************************************************/
/*@{*/

/**
 * Returns the user's home directory.
 *
 * @return The user's home directory.
 *
 * @see gaim_user_dir()
 */
const gchar *gaim_home_dir(void);

/**
 * Returns the gaim settings directory in the user's home directory.
 *
 * @return The gaim settings directory.
 *
 * @see gaim_home_dir()
 */
char *gaim_user_dir(void);

/**
 * Builds a complete path from the root, making any directories along
 * the path which do not already exist.
 *
 * @param path The path you wish to create.  Note that it must start
 *        from the root or this function will fail.
 * @param mode Unix-style permissions for this directory.
 *
 * @return 0 for success, nonzero on any error.
 */
int gaim_build_dir(const char *path, int mode);

/**
 * Creates a temporary file and returns a file pointer to it.
 *
 * This is like mkstemp(), but returns a file pointer and uses a
 * pre-set template. It uses the semantics of tempnam() for the
 * directory to use and allocates the space for the file path.
 *
 * The caller is responsible for closing the file and removing it when
 * done, as well as freeing the space pointed to by @a path with
 * g_free().
 *
 * @param path The returned path to the temp file.
 *
 * @return A file pointer to the temporary file, or @c NULL on failure.
 */
FILE *gaim_mkstemp(char **path);

/**
 * Checks if the given program name is valid and executable.
 *
 * @param program The file name of the application.
 *
 * @return True if the program is runable.
 */
gboolean gaim_program_is_valid(const char *program);

/**
 * Returns the IP address from a socket file descriptor.
 *
 * @param fd The socket file descriptor.
 *
 * @return The IP address, or @c NULL on error.
 */
char *gaim_fd_get_ip(int fd);

/*@}*/


/**************************************************************************/
/** @name String Functions                                                */
/**************************************************************************/
/*@{*/

/**
 * Normalizes a string, so that it is suitable for comparison.
 *
 * The returned string will point to a static buffer, so if the
 * string is intended to be kept long-term, you <i>must</i>
 * g_strdup() it. Also, calling normalize() twice in the same line
 * will lead to problems.
 *
 * @param account  The account the string belongs to.
 * @param str      The string to normalize.
 *
 * @return A pointer to the normalized version stored in a static buffer.
 */
const char *gaim_normalize(const GaimAccount *account, const char *str);

/**
 * Compares two strings to see if the first contains the second as
 * a proper prefix.
 * 
 * @param s  The string to check.
 * @param p  The prefix in question.
 * 
 * @return   TRUE if p is a prefix of s, otherwise FALSE.
 */
gboolean gaim_str_has_prefix(const char *s, const char *p);

/**
 * Compares two strings to see if the second is a proper suffix
 * of the first.
 * 
 * @param s  The string to check.
 * @param x  The suffix in question.
 * 
 * @return   TRUE if x is a a suffix of s, otherwise FALSE.
 */
gboolean gaim_str_has_suffix(const char *s, const char *x);

/**
 * Looks for %n, %d, or %t in a string, and replaces them with the
 * specified name, date, and time, respectively.
 *
 * The returned string is stored in a static buffer, so the result
 * should be g_strdup()'d if it's intended to be used for long.
 *
 * @param str  The string that may contain the special variables.
 * @param name The sender name.
 *
 * @return A new string where the special variables are expanded.
 */
const char *gaim_str_sub_away_formatters(const char *str, const char *name);

/**
 * Copies a string and replaces all HTML linebreaks with newline characters.
 *
 * @param dest     The destination string.
 * @param src      The source string.
 * @param dest_len The destination string length.
 *
 * @see gaim_strncpy_withhtml()
 * @see gaim_strdup_withhtml()
 */
void gaim_strncpy_nohtml(char *dest, const char *src, size_t dest_len);

/**
 * Copies a string and replaces all newline characters with HTML linebreaks.
 *
 * @param dest     The destination string.
 * @param src      The source string.
 * @param dest_len The destination string length.
 *
 * @see gaim_strncpy_nohtml()
 * @see gaim_strdup_withhtml()
 */
void gaim_strncpy_withhtml(gchar *dest, const gchar *src, size_t dest_len);

/**
 * Duplicates a string and replaces all newline characters from the
 * source string with HTML linebreaks.
 *
 * @param src The source string.
 *
 * @return The new string.
 *
 * @see gaim_strncpy_nohtml()
 * @see gaim_strncpy_withhtml()
 */
char *gaim_strdup_withhtml(const char *src);

/**
 * Ensures that all linefeeds have a matching carriage return.
 *
 * @param str The source string.
 *
 * @return The string with carriage returns.
 */
char *gaim_str_add_cr(const char *str);

/**
 * Strips all carriage returns from a string.
 *
 * @param str The string to strip carriage returns from.
 */
void gaim_str_strip_cr(char *str);

/**
 * Given a string, this replaces one substring with another
 * and returns a newly allocated string.
 *
 * @param string The string from which to replace stuff.
 * @param delimiter The substring you want replaced.
 * @param replacement The substring you want inserted in place
 *        of the delimiting substring.
 */
char *gaim_strreplace(const char *string, const char *delimiter,
					  const char *replacement);

/**
 * This is like strstr, except that it ignores ASCII case in
 * searching for the substring.
 *
 * @param haystack The string to search in.
 * @param needle   The substring to find.
 *
 * @return the location of the substring if found, or NULL if not
 */
const char *gaim_strcasestr(const char *haystack, const char *needle);

/**
 * Returns a string representing a filesize in the appropriate
 * units (MB, KB, GB, etc.)
 *
 * @param size The size
 *
 * @return The string in units form. This must be freed.
 */
char *gaim_str_size_to_units(size_t size);

/**
 * Converts seconds into a human-readable form.
 *
 * @param sec The seconds.
 *
 * @return A human-readable form, containing days, hours, minutes, and
 *         seconds.
 */
char *gaim_str_seconds_to_string(guint sec);

/*@}*/


/**************************************************************************/
/** @name URI/URL Functions                                               */
/**************************************************************************/
/*@{*/

/**
 * Parses a URL, returning its host, port, and file path.
 *
 * The returned data must be freed.
 *
 * @param url      The URL to parse.
 * @param ret_host The returned host.
 * @param ret_port The returned port.
 * @param ret_path The returned path.
 */
gboolean gaim_url_parse(const char *url, char **ret_host, int *ret_port,
						char **ret_path);

/**
 * Fetches the data from a URL, and passes it to a callback function.
 *
 * @param url        The URL.
 * @param full       TRUE if this is the full URL, or FALSE if it's a
 *                   partial URL.
 * @param cb         The callback function.
 * @param data       The user data to pass to the callback function.
 * @param user_agent The user agent field to use, or NULL.
 * @param http11     TRUE if HTTP/1.1 should be used to download the file.
 */
void gaim_url_fetch(const char *url, gboolean full,
					const char *user_agent, gboolean http11,
					void (*cb)(void *, const char *, size_t),
					void *data);
/**
 * Decodes a URL into a plain string.
 *
 * This will change hex codes and such to their ascii equivalents.
 *
 * @param str The string to translate.
 *
 * @return The resulting string.
 */
const char *gaim_url_decode(const char *str);

/**
 * Encodes a URL into an escaped string.
 *
 * This will change non-alphanumeric characters to hex codes.
 *
 * @param str The string to translate.
 *
 * @return The resulting string.
 */
const char *gaim_url_encode(const char *str);

/*@}*/

/**************************************************************************
 * UTF8 String Functions
 **************************************************************************/
/*@{*/

/**
 * Attempts to convert a string to UTF-8 from an unknown encoding.
 *
 * This function checks the locale and tries sane defaults.
 *
 * @param str The source string.
 *
 * @return The UTF-8 string, or @c NULL if it could not be converted.
 */
char *gaim_utf8_try_convert(const char *str);

/**
 * Compares two UTF-8 strings.
 *
 * @param a The first string.
 * @param b The second string.
 *
 * @return -1 if @a is less than @a b.
 *          0 if @a is equal to @a b.
 *          1 if @a is greater than @a b.
 */
int gaim_utf8_strcasecmp(const char *a, const char *b);

/**
 * Checks for messages starting with "/me "
 *
 * @param message The message to check
 * @param len     The message length, or -1
 *
 * @return TRUE if it starts with /me, and it has been removed, otherwise FALSE
 */
gboolean gaim_message_meify(char *message, size_t len);

/**
 * Removes the underscore characters from a string used identify the mnemonic
 * character.
 *
 * @param in  The string to strip
 *
 * @return The stripped string
 */
char * gaim_text_strip_mnemonic(const char *in);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* _GAIM_UTIL_H_ */

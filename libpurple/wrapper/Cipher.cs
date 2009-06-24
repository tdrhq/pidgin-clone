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
	public class Cipher
	{
		/*
		 * gchar * purple_cipher_get_name(PurpleCipher * cipher)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_cipher_get_name(IntPtr cipher);

		public static string GetName(PurpleCipher cipher)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_cipher_get_capabilities(PurpleCipher * cipher)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_cipher_get_capabilities(IntPtr cipher);

		public static uint GetCapabilities(PurpleCipher cipher)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_cipher_digest_region(gchar * name, guchar * data, size_t data_len, size_t in_len, guchar digest, size_t * out_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_cipher_digest_region(string name, IntPtr data, UNKNOWN data_len, UNKNOWN in_len, UNKNOWN digest, IntPtr out_len);

		public static bool DigestRegion(string name, guchar data, size_t data_len, size_t in_len, guchar digest, size_t out_len)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleCipher * purple_ciphers_find_cipher(gchar * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ciphers_find_cipher(string name);

		public static PurpleCipher CiphersFindCipher(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleCipher * purple_ciphers_register_cipher(gchar * name, PurpleCipherOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ciphers_register_cipher(string name, IntPtr ops);

		public static PurpleCipher CiphersRegisterCipher(string name, PurpleCipherOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_ciphers_unregister_cipher(PurpleCipher * cipher)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_ciphers_unregister_cipher(IntPtr cipher);

		public static bool CiphersUnregisterCipher(PurpleCipher cipher)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_ciphers_get_ciphers()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ciphers_get_ciphers();

		public static GList CiphersGetCiphers()
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_ciphers_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ciphers_get_handle();

		public static IntPtr CiphersGetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_ciphers_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ciphers_init();

		public static void CiphersInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_ciphers_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ciphers_uninit();

		public static void CiphersUninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cipher_context_set_option(PurpleCipherContext * context, gchar * name, gpointer value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_option(IntPtr context, string name, IntPtr value);

		public static void ContextSetOption(PurpleCipherContext context, string name, IntPtr value)
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_cipher_context_get_option(PurpleCipherContext * context, gchar * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cipher_context_get_option(IntPtr context, string name);

		public static IntPtr ContextGetOption(PurpleCipherContext context, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleCipherContext * purple_cipher_context_new(PurpleCipher * cipher, void * extra)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cipher_context_new(IntPtr cipher, IntPtr extra);

		public static PurpleCipherContext ContextNew(PurpleCipher cipher, IntPtr extra)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleCipherContext * purple_cipher_context_new_by_name(gchar * name, void * extra)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cipher_context_new_by_name(string name, IntPtr extra);

		public static PurpleCipherContext ContextNewByName(string name, IntPtr extra)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cipher_context_reset(PurpleCipherContext * context, gpointer extra)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_reset(IntPtr context, IntPtr extra);

		public static void ContextReset(PurpleCipherContext context, IntPtr extra)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cipher_context_destroy(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_destroy(IntPtr context);

		public static void ContextDestroy(PurpleCipherContext context)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cipher_context_set_iv(PurpleCipherContext * context, guchar * iv, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_iv(IntPtr context, IntPtr iv, UNKNOWN len);

		public static void ContextSetIv(PurpleCipherContext context, guchar iv, size_t len)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cipher_context_append(PurpleCipherContext * context, guchar * data, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_append(IntPtr context, IntPtr data, UNKNOWN len);

		public static void ContextAppend(PurpleCipherContext context, guchar data, size_t len)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_cipher_context_digest(PurpleCipherContext * context, size_t in_len, guchar digest, size_t * out_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_cipher_context_digest(IntPtr context, UNKNOWN in_len, UNKNOWN digest, IntPtr out_len);

		public static bool ContextDigest(PurpleCipherContext context, size_t in_len, guchar digest, size_t out_len)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_cipher_context_digest_to_str(PurpleCipherContext * context, size_t in_len, gchar digest_s, size_t * out_len)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_cipher_context_digest_to_str(IntPtr context, UNKNOWN in_len, char digest_s, IntPtr out_len);

		public static bool ContextDigestToStr(PurpleCipherContext context, size_t in_len, char digest_s, size_t out_len)
		{
			throw new NotImplementedException();
		}

		/*
		 * gint purple_cipher_context_encrypt(PurpleCipherContext * context, guchar data, size_t len, guchar output, size_t * outlen)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_cipher_context_encrypt(IntPtr context, UNKNOWN data, UNKNOWN len, UNKNOWN output, IntPtr outlen);

		public static int ContextEncrypt(PurpleCipherContext context, guchar data, size_t len, guchar output, size_t outlen)
		{
			throw new NotImplementedException();
		}

		/*
		 * gint purple_cipher_context_decrypt(PurpleCipherContext * context, guchar data, size_t len, guchar output, size_t * outlen)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_cipher_context_decrypt(IntPtr context, UNKNOWN data, UNKNOWN len, UNKNOWN output, IntPtr outlen);

		public static int ContextDecrypt(PurpleCipherContext context, guchar data, size_t len, guchar output, size_t outlen)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cipher_context_set_salt(PurpleCipherContext * context, guchar * salt)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_salt(IntPtr context, IntPtr salt);

		public static void ContextSetSalt(PurpleCipherContext context, guchar salt)
		{
			throw new NotImplementedException();
		}

		/*
		 * size_t purple_cipher_context_get_salt_size(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_cipher_context_get_salt_size(IntPtr context);

		public static size_t ContextGetSaltSize(PurpleCipherContext context)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cipher_context_set_key(PurpleCipherContext * context, guchar * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_key(IntPtr context, IntPtr key);

		public static void ContextSetKey(PurpleCipherContext context, guchar key)
		{
			throw new NotImplementedException();
		}

		/*
		 * size_t purple_cipher_context_get_key_size(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_cipher_context_get_key_size(IntPtr context);

		public static size_t ContextGetKeySize(PurpleCipherContext context)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cipher_context_set_batch_mode(PurpleCipherContext * context, PurpleCipherBatchMode mode)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_batch_mode(IntPtr context, UNKNOWN mode);

		public static void ContextSetBatchMode(PurpleCipherContext context, PurpleCipherBatchMode mode)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleCipherBatchMode purple_cipher_context_get_batch_mode(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_cipher_context_get_batch_mode(IntPtr context);

		public static PurpleCipherBatchMode ContextGetBatchMode(PurpleCipherContext context)
		{
			throw new NotImplementedException();
		}

		/*
		 * size_t purple_cipher_context_get_block_size(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_cipher_context_get_block_size(IntPtr context);

		public static size_t ContextGetBlockSize(PurpleCipherContext context)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cipher_context_set_key_with_len(PurpleCipherContext * context, guchar * key, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_key_with_len(IntPtr context, IntPtr key, UNKNOWN len);

		public static void ContextSetKeyWithLen(PurpleCipherContext context, guchar key, size_t len)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cipher_context_set_data(PurpleCipherContext * context, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_data(IntPtr context, IntPtr data);

		public static void ContextSetData(PurpleCipherContext context, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_cipher_context_get_data(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cipher_context_get_data(IntPtr context);

		public static IntPtr ContextGetData(PurpleCipherContext context)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_cipher_http_digest_calculate_session_key(gchar * algorithm, gchar * username, gchar * realm, gchar * password, gchar * nonce, gchar * client_nonce)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_cipher_http_digest_calculate_session_key(string algorithm, string username, string realm, string password, string nonce, string client_nonce);

		public static string HttpDigestCalculateSessionKey(string algorithm, string username, string realm, string password, string nonce, string client_nonce)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_cipher_http_digest_calculate_response(gchar * algorithm, gchar * method, gchar * digest_uri, gchar * qop, gchar * entity, gchar * nonce, gchar * nonce_count, gchar * client_nonce, gchar * session_key)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_cipher_http_digest_calculate_response(string algorithm, string method, string digest_uri, string qop, string entity, string nonce, string nonce_count, string client_nonce, string session_key);

		public static string HttpDigestCalculateResponse(string algorithm, string method, string digest_uri, string qop, string entity, string nonce, string nonce_count, string client_nonce, string session_key)
		{
			throw new NotImplementedException();
		}

	}
}


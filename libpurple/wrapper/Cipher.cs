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
	public class Cipher
	{
		public enum PurpleCipherBatchMode
		{
			PURPLE_CIPHER_BATCH_MODE_ECB,
			PURPLE_CIPHER_BATCH_MODE_CBC
		};

		public enum PurpleCipherCaps
		{
			PURPLE_CIPHER_CAPS_SET_OPT = 1 << 1,
			PURPLE_CIPHER_CAPS_GET_OPT = 1 << 2,
			PURPLE_CIPHER_CAPS_INIT = 1 << 3,
			PURPLE_CIPHER_CAPS_RESET = 1 << 4,
			PURPLE_CIPHER_CAPS_UNINIT = 1 << 5,
			PURPLE_CIPHER_CAPS_SET_IV = 1 << 6,
			PURPLE_CIPHER_CAPS_APPEND = 1 << 7,
			PURPLE_CIPHER_CAPS_DIGEST = 1 << 8,
			PURPLE_CIPHER_CAPS_ENCRYPT = 1 << 9,
			PURPLE_CIPHER_CAPS_DECRYPT = 1 << 10,
			PURPLE_CIPHER_CAPS_SET_SALT = 1 << 11,
			PURPLE_CIPHER_CAPS_GET_SALT_SIZE = 1 << 12,
			PURPLE_CIPHER_CAPS_SET_KEY = 1 << 13,
			PURPLE_CIPHER_CAPS_GET_KEY_SIZE = 1 << 14,
			PURPLE_CIPHER_CAPS_SET_BATCH_MODE = 1 << 15,
			PURPLE_CIPHER_CAPS_GET_BATCH_MODE = 1 << 16,
			PURPLE_CIPHER_CAPS_GET_BLOCK_SIZE = 1 << 17,
			PURPLE_CIPHER_CAPS_SET_KEY_WITH_LEN = 1 << 18,
			PURPLE_CIPHER_CAPS_UNKNOWN = 1 << 19
		};

		/*
		 * gchar * purple_cipher_get_name(PurpleCipher * cipher)
		 * 
		 * Could not generate a wrapper for purple_cipher_get_name in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipher * cipher).
		 */

		/*
		 * guint purple_cipher_get_capabilities(PurpleCipher * cipher)
		 * 
		 * Could not generate a wrapper for purple_cipher_get_capabilities in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipher * cipher).
		 */

		/*
		 * PurpleCipher * purple_ciphers_find_cipher(gchar * name)
		 * 
		 * Could not generate a wrapper for purple_ciphers_find_cipher in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipher * purple_ciphers_find_cipher(gchar * name)).
		 */

		/*
		 * PurpleCipher * purple_ciphers_register_cipher(gchar * name, PurpleCipherOps * ops)
		 * 
		 * Could not generate a wrapper for purple_ciphers_register_cipher in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipher * purple_ciphers_register_cipher(gchar * name, PurpleCipherOps * ops)).
		 */

		/*
		 * gboolean purple_ciphers_unregister_cipher(PurpleCipher * cipher)
		 * 
		 * Could not generate a wrapper for purple_ciphers_unregister_cipher in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipher * cipher).
		 */

		/*
		 * GList * purple_ciphers_get_ciphers()
		 * 
		 * Could not generate a wrapper for purple_ciphers_get_ciphers in file "cipher.h".
		 * Message: The type could not be resolved (GList * purple_ciphers_get_ciphers()).
		 */

		/*
		 * gpointer purple_ciphers_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ciphers_get_handle();

		public static IntPtr CiphersGetHandle()
		{
			return purple_ciphers_get_handle();
		}

		/*
		 * void purple_ciphers_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ciphers_init();

		public static void CiphersInit()
		{
			purple_ciphers_init();
		}

		/*
		 * void purple_ciphers_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_ciphers_uninit();

		public static void CiphersUninit()
		{
			purple_ciphers_uninit();
		}

		/*
		 * void purple_cipher_context_set_option(PurpleCipherContext * context, gchar * name, gpointer value)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_set_option in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * gpointer purple_cipher_context_get_option(PurpleCipherContext * context, gchar * name)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_get_option in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * PurpleCipherContext * purple_cipher_context_new(PurpleCipher * cipher, void * extra)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_new in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * purple_cipher_context_new(PurpleCipher * cipher, void * extra)).
		 */

		/*
		 * PurpleCipherContext * purple_cipher_context_new_by_name(gchar * name, void * extra)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_new_by_name in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * purple_cipher_context_new_by_name(gchar * name, void * extra)).
		 */

		/*
		 * void purple_cipher_context_reset(PurpleCipherContext * context, gpointer extra)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_reset in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * void purple_cipher_context_destroy(PurpleCipherContext * context)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_destroy in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * void purple_cipher_context_set_iv(PurpleCipherContext * context, guchar * iv, size_t len)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_set_iv in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * void purple_cipher_context_append(PurpleCipherContext * context, guchar * data, size_t len)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_append in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * void purple_cipher_context_set_salt(PurpleCipherContext * context, guchar * salt)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_set_salt in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * size_t purple_cipher_context_get_salt_size(PurpleCipherContext * context)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_get_salt_size in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * void purple_cipher_context_set_key(PurpleCipherContext * context, guchar * key)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_set_key in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * size_t purple_cipher_context_get_key_size(PurpleCipherContext * context)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_get_key_size in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * void purple_cipher_context_set_batch_mode(PurpleCipherContext * context, PurpleCipherBatchMode mode)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_set_batch_mode in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * PurpleCipherBatchMode purple_cipher_context_get_batch_mode(PurpleCipherContext * context)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_get_batch_mode in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * size_t purple_cipher_context_get_block_size(PurpleCipherContext * context)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_get_block_size in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * void purple_cipher_context_set_key_with_len(PurpleCipherContext * context, guchar * key, size_t len)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_set_key_with_len in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * void purple_cipher_context_set_data(PurpleCipherContext * context, gpointer data)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_set_data in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

		/*
		 * gpointer purple_cipher_context_get_data(PurpleCipherContext * context)
		 * 
		 * Could not generate a wrapper for purple_cipher_context_get_data in file "cipher.h".
		 * Message: The type could not be resolved (PurpleCipherContext * context).
		 */

	}
}


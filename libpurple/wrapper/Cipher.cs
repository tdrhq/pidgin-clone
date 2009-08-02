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
	 * File: cipher.h
	 */
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
		 * Function Name: purple_cipher_get_name
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_cipher_get_name(PurpleCipher * cipher)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_cipher_get_name(IntPtr cipher);

		public static string GetName(/* Resolved as PointerToUnknownStruct to PurpleCipher */ IntPtr cipher)
		{
			/* Unable to process cipher, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_get_capabilities
		 * Function Type: guint
		 * Function RawType: guint
		 * Function Category: Native
		 */
		/*
		 * guint purple_cipher_get_capabilities(PurpleCipher * cipher)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_cipher_get_capabilities(IntPtr cipher);

		public static uint GetCapabilities(/* Resolved as PointerToUnknownStruct to PurpleCipher */ IntPtr cipher)
		{
			/* Unable to process cipher, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_ciphers_find_cipher
		 * Function Type: PurpleCipher *
		 * Function RawType: PurpleCipher
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleCipher * purple_ciphers_find_cipher(gchar * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ciphers_find_cipher(string name);

		public static /* Resolved as PointerToUnknownStruct to PurpleCipher */ IntPtr CiphersFindCipher(string name)
		{
			/* Unable to process the return value of purple_ciphers_find_cipher, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_ciphers_register_cipher
		 * Function Type: PurpleCipher *
		 * Function RawType: PurpleCipher
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleCipher * purple_ciphers_register_cipher(gchar * name, PurpleCipherOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_ciphers_register_cipher(string name, IntPtr ops);

		public static /* Resolved as PointerToUnknownStruct to PurpleCipher */ IntPtr CiphersRegisterCipher(string name, PurpleCipherOps ops)
		{
			/* Unable to process the return value of purple_ciphers_register_cipher, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_ciphers_unregister_cipher
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_ciphers_unregister_cipher(PurpleCipher * cipher)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_ciphers_unregister_cipher(IntPtr cipher);

		public static bool CiphersUnregisterCipher(/* Resolved as PointerToUnknownStruct to PurpleCipher */ IntPtr cipher)
		{
			/* Unable to process cipher, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_ciphers_get_ciphers
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_ciphers_get_ciphers()
		 * 
		 * Could not generate a wrapper for purple_ciphers_get_ciphers in file "cipher.h".
		 * Message: Unknown inner-type of argument 0 of purple_ciphers_get_ciphers
		 */

		/*
		 * Function Name: purple_ciphers_get_handle
		 * Function Type: gpointer
		 * Function RawType: gpointer
		 * Function Category: VoidPointer
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
		 * Function Name: purple_ciphers_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_ciphers_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_cipher_context_set_option
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cipher_context_set_option(PurpleCipherContext * context, gchar * name, gpointer value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_option(IntPtr context, string name, IntPtr value);

		public static void ContextSetOption(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context, string name, IntPtr value)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_get_option
		 * Function Type: gpointer
		 * Function RawType: gpointer
		 * Function Category: VoidPointer
		 */
		/*
		 * gpointer purple_cipher_context_get_option(PurpleCipherContext * context, gchar * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cipher_context_get_option(IntPtr context, string name);

		public static IntPtr ContextGetOption(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context, string name)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_new
		 * Function Type: PurpleCipherContext *
		 * Function RawType: PurpleCipherContext
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleCipherContext * purple_cipher_context_new(PurpleCipher * cipher, void * extra)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cipher_context_new(IntPtr cipher, IntPtr extra);

		public static /* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr ContextNew(/* Resolved as PointerToUnknownStruct to PurpleCipher */ IntPtr cipher, IntPtr extra)
		{
			/* Unable to process cipher, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_new_by_name
		 * Function Type: PurpleCipherContext *
		 * Function RawType: PurpleCipherContext
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleCipherContext * purple_cipher_context_new_by_name(gchar * name, void * extra)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cipher_context_new_by_name(string name, IntPtr extra);

		public static /* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr ContextNewByName(string name, IntPtr extra)
		{
			/* Unable to process the return value of purple_cipher_context_new_by_name, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_reset
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cipher_context_reset(PurpleCipherContext * context, gpointer extra)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_reset(IntPtr context, IntPtr extra);

		public static void ContextReset(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context, IntPtr extra)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cipher_context_destroy(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_destroy(IntPtr context);

		public static void ContextDestroy(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_set_iv
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cipher_context_set_iv(PurpleCipherContext * context, guchar * iv, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_iv(IntPtr context, IntPtr iv, ulong len);

		public static void ContextSetIv(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context, /* Resolved as PointerToUnknownStruct to guchar */ IntPtr iv, ulong len)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			/* Unable to process iv, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_append
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cipher_context_append(PurpleCipherContext * context, guchar * data, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_append(IntPtr context, IntPtr data, ulong len);

		public static void ContextAppend(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context, /* Resolved as PointerToUnknownStruct to guchar */ IntPtr data, ulong len)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			/* Unable to process data, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_set_salt
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cipher_context_set_salt(PurpleCipherContext * context, guchar * salt)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_salt(IntPtr context, IntPtr salt);

		public static void ContextSetSalt(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context, /* Resolved as PointerToUnknownStruct to guchar */ IntPtr salt)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			/* Unable to process salt, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_get_salt_size
		 * Function Type: size_t
		 * Function RawType: size_t
		 * Function Category: Native
		 */
		/*
		 * size_t purple_cipher_context_get_salt_size(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_cipher_context_get_salt_size(IntPtr context);

		public static ulong ContextGetSaltSize(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_set_key
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cipher_context_set_key(PurpleCipherContext * context, guchar * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_key(IntPtr context, IntPtr key);

		public static void ContextSetKey(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context, /* Resolved as PointerToUnknownStruct to guchar */ IntPtr key)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			/* Unable to process key, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_get_key_size
		 * Function Type: size_t
		 * Function RawType: size_t
		 * Function Category: Native
		 */
		/*
		 * size_t purple_cipher_context_get_key_size(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_cipher_context_get_key_size(IntPtr context);

		public static ulong ContextGetKeySize(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_set_batch_mode
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cipher_context_set_batch_mode(PurpleCipherContext * context, PurpleCipherBatchMode mode)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_batch_mode(IntPtr context, Cipher.PurpleCipherBatchMode mode);

		public static void ContextSetBatchMode(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context, Cipher.PurpleCipherBatchMode mode)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			/* Unable to process mode, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_get_batch_mode
		 * Function Type: PurpleCipherBatchMode
		 * Function RawType: PurpleCipherBatchMode
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleCipherBatchMode purple_cipher_context_get_batch_mode(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern Cipher.PurpleCipherBatchMode purple_cipher_context_get_batch_mode(IntPtr context);

		public static Cipher.PurpleCipherBatchMode ContextGetBatchMode(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_get_block_size
		 * Function Type: size_t
		 * Function RawType: size_t
		 * Function Category: Native
		 */
		/*
		 * size_t purple_cipher_context_get_block_size(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_cipher_context_get_block_size(IntPtr context);

		public static ulong ContextGetBlockSize(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_set_key_with_len
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cipher_context_set_key_with_len(PurpleCipherContext * context, guchar * key, size_t len)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_key_with_len(IntPtr context, IntPtr key, ulong len);

		public static void ContextSetKeyWithLen(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context, /* Resolved as PointerToUnknownStruct to guchar */ IntPtr key, ulong len)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			/* Unable to process key, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_set_data
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cipher_context_set_data(PurpleCipherContext * context, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cipher_context_set_data(IntPtr context, IntPtr data);

		public static void ContextSetData(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context, IntPtr data)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_cipher_context_get_data
		 * Function Type: gpointer
		 * Function RawType: gpointer
		 * Function Category: VoidPointer
		 */
		/*
		 * gpointer purple_cipher_context_get_data(PurpleCipherContext * context)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cipher_context_get_data(IntPtr context);

		public static IntPtr ContextGetData(/* Resolved as PointerToUnknownStruct to PurpleCipherContext */ IntPtr context)
		{
			/* Unable to process context, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}


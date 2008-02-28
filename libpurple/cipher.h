/**
 * @file cipher.h Purple Cipher API
 * @ingroup core
 */

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
#ifndef PURPLE_CIPHER_H
#define PURPLE_CIPHER_H

#include <glib.h>

/*******************************************************************************
 * New Cipher Header
 ******************************************************************************/
#include <glib-object.h>

#define PURPLE_TYPE_CIPHER				(purple_cipher_get_gtype())
#define PURPLE_CIPHER(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), PURPLE_TYPE_CIPHER, PurpleCipher))
#define PURPLE_CIPHER_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), PURPLE_TYPE_CIPHER, PurpleCipherClass))
#define PURPLE_IS_CIPHER(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), PURPLE_TYPE_CIPHER))
#define PURPLE_IS_CIPHER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), PURPLE_TYPE_CIPHER))
#define PURPLE_CIPHER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), PURPLE_TYPE_CIPHER, PurpleCipherClass))

typedef struct _PurpleCipher			PurpleCipher;
typedef struct _PurpleCipherPriv		PurpleCipherPriv;
typedef struct _PurpleCipherClass		PurpleCipherClass;

#define PURPLE_TYPE_CIPHER_BATCH_MODE	(purple_cipher_batch_mode_get_gtype())

/**
 * Modes for batch encrypters
 */
typedef enum  {
	PURPLE_CIPHER_BATCH_MODE_ECB,
	PURPLE_CIPHER_BATCH_MODE_CBC
} PurpleCipherBatchMode;

struct _PurpleCipher {
	GObject gparent;

	PurpleCipherPriv *priv;

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
};

struct _PurpleCipherClass {
	GObjectClass gparent;

	/** The reset function */
	void (*reset)(PurpleCipher *cipher);

	/** The set initialization vector function */
	void (*set_iv)(PurpleCipher *cipher, guchar *iv, size_t len);

	/** The append data function */
	void (*append)(PurpleCipher *cipher, const guchar *data, size_t len);

	/** The digest function */
	gboolean (*digest)(PurpleCipher *cipher, size_t in_len, guchar digest[], size_t *out_len);

	/** The encrypt function */
	int (*encrypt)(PurpleCipher *cipher, const guchar data[], size_t len, guchar output[], size_t *outlen);

	/** The decrypt function */
	int (*decrypt)(PurpleCipher *cipher, const guchar data[], size_t len, guchar output[], size_t *outlen);

	/** The set salt function */
	void (*set_salt)(PurpleCipher *cipher, guchar *salt);

	/** The get salt size function */
	size_t (*get_salt_size)(PurpleCipher *cipher);

	/** The set key function */
	void (*set_key)(PurpleCipher *cipher, const guchar *key);

	/** The get key size function */
	size_t (*get_key_size)(PurpleCipher *cipher);

	/** The set batch mode function */
	void (*set_batch_mode)(PurpleCipher *cipher, PurpleCipherBatchMode mode);

	/** The get batch mode function */
	PurpleCipherBatchMode (*get_batch_mode)(PurpleCipher *cipher);

	/** The get block size function */
	size_t (*get_block_size)(PurpleCipher *cipher);

	/** The set key with length function */
	void (*set_key_with_len)(PurpleCipher *cipher, const guchar *key, size_t len);

	/** The get hash function */
	PurpleCipher *(*get_hash)(PurpleCipher *cipher);

	/** The set hash function */
	void (*set_hash)(PurpleCipher *cipher, PurpleCipher *hash);

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
};

G_BEGIN_DECLS

/*****************************************************************************/
/** @name PurpleCipher API													 */
/*****************************************************************************/
/*@{*/

GType purple_cipher_get_gtype(void);
GType purple_cipher_batch_mode_get_gtype(void);

/**
 * Resets a cipher to it's default value
 * @note If you have set an IV you will have to set it after resetting
 *
 * @param cipher  The cipher to reset
  */
void purple_cipher_reset(PurpleCipher *cipher);

/**
 * Sets the initialization vector for a cipher
 * @note This should only be called right after a cipher is created or reset
 *
 * @param cipher  The cipher to set the IV to
 * @param iv      The initialization vector to set
 * @param len     The len of the IV
 */
void purple_cipher_set_iv(PurpleCipher *cipher, guchar *iv, size_t len);

/**
 * Appends data to the cipher
 *
 * @param cipher  The cipher to append data to
 * @param data    The data to append
 * @param len     The length of the data
 */
void purple_cipher_append(PurpleCipher *cipher, const guchar *data, size_t len);

/**
 * Digests a cipher
 *
 * @param cipher  The cipher to digest
 * @param in_len  The length of the buffer
 * @param digest  The return buffer for the digest
 * @param out_len The length of the returned value
 */
gboolean purple_cipher_digest(PurpleCipher *cipher, size_t in_len, guchar digest[], size_t *out_len);

/**
 * Converts a guchar digest into a hex string
 *
 * @param cipher   The cipher to get a digest from
 * @param in_len   The length of the buffer
 * @param digest_s The return buffer for the string digest
 * @param out_len  The length of the returned value
 */
gboolean purple_cipher_digest_to_str(PurpleCipher *cipher, size_t in_len, gchar digest_s[], size_t *out_len);

/**
 * Encrypts data using the cipher
 *
 * @param cipher  The cipher
 * @param data    The data to encrypt
 * @param len     The length of the data
 * @param output  The output buffer
 * @param outlen  The len of data that was outputed
 *
 * @return A cipher specific status code
 */
gint purple_cipher_encrypt(PurpleCipher *cipher, const guchar data[], size_t len, guchar output[], size_t *outlen);

/**
 * Decrypts data using the cipher
 *
 * @param cipher  The cipher
 * @param data    The data to encrypt
 * @param len     The length of the returned value
 * @param output  The output buffer
 * @param outlen  The len of data that was outputed
 *
 * @return A cipher specific status code
 */
gint purple_cipher_decrypt(PurpleCipher *cipher, const guchar data[], size_t len, guchar output[], size_t *outlen);

/**
 * Sets the salt on a cipher
 *
 * @param cipher  The cipher whose salt to set
 * @param salt    The salt
 */
void purple_cipher_set_salt(PurpleCipher *cipher, guchar *salt);

/**
 * Gets the size of the salt if the cipher supports it
 *
 * @param cipher  The cipher whose salt size to get
 *
 * @return The size of the salt
 */
size_t purple_cipher_get_salt_size(PurpleCipher *cipher);

/**
 * Sets the key on a cipher
 *
 * @param cipher  The cipher whose key to set
 * @param key     The key
 */
void purple_cipher_set_key(PurpleCipher *cipher, const guchar *key);

/**
 * Gets the key size for a cipher
 *
 * @param cipher  The cipher whose key size to get
 *
 * @return The size of the key
 */
size_t purple_cipher_get_key_size(PurpleCipher *cipher);

/**
 * Sets the batch mode of a cipher
 *
 * @param cipher  The cipher whose batch mode to set
 * @param mode    The batch mode under which the cipher should operate
 *
 */
void purple_cipher_set_batch_mode(PurpleCipher *cipher, PurpleCipherBatchMode mode);

/**
 * Gets the batch mode of a cipher
 *
 * @param cipher  The cipher whose batch mode to get
 *
 * @return The batch mode under which the cipher is operating
 */
PurpleCipherBatchMode purple_cipher_get_batch_mode(PurpleCipher *cipher);

/**
 * Gets the block size of a cipher
 *
 * @param cipher  The cipher whose block size to get
 *
 * @return The block size of the cipher
 */
size_t purple_cipher_get_block_size(PurpleCipher *cipher);

/**
 * Sets the key with a given length on a cipher
 *
 * @param cipher  The cipher whose key to set
 * @param key     The key
 * @param len     The length of the key
 *
 */
void purple_cipher_set_key_with_len(PurpleCipher *cipher, const guchar *key, size_t len);

/**
 * Sets the internal hash of a cipher
 *
 * @param cipher   The cipher whose hash to set
 * @param hash	   The internal hash the cipher should use
 *
 */
void purple_cipher_set_hash(PurpleCipher *cipher, PurpleCipher *hash);

/**
 * Gets the internal hash of a cipher
 *
 * @param cipher   The cipher whose hash to get
 *
 * @return The internal hash the cipher is using.
 */
PurpleCipher *purple_cipher_get_hash(PurpleCipher *cipher);

/*@}*/

G_END_DECLS

#endif /* PURPLE_CIPHER_H */

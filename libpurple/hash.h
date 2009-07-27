/**
 * @file hash.h Purple Hash API
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
#ifndef PURPLE_HASH_H
#define PURPLE_HASH_H

#include <glib.h>

#include <pobject.h>

#define PURPLE_TYPE_HASH				(purple_hash_get_gtype())
#define PURPLE_HASH(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), PURPLE_TYPE_HASH, PurpleHash))
#define PURPLE_HASH_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), PURPLE_TYPE_HASH, PurpleHashClass))
#define PURPLE_IS_HASH(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), PURPLE_TYPE_HASH))
#define PURPLE_IS_HASH_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), PURPLE_TYPE_HASH))
#define PURPLE_HASH_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), PURPLE_TYPE_HASH, PurpleHashClass))

typedef struct _PurpleHash			PurpleHash;
typedef struct _PurpleHashClass		PurpleHashClass;

struct _PurpleHash {
	PurpleObject parent;

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
};

struct _PurpleHashClass {
	PurpleObjectClass parent;

	/** The reset function */
	void (*reset)(PurpleHash *hash);

	/** The set initialization vector function */
	void (*set_iv)(PurpleHash *hash, guchar *iv, size_t len);

	/** The append data function */
	void (*append)(PurpleHash *hash, const guchar *data, size_t len);

	/** The digest function */
	gboolean (*digest)(PurpleHash *hash, size_t in_len, guchar digest[], size_t *out_len);

	/** The set salt function */
	void (*set_salt)(PurpleHash *hash, guchar *salt);

	/** The get salt size function */
	size_t (*get_salt_size)(PurpleHash *hash);

	/** The set key function */
	void (*set_key)(PurpleHash *hash, const guchar *key);

	/** The get key size function */
	size_t (*get_key_length)(PurpleHash *hash);

	/** The set key with length function */
	void (*set_key_with_length)(PurpleHash *hash, const guchar *key, size_t length);

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
};

G_BEGIN_DECLS

/*****************************************************************************/
/** @name PurpleHash API													 */
/*****************************************************************************/
/*@{*/

GType purple_hash_get_gtype(void);

/**
 * Resets a hash to it's default value
 * @note If you have set an IV you will have to set it after resetting
 *
 * @param hash  The hash to reset
  */
void purple_hash_reset(PurpleHash *hash);

/**
 * Sets the initialization vector for a hash
 * @note This should only be called right after a hash is created or reset
 *
 * @param hash  The hash to set the IV to
 * @param iv      The initialization vector to set
 * @param len     The len of the IV
 */
void purple_hash_set_iv(PurpleHash *hash, guchar *iv, size_t len);

/**
 * Appends data to the hash
 *
 * @param hash  The hash to append data to
 * @param data    The data to append
 * @param len     The length of the data
 */
void purple_hash_append(PurpleHash *hash, const guchar *data, size_t len);

/**
 * Digests a hash
 *
 * @param hash  The hash to digest
 * @param in_len  The length of the buffer
 * @param digest  The return buffer for the digest
 * @param out_len The length of the returned value
 */
gboolean purple_hash_digest(PurpleHash *hash, size_t in_len, guchar digest[], size_t *out_len);

/**
 * Converts a guchar digest into a hex string
 *
 * @param hash   The hash to get a digest from
 * @param in_len   The length of the buffer
 * @param digest_s The return buffer for the string digest
 * @param out_len  The length of the returned value
 */
gboolean purple_hash_digest_to_str(PurpleHash *hash, size_t in_len, gchar digest_s[], size_t *out_len);

/**
 * Sets the salt on a hash
 *
 * @param hash  The hash whose salt to set
 * @param salt    The salt
 */
void purple_hash_set_salt(PurpleHash *hash, guchar *salt);

/**
 * Gets the size of the salt if the hash supports it
 *
 * @param hash  The hash whose salt size to get
 *
 * @return The size of the salt
 */
size_t purple_hash_get_salt_size(PurpleHash *hash);

/**
 * Sets the key on a hash
 *
 * @param hash  The hash whose key to set
 * @param key     The key
 */
void purple_hash_set_key(PurpleHash *hash, const guchar *key);

/**
 * Gets the key size for a hash
 *
 * @param hash  The hash whose key size to get
 *
 * @return The size of the key
 */
size_t purple_hash_get_key_length(PurpleHash *hash);

/**
 * Sets the key with a given length on a hash
 *
 * @param hash  The hash whose key to set
 * @param key     The key
 * @param len     The length of the key
 *
 */
void purple_hash_set_key_with_length(PurpleHash *hash, const guchar *key, size_t length);

/*@}*/

G_END_DECLS

#endif /* PURPLE_HASH_H */

/**
 * @file cipher.h Purple Cipher API
 * @ingroup core
 *
 * purple
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * @see @ref cipher-signals
 */
#ifndef PURPLE_CIPHER_H
#define PURPLE_CIPHER_H

#include <glib.h>
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

	void (*reset)(PurpleCipher *cipher);

	void (*set_iv)(PurpleCipher *cipher, guchar *iv, size_t len);

	void (*append)(PurpleCipher *cipher, const guchar *data, size_t len);

	gboolean (*digest)(PurpleCipher *cipher, size_t in_len, guchar digest[], size_t *out_len);

	int (*encrypt)(PurpleCipher *cipher, const guchar data[], size_t len, guchar output[], size_t *outlen);
	int (*decrypt)(PurpleCipher *cipher, const guchar data[], size_t len, guchar output[], size_t *outlen);

	void (*set_salt)(PurpleCipher *cipher, guchar *salt);
	size_t (*get_salt_size)(PurpleCipher *cipher);

	void (*set_key)(PurpleCipher *cipher, const guchar *key);
	size_t (*get_key_size)(PurpleCipher *cipher);

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
};

G_BEGIN_DECLS

GType purple_cipher_get_gtype(void);

void purple_cipher_reset(PurpleCipher *cipher);
void purple_cipher_set_iv(PurpleCipher *cipher, guchar *iv, size_t len);
void purple_cipher_append(PurpleCipher *cipher, const guchar *data, size_t len);
gboolean purple_cipher_digest(PurpleCipher *cipher, size_t in_len, guchar digest[], size_t *out_len);
gboolean purple_cipher_digest_to_str(PurpleCipher *cipher, size_t in_len, gchar digest_s[], size_t *out_len);
gint purple_cipher_encrypt(PurpleCipher *cipher, const guchar data[], size_t len, guchar output[], size_t *outlen);
gint purple_cipher_decrypt(PurpleCipher *cipher, const guchar data[], size_t len, guchar output[], size_t *outlen);
void purple_cipher_set_salt(PurpleCipher *cipher, guchar *salt);
size_t purple_cipher_get_salt_size(PurpleCipher *cipher);
void purple_cipher_set_key(PurpleCipher *cipher, const guchar *key);
size_t purple_cipher_get_key_size(PurpleCipher *cipher);
void purple_cipher_set_data(PurpleCipher *cipher, gpointer data);
gpointer purple_cipher_get_data(PurpleCipher *cipher);

/*@}*/
/*****************************************************************************/
/** @name Purple Cipher HTTP Digest Helper Functions							 */
/*****************************************************************************/
/*@{*/

/**
 * Calculates a session key for HTTP Digest authentation
 *
 * See RFC 2617 for more information.
 *
 * @param algorithm    The hash algorithm to use
 * @param username     The username provided by the user
 * @param realm        The authentication realm provided by the server
 * @param password     The password provided by the user
 * @param nonce        The nonce provided by the server
 * @param client_nonce The nonce provided by the client
 *
 * @return The session key, or @c NULL if an error occurred.
 */
gchar *purple_cipher_http_digest_calculate_session_key(
		const gchar *algorithm, const gchar *username,
		const gchar *realm, const gchar *password,
		const gchar *nonce, const gchar *client_nonce);

/** Calculate a response for HTTP Digest authentication
 *
 * See RFC 2617 for more information.
 *
 * @param algorithm         The hash algorithm to use
 * @param method            The HTTP method in use
 * @param digest_uri        The URI from the initial request
 * @param qop               The "quality of protection"
 * @param entity            The entity body
 * @param nonce             The nonce provided by the server
 * @param nonce_count       The nonce count
 * @param client_nonce      The nonce provided by the client
 * @param session_key       The session key from purple_cipher_http_digest_calculate_session_key()
 *
 * @return The hashed response, or @c NULL if an error occurred.
 */
gchar *purple_cipher_http_digest_calculate_response(
		const gchar *algorithm, const gchar *method,
		const gchar *digest_uri, const gchar *qop,
		const gchar *entity, const gchar *nonce,
		const gchar *nonce_count, const gchar *client_nonce,
		const gchar *session_key);

/*@}*/

G_END_DECLS

#endif /* PURPLE_CIPHER_H */

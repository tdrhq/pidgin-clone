/*
 * purple
 *
 * Purple is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 */
#include <glib.h>
#include <string.h>
#include <stdio.h>

#include "internal.h"
#include "cipher.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "signals.h"
#include "value.h"

/*******************************************************************************
 * Structs
 ******************************************************************************/

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_cipher_class_init(PurpleCipherClass *klass) {
	parent_class = g_type_class_peek_parent(klass);
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
purple_cipher_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleCipherClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_cipher_class_init,
			NULL,
			NULL,
			sizeof(PurpleCipher),
			0,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
									  "PurpleCipher",
									  &info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}

void
purple_cipher_reset(PurpleCipher *cipher) {
	PurpleCipherClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_CIPHER(cipher));

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->reset)
		klass->reset(cipher);
}

void
purple_cipher_set_iv(PurpleCipher *cipher, guchar *iv, size_t len) {
	PurpleCipherClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_CIPHER(cipher));
	g_return_if_fail(iv);

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->set_iv)
		klass->set_iv(cipher, iv, len);
}

void
purple_cipher_append(PurpleCipher *cipher, const guchar *data, size_t len) {
	PurpleCipherClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_CIPHER(cipher));

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->append)
		klass->append(cipher, data, len);
}

gboolean
purple_cipher_digest(PurpleCipher *cipher, size_t in_len, guchar digest[],
					 size_t *out_len)
{
	PurpleCipherClass *klass = NULL;

	g_return_val_if_fail(PURPLE_IS_CIPHER(cipher), FALSE);

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->digest)
		return klass->digest(cipher, in_len, digest, out_len);

	return FALSE;
}

gboolean
purple_cipher_digest_to_str(PurpleCipher *cipher, size_t in_len,
							gchar digest_s[], size_t *out_len)
{
	/* 8k is a bit excessive, will tweak later. */
	guchar digest[BUF_LEN * 4];
	gint n = 0;
	size_t dlen = 0;

	g_return_val_if_fail(cipher, FALSE);
	g_return_val_if_fail(digest_s, FALSE);

	if(!purple_cipher_digest(cipher, sizeof(digest), digest, &dlen))
		return FALSE;

	/* in_len must be greater than dlen * 2 so we have room for the NUL. */
	if(in_len <= dlen * 2)
		return FALSE;

	for(n = 0; n < dlen; n++)
		sprintf(digest_s + (n * 2), "%02x", digest[n]);

	digest_s[n * 2] = '\0';

	if(out_len)
		*out_len = dlen * 2;

	return TRUE;
}

gint
purple_cipher_encrypt(PurpleCipher *cipher, const guchar data[], size_t len,
					  guchar output[], size_t *outlen)
{
	PurpleCipherClass *klass = NULL;

	g_return_val_if_fail(PURPLE_IS_CIPHER(cipher), -1);

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->encrypt)
		return klass->encrypt(cipher, data, len, output, outlen);

	if(outlen)
		*outlen = -1;

	return -1;
}

gint
purple_cipher_decrypt(PurpleCipher *cipher, const guchar data[], size_t len,
					  guchar output[], size_t *outlen)
{
	PurpleCipherClass *klass = NULL;

	g_return_val_if_fail(PURPLE_IS_CIPHER(cipher), -1);

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->decrypt)
		return klass->decrypt(cipher, data, len, output, outlen);

	if(outlen)
		*outlen = -1;

	return -1;
}

void
purple_cipher_set_salt(PurpleCipher *cipher, guchar *salt) {
	PurpleCipherClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_CIPHER(cipher));

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->set_salt)
		klass->set_salt(cipher, salt);
}

size_t
purple_cipher_get_salt_size(PurpleCipher *cipher) {
	PurpleCipherClass *klass = NULL;

	g_return_val_if_fail(PURPLE_IS_CIPHER(cipher), -1);

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->get_salt_size)
		return klass->get_salt_size(cipher);

	return -1;
}

void
purple_cipher_set_key(PurpleCipher *cipher, const guchar *key) {
	PurpleCipherClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_CIPHER(cipher));

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->set_key)
		klass->set_key(cipher, key);
}

size_t
purple_cipher_get_key_size(PurpleCipher *cipher) {
	PurpleCipherClass *klass = NULL;

	g_return_val_if_fail(PURPLE_IS_CIPHER(cipher), -1);

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->get_key_size)
		return klass->get_key_size(cipher);

	return -1;
}

#if 0

gchar *purple_cipher_http_digest_calculate_session_key(
		const gchar *algorithm,
		const gchar *username,
		const gchar *realm,
		const gchar *password,
		const gchar *nonce,
		const gchar *client_nonce)
{
	PurpleCipher *cipher;
	PurpleCipherContext *cipher;
	gchar hash[33]; /* We only support MD5. */

	g_return_val_if_fail(username != NULL, NULL);
	g_return_val_if_fail(realm    != NULL, NULL);
	g_return_val_if_fail(password != NULL, NULL);
	g_return_val_if_fail(nonce    != NULL, NULL);

	/* Check for a supported algorithm. */
	g_return_val_if_fail(algorithm == NULL ||
						 *algorithm == '\0' ||
						 g_ascii_strcasecmp(algorithm, "MD5") ||
						 g_ascii_strcasecmp(algorithm, "MD5-sess"), NULL);

	cipher = purple_ciphers_find_cipher("md5");
	g_return_val_if_fail(cipher != NULL, NULL);

	cipher = purple_cipher_cipher_new(cipher, NULL);

	purple_cipher_cipher_append(cipher, (guchar *)username, strlen(username));
	purple_cipher_cipher_append(cipher, (guchar *)":", 1);
	purple_cipher_cipher_append(cipher, (guchar *)realm, strlen(realm));
	purple_cipher_cipher_append(cipher, (guchar *)":", 1);
	purple_cipher_cipher_append(cipher, (guchar *)password, strlen(password));

	if (algorithm != NULL && !strcasecmp(algorithm, "MD5-sess"))
	{
		guchar digest[16];

		if (client_nonce == NULL)
		{
			purple_cipher_cipher_destroy(cipher);
			purple_debug_error("cipher", "Required client_nonce missing for MD5-sess digest calculation.");
			return NULL;
		}

		purple_cipher_cipher_digest(cipher, sizeof(digest), digest, NULL);
		purple_cipher_cipher_destroy(cipher);

		cipher = purple_cipher_cipher_new(cipher, NULL);
		purple_cipher_cipher_append(cipher, digest, sizeof(digest));
		purple_cipher_cipher_append(cipher, (guchar *)":", 1);
		purple_cipher_cipher_append(cipher, (guchar *)nonce, strlen(nonce));
		purple_cipher_cipher_append(cipher, (guchar *)":", 1);
		purple_cipher_cipher_append(cipher, (guchar *)client_nonce, strlen(client_nonce));
	}

	purple_cipher_cipher_digest_to_str(cipher, sizeof(hash), hash, NULL);
	purple_cipher_cipher_destroy(cipher);

	return g_strdup(hash);
}

gchar *purple_cipher_http_digest_calculate_response(
		const gchar *algorithm,
		const gchar *method,
		const gchar *digest_uri,
		const gchar *qop,
		const gchar *entity,
		const gchar *nonce,
		const gchar *nonce_count,
		const gchar *client_nonce,
		const gchar *session_key)
{
	PurpleCipher *cipher;
	PurpleCipherContext *cipher;
	static gchar hash2[33]; /* We only support MD5. */

	g_return_val_if_fail(method      != NULL, NULL);
	g_return_val_if_fail(digest_uri  != NULL, NULL);
	g_return_val_if_fail(nonce       != NULL, NULL);
	g_return_val_if_fail(session_key != NULL, NULL);

	/* Check for a supported algorithm. */
	g_return_val_if_fail(algorithm == NULL ||
						 *algorithm == '\0' ||
						 g_ascii_strcasecmp(algorithm, "MD5") ||
						 g_ascii_strcasecmp(algorithm, "MD5-sess"), NULL);

	/* Check for a supported "quality of protection". */
	g_return_val_if_fail(qop == NULL ||
						 *qop == '\0' ||
						 g_ascii_strcasecmp(qop, "auth") ||
						 g_ascii_strcasecmp(qop, "auth-int"), NULL);

	cipher = purple_ciphers_find_cipher("md5");
	g_return_val_if_fail(cipher != NULL, NULL);

	cipher = purple_cipher_cipher_new(cipher, NULL);

	purple_cipher_cipher_append(cipher, (guchar *)method, strlen(method));
	purple_cipher_cipher_append(cipher, (guchar *)":", 1);
	purple_cipher_cipher_append(cipher, (guchar *)digest_uri, strlen(digest_uri));

	if (qop != NULL && !g_ascii_strcasecmp(qop, "auth-int"))
	{
		PurpleCipherContext *cipher2;
		gchar entity_hash[33];

		if (entity == NULL)
		{
			purple_cipher_cipher_destroy(cipher);
			purple_debug_error("cipher", "Required entity missing for auth-int digest calculation.");
			return NULL;
		}

		cipher2 = purple_cipher_cipher_new(cipher, NULL);
		purple_cipher_cipher_append(cipher2, (guchar *)entity, strlen(entity));
		purple_cipher_cipher_digest_to_str(cipher2, sizeof(entity_hash), entity_hash, NULL);
		purple_cipher_cipher_destroy(cipher2);

		purple_cipher_cipher_append(cipher, (guchar *)":", 1);
		purple_cipher_cipher_append(cipher, (guchar *)entity_hash, strlen(entity_hash));
	}

	purple_cipher_cipher_digest_to_str(cipher, sizeof(hash2), hash2, NULL);
	purple_cipher_cipher_destroy(cipher);

	cipher = purple_cipher_cipher_new(cipher, NULL);
	purple_cipher_cipher_append(cipher, (guchar *)session_key, strlen(session_key));
	purple_cipher_cipher_append(cipher, (guchar *)":", 1);
	purple_cipher_cipher_append(cipher, (guchar *)nonce, strlen(nonce));
	purple_cipher_cipher_append(cipher, (guchar *)":", 1);

	if (qop != NULL && *qop != '\0')
	{
		if (nonce_count == NULL)
		{
			purple_cipher_cipher_destroy(cipher);
			purple_debug_error("cipher", "Required nonce_count missing for digest calculation.");
			return NULL;
		}

		if (client_nonce == NULL)
		{
			purple_cipher_cipher_destroy(cipher);
			purple_debug_error("cipher", "Required client_nonce missing for digest calculation.");
			return NULL;
		}

		purple_cipher_cipher_append(cipher, (guchar *)nonce_count, strlen(nonce_count));
		purple_cipher_cipher_append(cipher, (guchar *)":", 1);
		purple_cipher_cipher_append(cipher, (guchar *)client_nonce, strlen(client_nonce));
		purple_cipher_cipher_append(cipher, (guchar *)":", 1);

		purple_cipher_cipher_append(cipher, (guchar *)qop, strlen(qop));

		purple_cipher_cipher_append(cipher, (guchar *)":", 1);
	}

	purple_cipher_cipher_append(cipher, (guchar *)hash2, strlen(hash2));
	purple_cipher_cipher_digest_to_str(cipher, sizeof(hash2), hash2, NULL);
	purple_cipher_cipher_destroy(cipher);

	return g_strdup(hash2);
}
#endif

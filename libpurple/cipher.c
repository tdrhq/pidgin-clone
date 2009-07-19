/*
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */
#include "internal.h"
#include "cipher.h"

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
 * PurpleCipher API
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

GType
purple_cipher_batch_mode_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GEnumValue values[] = {
			{ PURPLE_CIPHER_BATCH_MODE_ECB, "ECB", "ECB" },
			{ PURPLE_CIPHER_BATCH_MODE_CBC, "CBC", "CBC" },
			{ 0, NULL, NULL },
		};

		type = g_enum_register_static("PurpleCipherBatchMode", values);
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
purple_cipher_set_iv(PurpleCipher *cipher, guchar *iv, size_t len)
{
	PurpleCipherClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_CIPHER(cipher));
	g_return_if_fail(iv);

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->set_iv)
		klass->set_iv(cipher, iv, len);
}

void
purple_cipher_append(PurpleCipher *cipher, const guchar *data,
								size_t len)
{
	PurpleCipherClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_CIPHER(cipher));

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->append)
		klass->append(cipher, data, len);
}

gboolean
purple_cipher_digest(PurpleCipher *cipher, size_t in_len,
						   guchar digest[], size_t *out_len)
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
purple_cipher_encrypt(PurpleCipher *cipher, const guchar data[],
							size_t len, guchar output[], size_t *outlen)
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
purple_cipher_decrypt(PurpleCipher *cipher, const guchar data[],
							size_t len, guchar output[], size_t *outlen)
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

void
purple_cipher_set_batch_mode(PurpleCipher *cipher,
                                     PurpleCipherBatchMode mode)
{
	PurpleCipherClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_CIPHER(cipher));

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->set_batch_mode)
		klass->set_batch_mode(cipher, mode);
}

PurpleCipherBatchMode
purple_cipher_get_batch_mode(PurpleCipher *cipher)
{
	PurpleCipherClass *klass = NULL;

	g_return_val_if_fail(PURPLE_IS_CIPHER(cipher), -1);

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->get_batch_mode)
		return klass->get_batch_mode(cipher);

	return -1;
}

size_t
purple_cipher_get_block_size(PurpleCipher *cipher)
{
	PurpleCipherClass *klass = NULL;

	g_return_val_if_fail(PURPLE_IS_CIPHER(cipher), -1);

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->get_block_size)
		return klass->get_block_size(cipher);

	return -1;
}

void
purple_cipher_set_key_with_len(PurpleCipher *cipher,
                                       const guchar *key, size_t len)
{
	PurpleCipherClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_CIPHER(cipher));
	g_return_if_fail(key);

	klass = PURPLE_CIPHER_GET_CLASS(cipher);

	if(klass && klass->set_key_with_len)
		klass->set_key_with_len(cipher, key, len);
}


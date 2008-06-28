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
#include "hmaccipher.h"

#include <string.h>

/*******************************************************************************
 * Structs
 ******************************************************************************/
struct _PurpleHMACCipherPriv {
	PurpleCipher *hash;
	int blocksize;
	guchar *opad;
};

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_NONE,
	PROP_BLOCK_SIZE,
	PROP_HASH,
	PROP_KEY,
	PROP_LAST,
};

/*******************************************************************************
 * Globals
 ******************************************************************************/
static GObjectClass *parent_class = NULL;

/*******************************************************************************
 * Cipher Stuff
 ******************************************************************************/
static void
purple_hmac_cipher_reset(PurpleCipher *cipher)
{
	PurpleHMACCipher *hmac_cipher = PURPLE_HMAC_CIPHER(cipher);

	if(hmac_cipher->priv->hash)
	{
		g_object_unref(G_OBJECT(hmac_cipher->priv->hash));
		hmac_cipher->priv->hash = NULL;
	}

	hmac_cipher->priv->blocksize = 0;

	if(hmac_cipher->priv->opad)
	{
		g_free(hmac_cipher->priv->opad);
		hmac_cipher->priv->opad = NULL;
	}
}

static void
purple_hmac_cipher_append(PurpleCipher *cipher, const guchar *data, size_t len)
{
	PurpleHMACCipher *hmac_cipher = PURPLE_HMAC_CIPHER(cipher);

	g_return_if_fail(hmac_cipher->priv->hash);

	purple_cipher_append(hmac_cipher->priv->hash, data, len);
}

static gboolean
purple_hmac_cipher_digest(PurpleCipher *cipher, size_t in_len, guchar *out, size_t *out_len)
{
	PurpleHMACCipher *hmac_cipher = PURPLE_HMAC_CIPHER(cipher);
	PurpleCipher *hash = hmac_cipher->priv->hash;
	guchar *inner_hash;
	size_t hash_len;
	gboolean result;

	g_return_val_if_fail(hash, FALSE);

	inner_hash = g_malloc(100); /* TODO: Should be enough for now... */
	result = purple_cipher_digest(hash, 100, inner_hash, &hash_len);

	purple_cipher_reset(hash);

	purple_cipher_append(hash, hmac_cipher->priv->opad, hmac_cipher->priv->blocksize);
	purple_cipher_append(hash, inner_hash, hash_len);

	g_free(inner_hash);

	result = result && purple_cipher_digest(hash, in_len, out, out_len);

	return result;
}

static void
purple_hmac_cipher_set_key_with_len(PurpleCipher *cipher, const guchar * key, size_t key_len)
{
	PurpleHMACCipher *hmac_cipher = PURPLE_HMAC_CIPHER(cipher);
	int blocksize, i;
	guchar *ipad;
	guchar *full_key;

	g_return_if_fail(hmac_cipher->priv->hash);

	g_free(hmac_cipher->priv->opad);

	blocksize = hmac_cipher->priv->blocksize;
	ipad = g_malloc(blocksize);
	hmac_cipher->priv->opad = g_malloc(blocksize);

	if (key_len > blocksize) {
		purple_cipher_reset(hmac_cipher->priv->hash);
		purple_cipher_append(hmac_cipher->priv->hash, key, key_len);
		full_key = g_malloc(100); /* TODO: Should be enough for now... */
		purple_cipher_digest(hmac_cipher->priv->hash, 100, full_key, &key_len);
	} else
		full_key = g_memdup(key, key_len);

    if (key_len < blocksize) {
		full_key = g_realloc(full_key, blocksize);
		memset(full_key + key_len, 0, blocksize - key_len);
    }

	for(i = 0; i < blocksize; i++) {
		ipad[i] = 0x36 ^ full_key[i];
		hmac_cipher->priv->opad[i] = 0x5c ^ full_key[i];
	}

	g_free(full_key);

	purple_cipher_reset(hmac_cipher->priv->hash);
	purple_cipher_append(hmac_cipher->priv->hash, ipad, blocksize);
	g_free(ipad);

	g_object_notify(G_OBJECT(hmac_cipher), "key");
}

static void
purple_hmac_cipher_set_key(PurpleCipher *cipher, const guchar *key)
{
	purple_hmac_cipher_set_key_with_len(cipher, key, strlen((char *)key));
}

static size_t
purple_hmac_cipher_get_block_size(PurpleCipher *cipher)
{
	PurpleHMACCipher *hmac_cipher = PURPLE_HMAC_CIPHER(cipher);

	return hmac_cipher->priv->blocksize;
}

static void
purple_hmac_cipher_set_hash(PurpleCipher *cipher,
							PurpleCipher *hash)
{
	PurpleHMACCipher *hmac_cipher = PURPLE_HMAC_CIPHER(cipher);

#if GLIB_CHECK_VERSION(2,10,0)
	hmac_cipher->priv->hash = g_object_ref_sink(hash);
#else
	hmac_cipher->priv->hash = g_object_ref(G_OBJECT(hash));
#endif

	hmac_cipher->priv->blocksize = purple_cipher_get_block_size(hash);

	g_object_notify(G_OBJECT(hmac_cipher), "hash");
}

static PurpleCipher *
purple_hmac_cipher_get_hash(PurpleCipher *cipher)
{
	PurpleHMACCipher *hmac_cipher = PURPLE_HMAC_CIPHER(cipher);

	return hmac_cipher->priv->hash;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_hmac_cipher_set_property(GObject *obj, guint param_id,
								const GValue *value,
								GParamSpec *pspec)
{
	PurpleCipher *cipher = PURPLE_CIPHER(obj);

	switch(param_id) {
		case PROP_HASH:
			purple_cipher_set_hash(cipher, g_value_get_object(value));
			break;
		case PROP_KEY:
			purple_cipher_set_key(cipher, (guchar *)g_value_get_string(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_hmac_cipher_get_property(GObject *obj, guint param_id, GValue *value,
								GParamSpec *pspec)
{
	PurpleCipher *cipher = PURPLE_CIPHER(obj);

	switch(param_id) {
		case PROP_BLOCK_SIZE:
			g_value_set_int(value,
							purple_cipher_get_block_size(cipher));
			break;
		case PROP_HASH:
			g_value_set_object(value,
							   purple_cipher_get_hash(cipher));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_hmac_cipher_finalize(GObject *obj) {
	PurpleCipher *cipher = PURPLE_CIPHER(obj);
	PurpleHMACCipher *hmac_cipher = PURPLE_HMAC_CIPHER(obj);

	/* reset the cipher so we don't leave any data around... */
	purple_hmac_cipher_reset(cipher);

	g_free(hmac_cipher->priv);
}

static void
purple_hmac_cipher_class_init(PurpleHMACCipherClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	PurpleCipherClass *cipher_class = PURPLE_CIPHER_CLASS(klass);
	GParamSpec *pspec;

	parent_class = g_type_class_peek_parent(klass);

	obj_class->finalize = purple_hmac_cipher_finalize;
	obj_class->get_property = purple_hmac_cipher_get_property;
	obj_class->set_property = purple_hmac_cipher_set_property;

	cipher_class->reset = purple_hmac_cipher_reset;
	cipher_class->append = purple_hmac_cipher_append;
	cipher_class->digest = purple_hmac_cipher_digest;

	cipher_class->set_key = purple_hmac_cipher_set_key;
	cipher_class->set_key_with_len = purple_hmac_cipher_set_key_with_len;
	cipher_class->get_block_size = purple_hmac_cipher_get_block_size;
	cipher_class->set_hash = purple_hmac_cipher_set_hash;
	cipher_class->get_hash = purple_hmac_cipher_get_hash;

	pspec = g_param_spec_int("blockSize", "blockSize", "blockSize",
							 G_MININT, G_MAXINT, 0,
							 G_PARAM_READABLE);
	g_object_class_install_property(obj_class, PROP_BLOCK_SIZE, pspec);

	pspec = g_param_spec_object("hash", "hash", "hash", PURPLE_TYPE_CIPHER,
								G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_HASH, pspec);

	pspec = g_param_spec_string("key", "key", "key", NULL,
								G_PARAM_WRITABLE);
	g_object_class_install_property(obj_class, PROP_KEY, pspec);
}

static void
purple_hmac_cipher_init(PurpleCipher *cipher) {
	PurpleHMACCipher *hmac_cipher = PURPLE_HMAC_CIPHER(cipher);

	hmac_cipher->priv = g_new0(PurpleHMACCipherPriv, 1);

	purple_hmac_cipher_reset(cipher);
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
purple_hmac_cipher_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleHMACCipherClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_hmac_cipher_class_init,
			NULL,
			NULL,
			sizeof(PurpleHMACCipher),
			0,
			(GInstanceInitFunc)purple_hmac_cipher_init,
			NULL,
		};

		type = g_type_register_static(PURPLE_TYPE_CIPHER,
									  "PurpleHMACCipher",
									  &info, 0);
	}

	return type;
}

PurpleCipher *
purple_hmac_cipher_new(void) {
	return g_object_new(PURPLE_TYPE_HMAC_CIPHER, NULL);
}

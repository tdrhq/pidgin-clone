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
#include "des3cipher.h"
#include "descipher.h"

#include <string.h>

/******************************************************************************
 * Structs
 *****************************************************************************/
struct _PurpleDES3CipherPriv
{
	PurpleCipherBatchMode mode;
	guchar iv[8];
	/* First key for encryption */
	PurpleCipher *key1;
	/* Second key for decryption */
	PurpleCipher *key2;
	/* Third key for encryption */
	PurpleCipher *key3;
};

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_NONE,
	PROP_BATCH_MODE,
	PROP_IV,
	PROP_KEY,
	PROP_LAST,
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Cipher Stuff
 *****************************************************************************/
/*
 *  Fill a DES3 context with subkeys calculated from 3 64bit key.
 *  Does not check parity bits, but simply ignore them.
 *  Does not check for weak keys.
 **/
static void
purple_des3_cipher_set_key(PurpleCipher *cipher, const guchar * key)
{
	PurpleDES3Cipher *des3_cipher = PURPLE_DES3_CIPHER(cipher);

	purple_cipher_set_key(PURPLE_CIPHER(des3_cipher->priv->key1), key +  0);
	purple_cipher_set_key(PURPLE_CIPHER(des3_cipher->priv->key2), key +  8);
	purple_cipher_set_key(PURPLE_CIPHER(des3_cipher->priv->key3), key + 16);

	g_object_notify(G_OBJECT(des3_cipher), "key");
}

static gint
purple_des3_cipher_ecb_encrypt(PurpleDES3Cipher *des3_cipher, const guchar data[],
                 size_t len, guchar output[], size_t *outlen)
{
	int offset = 0;
	int i = 0;
	int tmp;
	guint8 buf[8] = {0,0,0,0,0,0,0,0};

	while (offset + 8 <= len) {
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key1),
									data + offset, output + offset, 0);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key2),
									output + offset, buf, 1);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key3),
									buf, output + offset, 0);

		offset += 8;
	}

	*outlen = len;

	if (offset < len) {
		*outlen += len - offset;
		tmp = offset;
		memset(buf, 0, 8);

		while (tmp < len) {
			buf[i++] = data[tmp];
			tmp++;
		}

		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key1),
									buf, output + offset, 0);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key2),
									output + offset, buf, 1);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key3),
									buf, output + offset, 0);
	}

	return 0;
}

static gint
purple_des3_cipher_cbc_encrypt(PurpleDES3Cipher *des3_cipher, const guchar data[],
							   size_t len, guchar output[], size_t *outlen)
{
	int offset = 0;
	int i = 0;
	int tmp;
	guint8 buf[8];

	memcpy(buf, des3_cipher->priv->iv, 8);

	while (offset + 8 <= len) {
		for (i = 0; i < 8; i++)
			buf[i] ^= data[offset + i];

		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key1),
									buf, output + offset, 0);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key2),
									output + offset, buf, 1);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key3),
									buf, output + offset, 0);

		memcpy(buf, output+offset, 8);
		offset += 8;
	}

	*outlen = len;

	if (offset < len) {
		*outlen += len - offset;
		tmp = offset;
		i = 0;

		while (tmp < len) {
			buf[i++] ^= data[tmp];
			tmp++;
		}

		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key1),
									buf, output + offset, 0);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key2),
									output + offset, buf, 1);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key3),
									buf, output + offset, 0);
	}

	return 0;
}

static gint
purple_des3_cipher_encrypt(PurpleCipher *cipher, const guchar data[],
						   size_t len, guchar output[], size_t *outlen)
{
	PurpleDES3Cipher *des3_cipher = PURPLE_DES3_CIPHER(cipher);

	if (des3_cipher->priv->mode == PURPLE_CIPHER_BATCH_MODE_ECB) {
		return purple_des3_cipher_ecb_encrypt(des3_cipher, data, len, output, outlen);
	} else if (des3_cipher->priv->mode == PURPLE_CIPHER_BATCH_MODE_CBC) {
		return purple_des3_cipher_cbc_encrypt(des3_cipher, data, len, output, outlen);
	} else {
		g_return_val_if_reached(0);
	}

	return 0;
}

static gint
purple_des3_cipher_ecb_decrypt(PurpleDES3Cipher *des3_cipher, const guchar data[],
                 size_t len, guchar output[], size_t *outlen)
{
	int offset = 0;
	int i = 0;
	int tmp;
	guint8 buf[8] = {0,0,0,0,0,0,0,0};

	while (offset + 8 <= len) {
		/* NOTE: Apply key in reverse */
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key3),
									data + offset, output + offset, 1);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key2),
									output + offset, buf, 0);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key1),
									buf, output + offset, 1);

		offset += 8;
	}

	*outlen = len;

	if (offset < len) {
		*outlen += len - offset;
		tmp = offset;
		memset(buf, 0, 8);

		while (tmp < len) {
			buf[i++] = data[tmp];
			tmp++;
		}

		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key3),
									buf, output + offset, 1);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key2),
									output + offset, buf, 0);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key1),
									buf, output + offset, 1);
	}

	return 0;
}

static gint
purple_des3_cipher_cbc_decrypt(PurpleDES3Cipher *des3_cipher, const guchar data[],
                 size_t len, guchar output[], size_t *outlen)
{
	int offset = 0;
	int i = 0;
	int tmp;
	guint8 buf[8] = {0,0,0,0,0,0,0,0};
	guint8 link[8];

	memcpy(link, des3_cipher->priv->iv, 8);

	while (offset + 8 <= len) {
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key3),
									data + offset, output + offset, 1);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key2),
									output + offset, buf, 0);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key1),
									buf, output + offset, 1);

		for (i = 0; i < 8; i++)
			output[offset + i] ^= link[i];

		memcpy(link, data + offset, 8);

		offset+=8;
	}

	*outlen = len;

	if(offset<len) {
		*outlen += len - offset;
		tmp = offset;
		memset(buf, 0, 8);
		i = 0;

		while(tmp<len) {
			buf[i++] = data[tmp];
			tmp++;
		}

		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key3),
									buf, output + offset, 1);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key2),
									output + offset, buf, 0);
		purple_des_cipher_ecb_crypt(PURPLE_DES_CIPHER(des3_cipher->priv->key1),
									buf, output + offset, 1);

		for (i = 0; i < 8; i++)
			output[offset + i] ^= link[i];
	}

	return 0;
}

static gint
purple_des3_cipher_decrypt(PurpleCipher *cipher, const guchar data[],
             size_t len, guchar output[], size_t *outlen)
{
	PurpleDES3Cipher *des3_cipher = PURPLE_DES3_CIPHER(cipher);

	if (des3_cipher->priv->mode == PURPLE_CIPHER_BATCH_MODE_ECB) {
		return purple_des3_cipher_ecb_decrypt(des3_cipher, data, len, output, outlen);
	} else if (des3_cipher->priv->mode == PURPLE_CIPHER_BATCH_MODE_CBC) {
		return purple_des3_cipher_cbc_decrypt(des3_cipher, data, len, output, outlen);
	} else {
		g_return_val_if_reached(0);
	}

	return 0;
}

static void
purple_des3_cipher_set_batch_mode(PurpleCipher *cipher, PurpleCipherBatchMode mode)
{
	PurpleDES3Cipher *des3_cipher = PURPLE_DES3_CIPHER(cipher);

	des3_cipher->priv->mode = mode;

	g_object_notify(G_OBJECT(des3_cipher), "batchMode");
}

static PurpleCipherBatchMode
purple_des3_cipher_get_batch_mode(PurpleCipher *cipher)
{
	PurpleDES3Cipher *des3_cipher = PURPLE_DES3_CIPHER(cipher);

	return des3_cipher->priv->mode;
}

static void
purple_des3_cipher_set_iv(PurpleCipher *cipher, guchar *iv, size_t len)
{
	PurpleDES3Cipher *des3_cipher = PURPLE_DES3_CIPHER(cipher);

	g_return_if_fail(len == 8);

	memcpy(des3_cipher->priv->iv, iv, len);

	g_object_notify(G_OBJECT(des3_cipher), "iv");
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_des3_cipher_get_property(GObject *obj, guint param_id, GValue *value,
								GParamSpec *pspec)
{
	PurpleCipher *cipher = PURPLE_CIPHER(obj);

	switch(param_id) {
		case PROP_BATCH_MODE:
			g_value_set_enum(value,
							 purple_cipher_get_batch_mode(cipher));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_des3_cipher_set_property(GObject *obj, guint param_id,
								const GValue *value, GParamSpec *pspec)
{
	PurpleCipher *cipher = PURPLE_CIPHER(obj);
	guchar *iv;

	switch(param_id) {
		case PROP_BATCH_MODE:
			purple_cipher_set_batch_mode(cipher,
										 g_value_get_enum(value));
			break;
		case PROP_IV:
#warning I really dont like this...
			iv = (guchar *)g_value_get_string(value);
			purple_cipher_set_iv(cipher, iv, strlen((gchar*)iv));
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
purple_des3_cipher_finalize(GObject *obj) {
	PurpleDES3Cipher *des3_cipher = PURPLE_DES3_CIPHER(obj);

	g_object_unref(G_OBJECT(des3_cipher->priv->key1));
	g_object_unref(G_OBJECT(des3_cipher->priv->key2));
	g_object_unref(G_OBJECT(des3_cipher->priv->key3));

	memset(des3_cipher->priv->iv, 0, sizeof(des3_cipher->priv->iv));

	g_free(des3_cipher->priv);
}

static void
purple_des3_cipher_class_init(PurpleDES3CipherClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	PurpleCipherClass *cipher_class = PURPLE_CIPHER_CLASS(klass);
	GParamSpec *pspec;

	parent_class = g_type_class_peek_parent(klass);

	obj_class->finalize = purple_des3_cipher_finalize;
	obj_class->get_property = purple_des3_cipher_get_property;
	obj_class->set_property = purple_des3_cipher_set_property;

	cipher_class->set_iv = purple_des3_cipher_set_iv;
	cipher_class->encrypt = purple_des3_cipher_encrypt;
	cipher_class->decrypt = purple_des3_cipher_decrypt;
	cipher_class->set_key = purple_des3_cipher_set_key;
	cipher_class->set_batch_mode = purple_des3_cipher_set_batch_mode;
	cipher_class->get_batch_mode = purple_des3_cipher_get_batch_mode;

	pspec = g_param_spec_enum("batchMode", "batchMode", "batchMode",
							  PURPLE_TYPE_CIPHER_BATCH_MODE, 0,
							  G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_BATCH_MODE, pspec);

	pspec = g_param_spec_string("iv", "iv", "iv", NULL,
								G_PARAM_WRITABLE);
	g_object_class_install_property(obj_class, PROP_IV, pspec);

	pspec = g_param_spec_string("key", "key", "key", NULL,
								G_PARAM_WRITABLE);
	g_object_class_install_property(obj_class, PROP_KEY, pspec);
}

static void
purple_des3_cipher_init(PurpleCipher *cipher) {
	PurpleDES3Cipher *des3_cipher = PURPLE_DES3_CIPHER(cipher);

	des3_cipher->priv = g_new0(PurpleDES3CipherPriv, 1);

	des3_cipher->priv->key1 = purple_des_cipher_new();
	des3_cipher->priv->key2 = purple_des_cipher_new();
	des3_cipher->priv->key3 = purple_des_cipher_new();
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
purple_des3_cipher_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleDES3CipherClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_des3_cipher_class_init,
			NULL,
			NULL,
			sizeof(PurpleDES3Cipher),
			0,
			(GInstanceInitFunc)purple_des3_cipher_init,
			NULL
		};

		type = g_type_register_static(PURPLE_TYPE_CIPHER,
									  "PurpleDES3Cipher",
									  &info, 0);
	}

	return type;
}

PurpleCipher *
purple_des3_cipher_new(void) {
	return g_object_new(PURPLE_TYPE_DES3_CIPHER, NULL);
}

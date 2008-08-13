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
#include "rc4cipher.h"

/*******************************************************************************
 * Structs
 ******************************************************************************/
struct _PurpleRC4CipherPriv {
  guchar state[256];
  guchar x;
  guchar y;
  gint key_len;
};

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_KEY_LEN,
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
static void
purple_rc4_cipher_reset(PurpleCipher *cipher) {
	PurpleRC4Cipher *rc4_cipher = PURPLE_RC4_CIPHER(cipher);
	guint i;

	for(i = 0; i < 256; i++)
		rc4_cipher->priv->state[i] = i;
	rc4_cipher->priv->x = 0;
	rc4_cipher->priv->y = 0;

	/* default is 5 bytes (40bit key) */
	rc4_cipher->priv->key_len = 5;
}

static void
purple_rc4_cipher_set_key (PurpleCipher *cipher, const guchar * key) {
	PurpleRC4Cipher *rc4_cipher = PURPLE_RC4_CIPHER(cipher);
	guchar *state;
	guchar temp_swap;
	guchar x, y;
	guint i;

	x = 0;
	y = 0;
	state = &rc4_cipher->priv->state[0];
	for(i = 0; i < 256; i++)
	{
		y = (key[x] + state[i] + y) % 256;
		temp_swap = state[i];
		state[i] = state[y];
		state[y] = temp_swap;
		x = (x + 1) % rc4_cipher->priv->key_len;
	}

	g_object_notify(G_OBJECT(rc4_cipher), "key");
}


static size_t
purple_rc4_cipher_get_key_size (PurpleCipher *cipher)
{
	PurpleRC4Cipher *rc4_cipher = PURPLE_RC4_CIPHER(cipher);

	return rc4_cipher->priv->key_len;
}


static gint
purple_rc4_cipher_encrypt(PurpleCipher *cipher, const guchar data[], size_t len,
						  guchar output[], size_t *outlen)
{
	PurpleRC4Cipher *rc4_cipher = PURPLE_RC4_CIPHER(cipher);
	guchar temp_swap;
	guchar x, y, z;
	guchar *state;
	guint i;

	x = rc4_cipher->priv->x;
	y = rc4_cipher->priv->y;
	state = &rc4_cipher->priv->state[0];

	for(i = 0; i < len; i++)
	{
		x = (x + 1) % 256;
		y = (state[x] + y) % 256;
		temp_swap = state[x];
		state[x] = state[y];
		state[y] = temp_swap;
		z = state[x] + (state[y]) % 256;
		output[i] = data[i] ^ state[z];
	}
	rc4_cipher->priv->x = x;
	rc4_cipher->priv->y = y;
	if(outlen)
		*outlen = len;

	return 0;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_rc4_cipher_set_property(GObject *obj, guint param_id,
							   const GValue *value, GParamSpec *pspec)
{
	PurpleCipher *cipher = PURPLE_CIPHER(obj);
	PurpleRC4Cipher *rc4_cipher = PURPLE_RC4_CIPHER(obj);

	switch(param_id) {
		case PROP_KEY_LEN:
			purple_rc4_cipher_set_key_len(rc4_cipher, g_value_get_int(value));
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
purple_rc4_cipher_get_property(GObject *obj, guint param_id, GValue *value,
							   GParamSpec *pspec)
{
	PurpleRC4Cipher *rc4_cipher = PURPLE_RC4_CIPHER(obj);

	switch(param_id) {
		case PROP_KEY_LEN:
			g_value_set_int(value,
							purple_rc4_cipher_get_key_len(rc4_cipher));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_rc4_cipher_finalize(GObject *obj) {
	PurpleCipher *cipher = PURPLE_CIPHER(obj);
	PurpleRC4Cipher *rc4_cipher = PURPLE_RC4_CIPHER(obj);

	/* reset the cipher so we don't leave any data around... */
	purple_rc4_cipher_reset(cipher);

	g_free(rc4_cipher->priv);
}

static void
purple_rc4_cipher_class_init(PurpleRC4CipherClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	PurpleCipherClass *cipher_class = PURPLE_CIPHER_CLASS(klass);
	GParamSpec *pspec = NULL;

	parent_class = g_type_class_peek_parent(klass);

	obj_class->set_property = purple_rc4_cipher_set_property;
	obj_class->get_property = purple_rc4_cipher_get_property;
	obj_class->finalize = purple_rc4_cipher_finalize;

	cipher_class->reset = purple_rc4_cipher_reset;
	cipher_class->encrypt = purple_rc4_cipher_encrypt;
	cipher_class->set_key = purple_rc4_cipher_set_key;
	cipher_class->get_key_size = purple_rc4_cipher_get_key_size;

	pspec = g_param_spec_int("key_len", "key_len", "key_len",
							 G_MININT, G_MAXINT, 0,
							 G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_KEY_LEN, pspec);

	pspec = g_param_spec_string("key", "key", "key", NULL,
								G_PARAM_WRITABLE);
	g_object_class_install_property(obj_class, PROP_KEY, pspec);
}

static void
purple_rc4_cipher_init(PurpleCipher *cipher) {
	PurpleRC4Cipher *rc4_cipher = PURPLE_RC4_CIPHER(cipher);

	rc4_cipher->priv = g_new0(PurpleRC4CipherPriv, 1);

	purple_rc4_cipher_reset(cipher);
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
purple_rc4_cipher_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleRC4CipherClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_rc4_cipher_class_init,
			NULL,
			NULL,
			sizeof(PurpleRC4Cipher),
			0,
			(GInstanceInitFunc)purple_rc4_cipher_init,
			NULL,
		};

		type = g_type_register_static(PURPLE_TYPE_CIPHER,
									  "PurpleRC4Cipher",
									  &info, 0);
	}

	return type;
}

PurpleCipher *
purple_rc4_cipher_new(void) {
	return g_object_new(PURPLE_TYPE_RC4_CIPHER, NULL);
}

void
purple_rc4_cipher_set_key_len(PurpleRC4Cipher *rc4_cipher,
							  gint key_len)
{
	g_return_if_fail(PURPLE_IS_RC4_CIPHER(rc4_cipher));

	rc4_cipher->priv->key_len = key_len;

	g_object_notify(G_OBJECT(rc4_cipher), "key_len");
}

gint
purple_rc4_cipher_get_key_len(PurpleRC4Cipher *rc4_cipher)
{
	g_return_val_if_fail(PURPLE_IS_RC4_CIPHER(rc4_cipher), 0);

	return rc4_cipher->priv->key_len;
}

/*
 */
#include "sha1cipher.h"

#include <string.h>

#define SHA1_DIGEST_SIZE		16
#define SHA1_HMAC_BLOCK_SIZE	64
#define SHA1_BLOCK_WORDS		16
#define SHA1_HASH_WORDS		4

/******************************************************************************
 * Structs
 *****************************************************************************/
struct _PurpleSHA1CipherPriv {
	guint32 H[5];
	guint32 W[80];

	gint lenW;

	guint32 sizeHi;
	guint32 sizeLo;
};

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_SIZE_HI,
	PROP_SIZE_LO,
	PROP_LEN_W,
	PROP_LAST,
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Helpers
 *****************************************************************************/
#define SHA1_ROTL(X,n) ((((X) << (n)) | ((X) >> (32-(n)))) & 0xFFFFFFFF)

/******************************************************************************
 * Cipher Stuff
 *****************************************************************************/
static void
purple_sha1_cipher_reset(PurpleCipher *cipher) {
	PurpleSHA1Cipher *sha1_cipher = PURPLE_SHA1_CIPHER(cipher);

	sha1_cipher->priv->lenW = 0;
	sha1_cipher->priv->sizeHi = 0;
	sha1_cipher->priv->sizeLo = 0;

	sha1_cipher->priv->H[0] = 0x67452301;
	sha1_cipher->priv->H[1] = 0xefcdab89;
	sha1_cipher->priv->H[2] = 0x98badcfe;
	sha1_cipher->priv->H[3] = 0x10325476;
	sha1_cipher->priv->H[4] = 0xc3d2e1f0;

	memset(sha1_cipher->priv->W, 0, sizeof(sha1_cipher->priv->W));
}

static void
purple_sha1_cipher_hash_block(PurpleSHA1Cipher *sha1_cipher) {
	gint i;
	guint32 A, B, C, D, E, T;

	for(i = 16; i < 80; i++) {
		sha1_cipher->priv->W[i] =
			SHA1_ROTL(sha1_cipher->priv->W[i -  3] ^
					  sha1_cipher->priv->W[i -  8] ^
					  sha1_cipher->priv->W[i - 14] ^
					  sha1_cipher->priv->W[i - 16], 1);
	}

	A = sha1_cipher->priv->H[0];
	B = sha1_cipher->priv->H[1];
	C = sha1_cipher->priv->H[2];
	D = sha1_cipher->priv->H[3];
	E = sha1_cipher->priv->H[4];

	for(i = 0; i < 20; i++) {
		T = (SHA1_ROTL(A, 5) + (((C ^ D) & B) ^ D) + E +
			sha1_cipher->priv->W[i] + 0x5A827999) & 0xFFFFFFFF;
		E = D;
		D = C;
		C = SHA1_ROTL(B, 30);
		B = A;
		A = T;
	}

	for(i = 20; i < 40; i++) {
		T = (SHA1_ROTL(A, 5) + (B ^ C ^ D) + E + sha1_cipher->priv->W[i] +
			0x6ED9EBA1) & 0xFFFFFFFF;
		E = D;
		D = C;
		C = SHA1_ROTL(B, 30);
		B = A;
		A = T;
	}

	for(i = 40; i < 60; i++) {
		T = (SHA1_ROTL(A, 5) + ((B & C) | (D & (B | C))) + E +
			sha1_cipher->priv->W[i] + 0x8F1BBCDC) & 0xFFFFFFFF;
		E = D;
		D = C;
		C = SHA1_ROTL(B, 30);
		B = A;
		A = T;
	}

	for(i = 60; i < 80; i++) {
		T = (SHA1_ROTL(A, 5) + (B ^ C ^ D) + E + sha1_cipher->priv->W[i] +
			0xCA62C1D6) & 0xFFFFFFFF;
		E = D;
		D = C;
		C = SHA1_ROTL(B, 30);
		B = A;
		A = T;
	}

	sha1_cipher->priv->H[0] += A;
	sha1_cipher->priv->H[1] += B;
	sha1_cipher->priv->H[2] += C;
	sha1_cipher->priv->H[3] += D;
	sha1_cipher->priv->H[4] += E;
}

static void
purple_sha1_cipher_append(PurpleCipher *cipher, const guchar *data,
						 size_t len)
{
	PurpleSHA1Cipher *sha1_cipher = PURPLE_SHA1_CIPHER(cipher);
	gint i;

	for(i = 0; i < len; i++) {
		sha1_cipher->priv->W[sha1_cipher->priv->lenW / 4] <<= 8;
		sha1_cipher->priv->W[sha1_cipher->priv->lenW / 4] |= data[i];

		if((++sha1_cipher->priv->lenW) % 64 == 0) {
			purple_sha1_cipher_hash_block(sha1_cipher);
			sha1_cipher->priv->lenW = 0;
		}

		sha1_cipher->priv->sizeLo += 8;
		sha1_cipher->priv->sizeHi += (sha1_cipher->priv->sizeLo < 8);
	}
}

static gboolean
purple_sha1_cipher_digest(PurpleCipher *cipher, size_t in_len,
						 guchar digest[16], size_t *out_len)
{
	PurpleSHA1Cipher *sha1_cipher = PURPLE_SHA1_CIPHER(cipher);
	guchar pad0x80 = 0x80, pad0x00 = 0x00;
	guchar padlen[8];
	gint i;

	g_return_val_if_fail(in_len >= 20, FALSE);

	padlen[0] = (guchar)((sha1_cipher->priv->sizeHi >> 24) & 255);
	padlen[1] = (guchar)((sha1_cipher->priv->sizeHi >> 16) & 255);
	padlen[2] = (guchar)((sha1_cipher->priv->sizeHi >>  8) & 255);
	padlen[3] = (guchar)((sha1_cipher->priv->sizeHi >>  0) & 255);
	padlen[4] = (guchar)((sha1_cipher->priv->sizeLo >> 24) & 255);
	padlen[5] = (guchar)((sha1_cipher->priv->sizeLo >> 16) & 255);
	padlen[6] = (guchar)((sha1_cipher->priv->sizeLo >>  8) & 255);
	padlen[7] = (guchar)((sha1_cipher->priv->sizeLo >>  0) & 255);

	/* pad with a 1, then zeroes, then length */
	purple_sha1_cipher_append(cipher, &pad0x80, 1);
	while(sha1_cipher->priv->lenW != 56)
		purple_sha1_cipher_append(cipher, &pad0x00, 1);
	purple_sha1_cipher_append(cipher, padlen, 8);

	for(i = 0; i < 20; i++) {
		digest[i] = (guchar)(sha1_cipher->priv->H[i / 4] >> 24);
		sha1_cipher->priv->H[i / 4] <<= 8;
	}

	purple_sha1_cipher_reset(cipher);

	if(out_len)
		*out_len = 20;

	return TRUE;
}

static size_t
purple_sha1_cipher_get_block_size(PurpleCipher *cipher)
{
	return SHA1_HMAC_BLOCK_SIZE;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_sha1_cipher_set_property(GObject *obj, guint param_id,
								const GValue *value, GParamSpec *pspec)
{
	PurpleSHA1Cipher *sha1_cipher = PURPLE_SHA1_CIPHER(obj);

	switch(param_id) {
		case PROP_SIZE_HI:
			purple_sha1_cipher_set_size_hi(sha1_cipher,
										   g_value_get_int(value));
			break;
		case PROP_SIZE_LO:
			purple_sha1_cipher_set_size_lo(sha1_cipher,
										   g_value_get_int(value));
			break;
		case PROP_LEN_W:
			purple_sha1_cipher_set_len_w(sha1_cipher,
										 g_value_get_int(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_sha1_cipher_get_property(GObject *obj, guint param_id, GValue *value,
								GParamSpec *pspec)
{
	PurpleSHA1Cipher *sha1_cipher = PURPLE_SHA1_CIPHER(obj);

	switch(param_id) {
		case PROP_SIZE_HI:
			g_value_set_int(value,
							purple_sha1_cipher_get_size_hi(sha1_cipher));
			break;
		case PROP_SIZE_LO:
			g_value_set_int(value,
							purple_sha1_cipher_get_size_lo(sha1_cipher));
			break;
		case PROP_LEN_W:
			g_value_set_int(value,
							purple_sha1_cipher_get_len_w(sha1_cipher));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_sha1_cipher_finalize(GObject *obj) {
	PurpleCipher *cipher = PURPLE_CIPHER(obj);
	PurpleSHA1Cipher *sha1_cipher = PURPLE_SHA1_CIPHER(obj);

	/* reset the cipher so we don't leave any data around... */
	purple_sha1_cipher_reset(cipher);

	g_free(sha1_cipher->priv);
}

static void
purple_sha1_cipher_class_init(PurpleSHA1CipherClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	PurpleCipherClass *cipher_class = PURPLE_CIPHER_CLASS(klass);
	GParamSpec *pspec = NULL;

	parent_class = g_type_class_peek_parent(klass);

	obj_class->set_property = purple_sha1_cipher_set_property;
	obj_class->get_property = purple_sha1_cipher_get_property;
	obj_class->finalize = purple_sha1_cipher_finalize;

	cipher_class->reset = purple_sha1_cipher_reset;
	cipher_class->append = purple_sha1_cipher_append;
	cipher_class->digest = purple_sha1_cipher_digest;
	cipher_class->get_block_size = purple_sha1_cipher_get_block_size;

	pspec = g_param_spec_int("sizeHi", "sizeHi", "sizeHi",
							 G_MININT, G_MAXINT, 0,
							 G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_SIZE_HI, pspec);

	pspec = g_param_spec_int("sizeLo", "sizeLo", "sizeLo",
							 G_MININT, G_MAXINT, 0,
							 G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_SIZE_LO, pspec);

	pspec = g_param_spec_int("lenW", "lenW", "lenW",
							 G_MININT, G_MAXINT, 0,
							 G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_LEN_W, pspec);
}

static void
purple_sha1_cipher_init(PurpleCipher *cipher) {
	PurpleSHA1Cipher *sha1_cipher = PURPLE_SHA1_CIPHER(cipher);

	sha1_cipher->priv = g_new0(PurpleSHA1CipherPriv, 1);

	purple_sha1_cipher_reset(cipher);
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
purple_sha1_cipher_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleSHA1CipherClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_sha1_cipher_class_init,
			NULL,
			NULL,
			sizeof(PurpleSHA1Cipher),
			0,
			(GInstanceInitFunc)purple_sha1_cipher_init,
			NULL,
		};

		type = g_type_register_static(PURPLE_TYPE_CIPHER,
									  "PurpleSHA1Cipher",
									  &info, 0);
	}

	return type;
}

PurpleCipher *
purple_sha1_cipher_new(void) {
	return g_object_new(PURPLE_TYPE_SHA1_CIPHER, NULL);
}

gint32
purple_sha1_cipher_get_size_hi(PurpleSHA1Cipher *sha1_cipher) {
	g_return_val_if_fail(PURPLE_IS_SHA1_CIPHER(sha1_cipher), 0);

	return sha1_cipher->priv->sizeHi;
}

void
purple_sha1_cipher_set_size_hi(PurpleSHA1Cipher *sha1_cipher,
								 gint32 size_high)
{
	g_return_if_fail(PURPLE_IS_SHA1_CIPHER(sha1_cipher));

	sha1_cipher->priv->sizeHi = size_high;

	g_object_notify(G_OBJECT(sha1_cipher), "sizeHi");
}

gint32
purple_sha1_cipher_get_size_lo(PurpleSHA1Cipher *sha1_cipher) {
	g_return_val_if_fail(PURPLE_IS_SHA1_CIPHER(sha1_cipher), 0);

	return sha1_cipher->priv->sizeLo;
}

void
purple_sha1_cipher_set_size_lo(PurpleSHA1Cipher *sha1_cipher,
								gint32 size_low)
{
	g_return_if_fail(PURPLE_IS_SHA1_CIPHER(sha1_cipher));

	sha1_cipher->priv->sizeLo = size_low;

	g_object_notify(G_OBJECT(sha1_cipher), "sizeLo");
}

gint32
purple_sha1_cipher_get_len_w(PurpleSHA1Cipher *sha1_cipher) {
	g_return_val_if_fail(PURPLE_IS_SHA1_CIPHER(sha1_cipher), 0);

	return sha1_cipher->priv->lenW;
}

void
purple_sha1_cipher_set_len_w(PurpleSHA1Cipher *sha1_cipher, gint32 lenw) {
	g_return_if_fail(PURPLE_IS_SHA1_CIPHER(sha1_cipher));

	sha1_cipher->priv->lenW = lenw;

	g_object_notify(G_OBJECT(sha1_cipher), "lenW");
}


/*
 * Original md5
 * Copyright (C) 2001-2003  Christophe Devine <c.devine@cr0.net>
 */
#include "md5cipher.h"

#include <string.h>

#define MD5_HMAC_BLOCK_SIZE	64

/******************************************************************************
 * Structs
 *****************************************************************************/
struct _PurpleMD5CipherPriv {
	guint32 total[2];
	guint32 state[4];
	guchar buffer[64];
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Helpers
 *****************************************************************************/
#define MD5_GET_GUINT32(n,b,i) {			\
	(n) = ((guint32)(b) [(i)    ]      )	\
		| ((guint32)(b) [(i) + 1] <<  8)	\
		| ((guint32)(b) [(i) + 2] << 16)	\
		| ((guint32)(b) [(i) + 3] << 24);	\
}

#define MD5_PUT_GUINT32(n,b,i) {			\
	(b)[(i)    ] = (guchar)((n)      );		\
	(b)[(i) + 1] = (guchar)((n) >>  8);		\
	(b)[(i) + 2] = (guchar)((n) >> 16);		\
	(b)[(i) + 3] = (guchar)((n) >> 24);		\
}

/******************************************************************************
 * Cipher Stuff
 *****************************************************************************/
static void
purple_md5_cipher_reset(PurpleCipher *cipher) {
	PurpleMD5Cipher *md5_cipher = PURPLE_MD5_CIPHER(cipher);

	md5_cipher->priv->total[0] = 0;
	md5_cipher->priv->total[1] = 0;

	md5_cipher->priv->state[0] = 0x67452301;
	md5_cipher->priv->state[1] = 0xEFCDAB89;
	md5_cipher->priv->state[2] = 0x98BADCFE;
	md5_cipher->priv->state[3] = 0x10325476;

	memset(md5_cipher->priv->buffer, 0, sizeof(md5_cipher->priv->buffer));
}

static void
purple_md5_cipher_process(PurpleMD5Cipher *md5_cipher, const guchar data[64]) {
	guint32 X[16], A, B, C, D;

	A = md5_cipher->priv->state[0];
	B = md5_cipher->priv->state[1];
	C = md5_cipher->priv->state[2];
	D = md5_cipher->priv->state[3];

	MD5_GET_GUINT32(X[ 0], data,  0);
	MD5_GET_GUINT32(X[ 1], data,  4);
	MD5_GET_GUINT32(X[ 2], data,  8);
	MD5_GET_GUINT32(X[ 3], data, 12);
	MD5_GET_GUINT32(X[ 4], data, 16);
	MD5_GET_GUINT32(X[ 5], data, 20);
	MD5_GET_GUINT32(X[ 6], data, 24);
	MD5_GET_GUINT32(X[ 7], data, 28);
	MD5_GET_GUINT32(X[ 8], data, 32);
	MD5_GET_GUINT32(X[ 9], data, 36);
	MD5_GET_GUINT32(X[10], data, 40);
	MD5_GET_GUINT32(X[11], data, 44);
	MD5_GET_GUINT32(X[12], data, 48);
	MD5_GET_GUINT32(X[13], data, 52);
	MD5_GET_GUINT32(X[14], data, 56);
	MD5_GET_GUINT32(X[15], data, 60);

	#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))
	#define P(a,b,c,d,k,s,t) {		\
		a += F(b,c,d) + X[k] + t;	\
		a = S(a,s) + b;				\
	}

	/* first pass */
	#define F(x,y,z) (z ^ (x & (y ^ z)))
	P(A, B, C, D,  0,  7, 0xD76AA478);
	P(D, A, B, C,  1, 12, 0xE8C7B756);
	P(C, D, A, B,  2, 17, 0x242070DB);
	P(B, C, D, A,  3, 22, 0xC1BDCEEE);
	P(A, B, C, D,  4,  7, 0xF57C0FAF);
	P(D, A, B, C,  5, 12, 0x4787C62A);
	P(C, D, A, B,  6, 17, 0xA8304613);
	P(B, C, D, A,  7, 22, 0xFD469501);
	P(A, B, C, D,  8,  7, 0x698098D8);
	P(D, A, B, C,  9, 12, 0x8B44F7AF);
	P(C, D, A, B, 10, 17, 0xFFFF5BB1);
	P(B, C, D, A, 11, 22, 0x895CD7BE);
	P(A, B, C, D, 12,  7, 0x6B901122);
	P(D, A, B, C, 13, 12, 0xFD987193);
	P(C, D, A, B, 14, 17, 0xA679438E);
	P(B, C, D, A, 15, 22, 0x49B40821);
	#undef F

	/* second pass */
	#define F(x,y,z) (y ^ (z & (x ^ y)))
	P(A, B, C, D,  1,  5, 0xF61E2562);
	P(D, A, B, C,  6,  9, 0xC040B340);
	P(C, D, A, B, 11, 14, 0x265E5A51);
	P(B, C, D, A,  0, 20, 0xE9B6C7AA);
	P(A, B, C, D,  5,  5, 0xD62F105D);
	P(D, A, B, C, 10,  9, 0x02441453);
	P(C, D, A, B, 15, 14, 0xD8A1E681);
	P(B, C, D, A,  4, 20, 0xE7D3FBC8);
	P(A, B, C, D,  9,  5, 0x21E1CDE6);
	P(D, A, B, C, 14,  9, 0xC33707D6);
	P(C, D, A, B,  3, 14, 0xF4D50D87);
	P(B, C, D, A,  8, 20, 0x455A14ED);
	P(A, B, C, D, 13,  5, 0xA9E3E905);
	P(D, A, B, C,  2,  9, 0xFCEFA3F8);
	P(C, D, A, B,  7, 14, 0x676F02D9);
	P(B, C, D, A, 12, 20, 0x8D2A4C8A);
	#undef F

	/* third pass */
	#define F(x,y,z) (x ^ y ^ z)
	P(A, B, C, D,  5,  4, 0xFFFA3942);
	P(D, A, B, C,  8, 11, 0x8771F681);
	P(C, D, A, B, 11, 16, 0x6D9D6122);
	P(B, C, D, A, 14, 23, 0xFDE5380C);
	P(A, B, C, D,  1,  4, 0xA4BEEA44);
	P(D, A, B, C,  4, 11, 0x4BDECFA9);
	P(C, D, A, B,  7, 16, 0xF6BB4B60);
	P(B, C, D, A, 10, 23, 0xBEBFBC70);
	P(A, B, C, D, 13,  4, 0x289B7EC6);
	P(D, A, B, C,  0, 11, 0xEAA127FA);
	P(C, D, A, B,  3, 16, 0xD4EF3085);
	P(B, C, D, A,  6, 23, 0x04881D05);
	P(A, B, C, D,  9,  4, 0xD9D4D039);
	P(D, A, B, C, 12, 11, 0xE6DB99E5);
	P(C, D, A, B, 15, 16, 0x1FA27CF8);
	P(B, C, D, A,  2, 23, 0xC4AC5665);
	#undef F

	/* forth pass */
	#define F(x,y,z) (y ^ (x | ~z))
	P(A, B, C, D,  0,  6, 0xF4292244);
	P(D, A, B, C,  7, 10, 0x432AFF97);
	P(C, D, A, B, 14, 15, 0xAB9423A7);
	P(B, C, D, A,  5, 21, 0xFC93A039);
	P(A, B, C, D, 12,  6, 0x655B59C3);
	P(D, A, B, C,  3, 10, 0x8F0CCC92);
	P(C, D, A, B, 10, 15, 0xFFEFF47D);
	P(B, C, D, A,  1, 21, 0x85845DD1);
	P(A, B, C, D,  8,  6, 0x6FA87E4F);
	P(D, A, B, C, 15, 10, 0xFE2CE6E0);
	P(C, D, A, B,  6, 15, 0xA3014314);
	P(B, C, D, A, 13, 21, 0x4E0811A1);
	P(A, B, C, D,  4,  6, 0xF7537E82);
	P(D, A, B, C, 11, 10, 0xBD3AF235);
	P(C, D, A, B,  2, 15, 0x2AD7D2BB);
	P(B, C, D, A,  9, 21, 0xEB86D391);
	#undef F
	#undef P
	#undef S

	md5_cipher->priv->state[0] += A;
	md5_cipher->priv->state[1] += B;
	md5_cipher->priv->state[2] += C;
	md5_cipher->priv->state[3] += D;
}

static void
purple_md5_cipher_append(PurpleCipher *cipher, const guchar *data,
						 size_t len)
{
	PurpleMD5Cipher *md5_cipher = PURPLE_MD5_CIPHER(cipher);
	guint32 left = 0, fill = 0;

	left = md5_cipher->priv->total[0] & 0x3F;
	fill = 64 - left;

	md5_cipher->priv->total[0] += len;
	md5_cipher->priv->total[0] &= 0xFFFFFFFF;

	if(md5_cipher->priv->total[0] < len)
		md5_cipher->priv->total[1]++;

	if(left && len >= fill) {
		memcpy((md5_cipher->priv->buffer + left), data, fill);
		purple_md5_cipher_process(md5_cipher, md5_cipher->priv->buffer);
		len -= fill;
		data += fill;
		left = 0;
	}

	while(len >= 64) {
		purple_md5_cipher_process(md5_cipher, data);
		len -= 64;
		data += 64;
	}

	if(len) {
		memcpy((md5_cipher->priv->buffer + left), data, len);
	}
}

static gboolean
purple_md5_cipher_digest(PurpleCipher *cipher, size_t in_len,
						 guchar digest[16], size_t *out_len)
{
	PurpleMD5Cipher *md5_cipher = PURPLE_MD5_CIPHER(cipher);
	guint32 last, pad;
	guint32 high, low;
	guchar message[8];
	guchar padding[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	g_return_val_if_fail(in_len >= 16, FALSE);

	high = (md5_cipher->priv->total[0] >> 23)
		 | (md5_cipher->priv->total[1] << 3);
	low = (md5_cipher->priv->total[0] << 3);

	MD5_PUT_GUINT32(low, message, 0);
	MD5_PUT_GUINT32(high, message, 4);

	last = md5_cipher->priv->total[0] & 0x3F;
	pad = (last < 56) ? (56 - last) : (120 - last);

	purple_md5_cipher_append(cipher, padding, pad);
	purple_md5_cipher_append(cipher, message, 8);

	MD5_PUT_GUINT32(md5_cipher->priv->state[0], digest, 0);
	MD5_PUT_GUINT32(md5_cipher->priv->state[1], digest, 4);
	MD5_PUT_GUINT32(md5_cipher->priv->state[2], digest, 8);
	MD5_PUT_GUINT32(md5_cipher->priv->state[3], digest, 12);

	if(out_len)
		*out_len = 16;

	return TRUE;
}

static size_t
purple_md5_cipher_get_block_size(PurpleCipher *cipher)
{
	return MD5_HMAC_BLOCK_SIZE;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_md5_cipher_finalize(GObject *obj) {
	PurpleCipher *cipher = PURPLE_CIPHER(obj);
	PurpleMD5Cipher *md5_cipher = PURPLE_MD5_CIPHER(obj);

	/* reset the cipher so we don't leave any data around... */
	purple_md5_cipher_reset(cipher);

	g_free(md5_cipher->priv);
}

static void
purple_md5_cipher_class_init(PurpleMD5CipherClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	PurpleCipherClass *cipher_class = PURPLE_CIPHER_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	obj_class->finalize = purple_md5_cipher_finalize;

	cipher_class->reset = purple_md5_cipher_reset;
	cipher_class->append = purple_md5_cipher_append;
	cipher_class->digest = purple_md5_cipher_digest;
	cipher_class->get_block_size = purple_md5_cipher_get_block_size;
}

static void
purple_md5_cipher_init(PurpleCipher *cipher) {
	PurpleMD5Cipher *md5_cipher = PURPLE_MD5_CIPHER(cipher);

	md5_cipher->priv = g_new0(PurpleMD5CipherPriv, 1);

	purple_md5_cipher_reset(cipher);
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
purple_md5_cipher_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleMD5CipherClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_md5_cipher_class_init,
			NULL,
			NULL,
			sizeof(PurpleMD5Cipher),
			0,
			(GInstanceInitFunc)purple_md5_cipher_init,
			NULL,
		};

		type = g_type_register_static(PURPLE_TYPE_CIPHER,
									  "PurpleMD5Cipher",
									  &info, 0);
	}

	return type;
}

PurpleCipher *
purple_md5_cipher_new(void) {
	return g_object_new(PURPLE_TYPE_MD5_CIPHER, NULL);
}

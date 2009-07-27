/*
 */
#include "sha256cipher.h"

#include <string.h>

#define SHA256_DIGEST_SIZE		16
#define SHA256_HMAC_BLOCK_SIZE	64
#define SHA256_BLOCK_WORDS		16
#define SHA256_HASH_WORDS		4


/******************************************************************************
 * Structs
 *****************************************************************************/
#if !GLIB_CHECK_VERSION(2,16,0)
struct _PurpleSHA256CipherPriv {
	guint32 H[8];
	guint32 W[64];

	gint lenW;

	guint32 sizeHi;
	guint32 sizeLo;
};
#endif
/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_LAST,
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Helpers
 *****************************************************************************/
#define SHA256_ROTR(X,n) ((((X) >> (n)) | ((X) << (32-(n)))) & 0xFFFFFFFF)

/******************************************************************************
 * Cipher Stuff
 *****************************************************************************/

#if !GLIB_CHECK_VERSION(2,16,0)
static const guint32 sha256_K[64] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

struct SHA256Context {
	guint32 H[8];
	guint32 W[64];

	gint lenW;

	guint32 sizeHi;
	guint32 sizeLo;
};

static void
purple_sha256_cipher_hash_block(PurpleSHA256Cipher *sha256_cipher)
{
	gint i;
	guint32 A, B, C, D, E, F, G, H, T1, T2;

	for(i = 16; i < 64; i++) {
		sha256_cipher->priv->W[i] =
			  (SHA256_ROTR(sha256_cipher->priv->W[i-2], 17) ^ SHA256_ROTR(sha256_cipher->priv->W[i-2],  19) ^ (sha256_cipher->priv->W[i-2] >> 10))
			+ sha256_cipher->priv->W[i-7]
			+ (SHA256_ROTR(sha256_cipher->priv->W[i-15], 7) ^ SHA256_ROTR(sha256_cipher->priv->W[i-15], 18) ^ (sha256_cipher->priv->W[i-15] >> 3))
			+ sha256_cipher->priv->W[i-16];
	}

	A = sha256_cipher->priv->H[0];
	B = sha256_cipher->priv->H[1];
	C = sha256_cipher->priv->H[2];
	D = sha256_cipher->priv->H[3];
	E = sha256_cipher->priv->H[4];
	F = sha256_cipher->priv->H[5];
	G = sha256_cipher->priv->H[6];
	H = sha256_cipher->priv->H[7];

	for(i = 0; i < 64; i++) {
        T1 = H
			+ (SHA256_ROTR(E, 6) ^ SHA256_ROTR(E, 11) ^ SHA256_ROTR(E, 25))
			+ ((E & F) ^ ((~E) & G))
			+ sha256_K[i] + sha256_cipher->priv->W[i];
        T2 = (SHA256_ROTR(A, 2) ^ SHA256_ROTR(A, 13) ^ SHA256_ROTR(A, 22))
			+ ((A & B) ^ (A & C) ^ (B & C));
		H = G;
		G = F;
		F = E;
		E = D + T1;
		D = C;
		C = B;
		B = A;
		A = T1 + T2;
	}

	sha256_cipher->priv->H[0] += A;
	sha256_cipher->priv->H[1] += B;
	sha256_cipher->priv->H[2] += C;
	sha256_cipher->priv->H[3] += D;
	sha256_cipher->priv->H[4] += E;
	sha256_cipher->priv->H[5] += F;
	sha256_cipher->priv->H[6] += G;
	sha256_cipher->priv->H[7] += H;
}

static void
purple_sha256_cipher_reset(PurpleCipher *cipher)
{
	PurpleSHA256Cipher *sha256_cipher = PURPLE_SHA256_CIPHER(cipher);

	sha256_cipher->priv->lenW = 0;
	sha256_cipher->priv->sizeHi = 0;
	sha256_cipher->priv->sizeLo = 0;

	sha256_cipher->priv->H[0] = 0x6a09e667;
	sha256_cipher->priv->H[1] = 0xbb67ae85;
	sha256_cipher->priv->H[2] = 0x3c6ef372;
	sha256_cipher->priv->H[3] = 0xa54ff53a;
	sha256_cipher->priv->H[4] = 0x510e527f;
	sha256_cipher->priv->H[5] = 0x9b05688c;
	sha256_cipher->priv->H[6] = 0x1f83d9ab;
	sha256_cipher->priv->H[7] = 0x5be0cd19;

	memset(sha256_cipher->priv->W, 0, sizeof(sha256_cipher->priv->W));
}

static void
purple_sha256_cipher_append(PurpleCipher *cipher, const guchar *data,
                            size_t len)
{
	PurpleSHA256Cipher *sha256_cipher = PURPLE_SHA256_CIPHER(cipher);
	gint i;

	for(i = 0; i < len; i++) {
		sha256_cipher->priv->W[sha256_cipher->priv->lenW / 4] <<= 8;
		sha256_cipher->priv->W[sha256_cipher->priv->lenW / 4] |= data[i];

		if((++sha256_cipher->priv->lenW) % 64 == 0) {
			purple_sha256_cipher_hash_block(sha256_cipher);
			sha256_cipher->priv->lenW = 0;
		}

		sha256_cipher->priv->sizeLo += 8;
		sha256_cipher->priv->sizeHi += (sha256_cipher->priv->sizeLo < 8);
	}
}

static gboolean
purple_sha256_cipher_digest(PurpleCipher *cipher, size_t in_len,
                            guchar digest[32], size_t *out_len)
{
	PurpleSHA256Cipher *sha256_cipher = PURPLE_SHA256_CIPHER(cipher);
	guchar pad0x80 = 0x80, pad0x00 = 0x00;
	guchar padlen[8];
	gint i;

	g_return_val_if_fail(in_len >= 32, FALSE);

	padlen[0] = (guchar)((sha256_cipher->priv->sizeHi >> 24) & 255);
	padlen[1] = (guchar)((sha256_cipher->priv->sizeHi >> 16) & 255);
	padlen[2] = (guchar)((sha256_cipher->priv->sizeHi >> 8) & 255);
	padlen[3] = (guchar)((sha256_cipher->priv->sizeHi >> 0) & 255);
	padlen[4] = (guchar)((sha256_cipher->priv->sizeLo >> 24) & 255);
	padlen[5] = (guchar)((sha256_cipher->priv->sizeLo >> 16) & 255);
	padlen[6] = (guchar)((sha256_cipher->priv->sizeLo >> 8) & 255);
	padlen[7] = (guchar)((sha256_cipher->priv->sizeLo >> 0) & 255);

	/* pad with a 1, then zeroes, then length */
	purple_sha256_cipher_append(cipher, &pad0x80, 1);
	while(sha256_cipher->priv->lenW != 56)
		purple_sha256_cipher_append(cipher, &pad0x00, 1);
	purple_sha256_cipher_append(cipher, padlen, 8);

	for(i = 0; i < 32; i++) {
		digest[i] = (guchar)(sha256_cipher->priv->H[i / 4] >> 24);
		sha256_cipher->priv->H[i / 4] <<= 8;
	}

	purple_sha256_cipher_reset(cipher);

	if(out_len)
		*out_len = 32;

	return TRUE;
}
#endif /* !GLIB_CHECK_VERSION */

static size_t
purple_sha256_cipher_get_block_size(PurpleCipher *cipher)
{
	return SHA256_HMAC_BLOCK_SIZE;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
#if !GLIB_CHECK_VERSION(2,16,0)
static void
purple_sha256_cipher_finalize(GObject *obj)
{
	PurpleCipher *cipher = PURPLE_CIPHER(obj);
	PurpleSHA256Cipher *sha256_cipher = PURPLE_SHA256_CIPHER(obj);

	/* reset the cipher so we don't leave any data around... */
	purple_sha256_cipher_reset(cipher);

	g_free(sha256_cipher->priv);
}
#endif /* !GLIB_CHECK_VERSION(2,16,0) */

static void
purple_sha256_cipher_class_init(PurpleSHA256CipherClass *klass) {
#if !GLIB_CHECK_VERSION(2,16,0)
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
#endif
	PurpleCipherClass *cipher_class = PURPLE_CIPHER_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

#if !GLIB_CHECK_VERSION(2,16,0)
	obj_class->finalize = purple_sha256_cipher_finalize;

	cipher_class->reset = purple_sha256_cipher_reset;
	cipher_class->append = purple_sha256_cipher_append;
	cipher_class->digest = purple_sha256_cipher_digest;
#endif
	cipher_class->get_block_size = purple_sha256_cipher_get_block_size;
}

static void
purple_sha256_cipher_init(PurpleCipher *cipher) {
#if !GLIB_CHECK_VERSION(2,16,0)
	PurpleSHA256Cipher *sha256_cipher = PURPLE_SHA256_CIPHER(cipher);

	sha256_cipher->priv = g_new0(PurpleSHA256CipherPriv, 1);

	purple_sha256_cipher_reset(cipher);
#endif
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
purple_sha256_cipher_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleSHA256CipherClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_sha256_cipher_class_init,
			NULL,
			NULL,
			sizeof(PurpleSHA256Cipher),
			0,
			(GInstanceInitFunc)purple_sha256_cipher_init,
			NULL,
		};

		GType parent = G_TYPE_INVALID;

#if GLIB_CHECK_VERSION(2,16,0)
		parent = PURPLE_TYPE_G_HASH;
#else
		parent = PURPLE_TYPE_CIPHER;
#endif

		type = g_type_register_static(parent,
									  "PurpleSHA256Cipher",
									  &info, 0);
	}

	return type;
}

PurpleCipher *
purple_sha256_cipher_new(void) {
#if GLIB_CHECK_VERSION(2,16,0)
	return g_object_new(PURPLE_TYPE_SHA256_CIPHER,
			"checksum_type", G_CHECKSUM_SHA256,
			NULL);
#else
	return g_object_new(PURPLE_TYPE_SHA256_CIPHER, NULL);
#endif
}


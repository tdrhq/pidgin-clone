/*
 * Original md4 taken from linux kernel
 * MD4 Message Digest Algorithm (RFC1320).
 *
 * Implementation derived from Andrew Tridgell and Steve French's
 * CIFS MD4 implementation, and the cryptoapi implementation
 * originally based on the public domain implementation written
 * by Colin Plumb in 1993.
 *
 * Copyright (c) Andrew Tridgell 1997-1998.
 * Modified by Steve French (sfrench@us.ibm.com) 2002
 * Copyright (c) Cryptoapi developers.
 * Copyright (c) 2002 David S. Miller (davem@redhat.com)
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 */
#include "md4cipher.h"

#include <string.h>

#define MD4_DIGEST_SIZE		16
#define MD4_HMAC_BLOCK_SIZE	64
#define MD4_BLOCK_WORDS		16
#define MD4_HASH_WORDS		4

/******************************************************************************
 * Structs
 *****************************************************************************/
struct _PurpleMD4CipherPriv {
	guint32 hash[MD4_HASH_WORDS];
	guint32 block[MD4_BLOCK_WORDS];
	guint64 byte_count;
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Helpers
 *****************************************************************************/
#define ROUND1(a,b,c,d,k,s) \
	(a = lshift(a + F(b,c,d) + k, s))

#define ROUND2(a,b,c,d,k,s) \
	(a = lshift(a + G(b,c,d) + k + (guint32)0x5a827999,s))

#define ROUND3(a,b,c,d,k,s) \
	(a = lshift(a + H(b,c,d) + k + (guint32)0x6ed9eba1,s))

static inline guint32
lshift(guint32 x, unsigned int s) {
	x &= 0xffffffff;
	return (((x << s) & 0xffffffff) | (x >> (32 - s)));
}

static inline guint32
F(guint32 x, guint32 y, guint32 z) {
	return ((x & y) | ((~x) & z));
}

static inline guint32
G(guint32 x, guint32 y, guint32 z) {
	return ((x & y) | (x & z) | (y & z));
}

static inline guint32
H(guint32 x, guint32 y, guint32 z) {
	return (x ^ y ^ z);
}

static inline void
le32_to_cpu_array(guint32 *buf, unsigned int words) {
	while(words--) {
		*buf = GUINT_FROM_LE(*buf);
		buf++;
	}
}

static inline void
cpu_to_le32_array(guint32 *buf, unsigned int words) {
	while(words--) {
		*buf = GUINT_TO_LE(*buf);
		buf++;
	}
}

/******************************************************************************
 * Cipher Stuff
 *****************************************************************************/
static void
purple_md4_cipher_reset(PurpleCipher *cipher) {
	PurpleMD4Cipher *md4_cipher = PURPLE_MD4_CIPHER(cipher);

	md4_cipher->priv->hash[0] = 0x67452301;
	md4_cipher->priv->hash[1] = 0xefcdab89;
	md4_cipher->priv->hash[2] = 0x98badcfe;
	md4_cipher->priv->hash[3] = 0x10325476;

	md4_cipher->priv->byte_count = 0;

	memset(md4_cipher->priv->block, 0, sizeof(md4_cipher->priv->block));
}

static void
md4_transform(guint32 *hash, guint32 const *in) {
	guint32 a, b, c, d;

	a = hash[0];
	b = hash[1];
	c = hash[2];
	d = hash[3];

	ROUND1(a, b, c, d, in[0], 3);
	ROUND1(d, a, b, c, in[1], 7);
	ROUND1(c, d, a, b, in[2], 11);
	ROUND1(b, c, d, a, in[3], 19);
	ROUND1(a, b, c, d, in[4], 3);
	ROUND1(d, a, b, c, in[5], 7);
	ROUND1(c, d, a, b, in[6], 11);
	ROUND1(b, c, d, a, in[7], 19);
	ROUND1(a, b, c, d, in[8], 3);
	ROUND1(d, a, b, c, in[9], 7);
	ROUND1(c, d, a, b, in[10], 11);
	ROUND1(b, c, d, a, in[11], 19);
	ROUND1(a, b, c, d, in[12], 3);
	ROUND1(d, a, b, c, in[13], 7);
	ROUND1(c, d, a, b, in[14], 11);
	ROUND1(b, c, d, a, in[15], 19);

	ROUND2(a, b, c, d,in[ 0], 3);
	ROUND2(d, a, b, c, in[4], 5);
	ROUND2(c, d, a, b, in[8], 9);
	ROUND2(b, c, d, a, in[12], 13);
	ROUND2(a, b, c, d, in[1], 3);
	ROUND2(d, a, b, c, in[5], 5);
	ROUND2(c, d, a, b, in[9], 9);
	ROUND2(b, c, d, a, in[13], 13);
	ROUND2(a, b, c, d, in[2], 3);
	ROUND2(d, a, b, c, in[6], 5);
	ROUND2(c, d, a, b, in[10], 9);
	ROUND2(b, c, d, a, in[14], 13);
	ROUND2(a, b, c, d, in[3], 3);
	ROUND2(d, a, b, c, in[7], 5);
	ROUND2(c, d, a, b, in[11], 9);
	ROUND2(b, c, d, a, in[15], 13);

	ROUND3(a, b, c, d,in[ 0], 3);
	ROUND3(d, a, b, c, in[8], 9);
	ROUND3(c, d, a, b, in[4], 11);
	ROUND3(b, c, d, a, in[12], 15);
	ROUND3(a, b, c, d, in[2], 3);
	ROUND3(d, a, b, c, in[10], 9);
	ROUND3(c, d, a, b, in[6], 11);
	ROUND3(b, c, d, a, in[14], 15);
	ROUND3(a, b, c, d, in[1], 3);
	ROUND3(d, a, b, c, in[9], 9);
	ROUND3(c, d, a, b, in[5], 11);
	ROUND3(b, c, d, a, in[13], 15);
	ROUND3(a, b, c, d, in[3], 3);
	ROUND3(d, a, b, c, in[11], 9);
	ROUND3(c, d, a, b, in[7], 11);
	ROUND3(b, c, d, a, in[15], 15);

	hash[0] += a;
	hash[1] += b;
	hash[2] += c;
	hash[3] += d;
}

static inline void
md4_transform_helper(PurpleMD4Cipher *md4_cipher) {
	le32_to_cpu_array(md4_cipher->priv->block,
					  sizeof(md4_cipher->priv->block) / sizeof(guint32));
	md4_transform(md4_cipher->priv->hash, md4_cipher->priv->block);
}

static void
purple_md4_cipher_append(PurpleCipher *cipher, const guchar *data,
						 size_t len)
{
	PurpleMD4Cipher *md4_cipher = PURPLE_MD4_CIPHER(cipher);
	const guint32 avail = sizeof(md4_cipher->priv->block) -
						  (md4_cipher->priv->byte_count & 0x3f);

	md4_cipher->priv->byte_count += len;

	if(avail > len) {
		memcpy((char *)md4_cipher->priv->block +
			   (sizeof(md4_cipher->priv->block) - avail),
			   data, len);
		return;
	}

	memcpy((char *)md4_cipher->priv->block +
		   (sizeof(md4_cipher->priv->block) - avail),
		   data, avail);

	md4_transform_helper(md4_cipher);
	data += avail;
	len -= avail;

	while(len >= sizeof(md4_cipher->priv->block)) {
		memcpy(md4_cipher->priv->block, data, sizeof(md4_cipher->priv->block));
		md4_transform_helper(md4_cipher);
		data += sizeof(md4_cipher->priv->block);
		len -= sizeof(md4_cipher->priv->block);
	}

	memcpy(md4_cipher->priv->block, data, len);
}

static gboolean
purple_md4_cipher_digest(PurpleCipher *cipher, size_t in_len,
						 guchar digest[16], size_t *out_len)
{
	PurpleMD4Cipher *md4_cipher = PURPLE_MD4_CIPHER(cipher);
	const unsigned int offset = md4_cipher->priv->byte_count & 0x3f;
	gchar *p = (gchar *)md4_cipher->priv->block + offset;
	gint padding = 56 - (offset + 1);

	if(in_len < 16)
		return FALSE;

	if(out_len)
		*out_len = 16;

	*p++ = 0x80;

	if(padding < 0) {
		memset(p, 0x00, padding + sizeof(guint64));
		md4_transform_helper(md4_cipher);
		p = (gchar *)md4_cipher->priv->block;
		padding = 56;
	}

	memset(p, 0, padding);
	md4_cipher->priv->block[14] = md4_cipher->priv->byte_count << 3;
	md4_cipher->priv->block[15] = md4_cipher->priv->byte_count >> 29;
	le32_to_cpu_array(md4_cipher->priv->block,
					  (sizeof(md4_cipher->priv->block) - sizeof(guint64)) /
					  sizeof(guint32));
	md4_transform(md4_cipher->priv->hash, md4_cipher->priv->block);
	cpu_to_le32_array(md4_cipher->priv->hash,
					  sizeof(md4_cipher->priv->hash) / sizeof(guint32));
	memcpy(digest, md4_cipher->priv->hash, sizeof(md4_cipher->priv->hash));

	return TRUE;
}

static size_t
purple_md4_cipher_get_block_size(PurpleCipher *cipher)
{
	return MD4_HMAC_BLOCK_SIZE;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_md4_cipher_finalize(GObject *obj) {
	PurpleCipher *cipher = PURPLE_CIPHER(obj);
	PurpleMD4Cipher *md4_cipher = PURPLE_MD4_CIPHER(obj);

	/* reset the cipher so we don't leave any data around... */
	purple_md4_cipher_reset(cipher);

	g_free(md4_cipher->priv);
}

static void
purple_md4_cipher_class_init(PurpleMD4CipherClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	PurpleCipherClass *cipher_class = PURPLE_CIPHER_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	obj_class->finalize = purple_md4_cipher_finalize;

	cipher_class->reset = purple_md4_cipher_reset;
	cipher_class->append = purple_md4_cipher_append;
	cipher_class->digest = purple_md4_cipher_digest;
	cipher_class->get_block_size = purple_md4_cipher_get_block_size;
}

static void
purple_md4_cipher_init(PurpleCipher *cipher) {
	PurpleMD4Cipher *md4_cipher = PURPLE_MD4_CIPHER(cipher);

	md4_cipher->priv = g_new0(PurpleMD4CipherPriv, 1);

	purple_md4_cipher_reset(cipher);
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
purple_md4_cipher_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleMD4CipherClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_md4_cipher_class_init,
			NULL,
			NULL,
			sizeof(PurpleMD4Cipher),
			0,
			(GInstanceInitFunc)purple_md4_cipher_init,
			NULL,
		};

		type = g_type_register_static(PURPLE_TYPE_CIPHER,
									  "PurpleMD4Cipher",
									  &info, 0);
	}

	return type;
}

PurpleCipher *
purple_md4_cipher_new(void) {
	return g_object_new(PURPLE_TYPE_MD4_CIPHER, NULL);
}

#include "module.h"

MODULE = Purple::Cipher  PACKAGE = Purple::Cipher  PREFIX = purple_cipher_
PROTOTYPES: ENABLE

void
purple_cipher_reset(cipher)
	Purple::Cipher cipher

void
purple_cipher_set_iv(cipher, iv, len)
	Purple::Cipher cipher
	guchar * iv
	size_t len

void
purple_cipher_append(cipher, data, len)
	Purple::Cipher cipher
	const guchar * data
	size_t len

gboolean
purple_cipher_digest(cipher, in_len, digest, out_len)
	Purple::Cipher cipher
	size_t in_len
	guchar &digest
	size_t * out_len

gboolean
purple_cipher_digest_to_str(cipher, in_len, digest_s, out_len)
	Purple::Cipher cipher
	size_t in_len
	gchar &digest_s
	size_t * out_len

gint
purple_cipher_encrypt(cipher, data, len, output, outlen)
	Purple::Cipher cipher
	guchar &data
	size_t len
	guchar &output
	size_t * outlen

gint
purple_cipher_decrypt(cipher, data, len, output, outlen)
	Purple::Cipher cipher
	guchar &data
	size_t len
	guchar &output
	size_t * outlen

void
purple_cipher_set_salt(cipher, salt)
	Purple::Cipher cipher
	guchar * salt

size_t
purple_cipher_get_salt_size(cipher);
	Purple::Cipher cipher

void
purple_cipher_set_key(cipher, key)
	Purple::Cipher cipher
	const guchar * key

size_t
purple_cipher_get_key_size(cipher)
	Purple::Cipher cipher

void
purple_cipher_set_batch_mode(cipher, mode)
	Purple::Cipher cipher
	Purple::Cipher::BatchMode mode

Purple::Cipher::BatchMode
purple_cipher_get_batch_mode(cipher)
	Purple::Cipher cipher

size_t
purple_cipher_get_block_size(cipher)
	Purple::Cipher cipher

void
purple_cipher_set_key_with_len(cipher, key, len)
	Purple::Cipher cipher
	const guchar * key
	size_t len

void
purple_cipher_set_hash(cipher, hash);
	Purple::Cipher cipher
	Purple::Cipher hash

Purple::Cipher
purple_cipher_get_hash(cipher)
	Purple::Cipher cipher

MODULE = Purple::Cipher  PACKAGE = Purple::DESCipher  PREFIX = purple_des_cipher_
PROTOTYPES: ENABLE

Purple::Cipher
purple_des_cipher_new()

MODULE = Purple::Cipher PACKAGE = Purple::MD4Cipher PREFIX = purple_md4_cipher_
PROTOTYPES: ENABLE

Purple::Cipher
purple_md4_cipher_new()

MODULE = Purple::Cipher PACKAGE = Purple::MD5Cipher PREFIX = purple_md5_cipher_
PROTOTYPES: ENABLE

Purple::Cipher
purple_md5_cipher_new()

MODULE = Purple::Cipher PACKAGE = Purple::SHA1Cipher PREFIX = purple_sha1_cipher_
PROTOTYPES: ENABLE

Purple::Cipher
purple_sha1_cipher_new()

MODULE = Purple::Cipher PACKAGE = Purple::HMACCipher PREFIX = purple_hmac_cipher_
PROTOTYPES: ENABLE

Purple::Cipher
purple_hmac_cipher_new()

MODULE = Purple::Cipher PACKAGE = Purple::RC4Cipher PREFIX = purple_rc4_cipher_
PROTOTYPES: ENABLE

Purple::Cipher
purple_rc4_cipher_new()

MODULE = Purple::Cipher PACKAGE = Purple::DES3Cipher PREFIX = purple_des3_cipher_
PROTOTYPES: ENABLE

Purple::Cipher
purple_des3_cipher_new()

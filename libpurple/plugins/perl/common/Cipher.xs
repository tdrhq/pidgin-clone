#include "module.h"

MODULE = Purple::Cipher  PACKAGE = Purple::Cipher  PREFIX = purple_cipher_
PROTOTYPES: ENABLE

BOOT:
{
	HV *stash = gv_stashpv("Purple::Cipher::BatchMode", 1);
	HV *cipher_caps = gv_stashpv("Purple::Cipher::Caps", 1);

	static const constiv *civ, const_iv[] = {
#define const_iv(name) {#name, (IV)PURPLE_CIPHER_BATCH_MODE_##name}
		const_iv(ECB),
		const_iv(CBC),
#undef const_iv
	};

	static const constiv bm_const_iv[] = {
#define const_iv(name) {#name, (IV)PURPLE_CIPHER_CAPS_##name}
		const_iv(SET_OPT),
		const_iv(GET_OPT),
		const_iv(INIT),
		const_iv(RESET),
		const_iv(UNINIT),
		const_iv(SET_IV),
		const_iv(APPEND),
		const_iv(DIGEST),
		const_iv(ENCRYPT),
		const_iv(DECRYPT),
		const_iv(SET_SALT),
		const_iv(GET_SALT_SIZE),
		const_iv(SET_KEY),
		const_iv(GET_KEY_SIZE),
		const_iv(SET_BATCH_MODE),
		const_iv(GET_BATCH_MODE),
		const_iv(GET_BLOCK_SIZE),
		const_iv(SET_KEY_WITH_LEN),
		const_iv(UNKNOWN),
#undef const_iv
	};

	for (civ = const_iv + sizeof(const_iv) / sizeof(const_iv[0]); civ-- > const_iv; )
		newCONSTSUB(stash, (char *)civ->name, newSViv(civ->iv));

	for (civ = bm_const_iv + sizeof(bm_const_iv) / sizeof(bm_const_iv[0]); civ-- > bm_const_iv; )
		newCONSTSUB(cipher_caps, (char *)civ->name, newSViv(civ->iv));
}

size_t
purple_cipher_digest_region(name, data_sv, in_len, digest)
	const gchar *name
	SV *data_sv
	size_t in_len
	SV *digest
	PREINIT:
		gboolean ret;
		guchar *buff = NULL;
		guchar *data = NULL;
		size_t data_len;
	CODE:
		data = SvPV(data_sv, data_len);
		SvUPGRADE(digest, SVt_PV);
		buff = SvGROW(digest, in_len);
		ret = purple_cipher_digest_region(name, data, data_len, in_len, buff, &RETVAL);
		if(!ret) {
			SvSetSV_nosteal(digest, &PL_sv_undef);
			XSRETURN_UNDEF;
		}
		SvCUR_set(digest, RETVAL);
		SvPOK_only(digest);
	OUTPUT:
		RETVAL

MODULE = Purple::Cipher  PACKAGE = Purple::Cipher  PREFIX = purple_cipher_
PROTOTYPES: ENABLE

void
purple_cipher_reset(cipher)
	Purple::Cipher cipher

void
purple_cipher_set_iv(Purple::Cipher cipher, guchar *iv, size_t length(iv))
	PROTOTYPE: $$

void
purple_cipher_append(Purple::Cipher cipher, guchar *data, size_t length(data))
	PROTOTYPE: $$

size_t
purple_cipher_digest(cipher, in_len, digest)
	Purple::Cipher cipher
	size_t in_len
	SV *digest
	PREINIT:
		gboolean ret;
		guchar *buff = NULL;
	CODE:
		SvUPGRADE(digest, SVt_PV);
		buff = SvGROW(digest, in_len);
		ret = purple_cipher_digest(cipher, in_len, buff, &RETVAL);
		if(!ret) {
			SvSetSV_nosteal(digest, &PL_sv_undef);
			XSRETURN_UNDEF;
		}
		SvCUR_set(digest, RETVAL);
		SvPOK_only(digest);
	OUTPUT:
		RETVAL

size_t
purple_cipher_digest_to_str(cipher, in_len, digest_s)
	Purple::Cipher cipher
	size_t in_len
	SV *digest_s
	PREINIT:
		gboolean ret;
		gchar *buff = NULL;
	CODE:
		in_len += 1; /* perl shouldn't need to care about '\0' at the end */
		SvUPGRADE(digest_s, SVt_PV);
		buff = SvGROW(digest_s, in_len);
		ret = purple_cipher_digest_to_str(cipher, in_len, buff, &RETVAL);
		if(!ret) {
			SvSetSV_nosteal(digest_s, &PL_sv_undef);
			XSRETURN_UNDEF;
		}
		SvCUR_set(digest_s, RETVAL);
		SvPOK_only(digest_s);
	OUTPUT:
		RETVAL

gint
purple_cipher_encrypt(cipher, data_sv, output, OUTLIST size_t outlen)
	Purple::Cipher cipher
	SV *data_sv
	SV *output
	PROTOTYPE: $$$
	PREINIT:
		size_t datalen;
		guchar *buff = NULL;
		guchar *data = NULL;
	CODE:
		data = SvPV(data_sv, datalen);
		SvUPGRADE(output, SVt_PV);
		buff = SvGROW(output, datalen);
		RETVAL = purple_cipher_encrypt(cipher, data, datalen, buff, &outlen);
		if(outlen != 0) {
			SvPOK_only(output);
			SvCUR_set(output, outlen);
		} else {
			SvSetSV_nosteal(output, &PL_sv_undef);
		}
	OUTPUT:
		RETVAL

gint
purple_cipher_decrypt(cipher, data_sv, output, OUTLIST size_t outlen)
	Purple::Cipher cipher
	SV *data_sv
	SV *output
	PROTOTYPE: $$$
	PREINIT:
		size_t datalen;
		guchar *buff = NULL;
		guchar *data = NULL;
	CODE:
		data = SvPV(data_sv, datalen);
		SvUPGRADE(output, SVt_PV);
		buff = SvGROW(output, datalen);
		RETVAL = purple_cipher_decrypt(cipher, data, datalen, buff, &outlen);
		if(outlen != 0) {
			SvPOK_only(output);
			SvCUR_set(output, outlen);
		} else {
			SvSetSV_nosteal(output, &PL_sv_undef);
		}
	OUTPUT:
		RETVAL

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

Purple::Cipher::BatchMode
purple_cipher_get_batch_mode(cipher)
	Purple::Cipher cipher

size_t
purple_cipher_get_block_size(cipher)
	Purple::Cipher cipher

void
purple_cipher_set_batch_mode(cipher, mode)
	Purple::Cipher cipher
	Purple::Cipher::BatchMode mode

void
purple_cipher_set_key_with_len(Purple::Cipher cipher, guchar *key, size_t length(key))
	PROTOTYPE: $$

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

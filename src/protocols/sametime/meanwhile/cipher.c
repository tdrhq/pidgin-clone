
/*
  Meanwhile - Unofficial Lotus Sametime Community Client Library
  Copyright (C) 2004  Christopher (siege) O'Brien
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
  
  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mw_channel.h"
#include "mw_cipher.h"
#include "mw_debug.h"
#include "mw_session.h"


/** From RFC2268 */
static unsigned char PT[] = {
  0xD9, 0x78, 0xF9, 0xC4, 0x19, 0xDD, 0xB5, 0xED,
  0x28, 0xE9, 0xFD, 0x79, 0x4A, 0xA0, 0xD8, 0x9D,
  0xC6, 0x7E, 0x37, 0x83, 0x2B, 0x76, 0x53, 0x8E,
  0x62, 0x4C, 0x64, 0x88, 0x44, 0x8B, 0xFB, 0xA2,
  0x17, 0x9A, 0x59, 0xF5, 0x87, 0xB3, 0x4F, 0x13,
  0x61, 0x45, 0x6D, 0x8D, 0x09, 0x81, 0x7D, 0x32,
  0xBD, 0x8F, 0x40, 0xEB, 0x86, 0xB7, 0x7B, 0x0B,
  0xF0, 0x95, 0x21, 0x22, 0x5C, 0x6B, 0x4E, 0x82,
  0x54, 0xD6, 0x65, 0x93, 0xCE, 0x60, 0xB2, 0x1C,
  0x73, 0x56, 0xC0, 0x14, 0xA7, 0x8C, 0xF1, 0xDC,
  0x12, 0x75, 0xCA, 0x1F, 0x3B, 0xBE, 0xE4, 0xD1,
  0x42, 0x3D, 0xD4, 0x30, 0xA3, 0x3C, 0xB6, 0x26,
  0x6F, 0xBF, 0x0E, 0xDA, 0x46, 0x69, 0x07, 0x57,
  0x27, 0xF2, 0x1D, 0x9B, 0xBC, 0x94, 0x43, 0x03,
  0xF8, 0x11, 0xC7, 0xF6, 0x90, 0xEF, 0x3E, 0xE7,
  0x06, 0xC3, 0xD5, 0x2F, 0xC8, 0x66, 0x1E, 0xD7,
  0x08, 0xE8, 0xEA, 0xDE, 0x80, 0x52, 0xEE, 0xF7,
  0x84, 0xAA, 0x72, 0xAC, 0x35, 0x4D, 0x6A, 0x2A,
  0x96, 0x1A, 0xD2, 0x71, 0x5A, 0x15, 0x49, 0x74,
  0x4B, 0x9F, 0xD0, 0x5E, 0x04, 0x18, 0xA4, 0xEC,
  0xC2, 0xE0, 0x41, 0x6E, 0x0F, 0x51, 0xCB, 0xCC,
  0x24, 0x91, 0xAF, 0x50, 0xA1, 0xF4, 0x70, 0x39,
  0x99, 0x7C, 0x3A, 0x85, 0x23, 0xB8, 0xB4, 0x7A,
  0xFC, 0x02, 0x36, 0x5B, 0x25, 0x55, 0x97, 0x31,
  0x2D, 0x5D, 0xFA, 0x98, 0xE3, 0x8A, 0x92, 0xAE,
  0x05, 0xDF, 0x29, 0x10, 0x67, 0x6C, 0xBA, 0xC9,
  0xD3, 0x00, 0xE6, 0xCF, 0xE1, 0x9E, 0xA8, 0x2C,
  0x63, 0x16, 0x01, 0x3F, 0x58, 0xE2, 0x89, 0xA9,
  0x0D, 0x38, 0x34, 0x1B, 0xAB, 0x33, 0xFF, 0xB0,
  0xBB, 0x48, 0x0C, 0x5F, 0xB9, 0xB1, 0xCD, 0x2E,
  0xC5, 0xF3, 0xDB, 0x47, 0xE5, 0xA5, 0x9C, 0x77,
  0x0A, 0xA6, 0x20, 0x68, 0xFE, 0x7F, 0xC1, 0xAD
};


void rand_key(char *key, gsize keylen) {
  srand(clock());
  while(keylen--) key[keylen] = rand() & 0xff;
}


void mwIV_init(char *iv) {
  static unsigned char normal_iv[] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
  };
  memcpy(iv, normal_iv, 8);

  /*
  *iv++ = 0x01; *iv++ = 0x23;
  *iv++ = 0x45; *iv++ = 0x67;
  *iv++ = 0x89; *iv++ = 0xab;
  *iv++ = 0xcd; *iv   = 0xef;
  */
}


/* This does not seem to produce the same results as normal RC2 key
   expansion would, but it works, so eh. It might be smart to farm
   this out to mozilla or openssl */
void mwKeyExpand(int *ekey, const char *key, gsize keylen) {
  char tmp[128];
  int i, j;

  /*
  g_message("expanding key from:");
  pretty_print(key, keylen);
  */

  if(keylen > 128) keylen = 128;
  memcpy(tmp, key, keylen);

  for(i = 0; keylen < 128; i++) {
    tmp[keylen] = PT[ (tmp[keylen - 1] + tmp[i]) & 0xff ];
    keylen++;
  }

  tmp[0] = PT[ tmp[0] & 0xff ];

  for(i = 0, j = 0; i < 64; i++) {
    ekey[i] = (tmp[j] & 0xff) | (tmp[j+1] << 8);
    j += 2;
  }
}


/* normal RC2 encryption given a full 128-byte (as 64 ints) key */
static void mwEncryptBlock(const int *ekey, char *out) {
  int a = (out[7] << 8) | (out[6] & 0xff);
  int b = (out[5] << 8) | (out[4] & 0xff);
  int c = (out[3] << 8) | (out[2] & 0xff);
  int d = (out[1] << 8) | (out[0] & 0xff);

  int i, j;

  for(i = 0; i < 16; i++) {
    j = i * 4;

    d += ((c & (a ^ 0xffff)) + (b & a) + ekey[j++]);
    d = (d << 1) | (d >> 15 & 0x0001);

    c += ((b & (d ^ 0xffff)) + (a & d) + ekey[j++]);
    c = (c << 2) | (c >> 14 & 0x0003);

    b += ((a & (c ^ 0xffff)) + (d & c) + ekey[j++]);
    b = (b << 3) | (b >> 13 & 0x0007);
    
    a += ((d & (b ^ 0xffff)) + (c & b) + ekey[j++]);
    a = (a << 5) | (a >> 11 & 0x001f);

    if(i == 4 || i == 10) {
      d += ekey[a & 0x003f];
      c += ekey[d & 0x003f];
      b += ekey[c & 0x003f];
      a += ekey[b & 0x003f];
    }    
  }

  *out++ = d & 0xff;
  *out++ = (d >> 8) & 0xff;
  *out++ = c & 0xff;
  *out++ = (c >> 8) & 0xff;
  *out++ = b & 0xff;
  *out++ = (b >> 8) & 0xff;
  *out++ = a & 0xff;
  *out++ = (a >> 8) & 0xff;
}


void mwEncryptExpanded(const int *ekey, char *iv,
		       struct mwOpaque *in_data,
		       struct mwOpaque *out_data) {

  char *i = in_data->data;
  gsize i_len = in_data->len;

  char *o;
  gsize o_len;

  int x, y;

  /* pad upwards to a multiple of 8 */
  o_len = (i_len & -8) + 8;
  o = g_malloc(o_len);

  out_data->data = o;
  out_data->len = o_len;

  /* figure out the amount of padding */
  y = o_len - i_len;

  /* copy in to out, and write padding bytes */
  memcpy(o, i, i_len);
  memset(o + i_len, y, y);

  /* encrypt in blocks */
  for(x = o_len; x > 0; x -= 8) {
    for(y = 8; y--; o[y] ^= iv[y]);
    mwEncryptBlock(ekey, o);
    memcpy(iv, o, 8);
    o += 8;
  }
}


void mwEncrypt(const char *key, gsize keylen, char *iv,
	       struct mwOpaque *in, struct mwOpaque *out) {

  int ekey[64];
  mwKeyExpand(ekey, key, keylen);
  mwEncryptExpanded(ekey, iv, in, out);
}


static void mwDecryptBlock(const int *ekey, char *out) {
  int a = (out[7] << 8) | (out[6] & 0xff);
  int b = (out[5] << 8) | (out[4] & 0xff);
  int c = (out[3] << 8) | (out[2] & 0xff);
  int d = (out[1] << 8) | (out[0] & 0xff);

  int i, j;

  for(i = 16; i--; ) {
    j = i * 4 + 3;

    a = (a << 11) | (a >> 5 & 0x07ff);
    a -= ((d & (b ^ 0xffff)) + (c & b) + ekey[j--]);

    b = (b << 13) | (b >> 3 & 0x1fff);
    b -= ((a & (c ^ 0xffff)) + (d & c) + ekey[j--]);

    c = (c << 14) | (c >> 2 & 0x3fff);
    c -= ((b & (d ^ 0xffff)) + (a & d) + ekey[j--]);

    d = (d << 15) | (d >> 1 & 0x7fff);
    d -= ((c & (a ^ 0xffff)) + (b & a) + ekey[j--]);

    if(i == 5 || i == 11) {
      a -= ekey[b & 0x003f];
      b -= ekey[c & 0x003f];
      c -= ekey[d & 0x003f];
      d -= ekey[a & 0x003f];
    }
  }

  *out++ = d & 0xff;
  *out++ = (d >> 8) & 0xff;
  *out++ = c & 0xff;
  *out++ = (c >> 8) & 0xff;
  *out++ = b & 0xff;
  *out++ = (b >> 8) & 0xff;
  *out++ = a & 0xff;
  *out++ = (a >> 8) & 0xff;
}


void mwDecryptExpanded(const int *ekey, char *iv,
		       struct mwOpaque *in_data,
		       struct mwOpaque *out_data) {

  char *i = in_data->data;
  gsize i_len = in_data->len;

  char *o;
  gsize o_len;

  int x, y;

  /* this doesn't check to ensure that in_data->len is a multiple of
     8, which is damn well ought to be. */

  o = g_malloc(i_len);
  o_len = i_len;
  memcpy(o, i, i_len);

  out_data->data = o;
  out_data->len = o_len;

  for(x = o_len; x > 0; x -= 8) {
    /* decrypt a block */
    mwDecryptBlock(ekey, o);

    /* modify the initialization vector */
    for(y = 8; y--; o[y] ^= iv[y]);
    memcpy(iv, i, 8);
    i += 8;
    o += 8;
  }

  /* shorten the length by the value of the filler in the padding
     bytes */
  out_data->len -= *(o - 1);
}


void mwDecrypt(const char *key, gsize keylen, char *iv,
	       struct mwOpaque *in, struct mwOpaque *out) {

  int ekey[64];
  mwKeyExpand(ekey, key, keylen);
  mwDecryptExpanded(ekey, iv, in, out);
}


struct mwCipher_RC2_40 {
  struct mwCipher cipher;
  int session_key[64];
  gboolean ready;
};


struct mwCipherInstance_RC2_40 {
  struct mwCipherInstance instance;
  int incoming_key[64];
  char outgoing_iv[8];
  char incoming_iv[8];
};


static const char *get_name_RC2_40() {
  return "RC2/40 Cipher";
}


static const char *get_desc_RC2_40() {
  return "RC2, 40-bit effective key";
}


static int encrypt_RC2_40(struct mwCipherInstance *ci,
			  struct mwOpaque *data) {

  struct mwCipherInstance_RC2_40 *cir;
  struct mwCipher_RC2_40 *cr;
  struct mwOpaque o = { 0, 0 };

  cir = (struct mwCipherInstance_RC2_40 *) ci;
  cr = (struct mwCipher_RC2_40 *) ci->cipher;

  mwEncryptExpanded(cr->session_key, cir->outgoing_iv, data, &o);

  mwOpaque_clear(data);
  data->data = o.data;
  data->len = o.len;

  return 0;
}


static int decrypt_RC2_40(struct mwCipherInstance *ci,
			  struct mwOpaque *data) {
  
  struct mwCipherInstance_RC2_40 *cir;
  struct mwCipher_RC2_40 *cr;
  struct mwOpaque o = { 0, 0 };

  cir = (struct mwCipherInstance_RC2_40 *) ci;
  cr = (struct mwCipher_RC2_40 *) ci->cipher;

  mwDecryptExpanded(cir->incoming_key, cir->incoming_iv, data, &o);

  mwOpaque_clear(data);
  data->data = o.data;
  data->len = o.len;

  return 0;
}


static struct mwCipherInstance *new_instance_RC2_40(struct mwCipher *cipher,
						    struct mwChannel *chan) {
  struct mwCipher_RC2_40 *cr;
  struct mwCipherInstance_RC2_40 *cir;
  struct mwCipherInstance *ci;

  cr = (struct mwCipher_RC2_40 *) cipher;

  cir = g_new0(struct mwCipherInstance_RC2_40, 1);
  ci = &cir->instance;

  ci->cipher = cipher;
  ci->channel = chan;

  /* a bit of lazy initialization here */
  if(! cr->ready) {
    struct mwLoginInfo *info = mwSession_getLoginInfo(cipher->session);
    mwKeyExpand(cr->session_key, info->login_id, 5);
    cr->ready = TRUE;
  }

  mwIV_init(cir->incoming_iv);
  mwIV_init(cir->outgoing_iv);

  return ci;
}


static struct mwEncryptItem *new_item_RC2_40(struct mwCipherInstance *ci) {
  struct mwEncryptItem *e = g_new0(struct mwEncryptItem, 1);
  e->id = mwCipher_RC2_40;
  return e;
}


static void accept_RC2_40(struct mwCipherInstance *ci) {
  struct mwCipherInstance_RC2_40 *cir;
  struct mwLoginInfo *info = mwChannel_getUser(ci->channel);

  cir = (struct mwCipherInstance_RC2_40 *) ci;
  mwKeyExpand(cir->incoming_key, info->login_id, 5);
}


static void accepted_RC2_40(struct mwCipherInstance *ci,
			    struct mwEncryptItem *item) {
  accept_RC2_40(ci);
}


struct mwCipher *mwCipher_new_RC2_40(struct mwSession *s) {
  struct mwCipher_RC2_40 *cr = g_new0(struct mwCipher_RC2_40, 1);
  struct mwCipher *c = &cr->cipher;

  c->session = s;
  c->type = mwCipher_RC2_40;
  c->get_name = get_name_RC2_40;
  c->get_desc = get_desc_RC2_40;
  c->new_instance = new_instance_RC2_40;
  c->new_item = new_item_RC2_40;

  c->accepted = accepted_RC2_40;
  c->accept = accept_RC2_40;

  c->encrypt = encrypt_RC2_40;
  c->decrypt = decrypt_RC2_40;

  return c;
}


struct mwSession *mwCipher_getSession(struct mwCipher *cipher) {
  g_return_val_if_fail(cipher != NULL, NULL);
  return cipher->session;
}


guint16 mwCipher_getType(struct mwCipher *cipher) {
  /* oh man, this is a bad failover... who the hell decided to make
     zero a real cipher id?? */
  g_return_val_if_fail(cipher != NULL, 0x00);
  return cipher->type;
}


const char *mwCipher_getName(struct mwCipher *cipher) {
  g_return_val_if_fail(cipher != NULL, NULL);
  g_return_val_if_fail(cipher->get_name != NULL, NULL);
  return cipher->get_name();
}


const char *mwCipher_getDesc(struct mwCipher *cipher) {
  g_return_val_if_fail(cipher != NULL, NULL);
  g_return_val_if_fail(cipher->get_desc != NULL, NULL);
  return cipher->get_desc();
}


void mwCipher_free(struct mwCipher *cipher) {
  if(! cipher) return;

  if(cipher->clear)
    cipher->clear(cipher);

  g_free(cipher);
}


struct mwCipherInstance *mwCipher_newInstance(struct mwCipher *cipher,
					      struct mwChannel *chan) {
  g_return_val_if_fail(cipher != NULL, NULL);
  g_return_val_if_fail(chan != NULL, NULL);
  g_return_val_if_fail(cipher->new_instance != NULL, NULL);
  return cipher->new_instance(cipher, chan);
}


struct mwCipher *mwCipherInstance_getCipher(struct mwCipherInstance *ci) {
  g_return_val_if_fail(ci != NULL, NULL);
  return ci->cipher;
}


struct mwChannel *mwCipherInstance_getChannel(struct mwCipherInstance *ci) {
  g_return_val_if_fail(ci != NULL, NULL);
  return ci->channel;
}


struct mwEncryptItem *mwCipherInstance_newItem(struct mwCipherInstance *ci) {
  struct mwCipher *cipher;

  g_return_val_if_fail(ci != NULL, NULL);
  cipher = ci->cipher;

  g_return_val_if_fail(cipher != NULL, NULL);
  g_return_val_if_fail(cipher->new_item != NULL, NULL);

  return cipher->new_item(ci);
}


void mwCipherInstance_offered(struct mwCipherInstance *ci,
			      struct mwEncryptItem *item) {
  struct mwCipher *cipher;

  g_return_if_fail(ci != NULL);

  cipher = ci->cipher;
  g_return_if_fail(cipher != NULL);

  if(cipher->offered) cipher->offered(ci, item);
}


void mwCipherInstance_offer(struct mwCipherInstance *ci) {
  struct mwCipher *cipher;

  g_return_if_fail(ci != NULL);

  cipher = ci->cipher;
  g_return_if_fail(cipher != NULL);

  if(cipher->offer) cipher->offer(ci);
}


void mwCipherInstance_accepted(struct mwCipherInstance *ci,
			       struct mwEncryptItem *item) {
  struct mwCipher *cipher;

  g_return_if_fail(ci != NULL);

  cipher = ci->cipher;
  g_return_if_fail(cipher != NULL);

  if(cipher->accepted) cipher->accepted(ci, item);
}


void mwCipherInstance_accept(struct mwCipherInstance *ci) {
  struct mwCipher *cipher;

  g_return_if_fail(ci != NULL);

  cipher = ci->cipher;
  g_return_if_fail(cipher != NULL);

  if(cipher->accept) cipher->accept(ci);
}


int mwCipherInstance_encrypt(struct mwCipherInstance *ci,
			     struct mwOpaque *data) {
  struct mwCipher *cipher;

  g_return_val_if_fail(data != NULL, 0);

  if(! ci) return 0;
  cipher = ci->cipher;

  g_return_val_if_fail(cipher != NULL, -1);

  return (cipher->encrypt)?
    cipher->encrypt(ci, data): 0;
}


int mwCipherInstance_decrypt(struct mwCipherInstance *ci,
			     struct mwOpaque *data) {
  struct mwCipher *cipher;

  g_return_val_if_fail(data != NULL, 0);

  if(! ci) return 0;
  cipher = ci->cipher;

  g_return_val_if_fail(cipher != NULL, -1);

  return (cipher->decrypt)?
    cipher->decrypt(ci, data): 0;
}


void mwCipherInstance_free(struct mwCipherInstance *ci) {
  struct mwCipher *cipher;

  if(! ci) return;

  cipher = ci->cipher;

  if(cipher && cipher->clear_instance)
    cipher->clear_instance(ci);

  g_free(ci);
}


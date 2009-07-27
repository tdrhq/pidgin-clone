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
#include "internal.h"

#include "hash.h"

/*******************************************************************************
 * Structs
 ******************************************************************************/

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_hash_finalize(GObject *obj) {
	purple_hash_reset(PURPLE_HASH(obj));

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
purple_hash_class_init(PurpleHashClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	obj_class->finalize = purple_hash_finalize;
}

/******************************************************************************
 * PurpleHash API
 *****************************************************************************/
GType
purple_hash_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleHashClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_hash_class_init,
			NULL,
			NULL,
			sizeof(PurpleHash),
			0,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
									  "PurpleHash",
									  &info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}

void
purple_hash_reset(PurpleHash *hash) {
	PurpleHashClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_HASH(hash));

	klass = PURPLE_HASH_GET_CLASS(hash);

	if(klass && klass->reset)
		klass->reset(hash);
}

void
purple_hash_set_iv(PurpleHash *hash, guchar *iv, size_t len)
{
	PurpleHashClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_HASH(hash));
	g_return_if_fail(iv);

	klass = PURPLE_HASH_GET_CLASS(hash);

	if(klass && klass->set_iv)
		klass->set_iv(hash, iv, len);
}

void
purple_hash_append(PurpleHash *hash, const guchar *data,
								size_t len)
{
	PurpleHashClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_HASH(hash));

	klass = PURPLE_HASH_GET_CLASS(hash);

	if(klass && klass->append)
		klass->append(hash, data, len);
}

gboolean
purple_hash_digest(PurpleHash *hash, size_t in_len,
						   guchar digest[], size_t *out_len)
{
	PurpleHashClass *klass = NULL;

	g_return_val_if_fail(PURPLE_IS_HASH(hash), FALSE);

	klass = PURPLE_HASH_GET_CLASS(hash);

	if(klass && klass->digest)
		return klass->digest(hash, in_len, digest, out_len);

	return FALSE;
}

gboolean
purple_hash_digest_to_str(PurpleHash *hash, size_t in_len,
								   gchar digest_s[], size_t *out_len)
{
	/* 8k is a bit excessive, will tweak later. */
	guchar digest[BUF_LEN * 4];
	gint n = 0;
	size_t dlen = 0;

	g_return_val_if_fail(hash, FALSE);
	g_return_val_if_fail(digest_s, FALSE);

	if(!purple_hash_digest(hash, sizeof(digest), digest, &dlen))
		return FALSE;

	/* in_len must be greater than dlen * 2 so we have room for the NUL. */
	if(in_len <= dlen * 2)
		return FALSE;

	for(n = 0; n < dlen; n++)
		sprintf(digest_s + (n * 2), "%02x", digest[n]);

	digest_s[n * 2] = '\0';

	if(out_len)
		*out_len = dlen * 2;

	return TRUE;
}

void
purple_hash_set_salt(PurpleHash *hash, guchar *salt) {
	PurpleHashClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_HASH(hash));

	klass = PURPLE_HASH_GET_CLASS(hash);

	if(klass && klass->set_salt)
		klass->set_salt(hash, salt);
}

size_t
purple_hash_get_salt_size(PurpleHash *hash) {
	PurpleHashClass *klass = NULL;

	g_return_val_if_fail(PURPLE_IS_HASH(hash), -1);

	klass = PURPLE_HASH_GET_CLASS(hash);

	if(klass && klass->get_salt_size)
		return klass->get_salt_size(hash);

	return -1;
}

void
purple_hash_set_key(PurpleHash *hash, const guchar *key) {
	PurpleHashClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_HASH(hash));

	klass = PURPLE_HASH_GET_CLASS(hash);

	if(klass && klass->set_key)
		klass->set_key(hash, key);
}

size_t
purple_hash_get_key_length(PurpleHash *hash) {
	PurpleHashClass *klass = NULL;

	g_return_val_if_fail(PURPLE_IS_HASH(hash), -1);

	klass = PURPLE_HASH_GET_CLASS(hash);

	if(klass && klass->get_key_length)
		return klass->get_key_length(hash);

	return -1;
}

void
purple_hash_set_key_with_length(PurpleHash *hash, const guchar *key,
								size_t length)
{
	PurpleHashClass *klass = NULL;

	g_return_if_fail(PURPLE_IS_HASH(hash));
	g_return_if_fail(key);

	klass = PURPLE_HASH_GET_CLASS(hash);

	if(klass && klass->set_key_with_length)
		klass->set_key_with_length(hash, key, length);
}


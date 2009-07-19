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

#if GLIB_CHECK_VERSION(2,16,0)
#include "ghash.h"

#define PURPLE_G_HASH_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), PURPLE_TYPE_G_HASH, PurpleGHashPrivate))

/*******************************************************************************
 * Structs
 ******************************************************************************/
typedef struct {
	GChecksum *checksum;
	GChecksumType type;
} PurpleGHashPrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_GCHECKSUM_TYPE,
	PROP_LAST,
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Helpers
 *****************************************************************************/
static void
purple_g_hash_set_checksum_type(PurpleGHash *cipher, GChecksumType type)
{
	PurpleGHashPrivate *priv = PURPLE_G_HASH_GET_PRIVATE(cipher);

	priv->type = type;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_g_hash_reset(PurpleCipher *cipher)
{
	PurpleGHashPrivate *priv = PURPLE_G_HASH_GET_PRIVATE(cipher);

	if(priv->checksum) {
#if GLIB_CHECK_VERSION(2,18,0)
		g_checksum_reset(priv->checksum);
#else
		g_checksum_free(priv->checksum);
		priv->checksum = g_checksum_new(priv->type);
#endif
	}
}

static void
purple_g_hash_append(PurpleCipher *cipher, const guchar *data, size_t len)
{
	PurpleGHashPrivate *priv = PURPLE_G_HASH_GET_PRIVATE(cipher);

	while (len >= G_MAXSSIZE) {
		g_checksum_update(priv->checksum, data, G_MAXSSIZE);
		len -= G_MAXSSIZE;
		data += G_MAXSSIZE;
	}

	if (len)
		g_checksum_update(priv->checksum, data, len);
}

static gboolean
purple_g_hash_digest(PurpleCipher *cipher, size_t in_len,
						guchar digest[16], size_t *out_len)
{
	PurpleGHashPrivate *priv = PURPLE_G_HASH_GET_PRIVATE(cipher);

	const gssize required_length = g_checksum_type_get_length(priv->type);

	g_return_val_if_fail(in_len >= required_length, FALSE);
	g_return_val_if_fail(priv->checksum != NULL, FALSE);

	g_checksum_get_digest(priv->checksum, digest, &in_len);

	purple_cipher_reset(cipher);

	if (out_len)
		*out_len = in_len;

	return TRUE;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_g_hash_set_property(GObject *obj, guint param_id,
                            const GValue *value, GParamSpec *pspec)
{
	PurpleGHash *cipher = PURPLE_G_HASH(obj);

	switch(param_id) {
		case PROP_GCHECKSUM_TYPE:
			purple_g_hash_set_checksum_type(cipher, g_value_get_int(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_g_hash_get_property(GObject *obj, guint param_id, GValue *value,
                            GParamSpec *pspec)
{
	PurpleGHash *g_hash = PURPLE_G_HASH(obj);

	switch(param_id) {
		case PROP_GCHECKSUM_TYPE:
			g_value_set_int(value, purple_g_hash_get_checksum_type(g_hash));
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_g_hash_finalize(GObject *obj) {
	PurpleGHashPrivate *priv = PURPLE_G_HASH_GET_PRIVATE(obj);

	g_checksum_reset(priv->checksum);
	g_checksum_free(priv->checksum);
	priv->checksum = NULL;

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
purple_g_hash_class_init(PurpleGHashClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	PurpleCipherClass *cipher_class = PURPLE_CIPHER_CLASS(klass);
	GParamSpec *pspec = NULL;

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(PurpleGHashPrivate));

	obj_class->set_property = purple_g_hash_set_property;
	obj_class->get_property = purple_g_hash_get_property;
	obj_class->finalize = purple_g_hash_finalize;

	cipher_class->reset = purple_g_hash_reset;
	cipher_class->append = purple_g_hash_append;
	cipher_class->digest = purple_g_hash_digest;

	pspec = g_param_spec_int("checksum_type", "Checksum Type",
			"The GChecksumType for this cipher",
			0, G_MAXINT, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property(obj_class, PROP_GCHECKSUM_TYPE, pspec);
}

/******************************************************************************
 * PurpleCipher API
 *****************************************************************************/
GType
purple_g_hash_get_type(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleGHashClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_g_hash_class_init,
			NULL,
			NULL,
			sizeof(PurpleGHash),
			0,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
									  "PurpleGHash",
									  &info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}

GChecksumType
purple_g_hash_get_checksum_type(PurpleGHash *cipher)
{
	PurpleGHashPrivate *priv = NULL;

	g_return_val_if_fail(PURPLE_IS_G_HASH(cipher), -1);

	priv = PURPLE_G_HASH_GET_PRIVATE(cipher);

	return priv->type;
}

#endif /* GLIB_CHECK_VERSION(2,16,0) */

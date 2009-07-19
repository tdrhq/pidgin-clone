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
#include "gcipher.h"

#define PURPLE_GCIPHER_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), PURPLE_TYPE_GCIPHER, PurpleGCipherPrivate))

/*******************************************************************************
 * Structs
 ******************************************************************************/
typedef struct {
	GChecksum *checksum;
	GChecksumType type;
} PurpleGCipherPrivate;

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
purple_gcipher_set_checksum_type(PurpleGCipher *cipher, GChecksumType type)
{
	PurpleGCipherPrivate *priv = PURPLE_GCIPHER_GET_PRIVATE(cipher);

	priv->type = type;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_gcipher_reset(PurpleCipher *cipher)
{
	PurpleGCipherPrivate *priv = PURPLE_GCIPHER_GET_PRIVATE(cipher);

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
purple_gcipher_append(PurpleCipher *cipher, const guchar *data, size_t len)
{
	PurpleGCipherPrivate *priv = PURPLE_GCIPHER_GET_PRIVATE(cipher);

	while (len >= G_MAXSSIZE) {
		g_checksum_update(priv->checksum, data, G_MAXSSIZE);
		len -= G_MAXSSIZE;
		data += G_MAXSSIZE;
	}

	if (len)
		g_checksum_update(priv->checksum, data, len);
}

static gboolean
purple_gcipher_digest(PurpleCipher *cipher, size_t in_len,
						guchar digest[16], size_t *out_len)
{
	PurpleGCipherPrivate *priv = PURPLE_GCIPHER_GET_PRIVATE(cipher);

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
purple_gcipher_set_property(GObject *obj, guint param_id,
                            const GValue *value, GParamSpec *pspec)
{
	PurpleGCipher *cipher = PURPLE_GCIPHER(obj);

	switch(param_id) {
		case PROP_GCHECKSUM_TYPE:
			purple_gcipher_set_checksum_type(cipher, g_value_get_int(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_gcipher_get_property(GObject *obj, guint param_id, GValue *value,
                            GParamSpec *pspec)
{
	PurpleGCipher *gcipher = PURPLE_GCIPHER(obj);

	switch(param_id) {
		case PROP_GCHECKSUM_TYPE:
			g_value_set_int(value, purple_gcipher_get_checksum_type(gcipher));
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_gcipher_finalize(GObject *obj) {
	PurpleGCipherPrivate *priv = PURPLE_GCIPHER_GET_PRIVATE(obj);

	g_checksum_reset(priv->checksum);
	g_checksum_free(priv->checksum);
	priv->checksum = NULL;

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
purple_gcipher_class_init(PurpleGCipherClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	PurpleCipherClass *cipher_class = PURPLE_CIPHER_CLASS(klass);
	GParamSpec *pspec = NULL;

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(PurpleGCipherPrivate));

	obj_class->set_property = purple_gcipher_set_property;
	obj_class->get_property = purple_gcipher_get_property;
	obj_class->finalize = purple_gcipher_finalize;

	cipher_class->reset = purple_gcipher_reset;
	cipher_class->append = purple_gcipher_append;
	cipher_class->digest = purple_gcipher_digest;

	pspec = g_param_spec_int("checksum_type", "Checksum Type",
			"The GChecksumType for this cipher",
			0, G_MAXINT, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property(obj_class, PROP_GCHECKSUM_TYPE, pspec);
}

/******************************************************************************
 * PurpleCipher API
 *****************************************************************************/
GType
purple_gcipher_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleGCipherClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_gcipher_class_init,
			NULL,
			NULL,
			sizeof(PurpleGCipher),
			0,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
									  "PurpleGCipher",
									  &info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}

GChecksumType
purple_gcipher_get_checksum_type(PurpleGCipher *cipher)
{
	PurpleGCipherPrivate *priv = NULL;

	g_return_val_if_fail(PURPLE_IS_GCIPHER(cipher), -1);

	priv = PURPLE_GCIPHER_GET_PRIVATE(cipher);

	return priv->type;
}

#endif /* GLIB_CHECK_VERSION(2,16,0) */

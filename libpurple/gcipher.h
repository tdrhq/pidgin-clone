/**
 * @file gcipher.h Purple Cipher API
 * @ingroup core
 */

/* purple
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
#ifndef PURPLE_GCIPHER_H
#define PURPLE_GCIPHER_H

#include <cipher.h>
#include <glib.h>
#if GLIB_CHECK_VERSION(2,16,0)

/*******************************************************************************
 * New Cipher Header
 ******************************************************************************/
#include <glib-object.h>

#define PURPLE_TYPE_GCIPHER				(purple_gcipher_get_gtype())
#define PURPLE_GCIPHER(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), PURPLE_TYPE_GCIPHER, PurpleGCipher))
#define PURPLE_GCIPHER_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), PURPLE_TYPE_GCIPHER, PurpleGCipherClass))
#define PURPLE_IS_GCIPHER(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), PURPLE_TYPE_GCIPHER))
#define PURPLE_IS_GCIPHER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), PURPLE_TYPE_GCIPHER))
#define PURPLE_GCIPHER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), PURPLE_TYPE_GCIPHER, PurpleGCipherClass))

typedef struct _PurpleGCipher		PurpleGCipher;
typedef struct _PurpleGCipherPriv	PurpleGCipherPriv;
typedef struct _PurpleGCipherClass	PurpleGCipherClass;

struct _PurpleGCipher {
	GObject gparent;

	PurpleGCipherPriv *priv;

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
};

struct _PurpleGCipherClass {
	GObjectClass gparent;

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
};

G_BEGIN_DECLS

GType purple_gcipher_get_gtype(void);

/**
 * Gets the type of a cipher
 *
 * @param cipher  The cipher
 * @returns The type of the cipher
 */
GChecksumType purple_gcipher_get_type(PurpleGCipher *cipher);

G_END_DECLS

#endif /* GLIB_CHECK_VERSION */
#endif /* PURPLE_GCIPHER_H */

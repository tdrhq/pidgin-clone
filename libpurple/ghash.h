/**
 * @file ghash.h Purple GHash API
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
#ifndef PURPLE_GHASH_H
#define PURPLE_GHASH_H

#include <cipher.h>
#include <glib.h>
#if GLIB_CHECK_VERSION(2,16,0)

#include <glib-object.h>

#define PURPLE_TYPE_G_HASH				(purple_g_hash_get_type())
#define PURPLE_G_HASH(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), PURPLE_TYPE_G_HASH, PurpleGHash))
#define PURPLE_G_HASH_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), PURPLE_TYPE_G_HASH, PurpleGHashClass))
#define PURPLE_IS_G_HASH(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), PURPLE_TYPE_G_HASH))
#define PURPLE_IS_G_HASH_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), PURPLE_TYPE_G_HASH))
#define PURPLE_G_HASH_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), PURPLE_TYPE_G_HASH, PurpleGHashClass))

typedef struct _PurpleGHash				PurpleGHash;
typedef struct _PurpleGHashClass		PurpleGHashClass;

struct _PurpleGHash {
	PurpleCipher parent;

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
};

struct _PurpleGHashClass {
	PurpleCipherClass parent;

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
};

G_BEGIN_DECLS

GType purple_g_hash_get_type(void);

/**
 * Gets the type of a hash
 *
 * @param hash  The hash
 * @returns The type of the cipher
 */
GChecksumType purple_g_hash_get_checksum_type(PurpleGHash *hash);

G_END_DECLS

#endif /* GLIB_CHECK_VERSION */
#endif /* PURPLE_G_HASH_H */

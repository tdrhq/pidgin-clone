/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 
#ifndef JINGLE_FILE_TRANSFER_H
#define JINGLE_FILE_TRANSFER_H

#include "config.h"

#include <glib.h>
#include <glib-object.h>

#include "content.h"
#include "xmlnode.h"
#include "ft.h"

G_BEGIN_DECLS

#define JINGLE_TYPE_FT            (jingle_file_transfer_get_type())
#define JINGLE_FT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), \
									JINGLE_TYPE_FT, JingleFT))
#define JINGLE_FT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), \
									JINGLE_TYPE_FT, JingleFTClass))
#define JINGLE_IS_FT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
									JINGLE_TYPE_FT))
#define JINGLE_IS_FT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), \
									JINGLE_TYPE_FT))
#define JINGLE_FT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), \
									JINGLE_TYPE_FT, JingleFTClass))

/** @copydoc _JingleFT */
typedef struct _JingleFT JingleFT;
/** @copydoc _JingleFTClass */
typedef struct _JingleFTClass JingleFTClass;
/** @copydoc _JingleFTPrivate */
typedef struct _JingleFTPrivate JingleFTPrivate;

/** The rtp class */
struct _JingleFTClass
{
	JingleContentClass parent_class;     /**< The parent class. */
};

/** The rtp class's private data */
struct _JingleFT
{
	JingleContent parent;                /**< The parent of this object. */
	JingleFTPrivate *priv;      /**< The private data of this object. */
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Gets the rtp class's GType
 *
 * @return The rtp class's GType.
 */
GType jingle_file_transfer_get_type(void);

PurpleXfer *jingle_file_transfer_new_xfer(PurpleConnection *gc, const gchar *who);

void jingle_file_transfer_send(PurpleConnection *gc, const gchar *who, 
	const gchar *file);

PurpleXfer *jingle_file_transfer_get_xfer(JingleContent *content);

#ifdef __cplusplus
}
#endif

G_END_DECLS


#endif /* JINGLE_FILE_TRANSFER_H */
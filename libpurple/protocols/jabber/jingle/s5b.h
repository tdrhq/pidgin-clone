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
 
#ifndef JINGLE_S5B_H
#define JINGLE_S5B_H
 
#include <glib.h>
#include <glib-object.h>

#include "transport.h"

G_BEGIN_DECLS

#define JINGLE_TYPE_S5B            (jingle_s5b_get_type())
#define JINGLE_S5B(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), JINGLE_TYPE_S5B, JingleS5B))
#define JINGLE_S5B_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), JINGLE_TYPE_S5B, JingleS5BClass))
#define JINGLE_IS_S5B(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), JINGLE_TYPE_S5B))
#define JINGLE_IS_S5B_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), JINGLE_TYPE_S5B))
#define JINGLE_S5B_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), JINGLE_TYPE_S5B, JingleS5BClass))

/** @copydoc _JingleS5B */
typedef struct _JingleS5B JingleS5B;
/** @copydoc _JingleS5BClass */
typedef struct _JingleS5BClass JingleS5BClass;
/** @copydoc _JingleS5BPrivate */
typedef struct _JingleS5BPrivate JingleS5BPrivate;
 
 /** The S5B class */
struct _JingleS5BClass
{
	JingleTransportClass parent_class;     /**< The parent class. */

	xmlnode *(*to_xml) (JingleTransport *transport, xmlnode *content, JingleActionType action);
	JingleTransport *(*parse) (xmlnode *transport);
};

/** The IBB class's private data */
struct _JingleS5B
{
	JingleTransport parent;                /**< The parent of this object. */
	JingleS5BPrivate *priv;      /**< The private data of this object. */
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Gets the S5B class's GType
 *
 * @return The S5B class's GType.
 */
GType jingle_s5b_get_type(void);

const gchar *jingle_s5b_get_sid(const JingleS5B *s5b);
void jingle_s5b_set_sid(JingleS5B *s5b, const gchar *sid);

void jingle_s5b_add_streamhosts(JingleS5B *s5b, const xmlnode *transport);

/* start discovering streamhosts, initiator will send session-initiate when
	done, receiver will send transport-info */
void jingle_s5b_gather_streamhosts(JingleSession *session, JingleS5B *s5b);

void jingle_s5b_attempt_connect(JingleSession *session, JingleS5B *s5b);

void jingle_s5b_stop_connection_attempts(JingleS5B *s5b);

gboolean jingle_s5b_is_connected_to_remote(const JingleS5B *s5b);
gboolean jingle_s5b_remote_is_connected(const JingleS5B *s5b);

void jingle_s5b_surrender(JingleS5B *s5b);
void jingle_s5b_take_command(JingleS5B *s5b);

#ifdef __cplusplus
}
#endif

G_END_DECLS
 
#endif /* JINGLE_S5B_H */
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
 
 #ifndef JINGLE_IBB_H
 #define JINGLE_IBB_H
 
 #include <glib.h>
#include <glib-object.h>

#include "transport.h"

G_BEGIN_DECLS

#define JINGLE_TYPE_IBB            (jingle_ibb_get_type())
#define JINGLE_IBB(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), JINGLE_TYPE_IBB, JingleIBB))
#define JINGLE_IBB_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), JINGLE_TYPE_IBB, JingleIBBClass))
#define JINGLE_IS_IBB(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), JINGLE_TYPE_IBB))
#define JINGLE_IS_IBB_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), JINGLE_TYPE_IBB))
#define JINGLE_IBB_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), JINGLE_TYPE_IBB, JingleIBBClass))

/** @copydoc _JingleIBB */
typedef struct _JingleIBB JingleIBB;
/** @copydoc _JingleIBBClass */
typedef struct _JingleIBBClass JingleIBBClass;
/** @copydoc _JingleIBBPrivate */
typedef struct _JingleIBBPrivate JingleIBBPrivate;
 
 /** The IBB class */
struct _JingleIBBClass
{
	JingleTransportClass parent_class;     /**< The parent class. */

	xmlnode *(*to_xml) (JingleTransport *transport, xmlnode *content, JingleActionType action);
	JingleTransport *(*parse) (xmlnode *transport);
};

/** The IBB class's private data */
struct _JingleIBB
{
	JingleTransport parent;                /**< The parent of this object. */
	JingleIBBPrivate *priv;      /**< The private data of this object. */
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Gets the IBB class's GType
 *
 * @return The IBB class's GType.
 */
GType jingle_ibb_get_type(void);

/* Set up the internal JabberIBBSession as being initiator */
void jingle_ibb_create_session(JingleIBB *ibb, JingleContent *content, 
	const gchar *sid, const gchar *who);

/* Callback handling */
typedef void (JingleIBBSentCallback)(JingleContent *);
typedef void (JingleIBBDataCallback)(JingleContent *, gconstpointer, gsize); 
typedef void (JingleIBBErrorCallback)(JingleContent *);

void jingle_ibb_set_data_sent_callback(JingleIBB *ibb, 
	JingleIBBSentCallback *cb);
void jingle_ibb_set_data_received_callback(JingleIBB *ibb,
	JingleIBBDataCallback *cb);
void jingle_ibb_set_error_callback(JingleIBB *ibb, JingleIBBErrorCallback *cb);

/* send a packet of data through the transport */
void jingle_ibb_send_data(JingleIBB *ibb, gconstpointer data, gsize size);

/* get the (max) block size of the IBB session */
gsize jingle_ibb_get_block_size(const JingleIBB *ibb);

#ifdef __cplusplus
}
#endif

G_END_DECLS
 
 #endif /* JINGLE_IBB_H */
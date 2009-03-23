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
 
#include "internal.h"

#include "jingle.h"
#include "session.h"
#include "content.h"
#include "s5b.h"
#include "debug.h"
#include "xmlnode.h"

struct _JingleS5BPrivate {
	/* S5B stuff here... */
	guint fd;
};

#define JINGLE_S5B_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), JINGLE_TYPE_S5B, JingleS5BPrivate))

static void jingle_s5b_class_init (JingleS5BClass *klass);
static void jingle_s5b_init (JingleS5B *s5b);
static void jingle_s5b_finalize (GObject *object);
static void jingle_s5b_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static void jingle_s5b_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static JingleTransport *jingle_s5b_parse_internal(xmlnode *s5b);
static xmlnode *jingle_s5b_to_xml_internal(JingleTransport *transport, xmlnode *content, JingleActionType action);

static JingleTransportClass *parent_class = NULL;

enum {
	PROP_0
};

GType
jingle_s5b_get_type()
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof(JingleS5BClass),
			NULL,
			NULL,
			(GClassInitFunc) jingle_s5b_class_init,
			NULL,
			NULL,
			sizeof(JingleS5B),
			0,
			(GInstanceInitFunc) jingle_s5b_init,
			NULL
		};
		type = g_type_register_static(JINGLE_TYPE_TRANSPORT, "JingleS5B", &info, 0);
	}
	return type;
}

static void
jingle_s5b_class_init (JingleS5BClass *klass)
{
	GObjectClass *gobject_class = (GObjectClass*)klass;
	parent_class = g_type_class_peek_parent(klass);

	gobject_class->finalize = jingle_s5b_finalize;
	gobject_class->set_property = jingle_s5b_set_property;
	gobject_class->get_property = jingle_s5b_get_property;
	klass->parent_class.to_xml = jingle_s5b_to_xml_internal;
	klass->parent_class.parse = jingle_s5b_parse_internal;
	klass->parent_class.transport_type = JINGLE_TRANSPORT_S5B;

	g_type_class_add_private(klass, sizeof(JingleS5BPrivate));
}

static void
jingle_s5b_init (JingleS5B *s5b)
{	
	s5b->priv = JINGLE_S5B_GET_PRIVATE(s5b);
	memset(s5b->priv, 0, sizeof(s5b->priv));
}

static void
jingle_s5b_finalize (GObject *s5b)
{
	JingleS5BPrivate *priv = JINGLE_S5B_GET_PRIVATE(s5b);
	purple_debug_info("jingle-s5b","jingle_s5b_finalize\n");
	
	G_OBJECT_CLASS(parent_class)->finalize(s5b);
}

static void
jingle_s5b_set_property (GObject *object, guint prop_id, const GValue *value, 
	GParamSpec *pspec)
{
	JingleS5B *s5b;
	g_return_if_fail(JINGLE_IS_S5B(object));

	s5b = JINGLE_S5B(object);

	switch (prop_id) {
		default:	
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
jingle_s5b_get_property (GObject *object, guint prop_id, GValue *value, 
	GParamSpec *pspec)
{
	JingleS5B *s5b;
	g_return_if_fail(JINGLE_IS_S5B(object));
	
	s5b = JINGLE_S5B(object);

	switch (prop_id) {
		default:	
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);	
			break;
	}
}

static JingleTransport *
jingle_s5b_parse_internal(xmlnode *s5b)
{
	JingleTransport *transport = parent_class->parse(s5b);
	JingleS5BPrivate *priv = JINGLE_S5B_GET_PRIVATE(transport);
	
	return transport;
}

static xmlnode *
jingle_s5b_to_xml_internal(JingleTransport *transport, xmlnode *content, 
	JingleActionType action)
{
	xmlnode *node = parent_class->to_xml(transport, content, action);

	purple_debug_info("jingle", "jingle_ibb_to_xml_internal\n");

	return node;
}


/**
 * @file xmlnode.c XML DOM functions
 *
 * gaim
 *
 * Gaim is the legal property of its developers, whose names are too numerous
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* A lot of this code at least resembles the code in libxode, but since
 * libxode uses memory pools that we simply have no need for, I decided to
 * write my own stuff.  Also, re-writing this lets me be as lightweight
 * as I want to be.  Thank you libxode for giving me a good starting point */

#include "internal.h"

#include <string.h>
#include <glib.h>

#include "xmlnode.h"

#ifdef _WIN32
# define NEWLINE_S "\r\n"
#else
# define NEWLINE_S "\n"
#endif

static xmlnode*
new_node(const char *name, XMLNodeType type)
{
	xmlnode *node = g_new0(xmlnode, 1);

	if(name)
		node->name = g_strdup(name);
	node->type = type;

	return node;
}

xmlnode*
xmlnode_new(const char *name)
{
	g_return_val_if_fail(name != NULL, NULL);

	return new_node(name, XMLNODE_TYPE_TAG);
}

xmlnode *
xmlnode_new_child(xmlnode *parent, const char *name)
{
	xmlnode *node;

	g_return_val_if_fail(parent != NULL, NULL);
	g_return_val_if_fail(name != NULL, NULL);

	node = new_node(name, XMLNODE_TYPE_TAG);

	xmlnode_insert_child(parent, node);

	return node;
}

void
xmlnode_insert_child(xmlnode *parent, xmlnode *child)
{
	g_return_if_fail(parent != NULL);
	g_return_if_fail(child != NULL);

	child->parent = parent;

	if(parent->child) {
		xmlnode *x;
		for(x = parent->child; x->next; x = x->next);
		x->next = child;
	} else {
		parent->child = child;
	}
}

void
xmlnode_insert_data(xmlnode *node, const char *data, gssize size)
{
	xmlnode *child;
	gsize real_size;

	g_return_if_fail(node != NULL);
	g_return_if_fail(data != NULL);
	g_return_if_fail(size != 0);

	real_size = size == -1 ? strlen(data) : size;

	child = new_node(NULL, XMLNODE_TYPE_DATA);

	child->data = g_memdup(data, real_size);
	child->data_sz = real_size;

	xmlnode_insert_child(node, child);
}

void
xmlnode_remove_attrib(xmlnode *node, const char *attr)
{
	xmlnode *attr_node, *sibling = NULL;

	g_return_if_fail(node != NULL);
	g_return_if_fail(attr != NULL);

	for(attr_node = node->child; attr_node; attr_node = attr_node->next)
	{
		if(attr_node->type == XMLNODE_TYPE_ATTRIB &&
				!strcmp(attr_node->name, attr)) {
			if(node->child == attr_node) {
				node->child = attr_node->next;
			} else {
				sibling->next = attr_node->next;
			}
			xmlnode_free(attr_node);
			return;
		}
		sibling = attr_node;
	}
}

void
xmlnode_set_attrib(xmlnode *node, const char *attr, const char *value)
{
	xmlnode *attrib_node;

	g_return_if_fail(node != NULL);
	g_return_if_fail(attr != NULL);
	g_return_if_fail(value != NULL);

	xmlnode_remove_attrib(node, attr);

	attrib_node = new_node(attr, XMLNODE_TYPE_ATTRIB);

	attrib_node->data = g_strdup(value);

	xmlnode_insert_child(node, attrib_node);
}

const char *
xmlnode_get_attrib(xmlnode *node, const char *attr)
{
	xmlnode *x;

	g_return_val_if_fail(node != NULL, NULL);

	for(x = node->child; x; x = x->next) {
		if(x->type == XMLNODE_TYPE_ATTRIB && !strcmp(attr, x->name)) {
			return x->data;
		}
	}

	return NULL;
}

void
xmlnode_free(xmlnode *node)
{
	xmlnode *x, *y;

	g_return_if_fail(node != NULL);

	x = node->child;
	while(x) {
		y = x->next;
		xmlnode_free(x);
		x = y;
	}

	if(node->name)
		g_free(node->name);
	if(node->data)
		g_free(node->data);
	g_free(node);
}

xmlnode*
xmlnode_get_child(const xmlnode *parent, const char *name)
{
	return xmlnode_get_child_with_namespace(parent, name, NULL);
}

xmlnode *
xmlnode_get_child_with_namespace(const xmlnode *parent, const char *name, const char *ns)
{
	xmlnode *x, *ret = NULL;
	char **names;
	char *parent_name, *child_name;

	g_return_val_if_fail(parent != NULL, NULL);

	names = g_strsplit(name, "/", 2);
	parent_name = names[0];
	child_name = names[1];

	for(x = parent->child; x; x = x->next) {
		const char *xmlns = NULL;
		if(ns)
			xmlns = xmlnode_get_attrib(x, "xmlns");

		if(x->type == XMLNODE_TYPE_TAG && name && !strcmp(parent_name, x->name)
				&& (!ns || (xmlns && !strcmp(ns, xmlns)))) {
			ret = x;
			break;
		}
	}

	if(child_name && ret)
		ret = xmlnode_get_child(ret, child_name);

	g_strfreev(names);
	return ret;
}

char *
xmlnode_get_data(xmlnode *node)
{
	GString *str = NULL;
	xmlnode *c;

	g_return_val_if_fail(node != NULL, NULL);

	for(c = node->child; c; c = c->next) {
		if(c->type == XMLNODE_TYPE_DATA) {
			if(!str)
				str = g_string_new("");
			str = g_string_append_len(str, c->data, c->data_sz);
		}
	}

	if (str == NULL)
		return NULL;

	return g_string_free(str, FALSE);
}

static char *
xmlnode_to_str_helper(xmlnode *node, int *len, gboolean formatting, int depth)
{
	GString *text = g_string_new("");
	xmlnode *c;
	char *node_name, *esc, *esc2, *tab = NULL;
	gboolean need_end = FALSE, pretty = formatting;

	if(pretty && depth) {
		tab = g_strnfill(depth, '\t');
		text = g_string_append(text, tab);
	}

	node_name = g_markup_escape_text(node->name, -1);
	g_string_append_printf(text, "<%s", node_name);

	for(c = node->child; c; c = c->next)
	{
		if(c->type == XMLNODE_TYPE_ATTRIB) {
			esc = g_markup_escape_text(c->name, -1);
			esc2 = g_markup_escape_text(c->data, -1);
			g_string_append_printf(text, " %s='%s'", esc, esc2);
			g_free(esc);
			g_free(esc2);
		} else if(c->type == XMLNODE_TYPE_TAG || c->type == XMLNODE_TYPE_DATA) {
			if(c->type == XMLNODE_TYPE_DATA)
				pretty = FALSE;
			need_end = TRUE;
		}
	}

	if(need_end) {
		g_string_append_printf(text, ">%s", pretty ? NEWLINE_S : "");

		for(c = node->child; c; c = c->next)
		{
			if(c->type == XMLNODE_TYPE_TAG) {
				int esc_len;
				esc = xmlnode_to_str_helper(c, &esc_len, pretty, depth+1);
				text = g_string_append_len(text, esc, esc_len);
				g_free(esc);
			} else if(c->type == XMLNODE_TYPE_DATA) {
				esc = g_markup_escape_text(c->data, c->data_sz);
				text = g_string_append(text, esc);
				g_free(esc);
			}
		}

		if(tab && pretty)
			text = g_string_append(text, tab);
		g_string_append_printf(text, "</%s>%s", node_name, formatting ? NEWLINE_S : "");
	} else {
		g_string_append_printf(text, "/>%s", formatting ? NEWLINE_S : "");
	}

	g_free(node_name);

	if(tab)
		g_free(tab);

	if(len)
		*len = text->len;

	return g_string_free(text, FALSE);
}

char *
xmlnode_to_str(xmlnode *node, int *len)
{
	return xmlnode_to_str_helper(node, len, FALSE, 0);
}

char *
xmlnode_to_formatted_str(xmlnode *node, int *len)
{
	char *xml, *xml_with_declaration;

	xml = xmlnode_to_str_helper(node, len, TRUE, 0);
	xml_with_declaration =
		g_strdup_printf("<?xml version='1.0' encoding='UTF-8' ?>" NEWLINE_S NEWLINE_S "%s", xml);
	g_free(xml);

	return xml_with_declaration;
}

struct _xmlnode_parser_data {
	xmlnode *current;
};

static void
xmlnode_parser_element_start(GMarkupParseContext *context,
		const char *element_name, const char **attrib_names,
		const char **attrib_values, gpointer user_data, GError **error)
{
	struct _xmlnode_parser_data *xpd = user_data;
	xmlnode *node;
	int i;

	if(!element_name) {
		return;
	} else {
		if(xpd->current)
			node = xmlnode_new_child(xpd->current, element_name);
		else
			node = xmlnode_new(element_name);

		for(i=0; attrib_names[i]; i++)
			xmlnode_set_attrib(node, attrib_names[i], attrib_values[i]);

		xpd->current = node;
	}
}

static void
xmlnode_parser_element_end(GMarkupParseContext *context,
		const char *element_name, gpointer user_data, GError **error)
{
	struct _xmlnode_parser_data *xpd = user_data;

	if(!element_name || !xpd->current)
		return;

	if(xpd->current->parent) {
		if(!strcmp(xpd->current->name, element_name))
			xpd->current = xpd->current->parent;
	}
}

static void
xmlnode_parser_element_text(GMarkupParseContext *context, const char *text,
		gsize text_len, gpointer user_data, GError **error)
{
	struct _xmlnode_parser_data *xpd = user_data;

	if(!xpd->current)
		return;

	if(!text || !text_len)
		return;

	xmlnode_insert_data(xpd->current, text, text_len);
}

static GMarkupParser xmlnode_parser = {
	xmlnode_parser_element_start,
	xmlnode_parser_element_end,
	xmlnode_parser_element_text,
	NULL,
	NULL
};


xmlnode *
xmlnode_from_str(const char *str, gssize size)
{
	struct _xmlnode_parser_data *xpd;
	xmlnode *ret;
	GMarkupParseContext *context;
	gsize real_size;

	g_return_val_if_fail(str != NULL, NULL);

	real_size = size < 0 ? strlen(str) : size;
	xpd = g_new0(struct _xmlnode_parser_data, 1);
	context = g_markup_parse_context_new(&xmlnode_parser, 0, xpd, NULL);

	if(!g_markup_parse_context_parse(context, str, real_size, NULL)) {
		while(xpd->current && xpd->current->parent)
			xpd->current = xpd->current->parent;
		if(xpd->current)
			xmlnode_free(xpd->current);
		xpd->current = NULL;
	}
	g_markup_parse_context_free(context);

	ret = xpd->current;
	g_free(xpd);
	return ret;
}

xmlnode *
xmlnode_copy(xmlnode *src)
{
	xmlnode *ret;
	xmlnode *child;
	xmlnode *sibling = NULL;

	if(!src)
		return NULL;

	ret = new_node(src->name, src->type);
	if(src->data) {
		if(src->data_sz) {
			ret->data = g_memdup(src->data, src->data_sz);
			ret->data_sz = src->data_sz;
		} else {
			ret->data = g_strdup(src->data);
		}
	}

	for(child = src->child; child; child = child->next) {
		if(sibling) {
			sibling->next = xmlnode_copy(child);
			sibling = sibling->next;
		} else {
			ret->child = xmlnode_copy(child);
			sibling = ret->child;
		}
		sibling->parent = ret;
	}

	return ret;
}

xmlnode *
xmlnode_get_next_twin(xmlnode *node)
{
	xmlnode *sibling;
	const char *ns = xmlnode_get_attrib(node, "xmlns");

	g_return_val_if_fail(node != NULL, NULL);
	g_return_val_if_fail(node->type == XMLNODE_TYPE_TAG, NULL);

	for(sibling = node->next; sibling; sibling = sibling->next) {
		const char *xmlns = NULL;
		if(ns)
			xmlns = xmlnode_get_attrib(sibling, "xmlns");

		if(sibling->type == XMLNODE_TYPE_TAG && !strcmp(node->name, sibling->name) &&
				(!ns || (xmlns && !strcmp(ns, xmlns))))
			return sibling;
	}

	return NULL;
}

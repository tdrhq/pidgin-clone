/*
 * gaim - Jabber Protocol Plugin
 *
 * Copyright (C) 2003, Nathan Walp <faceprint@faceprint.com>
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
 *
 */
#include "internal.h"
#include "debug.h"
#include "multi.h"
#include "notify.h"
#include "request.h"
#include "util.h"

#include "buddy.h"
#include "chat.h"
#include "jabber.h"
#include "iq.h"
#include "presence.h"
#include "xmlnode.h"


JabberBuddy *jabber_buddy_find(JabberStream *js, const char *name,
		gboolean create)
{
	JabberBuddy *jb;
	JabberID *jid = jabber_id_new(name);
	char *realname;

	if(!jid)
		return NULL;

	if(jid->node)
		realname = g_strdup_printf("%s@%s", jid->node, jid->domain);
	else
		realname = g_strdup(jid->domain);

	jb = g_hash_table_lookup(js->buddies, realname);

	if(!jb && create) {
		jb = g_new0(JabberBuddy, 1);
		g_hash_table_insert(js->buddies, g_strdup(realname), jb);
	}
	g_free(realname);

	jabber_id_free(jid);

	return jb;
}


JabberBuddyResource *jabber_buddy_find_resource(JabberBuddy *jb,
		const char *resource)
{
	JabberBuddyResource *jbr = NULL;
	GList *l;

	if(!jb)
		return NULL;

	for(l = jb->resources; l; l = l->next)
	{
		if(!jbr && !resource) {
			jbr = l->data;
		} else if(!resource) {
			if(((JabberBuddyResource *)l->data)->priority >= jbr->priority)
				jbr = l->data;
		} else if(((JabberBuddyResource *)l->data)->name) {
			if(!strcmp(((JabberBuddyResource *)l->data)->name, resource)) {
				jbr = l->data;
				break;
			}
		}
	}

	return jbr;
}

void jabber_buddy_track_resource(JabberBuddy *jb, const char *resource,
		int priority, int state, const char *status)
{
	JabberBuddyResource *jbr = jabber_buddy_find_resource(jb, resource);

	if(!jbr) {
		jbr = g_new0(JabberBuddyResource, 1);
		jbr->name = g_strdup(resource);
		jbr->capabilities = JABBER_CAP_XHTML;
		jb->resources = g_list_append(jb->resources, jbr);
	}
	jbr->priority = priority;
	jbr->state = state;
	if(jbr->status)
		g_free(jbr->status);
	jbr->status = g_strdup(status);
}

void jabber_buddy_remove_resource(JabberBuddy *jb, const char *resource)
{
	JabberBuddyResource *jbr = jabber_buddy_find_resource(jb, resource);

	if(!jbr)
		return;

	jb->resources = g_list_remove(jb->resources, jbr);

	g_free(jbr->name);
	if(jbr->status)
		g_free(jbr->status);
	g_free(jbr);
}

const char *jabber_buddy_get_status_msg(JabberBuddy *jb)
{
	JabberBuddyResource *jbr;

	if(!jb)
		return NULL;

	jbr = jabber_buddy_find_resource(jb, NULL);

	if(!jbr)
		return NULL;

	return jbr->status;
}

/*******
 * This is the old vCard stuff taken from the old prpl.  vCards, by definition
 * are a temporary thing until jabber can get its act together and come up
 * with a format for user information, hence the namespace of 'vcard-temp'
 *
 * Since I don't feel like putting that much work into something that's
 * _supposed_ to go away, i'm going to just copy the kludgy old code here,
 * and make it purdy when jabber comes up with a standards-track JEP to
 * replace vcard-temp
 *                                 --Nathan
 *******/

/*---------------------------------------*/
/* Jabber "set info" (vCard) support     */
/*---------------------------------------*/

/*
 * V-Card format:
 *
 *  <vCard prodid='' version='' xmlns=''>
 *    <FN></FN>
 *    <N>
 *	<FAMILY/>
 *	<GIVEN/>
 *    </N>
 *    <NICKNAME/>
 *    <URL/>
 *    <ADR>
 *	<STREET/>
 *	<EXTADD/>
 *	<LOCALITY/>
 *	<REGION/>
 *	<PCODE/>
 *	<COUNTRY/>
 *    </ADR>
 *    <TEL/>
 *    <EMAIL/>
 *    <ORG>
 *	<ORGNAME/>
 *	<ORGUNIT/>
 *    </ORG>
 *    <TITLE/>
 *    <ROLE/>
 *    <DESC/>
 *    <BDAY/>
 *  </vCard>
 *
 * See also:
 *
 *	http://docs.jabber.org/proto/html/vcard-temp.html
 *	http://www.vcard-xml.org/dtd/vCard-XML-v2-20010520.dtd
 */

/*
 * Cross-reference user-friendly V-Card entry labels to vCard XML tags
 * and attributes.
 *
 * Order is (or should be) unimportant.  For example: we have no way of
 * knowing in what order real data will arrive.
 *
 * Format: Label, Pre-set text, "visible" flag, "editable" flag, XML tag
 *         name, XML tag's parent tag "path" (relative to vCard node).
 *
 *         List is terminated by a NULL label pointer.
 *
 *	   Entries with no label text, but with XML tag and parent tag
 *	   entries, are used by V-Card XML construction routines to
 *	   "automagically" construct the appropriate XML node tree.
 *
 * Thoughts on future direction/expansion
 *
 *	This is a "simple" vCard.
 *
 *	It is possible for nodes other than the "vCard" node to have
 *      attributes.  Should that prove necessary/desirable, add an
 *      "attributes" pointer to the vcard_template struct, create the
 *      necessary tag_attr structs, and add 'em to the vcard_dflt_data
 *      array.
 *
 *	The above changes will (obviously) require changes to the vCard
 *      construction routines.
 */

struct vcard_template {
	char *label;			/* label text pointer */
	char *text;			/* entry text pointer */
	int  visible;			/* should entry field be "visible?" */
	int  editable;			/* should entry field be editable? */
	char *tag;			/* tag text */
	char *ptag;			/* parent tag "path" text */
	char *url;			/* vCard display format if URL */
} vcard_template_data[] = {
	{N_("Full Name"),          NULL, TRUE, TRUE, "FN",        NULL,  NULL},
	{N_("Family Name"),        NULL, TRUE, TRUE, "FAMILY",    "N",   NULL},
	{N_("Given Name"),         NULL, TRUE, TRUE, "GIVEN",     "N",   NULL},
	{N_("Nickname"),           NULL, TRUE, TRUE, "NICKNAME",  NULL,  NULL},
	{N_("URL"),                NULL, TRUE, TRUE, "URL",       NULL,  "<A HREF=\"%s\">%s</A>"},
	{N_("Street Address"),     NULL, TRUE, TRUE, "STREET",    "ADR", NULL},
	{N_("Extended Address"),   NULL, TRUE, TRUE, "EXTADD",    "ADR", NULL},
	{N_("Locality"),           NULL, TRUE, TRUE, "LOCALITY",  "ADR", NULL},
	{N_("Region"),             NULL, TRUE, TRUE, "REGION",    "ADR", NULL},
	{N_("Postal Code"),        NULL, TRUE, TRUE, "PCODE",     "ADR", NULL},
	{N_("Country"),            NULL, TRUE, TRUE, "COUNTRY",   "ADR", NULL},
	{N_("Telephone"),          NULL, TRUE, TRUE, "TELEPHONE", NULL,  NULL},
	{N_("Email"),              NULL, TRUE, TRUE, "EMAIL",     NULL,  "<A HREF=\"mailto:%s\">%s</A>"},
	{N_("Organization Name"),  NULL, TRUE, TRUE, "ORGNAME",   "ORG", NULL},
	{N_("Organization Unit"),  NULL, TRUE, TRUE, "ORGUNIT",   "ORG", NULL},
	{N_("Title"),              NULL, TRUE, TRUE, "TITLE",     NULL,  NULL},
	{N_("Role"),               NULL, TRUE, TRUE, "ROLE",      NULL,  NULL},
	{N_("Birthday"),           NULL, TRUE, TRUE, "BDAY",      NULL,  NULL},
	{N_("Description"),        NULL, TRUE, TRUE, "DESC",      NULL,  NULL},
	{"", NULL, TRUE, TRUE, "N",     NULL, NULL},
	{"", NULL, TRUE, TRUE, "ADR",   NULL, NULL},
	{"", NULL, TRUE, TRUE, "ORG",   NULL, NULL},
	{NULL, NULL, 0, 0, NULL, NULL, NULL}
};

/*
 * The "vCard" tag's attibute list...
 */
struct tag_attr {
	char *attr;
	char *value;
} vcard_tag_attr_list[] = {
	{"prodid",   "-//HandGen//NONSGML vGen v1.0//EN"},
	{"version",  "2.0",                             },
	{"xmlns",    "vcard-temp",                      },
	{NULL, NULL},
};


/*
 * Insert a tag node into an xmlnode tree, recursively inserting parent tag
 * nodes as necessary
 *
 * Returns pointer to inserted node
 *
 * Note to hackers: this code is designed to be re-entrant (it's recursive--it
 * calls itself), so don't put any "static"s in here!
 */
static xmlnode *insert_tag_to_parent_tag(xmlnode *start, const char *parent_tag, const char *new_tag)
{
	xmlnode *x = NULL;

	/*
	 * If the parent tag wasn't specified, see if we can get it
	 * from the vCard template struct.
	 */
	if(parent_tag == NULL) {
		struct vcard_template *vc_tp = vcard_template_data;

		while(vc_tp->label != NULL) {
			if(strcmp(vc_tp->tag, new_tag) == 0) {
				parent_tag = vc_tp->ptag;
				break;
			}
			++vc_tp;
		}
	}

	/*
	 * If we have a parent tag...
	 */
	if(parent_tag != NULL ) {
		/*
		 * Try to get the parent node for a tag
		 */
		if((x = xmlnode_get_child(start, parent_tag)) == NULL) {
			/*
			 * Descend?
			 */
			char *grand_parent = g_strdup(parent_tag);
			char *parent;

			if((parent = strrchr(grand_parent, '/')) != NULL) {
				*(parent++) = '\0';
				x = insert_tag_to_parent_tag(start, grand_parent, parent);
			} else {
				x = xmlnode_new_child(start, grand_parent);
			}
			g_free(grand_parent);
		} else {
			/*
			 * We found *something* to be the parent node.
			 * Note: may be the "root" node!
			 */
			xmlnode *y;
			if((y = xmlnode_get_child(x, new_tag)) != NULL) {
				return(y);
			}
		}
	}

	/*
	 * insert the new tag into its parent node
	 */
	return(xmlnode_new_child((x == NULL? start : x), new_tag));
}

/*
 * Send vCard info to Jabber server
 */
void jabber_set_info(GaimConnection *gc, const char *info)
{
	JabberIq *iq;
	JabberStream *js = gc->proto_data;
	xmlnode *vc_node;


	/*
	 * Send only if there's actually any *information* to send
	 */
	vc_node = xmlnode_from_str(info, -1);

	if(vc_node) {
		if (vc_node->name &&
				!g_ascii_strncasecmp(vc_node->name, "vcard", 5)) {
			iq = jabber_iq_new(js, JABBER_IQ_SET);
			xmlnode_insert_child(iq->node, vc_node);
			jabber_iq_send(iq);
		} else {
			xmlnode_free(vc_node);
		}
	}
}

/*
 * This is the callback from the "ok clicked" for "set vCard"
 *
 * Formats GSList data into XML-encoded string and returns a pointer
 * to said string.
 *
 * g_free()'ing the returned string space is the responsibility of
 * the caller.
 */
static void
jabber_format_info(GaimConnection *gc, GaimRequestFields *fields)
{
	GaimAccount *account;
	xmlnode *vc_node;
	GaimRequestField *field;
	const char *text;
	char *p;
	const struct vcard_template *vc_tp;
	struct tag_attr *tag_attr;

	vc_node = xmlnode_new("vCard");

	for(tag_attr = vcard_tag_attr_list; tag_attr->attr != NULL; ++tag_attr)
		xmlnode_set_attrib(vc_node, tag_attr->attr, tag_attr->value);

	for (vc_tp = vcard_template_data; vc_tp->label != NULL; vc_tp++) {
		if (*vc_tp->label == '\0')
			continue;

		field = gaim_request_fields_get_field(fields, vc_tp->tag);
		text  = gaim_request_field_string_get_value(field);

		gaim_debug(GAIM_DEBUG_INFO, "jabber",
				   "Setting %s to '%s'\n", vc_tp->tag, text);

		if (text != NULL && *text != '\0') {
			xmlnode *xp;

			if ((xp = insert_tag_to_parent_tag(vc_node,
											   NULL, vc_tp->tag)) != NULL) {

				xmlnode_insert_data(xp, text, -1);
			}
		}
	}

	p = xmlnode_to_str(vc_node);
	xmlnode_free(vc_node);

	account = gaim_connection_get_account(gc);

	if (account != NULL) {
		gaim_account_set_user_info(account, p);

		if (gc != NULL)
			serv_set_info(gc, p);
	}

	g_free(p);
}

/*
 * This gets executed by the proto action
 *
 * Creates a new GaimRequestFields struct, gets the XML-formatted user_info
 * string (if any) into GSLists for the (multi-entry) edit dialog and
 * calls the set_vcard dialog.
 */
void jabber_setup_set_info(GaimConnection *gc)
{
	GaimRequestFields *fields;
	GaimRequestFieldGroup *group;
	GaimRequestField *field;
	const struct vcard_template *vc_tp;
	char *user_info;
	char *cdata;
	xmlnode *x_vc_data = NULL;

	fields = gaim_request_fields_new();
	group = gaim_request_field_group_new(NULL);
	gaim_request_fields_add_group(fields, group);

	/*
	 * Get existing, XML-formatted, user info
	 */
	if((user_info = g_strdup(gaim_account_get_user_info(gc->account))) != NULL)
		x_vc_data = xmlnode_from_str(user_info, -1);
	else
		user_info = g_strdup("");

	/*
	 * Set up GSLists for edit with labels from "template," data from user info
	 */
	for(vc_tp = vcard_template_data; vc_tp->label != NULL; ++vc_tp) {
		xmlnode *data_node;
		if((vc_tp->label)[0] == '\0')
			continue;
		if(vc_tp->ptag == NULL) {
			data_node = xmlnode_get_child(x_vc_data, vc_tp->tag);
		} else {
			gchar *tag = g_strdup_printf("%s/%s", vc_tp->ptag, vc_tp->tag);
			data_node = xmlnode_get_child(x_vc_data, tag);
			g_free(tag);
		}
		if(data_node)
			cdata = xmlnode_get_data(data_node);
		else
			cdata = NULL;

		if(strcmp(vc_tp->tag, "DESC") == 0) {
			field = gaim_request_field_string_new(vc_tp->tag,
												  _(vc_tp->label), cdata,
												  TRUE);
		} else {
			field = gaim_request_field_string_new(vc_tp->tag,
												  _(vc_tp->label), cdata,
												  FALSE);
		}

		gaim_request_field_group_add_field(group, field);
	}

	if(x_vc_data != NULL)
		xmlnode_free(x_vc_data);

    g_free(user_info);

	gaim_request_fields(gc, _("Edit Jabber vCard"),
						_("Edit Jabber vCard"),
						_("All items below are optional. Enter only the "
						  "information with which you feel comfortable."),
						fields,
						_("Save"), G_CALLBACK(jabber_format_info),
						_("Cancel"), NULL,
						gc);
}

/*---------------------------------------*/
/* End Jabber "set info" (vCard) support */
/*---------------------------------------*/

/******
 * end of that ancient crap that needs to die
 ******/


static void jabber_vcard_parse(JabberStream *js, xmlnode *packet)
{
	GList *resources;
	const char *from = xmlnode_get_attrib(packet, "from");
	JabberBuddy *jb;
	JabberBuddyResource *jbr;
	GString *info_text;
	const char *resource_name;
	char *title;
	xmlnode *vcard;

	if(!from)
		return;

	resource_name = jabber_get_resource(from);

	jb = jabber_buddy_find(js, from, TRUE);
	info_text = g_string_new("");

	g_string_append_printf(info_text, "<b>%s:</b> %s<br/>\n", _("Jabber ID"),
			from);

	if(resource_name) {
		jbr = jabber_buddy_find_resource(jb, resource_name);
		if(jbr) {
			char *purdy = strdup_withhtml(jbr->status);
			g_string_append_printf(info_text, "<b>%s:</b> %s%s%s<br/>\n",
					_("Status"), jabber_get_state_string(jbr->state),
					purdy ? ": " : "",
					purdy ? purdy : "");
			g_free(purdy);
		} else {
			g_string_append_printf(info_text, "<b>%s:</b> %s<br/>\n",
					_("Status"), _("Unknown"));
		}
	} else {
		for(resources = jb->resources; resources; resources = resources->next) {
			char *purdy;
			jbr = resources->data;
			purdy = strdup_withhtml(jbr->status);
			g_string_append_printf(info_text, "<b>%s:</b> %s<br/>\n",
					_("Resource"), jbr->name);
			g_string_append_printf(info_text, "<b>%s:</b> %s%s%s<br/><br/>\n",
					_("Status"), jabber_get_state_string(jbr->state),
					purdy ? ": " : "",
					purdy ? purdy : "");
			g_free(purdy);
		}
	}

	if((vcard = xmlnode_get_child(packet, "vCard"))) {
		xmlnode *child;
		for(child = vcard->child; child; child = child->next)
		{
			xmlnode *child2;
			char *text;

			if(child->type != NODE_TYPE_TAG)
				continue;

			text = xmlnode_get_data(child);
			if(text && !strcmp(child->name, "FN")) {
				g_string_append_printf(info_text, "<b>%s:</b> %s<br/>\n",
						_("Full Name"), text);
			} else if(!strcmp(child->name, "N")) {
				for(child2 = child->child; child2; child2 = child2->next)
				{
					char *text2;

					if(child2->type != NODE_TYPE_TAG)
						continue;

					text2 = xmlnode_get_data(child2);
					if(text2 && !strcmp(child2->name, "FAMILY")) {
						g_string_append_printf(info_text,
								"<b>%s:</b> %s<br/>\n",
								_("Family Name"), text2);
					} else if(text2 && !strcmp(child2->name, "GIVEN")) {
						g_string_append_printf(info_text,
								"<b>%s:</b> %s<br/>\n",
								_("Given Name"), text2);
					} else if(text2 && !strcmp(child2->name, "MIDDLE")) {
						g_string_append_printf(info_text,
								"<b>%s:</b> %s<br/>\n",
								_("Middle Name"), text2);
					}
					g_free(text2);
				}
			} else if(text && !strcmp(child->name, "NICKNAME")) {
				serv_got_alias(js->gc, from, text);
				g_string_append_printf(info_text, "<b>%s:</b> %s<br/>\n",
						_("Nickname"), text);
			} else if(text && !strcmp(child->name, "BDAY")) {
				g_string_append_printf(info_text, "<b>%s:</b> %s<br/>\n",
						_("Birthday"), text);
			} else if(!strcmp(child->name, "ADR")) {
				/* show which address it is */
				if(child->child)
					g_string_append_printf(info_text, "<b>%s:</b><br/>\n",
							_("Address"));
				for(child2 = child->child; child2; child2 = child2->next)
				{
					char *text2;

					if(child2->type != NODE_TYPE_TAG)
						continue;

					text2 = xmlnode_get_data(child2);
					if(text2 && !strcmp(child2->name, "POBOX")) {
						g_string_append_printf(info_text,
								"&nbsp;<b>%s:</b> %s<br/>\n",
								_("P.O. Box"), text2);
					} else if(text2 && !strcmp(child2->name, "EXTADR")) {
						g_string_append_printf(info_text,
								"&nbsp;<b>%s:</b> %s<br/>\n",
								_("Extended Address"), text2);
					} else if(text2 && !strcmp(child2->name, "STREET")) {
						g_string_append_printf(info_text,
								"&nbsp;<b>%s:</b> %s<br/>\n",
								_("Street Address"), text2);
					} else if(text2 && !strcmp(child2->name, "LOCALITY")) {
						g_string_append_printf(info_text,
								"&nbsp;<b>%s:</b> %s<br/>\n",
								_("Locality"), text2);
					} else if(text2 && !strcmp(child2->name, "REGION")) {
						g_string_append_printf(info_text,
								"&nbsp;<b>%s:</b> %s<br/>\n",
								_("Region"), text2);
					} else if(text2 && !strcmp(child2->name, "PCODE")) {
						g_string_append_printf(info_text,
								"&nbsp;<b>%s:</b> %s<br/>\n",
								_("Postal Code"), text2);
					} else if(text2 && (!strcmp(child2->name, "CTRY")
								|| !strcmp(child2->name, "COUNTRY"))) {
						g_string_append_printf(info_text,
								"&nbsp;<b>%s:</b> %s<br/>\n",
								_("Country"), text2);
					}
					g_free(text2);
				}
			} else if(!strcmp(child->name, "TEL")) {
				char *number;
				if((child2 = xmlnode_get_child(child, "NUMBER"))) {
					/* show what kind of number it is */
					number = xmlnode_get_data(child2);
					if(number) {
						g_string_append_printf(info_text,
								"<b>%s:</b> %s<br/>\n", _("Telephone"), number);
						g_free(number);
					}
				} else if((number = xmlnode_get_data(child))) {
					/* lots of clients (including gaim) do this, but it's
					 * out of spec */
					g_string_append_printf(info_text,
							"<b>%s:</b> %s<br/>\n", _("Telephone"), number);
					g_free(number);
				}
			} else if(!strcmp(child->name, "EMAIL")) {
				char *userid;
				if((child2 = xmlnode_get_child(child, "USERID"))) {
					/* show what kind of email it is */
					userid = xmlnode_get_data(child2);
					if(userid) {
						g_string_append_printf(info_text,
								"<b>%s:</b> <a href='mailto:%s'>%s</a><br/>\n",
								_("Email"), userid, userid);
						g_free(userid);
					}
				} else if((userid = xmlnode_get_data(child))) {
					/* lots of clients (including gaim) do this, but it's
					 * out of spec */
						g_string_append_printf(info_text,
								"<b>%s:</b> <a href='mailto:%s'>%s</a><br/>\n",
								_("Email"), userid, userid);
					g_free(userid);
				}
			} else if(!strcmp(child->name, "ORG")) {
				for(child2 = child->child; child2; child2 = child2->next)
				{
					char *text2;

					if(child2->type != NODE_TYPE_TAG)
						continue;

					text2 = xmlnode_get_data(child2);
					if(text2 && !strcmp(child2->name, "ORGNAME")) {
						g_string_append_printf(info_text,
								"<b>%s:</b> %s<br/>\n",
								_("Organization Name"), text2);
					} else if(text2 && !strcmp(child2->name, "ORGUNIT")) {
						g_string_append_printf(info_text,
								"<b>%s:</b> %s<br/>\n",
								_("Organization Unit"), text2);
					}
					g_free(text2);
				}
			} else if(text && !strcmp(child->name, "TITLE")) {
				g_string_append_printf(info_text, "<b>%s:</b> %s<br/>\n",
						_("Title"), text);
			} else if(text && !strcmp(child->name, "ROLE")) {
				g_string_append_printf(info_text, "<b>%s:</b> %s<br/>\n",
						_("Role"), text);
			} else if(text && !strcmp(child->name, "DESC")) {
				g_string_append_printf(info_text, "<b>%s:</b> %s<br/>\n",
						_("Description"), text);
			}
			g_free(text);
		}
	}

	title = g_strdup_printf("User info for %s", from);

	gaim_notify_formatted(NULL, title, _("Jabber Profile"),
			NULL, info_text->str, NULL, NULL);

	g_free(title);
	g_string_free(info_text, TRUE);
}

void jabber_buddy_get_info(GaimConnection *gc, const char *who)
{
	JabberStream *js = gc->proto_data;
	JabberIq *iq;
	xmlnode *vcard;

	iq = jabber_iq_new(js, JABBER_IQ_GET);

	xmlnode_set_attrib(iq->node, "to", who);
	vcard = xmlnode_new_child(iq->node, "vCard");
	xmlnode_set_attrib(vcard, "xmlns", "vcard-temp");

	jabber_iq_set_callback(iq, jabber_vcard_parse);

	jabber_iq_send(iq);
}

void jabber_buddy_get_info_chat(GaimConnection *gc, int id,
		const char *resource)
{
	JabberStream *js = gc->proto_data;
	JabberChat *chat = jabber_chat_find_by_id(js, id);
	char *full_jid;

	if(!chat)
		return;

	full_jid = g_strdup_printf("%s@%s/%s", chat->room, chat->server, resource);
	jabber_buddy_get_info(gc, full_jid);
	g_free(full_jid);
}

static void jabber_buddy_set_invisibility(JabberStream *js, const char *who,
		gboolean invisible)
{
	JabberBuddy *jb = jabber_buddy_find(js, who, TRUE);
	xmlnode *presence;

	presence = jabber_presence_create(js->gc->away_state, js->gc->away);
	xmlnode_set_attrib(presence, "to", who);
	if(invisible) {
		xmlnode_set_attrib(presence, "type", "invisible");
		jb->invisible |= JABBER_INVIS_BUDDY;
	} else {
		jb->invisible &= ~JABBER_INVIS_BUDDY;
	}

	jabber_send(js, presence);
	xmlnode_free(presence);
}

static void jabber_buddy_make_invisible(GaimConnection *gc, const char *name)
{
	JabberStream *js = gc->proto_data;
	jabber_buddy_set_invisibility(js, name, TRUE);
}

static void jabber_buddy_make_visible(GaimConnection *gc, const char *name)
{
	JabberStream *js = gc->proto_data;
	jabber_buddy_set_invisibility(js, name, FALSE);
}

static void jabber_buddy_cancel_presence_notification(GaimConnection *gc,
		const char *name)
{
	JabberStream *js = gc->proto_data;

	/* I wonder if we should prompt the user before doing this */
	jabber_presence_subscription_set(js, name, "unsubscribed");
}

static void jabber_buddy_rerequest_auth(GaimConnection *gc, const char *name)
{
	JabberStream *js = gc->proto_data;

	jabber_presence_subscription_set(js, name, "subscribe");
}

GList *jabber_buddy_menu(GaimConnection *gc, const char *name)
{
	GList *m = NULL;
	struct proto_buddy_menu *pbm;
	JabberStream *js = gc->proto_data;
	JabberBuddy *jb = jabber_buddy_find(js, name, TRUE);

	pbm = g_new0(struct proto_buddy_menu, 1);
	if(jb->invisible & JABBER_INVIS_BUDDY) {
		pbm->label = _("Un-hide From");
		pbm->callback = jabber_buddy_make_visible;
	} else {
		pbm->label = _("Temporarily Hide From");
		pbm->callback = jabber_buddy_make_invisible;
	}
	pbm->gc = gc;
	m = g_list_append(m, pbm);

	if(jb->subscription & JABBER_SUB_FROM) {
		pbm = g_new0(struct proto_buddy_menu, 1);
		pbm->label = _("Cancel Presence Notification");
		pbm->callback = jabber_buddy_cancel_presence_notification;
		pbm->gc = gc;
		m = g_list_append(m, pbm);
	}

	if(!(jb->subscription & JABBER_SUB_TO)) {
		pbm = g_new0(struct proto_buddy_menu, 1);
		pbm->label = _("Re-request authorization");
		pbm->callback = jabber_buddy_rerequest_auth;
		pbm->gc = gc;
		m = g_list_append(m, pbm);
	}

	return m;
}

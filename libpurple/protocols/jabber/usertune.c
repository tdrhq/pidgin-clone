/*
 * purple - Jabber Protocol Plugin
 *
 * Copyright (C) 2007, Andreas Monitzer <andy@monitzer.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	 02111-1307	 USA
 *
 */

#include "internal.h"

#include "usertune.h"
#include "pep.h"
#include <string.h>
#include "internal.h"
#include "request.h"
#include "status.h"

static void jabber_tune_cb(JabberStream *js, const char *from, xmlnode *items) {
	/* it doesn't make sense to have more than one item here, so let's just pick the first one */
	xmlnode *item = xmlnode_get_child(items, "item");
	JabberBuddy *buddy = jabber_buddy_find(js, from, FALSE);
	xmlnode *tuneinfo, *tune;
	PurpleJabberTuneInfo tuneinfodata;
	JabberBuddyResource *resource;
	const char *status_id;
	
	/* ignore the tune of people not on our buddy list */
	if (!buddy || !item)
		return;
	
	tuneinfodata.artist = "";
	tuneinfodata.title = "";
	tuneinfodata.album = "";
	tuneinfodata.track = "";
	tuneinfodata.time = -1;
	tuneinfodata.url = "";
	
	tune = xmlnode_get_child_with_namespace(item, "tune", "http://jabber.org/protocol/tune");
	if (!tune)
		return;
	for (tuneinfo = tune->child; tuneinfo; tuneinfo = tuneinfo->next) {
		if (tuneinfo->type == XMLNODE_TYPE_TAG) {
			if (!strcmp(tuneinfo->name, "artist")) {
				if (tuneinfodata.artist[0] == '\0') /* only pick the first one */
					tuneinfodata.artist = xmlnode_get_data(tuneinfo);
			} else if (!strcmp(tuneinfo->name, "length")) {
				if (tuneinfodata.time == -1) {
					char *length = xmlnode_get_data(tuneinfo);
					if (length)
						tuneinfodata.time = strtol(length, NULL, 10);
				}
			} else if (!strcmp(tuneinfo->name, "source")) {
				if (tuneinfodata.album[0] == '\0') /* only pick the first one */
					tuneinfodata.album = xmlnode_get_data(tuneinfo);
			} else if (!strcmp(tuneinfo->name, "title")) {
				if (tuneinfodata.title[0] == '\0') /* only pick the first one */
					tuneinfodata.title = xmlnode_get_data(tuneinfo);
			} else if (!strcmp(tuneinfo->name, "track")) {
				if (tuneinfodata.track[0] == '\0') /* only pick the first one */
					tuneinfodata.track = xmlnode_get_data(tuneinfo);
			} else if (!strcmp(tuneinfo->name, "uri")) {
				if (tuneinfodata.url[0] == '\0') /* only pick the first one */
					tuneinfodata.url = xmlnode_get_data(tuneinfo);
			}
		}
	}
	resource = jabber_buddy_find_resource(buddy, NULL);
	if(!resource)
		return; /* huh? */
	status_id = jabber_buddy_state_get_status_id(resource->state);

	purple_prpl_got_user_status(js->gc->account, from, status_id, PURPLE_TUNE_ARTIST, tuneinfodata.artist, PURPLE_TUNE_TITLE, tuneinfodata.title, PURPLE_TUNE_ALBUM, tuneinfodata.album, PURPLE_TUNE_TRACK, tuneinfodata.track, PURPLE_TUNE_TIME, tuneinfodata.time, PURPLE_TUNE_URL, tuneinfodata.url, NULL);
}

void jabber_tune_init(void) {
	jabber_add_feature("tune", "http://jabber.org/protocol/tune", jabber_pep_namespace_only_when_pep_enabled_cb);
	jabber_pep_register_handler("tunen", "http://jabber.org/protocol/tune", jabber_tune_cb);
}

void jabber_tune_set(PurpleConnection *gc, const PurpleJabberTuneInfo *tuneinfo) {
	xmlnode *publish, *tunenode;
	JabberStream *js = gc->proto_data;
	
	publish = xmlnode_new("publish");
	xmlnode_set_attrib(publish,"node","http://jabber.org/protocol/tune");
	tunenode = xmlnode_new_child(xmlnode_new_child(publish, "item"), "tune");
	xmlnode_set_namespace(tunenode, "http://jabber.org/protocol/tune");
	
	if(tuneinfo) {
		if(tuneinfo->artist && tuneinfo->artist[0] != '\0')
			xmlnode_insert_data(xmlnode_new_child(tunenode, "artist"),tuneinfo->artist,-1);
		if(tuneinfo->title && tuneinfo->title[0] != '\0')
			xmlnode_insert_data(xmlnode_new_child(tunenode, "title"),tuneinfo->title,-1);
		if(tuneinfo->album && tuneinfo->album[0] != '\0')
			xmlnode_insert_data(xmlnode_new_child(tunenode, "source"),tuneinfo->album,-1);
		if(tuneinfo->url && tuneinfo->url[0] != '\0')
			xmlnode_insert_data(xmlnode_new_child(tunenode, "uri"),tuneinfo->url,-1);
		if(tuneinfo->time >= 0) {
			char *length = g_strdup_printf("%d", tuneinfo->time);
			xmlnode_insert_data(xmlnode_new_child(tunenode, "length"),length,-1);
			g_free(length);
		}
		if(tuneinfo->track && tuneinfo->track[0] != '\0')
			xmlnode_insert_data(xmlnode_new_child(tunenode, "track"),tuneinfo->track,-1);
	}
	
	jabber_pep_publish(js, publish);
	/* publish is freed by jabber_pep_publish -> jabber_iq_send -> jabber_iq_free
	   (yay for well-defined memory management rules) */
}
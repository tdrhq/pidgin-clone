/*
 * nmevent.c
 *
 * Copyright � 2004 Unpublished Work of Novell, Inc. All Rights Reserved.
 *
 * THIS WORK IS AN UNPUBLISHED WORK OF NOVELL, INC. NO PART OF THIS WORK MAY BE
 * USED, PRACTICED, PERFORMED, COPIED, DISTRIBUTED, REVISED, MODIFIED,
 * TRANSLATED, ABRIDGED, CONDENSED, EXPANDED, COLLECTED, COMPILED, LINKED,
 * RECAST, TRANSFORMED OR ADAPTED WITHOUT THE PRIOR WRITTEN CONSENT OF NOVELL,
 * INC. ANY USE OR EXPLOITATION OF THIS WORK WITHOUT AUTHORIZATION COULD SUBJECT
 * THE PERPETRATOR TO CRIMINAL AND CIVIL LIABILITY.
 *
 * AS BETWEEN [GAIM] AND NOVELL, NOVELL GRANTS [GAIM] THE RIGHT TO REPUBLISH
 * THIS WORK UNDER THE GPL (GNU GENERAL PUBLIC LICENSE) WITH ALL RIGHTS AND
 * LICENSES THEREUNDER.  IF YOU HAVE RECEIVED THIS WORK DIRECTLY OR INDIRECTLY
 * FROM [GAIM] AS PART OF SUCH A REPUBLICATION, YOU HAVE ALL RIGHTS AND LICENSES
 * GRANTED BY [GAIM] UNDER THE GPL.  IN CONNECTION WITH SUCH A REPUBLICATION, IF
 * ANYTHING IN THIS NOTICE CONFLICTS WITH THE TERMS OF THE GPL, SUCH TERMS
 * PREVAIL.
 *
 */

#include <glib.h>
#include <string.h>
#include <time.h>
#include "nmevent.h"
#include "nmfield.h"
#include "nmconn.h"
#include "nmuserrecord.h"

struct _NMEvent
{

	/* Event type */
	int type;

	/* The DN of the event source */
	char *source;

	/* Timestamp of the event */
	guint32 gmt;

	/* Conference to associate with the event */
	NMConference *conference;

	/* User record to associate with the event */
	NMUserRecord *user_record;

	/* Text associated with the event */
	char *text;

	/* Reference count for event structure */
	int ref_count;

};

/* Return a copy of src minus the RTF  */
static char *
_strip_rtf(const char *src, int len)
{
	const char *p = src;
	char *q;
	char *dest = g_new0(char, len + 1);
	int level = 0;

	/* Make sure we are dealing with rtf */
	if (strncmp("{\\rtf1", src, strlen("{\\rtf1")) != 0) {
		strncpy(dest, src, len);
		return dest;
	}
	p += strlen("{\\rtf1");

	q = dest;
	while (*p != '\0') {
		if (*p == '\\') {
			if (level == 0) {
				if (*(p + 1) == '\\' || *(p + 1) == '{' || *(p + 1) == '}') {
					*q++ = *(p + 1);
					p++;
				} else if (*(p + 1) == 't' && *(p + 2) == 'a' && *(p + 3) == 'b') {
					*q++ = '\t';
					p++;
				}
			}
			p++;
		} else if (*p == '{') {
			level++;
			p++;
		} else if (*p == '}') {
			level--;
			p++;
		} else if (level == 0) {
			if ((*p == ' ' || *p == '\r') && (*(p + 1) != '\\')) {
				p++;

				if (*p == '\n' &&
					(*(p + 1) == '{' || *(p + 1) == '}' || *(p + 1) == '\\')) {
					p++;
				} else {
					/* We found some text */
					while (*p != '\0' && *p != '\\' && *p != '{' && *p != '}') {
						*q++ = *p;
						p++;
					}
				}
			} else {
				p++;
			}
		} else {
			p++;
		}
	}

	return dest;
}

/* Handle getdetails response and set the new user record into the event */
static void
_got_user_for_event(NMUser * user, NMERR_T ret_val,
					gpointer resp_data, gpointer user_data)
{
	NMUserRecord *user_record;
	NMEvent *event;
	nm_event_cb cb;

	if (user == NULL)
		return;

	user_record = resp_data;
	event = user_data;

	if (ret_val == NM_OK) {
		if (event && user_record) {

			/* Add the user record to the event structure
			 * and make the callback.
			 */
			nm_event_set_user_record(event, user_record);
			if ((cb = nm_user_get_event_callback(user))) {
				cb(user, event);
			}
		}

	} else {
		/* Cleanup resp_data */

	}

	/* Clean up */
	if (event)
		nm_release_event(event);

}

/* Handle getdetails response, set the new user record into the event
 * and add the user record as a participant in the conference
 */
static void
_got_user_for_conference(NMUser * user, NMERR_T ret_val,
						 gpointer resp_data, gpointer user_data)
{
	NMUserRecord *user_record = resp_data;
	NMEvent *event = user_data;
	NMConference *conference;
	nm_event_cb cb;

	if (user == NULL)
		return;

	if (event && user_record) {

		conference = nm_event_get_conference(event);
		if (conference) {

			/* Add source of event as recip of the conference */
			nm_conference_add_participant(conference, user_record);

			/* Add the user record to the event structure
			 * and make the callback.
			 */
			nm_event_set_user_record(event, user_record);
			if ((cb = nm_user_get_event_callback(user))) {
				cb(user, event);
			}
		}
	}

	if (event)
		nm_release_event(event);
}

/* Read the receive message event, set up the event object, and
 * get details for the event source if we don't have them yet.
 */
static NMERR_T
handle_receive_message(NMUser * user, NMEvent * event, gboolean autoreply)
{
	NMConference *conference;
	NMUserRecord *user_record;
	NMConn *conn;
	NMERR_T rc = NM_OK;
	guint32 size = 0, flags = 0;
	char *msg = NULL;
	char *nortf = NULL;
	char *guid = NULL;

	conn = nm_user_get_conn(user);

	/* Read the conference guid */
	rc = nm_read_all(conn, (char *) &size, sizeof(size));
	if (rc == NM_OK) {
		guid = g_new0(char, size + 1);
		rc = nm_read_all(conn, guid, size);
	}

	/* Read the conference flags */
	if (rc == NM_OK) {
		rc = nm_read_all(conn, (char *) &flags, sizeof(flags));
	}

	/* Read the message text */
	if (rc == NM_OK) {
		rc = nm_read_all(conn, (char *) &size, sizeof(size));
		if (rc == NM_OK) {

			msg = g_new0(char, size + 1);
			rc = nm_read_all(conn, msg, size);

			gaim_debug(GAIM_DEBUG_INFO, "novell", "Message is %s\n", msg);

			/* Auto replies are not in RTF format! */
			if (!autoreply) {

				nortf = _strip_rtf((const char *) msg, size);

				gaim_debug(GAIM_DEBUG_INFO, "novell",
						   "Message without RTF is %s\n", nortf);

				/* Store the event data */
				nm_event_set_text(event, nortf);

			} else {

				/* Store the event data */
				nm_event_set_text(event, msg);
			}
		}
	}

	/* Check to see if we already know about the conference */
	conference = nm_conference_list_find(user, guid);
	if (conference) {

		nm_conference_set_flags(conference, flags);
		nm_event_set_conference(event, conference);

		/* Add a reference to the user record in our event object */
		user_record = nm_find_user_record(user, nm_event_get_source(event));
		if (user_record) {
			nm_event_set_user_record(event, user_record);
		}

	} else {

		/* This is a new conference, so create one and add it to our list */
		conference = nm_create_conference(guid);
		nm_conference_set_flags(conference, flags);

		/* Add a reference to the conference in the event */
		nm_event_set_conference(event, conference);

		/* Add new conference to the conference list */
		nm_conference_list_add(user, conference);

		/* Check to see if we have details for the event source yet */
		user_record = nm_find_user_record(user, nm_event_get_source(event));
		if (user_record) {

			/* We do so add the user record as a recipient of the conference */
			nm_conference_add_participant(conference, user_record);

			/* Add a reference to the user record in our event object */
			nm_event_set_user_record(event, user_record);

		} else {

			/* Need to go to the server to get details for the user */
			rc = nm_send_get_details(user, nm_event_get_source(event),
									 _got_user_for_conference, event);
			if (rc == NM_OK)
				rc = -1;		/* Not done processing the event yet! */
		}

		nm_release_conference(conference);
	}

	if (msg)
		g_free(msg);

	if (nortf)
		g_free(nortf);

	if (guid)
		g_free(guid);

	return rc;
}

/* Read the invite event, set up the event object, and
 * get details for the event source if we don't have them yet.
 */
static NMERR_T
handle_conference_invite(NMUser * user, NMEvent * event)
{
	NMERR_T rc = NM_OK;
	guint32 size = 0;
	char *guid = NULL;
	char *msg = NULL;
	NMConn *conn;
	NMUserRecord *user_record;

	conn = nm_user_get_conn(user);

	/* Read the conference guid */
	rc = nm_read_all(conn, (char *) &size, sizeof(size));
	if (rc == NM_OK) {
		guid = g_new0(char, size + 1);
		rc = nm_read_all(conn, guid, size);
	}

	/* Read the the message */
	if (rc == NM_OK) {
		rc = nm_read_all(conn, (char *) &size, sizeof(size));
		if (rc == NM_OK) {
			msg = g_new0(char, size + 1);
			rc = nm_read_all(conn, msg, size);
		}
	}

	/* Store the event data */
	if (rc == NM_OK) {
		NMConference *conference;

		nm_event_set_text(event, msg);

		conference = nm_conference_list_find(user, guid);
		if (conference == NULL) {
			conference = nm_create_conference(guid);

			/* Add new conference to the list and the event */
			nm_conference_list_add(user, conference);
			nm_event_set_conference(event, conference);

			/* Check to see if we have details for the event source yet */
			user_record = nm_find_user_record(user, nm_event_get_source(event));
			if (user_record) {

				/* Add a reference to the user record in our event object */
				nm_event_set_user_record(event, user_record);

			} else {

				/* Need to go to the server to get details for the user */
				rc = nm_send_get_details(user, nm_event_get_source(event),
										 _got_user_for_event, event);
				if (rc == NM_OK)
					rc = -1;	/* Not done processing the event yet! */
			}

			nm_release_conference(conference);

		}
	}

	if (msg)
		g_free(msg);

	if (guid)
		g_free(guid);

	return rc;
}

/* Read the invite notify event, set up the event object, and
 * get details for the event source if we don't have them yet.
 */
static NMERR_T
handle_conference_invite_notify(NMUser * user, NMEvent * event)
{
	NMERR_T rc = NM_OK;
	guint32 size = 0;
	char *guid = NULL;
	NMConn *conn;
	NMConference *conference;
	NMUserRecord *user_record;

	conn = nm_user_get_conn(user);

	/* Read the conference guid */
	rc = nm_read_all(conn, (char *) &size, sizeof(size));
	if (rc == NM_OK) {
		guid = g_new0(char, size + 1);
		rc = nm_read_all(conn, guid, size);
	}

	conference = nm_conference_list_find(user, guid);
	if (conference) {
		nm_event_set_conference(event, conference);

		/* Check to see if we have details for the event source yet */
		user_record = nm_find_user_record(user, nm_event_get_source(event));
		if (user_record) {

			/* Add a reference to the user record in our event object */
			nm_event_set_user_record(event, user_record);

		} else {

			/* Need to go to the server to get details for the user */
			rc = nm_send_get_details(user, nm_event_get_source(event),
									 _got_user_for_event, event);
			if (rc == NM_OK)
				rc = -1;		/* Not done processing the event yet! */
		}

	} else {
		rc = NMERR_CONFERENCE_NOT_FOUND;
	}


	if (guid)
		g_free(guid);

	return rc;
}

/* Read the conference reject event and set up the event object */
static NMERR_T
handle_conference_reject(NMUser * user, NMEvent * event)
{
	NMERR_T rc = NM_OK;
	guint32 size = 0;
	char *guid = NULL;
	NMConn *conn;
	NMConference *conference;

	conn = nm_user_get_conn(user);

	/* Read the conference guid */
	rc = nm_read_all(conn, (char *) &size, sizeof(size));
	if (rc == NM_OK) {
		guid = g_new0(char, size + 1);
		rc = nm_read_all(conn, guid, size);
	}

	if (rc == NM_OK) {
		conference = nm_conference_list_find(user, guid);
		if (conference) {
			nm_event_set_conference(event, conference);
		} else {
			rc = NMERR_CONFERENCE_NOT_FOUND;
		}
	}

	if (guid)
		g_free(guid);

	return rc;
}

/* Read the conference left event, set up the event object, and
 * remove the conference from the list if there are no more
 * participants
 */
static NMERR_T
handle_conference_left(NMUser * user, NMEvent * event)
{
	NMERR_T rc = NM_OK;
	guint32 size = 0, flags = 0;
	char *guid = NULL;
	NMConference *conference;
	NMConn *conn;

	conn = nm_user_get_conn(user);

	/* Read the conference guid */
	rc = nm_read_all(conn, (char *) &size, sizeof(size));
	if (rc == NM_OK) {
		guid = g_new0(char, size + 1);
		rc = nm_read_all(conn, guid, size);
	}

	/* Read the conference flags */
	if (rc == NM_OK) {
		rc = nm_read_all(conn, (char *) &flags, sizeof(flags));
	}

	if (rc == NM_OK) {
		conference = nm_conference_list_find(user, guid);
		if (conference) {
			nm_event_set_conference(event, conference);
			nm_conference_set_flags(conference, flags);

			nm_conference_remove_participant(conference, nm_event_get_source(event));
			if (nm_conference_get_participant_count(conference) == 0) {
				nm_conference_list_remove(user, conference);
			}

		} else {
			rc = NMERR_CONFERENCE_NOT_FOUND;
		}
	}

	if (guid)
		g_free(guid);

	return rc;
}

/* Read the conference closed, set up the event object, and
 * remove the conference from the list
 */
static NMERR_T
handle_conference_closed(NMUser * user, NMEvent * event)
{
	NMERR_T rc = NM_OK;
	guint32 size = 0;
	char *guid = NULL;
	NMConference *conference;
	NMConn *conn;

	conn = nm_user_get_conn(user);

	/* Read the conference guid */
	rc = nm_read_all(conn, (char *) &size, sizeof(size));
	if (rc == NM_OK) {
		guid = g_new0(char, size + 1);
		rc = nm_read_all(conn, guid, size);
	}

	if (rc == NM_OK) {
		conference = nm_conference_list_find(user, guid);
		if (conference) {
			nm_event_set_conference(event, conference);
			nm_conference_list_remove(user, conference);
		} else {
			rc = NMERR_CONFERENCE_NOT_FOUND;
		}
	}

	if (guid)
		g_free(guid);

	return rc;
}

/* Read the conference joined event, set up the event object, and
 * get details for the event source if we don't have them yet.
 */
static NMERR_T
handle_conference_joined(NMUser * user, NMEvent * event)
{
	NMERR_T rc = NM_OK;
	guint32 size = 0, flags = 0;
	char *guid = NULL;
	NMConn *conn;
	NMConference *conference;
	NMUserRecord *user_record;

	conn = nm_user_get_conn(user);

	/* Read the conference guid */
	rc = nm_read_all(conn, (char *) &size, sizeof(size));
	if (rc == NM_OK) {
		guid = g_new0(char, size + 1);
		rc = nm_read_all(conn, guid, size);
	}

	/* Read the conference flags */
	if (rc == NM_OK) {
		rc = nm_read_all(conn, (char *) &flags, sizeof(flags));
	}

	if (rc == NM_OK) {
		conference = nm_conference_list_find(user, guid);
		if (conference) {
			nm_conference_set_flags(conference, flags);

			nm_event_set_conference(event, conference);

			/* Add the new user to the participants list */
			user_record = nm_find_user_record(user, nm_event_get_source(event));
			if (user_record) {
				nm_conference_add_participant(conference, user_record);
			} else {

				/* Need to go to the server to get details for the user */
				rc = nm_send_get_details(user, nm_event_get_source(event),
										 _got_user_for_conference, event);
				if (rc == NM_OK)
					rc = -1;		/* Not done processing the event yet! */
			}

		} else {
			rc = NMERR_CONFERENCE_NOT_FOUND;
		}
	}

	if (guid)
		g_free(guid);

	return rc;
}

/* Read the typing event and set up the event object */
static NMERR_T
handle_typing(NMUser * user, NMEvent * event)
{
	NMERR_T rc = NM_OK;
	guint32 size = 0;
	char *guid = NULL;
	NMConference *conference;
	NMConn *conn;

	conn = nm_user_get_conn(user);

	/* Read the conference guid */
	rc = nm_read_all(conn, (char *) &size, sizeof(size));
	if (rc == NM_OK) {
		guid = g_new0(char, size + 1);
		rc = nm_read_all(conn, guid, size);
	}

	if (rc == NM_OK) {
		conference = nm_conference_list_find(user, guid);
		if (conference) {
			nm_event_set_conference(event, conference);
		} else {
			rc = NMERR_CONFERENCE_NOT_FOUND;
		}
	}

	if (guid)
		g_free(guid);

	return rc;
}

/* Read the event, set up the event object, and update
 * the status in the user record (for the event source)
 */
static NMERR_T
handle_status_change(NMUser * user, NMEvent * event)
{
	NMERR_T rc = NM_OK;
	guint16 status;
	guint32 size;
	char *text = NULL;
	NMUserRecord *user_record;
	NMConn *conn;

	conn = nm_user_get_conn(user);

	/* Read new status */
	rc = nm_read_all(conn, (char *) &status, sizeof(status));
	if (rc == NM_OK) {

		/* Read the status text */
		rc = nm_read_all(conn, (char *) &size, sizeof(size));
		if (rc == NM_OK) {
			if (size > 0) {
				text = g_new0(char, size + 1);
				rc = nm_read_all(conn, text, size);
			}
		}
	}

	if (rc == NM_OK) {
		nm_event_set_text(event, text);

		/* Get a reference to the user record and store the new status */
		user_record = nm_find_user_record(user, nm_event_get_source(event));
		if (user_record) {
			nm_event_set_user_record(event, user_record);
			nm_user_record_set_status(user_record, status, text);
		}
	}

	if (text)
		g_free(text);

	return rc;
}

/* Read the undeliverable event */
static NMERR_T
handle_undeliverable_status(NMUser * user, NMEvent * event)
{
	NMERR_T rc = NM_OK;
	guint32 size = 0;
	char *guid = NULL;
	NMConn *conn;

	conn = nm_user_get_conn(user);

	/* Read the conference guid */
	rc = nm_read_all(conn, (char *) &size, sizeof(size));
	if (rc == NM_OK) {
		guid = g_new0(char, size + 1);
		rc = nm_read_all(conn, guid, size);
	}

	if (guid)
		g_free(guid);

	return rc;
}

/*******************************************************************************
 * Event API -- see header file for comments
 ******************************************************************************/

NMEvent *
nm_create_event(int type, const char *source, guint32 gmt)
{
	NMEvent *event = g_new0(NMEvent, 1);

	event->type = type;
	event->gmt = gmt;

	if (source)
		event->source = g_strdup(source);

	event->ref_count = 1;

	return event;
}

void
nm_release_event(NMEvent * event)
{
	if (event == NULL) {
		return;
	}

	if (--(event->ref_count) == 0) {

		if (event->source)
			g_free(event->source);

		if (event->conference)
			nm_release_conference(event->conference);

		if (event->user_record)
			nm_release_user_record(event->user_record);

		if (event->text)
			g_free(event->text);

		g_free(event);
	}
}


NMConference *
nm_event_get_conference(NMEvent * event)
{
	if (event)
		return event->conference;
	else
		return NULL;
}

void
nm_event_set_conference(NMEvent * event, NMConference * conference)
{
	if (event && conference) {
		nm_conference_add_ref(conference);
		event->conference = conference;
	}
}

NMUserRecord *
nm_event_get_user_record(NMEvent * event)
{
	if (event)
		return event->user_record;
	else
		return NULL;
}

void
nm_event_set_user_record(NMEvent * event, NMUserRecord * user_record)
{
	if (event && user_record) {
		nm_user_record_add_ref(user_record);
		event->user_record = user_record;
	}
}

const char *
nm_event_get_text(NMEvent * event)
{
	if (event)
		return event->text;
	else
		return NULL;
}

void
nm_event_set_text(NMEvent * event, const char *text)
{
	if (event) {
		if (text)
			event->text = g_strdup(text);
		else
			event->text = NULL;
	}
}

const char *
nm_event_get_source(NMEvent * event)
{
	if (event)
		return event->source;
	else
		return NULL;
}

int
nm_event_get_type(NMEvent * event)
{
	if (event)
		return event->type;
	else
		return -1;
}

guint32
nm_event_get_gmt(NMEvent * event)
{
	if (event)
		return event->gmt;
	else
		return (guint32)-1;
}

NMERR_T
nm_process_event(NMUser * user, int type)
{
	NMERR_T rc = NM_OK;
	guint32 size = 0;
	NMEvent *event = NULL;
	char *source = NULL;
	nm_event_cb cb;
	NMConn *conn;

	if (user == NULL)
		return NMERR_BAD_PARM;

	if (type < NMEVT_START || type > NMEVT_STOP)
		return NMERR_PROTOCOL;

	conn = nm_user_get_conn(user);

	/* Read the event source */
	rc = nm_read_all(conn, (char *) &size, sizeof(size));
	if (rc == NM_OK) {
		if (size > 0) {
			source = g_new0(char, size);

			rc = nm_read_all(conn, source, size);
		}
	}

	/* Read the event data */
	if (rc == NM_OK) {
		event = nm_create_event(type, source, time(0));

		if (event) {

			switch (type) {
			case NMEVT_STATUS_CHANGE:
				rc = handle_status_change(user, event);
				break;

			case NMEVT_RECEIVE_MESSAGE:
				rc = handle_receive_message(user, event, FALSE);
				break;

			case NMEVT_RECEIVE_AUTOREPLY:
				rc = handle_receive_message(user, event, TRUE);
				break;

			case NMEVT_USER_TYPING:
			case NMEVT_USER_NOT_TYPING:
				rc = handle_typing(user, event);
				break;

			case NMEVT_CONFERENCE_LEFT:
				rc = handle_conference_left(user, event);
				break;

			case NMEVT_CONFERENCE_CLOSED:
				rc = handle_conference_closed(user, event);
				break;

			case NMEVT_CONFERENCE_JOINED:
				rc = handle_conference_joined(user, event);
				break;

			case NMEVT_CONFERENCE_INVITE:
				rc = handle_conference_invite(user, event);
				break;

			case NMEVT_CONFERENCE_REJECT:
				rc = handle_conference_reject(user, event);
				break;

			case NMEVT_CONFERENCE_INVITE_NOTIFY:
				rc = handle_conference_invite_notify(user, event);
				break;

			case NMEVT_UNDELIVERABLE_STATUS:
				rc = handle_undeliverable_status(user, event);
				break;

			case NMEVT_INVALID_RECIPIENT:
				/* Nothing else to read, just callback */
				break;

			case NMEVT_USER_DISCONNECT:
				/* Nothing else to read, just callback */
				break;

			case NMEVT_SERVER_DISCONNECT:
				/* Nothing else to read, just callback */
				break;

			case NMEVT_RECEIVE_FILE:
			case NMEVT_CONTACT_ADD:
				/* Safely ignored for now */
				break;

			default:
				gaim_debug(GAIM_DEBUG_INFO, "novell",
						   "Unknown event %d received.\n", type);
				rc = NMERR_PROTOCOL;
				break;
			}
		}
	}

	if (rc == (NMERR_T)-1) {
		/* -1 means that we are not ready to callback yet. */
		rc = NM_OK;
	} else if (rc == NM_OK && (cb = nm_user_get_event_callback(user))) {

		cb(user, event);

		if (event)
			nm_release_event(event);
	} else {
		if (event)
			nm_release_event(event);
	}

	/* Cleanup */
	if (source)
		g_free(source);

	return rc;
}

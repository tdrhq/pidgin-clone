/**
 * The QQ2003C protocol plugin
 *
 * for gaim
 *
 * Copyright (C) 2004 Puzzlebird
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

// START OF FILE
/*****************************************************************************/
#include "connection.h"		// GaimConnection
#include "debug.h"		// gaim_debug
#include "internal.h"		// _("get_text")
#include "notify.h"		// gaim_notify
#include "prefs.h"		// gaim_prefs_get_bool
#include "request.h"		// gaim_request_action

#include "header_info.h"	// cmd alias
#include "qq_proxy.h"		// qq_proxy_write
#include "sendqueue.h"

#define QQ_RESEND_MAX               5	// max resend per packet

typedef struct _gc_and_packet gc_and_packet;

struct _gc_and_packet {
	GaimConnection *gc;
	qq_sendpacket *packet;
};

/*****************************************************************************/
// Remove a packet with send_seq from sendqueue
void qq_sendqueue_remove(qq_data * qd, guint16 send_seq)
{
	GList *list;
	qq_sendpacket *p;

	g_return_if_fail(qd != NULL);

	list = qd->sendqueue;
	while (list != NULL) {
		p = (qq_sendpacket *) (list->data);
		if (p->send_seq == send_seq) {
			qd->sendqueue = g_list_remove(qd->sendqueue, p);
			g_free(p->buf);
			g_free(p);
			break;
		}
		list = list->next;
	}			// while
}				// qq_sendqueue_remove

/*****************************************************************************/
// clean up sendqueue and free all contents
void qq_sendqueue_free(qq_data * qd)
{
	qq_sendpacket *p;
	gint i;

	i = 0;
	while (qd->sendqueue != NULL) {
		p = (qq_sendpacket *) (qd->sendqueue->data);
		qd->sendqueue = g_list_remove(qd->sendqueue, p);
		g_free(p->buf);
		g_free(p);
		i++;
	}
	gaim_debug(GAIM_DEBUG_INFO, "QQ", "%d packets in sendqueue are freed!\n", i);
}				// qq_sendqueue_free

/*****************************************************************************/
// packet lost, agree to send again, (and will NOT prompt again)
// it is removed only when ack-ed by server
static void _qq_send_again(gc_and_packet * gp)
{
	GaimConnection *gc;
	qq_data *qd;
	qq_sendpacket *packet;
	GList *list;

	g_return_if_fail(gp != NULL && gp->gc != NULL && gp->packet != NULL);
	g_return_if_fail(gp->gc->proto_data != NULL);

	gc = gp->gc;
	packet = gp->packet;
	qd = (qq_data *) gc->proto_data;

	list = g_list_find(qd->sendqueue, packet);
	if (list != NULL) {
		packet->resend_times = 0;
		packet->sendtime = time(NULL);
		qq_proxy_write(qd, packet->buf, packet->len);
	}
	g_free(gp);
}				// _qq_send_again

/*****************************************************************************/
// packet lost, do not send again
static void _qq_send_cancel(gc_and_packet * gp)
{
	GaimConnection *gc;
	qq_data *qd;
	qq_sendpacket *packet;
	GList *list;

	g_return_if_fail(gp != NULL && gp->gc != NULL && gp->packet != NULL);
	g_return_if_fail(gp->gc->proto_data != NULL);

	gc = gp->gc;
	packet = gp->packet;
	qd = (qq_data *) gc->proto_data;

	list = g_list_find(qd->sendqueue, packet);
	if (list != NULL)
		qq_sendqueue_remove(qd, packet->send_seq);

	g_free(gp);
}				// _qq_send_cancel

/*****************************************************************************/
gboolean qq_sendqueue_timeout_callback(gpointer data)
{
	GaimConnection *gc;
	qq_data *qd;
	GList *list;
	qq_sendpacket *p;
	gc_and_packet *gp;
	time_t now;
	gint wait_time;
	gboolean need_action;

	gc = (GaimConnection *) data;
	qd = (qq_data *) gc->proto_data;
	now = time(NULL);
	list = qd->sendqueue;

	// empty queue, return TRUE so that timeout continues functioning
	if (qd->sendqueue == NULL)
		return TRUE;

	while (list != NULL) {	// remove all packet whose resend_times == -1
		p = (qq_sendpacket *) list->data;
		if (p->resend_times == -1) {	// to remove
			qd->sendqueue = g_list_remove(qd->sendqueue, p);
			g_free(p->buf);
			g_free(p);
			list = qd->sendqueue;
		} else
			list = list->next;
	}			// while list

	list = qd->sendqueue;
	while (list != NULL) {
		p = (qq_sendpacket *) list->data;
		if (p->resend_times >= QQ_RESEND_MAX) {
			if (p->resend_times == QQ_RESEND_MAX) {	// reach max
				switch (p->cmd) {
				case QQ_CMD_KEEP_ALIVE:
					if (qd->logged_in) {
						gaim_debug(GAIM_DEBUG_ERROR, "QQ", "Connection lost!\n");
						gaim_connection_error(gc, _("Connection lost!"));
						qd->logged_in = FALSE;
					}	// if logged_in
					p->resend_times = -1;
					break;
				case QQ_CMD_LOGIN:
					if (!qd->logged_in)	// cancel logging progress
						gaim_connection_error(gc, _("Login failed, no reply!"));
					p->resend_times = -1;
					break;
				case QQ_CMD_UPDATE_INFO:
					gaim_notify_error(gc, NULL,
							  _("Connection timeout!"), _("User info is not updated"));
					p->resend_times = -1;
					break;
				default:{
						need_action =
						    gaim_prefs_get_bool("/plugins/prpl/qq/prompt_for_missing_packet");
						if (!need_action)
							p->resend_times = -1;	// it will be removed next time
						else {	// prompt for action
							gp = g_new0(gc_and_packet, 1);
							gp->gc = gc;
							gp->packet = p;
							gaim_request_action
							    (gc, NULL,
							     _
							     ("Send packet"),
							     _
							     ("Packets lost, send again?"),
							     0, gp, 2,
							     _("Send"),
							     G_CALLBACK
							     (_qq_send_again),
							     _("Cancel"), G_CALLBACK(_qq_send_cancel));
							p->resend_times++;	// will send once more, but only once
						}	// if !need_action
					}	// default
				}	// switch
			}	// resend_times == QQ_RESEND_MAX
		} else {	// resend_times < QQ_RESEND_MAX, so sent it again
			wait_time = (gint) (QQ_SENDQUEUE_TIMEOUT / 1000);
			if (difftime(now, p->sendtime) > (wait_time * (p->resend_times + 1))) {
				qq_proxy_write(qd, p->buf, p->len);
				p->resend_times++;
				gaim_debug(GAIM_DEBUG_INFO,
					   "QQ", "<<< [%05d] send again for %d times!\n", p->send_seq, p->resend_times);
			}	// if difftime
		}		// if resend_times >= QQ_RESEND_MAX            
		list = list->next;
	}			// whiile list
	return TRUE;		// if we return FALSE, the timeout callback stops functioning
}				// qq_sendqueue_timeout_callback

/*****************************************************************************/
// END OF FILE

/**
* @file gntlog.h GNT Log API
* @ingroup finch
*
* finch
*
* Finch is the legal property of its developers, whose names are too numerous
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

#ifndef _GNT_LOG_H
#define _GNT_LOG_H

/* Finch includes */
#include "finch.h"

/* libgnt includes */
#include "gntwidget.h"
#include "gnttree.h"

/* libpurple includes */
#include "log.h"
#include "account.h"

typedef struct _FinchLogViewer FinchLogViewer;

/**
 * A Gnt Log Viewer.  You can look at logs with it.
 */
struct _FinchLogViewer {
	GList *logs;                   	/* The list of logs viewed in this viewer   */
	GntTree						*treestore;  	/* The treestore containing said logs       */

	GntWidget        	*window;     	/* The viewer's window                      */
	GntWidget        	*treeview;    /* The tree representing said treestore 		*/
	GntWidget        	*tv;    		  /* The textview to display said logs        */
	GntWidget        	*label;       /* The label at the top of the log viewer   */
	GntWidget        	*size_label;  /* The label with total log size           	*/
	GntWidget        	*entry;       /* The search entry, in which search terms  */
	                               	/* are entered                              */

	PurpleLogReadFlags flags;       /* The most recently used log flags         */
	char             	*search;      /* The string currently being searched for  */

};


void finch_log_test(void); /* Here until I finish making the log_viewer look right */

void finch_log_show(PurpleLogType type, gchar *screenname, PurpleAccount *account);
void finch_log_show_contact(PurpleContact *contact);

void finch_syslog_show(void);

/**************************************************************************/
/** @name Gnt Log Subsystem                                              */
/**************************************************************************/
/*@{*/

/**
 * Initializes the Gnt log subsystem.
 */
void finch_log_init(void);

/**
 * Returns the Gnt log subsystem handle.
 *
 * @return The Gnt log subsystem handle.
 */
void *finch_log_get_handle(void);

/**
 * Uninitializes the Gnt log subsystem.
 */
void finch_log_uninit(void);

/*@}*/
#endif /* _GNT_LOG_H */

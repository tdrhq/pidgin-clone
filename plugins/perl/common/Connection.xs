#include "module.h"

/* TODO
;

;
;
;
;;;;;
;;;



*/

/**********************XS Code for Conversation.xs*********************************/
MODULE = Gaim::Connection  PACKAGE = Gaim::GTK::Connection  PREFIX = gaim_gtk_connections_
PROTOTYPES: ENABLE

Gaim::Connection::UiOps
gaim_gtk_connections_get_ui_ops()

MODULE = Gaim::Connection  PACKAGE = Gaim::Connection  PREFIX = gaim_connections_
PROTOTYPES: ENABLE

Gaim::Account
gaim_connection_get_account(gc)
	Gaim::Connection gc

const char *
gaim_connection_get_password(gc)
	Gaim::Connection gc

const char *
gaim_connection_get_display_name(gc)
	Gaim::Connection gc

void 
gaim_connection_notice(gc, text)
	Gaim::Connection gc
	const char *text

void 
gaim_connection_error(gc, reason)
	Gaim::Connection gc
	const char *reason

void 
gaim_connections_disconnect_all()

void
gaim_connections_get_all()
PREINIT:
	GList *l;
PPCODE:
	for (l = gaim_connections_get_all(); l != NULL; l = l->next) {
		XPUSHs(sv_2mortal(gaim_perl_bless_object(l->data, "Gaim::ListEntry")));
	}


void
gaim_connections_get_connecting()
PREINIT:
	GList *l;
PPCODE:
	for (l = gaim_connections_get_connecting(); l != NULL; l = l->next) {
		XPUSHs(sv_2mortal(gaim_perl_bless_object(l->data, "Gaim::ListEntry")));
	}

void 
gaim_connections_set_ui_ops(ops)
	Gaim::Connection::UiOps ops

Gaim::Connection::UiOps
gaim_connections_get_ui_ops()

void 
gaim_connections_init()

void 
gaim_connections_uninit()

void *
gaim_connections_get_handle()

void 
gaim_connection_destroy(gc)
	Gaim::Connection gc

void 
gaim_connection_set_state(gc, state)
	Gaim::Connection gc
	Gaim::ConnectionState state

void 
gaim_connection_set_account(gc, account)
	Gaim::Connection gc
	Gaim::Account account

void 
gaim_connection_set_display_name(gc, name)
	Gaim::Connection gc
	const char *name

Gaim::ConnectionState 
gaim_connection_get_state(gc)
	Gaim::Connection gc


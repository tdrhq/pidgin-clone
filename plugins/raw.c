#define GAIM_PLUGINS
#include "gaim.h"
#include "prpl.h"
#ifdef MAX
#undef MAX
#undef MIN
#endif
#include "jabber/jabber.h"

static GtkWidget *window = NULL;
static GtkWidget *optmenu = NULL;
static struct gaim_connection *gc = NULL;
static GModule *me = NULL;

/* this is an evil hack.
 * gc->proto_data for Jabber connections can be cast to a jconn *.
 * gc->proto_data for MSN, TOC, and IRC connections can be cast to an int *.
 */

char *name()
{
	return "Raw";
}

char *description()
{
	return "Lets you send raw XML to Jabber, or raw commands to MSN, IRC, and TOC."
		" Not very useful except for debugging. Hit 'enter' in the entry to send."
		" Watch the debug window.";
}

static int goodbye()
{
	gaim_plugin_unload(me);
	return FALSE;
}

static void send_it(GtkEntry *entry)
{
	char *txt;
	if (!gc) return;
	txt = gtk_entry_get_text(entry);
	switch (gc->protocol) {
		case PROTO_TOC:
			{
				int *a = (int *)gc->proto_data;
				unsigned short seqno = htons(a[1]++ & 0xffff);
				unsigned short len = htons(strlen(txt) + 1);
				write(*a, "*\002", 2);
				write(*a, &seqno, 2);
				write(*a, &len, 2);
				write(*a, txt, ntohs(len));
				debug_printf("TOC C: %s\n", txt);
			}
			break;
		case PROTO_MSN:
			write(*(int *)gc->proto_data, txt, strlen(txt));
			write(*(int *)gc->proto_data, "\r\n", 2);
			debug_printf("MSN C: %s\n", txt);
			break;
		case PROTO_IRC:
			write(*(int *)gc->proto_data, txt, strlen(txt));
			write(*(int *)gc->proto_data, "\r\n", 2);
			debug_printf("IRC C: %s\n", txt);
			break;
		case PROTO_JABBER:
			jab_send_raw(*(jconn *)gc->proto_data, txt);
			break;
	}
	gtk_entry_set_text(entry, "");
}

static void set_gc(gpointer d, struct gaim_connection *c)
{
	gc = c;
}

static void redo_optmenu(struct gaim_connection *arg, gpointer x)
{
	GtkWidget *menu;
	GSList *g = connections;
	struct gaim_connection *c;

	menu = gtk_menu_new();
	gc = NULL;

	while (g) {
		char buf[256];
		GtkWidget *opt;
		c = (struct gaim_connection *)g->data;
		g = g->next;
		if (x && c == arg)
			continue;
		if (c->protocol != PROTO_TOC && c->protocol != PROTO_MSN &&
		    c->protocol != PROTO_IRC && c->protocol != PROTO_JABBER)
			continue;
		if (!gc)
			gc = c;
		g_snprintf(buf, sizeof buf, "%s (%s)", c->username, (*c->prpl->name)());
		opt = gtk_menu_item_new_with_label(buf);
		gtk_signal_connect(GTK_OBJECT(opt), "activate", GTK_SIGNAL_FUNC(set_gc), c);
		gtk_widget_show(opt);
		gtk_menu_append(GTK_MENU(menu), opt);
	}

	gtk_option_menu_remove_menu(GTK_OPTION_MENU(optmenu));
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optmenu), menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optmenu), 0);
}

char *gaim_plugin_init(GModule *h)
{
	GtkWidget *hbox;
	GtkWidget *entry;

	me = h;

	gaim_signal_connect(h, event_signon, redo_optmenu, NULL);
	gaim_signal_connect(h, event_signoff, redo_optmenu, me);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(goodbye), NULL);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), hbox);

	optmenu = gtk_option_menu_new();
	gtk_box_pack_start(GTK_BOX(hbox), optmenu, FALSE, FALSE, 5);

	redo_optmenu(NULL, NULL);

	entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 5);
	gtk_signal_connect(GTK_OBJECT(entry), "activate", GTK_SIGNAL_FUNC(send_it), NULL);

	gtk_widget_show_all(window);

	return NULL;
}

void gaim_plugin_remove()
{
	if (window)
		gtk_widget_destroy(window);
	window = NULL;
	me = NULL;
}

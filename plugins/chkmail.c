/* This is some funky code.  It is still being developed by Rob Flynn - rob@linuxpimps.com
 * I recommend not using this code right now. :)
*/

#define GAIM_PLUGINS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "gaim.h"

char username[] = "";
char password[] = "";
char mailhost[] = "";
int mailport = 110;

static void *handle = NULL;
extern GtkWidget *blist;
extern GtkWidget *buddies;

GList *tmp;
int lastnum = 0;
int orig = 0;
int mytimer;

void update_mail();

int num_msgs()
{
        struct in_addr *sin;
        char recv[1024];
        char command[256];
        int fd;
        int num = 0;
        int step = 0;
        int len;

        sin = (struct in_addr *)get_address(mailhost);
        fd = connect_address(sin->s_addr, mailport);
	while ((len = read(fd, recv, 1023))>0) {
		recv[len] = 0;
                if (!strncmp(recv, "-ERR", strlen("-ERR"))) { step = 4; break; 
                } else if (!strncmp(recv, "+OK", strlen("+OK"))) {
                        if (step == 3) {
                                if (sscanf(recv, "+OK %d %d\n", &num, &step) != 2)
                                        break;
                                g_snprintf(command, sizeof(command), "QUIT\n");
                                write(fd, command, strlen(command));
				close(fd);
                                return num;
                        }

                        if (step == 0) {
                                g_snprintf(command, sizeof(command), "USER %s\n", username);
                                write(fd, command, strlen(command));
                                step = 1;
                        } else if (step == 1) {
                                g_snprintf(command, sizeof(command), "PASS %s\n", password);
                                write(fd, command, strlen(command));
                                step = 2;
                        } else if (step == 2) {
                                g_snprintf(command, sizeof(command), "STAT\n");
                                write(fd, command, strlen(command));
                                step = 3;
                        }
                }
        }
        close(fd);
	return 0;
}

void destroy_mail_list()
{
        GList *list;
        GtkWidget *w;

        list = GTK_TREE(buddies)->children;

        while (list) {
                w = (GtkWidget *)list->data;
                if (!strcmp(GTK_LABEL(GTK_BIN(w)->child)->label, "Mail Server")) {
                        gtk_tree_remove_items(GTK_TREE(buddies), list);
                        if (!list)
                                break;
                }
                list = list->next;
        }
}


void setup_mail_list()
{
	GList *list;
	GtkWidget *w;
	GtkWidget *item;
	GtkWidget *tree;
	gchar *buf;

	list = GTK_TREE(buddies)->children;

	while (list) {
		w = (GtkWidget *)list->data;
		if (!strcmp(GTK_LABEL(GTK_BIN(w)->child)->label, "Mail Server")) {
			gtk_tree_remove_items(GTK_TREE(buddies), list);
			if (!list)
				break;
		}
		list = list->next;
	}

	item = gtk_tree_item_new_with_label("Mail Server");
	tree = gtk_tree_new();
	gtk_widget_show(item);
	gtk_widget_show(tree);
	gtk_tree_append(GTK_TREE(buddies), item);
	gtk_tree_item_set_subtree(GTK_TREE_ITEM(item), tree);
	gtk_tree_item_expand(GTK_TREE_ITEM(item));

	buf = g_malloc(BUF_LONG);

	g_snprintf(buf, BUF_LONG, "%s (%d/%d)", mailhost, lastnum - orig, orig);
	item = gtk_tree_item_new_with_label(buf);
	g_free(buf);

	gtk_tree_append(GTK_TREE(tree), item);
	gtk_widget_show(item);
}
	
void gaim_plugin_init(void *h) {
	handle = h;
	tmp = gtk_container_children(GTK_CONTAINER(blist));

	orig = num_msgs();
	lastnum = orig;

	gaim_signal_connect(handle, event_blist_update, setup_mail_list, NULL);
	setup_mail_list();
	
	mytimer = gtk_timeout_add(30000, (GtkFunction)update_mail, NULL);
}

void update_mail () {
	int newnum;

	gtk_timeout_remove(mytimer);

	newnum = num_msgs();

	if ( (newnum >= lastnum) && (newnum > 0)) {
		newnum = newnum - orig;
	} else {
		newnum = 0;
	}

	if (newnum < lastnum) {
		orig = 0;
	}

	lastnum = newnum;
	mytimer = gtk_timeout_add(30000, (GtkFunction)update_mail, NULL);
	setup_mail_list();
}


void gaim_plugin_remove() {
	gtk_timeout_remove(mytimer);
	destroy_mail_list();
	handle = NULL;
}

char *name() {
	return "Check Mail";
}

char *description() {
	return "Check email every X seconds.\n";
}

#include "config.h"
#include "gaim.h"
#include "applet.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

static void *handle;
static int check;
static time_t mtime;

static void init_file();
static void check_file();

extern void do_quit();

/* parse char * as if were word array */
char *getarg(char *, int, int);

/* go through file and run any commands */
void run_commands() {
	struct stat finfo;
	char filename[256];
	char buffer[1024];
	char *command, *arg1, *arg2;
	FILE *file;

	sprintf(filename, "%s/.gaim/control", getenv("HOME"));

	file = fopen(filename, "r+");
	while (fgets(buffer, sizeof buffer, file)) {
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = 0;
		debug_printf("read: %s\n", buffer);
		command = getarg(buffer, 0, 0);
		if (!strncasecmp(command, "signon", 6)) {
			struct aim_user *u = NULL;
			GSList *userlist = aim_users;
			arg1 = getarg(buffer, 1, 1);
			if (arg1) {
				while (userlist) {
					struct aim_user *current = userlist->data;
					if (!strcmp(current->username, arg1)) {
						u = current;
						break;
					}
					userlist = userlist->next;
				}
				free(arg1);
			}
			if (u) /* username found */
				serv_login(u);
		} else if (!strncasecmp(command, "signoff", 7)) {
			struct gaim_connection *gc = NULL;
			GSList *c = connections;
			arg1 = getarg(buffer, 1, 1);
			while (arg1 && c) {
				gc = c->data;
				if (!strcmp(gc->username, arg1)) {
					break;
				}
				gc = NULL;
				c = c->next;
			}
			if (gc)
				signoff(gc);
			else if (!arg1)
				signoff_all(NULL, NULL);
			free(arg1);
		} else if (!strncasecmp(command, "send", 4)) {
			struct conversation *c;
			arg1 = getarg(buffer, 1, 0);
			arg2 = getarg(buffer, 2, 1);
			c = find_conversation(arg1);
			if (!c) c = new_conversation(arg1);
			write_to_conv(c, arg2, WFLAG_SEND, NULL, time(NULL), -1);
			serv_send_im(c->gc, arg1, arg2, -1, 0);
			free(arg1);
			free(arg2);
		} else if (!strncasecmp(command, "away", 4)) {
			struct away_message a;
			arg1 = getarg(buffer, 1, 1);
			snprintf(a.message, 2048, "%s", arg1);
			a.name[0] = 0;
			do_away_message(NULL, &a);
			free(arg1);
		} else if (!strncasecmp(command, "hide", 4)) {
			hide_buddy_list();
		} else if (!strncasecmp(command, "unhide", 6)) {
			unhide_buddy_list();
		} else if (!strncasecmp(command, "back", 4)) {
			do_im_back();
		} else if (!strncasecmp(command, "quit", 4)) {
			do_quit();
		}
		free(command);
	}

	fclose(file);

	if (stat (filename, &finfo) != 0)
		return;
	mtime = finfo.st_mtime;
}

char *gaim_plugin_init(GModule *h) {
	handle = h;
	init_file();
	check = gtk_timeout_add(5000, (GtkFunction)check_file, NULL);
	return NULL;
}

void gaim_plugin_remove() {
	gtk_timeout_remove(check);
}

char *name() {
	return "Gaim File Control";
}

char *description() {
	return "Allows you to control gaim by entering commands in a file.";
}

/* check to see if the size of the file is > 0. if so, run commands */
void init_file() {
	/* most of this was taken from Bash v2.04 by the FSF */
	struct stat finfo;
	char file[256];

	sprintf(file, "%s/.gaim/control", getenv("HOME"));

	if ((stat (file, &finfo) == 0) && (finfo.st_size > 0))
		run_commands();
}

/* check to see if we need to run commands from the file */
void check_file() {
	/* most of this was taken from Bash v2.04 by the FSF */
	struct stat finfo;
	char file[256];

	sprintf(file, "%s/.gaim/control", getenv("HOME"));

	if ((stat (file, &finfo) == 0) && (finfo.st_size > 0))
		if (mtime != finfo.st_mtime) {
			debug_printf("control changed, checking\n");
			run_commands();
		}
}

char *getarg(char *line, int which, int remain) {
	char *arr;
	char *val;
	int count = -1;
	int i;
	int state = 0;

	for (i = 0; i < strlen(line) && count < which; i++) {
		switch (state) {
		case 0: /* in whitespace, expecting word */
			if (isalnum(line[i])) {
				count++;
				state = 1;
			}
			break;
		case 1: /* inside word, waiting for whitespace */
			if (isspace(line[i])) {
				state = 0;
			}
			break;
		}
	}

	arr = strdup(&line[i - 1]);
	if (remain)
		return arr;

	for (i = 0; i < strlen(arr) && isalnum(arr[i]); i++);
	arr[i] = 0;
	val = strdup(arr);
	arr[i] = ' ';
	free(arr);
	return val;
}

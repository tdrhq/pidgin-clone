/* $Id: libgg.c 4216 2002-11-29 12:48:01Z lschiere $ */

/*
 *  (C) Copyright 2001 Wojtek Kaniewski <wojtekka@irc.pl>,
 *                     Robert J. Wo�ny <speedy@ziew.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License Version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <sys/types.h>
#ifndef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <netdb.h>
#include <pwd.h>
#else
#include <fcntl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#ifndef _AIX
#  include <string.h>
#endif
#include <stdarg.h>
#include <time.h>
#ifdef sun
  #include <sys/filio.h>
#endif
#include <glib.h>
#if G_BYTE_ORDER == G_BIG_ENDIAN
#  define WORDS_BIGENDIAN 1
#endif
#include "libgg.h"
#include "config.h"
#include "gaim.h"
#include "proxy.h"

#ifdef _WIN32
#include "win32dep.h"
#endif

int gg_debug_level = (GG_DEBUG_NET | GG_DEBUG_TRAFFIC | GG_DEBUG_DUMP | GG_DEBUG_FUNCTION | GG_DEBUG_MISC);
int gg_http_use_proxy = 0;
int gg_http_proxy_port = 0;
char *gg_http_proxy_host = NULL;

/* temp -Herman */
static int ping_outstanding = 0;

#ifndef lint 

static char rcsid[]
#ifdef __GNUC__
__attribute__ ((unused))
#endif
= "$Id: libgg.c 4216 2002-11-29 12:48:01Z lschiere $";

#endif 

/*
 * fix32() // funkcja wewn�trzna
 *
 * dla maszyn big-endianowych zamienia kolejno�� bajt�w w ,,long''ach.
 */
static inline unsigned long fix32(unsigned long x)
{
#ifndef WORDS_BIGENDIAN
	return x;
#else
	return (unsigned long)
		(((x & (unsigned long) 0x000000ffU) << 24) |
                 ((x & (unsigned long) 0x0000ff00U) << 8) |
                 ((x & (unsigned long) 0x00ff0000U) >> 8) |
                 ((x & (unsigned long) 0xff000000U) >> 24));
#endif		
}

/*
 * fix16() // funkcja wewn�trzna
 *
 * dla maszyn big-endianowych zamienia kolejno�� bajt�w w ,,short''ach.
 */
static inline unsigned short fix16(unsigned short x)
{
#ifndef WORDS_BIGENDIAN
	return x;
#else
	return (unsigned short)
		(((x & (unsigned short) 0x00ffU) << 8) |
                 ((x & (unsigned short) 0xff00U) >> 8));
#endif
}

#ifndef _WIN32
/*
 * gg_resolve() // funkcja wewn�trzna
 *
 * tworzy pipe'y, forkuje si� i w drugim procesie zaczyna resolvowa� 
 * podanego hosta. zapisuje w sesji deskryptor pipe'u. je�li co� tam
 * b�dzie gotowego, znaczy, �e mo�na wczyta� ,,struct in_addr''. je�li
 * nie znajdzie, zwraca INADDR_NONE.
 *
 *  - fd - wska�nik gdzie wrzuci� deskryptor,
 *  - pid - gdzie wrzuci� pid dzieciaka,
 *  - hostname - nazwa hosta do zresolvowania.
 *
 * zwraca 0 je�li uda�o si� odpali� proces lub -1 w przypadku b��du.
 */
int gg_resolve(int *fd, int *pid, char *hostname)
{
	int pipes[2], res;
	struct in_addr a;

	gg_debug(GG_DEBUG_FUNCTION, "** gg_resolve(..., \"%s\");\n", hostname);
	
	if (!fd | !pid) {
		errno = EFAULT;
		return -1;
	}

	if (pipe(pipes) == -1)
		return -1;

	if ((res = fork()) == -1)
		return -1;

	if (!res) {
		if ((a.s_addr = inet_addr(hostname)) == INADDR_NONE) {
			struct hostent *he;
		
			if (!(he = gethostbyname(hostname)))
				a.s_addr = INADDR_NONE;
			else
				memcpy((char*) &a, he->h_addr, sizeof(a));
		}

		write(pipes[1], &a, sizeof(a));

		exit(0);
	}

	close(pipes[1]);

	*fd = pipes[0];
	*pid = res;

	return 0;
}
#endif /*!_WIN32*/

/*
 * gg_recv_packet() // funkcja wewn�trzna
 *
 * odbiera jeden pakiet gg i zwraca wska�nik do niego. pami�� po nim
 * wypada�oby uwolni�.
 *
 *  - sock - po��czony socket.
 *
 * je�li wyst�pi� b��d, zwraca NULL. reszta w errno.
 */
static void *gg_recv_packet(struct gg_session *sess)
{
	struct gg_header h;
	char *buf = NULL;
	int ret = 0, offset, size = 0;
	int sizeh = sizeof(struct gg_header);

	gg_debug(GG_DEBUG_FUNCTION, "** gg_recv_packet(...);\n");
	
	if (!sess) {
		errno = EFAULT;
		return NULL;
	}

	if (sess->recv_left < 1) {
		while (ret != sizeh) {
			ret = read(sess->fd, &h, sizeh);
			gg_debug(GG_DEBUG_MISC, "-- header recv(..., %d) = %d\n", sizeh, ret);
			if (ret < sizeh) {
				if (errno != EINTR) {
					gg_debug(GG_DEBUG_MISC, "-- errno = %d (%s)\n", errno, strerror(errno));
					return NULL;
				}
			}
		}

		h.type = fix32(h.type);
		h.length = fix32(h.length);
	} else {
		memcpy(&h, sess->recv_buf, sizeh);
	}

	/* jakie� sensowne limity na rozmiar pakietu */
	if (h.length < 0 || h.length > 65535) {
		gg_debug(GG_DEBUG_MISC, "-- invalid packet length (%d)\n", h.length);
		errno = ERANGE;
		return NULL;
	}

	if (sess->recv_left > 0) {
		gg_debug(GG_DEBUG_MISC, "-- resuming last gg_recv_packet()\n");
		size = sess->recv_left;
		offset = sess->recv_done;
		buf = sess->recv_buf;
	} else {
		if (!(buf = malloc(sizeh + h.length + 1))) {
			gg_debug(GG_DEBUG_MISC, "-- not enough memory\n");
			return NULL;
		}

		memcpy(buf, &h, sizeh);

		offset = 0;
		size = h.length;
	}

	while (size > 0) {
		ret = read(sess->fd, buf + sizeh + offset, size);
		gg_debug(GG_DEBUG_MISC, "-- body recv(..., %d) = %d\n", size, ret);
		if (ret > -1 && ret <= size) {
			offset += ret;
			size -= ret;
		} else if (ret == -1) {	
			gg_debug(GG_DEBUG_MISC, "-- errno = %d (%s)\n", errno, strerror(errno));
			if (errno == EAGAIN) {
				gg_debug(GG_DEBUG_MISC, "-- %d bytes received, %d left\n", offset, size);
				sess->recv_buf = buf;
				sess->recv_left = size;
				sess->recv_done = offset;
				return NULL;
			}
			if (errno != EINTR) {
//				errno = EINVAL;
				free(buf);
				return NULL;
			}
		}
	}

	sess->recv_left = 0;

	if ((gg_debug_level & GG_DEBUG_DUMP)) {
		int i;

		gg_debug(GG_DEBUG_DUMP, ">> received packet (type=%.2x):", h.type);
		for (i = 0; i < sizeh + h.length; i++) 
			gg_debug(GG_DEBUG_DUMP, " %.2x", (unsigned char) buf[i]);
		gg_debug(GG_DEBUG_DUMP, "\n");
	}

	return buf;
}

/*
 * gg_send_packet() // funkcja wewn�trzna
 *
 * konstruuje pakiet i wysy�a go w do serwera.
 *
 *  - sock - po��czony socket,
 *  - type - typ pakietu,
 *  - packet - wska�nik do struktury pakietu,
 *  - length - d�ugo�� struktury pakietu,
 *  - payload - dodatkowy tekst doklejany do pakietu (np. wiadomo��),
 *  - payload_length - d�ugo�� dodatkowego tekstu.
 *
 * je�li posz�o dobrze, zwraca 0. w przypadku b��du -1. je�li errno=ENOMEM,
 * zabrak�o pami�ci. inaczej by� b��d przy wysy�aniu pakietu. dla errno=0
 * nie wys�ano ca�ego pakietu.
 */
static int gg_send_packet(int sock, int type, void *packet, int length, void *payload, int payload_length)
{
	struct gg_header *h;
	int res, plen;
	char *tmp;

	gg_debug(GG_DEBUG_FUNCTION, "** gg_send_packet(0x%.2x, %d, %d);\n", type, length, payload_length);
	
	if (length < 0 || payload_length < 0) {
		gg_debug(GG_DEBUG_MISC, "-- invalid packet/payload length\n");
		errno = ERANGE;
		return -1;
	}

	if (!(tmp = malloc(sizeof(struct gg_header) + length + payload_length))) {
		gg_debug(GG_DEBUG_MISC, "-- not enough memory\n");
		return -1;
	}

	h = (struct gg_header*) tmp;
	h->type = fix32(type);
	h->length = fix32(length + payload_length);

	if (packet)
		memcpy(tmp + sizeof(struct gg_header), packet, length);
	if (payload)
		memcpy(tmp + sizeof(struct gg_header) + length, payload, payload_length);

	if ((gg_debug_level & GG_DEBUG_DUMP)) {
		int i;

		gg_debug(GG_DEBUG_DUMP, "%%%% sending packet (type=%.2x):", fix32(h->type));
		for (i = 0; i < sizeof(struct gg_header) + fix32(h->length); i++)
			gg_debug(GG_DEBUG_DUMP, " %.2x", (unsigned char) tmp[i]);
		gg_debug(GG_DEBUG_DUMP, "\n");
	}

	plen = sizeof(struct gg_header) + length + payload_length;
	if ((res = write(sock, tmp, plen)) < plen) {
		gg_debug(GG_DEBUG_MISC, "-- write() failed. res = %d, errno = %d (%s)\n", res, errno, strerror(errno));
		free(tmp);
		return -1;
	}

	free(tmp);	
	return 0;
}

#ifndef _WIN32
/*
 * gg_login()
 *
 * rozpoczyna procedur� ��czenia si� z serwerem. reszt� obs�guje si� przez
 * gg_watch_event.
 *
 *  - uin - numerek usera,
 *  - password - jego hase�ko,
 *  - async - ma by� asynchronicznie?
 *
 * UWAGA! program musi obs�u�y� SIGCHLD, je�li ��czy si� asynchronicznie,
 * �eby zrobi� pogrzeb zmar�emu procesowi resolvera.
 *
 * w przypadku b��du zwraca NULL, je�li idzie dobrze (async) albo posz�o
 * dobrze (sync), zwr�ci wska�nik do zaalokowanej struktury `gg_session'.
 */
struct gg_session *gg_login(uin_t uin, char *password, int async)
{
	struct gg_session *sess;
	char *hostname;
	int port;

	gg_debug(GG_DEBUG_FUNCTION, "** gg_login(%u, \"...\", %d);\n", uin, async);

	if (!(sess = malloc(sizeof(*sess))))
		return NULL;

	sess->uin = uin;
	if (!(sess->password = strdup(password))) {
		free(sess);
		return NULL;
	}
	sess->state = GG_STATE_RESOLVING;
	sess->check = GG_CHECK_READ;
	sess->async = async;
	sess->seq = 0;
	sess->recv_left = 0;
	sess->last_pong = 0;
	sess->server_ip = 0;
	sess->initial_status = 0;
        sess->type = GG_SESSION_GG;
	
	if (gg_http_use_proxy) {
		hostname = gg_http_proxy_host;
		port = gg_http_proxy_port;
	} else {
		hostname = GG_APPMSG_HOST;
		port = GG_APPMSG_PORT;
	};

	if (async) {
		if (gg_resolve(&sess->fd, &sess->pid, hostname)) {
			gg_debug(GG_DEBUG_MISC, "-- resolving failed\n");
			free(sess);
			return NULL;
		}
	} else {
		struct in_addr a;

		if ((a.s_addr = inet_addr(hostname)) == INADDR_NONE) {
			struct hostent *he;
	
			if (!(he = gethostbyname(hostname))) {
				gg_debug(GG_DEBUG_MISC, "-- host %s not found\n", hostname);
				free(sess);
				return NULL;
			} else
				memcpy((char*) &a, he->h_addr, sizeof(a));
		}

		if (!(sess->fd = gg_connect(&a, port, 0)) == -1) {
			gg_debug(GG_DEBUG_MISC, "-- connection failed\n");
			free(sess);
			return NULL;
		}

		sess->state = GG_STATE_CONNECTING;

		while (sess->state != GG_STATE_CONNECTED) {
			struct gg_event *e;

			if (!(e = gg_watch_fd(sess))) {
				gg_debug(GG_DEBUG_MISC, "-- some nasty error in gg_watch_fd()\n");
				free(sess);
				return NULL;
			}

			if (e->type == GG_EVENT_CONN_FAILED) {
				errno = EACCES;
				gg_debug(GG_DEBUG_MISC, "-- could not login\n");
				gg_free_event(e);
				free(sess);
				return NULL;
			}

			gg_free_event(e);
		}
	}

	return sess;
}
#endif /*!_WIN32*/

/* 
 * gg_free_session()
 *
 * zwalnia pami�� zajmowan� przez opis sesji.
 *
 *  - sess - opis sesji.
 *
 * nie zwraca niczego, bo i po co?
 */
void gg_free_session(struct gg_session *sess)
{
	if (!sess)
		return;

	free(sess->password);
	free(sess);
}

/*
 * gg_change_status()
 *
 * zmienia status u�ytkownika. przydatne do /away i /busy oraz /quit.
 *
 *  - sess - opis sesji,
 *  - status - nowy status u�ytkownika.
 *
 * je�li wys�a� pakiet zwraca 0, je�li nie uda�o si�, zwraca -1.
 */
int gg_change_status(struct gg_session *sess, int status)
{
	struct gg_new_status p;

	if (!sess) {
		errno = EFAULT;
		return -1;
	}

	if (sess->state != GG_STATE_CONNECTED) {
		errno = ENOTCONN;
		return -1;
	}

	gg_debug(GG_DEBUG_FUNCTION, "** gg_change_status(..., %d);\n", status);

	p.status = fix32(status);

	return gg_send_packet(sess->fd, GG_NEW_STATUS, &p, sizeof(p), NULL, 0);
}

/*
 * gg_logoff()
 *
 * wylogowuje u�ytkownika i zamyka po��czenie.
 *
 *  - sock - deskryptor socketu.
 *
 * nie zwraca b��d�w. skoro si� �egnamy, to olewamy wszystko.
 */
void gg_logoff(struct gg_session *sess)
{
	if (!sess)
		return;

	gg_debug(GG_DEBUG_FUNCTION, "** gg_logoff(...);\n");

	if (sess->state == GG_STATE_CONNECTED)
		gg_change_status(sess, GG_STATUS_NOT_AVAIL);
	
	if (sess->fd) {
		shutdown(sess->fd, 2);
		close(sess->fd);
	}
}

/*
 * gg_send_message()
 *
 * wysy�a wiadomo�� do innego u�ytkownika. zwraca losowy numer
 * sekwencyjny, kt�ry mo�na ola� albo wykorzysta� do potwierdzenia.
 *
 *  - sess - opis sesji,
 *  - msgclass - rodzaj wiadomo�ci,
 *  - recipient - numer adresata,
 *  - message - tre�� wiadomo�ci.
 *
 * w przypadku b��du zwraca -1, inaczej numer sekwencyjny.
 */
int gg_send_message(struct gg_session *sess, int msgclass, uin_t recipient, unsigned char *message)
{
	struct gg_send_msg s;

	if (!sess) {
		errno = EFAULT;
		return -1;
	}
	
	if (sess->state != GG_STATE_CONNECTED) {
		errno = ENOTCONN;
		return -1;
	}

	gg_debug(GG_DEBUG_FUNCTION, "** gg_send_message(..., %d, %u, \"...\");\n", msgclass, recipient);

	s.recipient = fix32(recipient);
	if (!sess->seq)
		sess->seq = 0x01740000 | (rand() & 0xffff);
	s.seq = fix32(sess->seq);
	s.msgclass = fix32(msgclass);
	sess->seq += (rand() % 0x300) + 0x300;
	
	if (gg_send_packet(sess->fd, GG_SEND_MSG, &s, sizeof(s), message, strlen(message) + 1) == -1)
		return -1;

	return fix32(s.seq);
}

/*
 * gg_ping()
 *
 * wysy�a do serwera pakiet typu yeah-i'm-still-alive.
 *
 *  - sess - zgadnij.
 *
 * je�li nie powiod�o si� wys�anie pakietu, zwraca -1. otherwise 0.
 */
int gg_ping(struct gg_session *sess)
{
	if (!sess) {
		errno = EFAULT;
		return -1;
	}

	if (sess->state != GG_STATE_CONNECTED) {
		errno = ENOTCONN;
		return -1;
	}

	gg_debug(GG_DEBUG_FUNCTION, "** gg_ping(...);\n");
	
	if(ping_outstanding) {
		debug_printf("Trying to send ping, when we havn't been ponged on last ping\n");
		return 1;
	}
	else {
		ping_outstanding = 1;
		return gg_send_packet(sess->fd, GG_PING, NULL, 0, NULL, 0);
	}
}

/*
 * gg_free_event()
 *
 * zwalnia pami�� zajmowan� przez informacj� o zdarzeniu
 *
 *  - event - wska�nik do informacji o zdarzeniu
 *
 * nie ma czego zwraca�.
 */
void gg_free_event(struct gg_event *e)
{
	if (!e)
		return;
	if (e->type == GG_EVENT_MSG)
		free(e->event.msg.message);
	if (e->type == GG_EVENT_NOTIFY)
		free(e->event.notify);
	free(e);
}

/*
 * gg_notify()
 *
 * wysy�a serwerowi list� ludk�w, za kt�rymi t�sknimy.
 *
 *  - sess - identyfikator sesji,
 *  - userlist - wska�nik do tablicy numer�w,
 *  - count - ilo�� numerk�w.
 *
 * je�li uda�o si�, zwraca 0. je�li b��d, dostajemy -1.
 */
int gg_notify(struct gg_session *sess, uin_t *userlist, int count)
{
	struct gg_notify *n;
	uin_t *u;
	int i, res = 0;

	if (!sess) {
		errno = EFAULT;
		return -1;
	}
	
	if (sess->state != GG_STATE_CONNECTED) {
		errno = ENOTCONN;
		return -1;
	}

	gg_debug(GG_DEBUG_FUNCTION, "** gg_notify(..., %d);\n", count);
	
	if (!userlist || !count)
		return 0;
	
	if (!(n = (struct gg_notify*) malloc(sizeof(*n) * count)))
		return -1;
	
	for (u = userlist, i = 0; i < count; u++, i++) { 
		n[i].uin = fix32(*u);
		n[i].dunno1 = 3;
	}
	
	if (gg_send_packet(sess->fd, GG_NOTIFY, n, sizeof(*n) * count, NULL, 0) == -1)
		res = -1;

	free(n);

	return res;
}

/*
 * gg_add_notify()
 *
 * dodaje w locie do listy ukochanych dany numerek.
 *
 *  - sess - identyfikator sesji,
 *  - uin - numerek ukochanej.
 *
 * je�li uda�o si� wys�a�, daje 0. inaczej -1.
 */
int gg_add_notify(struct gg_session *sess, uin_t uin)
{
	struct gg_add_remove a;

	if (!sess) {
		errno = EFAULT;
		return -1;
	}

	if (sess->state != GG_STATE_CONNECTED) {
		errno = ENOTCONN;
		return -1;
	}
	
	gg_debug(GG_DEBUG_FUNCTION, "** gg_add_notify(..., %u);\n", uin);
	
	a.uin = fix32(uin);
	a.dunno1 = 3;
	
	return gg_send_packet(sess->fd, GG_ADD_NOTIFY, &a, sizeof(a), NULL, 0);
}

/*
 * gg_remove_notify()
 *
 * w locie usuwa z listy zainteresowanych.
 *
 *  - sess - id sesji,
 *  - uin - numerek.
 *
 * zwraca -1 je�li by� b��d, 0 je�li si� uda�o wys�a� pakiet.
 */
int gg_remove_notify(struct gg_session *sess, uin_t uin)
{
	struct gg_add_remove a;

	if (!sess) {
		errno = EFAULT;
		return -1;
	}

	if (sess->state != GG_STATE_CONNECTED) {
		errno = ENOTCONN;
		return -1;
	}

	gg_debug(GG_DEBUG_FUNCTION, "** gg_remove_notify(..., %u);\n", uin);
	
	a.uin = fix32(uin);
	a.dunno1 = 3;
	
	return gg_send_packet(sess->fd, GG_REMOVE_NOTIFY, &a, sizeof(a), NULL, 0);
}

/*
 * gg_watch_fd_connected() // funkcja wewn�trzna
 *
 * patrzy na socketa, odbiera pakiet i wype�nia struktur� zdarzenia.
 *
 *  - sock - lalala, trudno zgadn��.
 *
 * je�li b��d -1, je�li dobrze 0.
 */
static int gg_watch_fd_connected(struct gg_session *sess, struct gg_event *e)
{
	struct gg_header *h;
	void *p;

	if (!sess) {
		errno = EFAULT;
		return -1;
	}

	gg_debug(GG_DEBUG_FUNCTION, "** gg_watch_fd_connected(...);\n");

	if (!(h = gg_recv_packet(sess))) {
		gg_debug(GG_DEBUG_MISC, "-- gg_recv_packet failed. errno = %d (%d)\n", errno, strerror(errno));
		return -1;
	}

	p = (void*) h + sizeof(struct gg_header);
	
	if (h->type == GG_RECV_MSG) {
		struct gg_recv_msg *r = p;

		gg_debug(GG_DEBUG_MISC, "-- received a message\n");

		if (h->length >= sizeof(*r)) {
			e->type = GG_EVENT_MSG;
			e->event.msg.msgclass = fix32(r->msgclass);
			e->event.msg.sender = fix32(r->sender);
			e->event.msg.message = strdup((char*) r + sizeof(*r));
			e->event.msg.time = fix32(r->time);
		}
	}

	if (h->type == GG_NOTIFY_REPLY) {
		struct gg_notify_reply *n = p;
		int count, i;

		gg_debug(GG_DEBUG_MISC, "-- received a notify reply\n");
		
		e->type = GG_EVENT_NOTIFY;
		if (!(e->event.notify = (void*) malloc(h->length + 2 * sizeof(*n)))) {
			gg_debug(GG_DEBUG_MISC, "-- not enough memory\n");
			free(h);
			return -1;
		}
		count = h->length / sizeof(*n);
		memcpy(e->event.notify, p, h->length);
		e->event.notify[count].uin = 0;
		for (i = 0; i < count; i++) {
			e->event.notify[i].uin = fix32(e->event.notify[i].uin);
			e->event.notify[i].status = fix32(e->event.notify[i].status);
		}
	}

	if (h->type == GG_STATUS) {
		struct gg_status *s = p;

		gg_debug(GG_DEBUG_MISC, "-- received a status change\n");

		if (h->length >= sizeof(*s)) {
			e->type = GG_EVENT_STATUS;
			memcpy(&e->event.status, p, h->length);
			e->event.status.uin = fix32(e->event.status.uin);
			e->event.status.status = fix32(e->event.status.status);
		}
	}

	if (h->type == GG_SEND_MSG_ACK) {
		struct gg_send_msg_ack *s = p;

		gg_debug(GG_DEBUG_MISC, "-- received a message ack\n");

		if (h->length >= sizeof(*s)) {
			e->type = GG_EVENT_ACK;
			e->event.ack.status = fix32(s->status);
			e->event.ack.recipient = fix32(s->recipient);
			e->event.ack.seq = fix32(s->seq);
		}
	}

	if (h->type == GG_PONG) {
		gg_debug(GG_DEBUG_MISC, "-- received a pong\n");
		ping_outstanding = 0;
		sess->last_pong = time(NULL);
	}

	free(h);

	return 0;
}

/*
 * gg_watch_fd()
 *
 * funkcja wywo�ywana, gdy co� si� stanie na obserwowanym deskryptorze.
 * zwraca klientowi informacj� o tym, co si� dzieje.
 *
 *  - sess - identyfikator sesji.
 *
 * zwraca wska�nik do struktury gg_event, kt�r� trzeba zwolni� p�niej
 * za pomoc� gg_free_event(). jesli rodzaj zdarzenia jest r�wny
 * GG_EVENT_NONE, nale�y je ola� kompletnie. je�li zwr�ci�o NULL,
 * sta�o si� co� niedobrego -- albo brak�o pami�ci albo zerwa�o
 * po��czenie albo co� takiego.
 */
struct gg_event *gg_watch_fd(struct gg_session *sess)
{
	struct gg_event *e;
	int res = 0;
#ifndef _WIN32
	int port;
#endif

	if (!sess) {
		errno = EFAULT;
		return NULL;
	}

	gg_debug(GG_DEBUG_FUNCTION, "** gg_watch_fd(...);\n");

	if (!(e = (void*) malloc(sizeof(*e)))) {
		gg_debug(GG_DEBUG_MISC, "-- not enough memory\n");
		return NULL;
	}

	e->type = GG_EVENT_NONE;

	switch (sess->state) {
#ifndef _WIN32
		/* Apparantly we will never be in this state as long as we are
		   using proxy_connect instead of gg_login - Herman */
		case GG_STATE_RESOLVING:
		{
			struct in_addr a;

			gg_debug(GG_DEBUG_MISC, "== GG_STATE_RESOLVING\n");

			if (read(sess->fd, &a, sizeof(a)) < sizeof(a) || a.s_addr == INADDR_NONE) {
				gg_debug(GG_DEBUG_MISC, "-- resolving failed\n");				

				errno = ENOENT;
				e->type = GG_EVENT_CONN_FAILED;
				e->event.failure = GG_FAILURE_RESOLVING;
				sess->state = GG_STATE_IDLE;

				close(sess->fd);

				break;
			}
			
			sess->server_ip = a.s_addr;

			close(sess->fd);

			waitpid(sess->pid, NULL, 0);

			gg_debug(GG_DEBUG_MISC, "-- resolved, now connecting\n");
			
			if (gg_http_use_proxy) {
				port = gg_http_proxy_port;
			} else {
				port = GG_APPMSG_PORT;
			};

			if ((sess->fd = gg_connect(&a, port, sess->async)) == -1) {
				struct in_addr *addr = (struct in_addr*) &sess->server_ip;
				
				gg_debug(GG_DEBUG_MISC, "-- connection failed, trying direct connection\n");

				if ((sess->fd = gg_connect(addr, GG_DEFAULT_PORT, sess->async)) == -1) {
				    gg_debug(GG_DEBUG_MISC, "-- connection failed, trying https connection\n");
				    if ((sess->fd = gg_connect(&a, GG_HTTPS_PORT, sess->async)) == -1) {		
					gg_debug(GG_DEBUG_MISC, "-- connect() failed. errno = %d (%s)\n", errno, strerror(errno));

					e->type = GG_EVENT_CONN_FAILED;
					e->event.failure = GG_FAILURE_CONNECTING;
					sess->state = GG_STATE_IDLE;
					break;
				    }
				}
				sess->state = GG_STATE_CONNECTING_GG;
				sess->check = GG_CHECK_WRITE;
			} else {
				sess->state = GG_STATE_CONNECTING;
				sess->check = GG_CHECK_WRITE;
			}
				
			break;
		}
#endif /* !_WIN32 */
		case GG_STATE_CONNECTING:
		{
			char buf[1024];
			int res, res_size = sizeof(res);

			gg_debug(GG_DEBUG_MISC, "== GG_STATE_CONNECTING\n");

			if (sess->async && (getsockopt(sess->fd, SOL_SOCKET, SO_ERROR, &res, &res_size) || res)) {
#if 0
				struct in_addr *addr = (struct in_addr*) &sess->server_ip;
				gg_debug(GG_DEBUG_MISC, "-- http connection failed, errno = %d (%s), trying direct connection\n", res, strerror(res));
				if ((sess->fd = gg_connect(addr, GG_DEFAULT_PORT, sess->async)) == -1) {
				    gg_debug(GG_DEBUG_MISC, "-- connection failed, trying https connection\n");
				    if ((sess->fd = gg_connect(addr, GG_HTTPS_PORT, sess->async)) == -1) {
					gg_debug(GG_DEBUG_MISC, "-- connect() failed. errno = %d (%s)\n", errno, strerror(errno));

					e->type = GG_EVENT_CONN_FAILED;
					e->event.failure = GG_FAILURE_CONNECTING;
					sess->state = GG_STATE_IDLE;
					break;
				    }
				}

				sess->state = GG_STATE_CONNECTING_GG;
				sess->check = GG_CHECK_WRITE;
#else
				gg_debug(GG_DEBUG_MISC, "-- http connection failed, errno = %d\n", res);
				e->type = GG_EVENT_CONN_FAILED;
				e->event.failure = GG_FAILURE_CONNECTING;
				sess->state = GG_STATE_IDLE;
#endif
				break;
			}
			
			gg_debug(GG_DEBUG_MISC, "-- http connection succeded, sending query\n");

			if (gg_http_use_proxy) {
				g_snprintf(buf, sizeof(buf) - 1,
					"GET http://" GG_APPMSG_HOST "/appsvc/appmsg.asp?fmnumber=%lu HTTP/1.0\r\n"
					"Host: " GG_APPMSG_HOST "\r\n"
					"User-Agent: " GG_HTTP_USERAGENT "\r\n"
					"Pragma: no-cache\r\n"
					"\r\n", sess->uin);
			} else {
				g_snprintf(buf, sizeof(buf) - 1,
					"GET /appsvc/appmsg.asp?fmnumber=%lu HTTP/1.0\r\n"
					"Host: " GG_APPMSG_HOST "\r\n"
					"User-Agent: " GG_HTTP_USERAGENT "\r\n"
					"Pragma: no-cache\r\n"
					"\r\n", sess->uin);
			};

    			gg_debug(GG_DEBUG_MISC, "=> -----BEGIN-HTTP-QUERY-----\n%s\n=> -----END-HTTP-QUERY-----\n", buf);
			if (write(sess->fd, buf, strlen(buf)) < strlen(buf)) {
				gg_debug(GG_DEBUG_MISC, "-- sending query failed\n");
				errno = EIO;
				e->type = GG_EVENT_CONN_FAILED;
				e->event.failure = GG_FAILURE_WRITING;
				sess->state = GG_STATE_IDLE;
				break;
			}

			sess->state = GG_STATE_READING_DATA;
			sess->check = GG_CHECK_READ;

			break;
		}

		case GG_STATE_READING_DATA:
		{
			char buf[1024], *tmp, *host;
			int port = GG_DEFAULT_PORT;
			struct in_addr a;

			gg_debug(GG_DEBUG_MISC, "== GG_STATE_READING_DATA\n");

			gg_read_line(sess->fd, buf, sizeof(buf) - 1);
			gg_chomp(buf);
	
			gg_debug(GG_DEBUG_TRAFFIC, "-- got http response (%s)\n", buf);
			if (strncmp(buf, "HTTP/1.", 7) || strncmp(buf + 9, "200", 3)) {
				gg_debug(GG_DEBUG_MISC, "-- but that's not what we've expected\n");

				errno = EINVAL;
				e->type = GG_EVENT_CONN_FAILED;
				e->event.failure = GG_FAILURE_INVALID;
				sess->state = GG_STATE_IDLE;
				break;
			}
	
			while (strcmp(buf, "\r\n") && strcmp(buf, ""))
				gg_read_line(sess->fd, buf, sizeof(buf) - 1);

			gg_read_line(sess->fd, buf, sizeof(buf) - 1);
			gg_chomp(buf);
	
			close(sess->fd);
	
			gg_debug(GG_DEBUG_TRAFFIC, "-- received http data (%s)\n", buf);
						
			tmp = buf;
			
			while (*tmp && *tmp != ' ')
				tmp++;
			while (*tmp && *tmp == ' ')
				tmp++;
			while (*tmp && *tmp != ' ')
				tmp++;
			while (*tmp && *tmp == ' ')
				tmp++;
			while (*tmp && *tmp != ' ')
				tmp++;
			while (*tmp && *tmp == ' ')
				tmp++;
			host = tmp;
			while (*tmp && *tmp != ' ')
				tmp++;
			*tmp = 0;

			if ((tmp = strchr(host, ':'))) {
				*tmp = 0;
				port = atoi(tmp+1);
			}

			a.s_addr = inet_addr(host);
			sess->server_ip = a.s_addr;
#if 0
			/* We need to watch this non-blocking socket so lets use proxy_connect 
			   in gg.c - Herman */
			if((sess->fd = gg_connect(&a, port, sess->assync)) == -1) {
				gg_debug(GG_DEBUG_MISC, "-- connection failed, trying https connection\n");
				if ((sess->fd = gg_connect(&a, GG_HTTPS_PORT, sess->async)) == -1) {
				    gg_debug(GG_DEBUG_MISC, "-- connection failed, errno = %d (%s)\n", errno, strerror(errno));

				    e->type = GG_EVENT_CONN_FAILED;
				    e->event.failure = GG_FAILURE_CONNECTING;
				    sess->state = GG_STATE_IDLE;
				    break;
				}
			}
#else
			sess->port = port;
#endif
			sess->state = GG_STATE_CONNECTING_GG;
			sess->check = GG_CHECK_WRITE;
		
			break;
		}

		case GG_STATE_CONNECTING_GG:
		{
			int res, res_size = sizeof(res);

			gg_debug(GG_DEBUG_MISC, "== GG_STATE_CONNECTING_GG\n");

			if (sess->async && (getsockopt(sess->fd, SOL_SOCKET, SO_ERROR, &res, &res_size) || res)) {
				struct in_addr *addr = (struct in_addr*) &sess->server_ip;

				gg_debug(GG_DEBUG_MISC, "-- connection failed, trying https connection\n");
				if ((sess->fd = gg_connect(addr, GG_HTTPS_PORT, sess->async)) == -1) {
				    gg_debug(GG_DEBUG_MISC, "-- connection failed, errno = %d (%s)\n", errno, strerror(errno));
				    
				    e->type = GG_EVENT_CONN_FAILED;
				    e->event.failure = GG_FAILURE_CONNECTING;
				    sess->state = GG_STATE_IDLE;
				    break;
				}
			}

			gg_debug(GG_DEBUG_MISC, "-- connected\n");
			
			sess->state = GG_STATE_READING_KEY;
			sess->check = GG_CHECK_READ;

			break;
		}

		case GG_STATE_READING_KEY:
		{
			struct gg_header *h;			
			struct gg_welcome *w;
			struct gg_login l;
			unsigned int hash;
			char *password = sess->password;

			gg_debug(GG_DEBUG_MISC, "== GG_STATE_READING_KEY\n");

			if (!(h = gg_recv_packet(sess))) {
				gg_debug(GG_DEBUG_MISC, "-- gg_recv_packet() failed. errno = %d (%s)\n", errno, strerror(errno));
				e->type = GG_EVENT_CONN_FAILED;
				e->event.failure = GG_FAILURE_READING;
				sess->state = GG_STATE_IDLE;
				close(sess->fd);
				break;
			}
	
			if (h->type != GG_WELCOME) {
				gg_debug(GG_DEBUG_MISC, "-- invalid packet received\n");

				free(h);
				close(sess->fd);
				errno = EINVAL;
				e->type = GG_EVENT_CONN_FAILED;
				e->event.failure = GG_FAILURE_INVALID;
				sess->state = GG_STATE_IDLE;
				break;
			}
	
			w = (void*) h + sizeof(struct gg_header);
			w->key = fix32(w->key);

			for (hash = 1; *password; password++)
				hash *= (*password) + 1;
			hash *= w->key;
	
			gg_debug(GG_DEBUG_DUMP, "%%%% klucz serwera %.4x, hash has�a %.8x\n", w->key, hash);
	
			free(h);

			free(sess->password);
			sess->password = NULL;
	
			l.uin = fix32(sess->uin);
			l.hash = fix32(hash);
			l.status = fix32(sess->initial_status ? sess->initial_status : GG_STATUS_AVAIL);
			l.dunno = fix32(0x0b);
			l.local_ip = 0;
			l.local_port = 0;
	
			gg_debug(GG_DEBUG_TRAFFIC, "-- sending GG_LOGIN packet\n");

			if (gg_send_packet(sess->fd, GG_LOGIN, &l, sizeof(l), NULL, 0) == -1) {
				gg_debug(GG_DEBUG_TRAFFIC, "-- oops, failed. errno = %d (%s)\n", errno, strerror(errno));

				close(sess->fd);
				e->type = GG_EVENT_CONN_FAILED;
				e->event.failure = GG_FAILURE_WRITING;
				sess->state = GG_STATE_IDLE;
				break;
			}
	
			sess->state = GG_STATE_READING_REPLY;

			break;
		}

		case GG_STATE_READING_REPLY:
		{
			struct gg_header *h;

			gg_debug(GG_DEBUG_MISC, "== GG_STATE_READING_REPLY\n");

			if (!(h = gg_recv_packet(sess))) {
				gg_debug(GG_DEBUG_MISC, "-- recv_packet failed\n");
				e->type = GG_EVENT_CONN_FAILED;
				e->event.failure = GG_FAILURE_READING;
				sess->state = GG_STATE_IDLE;
				close(sess->fd);
				break;
			}
	
			if (h->type == GG_LOGIN_OK) {
				gg_debug(GG_DEBUG_MISC, "-- login succeded\n");
				e->type = GG_EVENT_CONN_SUCCESS;
				sess->state = GG_STATE_CONNECTED;
				free(h);
				break;
			}

			if (h->type == GG_LOGIN_FAILED) {
				gg_debug(GG_DEBUG_MISC, "-- login failed\n");
				e->event.failure = GG_FAILURE_PASSWORD;
				errno = EACCES;
			} else {
				gg_debug(GG_DEBUG_MISC, "-- invalid packet\n");
				e->event.failure = GG_FAILURE_INVALID;
				errno = EINVAL;
			}

			e->type = GG_EVENT_CONN_FAILED;
			sess->state = GG_STATE_IDLE;
			close(sess->fd);
			free(h);

			break;
		}

		case GG_STATE_CONNECTED:
		{
			gg_debug(GG_DEBUG_MISC, "== GG_STATE_CONNECTED\n");

			if ((res = gg_watch_fd_connected(sess, e)) == -1) {

				gg_debug(GG_DEBUG_MISC, "-- watch_fd_connected failed. errno = %d (%s)\n", errno, strerror(errno));

 				if (errno == EAGAIN) {
					e->type = GG_EVENT_NONE;
					res = 0;
				} else
					res = -1;
			}
			break;
		}
	}

	if (res == -1) {
		free(e);
		e = NULL;
	}

	return e;
}

/*
 * Local variables:
 * c-indentation-style: k&r
 * c-basic-offset: 8
 * indent-tabs-mode: notnil
 * End:
 *
 * vim: shiftwidth=8:
 */

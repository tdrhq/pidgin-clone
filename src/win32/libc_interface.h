/*
 * gaim
 *
 * File: libc_interface.h
 *
 * Copyright (C) 2002-2003, Herman Bloggs <hermanator12002@yahoo.com>
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
#ifndef _LIBC_INTERFACE_H_
#define _LIBC_INTERFACE_H_
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#include <errno.h>
#include "libc_internal.h"

/* sys/socket.h */
extern int wgaim_socket(int namespace, int style, int protocol);
#define socket( namespace, style, protocol ) \
wgaim_socket( namespace, style, protocol )

extern int wgaim_connect(int socket, struct sockaddr *addr, u_long length);
#define connect( socket, addr, length ) \
wgaim_connect( socket, addr, length )

extern int wgaim_getsockopt(int socket, int level, int optname, void *optval, socklen_t *optlenptr);
#define getsockopt( args... ) \
wgaim_getsockopt( args )

extern int wgaim_setsockopt(int socket, int level, int optname, void *optval, socklen_t optlen);
#define setsockopt( args... ) \
wgaim_setsockopt( args )

extern int wgaim_getsockname (int socket, struct sockaddr *addr, socklen_t *lenptr);
#define getsockname( socket, addr, lenptr ) \
wgaim_getsockname( socket, addr, lenptr )

extern int wgaim_bind(int socket, struct sockaddr *addr, socklen_t length);
#define bind( socket, addr, length ) \
wgaim_bind( socket, addr, length )

extern int wgaim_listen(int socket, unsigned int n);
#define listen( socket, n ) \
wgaim_listen( socket, n )

/* sys/ioctl.h */
extern int wgaim_ioctl(int fd, int command, void* opt);
#define ioctl( fd, command, val ) \
wgaim_ioctl( fd, command, val )

/* fcntl.h */
extern int wgaim_fcntl(int socket, int command, int val);
#define fcntl( fd, command, val ) \
wgaim_fcntl( fd, command, val )

#define open( args... ) _open( ## args )

/* arpa/inet.h */
extern int wgaim_inet_aton(const char *name, struct in_addr *addr);
#define inet_aton( name, addr ) \
wgaim_inet_aton( name, addr )

/* netdb.h */
extern struct hostent* wgaim_gethostbyname(const char *name);
#define gethostbyname( name ) \
wgaim_gethostbyname( name )

/* netinet/in.h */
#define ntohl( netlong ) \
(unsigned int)ntohl( netlong )

/* string.h */
extern char* wgaim_strerror( int errornum );
#define hstrerror( herror ) \
wgaim_strerror( errno )
#define strerror( errornum ) \
wgaim_strerror( errornum )

extern char* wgaim_strsep(char **stringp, const char *delim);
#define strsep( stringp, delim ) \
wgaim_strsep( stringp, delim )

#define bzero( dest, size ) memset( dest, 0, size )

/* unistd.h */
extern int wgaim_read(int fd, void *buf, unsigned int size);
#define read( fd, buf, buflen ) \
wgaim_read( fd, buf, buflen )

extern int wgaim_write(int fd, const void *buf, unsigned int size);
#define write( socket, buf, buflen ) \
wgaim_write( socket, buf, buflen )

extern int wgaim_close(int fd);
#define close( fd ) \
wgaim_close( fd )

#define sleep(x) Sleep((x)*1000)

extern int wgaim_gethostname(char *name, size_t size);
#define gethostname( name, size ) \
wgaim_gethostname( name, size )

/* sys/time.h */
extern int wgaim_gettimeofday(struct timeval *p, struct timezone *z);
#define gettimeofday( timeval, timezone ) \
wgaim_gettimeofday( timeval, timezone )

/* stdio.h */
#define snprintf _snprintf
#define vsnprintf _vsnprintf

extern int wgaim_rename(const char *oldname, const char *newname);
#define rename( oldname, newname ) \
wgaim_rename( oldname, newname )

/* sys/stat.h */
#define mkdir(a,b) _mkdir((a))
#define fchmod(a,b)

/* time.h */
extern struct tm *wgaim_localtime_r(const time_t *time, struct tm *resultp);
#define localtime_r( time, resultp ) \
wgaim_localtime_r( time, resultp )

#endif /* _LIBC_INTERFACE_H_ */

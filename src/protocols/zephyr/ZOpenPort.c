/* This file is part of the Project Athena Zephyr Notification System.
 * It contains source for the ZOpenPort function.
 *
 *	Created by:	Robert French
 *
 *	$Source$
 *	$Author: chipx86 $
 *
 *	Copyright (c) 1987 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */
/* $Header$ */

#ifndef lint
static char rcsid_ZOpenPort_c[] = "$Header$";
#endif

#include "internal.h"
#include <sys/socket.h>

Code_t ZOpenPort(port)
    unsigned short *port;
{
    struct sockaddr_in bindin;
    int len;
    
    (void) ZClosePort();

    if ((__Zephyr_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	__Zephyr_fd = -1;
	return (errno);
    }

#ifdef SO_BSDCOMPAT
    {
      int on = 1;

      setsockopt(__Zephyr_fd, SOL_SOCKET, SO_BSDCOMPAT, (char *)&on, 
		 sizeof(on));
    }
#endif

    bindin.sin_family = AF_INET;

    if (port && *port)
	bindin.sin_port = *port;
    else
	bindin.sin_port = 0;

    bindin.sin_addr.s_addr = INADDR_ANY;

    if (bind(__Zephyr_fd, (struct sockaddr *)&bindin, sizeof(bindin)) < 0) {
	if (errno == EADDRINUSE && port && *port)
	    return (ZERR_PORTINUSE);
	else
	    return (errno);
    }

    if (!bindin.sin_port) {
	len = sizeof(bindin);
	if (getsockname(__Zephyr_fd, (struct sockaddr *)&bindin, &len))
	    return (errno);
    }
    
    __Zephyr_port = bindin.sin_port;
    __Zephyr_open = 1;

    if (port)
	*port = bindin.sin_port;

    return (ZERR_NONE);
}

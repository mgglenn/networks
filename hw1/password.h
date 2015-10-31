/**
* Mary Grace Glenn (mgglenn) 
* CPSC3600 001 
* HW 1 
* Due September 16th midnight
* 
* password.h
*
* This header file specifies certain restrictions
* on the password used between the server and client,
* for example, what kinds of charactes are allowed,
* what is the max length of N, et cetera.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>     /* for memset() */
#include <netinet/in.h> /* for in_addr */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <netdb.h>      /* for getHostByName() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <unistd.h>     /* for close() */
#include <time.h>
#include <signal.h>

#define MAX_N 8     /* Longest length of password*/

#ifndef PASSWORD_H
#define PASSWORD_H

#endif

/*
* Mary Grace Glenn (mgglenn)
* CPSC 3600 Homework 2
* 
* getfile.c
*
* Provides a simple TCP client that sends HTTP get requests.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

void DieWithError(char *errMessage);

int main(int argc, char *argv[]) {

	int sock;
	unsigned short webServPort = 8080;
	char *filename;

	/* CHECK PARAMETERS 

	if (( argc < 2) || ( argc > 6 )) {	
		fprintf(stderr, "Usage: %s < URL > [-p port] [-f filename]\n", argv[0]);
		fprintf(stderr, "URL (required): requested URL\n");
		fprintf(stderr, "port (optional): web server port, default 8080\n");
		fprintf(stderr, "filename (optional): doc saved as text file\n");
		exit(1);
	}
	else {
		if(argc >= 4) {
			int i = 4;
			while (i <= argc) {
				if(strcmp("-p", argv[i - 2]) == 0) {
					webServPort = atoi(argv[i -1]);
					fprintf(stderr, "port resolved\n");
				}
				if(strcmp("-f", argv[i - 2]) == 0) {
					filename = argv[i - 1];
					fprintf(stderr, "filename resolved\n");
				}
				i++;
			}
		}
	}
*/

	char *hostName = "www.cs.clemson.edu";
	char *pageName = "category";

/* SOCKET */

	if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) < 0)) {
		DieWithError("socket() failed\n");
	}
	else {
		printf("socket made...\n");
	}

	struct addrinfo info1, *info2, *curr;
	memset(&info1, 0, sizeof(info1));
	info1.ai_family = AF_UNSPEC;
	info1.ai_socktype = SOCK_STREAM;
	info1.ai_protocol = IPPROTO_TCP;

	if(getaddrinfo(hostName, "http", &info1, &info2)) {
		DieWithError("error with getting address information");
	}

	for(curr = info2; curr != NULL; curr = curr->ai_next) {
    		if ((sock = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol)) == -1) {
        		DieWithError("error creating socket");	
			continue;
    		}

    		if (connect(sock, curr->ai_addr, curr->ai_addrlen) == -1) {
        		close(sock);
        		DieWithError("error on connect\n");
			continue;
    		}
   		break; 
	}

	if (curr == NULL) {
    		fprintf(stderr, "failed to connect\n");
    		exit(2);
	}
	else {
		printf("connected\n");
	}

	char *format = "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nConnection: close\r\n\r\n";
	char *get = "HTMLGET 1.1";
	char *request;
	char *useragent = "HTMLGET 1.1";

	request = (char *)malloc(strlen(useragent) + strlen(get) + strlen(pageName) + strlen(hostName) - 5);

	sprintf(request, format, pageName, hostName, useragent);
	fprintf(stderr, "%s\n", request);

	if(send(sock, request, strlen(request), 0) != strlen(request)) {
		DieWithError("send didn't send stuff correctly...\n");
	}
	else {
		printf("sent correctly\n");
	}

	close (sock);
	DieWithError("socket closed");
}

void DieWithError(char *errorMessage) {
	fprintf(stderr, "%s\n", errorMessage);
	exit(1);
}

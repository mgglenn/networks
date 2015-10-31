/**
* Mary Grace Glenn (mgglenn)
* CPSC 3600 Homework 2
* 
* getfile.c
*
* Provides a simple TCP client
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 32 /* max length is 31 + null char */

void DieWithError(char *errMessage);

int main(int argc, char *argv[]) {

	int sock;
	struct sockaddr_in webServAddr;
	unsigned short webServPort = 8080;
	struct hostent *host;
	char *servIP;
	char *message;
	char *messageBuffer[RCVBUFSIZE];
	unsigned int messageStringLen;
	int bytesRcvd;
	int totalBytesRcvd;
	char *filename;

	/* CHECK PARAMETERS */

	if (( argc < 2) || ( argc > 6 )) {	
		fprintf(stderr, "Usage: %s < URL > [-p port] [-f filename]\n", argv[0]);
		fprintf(stderr, "URL (required): requested URL\n");
		fprintf(stderr, "port (optional): web server port, default 8080\n");
		fprintf(stderr, "filename (optional): doc saved as textual file\n");
		exit(1);
	}
	else {
		servIP = argv[1];

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

	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP) < 0)) {
		DieWithError("socket() failed\n");
	}

	memset(&webServAddr, 0, sizeof(webServAddr));
	webServAddr.sin_family = AF_INET;
	webServAddr.sin_addr.s_addr = inet_addr(servIP);
	webServAddr.sin_port = htons(webServPort);

	close (sock);
	exit(0);
}

void DieWithError(char *errorMessage) {
	fprintf(stderr, "%s\n", errorMessage);
	exit(1);
}

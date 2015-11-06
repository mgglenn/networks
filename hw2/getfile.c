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

void ClientDieWithError(char *errMessage);
char * formatRequest(char *hostName, char *pageName);
char * getHostName(char *url);
char * getPageName(char *url, int start);
int clientConn(char *hostName, int webServPort);
void clientSend(int sock, char *request);
void clientRead(int sock, char *filename);
void clientWrite(char * content, char *filename);

int main(int argc, char *argv[]) {

	unsigned short webServPort = 8080;
	char *filename = "n/a";

	/* CHECK PARAMETERS */
	if (( argc < 2) || ( argc > 6 )) {	
		fprintf(stderr, "Usage: %s < URL > [-t port] [-f filename]\n", argv[0]);
		fprintf(stderr, "URL (required): requested URL\n");
		fprintf(stderr, "port (optional): web server port, default 8080\n");
		fprintf(stderr, "filename (optional): doc saved as text file\n");
		exit(1);
	}
	else {
		if(argc >= 4) {
			int i = 4;
			while (i <= argc) {
				if(strcmp("-t", argv[i - 2]) == 0) {
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

	/* resolve host and page names */
	char *hostName, *pageName;
	char *url = (char *)malloc(strlen(argv[1]));
	strcpy(url, argv[1]);

	hostName = argv[1];

	if(strstr(hostName, "http://")) {
		webServPort = 80;
		hostName = getHostName(url);
		pageName = getPageName(argv[1], strlen(hostName) + strlen("http://"));
	}
	else {
		hostName = argv[1];
		pageName = "/";
	}

	fprintf(stderr, "HOST %s PAGE %s\n", hostName, pageName);

	int sock = clientConn(hostName, webServPort);

	char *request = formatRequest(hostName, pageName);
	fprintf(stderr, "%s", request);

	clientSend(sock, request);
	clientRead(sock, filename);

	close (sock);
	fprintf(stderr, "socket closed\n");
	exit(1);
}

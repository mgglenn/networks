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
char * formatRequest(char *hostName, char *pageName);
char * getHostName(char *url);
char * getPageName(char *url, int start);
int clientConn(char *hostName, int webServPort);
void clientSend(int sock, char *request);
void clientRead(int sock, char *filename);

int main(int argc, char *argv[]) {

	unsigned short webServPort = 80;
	char *filename;

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
	
	char *url = (char *)malloc(strlen(argv[1]));
	strcpy(url, argv[1]);
	char *hostName = getHostName(url); 
	char *pageName = getPageName(argv[1], strlen(hostName) + strlen("http://"));

	int sock = clientConn(hostName, webServPort);

	char *request = formatRequest(hostName, pageName);
	fprintf(stderr, "%s", request);

	clientSend(sock, request);
	clientRead(sock, filename);

	close (sock);
	fprintf(stderr, "socket closed\n");
	exit(1);
}

/* HELPER FUNCTIONS */

void DieWithError(char *errorMessage) {
	fprintf(stderr, "%s\n", errorMessage);
	exit(1);
}

char * formatRequest(char *hostName, char *pageName) {
	char *format = "GET %s HTTP/1.1\r\nUser-Agent: %s\r\nAccept: */*\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n";
        char *useragent = "mgglenn getfile";
        char *request = (char *)malloc(strlen(format) + strlen(hostName) + strlen(pageName) + strlen(useragent) - 5);
	sprintf(request, format, pageName, useragent, hostName);	
	
	return request;
}

char * getHostName(char *url) {
	char *host;
	const char delim[2] = "/";

	host = strtok(url, delim);
	host = strtok(NULL, delim);

	return host;
}

char * getPageName(char *url, int start) {
	if(start == strlen(url))
		return "/";

	return url + start;
}

int clientConn(char *hostName, int webServPort) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock < 0) DieWithError("error with socket()");

	struct hostent *host;
	host = gethostbyname(hostName);

	if(host == NULL) {
		close(sock);
		DieWithError("getbyhostname() failed");
	}

	unsigned int index = 0;

	while(host->h_addr_list[index] != NULL) {
		inet_ntoa(*(struct in_addr*)(host->h_addr_list[index]));
		index++;
	}
	
	struct sockaddr_in webServAddr;
	memset(&webServAddr, 0, sizeof(webServAddr));

	webServAddr.sin_family = AF_INET;
	bcopy((char *)host->h_addr, (char *)&webServAddr.sin_addr.s_addr, host->h_length);
	webServAddr.sin_port = htons(webServPort);

	if (connect(sock, (struct sockaddr *) &webServAddr, sizeof(webServAddr)) < 0)
		DieWithError("error on connect");

	return sock;
}

void clientSend(int sock, char * request) {

        if(send(sock, request, strlen(request), 0) != strlen(request)) {
                DieWithError("send didn't send stuff correctly...\n");
        }
        else {
                fprintf(stderr, "sent correctly\n");
        }
	return;
}

void clientRead(int sock, char *filename) {
        int totalBytesRcvd = 0;
        int bytesRcvd = 0;
        char content[BUFSIZ + 1];

        FILE *outputFile;

        if(filename) {
                outputFile = fopen(filename, "w");
        }

        while((bytesRcvd = recv(sock, content, BUFSIZ - 1, 0)) > 0) {
                totalBytesRcvd += bytesRcvd;
                content[bytesRcvd] = '\0';
                if(filename) {
                        fprintf(outputFile, "%s", content);     
                }
                else {
                        fprintf(stdout, "%s", content);
                }
        }

        if(filename) {
                fclose(outputFile);
        }

        if(bytesRcvd == -1)
                DieWithError("error reading content");
}

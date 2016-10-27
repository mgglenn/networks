/*
* Mary Grace Glenn (mgglenn)
* getfileHelper.c
* Contains all functions allowing for functionality of HTTP TCP web client.
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

/*
* Client exit function.
*/
void ClientDieWithError(char *errorMessage) {
        fprintf(stderr, "%s\n", errorMessage);
        exit(1);
}

/*
* Properly formats a given HTTP request.
*/
char * formatRequest(char *hostName, char *pageName) {
        char *format = "GET %s HTTP/1.1\r\nUser-Agent: %s\r\nAccept: */*\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n";
        char *useragent = "MGGCLIENT";
        char *request = (char *)malloc(strlen(format) + strlen(hostName) + strlen(pageName) + strlen(useragent) - 5);
        sprintf(request, format, pageName, useragent, hostName);

        return request;
}

/*
* Derives host name from a given url.
*/
char * getHostName(char *url) {
        char *host;
        const char delim[2] = "/";

        host = strtok(url, delim);
        host = strtok(NULL, delim);

        return host;
}

/* Derives host name from a given url */
char * getPageName(char *url, int start) {
        if(start == strlen(url))
                return "/";

        return url + start;
}

/* Returns client socket for sending information, establishes connection to server */
int clientConn(char *hostName, int webServPort) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);

        if(sock < 0) ClientDieWithError("error with socket()");

        struct hostent *host;
        host = gethostbyname(hostName);

        if(host == NULL) {
                close(sock);
                ClientDieWithError("getbyhostname() failed");
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
                ClientDieWithError("error on connect");

        return sock;
}

/* 
* Sends given formatted request to server socket.
*/
void clientSend(int sock, char * request) {
        if(send(sock, request, strlen(request), 0) != strlen(request)) {
                ClientDieWithError("error on send");
        }
        
	return;
}

/*
* Receives sent data from client, enables chunking.
*/ 
void clientRead(int sock, char *filename) {
       
	FILE *outputFile = NULL; 

	if(strcmp(filename, "n/a") != 0) {
                outputFile = fopen(filename, "w");
        }

	int bytesRcvd = 0;
        char content[(15 * BUFSIZ) + 1];

	memset(content, 0, sizeof(content));

	while((bytesRcvd = recv(sock, content, (BUFSIZ * 15), 0)) > 0) {

	 	if(strcmp(filename, "n/a") != 0) {
         		fprintf(outputFile, "%s", content);
         	}
        	 else {
                 	fprintf(stdout, "%s", content);
         	}

		if(strstr(content, "</html>") != NULL) break;

		memset(content, 0, bytesRcvd);
        }
                if(strcmp(filename, "n/a") != 0) {
                        fclose(outputFile);
                }
	fprintf(stderr, "\nfinished writing.\n");
}

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

char * replyFormat = "HTTP/1.1 %s\r\nDate: %s\r\nLast-Modified: %s\r\nContent-Type: %s\r\nContent-Length: %d\r\nServer: mgglenn webserver/1.2\r\nConnection: close\r\n\r\n";

char *outputFormat = "%s\t%s\t%s\t%s"; //"<METHOD><tab><URL><tab><DATE><tab><RESPONSE>";

/*
typedef struct
{
	char *method;
	char *host;
	char *file;

} HTTPrequest;
*/

typedef struct
{
	char *method;		//what the user wants
	char *resp; 		//Error or OK
	char *page;		//page request
	char *date; 		//Time sent
	char *allow; 		//only for 405
	char *lastMod; 		//lastModified
	char *cLength; 		//Content Length
	char *cType; 		//Content Type
} HTTPresponse;

void ServDieWithError(char *errorMessage) {
        fprintf(stderr, "%s\n", errorMessage);
        exit(0);
}

char *formatReply(HTTPresponse response) {

	return "";
}

char *parseContent(char *content, char * directory) {

	char tokens[40][31];
	char * delim = "\r\n :";

	int i = 0;

	char *curr = strtok(content, delim);

	while((curr != NULL) && (i < 40)) {
		strcpy(tokens[i], curr);
		//fprintf(stderr, "%d %s\n", i, curr);
		curr = strtok(NULL, delim);
		i++;
	}

	HTTPresponse servResponse;
	memset(&servResponse, 0, sizeof(servResponse));
	
	for(i = 0; i < 40; i++) {
		

	}

	
	return formatReply(servResponse);
}

void servReply(int clntSock, char * directory) {

        char content[BUFSIZ +1];
        int rcvMsgSize;

        if((rcvMsgSize = recv(clntSock, content, BUFSIZ, 0) < 0))
                ServDieWithError("recv() failed");

        char *reply = parseContent(content, directory);

        if(send(clntSock, reply, strlen(reply), 0) != strlen(reply))
                ServDieWithError("error sending to client");

        printf("message sent\n");
        close(clntSock);
        return;
}

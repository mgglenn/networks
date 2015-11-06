#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

typedef struct
{
	char *method;
	char *host;
	char *file;

} HTTPrequest;

typedef struct
{
	char *date;
	char *conn;
	char *allow;
	char *lastMod;
	char *cLength;
	char *cType;
} HTTPresponse;


void ServDieWithError(char *errorMessage) {
        fprintf(stderr, "%s\n", errorMessage);
        exit(0);
}

char *formatReply(char *content, char * directory) {

	char tokens[40][31];
	char * delim = "\r\n :";

	int i = 0;

	char *curr = strtok(content, delim);

	while((curr != NULL) && (i < 40)) {
		strcpy(tokens[i], curr);
		printf("%d %s\n", i, curr);
		curr = strtok(NULL, delim);
		i++;
	}

        return "";
}

void servReply(int clntSock, char * directory) {

        char content[BUFSIZ +1];
        int rcvMsgSize;

        if((rcvMsgSize = recv(clntSock, content, BUFSIZ, 0) < 0))
                ServDieWithError("recv() failed");

        char *reply = formatReply(content, directory);

        if(send(clntSock, reply, strlen(reply), 0) != strlen(reply))
                ServDieWithError("error sending to client");

        printf("message sent\n");
        close(clntSock);
        return;
}

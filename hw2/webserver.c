/*
* Mary Grace Glenn (mgglenn)
* CPSC 3600 homework 2
*
* 200
* 400
* 403
* 404
* 405
* 414
*/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

void DieWithError(char *errorMessage);
void servReply(int clntSock);
void respond200();
void respond400();
void respond403();
void respond404();
void respond405();
void respond414();
char *formatReply(char *request);

char * format = "HTTP/1.1 %s\r\nDate: %s\r\nLast-Modified: %s\r\nContent -Type: %s\r\nContent -Length: %d\r\nServer: mgglenn webserver/1.2\r\nConnection: close\r\n\r\n";

int main(int argc, char *argv[]) {

	int servSock;
	int clntSock;
	struct sockaddr_in servAddr;
	struct sockaddr_in clntAddr;
	unsigned short servPort = 8080;
	unsigned int clntLen;
	//char * directory = "./";

/* CHECK PARAMETERS */
        if ((argc > 4 )) {
                fprintf(stderr, "Usage: %s [-t port] [directory]\n", argv[0]);
                fprintf(stderr, "port (optional): web server port, default 8080\n");
                fprintf(stderr, "directory (optional): directory of file system, default is ./\n");
                exit(1);
        }
        else {
                if(argc >= 2) {
                        int i = 1;
                        while (i < argc) {
                                if(strcmp("-t", argv[i]) == 0) {
                                        servPort = atoi(argv[i +1]);
                                        fprintf(stderr, "port resolved\n");
					i++;
                                }
                                else if (servPort != 8080) {
                                        //directory = argv[i];
                                        fprintf(stderr, "directory resolved\n");
                                }
                                i++;
                        }
                }
        }

/* SOCKET STUFF */
	if((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("error creating socket");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(servPort);

	if(bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		DieWithError("error binding socket");
	}

	fprintf(stderr, "socked made and bound...\n");

	if(listen(servSock, 5) < 0 ) {
		DieWithError("error with socket listening");
	}

	fprintf(stderr, "waiting for client...\n");

	for(;;) {
		clntLen = sizeof(clntAddr);
		
		if((clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen)) < 0) {
			DieWithError("socket failed to accept client");
		}

		printf("Handling client %s\n", inet_ntoa(clntAddr.sin_addr));
		servReply(clntSock);
	}	
}

void DieWithError(char *errorMessage) {
	fprintf(stderr, "%s\n", errorMessage);
	exit(0);
}

void servReply(int clntSock) {

	char content[BUFSIZ +1];
	int rcvMsgSize;

	if((rcvMsgSize = recv(clntSock, content, BUFSIZ, 0) < 0))
		DieWithError("recv() failed");

	printf("%s\n", content);

	char *reply = "message received\n";

	if(send(clntSock, reply, strlen(reply), 0) != strlen(reply))
		DieWithError("error sending to client");

	printf("message sent\n");
	close(clntSock);
	return;
}

char *formatReply(char *filename) {

	return "";
}

void respond200() {}
void respond400() {}
void respond403() {}
void respond404() {}
void respond405() {}
void respond414() {}

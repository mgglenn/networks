/*
* Mary Grace Glenn (mgglenn)
* CPSC 3600 homework 2
*/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

void DieWithError(char *errorMessage);

int main(int argc, char *argv[]) {

	int servSock;
	int clntSock; 
	struct sockaddr_in servAddr;
	struct sockaddr_in clntAddr;
	unsigned short servPort = 8080; //by default
	char *root;
	unsigned int clntLen;

	/* ./arg1.out -p 12342 server */ 

	if(argc < 4) {
		fprintf(stderr, "Usage: %s -p < port no > < server >\n", argv[0]);
		exit(1);
	}
	else {
		int i = 1;
		while(i < argc -1) {
			if(strcmp("-p", argv[i]) == 0) {
				servPort = atoi(argv[i + 1]);
				fprintf(stderr, "port resolved\n");
			}
			else {
				root = argv[i];	
				fprintf(stderr, "root resolved\n");	
			}
			i++;
		}
	}

	servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if(servSock < 0) {
		DieWithError("socket() failed");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(servPort);

	/* Bind to local address */

	if(bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		DieWithError("bind() failed");
	}

	if(listen(servSock, 5) < 0) {
		DieWithError("listen failed");
	}

	while(1) {
		clntLen = sizeof(clntAddr);

		clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntLen);

		if(clntSock < 0) {
			DieWithError("accept failed");
		}
		else {
			printf("client connected! \n");
		}
	}
}

void DieWithError(char *errorMessage) {
	fprintf(stderr, "%s\n", errorMessage);
	exit(0);
}

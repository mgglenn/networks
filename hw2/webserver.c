/*
* Mary Grace Glenn (mgglenn)
* CPSC 3600 homework 2
* This server serves up files from its local directory or specified directory,
* on a given port or by 8080 by default. Uses HTTP over TCP.
*/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

void ServDieWithError(char *errorMessage);
void servReply(int clntSock, char *directory);


/*
* Handles GET and HEAD requests. 
*
*/
int main(int argc, char *argv[]) {

	int servSock;
	int clntSock;
	struct sockaddr_in servAddr;
	struct sockaddr_in clntAddr;
	unsigned short servPort = 1;
	unsigned int clntLen;
	char * directory = "."; //default

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
                                else if (servPort != 1) {
                                        directory = argv[i];
                                        fprintf(stderr, "directory resolved\n");
                                }
                                i++;
                        }
                }
        }

	if(servPort == 1) servPort = 8080;

/* SOCKET SET UP */
	if((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ServDieWithError("error creating socket");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(servPort);

	if(bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		ServDieWithError("error binding socket");
	}

	fprintf(stderr, "socked made and bound...\n");

	if(listen(servSock, 5) < 0 ) {
		ServDieWithError("error with socket listening");
	}

	fprintf(stderr, "waiting for client...\n");

/* WAIT FOR CLIENTS */
	for(;;) {
		clntLen = sizeof(clntAddr);
		
		if((clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen)) < 0) {
			ServDieWithError("socket failed to accept client");
		}

		fprintf(stderr, "Handling client %s\n", inet_ntoa(clntAddr.sin_addr));
		servReply(clntSock, directory);
	}	
}


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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
	char *temp;
	char *hostName;
	char *pageName;

	temp = strstr(argv[1], "www.");
	strcpy(pageName, strchr(temp, '/') + 1);

	temp = strstr(argv[1], "www.");
	hostName = strtok(temp, "/");

	printf("TEMP NAME: %s\n", temp);
	printf("HOST NAME: %s\n", hostName);	
	printf("PAGE NAME: %s\n", pageName);
	
/* SOCKET SHIT */

	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP) < 0)) {
		DieWithError("socket() failed\n");
	}
	else {
		printf("socket made...\n");
	}


/* RESOLVE IP ADDRESS 

	char *servIP = (char *)malloc(16);
	memset(servIP, 0, 16);
	struct hostent *host;

	if((host = gethostbyname(hostName)) == NULL) {
		DieWithError("couldn't resolve IP address");
	}

	if(inet_ntop(AF_INET, (void *)host->h_addr_list[0], servIP, 15) == NULL){
		DieWithError("couldn't resolve host");
	}
*/

//	printf("IP: %s\n", servIP);

/* MAKE SERVER ADDRESS */

	//struct sockaddr_in webServAddr;	
/*
	memset(&webServAddr, 0, sizeof(webServAddr));
	webServAddr.sin_family = AF_INET;
	webServAddr.sin_addr.s_addr = inet_addr(servIP);
	webServAddr.sin_port = htons(webServPort);
*/
/*
RESOLVE SERVIP 

	if (webServAddr.sin_addr.s_addr < 0) {
		host = gethostbyname(servIP);
		webServAddr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
	}

	if(connect(sock, (struct sockaddr *) &webServAddr, sizeof(webServAddr)) < 0 )
		DieWithError("connect() failed\n");
*/	
	close (sock);
	exit(0);
}

void DieWithError(char *errorMessage) {
	fprintf(stderr, "%s\n", errorMessage);
	exit(1);
}

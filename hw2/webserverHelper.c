/*
* Mary Grace Glenn (mgglenn)
* CPSC 3600 Homework 2 
* webserverHelper.c
* Contains all functions necessary for evaluating client requests and sending response.
*/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <netdb.h>	/*for gethostbyname() */
#include <time.h>	/*for getDate() */
#include <sys/stat.h>  /*for getLenght() */

/* struct for keeping information for HTTP handling and responses */
typedef struct
{
	char *method;		//only get or head
	char *page;		//page request
	char *host;		//host
	char *date; 		//Time sent
	char *type;		//file type
	char *lastMod;		//last modification
	int length;		//content length
	int resp;		//200, or any 400 error
} HTTPresponse;

/* struct used to handle information for HTTP response */
HTTPresponse servResponse;

/*
* Exits with error message.
*
*/
void ServDieWithError(char *errorMessage) {
        fprintf(stderr, "%s\n", errorMessage);
        exit(0);
}

/*
* Formats the fields of the server's HTTP request.
*
*/
char *formatReply() {

	char * replyFormat = "HTTP/1.1 %s\r\nDate: %s\r\nLast-Modified: %s\rContent-Type: %s\r\nContent-Length: %d\r\nServer: MGGHTTP/1.2\r\nConnection: close\r\n\r\n";
	char * reply405Format = "HTTP/1.1 %s\r\nAllow: GET, HEAD\r\nDate: %s\r\nLast-Modified: %s\rContent-Type: %s\r\nContent-Length: %d\r\nServer: MGGHTTP/1.2\r\nConnection: close\r\n\r\n";
	char * methodResponse = "";

	if(servResponse.resp == 200) {
		methodResponse = "200 OK";	
	}
	else if(servResponse.resp == 403) {
        	methodResponse = "403 Forbidden";
        }
	else if(servResponse.resp == 404) {
		methodResponse = "404 Not Found";
	}
	else if(servResponse.resp == 405) {
		methodResponse = "405 Method Not Found";
	}
	else if(servResponse.resp == 414) {
		methodResponse = "414 URI Too Long";
	}
	else {
                methodResponse = "400 Bad Request";
        }

	char * reply;

	if(servResponse.resp != 405) {
		reply = (char *)malloc(strlen(replyFormat) + strlen(methodResponse) + strlen(servResponse.date) + strlen(servResponse.lastMod) + strlen(servResponse.type));
		sprintf(reply, replyFormat, methodResponse, servResponse.date, servResponse.lastMod, servResponse.type, servResponse.length);	
	}
	else {
	        reply = (char *)malloc(strlen(reply405Format) + strlen(methodResponse) + strlen(servResponse.date) + strlen(servResponse.lastMod) + strlen(servResponse.type));
                sprintf(reply, reply405Format, methodResponse, servResponse.date, servResponse.lastMod, servResponse.type, servResponse.length);
	}
	return reply;
}

/*
* Formats the date and time of the request.
*
*/
char *getDate() {
	time_t rawtime;
  	struct tm * timeinfo;
  	char * date = (char *)malloc(30);

  	time(&rawtime);
  	timeinfo = localtime (&rawtime);	

	strftime (date, 80, "%a, %d %b %Y %X",timeinfo);

	return date;
}

/*
* Determines the time the file was last modified.
*/
char *getLastMod() {
	if(servResponse.resp != 200 && servResponse.resp != 403) return "n/a";

        struct stat fileSt;
        stat(servResponse.page, &fileSt);
	
	return ctime(&fileSt.st_mtime);	
}

/*
* Determines the size of a requested file.
*
*/
int getLength() {
	if(servResponse.resp != 200) return 0;
	
	struct stat st;
	stat(servResponse.page, &st);

	int size = (int)st.st_size;
	printf("SIZE %d\n", size);
	
	return size;	
	
}

/*
* Determiens file extension of requested file.
*
*/
char * getType () {
	
	char * type = "application/octet-stream";

	char * ext = strrchr(servResponse.page, '.');

	if(ext != NULL && (strlen(ext) > 1)) {

		if (strcmp(ext, ".js") == 0) {
			return "application/javascript";
		}
		else if (strcmp(ext, ".css") == 0) {
			return "test/css";
		}
		else if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) {
			return "text/html";
		}
		else if (strcmp(ext, ".txt") == 0) {
			return "text/plain";
		}
		else if (strcmp(ext, ".jpg") == 0) {
			return "image/jpeg";
		}
		else if (strcmp(ext, ".pdf") == 0) {
			return "application/pdf";
		}

	}
	
	return type;
}

/*
* Finishes adding to the fields of the request structure for the 
* client's given request.
*/
void buildResponse() {
        servResponse.date = getDate();
        servResponse.type = getType();
        servResponse.length = getLength();
        servResponse.lastMod = getLastMod();
} 

/*
* Determines the response the server will send.
*
*/
int getResponse(char *content, char *directory) {

	char tokens[40][31];
        char * delim = "\r\n :";
        int i = 0;
        char *curr = strtok(content, delim);

        /* Parse request */
        while((curr != NULL) && (i < 40)) {
                strcpy(tokens[i], curr);
                curr = strtok(NULL, delim);
                i++;
        }

        memset(&servResponse, 0, sizeof(servResponse));
        servResponse.resp = -1; // no error detected yet

        /* check file for host */
        for(i = 0; i < 39; i++) {
                curr = tokens[i];
                if(strcmp(curr, "Host") == 0){
                        if(gethostbyname(tokens[i+1]) != NULL){
                                servResponse.host = tokens[i+1];
                        }
                }
        }

        int notGet = 0;
        int notHead = 0;

        /* resolve directory + page */
        servResponse.page = (char *)malloc(strlen(directory) + strlen(tokens[1]));
        strcpy(servResponse.page, directory);
        strcat(servResponse.page, tokens[1]);

	/* check for 405 Bad Request*/
        servResponse.method = tokens[0];
        if(strcmp("GET", servResponse.method) != 0) notGet++;

        if(strcmp("HEAD", servResponse.method) != 0) notHead++;

        if((notGet + notHead) == 2) {
		return 405;
        }

	/* Make sure the file exists and we can read it, then we can send it!*/
        int canSend = 0;

        /* Check file exists */
        if(access(servResponse.page, F_OK ) == -1 ) {
		return 404;
        }
        else canSend++;

        /* check readability */
        if (servResponse.resp == -1 && (access(servResponse.page, R_OK) == -1)) {
		return 403;
        }
	else if (strstr(servResponse.page, directory) == NULL) {
		return 403;
	}
        else canSend++;

        /* check for host */
        if(servResponse.host == NULL) {
        	return 400;
	}

        /*check URI */
        if((strlen(servResponse.page) + strlen(servResponse.host) + strlen("http://") - strlen(directory)) > 200) {
		 return 414;
	}

	/* 200 if we can read the file and nothing else seems to be wrong */
        if(canSend == 2) {
                 return 200;
        }

        if(servResponse.resp == -1) servResponse.resp = 400;

	return 400;
}

/*
* Sends body of file to client if applicable (ie, it has 200 GET requst).
*/
char * attatchMessage(char * reply) {

	if((servResponse.resp == 200) && (strcmp("GET", servResponse.method) == 0)) {
		FILE * requestedFile = fopen(servResponse.page, "rb");

		if(requestedFile) {
			char * fileContent = malloc(servResponse.length);
			if (fread(fileContent, 1, servResponse.length + 1, requestedFile) == 0) {
				fprintf(stderr, "No file content written\n");
				exit(0);
			}

			fclose(requestedFile);
		
			char *fullReply = (char *)malloc(strlen(reply) + strlen(fileContent) + 2);
			strcpy(fullReply, reply);
			strcat(fullReply, fileContent);
			
			return fullReply;
		}
	}
	return reply;
}


/*
* Prints a summary of handling the client to STDOUT after it builds its formal
* HTTP response.
*/
void printOutput(char *content, char * directory) {

	servResponse.resp = getResponse(content, directory);
    	buildResponse();
	
	char *outputFormat = "%s\t%s\t%s\t%d";
	char *output = (char *)malloc(200);

	servResponse.date = servResponse.date + 4;
	sprintf(output, outputFormat, servResponse.method, servResponse.page, servResponse.date, servResponse.resp);
	
	if(servResponse.resp == -1) servResponse.resp = 400;

	servResponse.date = servResponse.date - 4;
	fprintf(stdout, "%s\n", output);
}

/*
* Given client socket clntSock and a directory, this method receives a message from a 
* client and then responds with the appropriate HTTP response, and prints a summary
* to STDOUT.
*/
void servReply(int clntSock, char * directory) {
        char content[BUFSIZ +1];
        int rcvMsgSize;

        if((rcvMsgSize = recv(clntSock, content, BUFSIZ, 0) < 0))
                ServDieWithError("recv() failed");

	printOutput(content, directory);	
	
	char *reply = attatchMessage(formatReply());

        if(send(clntSock, reply, strlen(reply), 0) != strlen(reply))
                ServDieWithError("error sending to client");

        close(clntSock);
	return;
}

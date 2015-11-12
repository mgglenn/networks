/**
* Mary Grace Glenn (mgglenn) 
* CPSC3600 001 
* HW 1 
* Due September 16th midnight
* 
* passwordServer.c
*
* This file provides functionality handling clients trying to guess this servers
* given password. First, a password of length N is generated from a given set
* of characters if one is not provided by the user. Then, the server
* handles one client request at a time to guess the password. If the password is
* correct, it sends SUCCESS, otherwise FAILURE. If the password is successully
* guessed, the server generates a new password and keeps tracks of the number
* of messages, answers, and client IPs it deals with.
*
* ROUTINES
*
* void serverCNTCode()
*
* char * generateNewPassword(int n)
*
* int main()
*/

#include "password.h"

int msgs = 0;      /* total number of messages received by server */
int answers = 0;   /* total number of correct answers clients guess */

/**
* serverCTNCode()
* 
* prints total messages, number of correct answers, and then exits.
*/
void serverCNTCCode() {
    printf("\npasswordServer: Total Messages\t%d\n", msgs);
    printf("passwordServer: Correct Answers\t%d\n", answers);
    printf("\npasswordServer:  CNT-C Interrupt,  exiting....\n");
    exit(0);
}

/**
* generateNewPassword(int n)
* 
* generates a random password of a given length, then returns it
*/
char * generateNewPassword(int n) {
   
    srand(time(NULL) * n + 736393);
    
    char * possibleChars = "1234abcdefghijklmnopqrstuvwxy~z!@#$%^&*()_-+={}[]|\\:;\"'<>,.\?/'";
    char * newPW = malloc(sizeof(char) * (n + 1));
    int i = 0;
    int r = rand() % strlen(possibleChars);
    while(i < n) {
        r = rand() % strlen(possibleChars);
        newPW[i] = possibleChars[r];
        i++;
    }

    newPW[n + 1] = '\0';
    return newPW;
}

/**
* main()
* 
* handles clients trying to guess the server's password
*/
int main(int argc, char *argv[])
{
    char * success = "SUCCESS";      /* client message for right password */
    char * failure = "FAILURE";      /* client message for wrong password */
   
    int sock;                        /* Socket */
    int serverN;                     /* password length N allowed by server */
    struct sockaddr_in pwServAddr;   /* Local address */
    struct sockaddr_in pwClntAddr;   /* Client address */
   
    unsigned int cliAddrLen;         /* Length of client's message */
   
    char * pwString;                 /* string containing server's password */
   
    unsigned short pwServPort;       /* Server port */
    int strLenRecvMsg;                 /* Size of client's message */

    if ((argc < 3) || (argc > 4))    /* Test number of params */
    {
        fprintf(stderr,"Usage:  %s <UDP SERVER PORT> < N > <inital password (optional)>\n", argv[0]);
        exit(1);
    }

    pwServPort = atoi(argv[1]);  /* Extract local port */
    serverN = atoi(argv[2]);    /* Extract serverN (length of password) */
   
    if(serverN > MAX_N) {
        fprintf(stderr, "Requested password size too large, exiting.\n");
        exit(0);
     }
   
   char pwBuffer[serverN];      /* Buffer that will receive client message */
   pwString = generateNewPassword(serverN);

   if(argc == 4) pwString = argv[3]; /* Replace with unique password if applicable */

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        fprintf(stderr, "ERROR: socket() failed\n");
        exit(1);
    }

    /* local address structure */
    memset(&pwServAddr, 0, sizeof(pwServAddr));   /* clear the structure */
    pwServAddr.sin_family = AF_INET;                /* Internet address family */
    pwServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* handle any interface */
    pwServAddr.sin_port = htons(pwServPort);      /* Local port */

    /* Bind to address */
    printf("passwordServer: About to bind to port %d\n", pwServPort);    
    if (bind(sock, (struct sockaddr *) &pwServAddr, sizeof(pwServAddr)) < 0) {
        fprintf(stderr, "ERROR: bind() failed\n");
        exit(1);
    }

    signal(SIGINT, serverCNTCCode); /* Implement terminating function to signal */

    while(1) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(pwClntAddr);

        /* Wait for client message ! */
        if ((strLenRecvMsg = recvfrom(sock, pwBuffer, serverN, 0,
            (struct sockaddr *) &pwClntAddr, &cliAddrLen)) < 0) {
            fprintf(stderr, "ERROR: recvfrom() failed\n");
	    exit(0);
        }
        else if (strLenRecvMsg != serverN) {
            fprintf(stderr, "ERROR: Improper message size.\n");
            exit(0);
        }
        else { 
       }

        if(strcmp(pwString, pwBuffer) == 0) {
            /** send success back to client */
            printf("passwordServer: Handled client %s\n", inet_ntoa(pwClntAddr.sin_addr));
            if (sendto(sock, success, strlen(success), 0,
                (struct sockaddr *) &pwClntAddr, sizeof(pwClntAddr)) < 0) {
                fprintf(stderr, "Error sending success message\n");
                exit(1);
            }
            else {
               printf("passwordServer: password: %s\ttheir answer: %s\n", pwString, pwBuffer);
               answers++; /* we've received the right answer */
               pwString = generateNewPassword(serverN);
               printf("passwordServer: new password generated.\n");
            }
        }
        else {
            /* Send failure back to client */
            if(sendto(sock, failure, strlen(failure), 0, (struct sockaddr *) &pwClntAddr, sizeof(pwClntAddr)) < 0) {
                fprintf(stderr, "ERROR: sending failure message\n");
                exit(0);
            }
        }
        msgs++; /* increment messages received */
    }
}

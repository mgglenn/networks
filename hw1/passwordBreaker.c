/**
* Mary Grace Glenn (mgglenn) 
* CPSC3600 001 
* HW 1 
* Due September 16th midnight
* 
* passwordBreaker.c
*
* This file provides functionality for guessing a server's given password of length
* N with a given character set (specified later in the file).
* The program opens a UDP socket to communicate with the server, generates all possi
* ble passwords, and then iterates through all of them until a match is confirmed.
* If the program is terminated at any point or the password is guessed correctly,
* the number of tries and time of the program is recorded.
*
* ROUTINES
*
* void clientCNTCode()
*
* void guessAllPassowords(char *password, int n, FILE *passwordFile)
*
* int main()
*/

#include "password.h"

void clientCNTCCode();

double progTime = 0; /*Time of program running */
int attempts = 0;    /* number of messages sent to server */

/**
* guessAllPassowords(char *password, int n, FILE *passwordFile)
* 
* generates all permutations of length n for the given character set,
* then stores it in a local file to be assessed later
*/
void guessAllPasswords(char * password, int n, FILE *passwordFile) {
    char * set = "1234abcdefghijklmnopqrstuvwxyz~!@#$%^&*()_-+={}[]|\\:;\"'<>,.\?/'";
         
    if (n == 0) { 
        fprintf(passwordFile, "%s\n", password);
        free(password);
         return;
    }
    else {
        int i;
        for (i = 0; i < strlen(set); i++) {
            char *newPass = malloc(sizeof(password));
            strcpy(newPass, password);
            newPass[strlen(newPass)] = set[i];
            guessAllPasswords(newPass, n -1, passwordFile);
        }
    }
}

/**
* int main()
* 
* guesses password of a given server until terminated or successful
*/
int main(int argc, char *argv[])
{
    clock_t progStart = clock();

    char *success = "SUCCESS";       /* the end result! */
    int sock;                        /* Socket descriptor */
    int clientN = 0;                 /* password length client will guess */
    struct sockaddr_in pwServAddr;   /* server address */
    struct sockaddr_in fromAddr;     /* Source address of pw */
    struct hostent *thehost;         /* Hostent for the server */
    unsigned short pwServPort;       /* server port */
    unsigned int fromSize;           /* value from recvfrom() */
    char *servIP;                    /* IP address of server */
    char *pwString = malloc(sizeof(char));
    char serverResp[strlen(success)];    /* Buffer to receive response from server */

    if ((argc < 3) || (argc > 4))    /* Test user's parameters for client */
    {
        fprintf(stderr,"Usage: %s <Server Name> <Server Port> < N >\n", argv[0]);
        exit(1);
    }

    signal (SIGINT, clientCNTCCode);

    servIP = argv[1];                /* extract IP address in dotted format */

    if (argc >= 3)
        pwServPort = atoi(argv[2]);  /* Use given port, if applicable */
    else {
       fprintf(stderr, "ERROR: Improper port. Exiting.\n");
       exit(0);
    }

    if(argc == 4) {
        clientN = atoi(argv[3]);     /* extract client's password length N */
        if(clientN > MAX_N) clientN = MAX_N;
    }
  
    int stringLengthResp = MAX_N;             /* Length of received response */

    /* Create a datagram/UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        fprintf(stderr, "ERROR: socket() failed\n");
        exit(1);
    }
    /* Construct the server address structure */
    memset(&pwServAddr, 0, sizeof(pwServAddr));    /* reset structure fields */
    pwServAddr.sin_family = AF_INET;                 /* Internet address family */
    pwServAddr.sin_addr.s_addr = inet_addr(servIP);  /* IP address of server*/
    pwServAddr.sin_port   = htons(pwServPort);     /* Server port to send to */

    /* Fix dotted decimal format into struct format */
    if (pwServAddr.sin_addr.s_addr == -1) {
        thehost = gethostbyname(servIP);
            pwServAddr.sin_addr.s_addr = *((unsigned long *) thehost->h_addr_list[0]);
    }

FILE *allPasswordsWrite; /* file we will write all possible passwords to */
allPasswordsWrite = fopen("allPasswords.txt", "w+");
guessAllPasswords(pwString, clientN, allPasswordsWrite);
fclose(allPasswordsWrite);

printf("passwordBreaker: sending guesses to the server: %s \n",servIP);
pwString = malloc(sizeof(char) * (clientN + 1));
FILE *allPasswordsRead; /* same file, but this time we will read guesses from it */
allPasswordsRead = fopen("allPasswords.txt", "r");

while(!feof(allPasswordsRead)) {

    pwString = fgets(pwString, sizeof(pwString), allPasswordsRead); // selectNextPassword()
   
    /* Send the guess to the server */
    clock_t timer1 = clock();
    if (sendto(sock, pwString, clientN, 0, (struct sockaddr *)
               &pwServAddr, sizeof(pwServAddr)) < 0) {
            fprintf(stderr, "ERROR: Nothing was sent!\n");
            exit(1);
    }
    else attempts++;

    clock_t timer2 = clock();

    /* drop this guess if the time out is greater than 1.5 seconds */
    double timeout = (double)(timer2 - timer1) / CLOCKS_PER_SEC;
    if(timeout >= 1.5) {
        printf("passwordBreaker: timeout popped, skipping loop.\n");
        continue;
    }
  
    /* Receive a response */
    fromSize = sizeof(fromAddr);
    if ((stringLengthResp = recvfrom(sock, serverResp, strlen(success), 0, (struct sockaddr *) &fromAddr, &fromSize)) < 0 ){
           fprintf(stderr, "ERROR: Error with response. exiting\n");
            exit(1);
    }

    if(strcmp(success, serverResp) == 0) {
       break; /* success! */
    }

    if (pwServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
        fprintf(stderr,"Error: received a packet from unknown source \n");
        exit(1);
    }

    /* no success, keep looping ... */
}

fclose(allPasswordsRead);

clock_t progStop = clock();
progTime = (double)( progStop - progStart) * 1000.0 / CLOCKS_PER_SEC;
progTime = progTime / 1000.0;

/* Print relevant statistics */
serverResp[stringLengthResp] = '\0';
printf("\npasswordBreaker:\tPASSWORD %s\t\t\tATTEMPTS %d\n\t\t\tTIME %f\n", pwString, attempts, progTime);
close(sock);
exit(0);

}

/*
* Print relevant statistics then terminate, since user wants to terminate program 
*/
void clientCNTCCode() {
  printf("\npasswordBreaker:\tATTEMPTS %d\n\t\t\tTIME %f\n", attempts, progTime);
  printf("\npasswordBreaker:  CNT-C Interrupt,  exiting....\n");
  exit(1);
}

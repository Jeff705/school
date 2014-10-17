/*
 * Chris Ragan
 * Dr. Remy
 * CPSC 3600
 * Project 1: passwordBreaker.c
 *
 * Included functions:
 * 		main()
 *		timeout_handler()
 *		exit_with_time()
 *		checkForSuccess()
 *		incrementPassword()
 *		
 */

#include "includes.h"

#define RECBUFSIZE 32 //receive buffer size
#define TIMEOUT_SEC 1 //how long in seconds to wait before timing out
#define MAX_TIMEOUTS 5 //number of timeouts allowed before forced exit

int attempt_counts;
struct timespec begin, end;

volatile int timeout_status = 0;
volatile int timeout_counter = 0;
int sockDescriptor;

void DieWithError(char *errMessage);

void timeout_handler(int signum);

void exit_with_time(int signum);

int checkForSuccess(char *msg);

void incrementPassword(int *password, int size);

void initializeAlphabet(char alphabet[]);

/*
 * The main loop. Initializes variables, sets up a UDP socket, and starts
 * slinging passwords of a defined size at the server/port (all argument values)
 * Will time out if no response; upon CTRL+C exit or timeout will display number
 * of attempts as well as the total time elapsed.
 */

int main(int argc, char *argv[]) {
	
	int solution_found = 0;
	int i;
	char alphabet[62];
	struct sockaddr_in servAddress;
	struct sockaddr_in returnAddress;
	unsigned short servPort;
	unsigned int fromSize;
	char *servIP;
	char replyBuffer[RECBUFSIZE];
	int passSize;
	int recvLen;

	clock_gettime(CLOCK_REALTIME, &begin); //get the time when we start

	signal(SIGALRM, timeout_handler); //bind timeout_handler() to SIGALRM
	signal(SIGINT, exit_with_time);   //bind exit_with_time() to SIGINT

	if(argc != 4) {
		DieWithError("Usage: passwordCracker <serverIP> <serverPort> <passwordSize>");
	}

	passSize = atoi(argv[3]);

	if(passSize < 1 || passSize > 8) {
		DieWithError("Password size must be in range [1-8].");
	}

	servIP = argv[1];
	servPort = atoi(argv[2]);

	if((sockDescriptor = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		DieWithError("socket() failed to create a socket!");
	}

	memset(&servAddress, 0, sizeof(servAddress));
	servAddress.sin_family		= AF_INET;
	servAddress.sin_addr.s_addr = inet_addr(servIP);
	servAddress.sin_port		= htons(servPort);

	int passIndices[passSize];
	char passAttempt[passSize + 1];
	initializeAlphabet(alphabet);

	for(i = 0; i < passSize; i++) {
		*(passIndices + i) = 0;
	}

	while(timeout_counter < MAX_TIMEOUTS)
	{
		timeout_status = 0;

		while(!timeout_status && solution_found != 1) {
		/*
		 * see timeout_handler() implementation below
		 */			
		alarm(TIMEOUT_SEC);

			i = fcntl(sockDescriptor, F_GETFL);
			
			/* We reset the O_NONBLOCK flag here to keep from having a silly
			 * amount of calls to recvfrom() in the event of a timeout...
			 */

			fcntl(sockDescriptor, F_SETFL, i & ~O_NONBLOCK);
			incrementPassword(passIndices, passSize);
			for(i = 0; i < passSize; i++) { //build string from alphabet to send
				*(passAttempt + i) = alphabet[passIndices[i]];
			}
			*(passAttempt + i) = '\0'; //terminate string

			if(sendto(sockDescriptor, passAttempt, passSize, 0, (struct sockaddr *) &servAddress, sizeof(servAddress)) != passSize) 
			{
				DieWithError("sendto() sent an unexpected number of bytes!");
			}
			fromSize = sizeof(returnAddress);
			
			if((recvLen = recvfrom(sockDescriptor, replyBuffer, RECBUFSIZE, 0,
					(struct sockaddr*) &returnAddress, &fromSize)) > 0)
			{
				solution_found = checkForSuccess(replyBuffer);
				attempt_counts++;
			}
			if(solution_found < 0) {
				printf("Received %s from server, unexpected\n", replyBuffer);
				DieWithError("Exiting...");
			}
			
		}
		if(solution_found) {
			printf("Success! Password is %s\n", passAttempt);
			exit_with_time(0);
		}
	}
	if(timeout_counter >= MAX_TIMEOUTS) {
		printf("Exceeded MAX_TIMEOUTS, exiting...");
		exit_with_time(0);
	}
	//we'll never get here, but -Wall cleanliness is next to godliness!
	return(1);
}

/*
 * If no reply received within the timeout threshold, SIGALRM will be set.
 * This function just lets the user know what's going on, breaks the recvfrom()
 * function out of blocking, and sets a couple of status variables
 *
 */
void timeout_handler(int signum) {
	printf("Attempt timed out, will attempt again...\n");
	timeout_status = 1;
	timeout_counter++;
	/* I found this necessary to keep the client rolling in case of a receive
	 * timeout... this stops recvfrom() blocking so the main loop can attempt
	 * another password.
	 */
	fcntl(sockDescriptor, F_SETFL, O_NONBLOCK);
}

/*
 * Calculates time elapsed from the beginning of the program, prints it in a
 * readable format, and exits.
 *
 */
void exit_with_time(int signum) {
	clock_gettime(CLOCK_REALTIME, &end);
	//this should be total time in usec
	unsigned long total_time_s = end.tv_sec - begin.tv_sec;
	unsigned long total_time_ns = end.tv_nsec - begin.tv_nsec;

	int hh = total_time_s / 3600;
	total_time_s -= hh * 3600;
	int mm = total_time_s / 60;
	total_time_s -= mm * 60;
	int ss = total_time_s;

	int ms = total_time_ns / 1000000;
	total_time_ns -= ms * 1000000;
	int us = total_time_ns / 1000;

	printf("\nRan %d attempts in %.2d:%.2d:%.2d:%.3d:%.3d (hh:mm:ss:ms:us)\n", 
			attempt_counts, hh, mm, ss, ms, us);
	fflush(stdout);
	exit(0);
}

/*
 * Just checks the server reply for SUCCESS or FAILURE, returning 1 or 0
 * respectively (or -1 if something else is received)
 *
 */

int checkForSuccess(char *msg) {
	int status;
	if(strcmp(msg, "SUCCESS") == 0) {
		status = 1;
	}
	else if(strcmp(msg, "FAILURE") == 0) {
		status = 0;
	}
	else status = -1;

	return status;
}

/*
 * The 'password' in this case is actually an array of ints, since the alphabet
 * is an array of allowable characters indexed by these values. So it's
 * effectively a base 62 counter with rollover, the width of which is determined
 * by 'size'. 
 */
void incrementPassword(int *password, int size) {
	*(password + size - 1) += 1;
	if(*(password + size - 1) >= 62) //check to see if we've rolled over this digit
	{
		*(password + size - 1) = 0;
		if(size == 1) //are we at the base case (most significant digit)?
		{
			return;
		}
		else {  //if not, we need to increment the next most significant digit
			incrementPassword(password, size - 1);
		}
	}
	//if the current digit didn't roll over, we're all done!
}

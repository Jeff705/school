#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>

#define RECBUFSIZE 32
#define TIMEOUT_SEC 1
#define RECV_INTERVAL_MS 20
#define MAX_TIMEOUTS 5

int attempt_counts;
struct timespec begin, end;

volatile int timeout_status = 0;
volatile int timeout_counter = 0;
int sockDescriptor;

//shamelessly stole function name from TCPEchoClient.c, because this name is awesome.
void DieWithError(char *errMessage);

int getIndexOf(char *source, char *token);

void timeout_handler(int signum);

void exit_with_time(int signum);

int checkForSuccess(char *msg);

void incrementPassword(char *password, int size);


int main(int argc, char *argv[]) {
	
//	int passIndices[];
	int solution_found = 0;
	int i;
	char alphabet[62];
	struct sockaddr_in servAddress;
	struct sockaddr_in returnAddress;
	unsigned short servPort;
	unsigned int fromSize;
	char *servIP;
//	char *passAttempt;
	char replyBuffer[RECBUFSIZE];
	int passSize;
	int recvLen;

	clock_gettime(CLOCK_REALTIME, &begin);

	signal(SIGALRM, timeout_handler);
	signal(SIGINT, exit_with_time);
	printf("is it here?[55]\n");
	if(argc != 4) {
		DieWithError("Usage: passwordCracker <serverIP> <serverPort> <passwordSize>");
	}

	passSize = atoi(argv[3]);

	if(passSize < 1 || passSize > 8) {
		DieWithError("Password size must be in range [1-8].");
	}

	servIP = argv[1];
	servPort = atoi(argv[2]);
	printf("perhaps here?[68]\n");
	if((sockDescriptor = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		DieWithError("socket() failed to create a socket!");
	}
	printf("what about here?[72]\n");
	memset(&servAddress, 0, sizeof(servAddress));
	servAddress.sin_family		= AF_INET;
	servAddress.sin_addr.s_addr = inet_addr(servIP);
	servAddress.sin_port		= htons(servPort);

/*INITIALIZE ALPHABET (because doing it by hand is annoying)
 *This will initialize an alphabet of 0-9, a-z, and A-Z
 */

	for(i = 0; i < 62; i++) {
		if((i + 48 < 58)){
			alphabet[i] = (i + 48);
		}

		else if((i + 55) < 91) {
			alphabet[i] = (i + 55);
		}

		else alphabet[i] = i + 61;
	}
	
	int passIndices[passSize];
	char passAttempt[passSize + 1];
/*	passIndices = malloc(sizeof(int)*passSize);
	passAttempt = malloc(passSize + 1); */

/* INITIALIZE PASSWORD INDICES
 *
 * passIndices is an array of integers which will be used to access members of
 * the alphabet[] array. This is the easiest representation I could think of to
 * make a password wrap around once we'd hit the max value (Z{n} where n is max
 * password length)
 *
 */
	for(i = 0; i < passSize; i++) {
		*(passIndices + i) = 0;
	}
	while(timeout_counter < MAX_TIMEOUTS)
	{
		alarm(TIMEOUT_SEC);
		timeout_status = 0;
		while(!timeout_status && solution_found != 1) {
			i = fcntl(sockDescriptor, F_GETFL);
			fcntl(sockDescriptor, F_SETFL, i & ~O_NONBLOCK);
			for(i = 0; i < passSize; i++) { //build string to send
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
			exit_with_time(0);
		}
	}
	if(timeout_counter >= MAX_TIMEOUTS) {
		DieWithError("Exceeded MAX_TIMEOUTS, exiting...");
	}

	return(1);
}

void DieWithError(char *errMessage) {
	perror(errMessage);
	exit_with_time(0);
}

void timeout_handler(int signum) {
	printf("timeout detected\n");
	timeout_status = 1;
	timeout_counter++;
	fcntl(sockDescriptor, F_SETFL, O_NONBLOCK);
}

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
	exit(0);
}

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

int getIndexOf(char *source, char *token) {
	return((int)(strchr(source, *token) - source));
}

void incrementPassword(char *password, int size) {
	
}

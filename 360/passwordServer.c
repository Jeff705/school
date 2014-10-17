/*
 * Chris Ragan
 * Dr. Remy
 * CPSC 3600
 * Project 1: passwordServer.c
 *
 * Included functions:
 * 		main()
 * 		goodRandom()
 *		generatePassword()
 * 		checkPassword()
 */

#include "includes.h"

#define RECV_MAX 12
#define INET_ADDR_SIZE 16

/* stats variables, globalized for easy access */

char clientList[20][INET_ADDR_SIZE];
int clientCount = 0;
int attemptCount = 0;
int successCount = 0;
char clientAddrString[INET_ADDR_SIZE];

int goodRandom();

void exit_with_stats();

void DieWithError(char *errMessage);

void generatePassword(char *password, int length, char alphabet[]);

int checkPassword(char *password, char *incoming, int length);

void initializeAlphabet(char alphabet[]);

int main(int argc, char *argv[]) {
	int socketDescriptor;
	struct sockaddr_in localAddress;
	struct sockaddr_in clientAddress;
	unsigned int clientAddrLength;
	int passwordLength;
	char recvBuffer[RECV_MAX];
	unsigned short serverPort;
	int recvdSize;
	int passwordGuessed = 0;
	char *reply;
	char *password;
	char alphabet[62];


	srand(time(NULL));

	initializeAlphabet(alphabet);
	signal(SIGINT, exit_with_stats);

	if(argc < 3) {
		DieWithError("Usage: ./passwordServer <serverPort> <passwordLength> [<initialPassword>]");
	}

	passwordLength = atoi(argv[2]);
	password = malloc(passwordLength + 1);

	if(argc == 4) {
		password = argv[3];
	}
	else {
		generatePassword(password, passwordLength, alphabet);
	}

	serverPort = atoi(argv[1]);

	if((socketDescriptor = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		DieWithError("socket() failed!");
	}

	memset(&localAddress, 0, sizeof(localAddress));
	localAddress.sin_family = AF_INET;
	localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddress.sin_port = htons(serverPort);

	if(bind(socketDescriptor, (struct sockaddr *) &localAddress, sizeof(localAddress)) <  0) {
		DieWithError("bind() failed!");
	}

	while(1) {
		clientAddrLength = sizeof(clientAddress);

		if((recvdSize = recvfrom(socketDescriptor, recvBuffer, RECV_MAX, 0, (struct sockaddr *)
			&clientAddress, &clientAddrLength)) < 0) {
			DieWithError("recvfrom() failed!");
		}
		clientCount++;
		attemptCount++;
		inet_ntop(AF_INET, &(clientAddress.sin_addr), clientAddrString, sizeof(clientAddrString));
		if(strcmp(clientAddrString, clientList[clientCount - 1])){
			strcpy(clientList[clientCount], clientAddrString);
		}
		else {clientCount--;}
		
		if((passwordGuessed = checkPassword(password, recvBuffer, passwordLength)))
		{
			printf("Password successfully guessed by client: %s\n\n",clientAddrString);
			generatePassword(password, passwordLength, alphabet);
			reply = "SUCCESS";
			successCount++;
			passwordGuessed = 0;
		}
		else { reply = "FAILURE"; }

		if(sendto(socketDescriptor, reply, 8, 0, (struct sockaddr *) &clientAddress, 
			sizeof(clientAddress)) != 8) {
			DieWithError("sendto() sent a different number of bytes than expected");
		}
	}
	return 1; //shouldn't ever get here, but this makes us -Wall clean :)
}

/*
 * Generates a random password of 'length' by randomly indexing the alphabet.
 * Prints the password so we can compare it to what the client guesses.
 */
void generatePassword(char *password, int length, char alphabet[]) {
	int i;
	for(i = 0; i < length; i++) {
		*(password + i) = alphabet[goodRandom()];
	}
	printf("Password set to %s\n", password);
}

/*
 * Generates a random number between 0 and 61, inclusive. The way this is
 * written eliminates the skew potentially introduced by using % 61 to limit
 * our values.
 */
int goodRandom() {
	int divisor = RAND_MAX / 62;
	int value;

	do {
		value = rand() / divisor;
	} while (value > 61);

	return value;
}

void exit_with_stats() {
	printf("%d\t%d\n",attemptCount,successCount);
	int i;
	for(i = 0; i < 20; i++) {
		if(strcmp("",clientList[i])){
			printf("%s\n",clientList[i]);
		}
	}
	fflush(stdout);
	exit(0);
}
/*
 * Does a character-by-character comparison of password stored vs. received.
 * Is this efficient? Probably not. But it works.
 */
int checkPassword(char *password, char *incoming, int length) {
	int i;
	int valid = 1;
	for(i = 0; i < length; i++) {
		if(*(password + i) != *(incoming + i)) {
			valid = 0;
		}
	}
	return valid;
}


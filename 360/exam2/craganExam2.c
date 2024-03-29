/*
 * Chris Ragan
 * Dr. Remy
 * Exam 2 code portion
 * 11/14/14
 *
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>

#define FILEBUFSIZE 1024 
#define MAXPENDING 5

void handleError(char* message);
void transferFile(char *buffer, FILE *fd, int clientSocket);
void exit_with_stats(int signum);
unsigned long resolveName(char *name);

int filesTransferred = 0;

int main(int argc, char **argv)
{

	int servSock;
	int clientSock;
	struct sockaddr_in servAddr;
	struct sockaddr_in clientAddr;
	unsigned short servPort;
	unsigned int clientLen;
	char *servIP;
	char fileBuffer[FILEBUFSIZE];
	
	if(argc < 2)
	{
		handleError("Usage: ./a.out < mode > < addl. parameters >\n");
	}
//CLIENT MODE
	if(atoi(argv[1]) == 0)
	{
		if(argc != 5)
		{
			handleError("Usage: ./a.out <0> <serverName> <serverPort> <fileToTransfer>\n");
		}

		FILE *transferMe = fopen(argv[4],"r");
		if(transferMe == NULL) { handleError("Failed to open specified file!"); }
		
		servIP = argv[2];
		servPort = atoi(argv[3]);
		
		if((clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
		{
			handleError("socket creation failed");
		}

		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = resolveName(servIP);
		servAddr.sin_port = htons(servPort);

		if(connect(clientSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		{
			handleError("connection failed");
		}

		printf("\nTransferring");
		int bytesRead;
		while(!feof(transferMe))
		{
			bytesRead = fread(fileBuffer, 1, FILEBUFSIZE, transferMe);
			send(clientSock, fileBuffer, bytesRead, 0);
			printf(".");

		}

		printf("\nFile transfer complete!\n");

	}
//SERVER MODE
	else if(atoi(argv[1]) == 1)
	{
		char fileIndex[10]; // this holds the current file index in string form
		if(argc != 4)
		{
			handleError("Usage: ./a.out <1> <serverPort> <fileToCreate>\n");
		}

		servPort = atoi(argv[2]);

		FILE *fileToWrite;
		char *fileBaseName = argv[3];
		char fileNameBuffer[64];
		
		signal(SIGINT,exit_with_stats);

		if((servSock= socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)  
		{
			handleError("socket creation failed");
		}

		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(servPort);
		
		if(bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		{
			handleError("binding socket failed");
		}

		if(listen(servSock, MAXPENDING) < 0)
		{
			handleError("listen() failed");
		}

		while(1)
		{

			clientLen = sizeof(clientAddr);
			
			if((clientSock = accept(servSock, (struct sockaddr *) &clientAddr, &clientLen)) < 0)
			{
				handleError("accept() failed");
			}
			strcpy(fileNameBuffer,fileBaseName);
			sprintf(fileIndex, "%d",filesTransferred);
			strcat(fileNameBuffer,fileIndex);
			fileToWrite = fopen(fileNameBuffer,"w");
			transferFile(fileBuffer, fileToWrite, clientSock);
			fclose(fileToWrite);
		}
	}

	else { handleError("Unrecognized mode, bailing out!"); }

	return 0;

}

void handleError(char* message)
{
	perror(message);
	exit(1);
}

void transferFile(char *buffer, FILE *fd, int clientSocket)
{
	int bytesReceived;
	if((bytesReceived = recv(clientSocket, buffer, FILEBUFSIZE, 0)) < 0)
	{
		fprintf(stderr,"Something went wrong with recv(), returning...");
	}
	else 
	{
		while(bytesReceived > 0)
		{
			fwrite(buffer, 1, bytesReceived, fd);
			bytesReceived = recv(clientSocket, buffer, FILEBUFSIZE, 0);
		}
		filesTransferred++;
	}

}

void exit_with_stats(int signum)
{
	printf("\nServer terminated.\nTotal files transferred: %d\n",filesTransferred);
	exit(0);
}

unsigned long resolveName(char name[])
{ 

	struct hostent *host;

	if ((host = gethostbyname(name)) == NULL) 
		{ 
			fprintf(stderr, "gethostbyname() failed"); 
			exit(1);
		}

	return *((unsigned long *) host->h_addr_list [0] ) ; 

}

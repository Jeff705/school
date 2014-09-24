/*
 * Chris Ragan
 * Dr. Remy
 * CPSC 3600
 * Project 1: passwordServer.c
 *
 * Common helper functions for server and breaker both.
 */
#include "includes.h"

/*
 * Pretty self-explanatory.
 */
void DieWithError(char *errMessage) {
	perror(errMessage);
	exit(1);
}

/*
 * Initializes a 62-char array to all possible alphanumeric values, 0-9, a-z, A-Z.
 * Keeps from cluttering up the code with a big ugly array of chars.
 */
void initializeAlphabet(char alphabet[]) {
	int i;
	for(i = 0; i < 62; i++) {
		if((i + 48 < 58)){
			alphabet[i] = (i + 48);
		}

		else if((i + 55) < 91) {
			alphabet[i] = (i + 55);
		}

		else alphabet[i] = i + 61;
	}
}
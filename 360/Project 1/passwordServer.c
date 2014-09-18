#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

/*
setup connection scaffolding
*/


/*
generate a random password of 'n' length

accept a connection
	die with error if it fails

go into handling loop

*/


/*
HANDLING LOOP
	read an incoming password
	compare to existing password
		if unequal, send back 'FAILURE' and wait for next password

		if equal, send back 'SUCCESS' and exit handling loop
*/
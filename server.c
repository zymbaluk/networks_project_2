#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include "gbnpacket.c"

#define PORTNUM 6969




int main(int argc, char *argv[]) {

	if( argc != 4 ) {
		printf("Usage: %s <server name> <output file name> <intentional drop>\n",
			argv[0]);
		exit(1);
	}

	//create socket
	struct hostent *hp;
	//The name of the server we're sending to is given as argv[1], it can 
	//be an ip address, or the colloquial name of the server (ie blanca or shavano)
	if ((hp = gethostbyname(argv[1])) == 0 ) {
		printf("Invalid or unknown host\n");
		exit(1);
	}
	//Create our socket
	int sk;
	if ((sk = socket( PF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Problem creating socket\n");
		exit(1);
	}
	
	FILE *file;
	file = fopen(argv[2], "wb");
	if (file == NULL) {
		printf("file %s not valid!\n",
			argv[2]);
		exit(1);
	}
	
	double loss_rate;
	loss_rate = atof(argv[3]);

	int ld;
	if ((ld = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("problem creating sock\n");
		exit(1);
	}


	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORTNUM);

	if( bind(ld, (struct sockaddr *) &server, sizeof(server)) < 0) {
		printf("Error binding\n");
		exit(1);
	}

	int length = sizeof(server);
	if (getsockname(ld, (struct sockaddr *) &server, &length)<0){
		printf("Error getsockname\n");
		exit(1);
	}

	printf("The server's UDP port number is %d\n", ntohs(server.sin_port));

	return(0);

}













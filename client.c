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

#define TIMEOUT 3
#define RETRIES 10
#define PORTNUM 6969
#define WINDOWGBN 5
#define WINDOWSAW 1

// Usage: 
int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in gb_serv_addr;
	struct sockaddr_in from_addr;
	unsigned short gbn_serv_port;
	unsigned int from_size;
	FILE *file;
	int window_size;

	//ensure we have the right number of args
	if (argc != 4) {
		printf("Usage: %s <file> <gbn|saw> <server-name>\n",
			argv[0]);
		exit(1);
	}

	//Attempt to open file, and ensure it's valid
	file = fopen(argv[1], "rb");
	if (file == NULL) {
		printf("file %s not valid!\n",
			argv[1]);
		exit(1);
	}

	//Determine whether we're using GBN or SAW and swet window_size
	if (strcmp(argv[2],"gbn") != 0 && strcmp(argv[2],"saw") != 0) {
		printf("Error: neither GBN nor SAW specified\n");
		exit(1);
	}
	if (strcmp(argv[2], "gbn") == 0) {
		window_size = WINDOWGBN;
	} 
	if (strcmp(argv[2], "saw") == 0) {
		window_size = WINDOWSAW;
	}

	struct hostent *hp;
	//construct our window of size window_size
	struct gbnpacket window[window_size];

	  if ((hp = gethostbyname(argv[3])) == 0 ) {
	    printf("Invalid or unknown host\n");
	    exit(1);
	  }	

	return 0;
}









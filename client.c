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

// globals
int sequence_number = 0;

//prototypes
struct gbnpacket next_packet(FILE *file); 

// Usage: 
int main(int argc, char *argv[]) {
	//ensure we have the right number of args
	if (argc != 4) {
		printf("Usage: %s <file> <gbn|saw> <server-name>\n",
			argv[0]);
		exit(1);
	}

	FILE *file;
	//Attempt to open file, and ensure it's valid
	file = fopen(argv[1], "rb");
	if (file == NULL) {
		printf("file %s not valid!\n",
			argv[1]);
		exit(1);
	}

	int window_size;
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

	//construct our window of size window_size
	struct gbnpacket window[window_size];

	struct hostent *hp;
	//The name of the server we're sending to is given as argv[3], it can be an ip address, or the colloquial name of the server (ie blanca or shavano)
	if ((hp = gethostbyname(argv[3])) == 0 ) {
		printf("Invalid or unknown host\n");
		exit(1);
	}

	//Create our socket
	int sk;
	if ((sk = socket( PF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Problem creating socket");
		exit(1);
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	
	memcpy( &server.sin_addr.s_addr, hp->h_addr, hp->h_length );
	
	server.sin_port = htons(PORTNUM);

	
	struct gbnpacket packet = next_packet(file);
	printf("contents: %s\n", packet.data);
	
/*
 * 	We should be able to send now through 
 *
 * 	n_sent = sendto(sk,buf,buf_len,0,
 * 	                  (struct sockaddr*) &server,sizeof(server));
 */

	return 0;
}

/*
//Shift our window array over by one
int shift_by_one(gbnpacket *window, window_size, FILE) {
	if(size == 1) {
		//window
	}
}
*/
struct gbnpacket next_packet(FILE *file) {
	struct gbnpacket packet;
	packet.type = 1;
	packet.sequence_number = sequence_number++;
	
	//Now read the data into the packet
	size_t size = fread( packet.data, sizeof(packet.data[0]), PACKET_SIZE, file);
	packet.length = size;

	return packet;
}



















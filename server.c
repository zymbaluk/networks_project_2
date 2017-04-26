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

//prototypes
struct gbnpacket build_ack(int sequence_number); 


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
	srand((unsigned int)time(NULL));

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

	struct sockaddr_in client;
	int n=0;
	int len = sizeof(client);

	struct gbnpacket rsp_packet;
	int sequence_number = 1;
	while(1) {
		struct gbnpacket current_packet;
		n = recvfrom(
			ld, 
			&current_packet,
			sizeof(current_packet),
			0,
			(struct sockaddr *) &client,
			&len);
	
		if(n>1) {
			printf("recieved packet %d %d\n", 
current_packet.sequence_number,
current_packet.type);
			if(current_packet.type == 0) {
				break;
			}

			if((double)rand()/RAND_MAX < loss_rate) {
				continue; //lose this one
			}

			//build acknowledgement packet
			rsp_packet = build_ack(current_packet.sequence_number);
			sendto(
				ld,
				&rsp_packet,
				sizeof(rsp_packet),
				0,
				(struct sockaddr *) &client,
				sizeof(client));
			//
			if(rsp_packet.sequence_number == sequence_number) {
				sequence_number++;
				fwrite(
					current_packet.data,
					sizeof(current_packet.data[0]),
					current_packet.length,
					file);
			}
		}

	}

	return(0);

}

struct gbnpacket build_ack(int sequence_number) {
	struct gbnpacket packet;
	packet.type = 2;
	packet.sequence_number = sequence_number;
	packet.length = 0;
	memset( packet.data, 0, PACKET_SIZE );
	return packet;
}














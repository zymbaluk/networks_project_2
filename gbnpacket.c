//This is the packet which will be sent to and from client and server.
/*
 * Type enum:
 * 	0 - kill packet. show's over
 * 	1 - normal data packet
 * 	2 - name packet. This packet is what you should name the file you're about to recieve
 */
#define PACKET_SIZE 512
struct gbnpacket {
	int type;
	int sequence_number;
	size_t length;
	char data[PACKET_SIZE];
};

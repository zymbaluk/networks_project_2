//This is the packet which will be sent to and from client and server.
struct gbnpacket {
	int type;
	int sequence_number;
	int length;
	char data[512];
};

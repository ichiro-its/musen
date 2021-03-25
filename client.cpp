// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080

// Driver code
int main() {
	char buffer[1024];
	char *hello = (char*)"Hello from client";
	struct sockaddr_in recipient;

	// Creating socket file descriptor
	int socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ < 0)
    {
		fprintf(stderr, "Failure creating socket\n");
		exit(EXIT_FAILURE);
	}

	// memset(&recipient, 0, sizeof(recipient));
	
	// Filling server information
	recipient.sin_family = AF_INET;
	recipient.sin_port = htons(PORT);
	recipient.sin_addr.s_addr = INADDR_ANY;
	
	int n;
    socklen_t len; 
	
	sendto(socket_, (const char *)hello, strlen(hello),
		MSG_CONFIRM, (const struct sockaddr *) &recipient,
		sizeof(recipient));
	printf("Hello message sent from client.\n");
		
	n = recvfrom(socket_, (char *)buffer, 1024,
				MSG_WAITALL, (struct sockaddr *) &recipient,
				&len);
	buffer[n] = '\0';
	printf("Server : %s\n", buffer);

	close(socket_);
	return 0;
}

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080

using namespace std;

int main() {
	char buffer[1024];
	char *hello = (char*)"Hello from client\0";
	struct sockaddr_in recipient;

	// Creating socket 
	int socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ < 0)
    {
		fprintf(stderr, "Failure creating socket\n");
		return false;
	}
	
	// Filling server information
	recipient.sin_family = AF_INET;
	recipient.sin_port = htons(PORT);
	recipient.sin_addr.s_addr = INADDR_ANY;
	
	int count=0;
    socklen_t len; 
	// Start communication
	while(true){
		sendto(socket_, (const char *)hello, strlen(hello),
			MSG_CONFIRM, (const struct sockaddr *) &recipient,
			sizeof(recipient));
		cout << "Hello message sent from client" << endl; 
			
		recvfrom(socket_, (char *)buffer, 1024,
					MSG_WAITALL, (struct sockaddr *) &recipient,
					&len);
		string s(buffer);
        s = s + " " + to_string(count);
        cout << "Server : " << s <<endl;

		count++;
		sleep(1);
	}

	close(socket_);
	return 0;
}

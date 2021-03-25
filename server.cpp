#include <iostream>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8080

using namespace std;

int main(){
    char buffer[1024];
    char *hello = (char*)"Hello from server";
    struct sockaddr_in sa, ca;
    
    int socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ < 0)
    {
		fprintf(stderr, "Failure creating socket\n");
		exit(EXIT_FAILURE);
	}

    // memset(&sa, 0, sizeof(sa));
    // memset(&ca, 0, sizeof(ca));

    sa.sin_family      = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port        = htons(PORT);

    if (bind(socket_, (struct sockaddr *)&sa, sizeof(sa))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int n;
    socklen_t address_len = sizeof(ca);
    n = recvfrom(socket_, (char *)buffer, 1024, MSG_WAITALL, 
                ( struct sockaddr *) &ca, &address_len);
    
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);
    sendto(socket_, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &ca,
            address_len);
    printf("Hello message sent from server.\n"); 
    

    // cout << socket_ << " " << sizeof(sa) <<endl;
}
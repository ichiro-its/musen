#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8080

using namespace std;

int main(){
    string a;
    char buffer[1024];
    char *hello = (char*)"Hello from server\0";
    struct sockaddr_in sa, ca;
    
    // Creating socket 
    int socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ < 0)
    {
		fprintf(stderr, "Failure creating socket\n");
		return false;
	}

    // Filling server information
    sa.sin_family      = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port        = htons(PORT);

    // Bind the socket with the server address
    if (bind(socket_, (struct sockaddr *)&sa, sizeof(sa))<0)
    {
        perror("bind failed");
        return false;
    }

    int count=0;
    socklen_t address_len = sizeof(ca);
    // Start communication
    while(true){

        recvfrom(socket_, (char *)buffer, 1024, MSG_WAITALL, 
                    ( struct sockaddr *) &ca, &address_len);
        string s(buffer);
        s = s + " " + to_string(count);
        cout << "Client : " << s <<endl;

        sendto(socket_, (const char *)hello, strlen(hello), 
            MSG_CONFIRM, (const struct sockaddr *) &ca,
                address_len);
        cout << "Hello message sent from server" << endl; 
        count++;
        sleep(1);
    }
    
    return 0;
}
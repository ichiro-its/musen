// Copyright (c) 2021 Ichiro ITS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>

#define PORT 8080

int main()
{
  char buffer[1024];
  char * hello = const_cast<char *>("Hello from client\0");
  struct sockaddr_in recipient;

  // Creating socket
  int socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socket_ < 0) {
    fprintf(stderr, "Failure creating socket\n");
    return false;
  }

  // Filling server information
  recipient.sin_family = AF_INET;
  recipient.sin_port = htons(PORT);
  recipient.sin_addr.s_addr = INADDR_ANY;

  int count = 0;
  socklen_t len;
  // Start communication
  while (true) {
    sendto(
      socket_, const_cast<char *>(hello), strlen(hello),
      MSG_CONFIRM, (const struct sockaddr *) &recipient,
      sizeof(recipient));
    std::cout << "Hello message sent from client" << std::endl;

    recvfrom(
      socket_, const_cast<char *>(buffer), 1024,
      MSG_WAITALL, (struct sockaddr *) &recipient,
      &len);

    std::string s(buffer);
    s = s + " " + std::to_string(count);
    std::cout << "Server : " << s << std::endl;

    count++;
    sleep(1);
  }

  close(socket_);
  return 0;
}

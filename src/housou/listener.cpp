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

#include <housou/listener.hpp>
#include <iostream>
#include <string>


namespace housou
{
Listener::Listener()
{
  sockfd = -1;
}

bool Listener::connect(int port)
{
  socket_port = port;

  // Creating socket
  sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd < 0) {
    fprintf(stderr, "Failure creating socket\n");
    return false;
  }

  // Filling server information
  recipient.sin_family = AF_INET;
  recipient.sin_addr.s_addr = htonl(INADDR_ANY);
  recipient.sin_port = htons(socket_port);

  return true;
}

void Listener::request(std::string data)
{
  message = const_cast<char *>(data.c_str());

  sendto(
    sockfd, const_cast<char *>(message), strlen(message),
    0, (const struct sockaddr *) &recipient,
    sizeof(recipient));
}

std::string Listener::recover(int length)
{
  char * buffer = new char[length];
  addr_len = sizeof(recipient);

  recvfrom(
    sockfd, const_cast<char *>(buffer), length,
    0, (struct sockaddr *) &recipient,
    &addr_len);

  std::string s(buffer);
  return s;
}

void Listener::close_socket()
{
  close(sockfd);
}

}  // namespace housou

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

#include <housou/broadcaster.hpp>
#include <iostream>
#include <string>

namespace housou
{
Broadcaster::Broadcaster()
{
  socket_ = -1;
}

bool Broadcaster::connect(int port)
{
  port_ = port;

  // Creating socket
  socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socket_ < 0) {
    fprintf(stderr, "Failure creating socket\n");
    return false;
  }

  // Filling server information
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port_);

  // Bind the socket with the server address
  if (bind(socket_, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind failed");
    return false;
  }

  return true;
}

std::string Broadcaster::wait()
{
  addr_len = sizeof(client_addr);

  recvfrom(
    socket_, const_cast<char *>(buffer), 1024, MSG_WAITALL,
    (struct sockaddr *) &client_addr, &addr_len);

  std::string s(buffer);
  return s;
}

void Broadcaster::send(std::string data)
{
  message = const_cast<char *>(data.c_str());

  sendto(
    socket_, message, strlen(message),
    MSG_CONFIRM, (const struct sockaddr *) &client_addr,
    addr_len);
}
}  // namespace housou

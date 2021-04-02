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

#include <housou/broadcaster.hpp>

#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

namespace housou
{

Broadcaster::Broadcaster(int port)
: port(port),
  sockfd(-1)
{
}

Broadcaster::~Broadcaster()
{
  disconnect();
}

bool Broadcaster::connect()
{
  // Failed if already connected
  if (sockfd >= 0) {
    return false;
  }

  // Create a new socket
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd < 0) {
    return false;
  }

  // Enable broadcast
  int opt = 1;
  setsockopt(
    sockfd, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<void *>(&opt),
    sizeof(opt));

  // Enable non-blocking
  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

  // Configure the sender address
  struct sockaddr_in sa;
  {
    memset(reinterpret_cast<void *>(&sa), 0, sizeof(sa));

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(port);
  }

  // Bind the socket with the sender address
  if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
    return false;
  }

  return true;
}

bool Broadcaster::disconnect()
{
  // Failed if not connected
  if (sockfd < 0) {
    return false;
  }

  // Close the socket
  close(sockfd);
  sockfd = -1;

  return true;
}

int Broadcaster::send(std::string data)
{
  // Skip if not connected
  if (sockfd < 0) {
    return 0;
  }

  // Obtain all interfaces
  struct ifaddrs * ifas;
  if (getifaddrs(&ifas) != 0) {
    return 0;
  }

  int lowest_sent = 0;

  // Repeat for all available interfaces
  for (auto & ifa = ifas; ifa != nullptr; ifa = ifa->ifa_next) {
    // Skip if null or if the address family is different
    if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family != AF_INET) {
      continue;
    }

    auto broadaddr = (struct sockaddr_in *)ifa->ifa_broadaddr;

    // Configure the recipent address
    struct sockaddr_in sa;
    {
      memset(reinterpret_cast<void *>(&sa), 0, sizeof(sa));

      sa.sin_family = AF_INET;
      sa.sin_addr.s_addr = ntohl(broadaddr->sin_addr.s_addr);
      sa.sin_port = htons(port);
    }

    // Send data to the recipent address
    int sent = sendto(
      sockfd, const_cast<char *>(data.c_str()), data.size(), 0,
      (struct sockaddr *)&sa, sizeof(sa));

    if (sent < lowest_sent) {
      lowest_sent = sent;
    }
  }

  return lowest_sent;
}

}  // namespace housou

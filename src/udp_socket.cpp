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

#include <housou/udp_socket.hpp>

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

namespace housou
{

UdpSocket::UdpSocket()
: sockfd(-1)
{
}

UdpSocket::~UdpSocket()
{
  disconnect();
}

bool UdpSocket::connect()
{
  if (is_connected()) {
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

  return true;
}

bool UdpSocket::disconnect()
{
  if (!is_connected()) {
    return false;
  }

  // Close the socket
  close(sockfd);
  sockfd = -1;

  return true;
}

bool UdpSocket::is_connected()
{
  return sockfd >= 0;
}

}  // namespace housou
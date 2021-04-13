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

#include <housou/listener/base_listener.hpp>

#include <arpa/inet.h>
#include <string.h>

namespace housou
{

BaseListener::BaseListener(int port)
: UdpSocket(),
  port(port)
{
}

bool BaseListener::connect()
{
  if (!UdpSocket::connect()) {
    return false;
  }

  // Configure the recipent address
  struct sockaddr_in sa;
  {
    memset(reinterpret_cast<void *>(&sa), 0, sizeof(sa));

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(port);
  }

  // Bind the socket with the recipent address
  if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
    return false;
  }

  return true;
}

int BaseListener::receive(void * buffer, int length)
{
  if (!is_connected()) {
    return 0;
  }

  struct sockaddr sa;
  socklen_t sa_len = sizeof(sa);

  // Receive data
  int received = recvfrom(sockfd, buffer, length, 0, &sa, &sa_len);

  return received;
}

int BaseListener::get_port()
{
  return port;
}

}  // namespace housou

// Copyright (c) 2021 ICHIRO ITS
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

#include <musen/listener/base_listener.hpp>

#include <arpa/inet.h>

#include <algorithm>
#include <cstring>
#include <memory>

namespace musen
{

BaseListener::BaseListener(const int & port, std::shared_ptr<UdpSocket> udp_socket)
: udp_socket(udp_socket),
  port(port)
{
}

bool BaseListener::connect()
{
  if (!udp_socket->connect()) {
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
  if (bind(udp_socket->get_sockfd(), (struct sockaddr *)&sa, sizeof(sa)) < 0) {
    return false;
  }

  return true;
}

bool BaseListener::disconnect()
{
  return udp_socket->disconnect();
}

int BaseListener::receive(void * buffer, const int & length)
{
  if (!udp_socket->is_connected() || length <= 0) {
    return 0;
  }

  struct sockaddr sa;
  socklen_t sa_len = sizeof(sa);

  // Receive data
  int received = recvfrom(udp_socket->get_sockfd(), buffer, length, 0, &sa, &sa_len);

  return std::max(received, 0);
}

std::shared_ptr<UdpSocket> BaseListener::get_udp_socket() const
{
  return udp_socket;
}

const int & BaseListener::get_port() const
{
  return port;
}

}  // namespace musen

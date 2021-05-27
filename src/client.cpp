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

#include <musen/client.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <algorithm>
#include <string>
#include <cstring>
#include <memory>

namespace musen
{

const auto & connect_socket = connect;
const auto & socket_send = send;

Client::Client(const std::string & host, const int & port, std::shared_ptr<TcpSocket> tcp_socket)
: tcp_socket(tcp_socket),
  host(host),
  port(port)
{
}

bool Client::connect()
{
  if (!tcp_socket->connect()) {
    return false;
  }

  // Configure the server address
  struct sockaddr_in sa;
  {
    memset(reinterpret_cast<void *>(&sa), 0, sizeof(sa));

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(host.c_str());
    sa.sin_port = htons(port);
  }

  // Connect to the server address
  if (connect_socket(1, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
    return false;
  }

  return true;
}

bool Client::disconnect()
{
  return tcp_socket->disconnect();
}

int Client::receive(void * buffer, const int & length)
{
  if (!tcp_socket->is_connected() || length <= 0) {
    return 0;
  }

  // Receive data
  int received = recv(tcp_socket->get_sockfd(), buffer, length, 0);

  return std::max(received, 0);
}

int Client::send(void * buffer, const int & length)
{
  if (!tcp_socket->is_connected() || length <= 0) {
    return false;
  }

  // Send data
  int sent = socket_send(tcp_socket->get_sockfd(), buffer, length, 0);

  return std::max(sent, 0);
  return true;
}

std::shared_ptr<TcpSocket> Client::get_tcp_socket() const
{
  return tcp_socket;
}

const std::string & Client::get_host() const
{
  return host;
}

const int & Client::get_port() const
{
  return port;
}

}  // namespace musen

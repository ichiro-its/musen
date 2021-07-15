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

#include <musen/tcp/server.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <algorithm>
#include <cstring>
#include <memory>
#include <string>

namespace musen
{

constexpr auto socket_accept = accept;

Server::Server(const int & port, std::shared_ptr<Socket> socket)
: socket(socket),
  connected(false),
  port(port)
{
}

bool Server::connect()
{
  if (is_connected()) {
    return false;
  }

  // Obtain the server address
  Address address(obtain_client_sa());

  // Bind the socket to the server address
  socket->bind(address);

  // Listen to incoming connection
  if (listen(socket->get_fd(), 3) < 0) {
    return false;
  }

  connected = true;

  return true;
}

bool Server::disconnect()
{
  if (!is_connected()) {
    return false;
  }

  socket = nullptr;
  connected = false;

  return true;
}

std::shared_ptr<Session> Server::accept()
{
  if (!is_connected()) {
    return nullptr;
  }

  // Obtain the server address
  auto sa = obtain_client_sa();
  auto sa_size = sizeof(sa);

  // Accept incoming connection
  auto sockfd = socket_accept(
    socket->get_fd(), (struct sockaddr *)&sa, reinterpret_cast<socklen_t *>(&sa_size));

  if (sockfd < 0) {
    return nullptr;
  }

  auto socket = std::make_shared<Socket>(sockfd);

  return std::make_shared<Session>(socket);
}

std::shared_ptr<Socket> Server::get_socket() const
{
  return socket;
}

bool Server::is_connected() const
{
  return connected;
}

const int & Server::get_port() const
{
  return port;
}

struct sockaddr_in Server::obtain_client_sa() const
{
  struct sockaddr_in sa;

  memset(reinterpret_cast<void *>(&sa), 0, sizeof(sa));

  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = htonl(INADDR_ANY);
  sa.sin_port = htons(port);

  return sa;
}

}  // namespace musen

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

#include <memory>

namespace musen
{

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

  // Bind the socket with the serve address
  socket->bind(make_any_address(port));

  // Listen to incoming connection
  socket->listen();

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

  // Accept incoming connection
  auto session_socket = socket->accept();

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

}  // namespace musen

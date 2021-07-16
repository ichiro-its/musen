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

#include <fcntl.h>
#include <musen/tcp/server.hpp>

#include <memory>

namespace musen
{

Server::Server(const int & port, std::shared_ptr<Socket> socket)
: socket(socket),
  port(port)
{
  // Bind the socket with the serve address
  socket->bind(make_any_address(port));

  // Listen to incoming connections
  socket->listen();
}

Server::~Server()
{
  socket = nullptr;
}

std::shared_ptr<Session> Server::accept()
{
  try {
    // Accept incoming connection
    auto session_socket = socket->accept();

    // Set the session socket's non-blocking status according to the server socket's
    auto was_nonblock = socket->get_status_flag(O_NONBLOCK);
    session_socket->set_status_flag(O_NONBLOCK, was_nonblock);

    return std::make_shared<Session>(session_socket);
  } catch (const std::system_error & err) {
    if (err.code().value() == EAGAIN) {
      return nullptr;
    }

    throw err;
  }
}

std::shared_ptr<Socket> Server::get_socket() const
{
  return socket;
}

const int & Server::get_port() const
{
  return port;
}

}  // namespace musen

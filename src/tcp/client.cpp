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

#include <musen/tcp/client.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

namespace musen
{

constexpr auto connect_socket = connect;
constexpr auto socket_send = send;

Client::Client(
  const Address & server_address, std::shared_ptr<Socket> socket)
: socket(socket),
  connected(false),
  server_address(server_address)
{
}

bool Client::connect()
{
  if (is_connected()) {
    return false;
  }

  socket->connect(server_address);

  connected = true;

  return true;
}

bool Client::disconnect()
{
  if (!is_connected()) {
    return false;
  }

  socket = nullptr;
  connected = false;

  return true;
}

size_t Client::send_raw(const char * data, const size_t & length)
{
  if (!is_connected() || length <= 0) {
    return 0;
  }

  // Send data
  int sent = socket_send(socket->get_fd(), data, length, 0);

  return std::max(sent, 0);
}

size_t Client::receive_raw(char * data, const size_t & length)
{
  if (!is_connected() || length <= 0) {
    return 0;
  }

  return socket->receive(data, length);
}

std::shared_ptr<Socket> Client::get_socket() const
{
  return socket;
}

bool Client::is_connected() const
{
  return connected;
}

const Address & Client::get_server_address() const
{
  return server_address;
}

}  // namespace musen

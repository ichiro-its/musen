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

#include <musen/udp/listener.hpp>

#include <memory>

namespace musen
{

Listener::Listener(const int & port, std::shared_ptr<Socket> socket)
: socket(socket),
  port(port)
{
  // Enable reuse port
  socket->set_option(SO_REUSEPORT, 1);

  // Bind the socket with the listen address
  socket->bind(make_any_address(port));
}

Listener::~Listener()
{
  socket = nullptr;
}

size_t Listener::receive_raw(char * data, const size_t & length)
{
  if (length <= 0) {
    return 0;
  }

  return socket->receive(data, length);
}

std::shared_ptr<Socket> Listener::get_socket() const
{
  return socket;
}

const int & Listener::get_port() const
{
  return port;
}

}  // namespace musen

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

#include <musen/tcp/session.hpp>

#include <memory>

namespace musen
{

Session::Session(std::shared_ptr<Socket> socket)
: socket(socket)
{
}

Session::~Session()
{
  socket = nullptr;
}

size_t Session::send_raw(const char * data, const size_t & length)
{
  return socket->send(data, length);
}

size_t Session::receive_raw(char * data, const size_t & length)
{
  try {
    return socket->receive(data, length);
  } catch (const std::system_error & err) {
    switch (err.code().value()) {
      case ENOTCONN:
        return 0;
    }

    throw err;
  }
}

std::shared_ptr<Socket> Session::get_socket() const
{
  return socket;
}

}  // namespace musen

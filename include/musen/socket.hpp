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

#ifndef MUSEN__SOCKET_HPP_
#define MUSEN__SOCKET_HPP_

#include <memory>

namespace musen
{

class Socket;

std::shared_ptr<Socket> make_tcp_socket();
std::shared_ptr<Socket> make_udp_socket();

class Socket
{
public:
  explicit Socket(const int & fd);
  Socket(const int & domain, const int & type, const int & protocol);

  ~Socket();

  void set_status_flags(const int & flags);
  int get_status_flags() const;

  void enable_non_blocking();
  void disable_non_blocking();

  bool is_non_blocking() const;

  const int & get_fd() const;

private:
  int fd;
};

}  // namespace musen

#endif  // MUSEN__SOCKET_HPP_
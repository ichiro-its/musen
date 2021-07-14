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

#include <arpa/inet.h>
#include <fcntl.h>
#include <musen/socket.hpp>
#include <sys/socket.h>
#include <unistd.h>

#include <memory>
#include <system_error>

namespace musen
{

std::shared_ptr<Socket> make_tcp_socket(const bool & non_blocking)
{
  auto type = SOCK_STREAM | (non_blocking ? SOCK_NONBLOCK : 0);
  auto socket = std::make_shared<Socket>(AF_INET, type, IPPROTO_IP);

  return socket;
}

std::shared_ptr<Socket> make_udp_socket(const bool & non_blocking)
{
  auto type = SOCK_DGRAM | (non_blocking ? SOCK_NONBLOCK : 0);
  auto socket = std::make_shared<Socket>(AF_INET, type, IPPROTO_IP);

  // Enable broadcast
  int opt = 1;
  setsockopt(
    socket->get_fd(), SOL_SOCKET, SO_BROADCAST, reinterpret_cast<void *>(&opt),
    sizeof(opt));

  return socket;
}

Socket::Socket(const int & fd)
: fd(fd)
{
  int error;
  socklen_t error_len = sizeof(error);

  if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &error_len) == -1) {
    throw std::system_error(errno, std::generic_category());
  }

  if (error != 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

Socket::Socket(const int & domain, const int & type, const int & protocol)
{
  fd = socket(domain, type, protocol);
  if (fd == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}

Socket::~Socket()
{
  close(fd);
}

void Socket::set_status_flags(const int & flags)
{
  if (fcntl(fd, F_SETFL, flags) == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}

int Socket::get_status_flags() const
{
  auto flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    throw std::system_error(errno, std::generic_category());
  }

  return flags;
}

void Socket::set_non_blocking(const bool & enable)
{
  auto flags = get_status_flags();
  set_status_flags(enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK));
}

bool Socket::is_non_blocking() const
{
  return get_status_flags() & O_NONBLOCK;
}

const int & Socket::get_fd() const
{
  return fd;
}

}  // namespace musen

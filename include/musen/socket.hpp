// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <sys/socket.h>

#include <memory>
#include <system_error>

#include "musen/address.hpp"

namespace musen {

class Socket;

std::shared_ptr<Socket> make_tcp_socket();
std::shared_ptr<Socket> make_udp_socket();

std::shared_ptr<Socket> make_blocking_tcp_socket();
std::shared_ptr<Socket> make_blocking_udp_socket();

class Socket {
 public:
  explicit Socket(const int & fd);
  Socket(const int & domain, const int & type, const int & protocol);

  ~Socket();

  void bind(const Address & address);
  void connect(const Address & address);
  void listen(const int & max_queue = 8);

  std::shared_ptr<Socket> accept();

  size_t send(const void * data, const size_t & length);
  size_t send_to(const void * data, const size_t & length, const Address & address);

  size_t receive(void * data, const size_t & length);

  void set_status_flags(const int & flags);
  int get_status_flags() const;

  void set_status_flag(const int & key, const bool & enable);
  bool get_status_flag(const int & key) const;

  template<typename T>
  void set_option(const int & key, const T & value);

  template<typename T>
  T get_option(const int & key) const;

  const int & get_fd() const;

 private:
  int fd;
};

template<typename T>
void Socket::set_option(const int & key, const T & value) {
  if (setsockopt(fd, SOL_SOCKET, key, &value, sizeof(value)) == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}

template<typename T>
T Socket::get_option(const int & key) const {
  T value;
  socklen_t value_len = sizeof(value);
  if (getsockopt(fd, SOL_SOCKET, key, &value, &value_len) == -1) {
    throw std::system_error(errno, std::generic_category());
  }

  return value;
}

}  // namespace musen

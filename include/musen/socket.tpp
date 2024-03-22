// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <sys/socket.h>
#include <system_error>

#include "musen/socket.hpp"

namespace musen {

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

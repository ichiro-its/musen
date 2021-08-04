// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace musen {

class Receiver {
 public:
  virtual size_t receive_raw(char * data, const size_t & length);

  std::string receive_string(const size_t & length);
  std::vector<std::string> receive_strings(
    const size_t & length, const std::string & delimiter = ",");

  template<typename T>
  std::optional<T> receive();
};

template<typename T>
std::optional<T> Receiver::receive() {
  T data;

  auto received = receive_raw(reinterpret_cast<char *>(&data), sizeof(T));

  if (received < sizeof(T)) {
    return std::nullopt;
  }

  return std::make_optional<T>(std::move(data));
}

}  // namespace musen

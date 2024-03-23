// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <string>
#include <vector>

namespace musen {

class Sender {
 public:
  virtual size_t send_raw(const char * data, const size_t & length);

  size_t send_string(const std::string & data);
  size_t send_strings(const std::vector<std::string> & data, const std::string & delimiter = ",");

  template<typename T>
  size_t send(const T & data);
};

}  // namespace musen

#include "musen/sender.tpp"

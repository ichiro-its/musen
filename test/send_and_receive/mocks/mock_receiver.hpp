// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <algorithm>
#include <memory>
#include <queue>
#include <vector>

#include "musen/musen.hpp"

namespace musen_test {

using Buffers = std::queue<std::vector<char>>;

class MockReceiver : public musen::Receiver {
 public:
  explicit MockReceiver(std::shared_ptr<Buffers> buffers)
    : buffers(buffers) {
  }

  size_t receive_raw(char * data, const size_t & length) override {
    if (buffers->empty()) {
      return 0;
    }

    auto buffer = buffers->front();
    buffers->pop();

    if (length >= buffer.size()) {
      std::copy(buffer.begin(), buffer.end(), data);
      return buffer.size();
    } else {
      std::copy_n(buffer.begin(), length, data);
      return length;
    }
  }

 private:
  std::shared_ptr<Buffers> buffers;
};

}  // namespace musen_test

// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "musen/musen.hpp"

namespace musen_test {

using Buffers = std::queue<std::vector<char>>;

class MockSender : public musen::Sender {
 public:
  explicit MockSender(std::shared_ptr<Buffers> buffers)
    : buffers(buffers) {
  }

  size_t send_raw(const char * data, const size_t & length) override {
    std::vector buffer(data, data + length);
    buffers->push(buffer);

    return buffer.size();
  }

 private:
  std::shared_ptr<Buffers> buffers;
};

}  // namespace musen_test

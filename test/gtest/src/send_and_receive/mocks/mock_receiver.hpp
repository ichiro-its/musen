// Copyright (c) 2021-2024 ICHIRO ITS
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

class MockReceiver : public musen::Receiver {
 public:
  explicit MockReceiver(std::shared_ptr<Buffers> buffers);

  size_t receive_raw(char * data, const size_t & length) override;

 private:
  std::shared_ptr<Buffers> buffers;
};

}  // namespace musen_test

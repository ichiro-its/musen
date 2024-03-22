// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <algorithm>

#include "./mock_receiver.hpp"

namespace musen_test {

MockReceiver::MockReceiver(std::shared_ptr<Buffers> buffers) {
  this->buffers = buffers;
}

size_t MockReceiver::receive_raw(char * data, const size_t & length) {
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

}  // namespace musen_test

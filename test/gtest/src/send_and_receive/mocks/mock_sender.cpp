// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "./mock_sender.hpp"

namespace musen_test {

MockSender::MockSender(std::shared_ptr<Buffers> buffers) {
  this->buffers = buffers;
}

size_t MockSender::send_raw(const char * data, const size_t & length) {
  std::vector buffer(data, data + length);
  buffers->push(buffer);

  return buffer.size();
}

}  // namespace musen_test

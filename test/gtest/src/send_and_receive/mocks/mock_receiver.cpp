#include <algorithm>

#include "./mock_receiver.hpp"

namespace musen_test {

MockReceiver::MockReceiver(std::shared_ptr<Buffers> buffers) {
  this->buffers = buffers;
}

size_t MockReceiver::receive_raw(char * data, size_t length) {
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

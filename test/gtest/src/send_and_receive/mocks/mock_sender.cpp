#include "./mock_sender.hpp"

namespace musen_test {

MockSender::MockSender(std::shared_ptr<Buffers> buffers) {
  this->buffers = buffers;
}

size_t MockSender::send_raw(const char * data, size_t length) {
  std::vector buffer(data, data + length);
  buffers->push(buffer);

  return buffer.size();
}

}  // namespace musen_test

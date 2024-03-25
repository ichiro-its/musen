#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "musen/musen.hpp"

namespace musen_test {

using Buffers = std::queue<std::vector<char>>;

class MockSender : public musen::Sender {
 public:
  explicit MockSender(std::shared_ptr<Buffers> buffers);

  size_t send_raw(const char * data, const size_t & length) override;

 private:
  std::shared_ptr<Buffers> buffers;
};

}  // namespace musen_test

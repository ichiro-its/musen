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

  size_t receive_raw(char * data, size_t length) override;

 private:
  std::shared_ptr<Buffers> buffers;
};

}  // namespace musen_test

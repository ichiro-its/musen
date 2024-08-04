#pragma once

#include <optional>
#include <string>
#include <vector>

namespace musen {

class Receiver {
 public:
  virtual size_t receive_raw(char * data, size_t length);

  std::string receive_string(size_t length);
  std::vector<std::string> receive_strings(size_t length, const std::string & delimiter = ",");

  template<typename T>
  std::optional<T> receive();
};

}  // namespace musen

#include "musen/receiver.tpp"

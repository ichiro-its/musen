#pragma once

#include <string>
#include <vector>

namespace musen {

class Sender {
 public:
  virtual size_t send_raw(const char * data, size_t length);

  size_t send_string(const std::string & data);
  size_t send_strings(const std::vector<std::string> & data, const std::string & delimiter = ",");

  template<typename T>
  size_t send(const T & data);
};

}  // namespace musen

#include "musen/sender.tpp"

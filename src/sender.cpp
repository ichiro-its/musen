// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <string>
#include <vector>

#include "musen/sender.hpp"

namespace musen {

size_t Sender::send_raw(const char * /*data*/, const size_t & /*length*/) {
  return 0;
}

size_t Sender::send_string(const std::string & data) {
  return send_raw(data.c_str(), data.size());
}

size_t Sender::send_strings(
  const std::vector<std::string> & data, const std::string & delimiter) {
  // Merge data using the delimiter
  std::string merged_data = "";
  for (size_t i = 0; i < data.size(); ++i) {
    merged_data += data[i];
    if (i != data.size() - 1) {
      merged_data += delimiter;
    }
  }

  // Add a string termination if it doesn't contain one
  if (merged_data[merged_data.size() - 1] != '\0') {
    merged_data += '\0';
  }

  return send_string(merged_data);
}

}  // namespace musen

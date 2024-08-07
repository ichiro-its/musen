#include <string>
#include <vector>

#include "musen/receiver.hpp"

namespace musen {

size_t Receiver::receive_raw(char * /*data*/, size_t /*length*/) {
  return 0;
}

std::string Receiver::receive_string(size_t length) {
  std::string data;
  if (length <= 0) {
    return data;
  }

  // Resize data according to the requested size
  data.resize(length);

  auto received = receive_raw(&data[0], length);

  // Resize data according to the received bytes size
  data.resize(received);

  // Add a string termination if it doesn't contain one
  if (data[data.size() - 1] != '\0') {
    data += '\0';
  }

  return data;
}

std::vector<std::string> Receiver::receive_strings(size_t length, const std::string & delimiter) {
  auto data = receive_string(length);

  std::vector<std::string> splitted_data;

  // split the received data by the delimiter
  size_t pos = 0;
  while ((pos = data.find(delimiter)) != std::string::npos) {
    splitted_data.push_back(data.substr(0, pos));
    data.erase(0, pos + delimiter.length());
  }

  // Insert the remaining token into vector of strings
  if (data.length() > 0) {
    splitted_data.push_back(data);
  }

  return splitted_data;
}

}  // namespace musen

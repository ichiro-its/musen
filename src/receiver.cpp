// Copyright (c) 2021 ICHIRO ITS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <string>
#include <vector>

#include "musen/receiver.hpp"

namespace musen {

size_t Receiver::receive_raw(char * /*data*/, const size_t & /*length*/) {
  return 0;
}

std::string Receiver::receive_string(const size_t & length) {
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

std::vector<std::string> Receiver::receive_strings(
  const size_t & length, const std::string & delimiter) {
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

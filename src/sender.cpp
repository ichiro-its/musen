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

#include <musen/sender.hpp>

#include <string>
#include <vector>

namespace musen
{

size_t Sender::send_raw(const char * /*data*/, const size_t & /*length*/)
{
  return 0;
}

size_t Sender::send_string(const std::string & data)
{
  return send_raw(data.c_str(), data.size());
}

size_t Sender::send_strings(
  const std::vector<std::string> & data, const std::string & delimiter)
{
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

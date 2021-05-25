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

#include <musen/broadcaster/string_broadcaster.hpp>

#include <memory>
#include <string>
#include <vector>

namespace musen
{

StringBroadcaster::StringBroadcaster(const int & port, std::shared_ptr<UdpSocket> udp_socket)
: BaseBroadcaster(port, udp_socket)
{
}

int StringBroadcaster::send(const std::string & message)
{
  return BaseBroadcaster::send(message.c_str(), message.size());
}

int StringBroadcaster::send(
  const std::vector<std::string> & messages, const std::string & delimiter)
{
  // Merge vector of strings using the delimiter
  std::string merged_message = "";
  for (size_t i = 0; i < messages.size(); ++i) {
    merged_message += messages[i];
    if (i != messages.size() - 1) {
      merged_message += delimiter;
    }
  }

  // Add a string termination if it doesn't contain one
  if (merged_message[merged_message.size() - 1] != '\0') {
    merged_message += '\0';
  }

  return send(merged_message);
}

}  // namespace musen

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

#include <musen/listener/string_listener.hpp>

#include <memory>
#include <string>
#include <vector>

namespace musen
{

StringListener::StringListener(const int & port, std::shared_ptr<UdpSocket> udp_socket)
: BaseListener(port, udp_socket)
{
}

std::string StringListener::receive(const int & length)
{
  std::string message;
  if (length <= 0) {
    return message;
  }

  // Resize message buffer according to the requested size
  message.resize(length);

  int received = BaseListener::receive(&message[0], length);

  // Resize message according to the received bytes
  message.resize(received);

  // Add a string termination if it doesn't contain one
  if (message[message.size() - 1] != '\0') {
    message += '\0';
  }

  return message;
}

std::vector<std::string> StringListener::receive(const int & length, const std::string & delimiter)
{
  std::vector<std::string> messages;

  auto received_message = receive(length);

  // Separate the received message by the delimiter
  size_t pos = 0;
  while ((pos = received_message.find(delimiter)) != std::string::npos) {
    messages.push_back(received_message.substr(0, pos));
    received_message.erase(0, pos + delimiter.length());
  }

  // Insert the remaining token into vector of strings
  if (received_message.length() > 0) {
    messages.push_back(received_message);
  }

  return messages;
}

}  // namespace musen

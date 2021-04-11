// Copyright (c) 2021 Ichiro ITS
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

#ifndef HOUSOU__LISTENER__STRING_LISTENER_HPP_
#define HOUSOU__LISTENER__STRING_LISTENER_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "./base_listener.hpp"

namespace housou
{

class StringListener : public BaseListener
{
public:
  explicit StringListener(int port)
  : BaseListener(port)
  {
  }

  std::string receive(int length)
  {
    char * buffer = new char[length];

    BaseListener::receive(buffer, length);

    std::string message(buffer);
    delete[] buffer;

    return message;
  }

  std::vector<std::string> receive(int length, std::string delimiter)
  {
    std::vector<std::string> message;

    std::string received_message = StringListener::receive(length);

    size_t pos = 0;

    while ((pos = received_message.find(delimiter)) != std::string::npos) {
      message.push_back(received_message.substr(0, pos));
      received_message.erase(0, pos + delimiter.length());
    }

    return message;
  }
};

}  // namespace housou

#endif  // HOUSOU__LISTENER__STRING_LISTENER_HPP_

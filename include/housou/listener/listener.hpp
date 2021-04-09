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

#ifndef HOUSOU__LISTENER__LISTENER_HPP_
#define HOUSOU__LISTENER__LISTENER_HPP_

#include <memory>

#include "./base_listener.hpp"

namespace housou
{

template<typename T>
class Listener : public BaseListener
{
public:
  explicit Listener(int port)
  : BaseListener(port)
  {
  }

  std::shared_ptr<T> receive()
  {
    auto data = std::make_shared<T>();

    int received = BaseListener::receive(data.get(), sizeof(T));

    if (received < (signed)sizeof(T)) {
      return nullptr;
    }

    return data;
  }
};

}  // namespace housou

#endif  // HOUSOU__LISTENER__LISTENER_HPP_

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

#ifndef MUSEN_TEST__MOCK_RECEIVER_HPP_
#define MUSEN_TEST__MOCK_RECEIVER_HPP_

#include <musen/musen.hpp>

#include <algorithm>
#include <memory>
#include <queue>
#include <vector>

namespace musen_test
{

using Buffers = std::queue<std::vector<char>>;

class MockReceiver : public musen::Receiver
{
public:
  explicit MockReceiver(std::shared_ptr<Buffers> buffers)
  : buffers(buffers)
  {
  }

  size_t receive_raw(char * data, const size_t & length) override
  {
    if (buffers->empty()) {
      return 0;
    }

    auto buffer = buffers->front();
    buffers->pop();

    if (length >= buffer.size()) {
      std::copy(buffer.begin(), buffer.end(), data);
      return buffer.size();
    } else {
      std::copy_n(buffer.begin(), length, data);
      return length;
    }
  }

private:
  std::shared_ptr<Buffers> buffers;
};

}  // namespace musen_test

#endif  // MUSEN_TEST__MOCK_RECEIVER_HPP_

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

#include <gtest/gtest.h>
#include <musen/musen.hpp>

#include <algorithm>
#include <memory>
#include <queue>
#include <utility>
#include <vector>

using Buffers = std::queue<std::vector<char>>;

class MockSender : public musen::Sender
{
public:
  explicit MockSender(std::shared_ptr<Buffers> buffers)
  : buffers(buffers)
  {
  }

  size_t send_raw(const char * data, const size_t & length) override
  {
    std::vector buffer(data, data + length);
    buffers->push(buffer);

    return buffer.size();
  }

private:
  std::shared_ptr<Buffers> buffers;
};

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

class SendAndReceiveTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    auto buffers = std::make_shared<Buffers>();

    sender = std::make_shared<MockSender>(buffers);
    receiver = std::make_shared<MockReceiver>(buffers);
  }

  std::shared_ptr<MockSender> sender;
  std::shared_ptr<MockReceiver> receiver;
};

TEST_F(SendAndReceiveTest, RawData) {
  char send_data[6] = {'A', 'B', 'C', 'D', 'E', 'F'};
  char receive_data[6];

  auto sent = sender->send_raw(send_data, 6);
  ASSERT_EQ(sent, 6u) << "Must sent 6 bytes of data";

  auto received = receiver->receive_raw(receive_data, 6);
  ASSERT_EQ(received, 6u) << "Must received 6 bytes of data";

  for (size_t i = 0; i < 6; ++i) {
    ASSERT_EQ(send_data[i], receive_data[i]) << "Sent and received data differs at index " << i;
  }
}

TEST_F(SendAndReceiveTest, RawDataReceiveLess) {
  char send_data[6] = {'A', 'B', 'C', 'D', 'E', 'F'};
  char receive_data[4];

  auto sent = sender->send_raw(send_data, 6);
  ASSERT_EQ(sent, 6u) << "Must sent 6 bytes of data";

  auto received = receiver->receive_raw(receive_data, 4);
  ASSERT_EQ(received, 4u) << "Must received 4 bytes of data";

  for (size_t i = 0; i < 4; ++i) {
    ASSERT_EQ(send_data[i], receive_data[i]) << "Sent and received data differs at index " << i;
  }
}

TEST_F(SendAndReceiveTest, RawDataSendLess) {
  char send_data[6] = {'A', 'B', 'C', 'D', 'E', 'F'};
  char receive_data[6];

  auto sent = sender->send_raw(send_data, 4);
  ASSERT_EQ(sent, 4u) << "Must sent 4 bytes of data";

  auto received = receiver->receive_raw(receive_data, 6);
  ASSERT_EQ(received, 4u) << "Must received 4 bytes of data";

  for (size_t i = 0; i < 4; ++i) {
    ASSERT_EQ(send_data[i], receive_data[i]) << "Sent and received data differs at index " << i;
  }
}

TEST_F(SendAndReceiveTest, RawDataReceiveNothing) {
  char receive_data[6];

  auto received = receiver->receive_raw(receive_data, 6);
  ASSERT_EQ(received, 0u) << "Must received 0 bytes of data";
}

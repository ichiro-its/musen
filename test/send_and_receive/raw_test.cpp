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
#include <musen_test/mock_receiver.hpp>
#include <musen_test/mock_sender.hpp>

#include <memory>

class SendAndReceiveRawTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    auto buffers = std::make_shared<musen_test::Buffers>();

    sender = std::make_shared<musen_test::MockSender>(buffers);
    receiver = std::make_shared<musen_test::MockReceiver>(buffers);
  }

  std::shared_ptr<musen_test::MockSender> sender;
  std::shared_ptr<musen_test::MockReceiver> receiver;

  char send_data[6] = {'A', 'B', 'C', 'D', 'E', 'F'};
  char receive_data[6];
};

TEST_F(SendAndReceiveRawTest, ReceiveAll) {
  auto sent = sender->send_raw(send_data, 6);
  ASSERT_EQ(sent, 6u) << "Must sent 6 bytes of data";

  auto received = receiver->receive_raw(receive_data, 6);
  ASSERT_EQ(received, 6u) << "Must received 6 bytes of data";

  for (size_t i = 0; i < 6; ++i) {
    ASSERT_EQ(receive_data[i], send_data[i]) << "Received and sent data differs at index " << i;
  }
}

TEST_F(SendAndReceiveRawTest, ReceiveLess) {
  auto sent = sender->send_raw(send_data, 6);
  ASSERT_EQ(sent, 6u) << "Must sent 6 bytes of data";

  auto received = receiver->receive_raw(receive_data, 4);
  ASSERT_EQ(received, 4u) << "Must received 4 bytes of data";

  for (size_t i = 0; i < 4; ++i) {
    ASSERT_EQ(receive_data[i], send_data[i]) << "Received and sent data differs at index " << i;
  }
}

TEST_F(SendAndReceiveRawTest, SendLess) {
  auto sent = sender->send_raw(send_data, 4);
  ASSERT_EQ(sent, 4u) << "Must sent 4 bytes of data";

  auto received = receiver->receive_raw(receive_data, 6);
  ASSERT_EQ(received, 4u) << "Must received 4 bytes of data";

  for (size_t i = 0; i < 4; ++i) {
    ASSERT_EQ(receive_data[i], send_data[i]) << "Received and sent data differs at index " << i;
  }
}

TEST_F(SendAndReceiveRawTest, ReceiveNothing) {
  auto received = receiver->receive_raw(receive_data, 6);
  ASSERT_EQ(received, 0u) << "Must received 0 bytes of data";
}

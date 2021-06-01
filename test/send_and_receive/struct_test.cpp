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

class SendAndReceiveStructTest : public ::testing::Test
{
protected:
  struct Foo
  {
    int a;
    float b;
  };

  void SetUp() override
  {
    auto buffers = std::make_shared<musen_test::Buffers>();

    sender = std::make_shared<musen_test::MockSender>(buffers);
    receiver = std::make_shared<musen_test::MockReceiver>(buffers);
  }

  std::shared_ptr<musen_test::MockSender> sender;
  std::shared_ptr<musen_test::MockReceiver> receiver;

  Foo send_data = {32, 0.125};
};

TEST_F(SendAndReceiveStructTest, ReceiveAll) {
  sender->send<Foo>(send_data);
  auto receive_data = receiver->receive<Foo>();

  ASSERT_TRUE(receive_data.has_value()) << "Received data must not be empty";

  ASSERT_EQ(receive_data->a, send_data.a) << "Sent and received data differs at a";
  ASSERT_DOUBLE_EQ(receive_data->b, send_data.b) << "Sent and received data differs at b";
}

TEST_F(SendAndReceiveStructTest, ReceiveNothing) {
  auto receive_data = receiver->receive<Foo>();
  ASSERT_FALSE(receive_data.has_value()) << "Must received nothing";
}

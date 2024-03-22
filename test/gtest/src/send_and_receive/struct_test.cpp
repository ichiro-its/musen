// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <memory>

#include "gtest/gtest.h"
#include "./mocks/mock_receiver.hpp"
#include "./mocks/mock_sender.hpp"

class SendAndReceiveStructTest : public ::testing::Test {
 protected:
  struct Foo {
    int a;
    float b;
  };

  void SetUp() override {
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

  ASSERT_EQ(receive_data->a, send_data.a) << "Received and sent data differs at a";
  ASSERT_DOUBLE_EQ(receive_data->b, send_data.b) << "Received and sent data differs at b";
}

TEST_F(SendAndReceiveStructTest, ReceiveNothing) {
  auto receive_data = receiver->receive<Foo>();
  ASSERT_FALSE(receive_data.has_value()) << "Must received nothing";
}

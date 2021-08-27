// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "./mocks/mock_receiver.hpp"
#include "./mocks/mock_sender.hpp"

class SendAndReceiveStringTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto buffers = std::make_shared<musen_test::Buffers>();

    sender = std::make_shared<musen_test::MockSender>(buffers);
    receiver = std::make_shared<musen_test::MockReceiver>(buffers);
  }

  std::shared_ptr<musen_test::MockSender> sender;
  std::shared_ptr<musen_test::MockReceiver> receiver;

  std::string send_data = "Hello World!";
};

TEST_F(SendAndReceiveStringTest, ReceiveAll) {
  sender->send_string(send_data);

  auto receive_data = receiver->receive_string(16);
  ASSERT_STREQ(receive_data.c_str(), send_data.c_str()) << "Unequal received and sent data";
}

TEST_F(SendAndReceiveStringTest, ReceivePartial) {
  sender->send_string(send_data);

  auto receive_data = receiver->receive_string(5);
  ASSERT_STREQ(receive_data.c_str(), send_data.substr(0, 5).c_str()) <<
    "Unequal received and sent data";
}

TEST_F(SendAndReceiveStringTest, ReceiveNothing) {
  auto receive_data = receiver->receive_string(16);
  ASSERT_EQ(receive_data.size(), 0u) << "Must received nothing";
}

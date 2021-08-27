// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <memory>

#include "gtest/gtest.h"
#include "./mocks/mock_receiver.hpp"
#include "./mocks/mock_sender.hpp"

class SendAndReceiveRawTest : public ::testing::Test {
 protected:
  void SetUp() override {
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

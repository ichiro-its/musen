// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "./mocks/mock_receiver.hpp"
#include "./mocks/mock_sender.hpp"

class SendAndReceiveStringsTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto buffers = std::make_shared<musen_test::Buffers>();

    sender = std::make_shared<musen_test::MockSender>(buffers);
    receiver = std::make_shared<musen_test::MockReceiver>(buffers);
  }

  std::shared_ptr<musen_test::MockSender> sender;
  std::shared_ptr<musen_test::MockReceiver> receiver;

  std::vector<std::string> send_data = {
    "Hello World!",
    "Hello Earth!",
    "Hello Mars!"
  };
};

TEST_F(SendAndReceiveStringsTest, ReceiveAll) {
  sender->send_strings(send_data);
  auto receive_data = receiver->receive_strings(64);

  ASSERT_EQ(receive_data.size(), send_data.size()) << "Unequal received and sent data size";

  for (size_t i = 0; i < receive_data.size(); ++i) {
    ASSERT_STREQ(receive_data[i].c_str(), send_data[i].c_str()) <<
      "Received and sent data differs at index " << i;
  }
}

TEST_F(SendAndReceiveStringsTest, ReceiveCustomDelimiter) {
  sender->send_strings(send_data, "-+-");
  auto receive_data = receiver->receive_strings(64, "-+-");

  ASSERT_EQ(receive_data.size(), send_data.size()) << "Unequal received and sent data size";

  for (size_t i = 0; i < receive_data.size(); ++i) {
    ASSERT_STREQ(receive_data[i].c_str(), send_data[i].c_str()) <<
      "Received and sent data differs at index " << i;
  }
}

TEST_F(SendAndReceiveStringsTest, ReceiveNothing) {
  auto receive_data = receiver->receive_string(64);
  ASSERT_EQ(receive_data.size(), 0u) << "Must received nothing";
}

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
#include <string>
#include <vector>

class SendAndReceiveStringsTest : public ::testing::Test
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

  std::vector<std::string> send_data = {
    "Hello World!",
    "Hello Earth!",
    "Hello Mars!"
  };
};

TEST_F(SendAndReceiveStringsTest, ReceiveAll) {
  sender->send_strings(send_data);
  auto receive_data = receiver->receive_strings(64);

  ASSERT_EQ(receive_data.size(), send_data.size()) << "Unequal sent and received data size";

  for (size_t i = 0; i < receive_data.size(); ++i) {
    ASSERT_STREQ(receive_data[i].c_str(), send_data[i].c_str()) <<
      "Sent and received data differs at index " << i;
  }
}

TEST_F(SendAndReceiveStringsTest, ReceiveCustomDelimiter) {
  sender->send_strings(send_data, "-+-");
  auto receive_data = receiver->receive_strings(64, "-+-");

  ASSERT_EQ(receive_data.size(), send_data.size()) << "Unequal sent and received data size";

  for (size_t i = 0; i < receive_data.size(); ++i) {
    ASSERT_STREQ(receive_data[i].c_str(), send_data[i].c_str()) <<
      "Sent and received data differs at index " << i;
  }
}

TEST_F(SendAndReceiveStringsTest, ReceiveNothing) {
  auto receive_data = receiver->receive_string(64);
  ASSERT_EQ(receive_data.size(), 0u) << "Must received nothing";
}

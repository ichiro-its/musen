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

#include <memory>
#include <string>
#include <vector>

class BroadcastAndListenTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    broadcaster = std::make_shared<musen::Broadcaster>(5000);
    listeners = {
      std::make_shared<musen::Listener>(5000),
      std::make_shared<musen::Listener>(5000),
      std::make_shared<musen::Listener>(5000)
    };

    // Trying to connect both broadcaster and listener
    ASSERT_TRUE(broadcaster->connect()) << "Unable to connect the broadcaster";
    for (size_t i = 0; i < listeners.size(); ++i) {
      ASSERT_TRUE(listeners[i]->connect()) << "Unable to connect listener " << i;
    }
  }

  void TearDown() override
  {
    // Trying to disconnect both broadcaster and listener
    ASSERT_TRUE(broadcaster->disconnect()) << "Unable to disconnect the broadcaster";
    for (size_t i = 0; i < listeners.size(); ++i) {
      ASSERT_TRUE(listeners[i]->disconnect()) << "Unable to disconnect listener " << i;
    }
  }

  std::shared_ptr<musen::Broadcaster> broadcaster;
  std::vector<std::shared_ptr<musen::Listener>> listeners;
};

TEST_F(BroadcastAndListenTest, SingleListener) {
  std::string broadcast_message = "Hello World!";
  auto active_listeners = listeners;

  // Do up to 3 times until the listener has received the message
  int iteration = 0;
  while (iteration++ < 3) {
    // Sending message
    broadcaster->send_string(broadcast_message);

    // Wait 10ms so the listener could receive the messages
    usleep(10 * 1000);

    for (size_t i = 0; i < active_listeners.size(); ++i) {
      auto listen_message = active_listeners[i]->receive_string(32);
      if (listen_message.size() > 0) {
        ASSERT_STREQ(listen_message.c_str(), broadcast_message.c_str()) <<
          "Unequal listened and broadcasted message";

        active_listeners.erase(active_listeners.begin() + i);
        --i;
      }
    }

    if (active_listeners.size() <= 0) {
      return SUCCEED();
    }
  }

  FAIL() << "Several listener did not receive any data";
}

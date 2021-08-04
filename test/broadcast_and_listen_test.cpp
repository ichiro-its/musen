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

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include <thread>

#include "gtest/gtest.h"
#include "musen/musen.hpp"

using std::chrono_literals::operator""ms;

class BroadcastAndListenTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::srand(std::time(0));

    port = 5000 + std::rand() % 1000;

    try {
      broadcaster = std::make_shared<musen::Broadcaster>(port);
    } catch (const std::system_error & err) {
      FAIL() << "Unable to start the broadcaster on port " << port << "! " << err.what();
    }

    for (size_t i = 0; i < 3; ++i) {
      try {
        auto new_listener = std::make_shared<musen::Listener>(port);
        listeners.push_back(new_listener);
      } catch (const std::system_error & err) {
        FAIL() << "Unable to start listener " << i << " on port " << port << "! " << err.what();
      }
    }
  }

  int port;

  std::shared_ptr<musen::Broadcaster> broadcaster;
  std::vector<std::shared_ptr<musen::Listener>> listeners;
};

TEST_F(BroadcastAndListenTest, MultipleListener) {
  std::string broadcast_message = "Hello World!";
  auto active_listeners = listeners;

  // Do up to 3 times until all listeners have received the message
  for (int it = 0; it < 3; ++it) {
    // Sending message
    broadcaster->send_string(broadcast_message);

    // Wait a bit so listeners could receive the messages
    std::this_thread::sleep_for(10ms);

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

  FAIL() << "Several listeners did not receive any data";
}

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

#include <string>

TEST(BroadcastAndListenTest, SingleListener) {
  musen::Broadcaster broadcaster(5000);
  musen::Listener listener(5000);

  // Trying to connect both broadcaster and listener
  ASSERT_TRUE(broadcaster.connect()) << "Unable to connect the broadcaster";
  ASSERT_TRUE(listener.connect()) << "Unable to connect the listener";

  std::string broadcast_message = "Hello World!";

  // Do up to 3 times until the listener has received the message
  int iteration = 0;
  while (iteration++ < 3) {
    // Sending message
    broadcaster.send_string(broadcast_message);

    // Wait 10ms so the listener could receive the messages
    usleep(10 * 1000);

    auto listen_message = listener.receive_string(32);
    if (listen_message.size() > 0) {
      ASSERT_STREQ(listen_message.c_str(), broadcast_message.c_str()) <<
        "Unequal broadcast and listen message size";

      return SUCCEED();
    }
  }

  FAIL() << "listener did not receive any data";
}

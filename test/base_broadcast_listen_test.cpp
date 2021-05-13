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

TEST(BaseBroadcastListenTest, BroadcastListen) {
  musen::BaseBroadcaster broadcaster(5000);
  musen::BaseListener listener(5000);

  // Trying to connect both broadcaster and listener
  ASSERT_TRUE(broadcaster.connect());
  ASSERT_TRUE(listener.connect());

  char broadcast_data[6] = {'A', 'B', 'C', 'D', 'E', 'F'};
  char listen_data[6];

  const auto & broadcast_and_listen =
    [&]() {
      // Do up to 3 times until the listener received a message
      int iteration = 0;
      while (iteration++ < 3) {
        // Sending data (should sent 4 bytes event if has 6 bytes of data)
        int sent = broadcaster.send(broadcast_data, 4);
        ASSERT_EQ(sent, 4);

        // Wait a bit so listener could receive the data
        usleep(10 * 1000);

        // Receiving data (should received 4 bytes even if requested 6 bytes)
        int received = listener.receive(listen_data, 6);
        if (received > 0) {
          // The received size should equal to sent size
          ASSERT_EQ(received, sent);

          // The received content should equal to sent content
          for (int i = 0; i < received; ++i) {
            ASSERT_EQ(listen_data[i], broadcast_data[i]);
          }

          return SUCCEED();
        }
      }

      FAIL() << "listener did not receive any data";
    };

  // Run with default configuration (broadcast send) for 3 times
  for (int i = 0; i < 3; ++i) {
    broadcast_and_listen();
  }

  // Run with specified target host and disable broadcast send for 3 times
  broadcaster.enable_broadcast(false);
  broadcaster.add_target_host("localhost");
  for (int i = 0; i < 3; ++i) {
    broadcast_and_listen();
  }
}

TEST(BaseBroadcastListenTest, BroadcastNothing) {
  musen::BaseBroadcaster broadcaster(5000);

  // Disable broadcast so it won't sent to anyone
  broadcaster.enable_broadcast(false);

  // Trying to connect the broadcaster
  ASSERT_TRUE(broadcaster.connect());

  char broadcast_data[6] = {'A', 'B', 'C', 'D', 'E', 'F'};

  // Do for 3 times
  for (int i = 0; i < 3; ++i) {
    // Should sent nothing
    int sent = broadcaster.send(broadcast_data, 4);
    ASSERT_LE(sent, 0);

    // Wait a bit
    usleep(10 * 1000);
  }
}

TEST(BaseBroadcastListenTest, ListenNothing) {
  musen::BaseListener listener(5000);

  // Trying to connect the listener
  ASSERT_TRUE(listener.connect());

  char listen_data[6];

  // Do for 3 times
  for (int i = 0; i < 3; ++i) {
    // Should received nothing
    int received = listener.receive(listen_data, 6);
    ASSERT_LE(received, 0);

    // Wait a bit
    usleep(10 * 1000);
  }
}

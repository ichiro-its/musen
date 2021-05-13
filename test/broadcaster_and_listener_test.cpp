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

struct Foo
{
  int a;
  float b;
};

TEST(BroadcasterAndListenerTest, SendAndReceive) {
  musen::Broadcaster<Foo> broadcaster(5000);
  musen::Listener<Foo> listener(5000);

  // Trying to connect both broadcaster and listener
  ASSERT_TRUE(broadcaster.connect());
  ASSERT_TRUE(listener.connect());

  Foo broadcast_data;
  broadcast_data.a = 32;
  broadcast_data.b = 0.125;

  // Do up to 3 times until the listener has received the data
  int iteration = 0;
  while (iteration++ < 3) {
    // Sending the data
    broadcaster.send(broadcast_data);

    // Wait a bit so the listener could receive the data
    usleep(10 * 1000);

    // Receiving data
    auto listen_data = listener.receive();
    if (listen_data != nullptr) {
      // Compare the received and the sent data
      ASSERT_EQ(listen_data->a, broadcast_data.a);
      ASSERT_DOUBLE_EQ(listen_data->b, broadcast_data.b);

      return SUCCEED();
    }
  }

  FAIL() << "listener did not receive any data";
}

TEST(BroadcasterAndListenerTest, ReceiveNothing) {
  musen::Listener<Foo> listener(5000);

  // Trying to connect the listener
  ASSERT_TRUE(listener.connect());

  // Do for 3 times
  for (int i = 0; i < 3; ++i) {
    // Must received nothing
    auto listen_data = listener.receive();
    ASSERT_EQ(listen_data, nullptr);

    // Wait a bit
    usleep(10 * 1000);
  }
}

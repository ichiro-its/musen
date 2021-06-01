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

#include <thread>

struct Foo
{
  int a;
  float b;
};

TEST(ClientAndServerTest, SendAndReceive) {
  musen::Server<Foo> * server = new musen::Server<Foo>(6000);
  musen::Client<Foo> * client = new musen::Client<Foo>("127.0.0.1", 6000);

  // Trying to connect both server and client
  std::thread thread_object([&]() {
      ASSERT_TRUE(server->connect());
    });

  // Wait a bit so the client could connect to server
  usleep(10 * 1000);
  ASSERT_TRUE(client->connect());
  thread_object.join();

  Foo sent_data;
  sent_data.a = 32;
  sent_data.b = 0.125;

  const auto & send_and_receive =
    [&]() {
      // Do up to 3 times until the client has received the data
      int iteration = 0;
      while (iteration++ < 3) {
        // Sending data (must send 4 bytes even if it has 6 bytes of data)
        client->send(sent_data);

        // Wait a bit so the server could receive the data
        usleep(10 * 1000);
        auto received = server->receive();

        ASSERT_EQ(received->a, sent_data.a);
        ASSERT_DOUBLE_EQ(received->b, sent_data.b);

        // Server send the received data
        server->send(*received);

        // Wait a bit so the client could receive the data
        usleep(10 * 1000);

        // Client receiving data
        auto received_data = client->receive();

        if (received_data.has_value()) {
          // Compare the received and the sent data
          ASSERT_EQ(received_data->a, sent_data.a);
          ASSERT_DOUBLE_EQ(received_data->b, sent_data.b);

          return SUCCEED();
        }
      }

      FAIL() << "server did not receive any data";
    };

  // Run with default configuration (send) for 3 times
  for (int i = 0; i < 3; ++i) {
    send_and_receive();
  }
}

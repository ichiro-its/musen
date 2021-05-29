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

TEST(BaseClientAndServerTest, SendAndReceive) {
  musen::BaseServer * server = new musen::BaseServer(5000);
  musen::BaseClient * client = new musen::BaseClient("127.0.0.1", 5000);

  // Trying to connect both server and client
  std::thread thread_object([&]() {
      ASSERT_TRUE(server->connect());
    });

  // Wait a bit so the client could connect to server
  usleep(10 * 1000);
  ASSERT_TRUE(client->connect());
  thread_object.join();

  char sent_data[6] = {'A', 'B', 'C', 'D', 'E', 'F'};
  char received_data[6];

  const auto & send_and_receive =
    [&]() {
      // Do up to 3 times until the client has received the data
      int iteration = 0;
      while (iteration++ < 3) {
        // Sending data (must send 4 bytes even if it has 6 bytes of data)
        int sent = client->send(sent_data, 4);
        ASSERT_EQ(sent, 4);

        // Wait a bit so the server could receive the data
        usleep(10 * 1000);

        // Receiving data (must receive 4 bytes even if it requested 6 bytes)
        server->receive(received_data, 6);
        server->send(received_data, 4);

        // Wait a bit so the client could receive the data
        usleep(10 * 1000);

        // Client receiving data
        int received = client->receive(received_data, 6);

        if (received > 0) {
          // The received size must equal to sent size
          ASSERT_EQ(received, sent);

          // The received content must be equal to sent content
          for (int i = 0; i < received; ++i) {
            ASSERT_EQ(received_data[i], sent_data[i]);
          }

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

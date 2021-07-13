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

#include <list>
#include <memory>
#include <string>
#include <thread>
#include <vector>

class ServerAndClientTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    server = std::make_shared<musen::Server>(5000);
    clients = {
      std::make_shared<musen::Client>("localhost", 5000),
      std::make_shared<musen::Client>("localhost", 5000),
      std::make_shared<musen::Client>("localhost", 5000)
    };

    // Process the server on a separate thread
    server_thread = std::make_shared<std::thread>(
      [this]() {
        // Trying to connect the server
        ASSERT_TRUE(server->connect()) << "Unable to connect the server";

        std::list<std::shared_ptr<musen::Session>> sessions;

        // Process each client while the server is connected
        while (server->is_connected()) {
          // Accept a new client
          auto new_session = server->accept();
          if (new_session != nullptr) {
            sessions.push_back(new_session);
          }

          // Echo the received message from each client
          for (auto session : sessions) {
            auto receive_message = session->receive_string(32);
            if (receive_message.size() > 0) {
              session->send_string(receive_message);
            }
          }
        }
      });

    // Wait 10ms so each client could connect to the server
    usleep(10 * 1000);

    // Trying to connect each client
    for (size_t i = 0; i < clients.size(); ++i) {
      ASSERT_TRUE(clients[i]->connect()) << "Unable to connect client " << i;
    }

  }

  void TearDown() override
  {
    // Trying to disconnect both server and clients
    ASSERT_TRUE(server->disconnect()) << "Unable to disconnect the server";
    for (size_t i = 0; i < clients.size(); ++i) {
      ASSERT_TRUE(clients[i]->disconnect()) << "Unable to disconnect client " << i;
    }

    if (server_thread) {
      server_thread->join();
    }
  }

  std::shared_ptr<musen::Server> server;
  std::vector<std::shared_ptr<musen::Client>> clients;

  std::shared_ptr<std::thread> server_thread;
};

TEST_F(ServerAndClientTest, SingleClient) {
  std::string send_message = "Hello World!";
  auto active_clients = clients;

  // Do up to 3 times until all clients have received the message back
  int iteration = 0;
  while (iteration++ < 3) {
    for (size_t i = 0; i < active_clients.size(); ++i) {
      auto client = active_clients[i];
      std::string send_message = "Hello World! " + std::to_string(i);

      // Sending message
      client->send_string(send_message);

      // Wait 10ms so the server could receive the messages
      usleep(10 * 1000);

      auto receive_message = client->receive_string(32);
      if (receive_message.size() > 0) {
        ASSERT_STREQ(receive_message.c_str(), send_message.c_str()) <<
          "Unequal received and sent message";

        active_clients.erase(active_clients.begin() + i);
        --i;
      }
    }

    if (active_clients.size() <= 0) {
      return SUCCEED();
    }
  }

  FAIL() << "Several clients did not receive any data";
}

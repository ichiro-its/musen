// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <cstdlib>
#include <list>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "gtest/gtest.h"
#include "musen/musen.hpp"

using std::chrono_literals::operator""ms;

class ServerAndClientTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::srand(std::time(0));

    server_address.ip = "127.0.0.1";
    server_address.port = 5000 + std::rand() % 1000;

    // Process the server on a separate thread
    server_thread = std::make_shared<std::thread>(
      [this]() {
        try {
          server = std::make_shared<musen::Server>(server_address.port);
        } catch (const std::system_error & err) {
          FAIL() << "Unable to start the server on port " << server_address.port << "! " << err.what();
        }

        std::weak_ptr<musen::Server> weak_server = server;

        std::list<std::shared_ptr<musen::Session>> sessions;

        // Process each client while the server is alive
        while (auto server = weak_server.lock()) {
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

    // Wait a bit so each client could connect to the server
    std::this_thread::sleep_for(10ms);

    for (size_t i = 0; i < 3; ++i) {
      try {
        auto new_client = std::make_shared<musen::Client>(server_address);
        clients.push_back(new_client);
      } catch (const std::system_error & err) {
        FAIL() << "Unable to connect client " << i <<
          " to the server on ip " << server_address.ip <<
          " and port " << server_address.port << "! " << err.what();
      }

      // Wait a bit so the next client could connect to the server
      std::this_thread::sleep_for(10ms);
    }
  }

  void TearDown() override {
    server = nullptr;
    server_thread->join();
  }

  musen::Address server_address;

  std::shared_ptr<musen::Server> server;
  std::vector<std::shared_ptr<musen::Client>> clients;

  std::shared_ptr<std::thread> server_thread;
};

TEST_F(ServerAndClientTest, MultipleClient) {
  auto active_clients = clients;

  // Do up to 3 times until all clients have received the message back
  for (int it = 0; it < 3; ++it) {
    for (size_t i = 0; i < active_clients.size(); ++i) {
      auto client = active_clients[i];
      std::string send_message = "Hello World! " + std::to_string(i);

      // Sending message
      client->send_string(send_message);

      // Wait a bit so the client could receive the messages
      std::this_thread::sleep_for(10ms);

      auto receive_message = client->receive_string(32);
      if (receive_message.size() > 0) {
        EXPECT_STREQ(receive_message.c_str(), send_message.c_str()) <<
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

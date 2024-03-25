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

// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "musen/musen.hpp"

using std::chrono_literals::operator""s;

int main() {
  int port = 5000;

  try {
    musen::Broadcaster broadcaster(port);

    std::vector<std::string> fruits = {"apple", "banana", "orange", "pear"};

    while (true) {
      std::vector<std::string> message;

      size_t length = 3 + (std::rand() % 3);
      for (size_t i = 0; i < length; ++i) {
        message.push_back(fruits[std::rand() % fruits.size()]);
      }

      broadcaster.send_strings(message, "-");

      std::cout << "Sent: ";
      for (auto & value : message) {
        std::cout << value << " ";
      }
      std::cout << std::endl;

      std::this_thread::sleep_for(1s);
    }
  } catch (const std::system_error & err) {
    std::cerr << "Failed to start the broadcaster on port " << port << "! " << err.what() << std::endl;
    return err.code().value();
  }

  return 0;
}

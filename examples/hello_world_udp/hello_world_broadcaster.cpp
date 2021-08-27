// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <iostream>
#include <string>
#include <thread>

#include "musen/musen.hpp"

using std::chrono_literals::operator""s;

int main() {
  int port = 5000;

  try {
    musen::Broadcaster broadcaster(port);

    int counter = 0;
    while (true) {
      std::string message = "Hello world! " + std::to_string(counter++);

      broadcaster.send_string(message);

      std::cout << "Sent: " << message << std::endl;

      std::this_thread::sleep_for(1s);
    }
  } catch (const std::system_error & err) {
    std::cerr << "Failed to start the broadcaster on port " << port << "! " << err.what() << std::endl;
    return err.code().value();
  }

  return 0;
}

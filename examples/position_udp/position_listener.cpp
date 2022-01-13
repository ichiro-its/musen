// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <iostream>
#include <thread>

#include "musen/musen.hpp"

using std::chrono_literals::operator""ms;

struct Position {
  int x;
  int y;
  int z;
};

int main() {
  int port = 5000;

  try {
    musen::Listener listener(5000);

    while (true) {
      auto position = listener.receive<Position>();

      if (position.has_value()) {
        std::cout << "Received: " <<
          position->x << ", " <<
          position->y << ", " <<
          position->z << std::endl;
      }

      std::this_thread::sleep_for(100ms);
    }
  } catch (const std::system_error & err) {
    std::cerr << "Failed to start the listener on port " << port << "! " << err.what() << std::endl;
    return err.code().value();
  }

  return 0;
}

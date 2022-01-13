// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <cstdlib>
#include <iostream>
#include <thread>

#include "musen/musen.hpp"

using std::chrono_literals::operator""s;

struct Position {
  int x;
  int y;
  int z;
};

int main() {
  musen::Address server_address("127.0.0.1", 5000);

  try {
    musen::Client client(server_address);

    while (true) {
      auto received_position = client.receive<Position>();

      if (received_position.has_value()) {
        std::cout << "Received: " <<
          received_position->x << ", " <<
          received_position->y << ", " <<
          received_position->z << std::endl;
      }

      Position position;

      position.x = std::rand() % 100;
      position.y = std::rand() % 100;
      position.z = std::rand() % 100;

      client.send(position);

      std::cout << "Sent: " <<
        position.x << ", " <<
        position.y << ", " <<
        position.z << std::endl;

      std::this_thread::sleep_for(1s);
    }
  } catch (const std::system_error & err) {
    std::cerr << "Failed to connect the client to the server on ip " << server_address.ip <<
      " and port " << server_address.port << "! " << err.what() << std::endl;

    return err.code().value();
  }

  return 0;
}

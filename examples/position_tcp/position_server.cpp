// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include <thread>

#include "musen/musen.hpp"

using std::chrono_literals::operator""s;

struct Position {
  int x;
  int y;
  int z;
};

int main() {
  int port = 5000;

  try {
    musen::Server server(5000);

    std::list<std::shared_ptr<musen::Session>> sessions;

    while (true) {
      auto new_session = server.accept();
      if (new_session != nullptr) {
        sessions.push_back(new_session);
      }

      for (auto session : sessions) {
        Position position;

        position.x = std::rand() % 100;
        position.y = std::rand() % 100;
        position.z = std::rand() % 100;

        session->send(position);

        std::cout << "Sent: " <<
          position.x << ", " <<
          position.y << ", " <<
          position.z << std::endl;

        auto received_position = session->receive<Position>();

        if (received_position.has_value()) {
          std::cout << "Received: " <<
            received_position->x << ", " <<
            received_position->y << ", " <<
            received_position->z << std::endl;
        }
      }

      std::this_thread::sleep_for(1s);
    }
  } catch (const std::system_error & err) {
    std::cerr << "Failed to start the server on port " << port << "! " << err.what() << std::endl;
    return err.code().value();
  }

  return 0;
}

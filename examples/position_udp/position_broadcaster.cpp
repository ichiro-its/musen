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
  int port = 5000;

  try {
    musen::Broadcaster broadcaster(port);

    while (true) {
      Position position;

      position.x = std::rand() % 100;
      position.y = std::rand() % 100;
      position.z = std::rand() % 100;

      broadcaster.send(position);

      std::cout << "Sent: " <<
        position.x << ", " <<
        position.y << ", " <<
        position.z << std::endl;

      std::this_thread::sleep_for(1s);
    }
  } catch (const std::system_error & err) {
    std::cerr << "Failed to start the broadcaster on port " << port << "! " << err.what() << std::endl;
    return err.code().value();
  }

  return 0;
}

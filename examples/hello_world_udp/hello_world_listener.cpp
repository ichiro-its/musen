#include <iostream>
#include <string>
#include <thread>

#include "musen/musen.hpp"

using std::chrono_literals::operator""ms;

int main() {
  int port = 5000;

  try {
    musen::Listener listener(port);

    while (true) {
      auto message = listener.receive_string(64);

      if (message.size() > 0) {
        std::cout << "Received: " << message << std::endl;
      }

      std::this_thread::sleep_for(100ms);
    }
  } catch (const std::system_error & err) {
    std::cerr << "Failed to start the listener on port " << port << "! " << err.what() << std::endl;
    return err.code().value();
  }

  return 0;
}

#include <memory>

#include "musen/tcp/session.hpp"

namespace musen {

Session::Session(std::shared_ptr<Socket> socket) {
  this->socket = socket;
}

Session::~Session() {
  socket = nullptr;
}

size_t Session::send_raw(const char * data, size_t length) {
  return socket->send(data, length);
}

size_t Session::receive_raw(char * data, size_t length) {
  try {
    return socket->receive(data, length);
  } catch (const std::system_error & err) {
    switch (err.code().value()) {
      case ENOTCONN:
        return 0;
    }

    throw err;
  }
}

std::shared_ptr<Socket> Session::get_socket() const {
  return socket;
}

}  // namespace musen

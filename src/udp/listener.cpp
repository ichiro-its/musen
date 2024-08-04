#include <memory>

#include "musen/udp/listener.hpp"

namespace musen {

Listener::Listener(int port, std::shared_ptr<Socket> socket) {
  this->socket = socket;
  this->port = port;

  // Enable reuse port
  socket->set_option(SO_REUSEPORT, 1);

  // Bind the socket with the listen address
  socket->bind(make_any_address(port));
}

Listener::~Listener() {
  socket = nullptr;
}

size_t Listener::receive_raw(char * data, size_t length) {
  return socket->receive(data, length);
}

std::shared_ptr<Socket> Listener::get_socket() const {
  return socket;
}

int Listener::get_port() const {
  return port;
}

}  // namespace musen

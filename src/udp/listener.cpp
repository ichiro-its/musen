// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <memory>

#include "musen/udp/listener.hpp"

namespace musen {

Listener::Listener(const int & port, std::shared_ptr<Socket> socket)
: socket(socket), port(port) {
  // Enable reuse port
  socket->set_option(SO_REUSEPORT, 1);

  // Bind the socket with the listen address
  socket->bind(make_any_address(port));
}

Listener::~Listener() {
  socket = nullptr;
}

size_t Listener::receive_raw(char * data, const size_t & length) {
  return socket->receive(data, length);
}

std::shared_ptr<Socket> Listener::get_socket() const {
  return socket;
}

const int & Listener::get_port() const {
  return port;
}

}  // namespace musen

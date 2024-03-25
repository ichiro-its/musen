#include <limits>
#include <list>
#include <memory>
#include <optional>
#include <string>

#include "musen/udp/broadcaster.hpp"

namespace musen {

Broadcaster::Broadcaster(const int & port, std::shared_ptr<Socket> socket) {
  this->socket = socket;
  this->port = port;

  enable_broadcast(true);
}

Broadcaster::~Broadcaster() {
  socket = nullptr;
}

size_t Broadcaster::send_raw(const char * data, const size_t & length) {
  // Obtain all addresses
  auto addresses = broadcast_addresses;
  for (const auto & ip : target_ips) {
    addresses.push_back(Address(ip, port));
  }

  // Sent to each recipent socket addresses
  std::optional<size_t> lowest_sent;
  for (const auto & address : addresses) {
    auto sent = socket->send_to(data, length, address);

    if (sent < lowest_sent.value_or(std::numeric_limits<size_t>::max())) {
      lowest_sent = sent;
    }
  }

  // Return the lowest sent size from all addresses
  return lowest_sent.value_or(0);
}

void Broadcaster::enable_broadcast(const bool & enable) {
  socket->set_option<int>(SO_BROADCAST, enable);

  broadcast_addresses.clear();
  if (socket->get_option<bool>(SO_BROADCAST)) {
    for (const auto & ip : obtain_broadcast_ips()) {
      broadcast_addresses.push_back(Address(ip, port));
    }
  }
}

std::shared_ptr<Socket> Broadcaster::get_socket() const {
  return socket;
}

const int & Broadcaster::get_port() const {
  return port;
}

}  // namespace musen

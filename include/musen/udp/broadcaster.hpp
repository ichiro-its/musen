#pragma once

#include <list>
#include <memory>
#include <string>

#include "musen/address.hpp"
#include "musen/sender.hpp"
#include "musen/socket.hpp"

namespace musen {

class Broadcaster : public Sender {
 public:
  explicit Broadcaster(int port, std::shared_ptr<Socket> socket = make_udp_socket());
  ~Broadcaster();

  size_t send_raw(const char * data, size_t length) override;

  void enable_broadcast(bool enable);

  std::shared_ptr<Socket> get_socket() const;
  int get_port() const;

  std::list<std::string> target_ips;

 protected:
  std::shared_ptr<Socket> socket;
  int port;

  std::list<Address> broadcast_addresses;
};

}  // namespace musen

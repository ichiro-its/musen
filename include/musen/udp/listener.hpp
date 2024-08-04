#pragma once

#include <memory>

#include "musen/receiver.hpp"
#include "musen/socket.hpp"

namespace musen {

class Listener : public Receiver {
 public:
  explicit Listener(int port, std::shared_ptr<Socket> socket = make_udp_socket());
  ~Listener();

  size_t receive_raw(char * data, size_t length) override;

  std::shared_ptr<Socket> get_socket() const;
  int get_port() const;

 protected:
  std::shared_ptr<Socket> socket;
  int port;
};

}  // namespace musen

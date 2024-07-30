#pragma once

#include <memory>

#include "musen/address.hpp"
#include "musen/receiver.hpp"
#include "musen/sender.hpp"
#include "musen/socket.hpp"

namespace musen {

class Client : public Sender, public Receiver {
 public:
  explicit Client(
    const Address & server_address, std::shared_ptr<Socket> socket = make_tcp_socket());

  ~Client();

  size_t send_raw(const char * data, size_t length) override;
  size_t receive_raw(char * data, size_t length) override;

  std::shared_ptr<Socket> get_socket() const;
  const Address & get_server_address() const;

 protected:
  std::shared_ptr<Socket> socket;
  Address server_address;
};

}  // namespace musen

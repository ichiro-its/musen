#pragma once

#include <memory>

#include "musen/sender.hpp"
#include "musen/receiver.hpp"
#include "musen/socket.hpp"

namespace musen {

class Session : public Sender, public Receiver {
 public:
  explicit Session(std::shared_ptr<Socket> socket);
  ~Session();

  size_t send_raw(const char * data, size_t length) override;
  size_t receive_raw(char * data, size_t length) override;

  std::shared_ptr<Socket> get_socket() const;

 protected:
  std::shared_ptr<Socket> socket;
};

}  // namespace musen

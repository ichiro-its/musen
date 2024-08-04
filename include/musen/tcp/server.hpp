#pragma once

#include <memory>
#include <string>

#include "musen/tcp/session.hpp"
#include "musen/socket.hpp"

namespace musen {

class Server {
 public:
  explicit Server(int port, std::shared_ptr<Socket> socket = make_tcp_socket());
  ~Server();

  std::shared_ptr<Session> accept();

  std::shared_ptr<Socket> get_socket() const;
  int get_port() const;

 protected:
  std::shared_ptr<Socket> socket;
  int port;
};

}  // namespace musen

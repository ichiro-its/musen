// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <memory>
#include <string>

#include "musen/tcp/session.hpp"
#include "musen/socket.hpp"

namespace musen {

class Server {
 public:
  explicit Server(const int & port, std::shared_ptr<Socket> socket = make_tcp_socket());
  ~Server();

  std::shared_ptr<Session> accept();

  std::shared_ptr<Socket> get_socket() const;
  const int & get_port() const;

 protected:
  std::shared_ptr<Socket> socket;
  int port;
};

}  // namespace musen

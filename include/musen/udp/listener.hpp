// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <memory>

#include "musen/receiver.hpp"
#include "musen/socket.hpp"

namespace musen {

class Listener : public Receiver {
 public:
  explicit Listener(const int & port, std::shared_ptr<Socket> socket = make_udp_socket());
  ~Listener();

  size_t receive_raw(char * data, const size_t & length) override;

  std::shared_ptr<Socket> get_socket() const;
  const int & get_port() const;

 protected:
  std::shared_ptr<Socket> socket;
  int port;
};

}  // namespace musen

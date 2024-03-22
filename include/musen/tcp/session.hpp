// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

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

  size_t send_raw(const char * data, const size_t & length) override;
  size_t receive_raw(char * data, const size_t & length) override;

  std::shared_ptr<Socket> get_socket() const;

 protected:
  std::shared_ptr<Socket> socket;
};

}  // namespace musen

// Copyright (c) 2021 ICHIRO ITS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

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
  explicit Broadcaster(const int & port, std::shared_ptr<Socket> socket = make_udp_socket());
  ~Broadcaster();

  size_t send_raw(const char * data, const size_t & length) override;

  void enable_broadcast(const bool & enable);

  std::shared_ptr<Socket> get_socket() const;
  const int & get_port() const;

  std::list<std::string> target_ips;

 protected:
  std::shared_ptr<Socket> socket;
  int port;

  std::list<Address> broadcast_addresses;
};

}  // namespace musen

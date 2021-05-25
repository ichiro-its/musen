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

#ifndef MUSEN__LISTENER__BASE_LISTENER_HPP_
#define MUSEN__LISTENER__BASE_LISTENER_HPP_

#include <memory>

#include "../udp_socket.hpp"

namespace musen
{

class BaseListener : public UdpSocket
{
public:
  explicit BaseListener(
    const int & port, std::shared_ptr<UdpSocket> udp_socket = std::make_shared<UdpSocket>());

  bool connect();
  bool disconnect();

  int receive(void * buffer, const int & length);

  std::shared_ptr<UdpSocket> get_udp_socket() const;

  const int & get_port() const;

protected:
  std::shared_ptr<UdpSocket> udp_socket;

  int port;
};

}  // namespace musen

#endif  // MUSEN__LISTENER__BASE_LISTENER_HPP_

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

#include <musen/udp/broadcaster.hpp>

#include <algorithm>
#include <cstring>
#include <list>
#include <memory>
#include <string>

namespace musen
{

Broadcaster::Broadcaster(const int & port, std::shared_ptr<Socket> socket)
: socket(socket),
  port(port)
{
  enable_broadcast(true);
}

Broadcaster::~Broadcaster()
{
  socket = nullptr;
}

size_t Broadcaster::send_raw(const char * data, const size_t & length)
{
  if (length <= 0) {
    return 0;
  }

  // Obtain all addresses
  auto addresses = broadcast_addresses;
  for (const auto & ip : target_ips) {
    addresses.push_back(Address(ip, port));
  }

  // Sent to each recipent socket addresses
  int lowest_sent = -1;
  for (const auto & address : addresses) {
    auto sa = address.sockaddr_in();
    int sent = sendto(
      socket->get_fd(), data, length, 0, (struct sockaddr *)&sa, sizeof(sa));

    // If lowest_sent is not yet set (-1) or sent is less than lowest_sent
    if (lowest_sent < 0 || sent < lowest_sent) {
      lowest_sent = sent;
    }
  }

  // Return the lowest sent size from all addresses
  return std::max(lowest_sent, 0);
}

void Broadcaster::enable_broadcast(const bool & enable)
{
  broadcast = enable;

  broadcast_addresses.clear();
  if (broadcast) {
    for (const auto & ip : obtain_broadcast_ips()) {
      broadcast_addresses.push_back(Address(ip, port));
    }
  }
}

std::shared_ptr<Socket> Broadcaster::get_socket() const
{
  return socket;
}

const int & Broadcaster::get_port() const
{
  return port;
}

}  // namespace musen

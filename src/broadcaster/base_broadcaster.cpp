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

#include <musen/broadcaster/base_broadcaster.hpp>

#include <ifaddrs.h>

#include <algorithm>
#include <cstring>
#include <list>
#include <memory>
#include <string>

namespace musen
{

BaseBroadcaster::BaseBroadcaster(const int & port, std::shared_ptr<UdpSocket> udp_socket)
: udp_socket(udp_socket),
  broadcast(true),
  port(port)
{
}

bool BaseBroadcaster::connect()
{
  auto result = udp_socket->connect();

  if (result) {
    // Reobtain recipent socket addresses after connected
    recipent_sas = obtain_recipent_sas();
  }

  return result;
}

bool BaseBroadcaster::disconnect()
{
  return udp_socket->disconnect();
}

int BaseBroadcaster::send(const char * data, const int & length)
{
  if (!udp_socket->is_connected() || length <= 0) {
    return 0;
  }

  // Sent to each recipent socket addresses
  int lowest_sent = -1;
  for (const auto & sa : recipent_sas) {
    int sent = sendto(
      udp_socket->get_sockfd(), data, length, 0, (struct sockaddr *)&sa, sizeof(sa));

    // If lowest_sent is not yet set (-1) or sent is less than lowest_sent
    if (lowest_sent < 0 || sent < lowest_sent) {
      lowest_sent = sent;
    }
  }

  // Return the lowest sent size from all addresses
  return std::max(lowest_sent, 0);
}

void BaseBroadcaster::enable_broadcast(const bool & enable)
{
  broadcast = enable;

  // Reobtain recipent socket addresses after broadcast changed
  recipent_sas = obtain_recipent_sas();
}

void BaseBroadcaster::add_target_host(const std::string & target_host)
{
  target_hosts.push_back(target_host);

  // Reobtain recipent socket addresses after target host changed
  recipent_sas = obtain_recipent_sas();
}

std::shared_ptr<UdpSocket> BaseBroadcaster::get_udp_socket() const
{
  return udp_socket;
}

const int & BaseBroadcaster::get_port() const
{
  return port;
}

std::list<struct sockaddr_in> BaseBroadcaster::obtain_recipent_sas() const
{
  std::list<struct sockaddr_in> sas;

  sas.splice(sas.end(), obtain_recipent_sas_from_broadcast_ifas());
  sas.splice(sas.end(), obtain_recipent_sas_from_target_hosts());

  return sas;
}

std::list<struct sockaddr_in> BaseBroadcaster::obtain_recipent_sas_from_broadcast_ifas() const
{
  std::list<struct sockaddr_in> sas;

  if (!broadcast) {
    return sas;
  }

  // Obtain all interfaces
  struct ifaddrs * ifas;
  if (getifaddrs(&ifas) != 0) {
    return sas;
  }

  // Repeat for all available interfaces
  for (auto ifa = ifas; ifa != nullptr; ifa = ifa->ifa_next) {
    // Skip if null or if the address family is different
    if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family != AF_INET) {
      continue;
    }

    // Get the broadcast address
    const auto & broadaddr = (struct sockaddr_in *)ifa->ifa_broadaddr;

    // Configure the recipent socket address
    struct sockaddr_in sa;
    {
      memset(reinterpret_cast<void *>(&sa), 0, sizeof(sa));

      sa.sin_family = AF_INET;
      sa.sin_addr = broadaddr->sin_addr;
      sa.sin_port = htons(port);
    }

    sas.push_back(sa);
  }

  freeifaddrs(ifas);

  return sas;
}

std::list<struct sockaddr_in> BaseBroadcaster::obtain_recipent_sas_from_target_hosts() const
{
  std::list<struct sockaddr_in> sas;

  // Append recipent socket addresses with target hosts
  for (const auto & target_host : target_hosts) {
    // Configure the recipent socket address
    struct sockaddr_in sa;
    {
      memset(reinterpret_cast<void *>(&sa), 0, sizeof(sa));

      sa.sin_family = AF_INET;
      sa.sin_port = htons(port);

      inet_aton(target_host.c_str(), &sa.sin_addr);
    }

    sas.push_back(sa);
  }

  return sas;
}

}  // namespace musen

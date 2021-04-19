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

#include <housou/broadcaster/base_broadcaster.hpp>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <string.h>

#include <string>
#include <vector>

namespace housou
{

BaseBroadcaster::BaseBroadcaster(int port)
: UdpSocket(),
  port(port)
{
}

int BaseBroadcaster::send(const void * data, int length)
{
  if (!is_connected() || length <= 0) {
    return 0;
  }

  std::vector<struct sockaddr_in> sas;

  if (broadcast) {
    // Obtain all interfaces
    struct ifaddrs * ifas;
    if (getifaddrs(&ifas) != 0) {
      return 0;
    }

    // Repeat for all available interfaces
    for (auto & ifa = ifas; ifa != nullptr; ifa = ifa->ifa_next) {
      // Skip if null or if the address family is different
      if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family != AF_INET) {
        continue;
      }

      auto broadaddr = (struct sockaddr_in *)ifa->ifa_broadaddr;

      // Configure the recipent address
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
  }

  for (auto & target_host : target_hosts) {
    // Configure the recipent address
    struct sockaddr_in sa;
    {
      memset(reinterpret_cast<void *>(&sa), 0, sizeof(sa));

      sa.sin_family = AF_INET;
      sa.sin_port = htons(port);

      inet_aton(target_host.c_str(), &sa.sin_addr);
    }

    sas.push_back(sa);
  }

  int lowest_sent = 0;

  for (auto & sa : sas) {
    // Send data to the recipent address
    int sent = sendto(sockfd, data, length, 0, (struct sockaddr *)&sa, sizeof(sa));

    if (sent < lowest_sent) {
      lowest_sent = sent;
    }
  }

  return lowest_sent;
}

void BaseBroadcaster::enable_broadcast(bool enable)
{
  broadcast = enable;
}

void BaseBroadcaster::add_target_host(std::string target_host)
{
  target_hosts.push_back(target_host);
}

int BaseBroadcaster::get_port()
{
  return port;
}

}  // namespace housou

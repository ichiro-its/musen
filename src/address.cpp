// Copyright (c) 2021-2024 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <ifaddrs.h>

#include <cstring>
#include <list>
#include <string>
#include <system_error>

#include "musen/address.hpp"

namespace musen {

Address make_any_address(const int & port) {
  return Address("0.0.0.0", port);
}

std::list<std::string> obtain_broadcast_ips() {
  std::list<std::string> ips;

  // Obtain all interfaces
  struct ifaddrs * ifas;
  if (getifaddrs(&ifas) != 0) {
    freeifaddrs(ifas);
    throw std::system_error(errno, std::generic_category());
  }

  // Repeat for all available interfaces
  for (auto ifa = ifas; ifa != nullptr; ifa = ifa->ifa_next) {
    // Skip if is null or if the address family is different
    if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family != AF_INET) {
      continue;
    }

    // Get and push the broadcast address
    const auto & broadaddr = (struct sockaddr_in *)ifa->ifa_broadaddr;
    ips.push_back(inet_ntoa(broadaddr->sin_addr));
  }

  freeifaddrs(ifas);

  return ips;
}

Address::Address(const std::string & ip, const int & port) {
  this->ip = ip;
  this->port = port;
}

Address::Address(const struct sockaddr_in & sa)
  : Address(inet_ntoa(sa.sin_addr), ntohs(sa.sin_port)) {
}

Address::Address()
  : Address("", 0) {
}

struct sockaddr_in Address::sockaddr_in() const {
  struct sockaddr_in sa;
  memset(&sa, 0, sizeof(sa));

  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);

  inet_aton(ip.c_str(), &sa.sin_addr);

  return sa;
}

}  // namespace musen

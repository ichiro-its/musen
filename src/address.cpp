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

Address::Address(const std::string & ip, const int & port)
: ip(ip), port(port) {
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

#pragma once

#include <arpa/inet.h>

#include <list>
#include <string>

namespace musen {

struct Address;

Address make_any_address(const int & port);

std::list<std::string> obtain_broadcast_ips();

struct Address {
  Address(const std::string & ip, const int & port);
  explicit Address(const sockaddr_in & sa);
  Address();

  struct sockaddr_in sockaddr_in() const;

  std::string ip;
  int port;
};

}  // namespace musen

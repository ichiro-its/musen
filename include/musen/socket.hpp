#pragma once

#include <memory>

#include "musen/address.hpp"

namespace musen {

class Socket;

std::shared_ptr<Socket> make_tcp_socket();
std::shared_ptr<Socket> make_udp_socket();

std::shared_ptr<Socket> make_blocking_tcp_socket();
std::shared_ptr<Socket> make_blocking_udp_socket();

class Socket {
 public:
  explicit Socket(int fd);
  Socket(int domain, int type, int protocol);

  ~Socket();

  void bind(const Address & address);
  void connect(const Address & address);
  void listen(int max_queue = 8);

  std::shared_ptr<Socket> accept();

  size_t send(const void * data, size_t length);
  size_t send_to(const void * data, size_t length, const Address & address);

  size_t receive(void * data, size_t length);

  void set_status_flags(int flags);
  int get_status_flags() const;

  void set_status_flag(int key, bool enable);
  bool get_status_flag(int key) const;

  template<typename T>
  void set_option(int key, const T & value);

  template<typename T>
  T get_option(int key) const;

  int get_fd() const;

 private:
  int fd;
};

}  // namespace musen

#include "musen/socket.tpp"

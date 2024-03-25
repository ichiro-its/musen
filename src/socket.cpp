#include <fcntl.h>
#include <unistd.h>

#include <memory>

#include "musen/socket.hpp"

namespace musen {

constexpr auto socket_accept = accept;
constexpr auto socket_bind = bind;
constexpr auto socket_connect = connect;
constexpr auto socket_listen = listen;
constexpr auto socket_send = send;

std::shared_ptr<Socket> make_tcp_socket() {
  return std::make_shared<Socket>(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_IP);
}

std::shared_ptr<Socket> make_udp_socket() {
  return std::make_shared<Socket>(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_IP);
}

std::shared_ptr<Socket> make_blocking_tcp_socket() {
  return std::make_shared<Socket>(AF_INET, SOCK_STREAM, IPPROTO_IP);
}

std::shared_ptr<Socket> make_blocking_udp_socket() {
  return std::make_shared<Socket>(AF_INET, SOCK_DGRAM, IPPROTO_IP);
}

Socket::Socket(const int & fd) {
  this->fd = fd;

  if (get_option<int>(SO_ERROR) != 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

Socket::Socket(const int & domain, const int & type, const int & protocol) {
  fd = socket(domain, type, protocol);
  if (fd == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}

Socket::~Socket() {
  close(fd);
}

void Socket::bind(const Address & address) {
  auto sa = address.sockaddr_in();
  if (socket_bind(fd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}

void Socket::connect(const Address & address) {
  auto sa = address.sockaddr_in();
  if (socket_connect(fd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}

void Socket::listen(const int & max_queue) {
  if (socket_listen(fd, max_queue) == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}

std::shared_ptr<Socket> Socket::accept() {
  struct sockaddr sa;
  socklen_t len = sizeof(sa);

  auto retval = socket_accept(fd, &sa, &len);
  if (retval == -1) {
    throw std::system_error(errno, std::generic_category());
  }

  return std::make_shared<Socket>(retval);
}

size_t Socket::send(const void * data, const size_t & length) {
  auto retval = socket_send(fd, data, length, MSG_NOSIGNAL);
  if (retval == -1) {
    throw std::system_error(errno, std::generic_category());
  }

  return retval;
}

size_t Socket::send_to(const void * data, const size_t & length, const Address & address) {
  auto sa = address.sockaddr_in();
  auto retval = sendto(fd, data, length, MSG_NOSIGNAL, (struct sockaddr *)&sa, sizeof(sa));
  if (retval == -1) {
    throw std::system_error(errno, std::generic_category());
  }

  return retval;
}

size_t Socket::receive(void * data, const size_t & length) {
  auto retval = recv(fd, data, length, 0);
  if (retval == -1) {
    if (errno == EAGAIN) {
      return 0;
    }

    throw std::system_error(errno, std::generic_category());
  }

  return retval;
}

void Socket::set_status_flags(const int & flags) {
  if (fcntl(fd, F_SETFL, flags) == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}

int Socket::get_status_flags() const {
  auto flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    throw std::system_error(errno, std::generic_category());
  }

  return flags;
}

void Socket::set_status_flag(const int & key, const bool & enable) {
  auto flags = get_status_flags();
  set_status_flags(enable ? (flags | key) : (flags & ~key));
}

bool Socket::get_status_flag(const int & key) const {
  return get_status_flags() & key;
}

const int & Socket::get_fd() const {
  return fd;
}

}  // namespace musen

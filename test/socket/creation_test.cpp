// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <fcntl.h>

#include "gtest/gtest.h"
#include "musen/musen.hpp"

TEST(SocketCreationTest, MakeTcp) {
  auto socket = musen::make_tcp_socket();
  EXPECT_TRUE(socket->get_status_flag(O_NONBLOCK));
}

TEST(SocketCreationTest, MakeUdp) {
  auto socket = musen::make_udp_socket();
  EXPECT_TRUE(socket->get_status_flag(O_NONBLOCK));
}

TEST(SocketCreationTest, MakeSocketBlocking) {
  auto tcp_socket = musen::make_blocking_tcp_socket();
  auto udp_socket = musen::make_blocking_udp_socket();

  EXPECT_FALSE(tcp_socket->get_status_flag(O_NONBLOCK));
  EXPECT_FALSE(udp_socket->get_status_flag(O_NONBLOCK));
}

TEST(SocketCreationTest, CustomFdFromOthers) {
  auto a = musen::make_tcp_socket();
  auto b = musen::Socket(a->get_fd());

  EXPECT_EQ(a->get_fd(), b.get_fd());
}

TEST(SocketCreationTest, CatchInvalidCustomFd) {
  try {
    musen::Socket socket(-1);
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), EBADF) << "Error must be caused by invalid file descriptor";
  }
}

TEST(SocketCreationTest, CatchObsoleteCustomFdFromOthers) {
  try {
    int fd;
    {
      auto a = musen::make_tcp_socket();
      fd = a->get_fd();
    }

    musen::Socket b(fd);

    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), EBADF) << "Error must be caused by invalid file descriptor";
  }
}

TEST(SocketCreationTest, CustomType) {
  musen::Socket socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
}

TEST(SocketCreationTest, CatchInvalidCustomType) {
  try {
    musen::Socket socket(255, 255, 255);
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), EINVAL) << "Error must be caused by invalid type";
  }
}

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

#include <gtest/gtest.h>
#include <musen/musen.hpp>

#include <map>
#include <memory>
#include <string>

TEST(SocketTest, MakeTcp) {
  auto socket = musen::make_tcp_socket();
}

TEST(SocketTest, MakeUdp) {
  auto socket = musen::make_udp_socket();
}

TEST(SocketTest, CustomFdFromOthers) {
  auto a = musen::make_tcp_socket();
  auto b = musen::Socket(a->get_fd());

  EXPECT_EQ(a->get_fd(), b.get_fd());
}

TEST(SocketTest, CatchInvalidCustomFd) {
  try {
    musen::Socket socket(-1);
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), EBADF) << "Error must be caused of invalid file descriptor";
  }
}

TEST(SocketTest, CatchObsoleteCustomFdFromOthers) {
  try {
    int fd;
    {
      auto a = musen::make_tcp_socket();
      fd = a->get_fd();
    }

    musen::Socket b(fd);

    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), EBADF) << "Error must be caused of invalid file descriptor";
  }
}

TEST(SocketTest, CustomType) {
  musen::Socket socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
}

TEST(SocketTest, CatchInvalidCustomType) {
  try {
    musen::Socket socket(255, 255, 255);
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), EINVAL) << "Error must be cause of invalid type";
  }
}

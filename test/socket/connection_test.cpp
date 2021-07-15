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

TEST(SocketConnectionTest, Bind) {
  auto a = musen::make_udp_socket();
  auto b = musen::make_udp_socket();

  auto address = musen::Address("127.0.0.1", 5000);

  // First bind must be success
  a->bind(address);

  // Second bind must be failed because of address already in use
  try {
    b->bind(address);
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    ASSERT_EQ(err.code().value(), EADDRINUSE) << "Error must be caused by address already in use";
  }
}

TEST(SocketConnectionTest, Connect) {
  auto udp_socket = musen::make_udp_socket(false);
  auto tcp_socket = musen::make_tcp_socket(false);

  musen::Address address("127.0.0.1", 5000);

  // Connect will work directly on UDP socket
  udp_socket->connect(address);

  // Connect will failed on TCP socket if a server doesn't exist
  try {
    tcp_socket->connect(address);
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    ASSERT_EQ(err.code().value(), ECONNREFUSED) << "Error must be caused by refused connection";
  }
}

TEST(SocketConnectionTest, Listen) {
  auto tcp_socket = musen::make_tcp_socket();
  auto udp_socket = musen::make_udp_socket();

  // Listen will work on TCP socket
  tcp_socket->listen();

  // Listen is not supported on UDP socket
  try {
    udp_socket->listen();
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    ASSERT_EQ(err.code().value(), EOPNOTSUPP) << "Error must be caused by not supported operation";
  }
}

TEST(SocketConnectionTest, SendToNoOne) {
  auto socket = musen::make_udp_socket();

  char data[4] = {'a', 'b', 'c', 'd'};
  auto sent = socket->send_to(data, sizeof(data), musen::Address("1.2.3.4", 5000));

  ASSERT_EQ(sent, sizeof(data));
}

TEST(SocketConnectionTest, ReceiveNothing) {
  auto socket = musen::make_udp_socket();

  char data[32];
  auto received = socket->receive(data, sizeof(data));

  ASSERT_EQ(received, 0u);
}

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

TEST(SocketTest, Initialize) {
  musen::BaseSocket a;
  ASSERT_LT(a.get_sockfd(), 0);

  musen::BaseSocket b(1);
  ASSERT_EQ(b.get_sockfd(), 1);
}

TEST(SocketTest, ConnectDisconnect) {
  std::map<std::string, std::shared_ptr<musen::BaseSocket>> sockets = {
    {"UDP socket", std::make_shared<musen::UdpSocket>()},
    {"TCP socket", std::make_shared<musen::TcpSocket>()}
  };

  for (auto & socket_pair : sockets) {
    auto name = socket_pair.first;
    auto socket = socket_pair.second;

    ASSERT_FALSE(socket->is_connected()) << "The " << name << " must be disconnected";

    // Trying to connect the socket
    ASSERT_TRUE(socket->connect()) << "Unable to connect the " << name;
    ASSERT_TRUE(socket->is_connected()) << "The " << name << " must be connected";

    // Must be failed because the socket is already connected
    ASSERT_FALSE(socket->connect()) << "Must be unable to connect the " << name;
    ASSERT_TRUE(socket->is_connected()) << "The " << name << " must be connected";

    // Trying to disconnect the socket
    ASSERT_TRUE(socket->disconnect()) << "Unable to disconnect the " << name;
    ASSERT_FALSE(socket->is_connected()) << "The " << name << " must be disconnected";

    // Must be failed because the socket is already disconnected
    ASSERT_FALSE(socket->disconnect()) << "Must be unable to connect the " << name;
    ASSERT_FALSE(socket->is_connected()) << "The " << name << " must be disconnected";
  }
}

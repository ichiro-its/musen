#include "gtest/gtest.h"
#include "musen/musen.hpp"

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
    EXPECT_EQ(err.code().value(), EADDRINUSE) << "Error must be caused by address already in use";
  }
}

TEST(SocketConnectionTest, Connect) {
  auto udp_socket = musen::make_blocking_udp_socket();
  auto tcp_socket = musen::make_blocking_tcp_socket();

  musen::Address address("127.0.0.1", 5000);

  // Connect will work directly on UDP socket
  udp_socket->connect(address);

  // Connect will failed on TCP socket if a server doesn't exist
  try {
    tcp_socket->connect(address);
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), ECONNREFUSED) << "Error must be caused by refused connection";
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
    EXPECT_EQ(err.code().value(), EOPNOTSUPP) << "Error must be caused by not supported operation";
  }
}

TEST(SocketConnectionTest, AcceptNothing) {
  auto socket = musen::make_tcp_socket();

  // Listen is required before accepting a connection
  socket->listen();

  // Accept nothing will results in error
  try {
    socket->accept();
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), EAGAIN) << "Error must be caused by unavailable resource";
  }
}

TEST(SocketConnectionTest, SendNothing) {
  auto socket = musen::make_udp_socket();
  socket->connect(musen::Address("127.0.0.1", 5000));

  auto sent = socket->send(nullptr, 0);

  EXPECT_EQ(sent, 0u);
}

TEST(SocketConnectionTest, SendToNoOne) {
  auto socket = musen::make_udp_socket();

  char data[4] = {'a', 'b', 'c', 'd'};
  auto sent = socket->send_to(data, sizeof(data), musen::Address("1.2.3.4", 5000));

  EXPECT_EQ(sent, sizeof(data));
}

TEST(SocketConnectionTest, ReceiveNothing) {
  auto socket = musen::make_udp_socket();

  char data[32];
  auto received = socket->receive(data, sizeof(data));

  EXPECT_EQ(received, 0u);
}

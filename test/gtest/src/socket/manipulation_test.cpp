#include <fcntl.h>

#include <memory>

#include "gtest/gtest.h"
#include "musen/musen.hpp"

class SocketManipulationTest : public ::testing::Test {
 protected:
  void SetUp() override {
    socket = musen::make_tcp_socket();
  }

  std::shared_ptr<musen::Socket> socket;
};

TEST_F(SocketManipulationTest, GetStatusFlags) {
  auto flags = socket->get_status_flags();
  EXPECT_NE(flags, -1) << "Invalid status flags";
}

TEST_F(SocketManipulationTest, SetStatusFlags) {
  socket->set_status_flags(socket->get_status_flags() | O_NONBLOCK);
}

TEST_F(SocketManipulationTest, CatchInvalidSetStatusFlags) {
  try {
    socket->set_status_flags(-1);
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), EINVAL) << "Error must be caused by invalid flags";
  }
}

TEST_F(SocketManipulationTest, EnableDisableFlag) {
  socket->set_status_flag(O_NONBLOCK, false);
  ASSERT_FALSE(socket->get_status_flag(O_NONBLOCK));

  socket->set_status_flag(O_NONBLOCK, true);
  ASSERT_TRUE(socket->get_status_flag(O_NONBLOCK));
}

TEST_F(SocketManipulationTest, SetGetOption) {
  socket->set_option(SO_KEEPALIVE, 1);
  ASSERT_EQ(socket->get_option<bool>(SO_KEEPALIVE), 1);

  socket->set_option(SO_KEEPALIVE, 0);
  ASSERT_EQ(socket->get_option<int>(SO_KEEPALIVE), 0);
}

TEST_F(SocketManipulationTest, CatchInvalidSetOptionKey) {
  try {
    socket->set_option(-1, 0);
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), ENOPROTOOPT) << "Error must be caused by invalid option key";
  }
}

TEST_F(SocketManipulationTest, CatchInvalidGetOptionKey) {
  try {
    socket->get_option<int>(-1);
    FAIL() << "Expected a system error";
  } catch (const std::system_error & err) {
    EXPECT_EQ(err.code().value(), ENOPROTOOPT) << "Error must be caused by invalid option key";
  }
}

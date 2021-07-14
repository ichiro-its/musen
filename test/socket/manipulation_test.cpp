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

#include <fcntl.h>
#include <gtest/gtest.h>
#include <musen/musen.hpp>

#include <memory>

class SocketManipulationTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    socket = musen::make_tcp_socket();
  }

  std::shared_ptr<musen::Socket> socket;
};

TEST_F(SocketManipulationTest, GetStatusFlags) {
  auto flags = socket->get_status_flags();
  ASSERT_NE(flags, -1) << "Invalid status flags";
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

TEST_F(SocketManipulationTest, EnableDisableNonBlocking) {
  socket->set_non_blocking(false);
  ASSERT_FALSE(socket->is_non_blocking());

  socket->set_non_blocking(true);
  ASSERT_TRUE(socket->is_non_blocking());

  socket->set_non_blocking(false);
  ASSERT_FALSE(socket->is_non_blocking());
}

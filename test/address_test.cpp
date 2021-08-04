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

#include "gtest/gtest.h"
#include "musen/musen.hpp"

TEST(AddressTest, EmptyInitialization) {
  musen::Address address;
}

TEST(AddressTest, MakeAnyAddress) {
  auto address = musen::make_any_address(0);

  auto sa = address.sockaddr_in();
  EXPECT_EQ(sa.sin_addr.s_addr, htonl(INADDR_ANY)) << "Expected an INADDR_ANY value";
}

TEST(AddressTest, Conversion) {
  auto a = musen::Address("127.0.0.1", 5000);
  auto b = musen::Address(a.sockaddr_in());

  EXPECT_EQ(a.ip, b.ip) << "Expected an equal IP";
  EXPECT_EQ(a.port, b.port) << "Expected an equal port";
}

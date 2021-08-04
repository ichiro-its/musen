// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

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

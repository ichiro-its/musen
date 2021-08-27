// Copyright (c) 2021 ICHIRO ITS
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "musen/sender.hpp"

namespace musen {

template<typename T>
size_t Sender::send(const T & data) {
  return send_raw((const char *)&data, sizeof(data));
}

}  // namespace musen

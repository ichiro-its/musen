#pragma once

#include "musen/sender.hpp"

namespace musen {

template<typename T>
size_t Sender::send(const T & data) {
  return send_raw((const char *)&data, sizeof(data));
}

}  // namespace musen

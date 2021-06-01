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

#ifndef MUSEN__CLIENT__CLIENT_HPP_
#define MUSEN__CLIENT__CLIENT_HPP_

#include <memory>
#include <optional>  // NOLINT
#include <string>
#include <utility>

#include "./base_client.hpp"

namespace musen
{

template<typename T>
class Client : public BaseClient
{
public:
  explicit Client(
    const std::string & host, const int & port,
    std::shared_ptr<TcpSocket> tcp_socket = std::make_shared<TcpSocket>())
  : BaseClient(host, port, tcp_socket)
  {
  }

  std::optional<T> receive()
  {
    T data;

    int received = BaseClient::receive(&data, sizeof(T));

    if (received < (signed)sizeof(T)) {
      return std::nullopt;
    }

    return std::make_optional<T>(std::move(data));
  }

  int send(const T & data)
  {
    return BaseClient::send((const char *)&data, sizeof(data));
  }
};

}  // namespace musen

#endif  // MUSEN__CLIENT__CLIENT_HPP_

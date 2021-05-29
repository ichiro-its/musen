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

#ifndef MUSEN__SERVER__SERVER_HPP_
#define MUSEN__SERVER__SERVER_HPP_

#include <memory>
#include <string>

#include "./base_server.hpp"

namespace musen
{

template<typename T>
class Server : public BaseServer
{
public:
  explicit Server(
    const int & port, std::shared_ptr<TcpSocket> tcp_socket = std::make_shared<TcpSocket>())
  : BaseServer(port, tcp_socket)
  {
  }

  std::shared_ptr<T> receive()
  {
    auto data = std::make_shared<T>();

    int received = BaseServer::receive(data.get(), sizeof(T));

    if (received < (signed)sizeof(T)) {
      return nullptr;
    }

    return data;
  }

  int send(const T & data)
  {
    return BaseServer::send((const char *)&data, sizeof(data));
  }
};

}  // namespace musen

#endif  // MUSEN__SERVER__SERVER_HPP_

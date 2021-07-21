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

#include <musen/musen.hpp>

#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

struct Position
{
  int x;
  int y;
  int z;
};

int main()
{
  int port = 5000;

  try {
    musen::Broadcaster broadcaster(port);

    while (true) {
      Position position;

      position.x = std::rand() % 100;
      position.y = std::rand() % 100;
      position.z = std::rand() % 100;

      broadcaster.send(position);

      std::cout << "Sent: " <<
        position.x << ", " <<
        position.y << ", " <<
        position.z << std::endl;

      std::this_thread::sleep_for(1s);
    }
  } catch (const std::system_error & err) {
    std::cerr << "Failed to start the broadcaster on port " << port << "! " <<
      err.what() << std::endl;

    return err.code().value();
  }

  return 0;
}

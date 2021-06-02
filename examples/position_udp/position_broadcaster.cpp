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

#include <stdlib.h>
#include <unistd.h>

#include <ctime>
#include <iostream>

struct Position
{
  int x;
  int y;
  int z;
};

int main()
{
  musen::Broadcaster broadcaster(8080);

  broadcaster.enable_broadcast(false);
  broadcaster.add_target_host("localhost");

  if (!broadcaster.connect()) {
    std::cerr << "Failed to connect broadcaster on port " <<
      broadcaster.get_port() << "!" << std::endl;

    return 1;
  }

  unsigned int seed = time(NULL);

  while (true) {
    Position position;

    position.x = rand_r(&seed) % 100;
    position.y = rand_r(&seed) % 100;
    position.z = rand_r(&seed) % 100;

    broadcaster.send(position);

    std::cout << "Sent: " <<
      position.x << ", " <<
      position.y << ", " <<
      position.z << std::endl;

    sleep(1);
  }

  broadcaster.disconnect();

  return 0;
}
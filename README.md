# Musen (無線)

[![latest version](https://img.shields.io/github/v/release/ichiro-its/musen)](https://github.com/ichiro-its/musen/releases/)
[![commits since latest version](https://img.shields.io/github/commits-since/ichiro-its/musen/latest)](https://github.com/ichiro-its/musen/commits/master)
[![license](https://img.shields.io/github/license/ichiro-its/musen)](./LICENSE)

[Musen](https://ichiro-its.github.io/musen) (無線, wireless) is a package that provides [UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol) and [TCP](https://en.wikipedia.org/wiki/Transmission_Control_Protocol) socket communication library.
This library is written in [C++](https://isocpp.org/) and currently only works on [Linux](https://www.linux.org/) based operating system.
In this library, the UDP socket communication will be handled by Broadcaster and Listener objects while the TCP socket communication will be handled by Server, Session, and Client objects.

## Features

- Send and receive data on UDP socket using Broadcaster and Listener objects.
- Send and receive data on TCP socket using Server, Session, and Client objects.
- Specify the port number and target hosts to be used.
- Message serialization support for string, list of string, and struct data.

## Requirement

- [C++17](https://en.cppreference.com/w/cpp/compiler_support).
- Documentation requirement (optional):
  - [Doxygen](https://www.doxygen.nl/).
- Test requirement (optional):
  - [Uncrustify](http://uncrustify.sourceforge.net/).
  - [GoogleTest](https://google.github.io/googletest/).

## Installation

### Binary Packages

See [releases](https://github.com/ichiro-its/musen/releases) for the latest version of this package.

### Build From Source

- Create a build directory and change the working directory to it.
  ```sh
  $ mkdir build && cd build
  ```
- Configure CMake and build the project.
  ```sh
  $ cmake .. && make
  ```
  > Optionally, you could speed up the build process by specifying the parallel job using `-j` option, see [this](https://www.gnu.org/software/make/manual/html_node/Parallel.html).
- (Optional) generate documentation using the configured CMake.
  ```sh
  $ make doc
  ```
- (Optional) format source code using the configured CMake.
  ```sh
  $ make lint
  ```
- (Optional) reconfigure CMake to build a Debian packages.
  ```sh
  $ cmake -DCMAKE_INSTALL_PREFIX=/usr .. && make && cpack
  ```
- (Optional) reconfigure CMake to run the unit tests.
  ```sh
  $ cmake -DBUILD_TESTING=ON .. && make && ctest --verbose
  ```

## Usages

See [examples](https://github.com/ichiro-its/musen/blob/master/examples) for information on how to use this package.

## Documentation

You can read the full API documentation in the generated `doc` directory (see [this](#Build-From-Source)) or in [here](https://ichiro-its.github.io/musen).

## License

This project is licensed under the terms of the [MIT License](https://github.com/ichiro-its/musen/blob/master/LICENSE).

Copyright © 2021-2024 [ICHIRO ITS](https://ichiro.its.ac.id/)

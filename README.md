# Musen (無線)

[![latest version](https://img.shields.io/github/v/release/ichiro-its/musen)](https://github.com/ichiro-its/musen/releases/)
[![commits since latest version](https://img.shields.io/github/commits-since/ichiro-its/musen/latest)](https://github.com/ichiro-its/musen/commits/master)
[![milestone](https://img.shields.io/github/milestones/progress/ichiro-its/musen/1?label=milestone)](https://github.com/ichiro-its/musen/milestone/2)
[![license](https://img.shields.io/github/license/ichiro-its/musen)](./LICENSE)
[![build and test status](https://img.shields.io/github/workflow/status/ichiro-its/musen/Build%20and%20Test?label=build%20and%20test)](https://github.com/ichiro-its/musen/actions)

Musen (無線, wireless) is a [ROS 2](https://docs.ros.org/en/foxy/index.html) package that provides a [UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol) broadcast communication library for a ROS 2 project.
This package is written in C++ and currently only works on Linux based operating system.
In this package, the broadcast communication will be handled by a Broadcaster object that will send a message to multiple targets and a Listener object that will receive a message from multiple sources.

## Features

- Send and receive data on UDP using Broadcaster and Listener objects.
- Specify the port number and target hosts to be used.
- Message serialization support for string, list of string, and struct data.

## Requirement

- This package requires [ROS 2 Foxy Fitzroy](https://docs.ros.org/en/foxy/).

## Installation

### Binary Packages

- See [releases](https://github.com/ichiro-its/musen/releases) for the latest version of this package.
- Alternatively, this package is available on [ICHIRO ITS Repository](https://repository.ichiro-its.org/) as a `ros-foxy-musen` package.

### Build From Source

- Install colcon as in [this guide](https://colcon.readthedocs.io/en/released/user/installation.html).
- Build the project and source the result.
  ```bash
  $ colcon build
  $ source install/setup.bash
  ```
  > See [this guide](https://docs.ros.org/en/foxy/Tutorials/Workspace/Creating-A-Workspace.html) for more information on how to setup a workspace in ROS 2.

## Usages

See [examples](./examples) for information on how to use this package.

## License

This project is maintained by [ICHIRO ITS](https://ichiro-its.org/) and licensed under the [MIT License](./LICENSE).

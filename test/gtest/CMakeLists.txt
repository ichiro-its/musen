# Copyright (c) 2021-2024 ICHIRO ITS
#
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT.

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}_tests
  "src/send_and_receive/mocks/mock_receiver.cpp"
  "src/send_and_receive/mocks/mock_receiver.hpp"
  "src/send_and_receive/mocks/mock_sender.cpp"
  "src/send_and_receive/mocks/mock_sender.hpp"
  "src/send_and_receive/raw_test.cpp"
  "src/send_and_receive/string_test.cpp"
  "src/send_and_receive/strings_test.cpp"
  "src/send_and_receive/struct_test.cpp"
  "src/socket/connection_test.cpp"
  "src/socket/creation_test.cpp"
  "src/socket/manipulation_test.cpp"
  "src/address_test.cpp"
  "src/broadcast_and_listen_test.cpp"
  "src/server_and_client_test.cpp")

target_link_libraries(${PROJECT_NAME}_tests
  ${PROJECT_NAME} gtest gtest_main Threads::Threads)

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME}_tests)

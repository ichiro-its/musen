cmake_minimum_required(VERSION 3.5)

project(musen)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Wpedantic -fPIC)

install(DIRECTORY "include" DESTINATION ".")

add_library(${PROJECT_NAME} SHARED
  "src/tcp/client.cpp"
  "src/tcp/server.cpp"
  "src/tcp/session.cpp"
  "src/udp/broadcaster.cpp"
  "src/udp/listener.cpp"
  "src/address.cpp"
  "src/receiver.cpp"
  "src/sender.cpp"
  "src/socket.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>")

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_export
  ARCHIVE DESTINATION "lib"
  LIBRARY DESTINATION "lib"
  RUNTIME DESTINATION "bin")

include_directories("examples")

add_custom_target(lint
  COMMAND find "examples" "include" "src" "test" '\(' -name '*.cpp' -o -name '*.hpp' -o -name '*.tpp' '\)' -exec uncrustify -c test/uncrustify/config.cfg --replace --no-backup {} '\;'
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

if(BUILD_TESTING)
  enable_testing()

  add_test(NAME ${PROJECT_NAME}_cppcheck_test
    COMMAND cppcheck "examples" "include" "src" "test"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

  add_test(NAME ${PROJECT_NAME}_cpplint_test
    COMMAND cpplint --linelength=120 --filter=-build/c++11 --headers=tpp --recursive "examples" "include" "src" "test"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

  add_test(NAME ${PROJECT_NAME}_uncrustify_test
    COMMAND python test/uncrustify/test.py "examples" "include" "src" "test"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

  include_directories("test/gtest")
endif()

install(EXPORT ${PROJECT_NAME}_export
  FILE ${PROJECT_NAME}-config.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "lib/cmake/${PROJECT_NAME}")

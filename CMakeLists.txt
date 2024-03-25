cmake_minimum_required(VERSION 3.5)

project(musen)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Wpedantic -fPIC)

install(DIRECTORY "include/musen"
  DESTINATION "include")

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

add_subdirectory("examples")

include("cmake/doc.cmake")

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory("test/gtest")
endif()

install(EXPORT ${PROJECT_NAME}_export
  FILE ${PROJECT_NAME}-config.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "lib/cmake/${PROJECT_NAME}")

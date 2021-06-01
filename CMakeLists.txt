cmake_minimum_required(VERSION 3.5)

project(musen)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)

install(DIRECTORY "include" DESTINATION ".")

add_library(${PROJECT_NAME}
  "src/broadcaster/base_broadcaster.cpp"
  "src/broadcaster/string_broadcaster.cpp"
  "src/listener/base_listener.cpp"
  "src/listener/string_listener.cpp"
  "src/socket/base_socket.cpp"
  "src/socket/tcp_socket.cpp"
  "src/socket/udp_socket.cpp"
  "src/client/base_client.cpp"
  "src/server/base_server.cpp"
  "src/receiver.cpp"
  "src/sender.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION "lib"
  LIBRARY DESTINATION "lib"
  RUNTIME DESTINATION "bin")

set(EXECUTABLES
  "examples/fruits_udp/fruits_broadcaster.cpp"
  "examples/fruits_udp/fruits_listener.cpp"
  "examples/hello_world_udp/hello_world_broadcaster.cpp"
  "examples/hello_world_udp/hello_world_listener.cpp"
  "examples/position_tcp/position_client.cpp"
  "examples/position_tcp/position_server.cpp"
  "examples/position_udp/position_broadcaster.cpp"
  "examples/position_udp/position_listener.cpp")

foreach(EXECUTABLE ${EXECUTABLES})
  get_filename_component(TARGET ${EXECUTABLE} NAME_WE)

  add_executable(${TARGET} ${EXECUTABLE})
  target_link_libraries(${TARGET} ${PROJECT_NAME})

  install(TARGETS ${TARGET}
    DESTINATION "lib/${PROJECT_NAME}")
endforeach()

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  find_package(ament_lint_auto REQUIRED)

  ament_add_gtest(${PROJECT_NAME}_tests
    "test/base_broadcaster_and_listener_test.cpp"
    "test/broadcaster_and_listener_test.cpp"
    "test/string_broadcaster_and_listener_test.cpp"
    "test/base_client_and_server_test.cpp"
    "test/client_and_server_test.cpp"
    "test/tcp_socket_test.cpp"
    "test/udp_socket_test.cpp")

  target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME})

  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories("include")
ament_export_libraries(${PROJECT_NAME})
ament_package()

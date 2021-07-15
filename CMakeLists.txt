cmake_minimum_required(VERSION 3.5)

project(musen)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -fPIC)
endif()

find_package(ament_cmake REQUIRED)

install(DIRECTORY "include" DESTINATION ".")

add_library(${PROJECT_NAME} SHARED
  "src/tcp/client.cpp"
  "src/tcp/server.cpp"
  "src/tcp/session.cpp"
  "src/udp/broadcaster.cpp"
  "src/udp/listener.cpp"
  "src/receiver.cpp"
  "src/sender.cpp"
  "src/socket.cpp")

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
    "test/send_and_receive/raw_test.cpp"
    "test/send_and_receive/string_test.cpp"
    "test/send_and_receive/strings_test.cpp"
    "test/send_and_receive/struct_test.cpp"
    # "test/server_and_client_test.cpp"
    "test/socket/connection_test.cpp"
    "test/socket/creation_test.cpp"
    "test/socket/manipulation_test.cpp"
    "test/broadcast_and_listen_test.cpp")

  target_include_directories(${PROJECT_NAME}_tests PUBLIC "test/include")

  target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME})

  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories("include")
ament_export_libraries(${PROJECT_NAME})
ament_package()

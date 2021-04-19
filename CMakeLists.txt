cmake_minimum_required(VERSION 3.5)

project(musen)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)

add_library(${PROJECT_NAME}
  "src/broadcaster/base_broadcaster.cpp"
  "src/broadcaster/string_broadcaster.cpp"
  "src/listener/base_listener.cpp"
  "src/listener/string_listener.cpp"
  "src/udp_socket.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(DIRECTORY "include" DESTINATION ".")

install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION "lib"
  LIBRARY DESTINATION "lib"
  RUNTIME DESTINATION "bin")

add_executable(hello_world_broadcaster "examples/hello_world_broadcaster.cpp")
target_include_directories(hello_world_broadcaster PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(hello_world_broadcaster ${PROJECT_NAME})

add_executable(hello_world_listener "examples/hello_world_listener.cpp")
target_include_directories(hello_world_listener PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(hello_world_listener ${PROJECT_NAME})

add_executable(position_broadcaster "examples/position_broadcaster.cpp")
target_include_directories(position_broadcaster PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(position_broadcaster ${PROJECT_NAME})

add_executable(position_listener "examples/position_listener.cpp")
target_include_directories(position_listener PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(position_listener ${PROJECT_NAME})

add_executable(fruits_broadcaster "examples/fruits_broadcaster.cpp")
target_include_directories(fruits_broadcaster PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(fruits_broadcaster ${PROJECT_NAME})

add_executable(fruits_listener "examples/fruits_listener.cpp")
target_include_directories(fruits_listener PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_link_libraries(fruits_listener ${PROJECT_NAME})

install(
  TARGETS
    hello_world_broadcaster
    hello_world_listener
    position_broadcaster
    position_listener
    fruits_broadcaster
    fruits_listener
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories("include")
ament_export_libraries(${PROJECT_NAME})
ament_package()

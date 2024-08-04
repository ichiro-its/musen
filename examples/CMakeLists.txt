set(EXAMPLES
  "fruits_udp/fruits_broadcaster.cpp"
  "fruits_udp/fruits_listener.cpp"
  "hello_world_udp/hello_world_broadcaster.cpp"
  "hello_world_udp/hello_world_listener.cpp"
  "position_tcp/position_client.cpp"
  "position_tcp/position_server.cpp"
  "position_udp/position_broadcaster.cpp"
  "position_udp/position_listener.cpp")

foreach(EXAMPLE ${EXAMPLES})
  get_filename_component(TARGET "${EXAMPLE}" NAME_WE)

  add_executable(${TARGET} "${EXAMPLE}")
  target_link_libraries(${TARGET} ${PROJECT_NAME})

  if(MUSEN_ENABLE_INSTALL)
    install(TARGETS ${TARGET} DESTINATION "bin")
  endif()
endforeach()

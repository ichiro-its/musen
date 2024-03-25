add_custom_target(lint
  COMMAND
    find "examples" "include" "src" "test"
    '\(' -name '*.cpp' -o -name '*.hpp' -o -name '*.tpp' '\)'
    -exec uncrustify -c test/uncrustify/config.cfg -l CPP --replace --no-backup {} '\;'
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

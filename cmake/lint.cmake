# Copyright (c) 2021 ICHIRO ITS
#
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT.

add_custom_target(lint
  COMMAND
    find "examples" "include" "src" "test"
    '\(' -name '*.cpp' -o -name '*.hpp' -o -name '*.tpp' '\)'
    -exec uncrustify -c test/uncrustify/config.cfg -l CPP --replace --no-backup {} '\;'
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

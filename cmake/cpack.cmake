# Copyright (c) 2021 ICHIRO ITS
#
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT.

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

set(CPACK_DEBIAN_PACKAGE_NAME "lib${PROJECT_NAME}-dev")

set(CPACK_DEBIAN_PACKAGE_VERSION 1.1.0)

string(TIMESTAMP NOW "%s")
set(CPACK_DEBIAN_PACKAGE_RELEASE ${NOW})

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "UDP and TCP socket communication library")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alfi Maulana <alfi.maulana.f@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/ichiro-its/musen")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "build-essential")

include(CPack)

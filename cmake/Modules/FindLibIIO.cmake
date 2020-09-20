# - Try to find libiio
# Once done this will define
#  LIBIIO_FOUND - System has libiio
#  LIBIIO_INCLUDE_DIRS - The libiio include directories
#  LIBIIO_LIBRARIES - The libraries needed to use libiio
#  LIBIIO_DEFINITIONS - Compiler switches required for using libiio

find_package(PkgConfig)
pkg_check_modules(LIBIIO_PKG QUIET libiio)
set(LIBIIO_DEFINITIONS ${LIBIIO_PKG_CFLAGS_OTHER})

find_path(LIBIIO_INCLUDE_DIR NAMES iio.h
          HINTS ${LIBIIO_PKG_INCLUDE_DIRS}
          PATHS
          /usr/include
          /usr/local/include )

find_library(LIBIIO_LIBRARY NAMES iio
             HINTS ${LIBIIO_PKG_LIBRARY_DIRS}
             PATHS
             /usr/lib
             /usr/local/lib )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBIIO_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LIBIIO  DEFAULT_MSG
                                  LIBIIO_LIBRARY LIBIIO_INCLUDE_DIR)

mark_as_advanced(LIBIIO_LIBRARY LIBIIO_INCLUDE_DIR)

set(LIBIIO_LIBRARIES ${LIBIIO_LIBRARY} )
set(LIBIIO_INCLUDE_DIRS ${LIBIIO_INCLUDE_DIR} )
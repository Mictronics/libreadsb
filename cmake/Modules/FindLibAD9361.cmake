# - Try to find libad9361
# Once done this will define
#  LIBAD9361_FOUND - System has libad9361
#  LIBAD9361_INCLUDE_DIRS - The libad9361 include directories
#  LIBAD9361_LIBRARIES - The libraries needed to use libad9361
#  LIBAD9361_DEFINITIONS - Compiler switches required for using libad9361

find_package(PkgConfig)
pkg_check_modules(LIBAD9361_PKG QUIET libad9361)
set(LIBAD9361_DEFINITIONS ${LIBAD9361_PKG_CFLAGS_OTHER})

find_path(LIBAD9361_INCLUDE_DIR NAMES ad9361.h
          HINTS ${LIBAD9361_PKG_INCLUDE_DIRS}
          PATHS
          /usr/include
          /usr/local/include )

find_library(LIBAD9361_LIBRARY NAMES ad9361
             HINTS ${LIBAD9361_PKG_LIBRARY_DIRS}
             PATHS
             /usr/lib
             /usr/local/lib )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBAD9361_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LIBAD9361  DEFAULT_MSG
                                  LIBAD9361_LIBRARY LIBAD9361_INCLUDE_DIR)

mark_as_advanced(LIBAD9361_LIBRARY LIBAD9361_INCLUDE_DIR)

set(LIBAD9361_LIBRARIES ${LIBAD9361_LIBRARY} )
set(LIBAD9361_INCLUDE_DIRS ${LIBAD9361_INCLUDE_DIR} )
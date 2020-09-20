# - Try to find libbladerf
# Once done this will define
#  LIBBLADERF_FOUND - System has libbladerf
#  LIBBLADERF_INCLUDE_DIRS - The libbladerf include directories
#  LIBBLADERF_LIBRARIES - The libraries needed to use libbladerf
#  LIBBLADERF_DEFINITIONS - Compiler switches required for using libbladerf

find_package(PkgConfig)
pkg_check_modules(LIBBLADERF_PKG QUIET libbladeRF)
set(LIBBLADERF_DEFINITIONS ${LIBBLADERF_PKG_CFLAGS_OTHER})

find_path(LIBBLADERF_INCLUDE_DIR NAMES libbladeRF.h
          HINTS ${LIBBLADERF_PKG_INCLUDE_DIRS}
          PATHS
          /usr/include
          /usr/local/include )

find_library(LIBBLADERF_LIBRARY NAMES bladeRF
             HINTS ${LIBBLADERF_PKG_LIBRARY_DIRS}
             PATHS
             /usr/lib
             /usr/local/lib )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBBLADERF_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LIBBLADERF  DEFAULT_MSG
                                  LIBBLADERF_LIBRARY LIBBLADERF_INCLUDE_DIR)

mark_as_advanced(LIBBLADERF_LIBRARY LIBBLADERF_INCLUDE_DIR)

set(LIBBLADERF_LIBRARIES ${LIBBLADERF_LIBRARY} )
set(LIBBLADERF_INCLUDE_DIRS ${LIBBLADERF_INCLUDE_DIR} )

########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 3.16.0)
project(libreadsb VERSION 0.1.0 DESCRIPTION "A Mode-S/ADSB/TIS decoder library." LANGUAGES C)

# Select the release build type by default to get optimization flags
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Debug")
   message(STATUS "Build type not specified: defaulting to debug.")
endif(NOT CMAKE_BUILD_TYPE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

if(NOT LIB_INSTALL_DIR)
   set(LIB_INSTALL_DIR lib)
endif()

# Set the version information here
set(VERSION_INFO_MAJOR_VERSION 0) # increment major on api compatibility changes
set(VERSION_INFO_MINOR_VERSION 1) # increment minor on feature-level changes
set(VERSION_INFO_PATCH_VERSION git) # increment patch for bug fixes and docs
include(Version) # setup version info

########################################################################
# Compiler specific setup
########################################################################
if(CMAKE_COMPILER_IS_GNUCC AND NOT WIN32)
    ADD_DEFINITIONS(-Wall)
    ADD_DEFINITIONS(-Wextra)
    ADD_DEFINITIONS(-Wno-unused-parameter)
    ADD_DEFINITIONS(-Wno-unused)
    ADD_DEFINITIONS(-Wsign-compare)
    ADD_DEFINITIONS(-Wdeclaration-after-statement)
    ADD_DEFINITIONS(-Wmissing-declarations)
    ADD_DEFINITIONS(-fno-common)
    ADD_DEFINITIONS(-fvisibility=hidden)
endif()

########################################################################
# Find build dependencies
########################################################################
find_package(PkgConfig)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_library(LIBM m REQUIRED)   
find_library(LIBRT rt REQUIRED)   

option(WITH_RTLSDR "Define if you want to enable RTLSDR support." OFF)
option(WITH_BIASTEE "Define if you want to enable bias tee support on RTLSDR v3 dongles." OFF)
if (WITH_RTLSDR)
    find_package(LibRTLSDR REQUIRED)
endif()

option(WITH_BLADERF "Define if you want to enable BladeRF support." OFF)
if (WITH_BLADERF)
    find_package(LibBladeRF REQUIRED)
endif()

option(WITH_PLUTOSDR "Define if you want to enable ADLAM-PLUTO support." OFF)
if (WITH_PLUTOSDR)
    find_package(LibIIO REQUIRED)
    find_package(LibAD9361 REQUIRED)
endif()

########################################################################
# Setup the include and linker paths
########################################################################
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${LIBUSB_INCLUDE_DIR}
    ${THREADS_PTHREADS_INCLUDE_DIR}
)

########################################################################
# Create uninstall target
########################################################################
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
@ONLY)

add_custom_target(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

########################################################################
# Add subdirectories
########################################################################
add_subdirectory(include)
add_subdirectory(src)

########################################################################
# Create Pkg Config File
########################################################################

INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/libreadsb.pc
    DESTINATION ${LIB_INSTALL_DIR}/pkgconfig
)

########################################################################
# Print Summary
########################################################################
MESSAGE(STATUS "Building for version: ${VERSION} / ${LIBVER}")
MESSAGE(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")

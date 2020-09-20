if(DEFINED __INCLUDED_VERSION_CMAKE)
    return()
endif()
set(__INCLUDED_VERSION_CMAKE TRUE)

# VERSION_INFO_* variables must be provided by user
set(MAJOR_VERSION ${VERSION_INFO_MAJOR_VERSION})
set(MINOR_VERSION ${VERSION_INFO_MINOR_VERSION})
set(PATCH_VERSION ${VERSION_INFO_PATCH_VERSION})

########################################################################
# Extract the version string from git describe.
########################################################################
find_package(Git QUIET)

if(GIT_FOUND)
    message(STATUS "Extracting version information from git describe...")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=4 --long
        OUTPUT_VARIABLE GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
else()
    set(GIT_DESCRIBE "v${MAJOR_VERSION}.${MINOR_VERSION}.x-xxx-xunknown")
endif()

########################################################################
# Use the logic below to set the version constants
########################################################################
if("${PATCH_VERSION}" STREQUAL "git")
    # VERSION: 3.6git-xxx-gxxxxxxxx
    # LIBVER:  3.6git
    set(VERSION "${GIT_DESCRIBE}")
    set(LIBVER  "${MAJOR_VERSION}.${MINOR_VERSION}${PATCH_VERSION}")
else()
    # This is a numbered release.
    # VERSION: 3.6.1
    # LIBVER:  3.6.1
    set(VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
    set(LIBVER "${VERSION}")
endif()

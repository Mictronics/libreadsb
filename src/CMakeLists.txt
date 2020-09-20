MACRO(LIBREADSB_APPEND_SRCS)
    LIST(APPEND libreadsb_srcs ${ARGV})
ENDMACRO(LIBREADSB_APPEND_SRCS)

LIBREADSB_APPEND_SRCS(
    util.c
    fifo.c
    geomag.c
    crc.c
    libreadsb.c
)

add_compile_definitions(_GNU_SOURCE)

if(LIBRTLSDR_FOUND)
    message(STATUS "RTL-SDR device input will be compiled.")
    include_directories(${LIBRTLSDR_INCLUDE_DIRS})
    list(APPEND SDR_LIBRARIES ${LIBRTLSDR_LIBRARIES})
    add_compile_definitions(ENABLE_RTLSDR)
    if (WITH_BIASTEE)
        message(STATUS "RTL-SDR v3 bias tee support will be compiled.")
        add_compile_definitions(ENABLE_RTLSDR_BIASTEE)
    endif()
endif()

if(LIBBLADERF_FOUND)
    message(STATUS "BladeRF device input will be compiled.")
    include_directories(${LIBBLADERF_INCLUDE_DIRS})
    list(APPEND SDR_LIBRARIES ${LIBBLADERF_LIBRARIES})
    add_compile_definitions(ENABLE_BLADERF)
endif()

if(LIBIIO_FOUND AND LIBAD9361_FOUND)
    message(STATUS "ADLAM-Pluto device input will be compiled.")
    include_directories(${LIBIIO_INCLUDE_DIRS})
    include_directories(${LIBAD9361_INCLUDE_DIRS})
    list(APPEND SDR_LIBRARIES ${LIBIIO_LIBRARIES})
    list(APPEND SDR_LIBRARIES ${LIBAD9361_LIBRARIES})
    add_compile_definitions(ENABLE_PLUTOSDR)
endif()

########################################################################
# Setup shared library variant
########################################################################
add_library(libreadsb_shared SHARED ${libreadsb_srcs})
set(LIBREADSB_HEADERS readsb.h)

set_target_properties(libreadsb_shared PROPERTIES DEFINE_SYMBOL "libreadsb_EXPORTS")
set_target_properties(libreadsb_shared PROPERTIES OUTPUT_NAME libreadsb)
set_target_properties(libreadsb_shared PROPERTIES SOVERSION ${MAJOR_VERSION})
set_target_properties(libreadsb_shared PROPERTIES VERSION ${LIBVER})
set_target_properties(libreadsb_shared PROPERTIES PUBLIC_HEADER ${LIBREADSB_HEADERS})
set_target_properties(libreadsb_shared PROPERTIES C_STANDARD 11)
set_target_properties(libreadsb_shared PROPERTIES C_STANDARD_REQUIRED ON)
set_target_properties(libreadsb_shared PROPERTIES C_EXTENSIONS ON)
target_link_libraries(libreadsb_shared ${SDR_LIBRARIES})
if(LIBM AND LIBRT)
    target_link_libraries(libreadsb_shared ${LIBM} ${LIBRT})
endif()

########################################################################
# Setup static library variant
########################################################################
add_library(libreadsb_static STATIC ${libreadsb_srcs})

set_property(TARGET libreadsb_static APPEND PROPERTY COMPILE_DEFINITIONS "libreadsb_STATIC" )
# Force same library filename for static and shared variants of the library
set_target_properties(libreadsb_static PROPERTIES OUTPUT_NAME libreadsb)
target_link_libraries(libreadsb_static ${SDR_LIBRARIES})
if(LIBM AND LIBRT)
    target_link_libraries(libreadsb_static ${LIBM} ${LIBRT})
endif()

########################################################################
# Install built library files & utilities
########################################################################
install(TARGETS ${INSTALL_TARGETS}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
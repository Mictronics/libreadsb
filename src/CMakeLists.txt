MACRO(LIBREADSB_APPEND_SRCS)
    LIST(APPEND libreadsb_srcs ${ARGV})
ENDMACRO(LIBREADSB_APPEND_SRCS)

LIBREADSB_APPEND_SRCS(
    libreadsb.c
)

########################################################################
# Setup shared library variant
########################################################################
add_library(libreadsb_shared SHARED ${libreadsb_srcs})

set_target_properties(libreadsb_shared PROPERTIES DEFINE_SYMBOL "libreadsb_EXPORTS")
set_target_properties(libreadsb_shared PROPERTIES OUTPUT_NAME libreadsb)
set_target_properties(libreadsb_shared PROPERTIES SOVERSION ${MAJOR_VERSION})
set_target_properties(libreadsb_shared PROPERTIES VERSION ${LIBVER})

########################################################################
# Setup static library variant
########################################################################
add_library(libreadsb_static STATIC ${libreadsb_srcs})

set_property(TARGET libreadsb_static APPEND PROPERTY COMPILE_DEFINITIONS "libreadsb_STATIC" )
# Force same library filename for static and shared variants of the library
set_target_properties(libreadsb_static PROPERTIES OUTPUT_NAME libreadsb)

########################################################################
# Install built library files & utilities
########################################################################
install(TARGETS ${INSTALL_TARGETS}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION bin
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
find_package(Doxygen)

if(NOT DOXYGEN_EXECUTABLE)
    message(FATAL_ERROR "doxygen not found")
endif()

#add_custom_target(doxygen ALL)
add_custom_target(doxygen)

add_custom_command(
    TARGET doxygen
    COMMAND ${CMAKE_COMMAND} -E env 
        CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}
        CMAKE_MAJOR_VERSION=${FIRMWARE_MAJOR_VERSION}
        CMAKE_MINOR_VERSION=${FIRMWARE_MINOR_VERSION}
        CMAKE_PATCH_VERSION=${FIRMWARE_PATCH_VERSION}
        ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building Doxygen documentation"
    VERBATIM
)
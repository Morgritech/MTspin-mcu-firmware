find_program(ARDUINO_CLI_EXECUTABLE arduino-cli)

if(NOT ARDUINO_CLI_EXECUTABLE)
    message(FATAL_ERROR "arduino-cli not found")   
else()
    message(STATUS "Found arduino-cli: ${ARDUINO_CLI_EXECUTABLE}")
endif()

set(ARDUINO_SKETCH_DIR ${CMAKE_SOURCE_DIR}/src)
set(ARDUINO_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(ARDUINO_BOARDS_FILE ${CMAKE_SOURCE_DIR}/arduino_boards.txt)

file(STRINGS ${ARDUINO_BOARDS_FILE} BOARDS)

foreach(BOARD IN LISTS BOARDS)
    string(REPLACE ":" "-" ARDUINO_BUILD_SUBDIR ${BOARD})
    execute_process(
        COMMAND ${ARDUINO_CLI_EXECUTABLE} compile -b ${BOARD} ${ARDUINO_SKETCH_DIR} --clean --build-path ${ARDUINO_BUILD_DIR}/${ARDUINO_BUILD_SUBDIR} #--build-property ${ARDUINO_EXTRA_BUILD_PROPERTIES}
    )
endforeach()

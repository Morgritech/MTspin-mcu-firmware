find_program(ARDUINO_CLI_EXECUTABLE arduino-cli)

if(NOT ARDUINO_CLI_EXECUTABLE)
    message(FATAL_ERROR "...arduino-cli not found...")   
else()
    message(STATUS "...Found arduino-cli: ${ARDUINO_CLI_EXECUTABLE}...")
endif()

# Install Arduino platforms/cores

message(STATUS "...Installing Arduino platforms/cores...")

set(ARDUINO_CORES_FILE ${CMAKE_SOURCE_DIR}/arduino-cores.txt)

execute_process(
    COMMAND ${ARDUINO_CLI_EXECUTABLE} core update-index
)

file(STRINGS ${ARDUINO_CORES_FILE} CORES)

foreach(CORE IN LISTS CORES)
    execute_process(
        COMMAND ${ARDUINO_CLI_EXECUTABLE} core install ${CORE}
        COMMAND_ERROR_IS_FATAL ANY
    )
endforeach()

# Install Arduino libraries

message(STATUS "...Installing arduino libraries...")

set(ARDUINO_LIBS_FILE ${CMAKE_SOURCE_DIR}/arduino-libs.txt)

file(STRINGS ${ARDUINO_LIBS_FILE} LIBS)

foreach(LIB IN LISTS LIBS)
    execute_process(
        COMMAND ${ARDUINO_CLI_EXECUTABLE} lib install ${LIB}
        COMMAND_ERROR_IS_FATAL ANY
    )
endforeach()

# Compile the project

message(STATUS "...Compiling the project...")

set(ARDUINO_SKETCH_DIR ${CMAKE_SOURCE_DIR}/src)
set(ARDUINO_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(ARDUINO_BOARDS_FILE ${CMAKE_SOURCE_DIR}/arduino-boards.txt)

file(STRINGS ${ARDUINO_BOARDS_FILE} BOARDS)

foreach(BOARD IN LISTS BOARDS)
    string(REPLACE ":" "-" ARDUINO_BUILD_SUBDIR ${BOARD})
    execute_process(
        COMMAND ${ARDUINO_CLI_EXECUTABLE} compile -b ${BOARD} ${ARDUINO_SKETCH_DIR} --clean --build-path ${ARDUINO_BUILD_DIR}/${ARDUINO_BUILD_SUBDIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endforeach()

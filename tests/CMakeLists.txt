# ---- Project ----

project(MTspin-mcu-firmware-tests)

# ---- Add source files ----

set(
    SOURCES
    main.cc
    hello_test.cc
)

# ---- Create binary ----

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME}
    gtest
)

# ---- Enable discovery of tests by CMake ----

include(GoogleTest)

gtest_add_tests(TARGET ${PROJECT_NAME})
#gtest_discover_tests(${PROJECT_NAME}) # Results in "no tests were found" error!

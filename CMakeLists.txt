cmake_minimum_required(VERSION 3.14..3.26)

# ---- Project ----

project(MTspin-mcu-firmware)

# ---- Package/library managers ----

# CMake's FetchContent
include(FetchContent)

# ---- Dependencies ----

# Google Test
include(scripts/cmake/get-gtest.cmake)
include(scripts/cmake/get-plantuml.cmake)
include(scripts/cmake/get-doxygen-awesome-css.cmake)
include(scripts/cmake/create-version-file.cmake)
include(scripts/cmake/target-doxygen.cmake)
include(scripts/cmake/target-source-lines-of-code.cmake)

# ---- Enable CMake’s test runner ----

enable_testing()

# ---- Project subdirectories ----

add_subdirectory(tests) # Find/read testLibrary library CMakeLists.txt file.

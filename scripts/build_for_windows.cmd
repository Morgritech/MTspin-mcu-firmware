@echo off

rem Specify the sketch directory.
set SKETCH_DIR=src
rem Specify the build directory.
set BUILD_DIR=build
rem Specify the directories to files listing the required arduino boards.
set BOARDS=arduino_boards.txt
rem Build for multiple boards.
for /f "usebackq delims=" %%B in ("%BOARDS%") do (
    arduino-cli compile -b %%B %SKETCH_DIR% --build-path %BUILD_DIR%
)

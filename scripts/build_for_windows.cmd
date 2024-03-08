@echo off

rem Specify the sketch directory.
set SKETCH_DIR=src
rem Specify the build directory.
set BUILD_DIR=build
rem Specify the directories to files listing the required arduino boards.
set BOARDS=arduino_boards.txt
rem Build for multiple boards.
rem Read one line at a time (No spaces or tabs as delimiters).
setlocal enabledelayedexpansion
for /f "usebackq delims=" %%B in ("%BOARDS%") do (
    rem Replace ":" with "-" to create the build sub-folder with the board name.
    set TEMP_DIR=%%B
    set BUILD_SUBDIR=!TEMP_DIR::=-!
    rem Compile clean (no cache) and use command line args if passed to the script.
    arduino-cli compile -b %%B %SKETCH_DIR% --clean --build-path %BUILD_DIR%\!BUILD_SUBDIR! %*
)
endlocal

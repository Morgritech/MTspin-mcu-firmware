@echo off

rem User prompt.
echo ...Please run in Command Prompt (CMD)... 
echo ...Ignore if already running in CMD...

rem rem Check admin privileges.
rem net session >nul 2>&1
rem if NOT %errorLevel% == 0 (
rem     echo "Please run as admin."
rem     exit /b 1 rem Exit only the script (no the terminal); error code 1.
rem )

rem Command line arguments
set SCRIPT_NAME=%~nx0
set CMD_SETUP=-setup
set CMD_BUILD=-build
set CMD_BOTH=-both

if NOT "%~1" == "%CMD_SETUP%" (
    if NOT "%~1" == "%CMD_BOTH%" (
        GOTO build
    )
)

echo ...Installing and setting up dependencies...

winget install --accept-package-agreements -e ArduinoSA.CLI
winget install --accept-package-agreements -e Kitware.CMake
winget install --accept-package-agreements -e DimitriVanHeesch.Doxygen
winget install --accept-package-agreements -e Graphviz.Graphviz
winget install --accept-package-agreements -e AlDanial.Cloc
rem If not already added;
rem add executables directories to windows environment path (current session) so they be executed directly.
if defined IS_PATH_UPDATED (
    GOTO path_updated rem "set PATH" fails due to unknown reasons when used inside of an "if-else" block.
)
set IS_PATH_UPDATED=true
rem C:\Program Files\Arduino CLI
set ARDUINO_CLI_DIR=%ProgramFiles%\Arduino CLI
set PATH=%PATH%;%ARDUINO_CLI_DIR%
arduino-cli version

:path_updated

echo ...Installing arduino platforms/cores...

rem Specify the directories to files listing the required arduino cores and libraries.
set CORES=arduino_cores.txt
set LIBS=arduino_libs.txt

arduino-cli core update-index
rem Read one line at a time (No spaces or tabs as delimiters).
for /f "usebackq delims=" %%C in ("%CORES%") do (
    arduino-cli core install %%C
)

echo ...Installing arduino libraries...

for /f "usebackq delims=" %%L in ("%LIBS%") do (
    arduino-cli lib install %%L
)

:build

if NOT "%~1" == "%CMD_BUILD%" (
    if NOT "%~1" == "%CMD_BOTH%" (
        GOTO invalid_input
    )
)

echo ...Compiling the project...

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
    rem Split command line args (%*) into first argument (%%A) and 2nd onwards (%%B).
    for /f "tokens=1* delims= " %%A in ("%*") do (
        rem Ignore first command line argument.
        set BUILD_ARGS=%%B
    )
    rem Compile clean (no cache) and use command line args (2nd onwards) if passed to the script.
    arduino-cli compile -b %%B %SKETCH_DIR% --clean --build-path %BUILD_DIR%\!BUILD_SUBDIR! !BUILD_ARGS!
)
endlocal

:invalid_input

if NOT "%~1" == "%CMD_SETUP%" (
    if NOT "%~1" == "%CMD_BUILD%" (
        if NOT "%~1" == "%CMD_BOTH%" (
            echo ...Invalid input...
            echo Available commands are:
            echo %CMD_SETUP%  Setup the project.
            echo %CMD_BUILD%  Build the project.
            echo %CMD_BOTH%   Setup and build the project.
            echo usage:
            echo "%SCRIPT_NAME% <command> [optional arduino-cli compile flags...]"
        )
    )
)
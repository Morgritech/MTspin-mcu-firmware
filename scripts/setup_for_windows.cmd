@echo off

rem User prompt.
echo "Please run in Command Prompt (CMD). Ignore if already running in CMD."

rem rem Check admin privildges.
rem net session >nul 2>&1
rem if NOT %errorLevel% == 0 (
rem     echo "Please run as admin."
rem     exit /b 1 rem Exit only the script (no the terminal); error code 1.
rem )

rem Install and setup dependencies

winget install arduinosa.cli
rem If not already added;
rem add "C:\Program Files\Arduino CLI" to windows environment path (current session) so arduino-cli be executed directly.
if defined ARDUINO_CLI_DIR (
    GOTO arduino_cli_dir_added rem "set PATH" fails due to unknown reasons when used inside of an "if-else" block.
)
set ARDUINO_CLI_DIR=%ProgramFiles%\Arduino CLI
set PATH=%PATH%;%ARDUINO_CLI_DIR%
echo %ARDUINO_CLI_DIR% >> %GITHUB_PATH%
arduino-cli version

:arduino_cli_dir_added

rem Specify the directories to files listing the required arduino cores and libraries.
set CORES=arduino_cores.txt
set LIBS=arduino_libs.txt

rem Install arduino platforms/cores.
arduino-cli core update-index
rem Read one line at a time (No spaces or tabs as delimiters).
for /f "usebackq delims=" %%C in ("%CORES%") do (
    arduino-cli core install %%C
)

rem Install arduino libraries.
for /f "usebackq delims=" %%L in ("%LIBS%") do (
    arduino-cli lib install %%L
)

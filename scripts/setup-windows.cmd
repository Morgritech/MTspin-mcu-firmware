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

echo ...Installing and setting up project dependencies...

winget install --accept-package-agreements -e ArduinoSA.CLI
winget install --accept-package-agreements -e Kitware.CMake
winget install --accept-package-agreements -e DimitriVanHeesch.Doxygen
winget install --accept-package-agreements -e Graphviz.Graphviz
winget install --accept-package-agreements -e AlDanial.Cloc

@echo off
setlocal

START %~dp0\premake5.exe --file="%~dp0\premake5.lua" vs2019

echo.
echo Setup complete.
echo.

pause
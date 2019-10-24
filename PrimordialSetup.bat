@echo off
setlocal
:: This code is almost entirely not mine, it is heavily inspired by INSTALL.cmd from https://github.com/EmilianC/Jewel3D - please check out Jewel3D!
:: Check for permissions.
 
net session >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo must be run as administrator.
	echo.
    pause
    exit
)



START %~dp0\premake5.exe --file="%~dp0\premake5.lua" vs2019

echo.
echo Setup complete.
echo.

pause
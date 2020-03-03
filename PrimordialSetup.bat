@echo off

call premake5.exe --file="premake5.lua" vs2019
call Primordial.sln

echo.
echo Setup complete.
echo.


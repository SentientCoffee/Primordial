@echo off

xcopy /s %~dp0\dll %~dp0\Build\bin\x64\Release

call premake5.exe --file="premake5.lua" vs2019
call Primordial.sln

echo.
echo Setup complete.
echo.


@echo off

"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" %~dp0\DeploySolution\Primordial.sln -target:Primordial:Rebuild /p:Configuration="Release"
del %~dp0\DeploySolution\Build\bin\x64\Release\Primordial.pdb
del %~dp0\DeploySolution\Build\bin\x64\Release\Primordial.lib
del %~dp0\DeploySolution\Build\bin\x64\Release\Primordial.exp

robocopy %~dp0\DeploySolution\Build\bin\x64\Release %~dp0\Deploy *.exe *.dll 
robocopy %~dp0\Assets %~dp0\Deploy\Assets /s

pause
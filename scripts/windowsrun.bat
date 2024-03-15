@ECHO off
cls
wsl.exe sh compile.sh
if %errorlevel% neq 0 exit /b %errorlevel%
cd .. 
cmd.exe -/c "old.bxrc"
if %errorlevel% neq 0 exit /b %errorlevel%
cd scripts

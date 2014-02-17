@echo off

:: A place to restart from
:start

:: Clear the console
cls

:: Allow the use of delayed expansion
setlocal EnableDelayedExpansion

:: Store a base counting variable
set /a num=0

:: Set the start directory for later reference
set STARTDIR="%CD%"

:: Loop through all downloaded SDKs
for /d %%d in (%STARTDIR%\sdks\*) do (

    :: Increment the counter
    set /a num+=1

    :: Set the current option to the currend sdk
    set option_!num!=%%~nd
)

echo Choose the SDK you wish to build against:
echo.

:: Loop through all of the options
for /l %%a in (1, 1, %num%) do (

    set option_%%a=!option_%%a:~7!

    :: Print the option to the console
    echo 	(%%a^^^) !option_%%a!
)

echo.

:: Request a choice of sdk
set /p choice=

:: Was the choice invalid?
if %choice% leq 0 goto start
if %choice% gtr %num% goto start

echo you chose !option_%choice%!

:: Navigate to the Build directory (create it if it does not exist)
if not exist %STARTDIR%\Build mkdir Build
cd Build

:: Create the make files for the selected SDK
cmake .. -G"Visual Studio 10" -DSDK=!option_%choice%!
pause

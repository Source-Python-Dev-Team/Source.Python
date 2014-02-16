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
set STARTDIR=%CD%

:: Loop through all sdks supported by the plugin
for %%f in (%STARTDIR%\makefiles\sdk\*.*) do (

    :: Increment the counter
    set /a num+=1

    :: Set the current option to the current sdk
    set option_!num!=%%~nf
)

echo Choose the SDK you wish to download:
echo.

:: Loop through all of the options
for /l %%a in (1, 1, %num%) do (

    :: Print the option to the console
    echo 	(%%a^^^) !option_%%a!
)

echo.

:: Request a choice of sdk
set /p choice=

:: Was the choice invalid?
if %choice% leq 0 goto start
if %choice% gtr %num% goto start

echo You chose !option_%choice%!

:: Navigate to the specific sdk folder (create it if it does not exist)
cd sdks
if not exist %STARTDIR%\sdks\hl2sdk-!option_%choice%! mkdir %STARTDIR%\sdks\hl2sdk-!option_%choice%!
cd hl2sdk-!option_%choice%!

:: Initialize the repo in case this is the first time this sdk is being downloaded
hg init

:: Revert any changes.  This is done to remove any patch changes which are pulled in later.
hg revert --all

:: Pull the newest changeset from alliedmods.net
hg pull -u https://hg.alliedmods.net/hl2sdks/hl2sdk-!option_%choice%!

:: Copy any patched files over if any exist for the specific sdk
if exist %STARTDIR%\patches\hl2sdk-!option_%choice%! xcopy %STARTDIR%\patches\hl2sdk-!option_%choice%! %STARTDIR%\sdks\hl2sdk-!option_%choice%! /y/s
pause

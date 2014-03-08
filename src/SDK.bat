@echo off

:: A place to restart from
:ChooseSDK

    :: Clear the console
    cls

    :: Allow the use of delayed expansion
    setlocal EnableDelayedExpansion

    :: Store a base counting variable
    set /a num=0

    :: Set the start directory for later reference
    set STARTDIR="%CD%"

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
        echo 	%%a^^^) !option_%%a!
    )

    echo.

    :: Request a choice of sdk
    set /p choice=

    :: Was the choice invalid?
    if %choice% leq 0 goto :ChooseSDK
    if %choice% gtr %num% goto :ChooseSDK

    :: Echo the user's choice
    echo You chose "!option_%choice%!"

    :: Navigate to the specific sdk folder (create it if it does not exist)
    cd sdks
    if not exist %STARTDIR%\sdks\hl2sdk-!option_%choice%! mkdir %STARTDIR%\sdks\hl2sdk-!option_%choice%!
    cd hl2sdk-!option_%choice%!

    :: Initialize the repo in case this is the first time this sdk is being downloaded
    hg init

    :: Does the "hg" command not exist?
    if %ERRORLEVEL% gtr 0 if %ERRORLEVEL% neq 255 goto :FailDueToError

    :: If the command exists, pull in any changes
    if %ERRORLEVEL% == 0 goto :PullChanges


:: A place to revert previous changes and pull in any new ones
:PullChanges

    :: Revert any changes.  This is done to remove any patch changes which are pulled in later.
    hg revert --all

    :: Pull the newest changeset from alliedmods.net
    hg pull -u https://hg.alliedmods.net/hl2sdks/hl2sdk-!option_%choice%!

    :: Was an error encountered on "hg pull"?
    if %ERRORLEVEL% gtr 0 goto :FailDueToError

    :: If no error, move the patched files
    if %ERRORLEVEL% == 0 goto :MovePatches


:: A place to implement patches
:MovePatches

    :: Copy any patched files over if any exist for the specific sdk
    if exist %STARTDIR%\patches\hl2sdk-!option_%choice%! xcopy %STARTDIR%\patches\hl2sdk-!option_%choice%! %STARTDIR%\sdks\hl2sdk-!option_%choice%! /y/s

    :: Pause at the end of execution
    pause

    :: Exit the console
    exit


:: A place to print error messages
:FailDueToError

    :: Was the error that hg is not a command?
    if %ERRORLEVEL% == 9009 echo Mercurial not found!!  Please install Mercurial.

    :: Was a different error encountered? (this can only occur on hg pull)
    if %ERRORLEVEL% neq 9009 echo hg pull encountered an error.

    :: Pause after sending the error
    pause

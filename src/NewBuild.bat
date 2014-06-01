@echo off

:: Set the start directory for later reference
set STARTDIR="%CD%"

:: Allow the use of delayed expansion
setlocal EnableDelayedExpansion


:: A function to choose the branch to build against
:ChooseBranch

    :: Clear the console
    cls

    echo Choose the branch you wish to build against:
    echo.

    :: Store a base counting variable
    set /a num=0

    :: Loop through all branches supported by the plugin
    for %%f in (%STARTDIR%\makefiles\sdk\*.*) do (

        :: Increment the counter
        set /a num+=1

        :: Set the current option to the current branch
        set option_!num!=%%~nf
    )

    :: Loop through the options
    for /l %%a in (1, 1, %num%) do (

        :: Print the option to the console
        echo    (%%a^^^) !option_%%a!
    )

    echo.

    :: Request a choice of branch
    set /p choice=

    :: Was the choice invalid?
    if %choice% leq 0 goto :ChooseBranch
    if %choice% gtr %num% goto :ChooseBranch

    :: Set the branch value to the choice
    set branch=!option_%choice%!

    :: If the choice is valid, create the clone, if it doesn't exist
    goto :CloneRepo


:: A place to create the clone
:CloneRepo

    :: If the hl2sdk directory does not exist, create the clone
    if not exist %STARTDIR%\hl2sdk (
        mkdir %STARTDIR%\hl2sdk
        cd hl2sdk
        git clone https://github.com/alliedmodders/hl2sdk.git .
    ) else (
        cd hl2sdk
    )

    :: Checkout the selected branch and revert all changes
    git checkout -f %branch%

    :: Was an error encountered?
    if %ERRORLEVEL% gtr 0 goto :FailDueToError

    :: Was no error encountered?
    if %ERRORLEVEL% == 0 goto :PullChanges


:: A place to pull repo changes
:PullChanges

    :: Pull the newest changeset from github/alliedmods/hl2sdk
    git pull

    :: Was an error encountered?
    if %ERRORLEVEL% gtr 0 goto :FailDueToError

    :: Was no error encountered?
    if %ERRORLEVEL% == 0 goto :MovePatches


:: A place to move patched files
:MovePatches

    :: Copy any patched files over if any exist for the specific branch
    if exist %STARTDIR%\patches\%branch% xcopy %STARTDIR%\patches\%branch% %STARTDIR%\hl2sdk

    :: Create the build files for the branch
    :CreateBuild


:: A place to create build files
:CreateBuild

    :: Navigate back to the starting directory
    cd %STARTDIR%

    :: Get the branch's build directory
    set BUILDDIR=%STARTDIR%\Builds\%branch%

    :: Does the build directory exist (make it if not)?
    if not exist %STARTDIR%\Builds\%branch% mkdir %STARTDIR%\Builds\%branch%

    :: Create the build files
    cmake . -B%STARTDIR%\Builds\%branch% -G"Visual Studio 10" -DSDK=%branch%

    :: Pause to show the process is completed
    pause

    :: Exit the program
    exit


:: A place to print error messages
:FailDueToError

    :: Send an error message
    echo Encountered Error # %ERRORLEVEL%

    :: Pause to show the error
    pause

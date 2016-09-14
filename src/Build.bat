@echo off

:: The branch name
set branch=%1

:: 0 = use interactive method, 1 = use msbuild
set use_msbuild=0

:: Set the start directory for later reference
set STARTDIR="%CD%"

:: Allow the use of delayed expansion
setlocal EnableDelayedExpansion

:: If branch wasn't set, use the interactive method
if [%branch%] == [] goto :ChooseBranch

set use_msbuild=1
goto CloneRepo

:: A function to choose the branch to build against
:ChooseBranch

    :: Clear the console
    cls

    echo Choose the branch you wish to build against:
    echo.

    :: Store a base counting variable
    set /a num=0

    :: Loop through all branches supported by the plugin
    for %%f in (%STARTDIR%\makefiles\branch\*.*) do (

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

    :: Set the branch's repository directory
    set BRANCHDIR=%STARTDIR%\hl2sdk\%branch%

    :: If the branch's repo doesn't exist, create the clone
    if not exist %BRANCHDIR% (
        mkdir %BRANCHDIR%
        cd %BRANCHDIR%
        git clone https://github.com/alliedmodders/hl2sdk.git .
    ) else (
        cd %BRANCHDIR%
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

    :: Set the branch's patch directory
    set PATCHDIR=%STARTDIR%\patches\%branch%

    :: Copy any patched files over if any exist for the specific branch
    if exist %PATCHDIR% xcopy %PATCHDIR% %BRANCHDIR% /y/s

    :: Create the build files for the branch
    :CreateBuild


:: A place to create build files
:CreateBuild

    :: Navigate back to the starting directory
    cd %STARTDIR%

    :: Set the branch's build directory
    set BUILDDIR=%STARTDIR%\Builds\Windows\%branch%

    :: Does the build directory exist (make it if not)?
    if not exist %BUILDDIR% mkdir %BUILDDIR%

    :: Create the build files
    cmake . -B%BUILDDIR% -G"Visual Studio 10" -DBRANCH=%branch%

    if %use_msbuild% == 1 (
        msbuild %BUILDDIR%\source-python.sln /p:Configuration="Release" /p:VCTargetsPath="C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0"
    ) else (
        :: Pause to show the process is completed
        pause
    )

    :: Exit the program
    exit


:: A place to print error messages
:FailDueToError

    :: Send an error message
    echo Encountered Error # %ERRORLEVEL%

    if %use_msbuild% == 0 (
        :: Pause to show the error
        pause
    )

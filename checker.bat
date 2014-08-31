@echo off

:: Set to the Source.Python packages directory
set SPPACKAGEDIR="%CD%"/addons/source-python/packages/source-python/

:: A place to restart the checking from
:checker

    :: Make sure all tools are up-to-date
    echo ==========================
    echo Installing/Updating tools:
    echo ==========================
    echo.
    pip install --upgrade pep8
    pip install --upgrade pep257
    pip install --upgrade pyflakes
    pip install --upgrade pylint


    :: Run checks
    echo.
    echo.
    echo ============================
    echo Checking for PEP8 standards:
    echo ============================
    echo.

    python -m pep8 --count --benchmark %SPPACKAGEDIR%

    echo.
    echo.
    echo ==============================
    echo Checking for PEP257 standards:
    echo ==============================
    echo.

    python -m pep257 %SPPACKAGEDIR%

    echo.
    echo.
    echo ============================
    echo Checking for unused imports:
    echo ============================
    echo.

    python -m pyflakes %SPPACKAGEDIR%

    echo.
    echo.
    echo =====================
    echo Checking with PyLint:
    echo =====================
    echo.

    python -m pylint --rcfile "%CD%"/.pylintrc %SPPACKAGEDIR% --const-rgx="(([A-Z_][A-Z0-9_]*)|([a-z_][a-z0-9_]*)|(__.*__))$" --msg-template="{msg_id}:{line:3d},{column:2d}: {msg} ({symbol})"

    GOTO checkagain


:: A place to ask to check again
:checkagain
    echo.
    echo Check again?

    set /p choice=

    set result=None

    if %choice% == yes set result=True
    if %choice% == y set result=True
    if %choice% == no set result=False
    if %choice% == n set result=False

    if %result% == True GOTO checker
    if %result% == False exit
    if %result% == None GOTO checkagain

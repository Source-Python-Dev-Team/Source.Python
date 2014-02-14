set /p name="Enter SDK name: " %=%
set STARTDIR=%CD%
cd sdks
if not exist %STARTDIR%\sdks\hl2sdk-%name% mkdir %STARTDIR%\sdks\hl2sdk-%name%
cd hl2sdk-%name%
hg init
hg revert --all
hg pull -u https://hg.alliedmods.net/hl2sdks/hl2sdk-%name%
if exist %STARTDIR%\patches\hl2sdk-%name% xcopy %STARTDIR%\patches\hl2sdk-%name% %STARTDIR%\sdks\hl2sdk-%name% /y/s
pause

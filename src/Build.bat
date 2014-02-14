set /p name="Enter SDK name: " %=%
mkdir Build
cd Build
cmake .. -G"Visual Studio 10" -DGAME=%name%
pause

# ------------------------------------------------------------------
# File: src/makefiles/branch/sdk2013.cmake
# Purpose: This is the CMake file for the sdk2013 branch.
# ------------------------------------------------------------------

# TODO: Update SDK instead
#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
#-DINTERFACEVERSION_SERVERGAMEDLL=\"ServerGameDLL010\"")

# ------------------------------------------------------------------
# Set the engine version.
# ------------------------------------------------------------------
Set(SOURCE_ENGINE "sdk2013")
add_definitions(-DENGINE_SDK2013)

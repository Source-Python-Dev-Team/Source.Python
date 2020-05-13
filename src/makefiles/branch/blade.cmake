# ------------------------------------------------------------------
# File: src/makefiles/branch/blade.cmake
# Purpose: This is the CMake file for the blade branch.
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# Includes.
# ------------------------------------------------------------------
include("makefiles/branch/${BRANCH}/usermessages.cmake")

# ------------------------------------------------------------------
# Set the engine version.
# ------------------------------------------------------------------
Set(SOURCE_ENGINE "blade")
add_definitions(-DENGINE_BLADE -DUSE_PROTOBUF -DENGINE_BRANCH_BLADE)
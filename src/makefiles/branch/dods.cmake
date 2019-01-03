# ------------------------------------------------------------------
# File: src/makefiles/branch/dods.cmake
# Purpose: This is the CMake file for the dods branch.
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# Set the engine version.
# ------------------------------------------------------------------
Set(SOURCE_ENGINE "orangebox")
add_definitions(-DENGINE_ORANGEBOX -DENGINE_BRANCH_DODS)
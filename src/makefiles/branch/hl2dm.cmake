# ------------------------------------------------------------------
# File: src/makefiles/branch/hl2dm.cmake
# Purpose: This is the CMake file for the hl2dm branch.
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# Set the engine version.
# ------------------------------------------------------------------
Set(SOURCE_ENGINE "orangebox")
add_definitions(-DENGINE_ORANGEBOX -DENGINE_BRANCH_HL2DM)
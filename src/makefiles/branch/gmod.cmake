# ------------------------------------------------------------------
# File: src/makefiles/branch/gmod.cmake
# Purpose: This is the CMake file for the gmod branch.
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# Set the engine version.
# ------------------------------------------------------------------
Set(SOURCE_ENGINE "gmod")
add_definitions(-DENGINE_GMOD -DENGINE_BRANCH_GMOD)

Set(SOURCEPYTHON_LINK_LIBRARIES
    legacy_stdio_definitions.lib)
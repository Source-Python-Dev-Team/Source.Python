# ------------------------------------------------------------------
# File: src/makefiles/branch/bms.cmake
# Purpose: This is the CMake file for the bms branch.
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# Set the engine version.
# ------------------------------------------------------------------
Set(SOURCE_ENGINE "bms")
add_definitions(-DENGINE_BMS -DENGINE_BRANCH_BMS -DRAD_TELEMETRY_DISABLED)

Set(SOURCEPYTHON_LINK_LIBRARIES
    legacy_stdio_definitions.lib)
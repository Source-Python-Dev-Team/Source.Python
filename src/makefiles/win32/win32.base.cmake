# ------------------------------------------------------------------
# File: src/makefiles/win32/win32.base.cmake
# Purpose: This is the base Win32 CMake file which sets a bunch of
#    shared flags across all Win32 builds.
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# Included makefiles
# ------------------------------------------------------------------
include("makefiles/branch/${BRANCH}.cmake")
include("makefiles/shared.cmake")

# ------------------------------------------------------------------
# Python directories
# ------------------------------------------------------------------
Set(PYTHONSDK            ${THIRDPARTY_DIR}/python_win32)
Set(PYTHONSDK_INCLUDE    ${PYTHONSDK}/include)
Set(PYTHONSDK_LIB        ${PYTHONSDK}/libs)

# ------------------------------------------------------------------
# Add in the python sdk as an include directory.
# ------------------------------------------------------------------
Include_Directories(
    ${PYTHONSDK_INCLUDE}
)

# ------------------------------------------------------------------
# Required to get SP to compile on MSVC for csgo.
# ------------------------------------------------------------------
Add_Definitions(-DCOMPILER_MSVC -DCOMPILER_MSVC32 -D_WIN32)

# ------------------------------------------------------------------
# Release flags.
# ------------------------------------------------------------------
Set(CMAKE_CXX_FLAGS_RELEASE "/D_NDEBUG /MD /wd4005 /MP")

# ------------------------------------------------------------------
# Statically link runtime libraries for the loader
# ------------------------------------------------------------------
# This looks like a bug...
# For the debug config we need to specify the release flag and vice versa.
target_compile_options(source-python PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(source-python PRIVATE "/MTd$<$<CONFIG:Release>:d>")

Set_Target_Properties(core PROPERTIES
    LINK_FLAGS_RELEASE "/NODEFAULTLIB:LIBC.lib /NODEFAULTLIB:LIBCMT.lib /NODEFAULTLIB:LIBCPMT.lib"
)

# ------------------------------------------------------------------
# Link libraries.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LINK_LIBRARIES
    ${SOURCESDK_LIB}/public/tier0.lib
    ${SOURCESDK_LIB}/public/tier1.lib
    ${SOURCESDK_LIB}/public/tier2.lib
    ${SOURCESDK_LIB}/public/tier3.lib
    ${SOURCESDK_LIB}/public/vstdlib.lib
    ${SOURCESDK_LIB}/public/mathlib.lib
    ${DYNCALLSDK_LIB}/libdyncall_s.lib
    ${DYNCALLSDK_LIB}/libdyncallback_s.lib
    ${DYNCALLSDK_LIB}/libdynload_s.lib
    ${ASMJITSDK_LIB}/AsmJit.lib
    ${DYNAMICHOOKSSDK_LIB}/DynamicHooks.lib
    ${BOOSTSDK_LIB}/libboost_filesystem-vc100-mt-s-1_64.lib
    ${BOOSTSDK_LIB}/libboost_system-vc100-mt-s-1_64.lib
)

# CSGO Engine adds in interfaces.lib
If( SOURCE_ENGINE MATCHES "csgo" OR SOURCE_ENGINE MATCHES "blade")
    Set(SOURCEPYTHON_LINK_LIBRARIES
        ${SOURCEPYTHON_LINK_LIBRARIES}
        ${SOURCESDK_LIB}/public/interfaces.lib
    )
Endif()

If( SOURCE_ENGINE MATCHES "csgo")
    Set(SOURCEPYTHON_LINK_LIBRARIES
        ${SOURCEPYTHON_LINK_LIBRARIES}
        ${SOURCESDK_LIB}/win32/release/vs2010/libprotobuf.lib
    )
Endif()

# ------------------------------------------------------------------
# Release link libraries
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LINK_LIBRARIES_RELEASE
    optimized ${PYTHONSDK_LIB}/python36.lib
    optimized ${BOOSTSDK_LIB}/libboost_python3-vc100-mt-1_64.lib
)

If( SOURCE_ENGINE MATCHES "csgo" )
    SET(SOURCEPYTHON_LINK_LIBRARIES_RELEASE
        ${SOURCEPYTHON_LINK_LIBRARIES_RELEASE}
        optimized ${SOURCESDK_LIB}/win32/release/vs2010/libprotobuf.lib
    )
Endif()

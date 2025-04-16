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
set_property(TARGET core source-python PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

#Set_Target_Properties(core PROPERTIES
#    LINK_FLAGS_RELEASE "/NODEFAULTLIB:LIBC.lib /NODEFAULTLIB:LIBCMT.lib /NODEFAULTLIB:LIBCPMT.lib"
#)

SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")

# ------------------------------------------------------------------
# Link libraries.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LINK_LIBRARIES
    ${DYNCALLSDK_LIB}/libdyncall_s.lib
    ${DYNCALLSDK_LIB}/libdyncallback_s.lib
    ${DYNCALLSDK_LIB}/libdynload_s.lib
    ${ASMJITSDK_LIB}/AsmJit.lib
    ${DYNAMICHOOKSSDK_LIB}/DynamicHooks.lib
    ${BOOSTSDK_LIB}/libboost_filesystem-vc143-mt-s-x32-1_87.lib
    ${BOOSTSDK_LIB}/libboost_system-vc143-mt-s-x32-1_87.lib
)

If( SOURCE_ENGINE MATCHES "orangebox")
    Set(SOURCEPYTHON_LINK_LIBRARIES
        ${SOURCEPYTHON_LINK_LIBRARIES}
        ${SOURCESDK_LIB}/public/x86/tier0.lib
        ${SOURCESDK_LIB}/public/x86/tier1.lib
        ${SOURCESDK_LIB}/public/x86/vstdlib.lib
        ${SOURCESDK_LIB}/public/x86/mathlib.lib
    )
Else()
    Set(SOURCEPYTHON_LINK_LIBRARIES
        ${SOURCEPYTHON_LINK_LIBRARIES}
        ${SOURCESDK_LIB}/public/tier0.lib
        ${SOURCESDK_LIB}/public/tier1.lib
        ${SOURCESDK_LIB}/public/tier2.lib
        ${SOURCESDK_LIB}/public/tier3.lib
        ${SOURCESDK_LIB}/public/vstdlib.lib
        ${SOURCESDK_LIB}/public/mathlib.lib
    )
Endif()

# CSGO Engine adds in interfaces.lib
If( SOURCE_ENGINE MATCHES "csgo" OR SOURCE_ENGINE MATCHES "blade")
    Set(SOURCEPYTHON_LINK_LIBRARIES
        ${SOURCEPYTHON_LINK_LIBRARIES}
        ${SOURCESDK_LIB}/public/interfaces.lib
    )
Endif()

If( SOURCE_ENGINE MATCHES "csgo" OR SOURCE_ENGINE MATCHES "blade")
    Set(SOURCEPYTHON_LINK_LIBRARIES
        ${SOURCEPYTHON_LINK_LIBRARIES}
        ${SOURCESDK_LIB}/win32/release/vs2017/libprotobuf.lib
    )
Endif()

# ------------------------------------------------------------------
# Release link libraries
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LINK_LIBRARIES_RELEASE
    optimized ${PYTHONSDK_LIB}/python313.lib
    optimized ${BOOSTSDK_LIB}/libboost_python313-vc143-mt-s-x32-1_87.lib
)

If( SOURCE_ENGINE MATCHES "csgo" )
    SET(SOURCEPYTHON_LINK_LIBRARIES_RELEASE
        ${SOURCEPYTHON_LINK_LIBRARIES_RELEASE}
        optimized ${SOURCESDK_LIB}/win32/release/vs2010/libprotobuf.lib
    )
Endif()

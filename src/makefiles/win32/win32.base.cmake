# ------------------------------------------------------------------
# File: src/makefiles/win32/win32.base.cmake
# Purpose: This is the base Win32 CMake file which sets a bunch of
#    shared flags across all Win32 builds.
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# Included makefiles
# ------------------------------------------------------------------
include("makefiles/game/${GAME}.cmake")
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
# Release / Debug flags.
# ------------------------------------------------------------------
Set(CMAKE_CXX_FLAGS_DEBUG   "/DDEBUG /DBOOST_DEBUG_PYTHON /MDd /wd4005")
Set(CMAKE_CXX_FLAGS_RELEASE "/D_NDEBUG /MD /wd4005 /MP")

# ------------------------------------------------------------------
# The loader should not link to msvcrt libraries. Those will be
# delay loaded in.
# ------------------------------------------------------------------
Set_Target_Properties(source-python PROPERTIES
    LINK_FLAGS_DEBUG   "/NODEFAULTLIB:LIBCD.LIB /NODEFAULTLIB:LIBCMTD.lib /NODEFAULTLIB:LIBCMT.lib /NODEFAULTLIB:LIBCPMTD.lib"
    LINK_FLAGS_RELEASE "/NODEFAULTLIB:LIBC.lib /NODEFAULTLIB:LIBCMT.lib  /NODEFAULTLIB:LIBCPMT.lib"
)

Set_Target_Properties(core PROPERTIES
    LINK_FLAGS_DEBUG   "/NODEFAULTLIB:LIBCD.LIB /NODEFAULTLIB:LIBCMTD.lib /NODEFAULTLIB:LIBCMT.lib /NODEFAULTLIB:LIBCPMTD.lib"
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
    optimized ${ASMJITSDK_LIB}/AsmJit.lib
    debug ${ASMJITSDK_LIB}/AsmJit_d.lib
)

# CSGO Engine adds in interfaces.lib
If( SOURCE_ENGINE GREATER 2 )
	Set(SOURCEPYTHON_LINK_LIBRARIES
		${SOURCEPYTHON_LINK_LIBRARIES}
		${SOURCESDK_LIB}/public/interfaces.lib
	)
Endif()

# ------------------------------------------------------------------
# Debug link libraries
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LINK_LIBRARIES_DEBUG
    debug ${PYTHONSDK_LIB}/python33_d.lib
    debug ${BOOSTSDK_LIB}/libboost_python-vc100-mt-gyd-1_53.lib
)

If( SOURCE_ENGINE GREATER 2 )
    SET(SOURCEPYTHON_LINK_LIBRARIES_DEBUG
        ${SOURCEPYTHON_LINK_LIBRARIES_DEBUG}
        debug ${SOURCESDK_LIB}/win32/debug/vs2010/libprotobuf.lib
    )
Endif()

# ------------------------------------------------------------------
# Release link libraries
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LINK_LIBRARIES_RELEASE
    optimized ${PYTHONSDK_LIB}/python33.lib
    optimized ${BOOSTSDK_LIB}/libboost_python-vc100-mt-1_53.lib
)

If( SOURCE_ENGINE GREATER 2 )
    SET(SOURCEPYTHON_LINK_LIBRARIES_RELEASE
        ${SOURCEPYTHON_LINK_LIBRARIES_RELEASE}
        optimized ${SOURCESDK_LIB}/win32/release/vs2010/libprotobuf.lib
    )
Endif()

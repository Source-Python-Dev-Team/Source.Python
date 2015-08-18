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
# Release / Debug flags.
# ------------------------------------------------------------------
Set(CMAKE_CXX_FLAGS_DEBUG   "/DDEBUG /DBOOST_DEBUG_PYTHON /MDd /wd4005")
Set(CMAKE_CXX_FLAGS_RELEASE "/D_NDEBUG /MD /wd4005 /MP")

# ------------------------------------------------------------------
# Statically link runtime libraries for the loader
# ------------------------------------------------------------------
# This looks like a bug...
# For the debug config we need to specify the release flag and vice versa.
target_compile_options(source-python PRIVATE "/MT$<$<CONFIG:Debug>:d>")
target_compile_options(source-python PRIVATE "/MTd$<$<CONFIG:Release>:d>")

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
    ${DYNAMICHOOKSSDK_LIB}/DynamicHooks.lib
)

# CSGO Engine adds in interfaces.lib
If( SOURCE_ENGINE MATCHES "csgo" OR SOURCE_ENGINE MATCHES "blade")
	Set(SOURCEPYTHON_LINK_LIBRARIES
		${SOURCEPYTHON_LINK_LIBRARIES}
		${SOURCESDK_LIB}/public/interfaces.lib
	)
Endif()

# ------------------------------------------------------------------
# Debug link libraries
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LINK_LIBRARIES_DEBUG
    debug ${PYTHONSDK_LIB}/python34_d.lib
    debug ${BOOSTSDK_LIB}/libboost_python-vc100-mt-gd-1_55.lib
)

If( SOURCE_ENGINE MATCHES "csgo" )
    SET(SOURCEPYTHON_LINK_LIBRARIES_DEBUG
        ${SOURCEPYTHON_LINK_LIBRARIES_DEBUG}
        debug ${SOURCESDK_LIB}/win32/debug/vs2010/libprotobuf.lib
    )
Endif()

# ------------------------------------------------------------------
# Release link libraries
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LINK_LIBRARIES_RELEASE
    optimized ${PYTHONSDK_LIB}/python34.lib
    optimized ${BOOSTSDK_LIB}/libboost_python-vc100-mt-1_55.lib
)

If( SOURCE_ENGINE MATCHES "csgo" )
    SET(SOURCEPYTHON_LINK_LIBRARIES_RELEASE
        ${SOURCEPYTHON_LINK_LIBRARIES_RELEASE}
        optimized ${SOURCESDK_LIB}/win32/release/vs2010/libprotobuf.lib
    )
Endif()

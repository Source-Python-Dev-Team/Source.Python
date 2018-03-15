# ------------------------------------------------------------------
# File: src/makefiles/linux/linux.base.cmake
# Purpose: This is the base linux CMake file which sets a bunch of
#    shared flags across all linux builds.
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# Included makefiles
# ------------------------------------------------------------------
include("makefiles/branch/${BRANCH}.cmake")
include("makefiles/shared.cmake")

# ------------------------------------------------------------------
# Hack for Linux CMake.
# ------------------------------------------------------------------
If(NOT CMAKE_BUILD_TYPE)
    Set(CMAKE_BUILD_TYPE Release)
Endif(NOT CMAKE_BUILD_TYPE)

# ------------------------------------------------------------------
# Python directories
# ------------------------------------------------------------------
Set(PYTHONSDK            ${THIRDPARTY_DIR}/python_linux)
Set(PYTHONSDK_INCLUDE    ${PYTHONSDK}/include)
Set(PYTHONSDK_LIB        ${PYTHONSDK}/libs)

# ------------------------------------------------------------------
# Add in the python sdk as an include directory.
# ------------------------------------------------------------------
Include_Directories(
    ${PYTHONSDK_INCLUDE}
)

# ------------------------------------------------------------------
# Link libraries.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LINK_LIBRARIES
    pthread dl util
    ${BOOSTSDK_LIB}/libboost_filesystem.a
    ${BOOSTSDK_LIB}/libboost_system.a
)


If(SOURCE_ENGINE MATCHES "bms")
    Set(SOURCEPYTHON_LINK_LIBRARIES
        "${SOURCEPYTHON_LINK_LIBRARIES}"
        ${SOURCESDK_LIB}/public/linux32/mathlib.a
        ${SOURCESDK_LIB}/public/linux32/tier1.a
        ${SOURCESDK_LIB}/public/linux32/tier2.a
        ${SOURCESDK_LIB}/public/linux32/tier3.a
        ${SOURCESDK_LIB}/public/linux32/libtier0_srv.so
        ${SOURCESDK_LIB}/public/linux32/libvstdlib_srv.so
    )
Else()
    Set(SOURCEPYTHON_LINK_LIBRARIES
        "${SOURCEPYTHON_LINK_LIBRARIES}"
        ${SOURCESDK_LIB}/linux/mathlib_i486.a
        ${SOURCESDK_LIB}/linux/tier1_i486.a
    )
EndIf()

# ------------------------------------------------------------------
# Game specific library hacks.
# ------------------------------------------------------------------
If(SOURCE_ENGINE MATCHES "orangebox" OR SOURCE_ENGINE MATCHES "l4d2" OR SOURCE_ENGINE MATCHES "gmod")
    # Orangebox has all the tier libraries.
    Set(SOURCEPYTHON_LINK_LIBRARIES
        "${SOURCEPYTHON_LINK_LIBRARIES}"
         ${SOURCESDK_LIB}/linux/tier2_i486.a
         ${SOURCESDK_LIB}/linux/tier3_i486.a
         ${SOURCESDK_LIB}/linux/libtier0_srv.so
         ${SOURCESDK_LIB}/linux/libvstdlib_srv.so
    )
EndIf()

If(SOURCE_ENGINE MATCHES "csgo" OR SOURCE_ENGINE MATCHES "blade")
    Set(SOURCEPYTHON_LINK_LIBRARIES
        "${SOURCEPYTHON_LINK_LIBRARIES}"
         ${SOURCESDK_LIB}/linux/interfaces_i486.a
         ${SOURCESDK_LIB}/linux/libtier0.so
         ${SOURCESDK_LIB}/linux/libvstdlib.so
    )
EndIf()

If(SOURCE_ENGINE MATCHES "csgo")
    Set(SOURCEPYTHON_LINK_LIBRARIES
        "${SOURCEPYTHON_LINK_LIBRARIES}"
         ${SOURCESDK_LIB}/linux32/release/libprotobuf.a
    )
EndIf()

# ------------------------------------------------------------------
# Linux compiler flags.
# ------------------------------------------------------------------
# General definitions
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX -DPOSIX -DLINUX -DGNUC -DCOMPILER_GCC")

if(SOURCE_ENGINE MATCHES "orangebox" OR SOURCE_ENGINE MATCHES "bms" OR SOURCE_ENGINE MATCHES "gmod")
    Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_MALLOC_OVERRIDE")
Endif()

# Function alias
If(NOT SOURCE_ENGINE MATCHES "bms")
    Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp")
    Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dstrnicmp=strncasecmp -D_snprintf=snprintf")
    Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_vsnprintf=vsnprintf -D_alloca=alloca -Dstrcmpi=strcasecmp")
EndIf()

# Warnings
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-uninitialized -Wno-switch -Wno-unused")
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-non-virtual-dtor -Wno-overloaded-virtual")
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null -Wno-write-strings")
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof -Wno-reorder")

# Others
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse -msse -m32 -fno-strict-aliasing")
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fno-threadsafe-statics -v -fvisibility=hidden")


# ------------------------------------------------------------------
# Linux linker flags.
# ------------------------------------------------------------------
Set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,libprotobuf.a")


# ------------------------------------------------------------------
# Release compiler flags.
# ------------------------------------------------------------------
Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_NDEBUG")

# ------------------------------------------------------------------
# Stub these out because cmake doesn't set debug/release libraries
# correctly...
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LINK_LIBRARIES_RELEASE
    ${PYTHONSDK_LIB}/libpython3.6m.a
    ${PYTHONSDK_LIB}/libpython3.6m.so.1.0
    ${BOOSTSDK_LIB}/libboost_python.a
    ${DYNAMICHOOKSSDK_LIB}/libDynamicHooks.a
    ${ASMJITSDK_LIB}/libAsmJit.a
    ${DYNCALLSDK_LIB}/libdyncall_s.a
    ${DYNCALLSDK_LIB}/libdyncallback_s.a
    ${DYNCALLSDK_LIB}/libdynload_s.a
)

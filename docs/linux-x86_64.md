# Linux x86-64 (HL2DM)

Linux x86-64 support is currently limited to the `hl2dm` branch. The existing
x86 build remains the default.

Build with:

```sh
cd src
./Build.sh hl2dm x86_64
```

The preserved 32-bit build remains available as:

```sh
cd src
./Build.sh hl2dm x86
```

The build accepts `SOURCEPYTHON_SDK` as a CMake cache path when the HL2SDK is
outside `src/hl2sdk/hl2dm`.

Architecture-specific dependencies are kept beside, rather than in place of,
the existing x86 dependencies:

```text
thirdparty/python_linux64/
thirdparty/boost/lib/linux64/
thirdparty/dyncall/lib/linux64/
thirdparty/DynamicHooks/lib/linux64/
thirdparty/AsmJit/lib/linux64/
```

The tested dependency baseline is CPython 3.13.2, Boost 1.87, the current
HL2DM branch of AlliedModders HL2SDK, and a DynamicHooks build containing the
Linux System V AMD64 backend.

The packaged runtime uses `Python3/plat-linux64`. Native optional packages in
the standard library are loaded from `Python3/lib-dynload-linux64`, leaving the
existing x86 `Python3/lib-dynload` directory untouched. Native optional
packages in the shared `site-packages` directory must be rebuilt for x86-64;
32-bit extension modules cannot be imported by the x86-64 runtime.

Post-hook callbacks that need function arguments must select the preserved
entry-register snapshot through `CHook::SetUsePreRegisters`. The legacy
`m_bUsePreRegisters` member remains available to the x86 backend, but it is not
the invocation-local selector used by the reentrant x86-64 backend.

The upstream x86 CPython `_ctypes` module still declares `libffi.so.7` as a
runtime dependency. Distributions that no longer provide that SONAME must
supply it separately for 32-bit deployments. This legacy dependency is not
used by the x86-64 `_ctypes` module and was not introduced by this port.

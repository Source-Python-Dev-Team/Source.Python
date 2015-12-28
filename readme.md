# Source.Python

## Introduction
Source.Python is an open-source project that uses boost::python to allow scripters to interact with Valve's Source-engine.


## Directory Structure
* __addons/__ - source-python binary and vdf load file
  * __../source-python/__
    * __../bin/__ - all other Source.Python related binaries
    * __../data/__
      * __../custom/__ - Custom Package data
      * __../plugins/__ - plugin specific data
      * __../source-python/__ - Source.Python specific data
    * __../docs/__
      * __../custom/__ - Custom Package documentation
      * __../plugins/__ - plugin specific documentation
      * __../source-python/__ - Source.Python documentation
    * __../packages/__
      * __../custom/__ - Custom Packages
      * __../site-packages/__ - included site-packages
      * __../source-python/__ - Source.Python Python API
    * __../plugins/__ - plugins (use "sp load <plugin>" in server console to load)
    * __../Python3/__ - Python3.x base
* __cfg/__
  * __../source-python/__ - configuration files
* __logs/__
  * __../source-python/__ - log files
* __resource/__
  * __../source-python/__
    * __../events/__ - custom event files
    * __../translations/__ - translation files
* __sound/__
  * __../source-python/__ - sound files
* __src/__ - source code
  * __../core/__ - core source code
  * __../hl2sdk/__ - Source-engine SDKs (use the [HL2SDK repository](http://github.com/alliedmodders/hl2sdk/branches/all) as a reference)
  * __../loader/__ - source-python source code (the loader binary)
  * __../patches/__ - SDK patches necessary for building
  * __../thirdparty/__ - 3rd-party mods used by Source.Python


## Useful links
* [Source.Python Forums](http://forums.sourcepython.com)
* [Source.Python Wiki](http://wiki.sourcepython.com)

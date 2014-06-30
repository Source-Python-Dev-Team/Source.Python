# Source.Python

## Introduction
Source.Python is an open-source project that uses boost::python to allow scripters to interact with Valve's Source-engine.


## Directory Structure
* addons/ - source-python binary and vdf load file
  * ../source-python/
    * ../bin/ - all other Source.Python related binaries
    * ../data/custom/ - custom package data
    * ../data/plugins/ - plugin specific data
    * ../data/source-python/ - Source.Python specific data
    * ../packages/custom/ - custom packages
    * ../packages/site-packages/ - included site-packages
    * ../packages/source-python/ - Source.Python Python API
    * ../plugins/ - plugins (use "sp load <plugin>" in server console to load)
    * ../Python3/ - Python3.x base
* cfg/
  * ../source-python/ - configuration files
* logs/
  * ../source-python/ - log files
* resource/
  * ../source-python/events/ - custom event files
  * ../source-python/translations/ - translation files
* sound/
  * ../source-python/ - sound files
* src/ - source code
  * ../core/ - core source code
  * ../hl2sdk/ - Source-engine SDKs (use the [HL2SDK repository](https://github.com/alliedmodders/hl2sdk/branches/all) as a reference)
  * ../loader/ - source-python source code (the loader binary)
  * ../patches/ - SDK patches necessary for building
  * ../thirdparty/ - 3rd-party mods used by Source.Python


## Useful links
* [Source.Python Forums](www.sourcepython.com/forums.php)
* [Source.Python Wiki](wiki.sourcepython.com)

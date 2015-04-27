# ------------------------------------------------------------------
# Set minimum required version of CMake
# ------------------------------------------------------------------
CMake_Minimum_Required(VERSION 2.8)

# ------------------------------------------------------------------
# Makefile includes.
# ------------------------------------------------------------------
include("makefiles/branch/${BRANCH}.cmake")
include("makefiles/shared.cmake")

# ------------------------------------------------------------------
# Loader
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LOADER_HEADERS
    loader/loader_main.h
)

Set(SOURCEPYTHON_LOADER_SOURCES
    loader/loader_main.cpp
)

Set(SOURCEPYTHON_LOADER_FILES
    ${SOURCEPYTHON_LOADER_HEADERS}
    ${SOURCEPYTHON_LOADER_SOURCES}
)

# ------------------------------------------------------------------
# Patches
# ------------------------------------------------------------------
Set(SOURCEPYTHON_PATCHES_HEADERS
    core/patches/${SOURCE_ENGINE}/undefined_symbols.h
)

Set(SOURCEPYTHON_PATCHES_SOURCES
    core/patches/${SOURCE_ENGINE}/patches.cpp
    core/patches/undefined_symbols.cpp
)

Set(SOURCEPYTHON_PATCHES_FILES
    ${SOURCEPYTHON_PATCHES_HEADERS}
    ${SOURCEPYTHON_PATCHES_SOURCES}
)


# ------------------------------------------------------------------
# Utilities
# ------------------------------------------------------------------
Set(SOURCEPYTHON_UTILITIES_HEADERS
    core/utilities/sp_util.h
    core/utilities/shared_utils.h
    core/utilities/call_python.h
    core/utilities/wrap_macros.h
    core/utilities/conversions.h
    core/utilities/ipythongenerator.h
)

Set(SOURCEPYTHON_UTILITIES_SOURCES
)

Set(SOURCEPYTHON_UTILITIES_FILES
    ${SOURCEPYTHON_UTILITIES_HEADERS}
    ${SOURCEPYTHON_UTILITIES_SOURCES}
)

# ------------------------------------------------------------------
# Core
# ------------------------------------------------------------------
Set(SOURCEPYTHON_CORE_HEADERS
    core/sp_python.h
    core/sp_main.h
    core/export_main.h
)

Set(SOURCEPYTHON_CORE_SOURCES
    core/sp_python.cpp
    core/sp_main.cpp
    core/export_main.cpp
)

Set(SOURCEPYTHON_CORE_FILES
    ${SOURCEPYTHON_CORE_HEADERS}
    ${SOURCEPYTHON_CORE_SOURCES}
)

# ------------------------------------------------------------------
# BitBuffers module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_BITBUFFERS_MODULE_HEADERS
    core/modules/bitbuffers/bitbuffers_wrap.h
)

Set(SOURCEPYTHON_BITBUFFERS_MODULE_SOURCES
    core/modules/bitbuffers/bitbuffers_wrap_python.cpp
)

# ------------------------------------------------------------------
# Colors module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_COLORS_MODULE_HEADERS
    core/modules/colors/colors_wrap.h
)

Set(SOURCEPYTHON_COLORS_MODULE_SOURCES
    core/modules/colors/colors_wrap_python.cpp
)

# ------------------------------------------------------------------
# Commands module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_COMMANDS_MODULE_HEADERS
    core/modules/commands/commands_client_wrap.h
    core/modules/commands/commands_wrap.h
    core/modules/commands/commands_say_wrap.h
    core/modules/commands/commands_server_wrap.h
)

Set(SOURCEPYTHON_COMMANDS_MODULE_SOURCES
    core/modules/commands/commands_client_wrap.cpp
    core/modules/commands/commands_client_wrap_python.cpp
    core/modules/commands/commands_wrap_python.cpp
    core/modules/commands/commands_say_wrap.cpp
    core/modules/commands/commands_say_wrap_python.cpp
    core/modules/commands/commands_server_wrap.cpp
    core/modules/commands/commands_server_wrap_python.cpp
)


# ------------------------------------------------------------------
# Core module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_CORE_MODULE_HEADERS
)

Set(SOURCEPYTHON_CORE_MODULE_SOURCES
    core/modules/core/core_wrap_python.cpp
)

# ------------------------------------------------------------------
# Cvars module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_CVARS_MODULE_HEADERS
    core/modules/cvars/cvars_wrap.h
    core/modules/cvars/${SOURCE_ENGINE}/cvars_wrap.h
)

Set(SOURCEPYTHON_CVARS_MODULE_SOURCES
    core/modules/cvars/cvars_wrap_python.cpp
)

# ------------------------------------------------------------------
# Effects module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_EFFECTS_MODULE_HEADERS
    core/modules/effects/effects_wrap.h
    core/modules/effects/${SOURCE_ENGINE}/effects_wrap_python.h
)

Set(SOURCEPYTHON_EFFECTS_MODULE_SOURCES
    core/modules/effects/effects_wrap_python.cpp
)

# ------------------------------------------------------------------
# Engines module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_ENGINES_MODULE_HEADERS
    core/modules/engines/engines_wrap.h
    core/modules/engines/${SOURCE_ENGINE}/engines_wrap.h
    core/modules/engines/${SOURCE_ENGINE}/engines_wrap_python.h
)

Set(SOURCEPYTHON_ENGINES_MODULE_SOURCES
    core/modules/engines/engines_wrap_python.cpp
)

# ------------------------------------------------------------------
# Entities module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_ENTITIES_MODULE_HEADERS
    core/modules/entities/entities_wrap.h
    core/modules/entities/${SOURCE_ENGINE}/entities_wrap.h
    core/modules/entities/entities_generator_wrap.h
    core/modules/entities/entities_helpers_wrap.h
    core/modules/entities/${SOURCE_ENGINE}/entities_helpers_wrap.h
    core/modules/entities/entities_factories_wrap.h
    core/modules/entities/${SOURCE_ENGINE}/entities_factories_wrap_python.h
    core/modules/entities/${SOURCE_ENGINE}/entities_wrap_python.h
    core/modules/entities/entities_datamaps_wrap.h
    core/modules/entities/${SOURCE_ENGINE}/entities_datamaps_wrap_python.h
    core/modules/entities/entities_props_wrap.h
    core/modules/entities/${SOURCE_ENGINE}/entities_props_wrap.h
    core/modules/entities/${SOURCE_ENGINE}/entities_props_wrap_python.h
    core/modules/entities/${SOURCE_ENGINE}/entities_constants_wrap_python.h
)

Set(SOURCEPYTHON_ENTITIES_MODULE_SOURCES
    core/modules/entities/entities_constants_wrap_python.cpp
    core/modules/entities/entities_helpers_wrap_python.cpp
    core/modules/entities/entities_wrap_python.cpp
    core/modules/entities/entities_factories_wrap_python.cpp
    core/modules/entities/entities_generator_wrap.cpp
    core/modules/entities/entities_datamaps_wrap_python.cpp
    core/modules/entities/entities_props_wrap_python.cpp
)

# ------------------------------------------------------------------
# Events module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_EVENTS_MODULE_HEADERS
    core/modules/events/events_wrap.h
)

Set(SOURCEPYTHON_EVENTS_MODULE_SOURCES
    core/modules/events/events_wrap_python.cpp
)

# ------------------------------------------------------------------
# Globals module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_GLOBALS_MODULE_HEADERS
    core/modules/globals/${SOURCE_ENGINE}/globals_wrap_python.h
)

Set(SOURCEPYTHON_GLOBALS_MODULE_SOURCES
    core/modules/globals/globals_wrap_python.cpp
)

# ------------------------------------------------------------------
# Keyvalues module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_KEYVALUES_MODULE_HEADERS
    core/modules/keyvalues/keyvalues_wrap.h
)

Set(SOURCEPYTHON_KEYVALUES_MODULE_SOURCES
    core/modules/keyvalues/keyvalues_wrap_python.cpp
    ${SOURCESDK}/tier1/KeyValues.cpp
)

# ------------------------------------------------------------------
# Listeners module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_LISTENERS_MODULE_HEADERS
    core/modules/listeners/listeners_manager.h
)

Set(SOURCEPYTHON_LISTENERS_MODULE_SOURCES
    core/modules/listeners/listeners_manager.cpp
    core/modules/listeners/listeners_wrap_python.cpp
)

# ------------------------------------------------------------------
# Mathlib module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_MATHLIB_MODULE_HEADERS
    core/modules/mathlib/mathlib_wrap.h
)

Set(SOURCEPYTHON_MATHLIB_MODULE_SOURCES
    core/modules/mathlib/mathlib_wrap_python.cpp
)

# ------------------------------------------------------------------
# Memory module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_MEMORY_MODULE_HEADERS
    core/modules/memory/memory_tools.h
    core/modules/memory/memory_scanner.h
    core/modules/memory/memory_hooks.h
)

Set(SOURCEPYTHON_MEMORY_MODULE_SOURCES
    core/modules/memory/memory_scanner.cpp
    core/modules/memory/memory_tools.cpp
    core/modules/memory/memory_hooks.cpp
    core/modules/memory/memory_wrap_python.cpp
)

# ------------------------------------------------------------------
# Messages module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_MESSAGES_MODULE_HEADERS
    core/modules/messages/messages.h
    core/modules/messages/messages_implementation_base.h
    core/modules/messages/${SOURCE_ENGINE}/messages_implementation.h
)

Set(SOURCEPYTHON_MESSAGES_MODULE_SOURCES
    core/modules/messages/messages.cpp
    core/modules/messages/messages_wrap_python.cpp
    core/modules/messages/messages_implementation_base.cpp
)

Set(SOURCEPYTHON_MESSAGES_MODULE_GAME_SOURCES
    ${SOURCEPYTHON_MESSAGES_MODULE_GAME_SOURCES}
    core/modules/messages/${SOURCE_ENGINE}/messages_implementation.cpp
)

# ------------------------------------------------------------------
# Players module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_PLAYERS_MODULE_HEADERS
    core/modules/players/players_generator_wrap.h
    core/modules/entities/${SOURCE_ENGINE}/entities_constants_wrap_python.h
)

Set(SOURCEPYTHON_PLAYERS_MODULE_SOURCES
    core/modules/players/players_bots_wrap_python.cpp
    core/modules/players/players_constants_wrap_python.cpp
    core/modules/players/players_helpers_wrap_python.cpp
    core/modules/players/players_wrap_python.cpp
    core/modules/players/players_generator_wrap.cpp
)

# ------------------------------------------------------------------
# Filters module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_FILTERS_MODULE_HEADERS
    core/modules/filters/filters_recipients_wrap.h
)

Set(SOURCEPYTHON_FILTERS_MODULE_SOURCES
    core/modules/filters/filters_recipients_wrap.cpp
    core/modules/filters/filters_recipients_wrap_python.cpp
)

# ------------------------------------------------------------------
# StringTables module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_STRINGTABLES_MODULE_HEADERS
)

Set(SOURCEPYTHON_STRINGTABLES_MODULE_SOURCES
    core/modules/stringtables/stringtables_wrap_python.cpp
)

# ------------------------------------------------------------------
# Studio module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_STUDIO_MODULE_HEADERS
    core/modules/studio/studio_wrap.h
)

Set(SOURCEPYTHON_STUDIO_MODULE_SOURCES
    # hl2sdk/${BRANCH}/public/studio.cpp
    core/modules/studio/studio_wrap_python.cpp
    core/modules/studio/studio_constants_wrap_python.cpp
    core/modules/studio/studio_cache_wrap_python.cpp
)

# ------------------------------------------------------------------
# Weapons module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_WEAPONS_MODULE_HEADERS
    core/modules/weapons/${SOURCE_ENGINE}/weapons_constants_wrap_python.h
    core/modules/weapons/${SOURCE_ENGINE}/weapons_scripts_wrap_python.h
)

Set(SOURCEPYTHON_WEAPONS_MODULE_SOURCES
    core/modules/weapons/weapons_constants_wrap_python.cpp
    core/modules/weapons/weapons_scripts_wrap_python.cpp
)

# ------------------------------------------------------------------
# All module source files
# ------------------------------------------------------------------
Set(SOURCEPYTHON_MODULE_FILES
    ${SOURCEPYTHON_BITBUFFERS_MODULE_HEADERS}
    ${SOURCEPYTHON_BITBUFFERS_MODULE_SOURCES}

    ${SOURCEPYTHON_COLORS_MODULE_HEADERS}
    ${SOURCEPYTHON_COLORS_MODULE_SOURCES}

    ${SOURCEPYTHON_COMMANDS_MODULE_HEADERS}
    ${SOURCEPYTHON_COMMANDS_MODULE_SOURCES}

    ${SOURCEPYTHON_CVARS_MODULE_HEADERS}
    ${SOURCEPYTHON_CVARS_MODULE_SOURCES}

    ${SOURCEPYTHON_EFFECTS_MODULE_HEADERS}
    ${SOURCEPYTHON_EFFECTS_MODULE_SOURCES}

    ${SOURCEPYTHON_ENGINES_MODULE_HEADERS}
    ${SOURCEPYTHON_ENGINES_MODULE_SOURCES}

    ${SOURCEPYTHON_ENTITIES_MODULE_HEADERS}
    ${SOURCEPYTHON_ENTITIES_MODULE_SOURCES}

    # Moved here cause CSourcePython relies on IEntityListener being
    # exposed at run-time.
    ${SOURCEPYTHON_CORE_MODULE_HEADERS}
    ${SOURCEPYTHON_CORE_MODULE_SOURCES}

    ${SOURCEPYTHON_EVENTS_MODULE_HEADERS}
    ${SOURCEPYTHON_EVENTS_MODULE_SOURCES}

    ${SOURCEPYTHON_GLOBALS_MODULE_HEADERS}
    ${SOURCEPYTHON_GLOBALS_MODULE_SOURCES}

    ${SOURCEPYTHON_KEYVALUES_MODULE_HEADERS}
    ${SOURCEPYTHON_KEYVALUES_MODULE_SOURCES}

    ${SOURCEPYTHON_LISTENERS_MODULE_HEADERS}
    ${SOURCEPYTHON_LISTENERS_MODULE_SOURCES}

    ${SOURCEPYTHON_MATHLIB_MODULE_HEADERS}
    ${SOURCEPYTHON_MATHLIB_MODULE_SOURCES}

    ${SOURCEPYTHON_MEMORY_MODULE_HEADERS}
    ${SOURCEPYTHON_MEMORY_MODULE_SOURCES}

    ${SOURCEPYTHON_MESSAGES_MODULE_HEADERS}
    ${SOURCEPYTHON_MESSAGES_MODULE_SOURCES}
    ${SOURCEPYTHON_MESSAGES_MODULE_GAME_SOURCES}

    ${SOURCEPYTHON_PLAYERS_MODULE_HEADERS}
    ${SOURCEPYTHON_PLAYERS_MODULE_SOURCES}

    ${SOURCEPYTHON_FILTERS_MODULE_HEADERS}
    ${SOURCEPYTHON_FILTERS_MODULE_SOURCES}

    ${SOURCEPYTHON_STRINGTABLES_MODULE_HEADERS}
    ${SOURCEPYTHON_STRINGTABLES_MODULE_SOURCES}

    ${SOURCEPYTHON_STUDIO_MODULE_HEADERS}
    ${SOURCEPYTHON_STUDIO_MODULE_SOURCES}

    ${SOURCEPYTHON_WEAPONS_MODULE_HEADERS}
    ${SOURCEPYTHON_WEAPONS_MODULE_SOURCES}
)

# ------------------------------------------------------------------
# Create source groups for everything to make our projects nice and
# clean.
# ------------------------------------------------------------------
Source_Group("Header Files\\Core"                           FILES ${SOURCEPYTHON_CORE_HEADERS})
Source_Group("Header Files\\Patches"                        FILES ${SOURCEPYTHON_PATCHES_HEADERS})
Source_Group("Header Files\\Utilities"                      FILES ${SOURCEPYTHON_UTILITIES_HEADERS})

Source_Group("Header Files\\Modules\\BitBuffers"             FILES ${SOURCEPYTHON_BITBUFFERS_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Colors"                 FILES ${SOURCEPYTHON_COLORS_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Commands"               FILES ${SOURCEPYTHON_COMMANDS_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Core"                   FILES ${SOURCEPYTHON_CORE_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Cvars"                  FILES ${SOURCEPYTHON_CVARS_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Effects"                FILES ${SOURCEPYTHON_EFFECTS_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Engines"                FILES ${SOURCEPYTHON_ENGINES_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Entities"               FILES ${SOURCEPYTHON_ENTITIES_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Events"                 FILES ${SOURCEPYTHON_EVENTS_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Globals"                FILES ${SOURCEPYTHON_GLOBALS_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\KeyValues"              FILES ${SOURCEPYTHON_KEYVALUES_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Listeners"              FILES ${SOURCEPYTHON_LISTENERS_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Mathlib"                FILES ${SOURCEPYTHON_MATHLIB_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Memory"                 FILES ${SOURCEPYTHON_MEMORY_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Messages"               FILES ${SOURCEPYTHON_MESSAGES_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Players"                FILES ${SOURCEPYTHON_PLAYERS_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Filters"                FILES ${SOURCEPYTHON_FILTERS_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\StringTables"           FILES ${SOURCEPYTHON_STRINGTABLES_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Studio"                 FILES ${SOURCEPYTHON_STUDIO_MODULE_HEADERS})
Source_Group("Header Files\\Modules\\Weapons"                FILES ${SOURCEPYTHON_WEAPONS_MODULE_HEADERS})

Source_Group("Source Files\\Core"                           FILES ${SOURCEPYTHON_CORE_SOURCES})
Source_Group("Source Files\\Patches"                        FILES ${SOURCEPYTHON_PATCHES_SOURCES})
Source_Group("Source Files\\Utilities"                      FILES ${SOURCEPYTHON_UTILITIES_SOURCES})

Source_Group("Source Files\\Modules\\BitBuffers"             FILES ${SOURCEPYTHON_BITBUFFERS_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Colors"                 FILES ${SOURCEPYTHON_COLORS_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Commands"               FILES ${SOURCEPYTHON_COMMANDS_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Core"                   FILES ${SOURCEPYTHON_CORE_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Cvars"                  FILES ${SOURCEPYTHON_CVARS_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Effects"                FILES ${SOURCEPYTHON_EFFECTS_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Engines"                FILES ${SOURCEPYTHON_ENGINES_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Entities"               FILES ${SOURCEPYTHON_ENTITIES_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Events"                 FILES ${SOURCEPYTHON_EVENTS_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Globals"                FILES ${SOURCEPYTHON_GLOBALS_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\KeyValues"              FILES ${SOURCEPYTHON_KEYVALUES_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Listeners"              FILES ${SOURCEPYTHON_LISTENERS_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Mathlib"                FILES ${SOURCEPYTHON_MATHLIB_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Memory"                 FILES ${SOURCEPYTHON_MEMORY_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Messages"               FILES ${SOURCEPYTHON_MESSAGES_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Messages\\${BRANCH}"    FILES ${SOURCEPYTHON_MESSAGES_MODULE_GAME_SOURCES})
Source_Group("Source Files\\Modules\\Players"                FILES ${SOURCEPYTHON_PLAYERS_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Filters"                FILES ${SOURCEPYTHON_FILTERS_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\StringTables"           FILES ${SOURCEPYTHON_STRINGTABLES_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Studio"                 FILES ${SOURCEPYTHON_STUDIO_MODULE_SOURCES})
Source_Group("Source Files\\Modules\\Weapons"                FILES ${SOURCEPYTHON_WEAPONS_MODULE_SOURCES})

# ------------------------------------------------------------------
# All SourcePython source files. Ideally we break out each group of
# files into their own defines (as above with SOURCEPYTHON_CORE_FILES.
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# This basically creates the SourcePython project. Certain project
# attributes like Target_Link_Libraries need to be declared after
# this line.
# ------------------------------------------------------------------
Add_Library(core SHARED
    ${SOURCEPYTHON_PATCHES_FILES}
    ${SOURCEPYTHON_CORE_FILES}
    ${SOURCEPYTHON_MODULE_FILES}
    ${SOURCEPYTHON_UTILITIES_FILES}
)

Add_Library(source-python SHARED
    ${SOURCEPYTHON_LOADER_FILES}
)

# ------------------------------------------------------------------
# Remove prefixes for core and source-python
# ------------------------------------------------------------------
Set_Target_Properties(core PROPERTIES PREFIX "")
Set_Target_Properties(source-python PROPERTIES PREFIX "")

# ------------------------------------------------------------------
# Include the base makefile.
# ------------------------------------------------------------------
If(WIN32)
    include("makefiles/win32/win32.base.cmake")
Else()
    include("makefiles/linux/linux.base.cmake")
EndIf()

# ------------------------------------------------------------------
# Definitions
# ------------------------------------------------------------------
Add_Definitions(
    -DSOURCE_ENGINE=${SOURCE_ENGINE}
    -DSOURCE_ENGINE_BRANCH=${BRANCH}
    -DBOOST_PYTHON_MAX_ARITY=32
)

# ------------------------------------------------------------------
# Setup link libraries for this project.
# ------------------------------------------------------------------
Target_Link_Libraries(core          ${SOURCEPYTHON_LINK_LIBRARIES})
Target_Link_Libraries(source-python ${SOURCEPYTHON_LINK_LIBRARIES})

# ------------------------------------------------------------------
# This is a hack for linux which doesn't seem to have proper
# configurations which breaks link libraries.
# ------------------------------------------------------------------
If(UNIX)
    If(CMAKE_BUILD_TYPE MATCHES Debug)
        Target_Link_Libraries(core ${SOURCEPYTHON_LINK_LIBRARIES_DEBUG})
    Else()
        Target_Link_Libraries(core ${SOURCEPYTHON_LINK_LIBRARIES_RELEASE})
    EndIf()
Else()
    # Do it the correct way.
    Target_Link_Libraries(core ${SOURCEPYTHON_LINK_LIBRARIES_DEBUG})
    Target_Link_Libraries(core ${SOURCEPYTHON_LINK_LIBRARIES_RELEASE})
EndIf()

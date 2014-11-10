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
# Utility
# ------------------------------------------------------------------
Set(SOURCEPYTHON_UTIL_HEADERS
    core/utility/sp_util.h
    core/utility/call_python.h
    core/utility/wrap_macros.h
    core/utility/ipythongenerator.h
)

Set(SOURCEPYTHON_UTIL_SOURCES
    core/utility/patches/${SOURCE_ENGINE}/patches.cpp
)

Set(SOURCEPYTHON_UTIL_FILES
    ${SOURCEPYTHON_UTIL_HEADERS}
    ${SOURCEPYTHON_UTIL_SOURCES}
)

# ------------------------------------------------------------------
# Core
# ------------------------------------------------------------------
Set(SOURCEPYTHON_CORE_HEADERS
    core/core/sp_python.h
    core/core/sp_main.h
    core/core/sp_gamedir.h
)

Set(SOURCEPYTHON_CORE_SOURCES
    core/core/sp_python.cpp
    core/core/sp_main.cpp
    core/core/sp_gamedir.cpp
)

Set(SOURCEPYTHON_CORE_FILES
    ${SOURCEPYTHON_CORE_HEADERS}
    ${SOURCEPYTHON_CORE_SOURCES}
)

# ------------------------------------------------------------------
# Addon system
# ------------------------------------------------------------------
Set(SOURCEPYTHON_ADDON_HEADERS
    core/addons/sp_addon.h
)

Set(SOURCEPYTHON_ADDON_SOURCES
    core/addons/sp_addon.cpp
)

Set(SOURCEPYTHON_ADDON_FILES
    ${SOURCEPYTHON_ADDON_HEADERS}
    ${SOURCEPYTHON_ADDON_SOURCES}
)

# ------------------------------------------------------------------
# Generic plugin stuff.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_GENERIC_HEADERS
    core/modules/export_main.h
)

Set(SOURCEPYTHON_GENERIC_SOURCES
    core/modules/export_main.cpp
)

Set(SOURCEPYTHON_GENERIC_FILES
    ${SOURCEPYTHON_GENERIC_HEADERS}
    ${SOURCEPYTHON_GENERIC_SOURCES}
)

# ------------------------------------------------------------------
# BaseTypes module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_BASETYPES_MODULE_HEADERS
	core/modules/basetypes/basetypes_wrap.h
	core/modules/basetypes/${SOURCE_ENGINE}/basetypes_wrap.h
	core/modules/basetypes/${SOURCE_ENGINE}/basetypes_wrap_python.h
)

Set(SOURCEPYTHON_BASETYPES_MODULE_SOURCES
    core/modules/basetypes/basetypes_wrap_python.cpp
)

# ------------------------------------------------------------------
# Bots module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_BOTS_MODULE_HEADERS
)

Set(SOURCEPYTHON_BOTS_MODULE_SOURCES
    core/modules/bots/bots_wrap_python.cpp
)

# ------------------------------------------------------------------
# Commands module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_COMMANDS_MODULE_HEADERS
    core/modules/commands/client_commands_wrap.h
    core/modules/commands/commands_wrap.h
    core/modules/commands/say_commands_wrap.h
    core/modules/commands/server_commands_wrap.h
)

Set(SOURCEPYTHON_COMMANDS_MODULE_SOURCES
    core/modules/commands/client_commands_wrap.cpp
    core/modules/commands/commands_wrap_python.cpp
    core/modules/commands/say_commands_wrap.cpp
    core/modules/commands/server_commands_wrap.cpp
)

# ------------------------------------------------------------------
# Constants module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_CONSTANTS_MODULE_HEADERS
    core/modules/constants/constants_const_wrap.h
    core/modules/constants/constants_const_wrap_python.h
    core/modules/constants/${SOURCE_ENGINE}/constants_const_wrap_python.h
    core/modules/constants/constants_in_buttons_wrap.h
    core/modules/constants/constants_in_buttons_wrap_python.h
)

Set(SOURCEPYTHON_CONSTANTS_MODULE_SOURCES
    core/modules/constants/constants_wrap_python.cpp
)

# ------------------------------------------------------------------
# Conversions module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_CONVERSIONS_MODULE_HEADERS
    core/modules/conversions/conversions_wrap.h
)

Set(SOURCEPYTHON_CONVERSIONS_MODULE_SOURCES
    core/modules/conversions/conversions_wrap_python.cpp
)

# ------------------------------------------------------------------
# Cvars module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_CVARS_MODULE_HEADERS
    core/modules/cvars/${SOURCE_ENGINE}/cvars_wrap.h
)

Set(SOURCEPYTHON_CVARS_MODULE_SOURCES
    core/modules/cvars/cvars_wrap_python.cpp
)

# ------------------------------------------------------------------
# Datamaps module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_DATAMAPS_MODULE_HEADERS
    core/modules/datamaps/datamaps_wrap.h
    core/modules/datamaps/${SOURCE_ENGINE}/datamaps_wrap_python.h
)

Set(SOURCEPYTHON_DATAMAPS_MODULE_SOURCES
    core/modules/datamaps/datamaps_wrap_python.cpp
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
    core/modules/entities/entities_props.h
    core/modules/entities/entities_generator_wrap.h
    core/modules/entities/entities_helpers_wrap.h
    core/modules/entities/${SOURCE_ENGINE}/entities_helpers_wrap.h
	core/modules/entities/entities_factories_wrap.h
	core/modules/entities/${SOURCE_ENGINE}/entities_wrap_python.h
)

Set(SOURCEPYTHON_ENTITIES_MODULE_SOURCES
    core/modules/entities/entities_props.cpp
    core/modules/entities/entities_wrap.cpp
    core/modules/entities/entities_wrap_python.cpp
    core/modules/entities/entities_generator_wrap.cpp
)

# ------------------------------------------------------------------
# Events module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_EVENTS_MODULE_HEADERS
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
    core/modules/memory/memory_callback.h
)

Set(SOURCEPYTHON_MEMORY_MODULE_SOURCES
    core/modules/memory/memory_scanner.cpp
    core/modules/memory/memory_tools.cpp
    core/modules/memory/memory_hooks.cpp
    core/modules/memory/memory_callback.cpp
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
)

Set(SOURCEPYTHON_PLAYERS_MODULE_SOURCES
    core/modules/players/players_wrap_python.cpp
    core/modules/players/players_generator_wrap.cpp
)

# ------------------------------------------------------------------
# Recipients module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_RECIPIENTS_MODULE_HEADERS
    core/modules/recipients/mrecipientfilter.h
)

Set(SOURCEPYTHON_RECIPIENTS_MODULE_SOURCES
    core/modules/recipients/mrecipientfilter.cpp
    core/modules/recipients/recipients_wrap_python.cpp
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
# All module source files
# ------------------------------------------------------------------
Set(SOURCEPYTHON_MODULE_FILES
    ${SOURCEPYTHON_BASETYPES_MODULE_HEADERS}
    ${SOURCEPYTHON_BASETYPES_MODULE_SOURCES}
    
    ${SOURCEPYTHON_BOTS_MODULE_HEADERS}
    ${SOURCEPYTHON_BOTS_MODULE_SOURCES}

    ${SOURCEPYTHON_COMMANDS_MODULE_HEADERS}
    ${SOURCEPYTHON_COMMANDS_MODULE_SOURCES}

    ${SOURCEPYTHON_CONSTANTS_MODULE_HEADERS}
    ${SOURCEPYTHON_CONSTANTS_MODULE_SOURCES}

    ${SOURCEPYTHON_CONVERSIONS_MODULE_HEADERS}
    ${SOURCEPYTHON_CONVERSIONS_MODULE_SOURCES}

    ${SOURCEPYTHON_CVARS_MODULE_HEADERS}
    ${SOURCEPYTHON_CVARS_MODULE_SOURCES}

    ${SOURCEPYTHON_DATAMAPS_MODULE_HEADERS}
    ${SOURCEPYTHON_DATAMAPS_MODULE_SOURCES}

    ${SOURCEPYTHON_EFFECTS_MODULE_HEADERS}
    ${SOURCEPYTHON_EFFECTS_MODULE_SOURCES}

    ${SOURCEPYTHON_ENGINES_MODULE_HEADERS}
    ${SOURCEPYTHON_ENGINES_MODULE_SOURCES}

    ${SOURCEPYTHON_ENTITIES_MODULE_HEADERS}
    ${SOURCEPYTHON_ENTITIES_MODULE_SOURCES}

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

    ${SOURCEPYTHON_RECIPIENTS_MODULE_HEADERS}
    ${SOURCEPYTHON_RECIPIENTS_MODULE_SOURCES}

    ${SOURCEPYTHON_STRINGTABLES_MODULE_HEADERS}
    ${SOURCEPYTHON_STRINGTABLES_MODULE_SOURCES}
)

# ------------------------------------------------------------------
# Create source groups for everything to make our projects nice and
# clean.
# ------------------------------------------------------------------
Source_Group("Header Files\\Addons"                         FILES ${SOURCEPYTHON_ADDON_HEADERS})
Source_Group("Header Files\\Core"                           FILES ${SOURCEPYTHON_CORE_HEADERS})
Source_Group("Header Files\\Util"                           FILES ${SOURCEPYTHON_UTIL_HEADERS})
Source_Group("Header Files\\Generic"                        FILES ${SOURCEPYTHON_GENERIC_HEADERS})

Source_Group("Header Files\\Module\\BaseTypes"              FILES ${SOURCEPYTHON_BASETYPES_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Bots"                   FILES ${SOURCEPYTHON_BOTS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Commands"               FILES ${SOURCEPYTHON_COMMANDS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Constants"              FILES ${SOURCEPYTHON_CONSTANTS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Conversions"            FILES ${SOURCEPYTHON_CONVERSIONS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Cvars"                  FILES ${SOURCEPYTHON_CVARS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\DataMaps"               FILES ${SOURCEPYTHON_DATAMAPS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Effects"                FILES ${SOURCEPYTHON_EFFECTS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Engines"                FILES ${SOURCEPYTHON_ENGINES_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Entities"               FILES ${SOURCEPYTHON_ENTITIES_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Events"                 FILES ${SOURCEPYTHON_EVENTS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Globals"                FILES ${SOURCEPYTHON_GLOBALS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\KeyValues"              FILES ${SOURCEPYTHON_KEYVALUES_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Listeners"              FILES ${SOURCEPYTHON_LISTENERS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Mathlib"                FILES ${SOURCEPYTHON_MATHLIB_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Memory"                 FILES ${SOURCEPYTHON_MEMORY_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Messages"               FILES ${SOURCEPYTHON_MESSAGES_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Players"                FILES ${SOURCEPYTHON_PLAYERS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Recipients"             FILES ${SOURCEPYTHON_RECIPIENTS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\StringTables"           FILES ${SOURCEPYTHON_STRINGTABLES_MODULE_HEADERS})

Source_Group("Source Files\\Addons"                         FILES ${SOURCEPYTHON_ADDON_SOURCES})
Source_Group("Source Files\\Core"                           FILES ${SOURCEPYTHON_CORE_SOURCES})
Source_Group("Source Files\\Util"                           FILES ${SOURCEPYTHON_UTIL_SOURCES})
Source_Group("Source Files\\Generic"                        FILES ${SOURCEPYTHON_GENERIC_SOURCES})
Source_Group("Source Files\\Module\\BaseTypes"              FILES ${SOURCEPYTHON_BASETYPES_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Bots"                   FILES ${SOURCEPYTHON_BOTS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Commands"               FILES ${SOURCEPYTHON_COMMANDS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Constants"              FILES ${SOURCEPYTHON_CONSTANTS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Conversions"            FILES ${SOURCEPYTHON_CONVERSIONS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Cvars"                  FILES ${SOURCEPYTHON_CVARS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\DataMaps"               FILES ${SOURCEPYTHON_DATAMAPS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Effects"                FILES ${SOURCEPYTHON_EFFECTS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Engines"                FILES ${SOURCEPYTHON_ENGINES_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Entities"               FILES ${SOURCEPYTHON_ENTITIES_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Events"                 FILES ${SOURCEPYTHON_EVENTS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Globals"                FILES ${SOURCEPYTHON_GLOBALS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\KeyValues"              FILES ${SOURCEPYTHON_KEYVALUES_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Listeners"              FILES ${SOURCEPYTHON_LISTENERS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Mathlib"                FILES ${SOURCEPYTHON_MATHLIB_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Memory"                 FILES ${SOURCEPYTHON_MEMORY_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Messages"               FILES ${SOURCEPYTHON_MESSAGES_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Messages\\${BRANCH}"    FILES ${SOURCEPYTHON_MESSAGES_MODULE_GAME_SOURCES})
Source_Group("Source Files\\Module\\Players"                FILES ${SOURCEPYTHON_PLAYERS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Recipients"             FILES ${SOURCEPYTHON_RECIPIENTS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\StringTables"           FILES ${SOURCEPYTHON_STRINGTABLES_MODULE_SOURCES})

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
    ${SOURCEPYTHON_CORE_FILES}
    ${SOURCEPYTHON_ADDON_FILES}
    ${SOURCEPYTHON_GENERIC_FILES}
    ${SOURCEPYTHON_MODULE_FILES}
    ${SOURCEPYTHON_UTIL_FILES}
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

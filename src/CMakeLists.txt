# ------------------------------------------------------------------
# Set minimum required version of CMake
# ------------------------------------------------------------------
CMake_Minimum_Required(VERSION 2.8)

# ------------------------------------------------------------------
# Makefile includes.
# ------------------------------------------------------------------
include("makefiles/game/${GAME}.cmake")
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
	core/utility/mrecipientfilter.h
    core/utility/mrecipientfilter_wrap.h
    core/utility/ipythongenerator.h
)

Set(SOURCEPYTHON_UTIL_SOURCES
	core/utility/mrecipientfilter.cpp
    core/utility/mrecipientfilter_wrap.cpp
    core/utility/mrecipientfilter_wrap_python.cpp
    core/utility/patches/engine${SOURCE_ENGINE}/patches.cpp
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
# Command module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_COMMAND_MODULE_HEADERS
    core/modules/commands/client_command_wrap.h
    core/modules/commands/command_wrap.h
    core/modules/commands/say_command_wrap.h
    core/modules/commands/server_command_wrap.h
)

Set(SOURCEPYTHON_COMMAND_MODULE_SOURCES
    core/modules/commands/client_command_wrap.cpp
    core/modules/commands/command_wrap.cpp
    core/modules/commands/command_wrap_python.cpp
    core/modules/commands/say_command_wrap.cpp
    core/modules/commands/server_command_wrap.cpp
)

# ------------------------------------------------------------------
# Cvar module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_CVAR_MODULE_HEADERS
    core/modules/cvar/cvar_wrap.h
)

Set(SOURCEPYTHON_CVAR_MODULE_SOURCES
    core/modules/cvar/cvar_wrap.cpp
    core/modules/cvar/cvar_wrap_python.cpp
)

# ------------------------------------------------------------------
# Engine module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_ENGINE_MODULE_HEADERS
	core/modules/engine/eiface_wrap.h
    core/modules/engine/eiface_engine_base.h
    core/modules/engine/engine${SOURCE_ENGINE}/eiface_engine_implementation.h
)

Set(SOURCEPYTHON_ENGINE_MODULE_SOURCES
    core/modules/engine/eiface_wrap.cpp
    core/modules/engine/eiface_engine_base.cpp
    core/modules/engine/engine${SOURCE_ENGINE}/eiface_engine_implementation.cpp
    core/modules/engine/eiface_wrap_python.cpp
)

Set(SOURCEPYTHON_ENGINE_MODULE_FILES
	${SOURCEPYTHON_ENGINE_MODULE_HEADERS}
	${SOURCEPYTHON_ENGINE_MODULE_SOURCES}
)

# ------------------------------------------------------------------
# Engine / Usermessage module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_ENGINE_USERMESSAGE_MODULE_HEADERS
    core/modules/engine/usermessage/usermessage.h
    core/modules/engine/usermessage/usermessage_implementation_base.h
    core/modules/engine/usermessage/${SOURCE_GAME}/usermessage_implementation.h
)

Set(SOURCEPYTHON_ENGINE_USERMESSAGE_MODULE_SOURCES
    core/modules/engine/usermessage/usermessage.cpp
    core/modules/engine/usermessage/usermessage_wrap_python.cpp
    core/modules/engine/usermessage/usermessage_implementation_base.cpp
    core/modules/engine/usermessage/${SOURCE_GAME}/usermessage_implementation.cpp
)

Set(SOURCEPYTHON_ENGINE_USERMESSAGE_MODULE_FILES
	${SOURCEPYTHON_ENGINE_USERMESSAGE_MODULE_HEADERS}
	${SOURCEPYTHON_ENGINE_USERMESSAGE_MODULE_SOURCES}
    ${SOURCEPYTHON_MODULE_USERMESSAGES_GAME_SOURCES}
)

# ------------------------------------------------------------------
# Event module.
# ------------------------------------------------------------------
Set(SOURCEPYTHON_EVENT_MODULE_HEADERS
    core/modules/events/events_wrap.h
)

Set(SOURCEPYTHON_EVENT_MODULE_SOURCES
    core/modules/events/events_wrap.cpp
    core/modules/events/events_wrap_python.cpp
)

# ------------------------------------------------------------------
# Entities module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_ENTITY_MODULE_HEADERS
    core/modules/entities/entities_wrap.h
    core/modules/entities/entities_props.h
    core/modules/entities/entities_generator_wrap.h
)

Set(SOURCEPYTHON_ENTITY_MODULE_SOURCES
    core/modules/entities/entities_props.cpp
    core/modules/entities/entities_wrap.cpp
    core/modules/entities/entities_wrap_python.cpp
    core/modules/entities/entities_generator_wrap.cpp
)

# ------------------------------------------------------------------
# Globals module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_GLOBALS_MODULE_HEADERS
    core/modules/globals/globals_wrap.h
    core/modules/globals/globals_engine_base.h
    core/modules/globals/engine${SOURCE_ENGINE}/globals_engine_implementation.h
)

Set(SOURCEPYTHON_GLOBALS_MODULE_SOURCES
    core/modules/globals/globals_wrap.cpp
    core/modules/globals/globals_engine_base.cpp
    core/modules/globals/engine${SOURCE_ENGINE}/globals_engine_implementation.cpp
    core/modules/globals/globals_wrap_python.cpp
)

# ------------------------------------------------------------------
# Keyvalues module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_KEYVALUES_MODULE_HEADERS
    core/modules/keyvalues/keyvalues_wrap.h
)

Set(SOURCEPYTHON_KEYVALUES_MODULE_SOURCES
    core/modules/keyvalues/keyvalues_wrap.cpp
    core/modules/keyvalues/keyvalues_wrap_python.cpp
    ${SOURCESDK}/tier1/KeyValues.cpp
)

# ------------------------------------------------------------------
# Players module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_PLAYERS_MODULE_HEADERS
    core/modules/players/players_wrap.h
    core/modules/players/players_generator_wrap.h
)

Set(SOURCEPYTHON_PLAYERS_MODULE_SOURCES
    core/modules/players/players_wrap.cpp
    core/modules/players/players_wrap_python.cpp
    core/modules/players/players_generator_wrap.cpp
)

# ------------------------------------------------------------------
# Tick Listener module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_TICKLISTENER_MODULE_HEADERS
    core/modules/ticklisteners/ticklisteners_wrap.h
)

Set(SOURCEPYTHON_TICKLISTENER_MODULE_SOURCES
    core/modules/ticklisteners/ticklisteners_wrap.cpp
    core/modules/ticklisteners/ticklisteners_wrap_python.cpp
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
# Vecmath module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_VECMATH_MODULE_HEADERS
    core/modules/vecmath/vecmath_wrap.h
)

Set(SOURCEPYTHON_VECMATH_MODULE_SOURCES
    core/modules/vecmath/vecmath_wrap.cpp
    core/modules/vecmath/vecmath_wrap_python.cpp
)

# ------------------------------------------------------------------
# Effects module
# ------------------------------------------------------------------
Set(SOURCEPYTHON_EFFECTS_MODULE_HEADERS
    core/modules/effects/effects_wrap.h
)

Set(SOURCEPYTHON_EFFECTS_MODULE_SOURCES
    core/modules/effects/effects_wrap.cpp
    core/modules/effects/effects_wrap_python.cpp
)

# ------------------------------------------------------------------
# All module source files
# ------------------------------------------------------------------
Set(SOURCEPYTHON_MODULE_FILES
    ${SOURCEPYTHON_COMMAND_MODULE_HEADERS}
    ${SOURCEPYTHON_COMMAND_MODULE_SOURCES}

    ${SOURCEPYTHON_CVAR_MODULE_HEADERS}
    ${SOURCEPYTHON_CVAR_MODULE_SOURCES}

	${SOURCEPYTHON_ENGINE_MODULE_FILES}
    ${SOURCEPYTHON_ENGINE_USERMESSAGE_MODULE_FILES}
    
    ${SOURCEPYTHON_EVENT_MODULE_HEADERS}
    ${SOURCEPYTHON_EVENT_MODULE_SOURCES}
    
    ${SOURCEPYTHON_ENTITY_MODULE_HEADERS}
    ${SOURCEPYTHON_ENTITY_MODULE_SOURCES}
    
    ${SOURCEPYTHON_KEYVALUES_MODULE_HEADERS}
    ${SOURCEPYTHON_KEYVALUES_MODULE_SOURCES}

    ${SOURCEPYTHON_PLAYERS_MODULE_HEADERS}
    ${SOURCEPYTHON_PLAYERS_MODULE_SOURCES}

    ${SOURCEPYTHON_TICKLISTENER_MODULE_HEADERS}
    ${SOURCEPYTHON_TICKLISTENER_MODULE_SOURCES}

    ${SOURCEPYTHON_GLOBALS_MODULE_HEADERS}
    ${SOURCEPYTHON_GLOBALS_MODULE_SOURCES}
    
    ${SOURCEPYTHON_MEMORY_MODULE_HEADERS}
    ${SOURCEPYTHON_MEMORY_MODULE_SOURCES}
    
    ${SOURCEPYTHON_VECMATH_MODULE_HEADERS}
    ${SOURCEPYTHON_VECMATH_MODULE_SOURCES}
    
    ${SOURCEPYTHON_EFFECTS_MODULE_HEADERS}
    ${SOURCEPYTHON_EFFECTS_MODULE_SOURCES}
)

# ------------------------------------------------------------------
# Create source groups for everything to make our projects nice and 
# clean.
# ------------------------------------------------------------------
Source_Group("Header Files\\Addons"			                FILES ${SOURCEPYTHON_ADDON_HEADERS})
Source_Group("Header Files\\Core"   		                FILES ${SOURCEPYTHON_CORE_HEADERS})
Source_Group("Header Files\\Util"   		                FILES ${SOURCEPYTHON_UTIL_HEADERS})
Source_Group("Header Files\\Generic"		                FILES ${SOURCEPYTHON_GENERIC_HEADERS})
Source_Group("Header Files\\Module\\Command"                FILES ${SOURCEPYTHON_COMMAND_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Cvar"	                FILES ${SOURCEPYTHON_CVAR_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Engine"	                FILES ${SOURCEPYTHON_ENGINE_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Engine\\Usermessages"	FILES ${SOURCEPYTHON_ENGINE_USERMESSAGE_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Event"	                FILES ${SOURCEPYTHON_EVENT_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Entities"               FILES ${SOURCEPYTHON_ENTITY_MODULE_HEADERS})
Source_Group("Header Files\\Module\\KeyValues"              FILES ${SOURCEPYTHON_KEYVALUES_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Players"                FILES ${SOURCEPYTHON_PLAYERS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\TickListeners"          FILES ${SOURCEPYTHON_TICKLISTENER_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Globals"                FILES ${SOURCEPYTHON_GLOBALS_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Memory"                 FILES ${SOURCEPYTHON_MEMORY_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Vecmath"                FILES ${SOURCEPYTHON_VECMATH_MODULE_HEADERS})
Source_Group("Header Files\\Module\\Effects"                FILES ${SOURCEPYTHON_EFFECTS_MODULE_HEADERS})

Source_Group("Source Files\\Addons" 		                FILES ${SOURCEPYTHON_ADDON_SOURCES})
Source_Group("Source Files\\Core"   		                FILES ${SOURCEPYTHON_CORE_SOURCES})
Source_Group("Source Files\\Util"   		                FILES ${SOURCEPYTHON_UTIL_SOURCES})
Source_Group("Source Files\\Generic"		                FILES ${SOURCEPYTHON_GENERIC_SOURCES})
Source_Group("Source Files\\Module\\Command"                FILES ${SOURCEPYTHON_COMMAND_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Cvar"	                FILES ${SOURCEPYTHON_CVAR_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Engine"	                FILES ${SOURCEPYTHON_ENGINE_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Engine\\Usermessages"	FILES ${SOURCEPYTHON_ENGINE_USERMESSAGE_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Engine\\Usermessages\\${GAME}"	FILES ${SOURCEPYTHON_MODULE_USERMESSAGES_GAME_SOURCES})
Source_Group("Source Files\\Module\\Event"	                FILES ${SOURCEPYTHON_EVENT_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Entities"               FILES ${SOURCEPYTHON_ENTITY_MODULE_SOURCES})
Source_Group("Source Files\\Module\\KeyValues"              FILES ${SOURCEPYTHON_KEYVALUES_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Players"                FILES ${SOURCEPYTHON_PLAYERS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\TickListeners"          FILES ${SOURCEPYTHON_TICKLISTENER_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Globals"                FILES ${SOURCEPYTHON_GLOBALS_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Memory"                 FILES ${SOURCEPYTHON_MEMORY_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Vecmath"                FILES ${SOURCEPYTHON_VECMATH_MODULE_SOURCES})
Source_Group("Source Files\\Module\\Effects"                FILES ${SOURCEPYTHON_EFFECTS_MODULE_SOURCES})

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
    -DBOOST_PYTHON_MAX_ARITY=18
    -DSOURCE_GAME=${SOURCE_GAME}
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

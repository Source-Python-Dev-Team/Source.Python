/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "globals_wrap.h"
#include "modules/export_main.h"


void export_maploadtype_t();
void export_globals();

//-----------------------------------------------------------------------------
// Exposes the globals_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(globals_c)
{
    export_maploadtype_t();
    export_globals();
}

void export_maploadtype_t()
{
	BOOST_ENUM( MapLoadType_t )
		ENUM_VALUE( "NewGame", MapLoad_NewGame )
		ENUM_VALUE( "LoadGame", MapLoad_LoadGame )
		ENUM_VALUE( "Transition", MapLoad_Transition )
		ENUM_VALUE( "Background", MapLoad_Background )
	BOOST_END_CLASS()
}

void export_globals()
{
    BOOST_ABSTRACT_CLASS(CGlobals)
        
        CLASS_METHOD(CGlobals,
            is_client,
            "Returns True if the game is a client."
        )

        CLASS_METHOD(CGlobals,
            is_remote_client,
            "Returns True if the game is a remote client."
        )

        CLASS_METHOD(CGlobals,
            get_network_base,
            "for encoding m_flSimulationTime, m_flAnimTime",
            args("nTick", "nEntity")
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "real_time",
            get_real_time,
            "Absolute time."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "frame_count",
            get_frame_count,
            "Absolute frame counter."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "absolute_frame_time",
            get_absolute_frame_time,
            "Non-paused frametime"
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "current_time",
            get_current_time,
            "Current time."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "frame_time",
            get_frame_time,
            "Time spent on last server or client frame (has nothing to do with think intervals)."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "max_clients",
            get_max_clients,
            "Current maxplayers setting."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "tick_count",
            get_tick_count,
            "Simulation ticks - does not increase when game is paused"
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "interval_per_tick",
            get_interval_per_tick,
            "Simulation tick interval"
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "interpolation_amount",
            get_interpolation_amount,
            "interpolation amount ( client-only ) based on fraction of next tick which has elapsed"
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "simulation_ticks_this_frame",
            get_simulation_ticks_this_frame,
            "Simulation ticks of this frame."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "network_protocol",
            get_network_protocol,
            "Network protocol."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "map_name",
            get_map_name,
            "Current map name."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "map_version",
            get_map_version,
            "Current map version."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "start_spot",
            get_start_spot,
            "Start spot name."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "load_type",
            get_load_type,
            "How the current map was loaded."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "map_load_failed",
            get_map_load_failed,
            "Failed to load map?"
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "deathmatch",
            get_deathmatch,
            "Deathmatch enabled?"
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "coop",
            get_coop,
            "Coop enabled?"
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "teamplay",
            get_teamplay,
            "Teamplay enabled?"
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "max_entities",
            get_max_entities,
            "Maximum number of entities."
        )
        
#if SOURCE_ENGINE == 3
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "map_group_name",
            get_map_group_name,
            "Current map group name."
        )
        
        CLASS_PROPERTY_READ_ONLY(CGlobals,
            "server_count",
            get_server_count,
            "Server count."
        )
    
        .add_property(
            "edicts",
            make_function(&CGlobals::get_edicts, reference_existing_object_policy()),
            "World edict?"
        )
#endif
    BOOST_END_CLASS()

	BOOST_FUNCTION(get_globals,
		"Returns the CGlobals singleton.",
		reference_existing_object_policy()
	);
}
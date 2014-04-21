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
#include "modules/export_main.h"
#include "modules/memory/memory_tools.h"
#include "edict.h"
#include ENGINE_INCLUDE_PATH(globals_wrap_python.h)


//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern CGlobalVars* gpGlobals;

//-----------------------------------------------------------------------------
// Exposes the globals_c module.
//-----------------------------------------------------------------------------
void export_maploadtype_t();
void export_globals();

DECLARE_SP_MODULE(_globals)
{
    export_maploadtype_t();
    export_globals();
}

void export_maploadtype_t()
{
	enum_<MapLoadType_t>("MapLoadType")
		.value("NewGame", MapLoad_NewGame)
		.value("LoadGame", MapLoad_LoadGame)
		.value("Transition", MapLoad_Transition)
		.value("Background", MapLoad_Background)
	;
}

void export_globals()
{
	GlobalsBase_Visitor(

	class_<CGlobalVarsBase>("_GlobalVarsBase", init<bool>())
		.def("is_client",
			&CGlobalVarsBase::IsClient,
			"Returns True if the game is a client."
		)

		.def("get_network_base",
			&CGlobalVarsBase::GetNetworkBase,
            "for encoding m_flSimulationTime, m_flAnimTime",
            args("tick", "entity")
		)

		.def_readwrite("real_time",
			&CGlobalVarsBase::realtime,
            "Absolute time."
		)

		.def_readwrite("frame_count",
			&CGlobalVarsBase::framecount,
            "Absolute frame counter."
		)

		.def_readwrite("absolute_frame_time",
			&CGlobalVarsBase::absoluteframetime,
            "Non-paused frame time."
		)

		.def_readwrite("current_time",
			&CGlobalVarsBase::curtime,
            "Current time."
		)

		.def_readwrite("frame_time",
			&CGlobalVarsBase::frametime,
            "Time spent on last server or client frame (has nothing to do with think intervals)."
		)

		.def_readwrite("max_clients",
			&CGlobalVarsBase::maxClients,
            "Current maxplayers setting."
		)

		.def_readwrite("tick_count",
			&CGlobalVarsBase::tickcount,
            "Simulation ticks - does not increase when game is paused."
		)

		.def_readwrite("interval_per_tick",
			&CGlobalVarsBase::interval_per_tick,
            "Simulation tick interval."
		)

		.def_readwrite("interpolation_amount",
			&CGlobalVarsBase::interpolation_amount,
            "interpolation amount ( client-only ) based on fraction of next tick which has elapsed."
		)

		.def_readwrite("simulated_ticks_this_frame",
			&CGlobalVarsBase::simTicksThisFrame,
            "Simulation ticks of this frame."
		)

		.def_readwrite("network_protocol",
			&CGlobalVarsBase::network_protocol,
            "Network protocol."
		)

		.NOT_IMPLEMENTED("is_remote_client")
		
	) ADD_MEM_TOOLS(CGlobalVarsBase, "_GlobalVarsBase"); // GlobalsBase_Visitor


	Globals_Visitor(

	class_< CGlobalVars, bases< CGlobalVarsBase> >("_GlobalVars", init<bool>())
		.add_property("map_name",
			make_getter(&CGlobalVars::mapname, return_value_policy<return_by_value>()),
            "Current map name."
		)

		.def_readwrite("map_version",
			&CGlobalVars::mapversion,
            "Current map version."
		)

		.add_property("start_spot",
			make_getter(&CGlobalVars::startspot, return_value_policy<return_by_value>()),
            "Start spot name."
		)

		.def_readwrite("load_type",
			&CGlobalVars::eLoadType,
            "How the current map was loaded."
		)

		.def_readwrite("map_load_failed",
			&CGlobalVars::bMapLoadFailed,
            "Failed to load map?"
		)

		.def_readwrite("is_deathmatch",
			&CGlobalVars::deathmatch,
            "Deathmatch enabled?"
		)

		.def_readwrite("is_coop",
			&CGlobalVars::coop,
            "Coop enabled?"
		)

		.def_readwrite("is_teamplay",
			&CGlobalVars::teamplay,
            "Teamplay enabled?"
		)

		.def_readwrite("max_entities",
			&CGlobalVars::maxEntities,
            "Maximum number of entities."
		)
		
		.NOT_IMPLEMENTED_ATTR("map_group_name")
		.NOT_IMPLEMENTED_ATTR("server_count")

	) ADD_MEM_TOOLS(CGlobalVars, "_GlobalVars"); // Globals_Visitor

	scope().attr("GlobalVars") = object(ptr(gpGlobals));
}

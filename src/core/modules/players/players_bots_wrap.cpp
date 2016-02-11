/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
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
// Includes.
//-----------------------------------------------------------------------------
#include "export_main.h"
using namespace boost::python;

#include "modules/memory/memory_tools.h"

#include "edict.h"
#include "public/game/server/iplayerinfo.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IBotManager* botmanager;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_botmanager(scope);
void export_botcontroller(scope);
void export_botcmd(scope);


//-----------------------------------------------------------------------------
// Declare the _players._bots module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_players, _bots)
{
	export_botmanager(_bots);
	export_botcontroller(_bots);
	export_botcmd(_bots);
}


//-----------------------------------------------------------------------------
// Exports IBotManager.
//-----------------------------------------------------------------------------
void export_botmanager(scope _bots)
{
	class_<IBotManager, boost::noncopyable>("_BotManager", no_init)
		.def("get_bot_controller",
			&IBotManager::GetBotController,
			"Returns the BotController object for the given bot edict.",
			args("edict"),
			reference_existing_object_policy()
		)

		.def("create_bot",
			&IBotManager::CreateBot,
			"Creates a new bot and spawns it into the server.",
			args("bot_name"),
			reference_existing_object_policy()
		)

		ADD_MEM_TOOLS(IBotManager)
	;

	_bots.attr("bot_manager") = object(ptr(botmanager));
}


//-----------------------------------------------------------------------------
// Exports IBotController.
//-----------------------------------------------------------------------------
void export_botcontroller(scope _bots)
{
	class_<IBotController, boost::noncopyable>("BotController", no_init)
		.def("set_abs_origin",
			&IBotController::SetAbsOrigin,
			"Sets the bot's absolute origin.",
			args("vec")
		)

		.def("set_abs_angles",
			&IBotController::SetAbsAngles,
			"Sets the bot's absolute angles.",
			args("ang")
		)

		.add_property("local_origin",
			&IBotController::GetLocalOrigin,
			&IBotController::SetLocalOrigin,
			"Returns the bot's local origin."
		)

		.add_property("local_angles",
			&IBotController::GetLocalAngles,
			&IBotController::SetLocalAngles,
			"Sets the bot's local angles."
		)

		.def("remove_all_items",
			&IBotController::RemoveAllItems,
			"Removes all items the bot is wearing.",
			args("bsuit")
		)

		.def("set_active_weapon",
			&IBotController::SetActiveWeapon,
			"Gives the bot a weapon.",
			args("weapon_name")
		)

		.def("is_eflag_set",
			&IBotController::IsEFlagSet,
			"Returns whether the given effect flag is set.",
			args("eflag")
		)

		.def("run_player_move",
			&IBotController::RunPlayerMove,
			"Fires a virtual move command to the bot.",
			args("cmd")
		)

		ADD_MEM_TOOLS(IBotController)
	;
}


//-----------------------------------------------------------------------------
// Exports CBotCmd.
//-----------------------------------------------------------------------------
void export_botcmd(scope _bots)
{
	class_<CBotCmd>("BotCmd")
		.def("reset",
			&CBotCmd::Reset,
			"Resets all values."
		)

		.def_readwrite("command_number",
			&CBotCmd::command_number,
			"For matching server and client commands for debugging."
		)

		.def_readwrite("tick_count",
			&CBotCmd::tick_count,
			"The tick the client created this command."
		)

		.def_readwrite("view_angles",
			&CBotCmd::viewangles,
			"Player instantaneous view angles."
		)

		.def_readwrite("forward_move",
			&CBotCmd::forwardmove,
			"Forward velocity."
		)

		.def_readwrite("side_move",
			&CBotCmd::sidemove,
			"Sideways velocity."
		)

		.def_readwrite("up_move",
			&CBotCmd::upmove,
			"Upward velocity."
		)

		.def_readwrite("buttons",
			&CBotCmd::buttons,
			"Button states."
		)

		.def_readwrite("impulse",
			&CBotCmd::impulse,
			"Impulse command."
		)

		.def_readwrite("weaponselect",
			&CBotCmd::weaponselect,
			"Current weapon ID."
		)

		.def_readwrite("weaponsubtype",
			&CBotCmd::weaponsubtype,
			"Current weapon ID."
		)

		.def_readwrite("random_seed",
			&CBotCmd::random_seed,
			"For shared random functions."
		)

		.def_readwrite("mousedx",
			&CBotCmd::mousedx,
			"Mouse accum in x from create move."
		)

		.def_readwrite("mousedy",
			&CBotCmd::mousedy,
			"Mouse accum in y from create move."
		)

		.def_readwrite("has_been_predicted",
			&CBotCmd::hasbeenpredicted,
			"Client only, tracks whether we've predicted this command at least once."
		)

		ADD_MEM_TOOLS(CBotCmd)
	;
}

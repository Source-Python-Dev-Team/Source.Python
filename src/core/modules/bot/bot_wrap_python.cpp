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

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "modules/export_main.h"
using namespace boost::python;

#include "edict.h"
#include "public/game/server/iplayerinfo.h"


// Externals
extern IBotManager* botmanager;

// ----------------------------------------------------------------------------
// Exposes the bot_c module
// ----------------------------------------------------------------------------
void export_botmanager();
void export_botcontroller();
void export_botcmd();

DECLARE_SP_MODULE(bot_c)
{
	export_botmanager();
	export_botcontroller();
	export_botcmd();
}

void export_botmanager()
{
	class_<IBotManager, boost::noncopyable>("BotManager", no_init)
		.def("get_bot_controller",
			&IBotManager::GetBotController,
			"Returns the BotController object for the given bot edict.",
			reference_existing_object_policy()
		)

		.def("create_bot",
			&IBotManager::CreateBot,
			"Creates a new bot and spawns it into the server.",
			reference_existing_object_policy()
		)
	;

	scope().attr("BotManager") = object(ptr(botmanager));
}

void export_botcontroller()
{
	class_<IBotController, boost::noncopyable>("BotController", no_init)
		.def("set_abs_origin",
			&IBotController::SetAbsOrigin,
			"Sets the bot's absolute origin."
		)

		.def("set_abs_angles",
			&IBotController::SetAbsAngles,
			"Sets the bot's absolute angles."
		)

		.def("set_local_origin",
			&IBotController::SetLocalOrigin,
			"Sets the bot's local origin."
		)

		.def("get_local_origin",
			&IBotController::GetLocalOrigin,
			"Returns the bot's local origin."
		)

		.def("set_local_angles",
			&IBotController::SetLocalAngles,
			"Sets the bot's local angles."
		)

		.def("get_local_angles",
			&IBotController::GetLocalAngles,
			"Returns the bot's local angles."
		)

		.def("remove_all_items",
			&IBotController::RemoveAllItems,
			"Removes all items the bot is wearing."
		)

		.def("set_active_weapon",
			&IBotController::SetActiveWeapon,
			"Gives the bot a weapon."
		)

		.def("is_eflag_set",
			&IBotController::IsEFlagSet,
			"Returns whether the given effect flag is set."
		)

		.def("run_player_move",
			&IBotController::RunPlayerMove,
			"Fires a virtual move command to the bot."
		)
	;
}

void export_botcmd()
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
	;
}
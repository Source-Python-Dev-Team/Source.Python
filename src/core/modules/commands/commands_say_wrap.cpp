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
#include "commands_say.h"
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern CSayCommandManager* GetSayCommand(const char* szName);

extern void RegisterSayFilter(PyObject* pCallable);
extern void UnregisterSayFilter(PyObject* pCallable);


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_say_command_manager(scope);


//-----------------------------------------------------------------------------
// Declare the _commands._say module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_commands, _say)
{
	export_say_command_manager(_say);

	// Helper functions...
	def("get_say_command",
		GetSayCommand,
		"Returns the SayCommandDispatcher instance for the given command",
		args("name"),
		reference_existing_object_policy()
	);

	def("register_say_filter",
		RegisterSayFilter,
		"Registers a callable to be called when clients use the say commands (say, say_team).",
		args("callable")
	);

	def("unregister_say_filter",
		UnregisterSayFilter,
		"Unregisters a say filter.",
		args("callable")
	);
}


//-----------------------------------------------------------------------------
// Expose CSayCommandManager.
//-----------------------------------------------------------------------------
void export_say_command_manager(scope _say)
{
	class_<CSayCommandManager, boost::noncopyable>("SayCommandDispatcher", no_init)
		.def("add_callback",
			&CSayCommandManager::AddCallback,
			"Adds a callback to the say command's list.",
			args("callable")
		)

		.def("remove_callback",
			&CSayCommandManager::RemoveCallback,
			"Removes a callback from the say command's list.",
			args("callable")
		)

		ADD_MEM_TOOLS(CSayCommandManager)
	;
}

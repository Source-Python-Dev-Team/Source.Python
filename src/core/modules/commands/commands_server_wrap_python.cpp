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
// Includes.
//-----------------------------------------------------------------------------
#include "commands_server_wrap.h"
#include "modules/export_main.h"
#include "utilities/wrap_macros.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern CServerCommandManager* GetServerCommand(const char* szName,
	const char* szHelpText = 0, int iFlags = 0);


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_server_command_manager();


//-----------------------------------------------------------------------------
// Overloads.
//-----------------------------------------------------------------------------
BOOST_PYTHON_FUNCTION_OVERLOADS(get_server_command_overloads, GetServerCommand, 1, 3)


//-----------------------------------------------------------------------------
// Declare the _commands._server module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_commands, _server)
{
	export_server_command_manager();

	// Helper functions...
	def("get_server_command",
		GetServerCommand,
		get_server_command_overloads("Gets the ServerCommandDispatcher instance using just the name or also the helptext and/or flags",
			args("name", "help_text", "flags")
		)[reference_existing_object_policy()]
	);
}


//-----------------------------------------------------------------------------
// Expose CServerCommandManager.
//-----------------------------------------------------------------------------
void export_server_command_manager()
{
	class_<CServerCommandManager, bases<ConCommandBase>, boost::noncopyable>("ServerCommandDispatcher", no_init)
		.def("add_callback",
			&CServerCommandManager::AddCallback,
			"Adds a callback to the server command's list.",
			args("callable")
		)

		.def("remove_callback",
			&CServerCommandManager::RemoveCallback,
			"Removes a callback from the server command's list.",
			args("callable")
		)

		ADD_MEM_TOOLS(CServerCommandManager, "ServerCommandDispatcher")
	;
}

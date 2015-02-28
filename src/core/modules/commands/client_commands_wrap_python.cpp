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
#include "client_commands_wrap.h"
#include "modules/export_main.h"
#include "utility/wrap_macros.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern CClientCommandManager* GetClientCommand(const char* szName);

extern void RegisterClientCommandFilter(PyObject* pCallable);
extern void UnregisterClientCommandFilter(PyObject* pCallable);


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_client_command_manager();


//-----------------------------------------------------------------------------
// Declare the _commands._client module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_commands, _client)
{
	export_client_command_manager();

	// Helper functions...
	def("get_client_command",
		GetClientCommand,
		"Returns the ClientCommandDispatcher instance for the given command",
		args("name"),
		reference_existing_object_policy()
	);

	def("register_client_command_filter",
		RegisterClientCommandFilter,
		"Registers a callable to be called when clients use commands.",
		args("callable")
	);

	def("unregister_client_command_filter",
		UnregisterClientCommandFilter,
		"Unregisters a client command filter.",
		args("callable")
	);
}


//-----------------------------------------------------------------------------
// Expose CClientCommandManager.
//-----------------------------------------------------------------------------
void export_client_command_manager()
{
	class_<CClientCommandManager, boost::noncopyable>("ClientCommandDispatcher", no_init)
		.def("add_callback",
			&CClientCommandManager::AddCallback,
			"Adds a callback to the client command's list.",
			args("callable")
		)

		.def("remove_callback",
			&CClientCommandManager::RemoveCallback,
			"Removes a callback from the client command's list.",
			args("callable")
		)

		ADD_MEM_TOOLS(CClientCommandManager, "ClientCommandDispatcher")
	;
}

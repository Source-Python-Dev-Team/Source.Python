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
#include "command_wrap.h"
#include "server_command_wrap.h"
#include "client_command_wrap.h"
#include "say_command_wrap.h"
#include "modules/export_main.h"
#include "utility/wrap_macros.h"

//-----------------------------------------------------------------------------
// Externs.
//-----------------------------------------------------------------------------
extern CClientCommandManager* get_client_command(const char* szName);
extern CServerCommandManager* get_server_command(const char* szName,
	const char* szHelpText = 0, int iFlags = 0);
extern CSayCommandManager* get_say_command(const char* szName);
extern void register_client_command_filter(PyObject* pCallable);
extern void unregister_client_command_filter(PyObject* pCallable);
extern void register_say_filter(PyObject* pCallable);
extern void unregister_say_filter(PyObject* pCallable);
extern void InitServerCommands();
extern void ClearAllServerCommands();
extern void RegisterSayCommands();
extern void UnregisterSayCommands();

//-----------------------------------------------------------------------------
// Initializes the Server and Say command modules.
//-----------------------------------------------------------------------------
void InitCommands()
{
	// Register the say and say_team commands
	RegisterSayCommands();

	// Register teh ConVar accessor.
	InitServerCommands();
}

//-----------------------------------------------------------------------------
// Clears all command instances.
//-----------------------------------------------------------------------------
void ClearAllCommands()
{
	// Unregisters the say and say_team commands
	UnregisterSayCommands();

	// Clear all Server Commands
	ClearAllServerCommands();
}

//-----------------------------------------------------------------------------
// Exposer functions.
//-----------------------------------------------------------------------------
void export_command();
void export_server_command();
void export_client_command();
void export_say_command();

//-----------------------------------------------------------------------------
// Overloads.
//-----------------------------------------------------------------------------
BOOST_PYTHON_FUNCTION_OVERLOADS( get_server_command_overloads, get_server_command, 1, 3)

//-----------------------------------------------------------------------------
// Exposes the command module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(command_c)
{
	export_command();
	export_server_command();
	export_client_command();
	export_say_command();
}

//-----------------------------------------------------------------------------
// Exposes the CICommand interface.
//-----------------------------------------------------------------------------
void export_command()
{
	enum_<CommandReturn>("CommandReturn")
		.value("CONTINUE", CONTINUE)
		.value("BLOCK", BLOCK)
		;

	BOOST_CLASS(CICommand)

		CLASS_METHOD(CICommand,
			get_arg_count,
			"Returns the number of arguments in the command"
		)

		CLASS_METHOD(CICommand,
			get_arg_string,
			"Returns the argument string for the command (does not include the command itself)"
		)

		CLASS_METHOD(CICommand,
			get_command_string,
			"Returns the entire command string"
		)

		CLASS_METHOD(CICommand,
			get_arg,
			"Gets the value of the argument at the given index",
			args("iIndex")
		)

	BOOST_END_CLASS()
}

//-----------------------------------------------------------------------------
// Exposes the Server Command interface.
//-----------------------------------------------------------------------------
void export_server_command()
{
	BOOST_ABSTRACT_CLASS(CServerCommandManager)

		CLASS_METHOD(CServerCommandManager,
			add_callback,
			"Adds a callback to the server command's list.",
			args("pCallable")
		)

		CLASS_METHOD(CServerCommandManager,
			remove_callback,
			"Removes a callback from the server command's list.",
			args("pCallable")
		)

	BOOST_END_CLASS()

	def("get_server_command",
		get_server_command,
		get_server_command_overloads("Gets the CServerCommandManager instance using just the name or also the helptext and/or flags",
			args("szName", "szHelpText", "iFlags")
		)[reference_existing_object_policy()]
	);
}

//-----------------------------------------------------------------------------
// Exposes the Client Command interface.
//-----------------------------------------------------------------------------
void export_client_command()
{
	BOOST_ABSTRACT_CLASS(CClientCommandManager)

		CLASS_METHOD(CClientCommandManager,
			add_callback,
			"Adds a callback to the client command's list.",
			args("pCallable")
		)

		CLASS_METHOD(CClientCommandManager,
			remove_callback,
			"Removes a callback from the client command's list.",
			args("pCallable")
		)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_client_command,
		"Returns the CClientCommandManager instance for the given command",
		args("szName"),
		reference_existing_object_policy()
	);

	BOOST_FUNCTION(register_client_command_filter,
		"Registers a callable to be called when clients use commands.",
		args("pCallable")
	);

	BOOST_FUNCTION(unregister_client_command_filter,
		"Unregisters a client command filter.",
		args("pCallable")
	);
}

//-----------------------------------------------------------------------------
// Exposes the Say Command interface.
//-----------------------------------------------------------------------------
void export_say_command()
{
	BOOST_ABSTRACT_CLASS(CSayCommandManager)

		CLASS_METHOD(CSayCommandManager,
			add_callback,
			"Adds a callback to the say command's list.",
			args("pCallable")
		)

		CLASS_METHOD(CSayCommandManager,
			remove_callback,
			"Removes a callback from the say command's list.",
			args("pCallable")
		)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_say_command,
		"Returns the CSayCommandManager instance for the given command",
		args("szName"),
		reference_existing_object_policy()
	);

	BOOST_FUNCTION(register_say_filter,
		"Registers a callable to be called when clients use the say commands (say, say_team).",
		args("pCallable")
	);

	BOOST_FUNCTION(unregister_say_filter,
		"Unregisters a say filter.",
		args("pCallable")
	);
}

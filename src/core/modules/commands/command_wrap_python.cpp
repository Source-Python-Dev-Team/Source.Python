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
extern CClientCommandManager* GetClientCommand(const char* szName);
extern CServerCommandManager* GetServerCommand(const char* szName,
	const char* szHelpText = 0, int iFlags = 0);
extern CSayCommandManager* GetSayCommand(const char* szName);

extern void RegisterClientCommandFilter(PyObject* pCallable);
extern void UnregisterClientCommandFilter(PyObject* pCallable);

extern void RegisterSayFilter(PyObject* pCallable);
extern void UnregisterSayFilter(PyObject* pCallable);

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
// Exposes the command module.
//-----------------------------------------------------------------------------
void export_command();
void export_server_command();
void export_client_command();
void export_say_command();

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
class CCommandExt
{
public:
	static const char* GetArg(CCommand command, unsigned int iIndex)
	{
		if ((int) iIndex >= command.ArgC())
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.");

		return command[iIndex];
	}
};

void export_command()
{
	enum_<CommandReturn>("CommandReturn")
		.value("CONTINUE", CONTINUE)
		.value("BLOCK", BLOCK)
	;

	class_<CCommand>("Command")
		.def("get_arg_count",
			&CCommand::ArgC,
			"Returns the number of arguments in the command"
		)

		.def("get_arg_string",
			&CCommand::ArgS,
			"Returns the argument string for the command (does not include the command itself)"
		)

		.def("get_command_string",
			&CCommand::GetCommandString,
			"Returns the entire command string"
		)

		.def("__getitem__",
			&CCommandExt::GetArg,
			"Gets the value of the argument at the given index",
			args("index")
		)

		.def("get_arg",
			&CCommandExt::GetArg,
			"Gets the value of the argument at the given index",
			args("index")
		)

		.def("get_max_command_length",
			&CCommand::MaxCommandLength
		)
		.staticmethod("get_max_command_length")
	;
}

//-----------------------------------------------------------------------------
// Exposes the Server Command interface.
//-----------------------------------------------------------------------------
BOOST_PYTHON_FUNCTION_OVERLOADS( get_server_command_overloads, GetServerCommand, 1, 3)

void export_server_command()
{
	// TODO: Rename
	class_<CServerCommandManager, boost::noncopyable>("ServerCommandManager", no_init)
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
	;

	def("get_server_command",
		GetServerCommand,
		get_server_command_overloads("Gets the ServerCommandManager instance using just the name or also the helptext and/or flags",
			args("name", "help_text", "flags")
		)[reference_existing_object_policy()]
	);
}

//-----------------------------------------------------------------------------
// Exposes the Client Command interface.
//-----------------------------------------------------------------------------
void export_client_command()
{
	// TODO: Rename
	class_<CClientCommandManager, boost::noncopyable>("ClientCommandManager", no_init)
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
	;

	def("get_client_command",
		GetClientCommand,
		"Returns the ClientCommandManager instance for the given command",
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
// Exposes the Say Command interface.
//-----------------------------------------------------------------------------
void export_say_command()
{
	// TODO: Rename
	class_<CSayCommandManager, boost::noncopyable>("SayCommandManager", no_init)
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
	;

	def("get_say_command",
		GetSayCommand,
		"Returns the SayCommandManager instance for the given command",
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

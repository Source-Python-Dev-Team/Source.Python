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
#include "commands_wrap.h"
#include "commands_server_wrap.h"
#include "commands_say_wrap.h"
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
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
// Forward declarations.
//-----------------------------------------------------------------------------
void export_command(scope);
void export_command_return(scope);
void export_concommandbase(scope);


//-----------------------------------------------------------------------------
// Declare the _commands module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_commands)
{
	export_command(_commands);
	export_command_return(_commands);
	export_concommandbase(_commands);
}


//-----------------------------------------------------------------------------
// Expose CCommand.
//-----------------------------------------------------------------------------
void export_command(scope _commands)
{
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

		ADD_MEM_TOOLS(CCommand, "Command")
	;
}


//-----------------------------------------------------------------------------
// Expose CommandReturn.
//-----------------------------------------------------------------------------
void export_command_return(scope _commands)
{
	enum_<CommandReturn>("CommandReturn")
		.value("CONTINUE", CONTINUE)
		.value("BLOCK", BLOCK)
	;
}


//-----------------------------------------------------------------------------
// Expose ConCommandBase.
//-----------------------------------------------------------------------------
void export_concommandbase(scope _commands)
{
	class_<ConCommandBase>("ConCommandBase")
		.def(init< const char*, optional< const char*, int> >())

		.def("is_command",
			&ConCommandBase::IsCommand,
			"Returns whether it's a command."
		)

		.def("is_flag_set",
			&ConCommandBase::IsFlagSet,
			"Returns whether the given flag is set or not.",
			args("flag")
		)

		.def("add_flags",
			&ConCommandBase::AddFlags,
			"Adds the given flags to the ConVar.",
			args("flag")
		)

		.def("remove_flags",
			&ConCommandBase::RemoveFlags,
			"Removes the given flags from the ConVar.",
			args("flag")
		)
		
		.def("get_flags",
			&ConCommandBase::GetFlags,
			"Returns its flags."
		)
		
		.def("get_name",
			&ConCommandBase::GetName,
			"Returns its name."
		)

		.def("get_help_text",
			&ConCommandBase::GetHelpText,
			"Returns the help text."
		)

		.def("get_next",
			GET_METHOD(ConCommandBase*, ConCommandBase, GetNext),
			"Returns the next ConCommandBase instance.",
			reference_existing_object_policy()
		)

		.def("is_registered",
			&ConCommandBase::IsRegistered,
			"Returns wheter the ConCommandBase instance is registered."
		)

		.def("get_dll_identifier",
			&ConCommandBase::GetDLLIdentifier,
			"Returns the DLL identifier."
		)

		ADD_MEM_TOOLS(ConCommandBase, "ConCommandBase")
	;
}

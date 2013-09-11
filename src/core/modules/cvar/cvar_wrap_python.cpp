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
#include "cvar_wrap.h"
#include "modules/export_main.h"

//-----------------------------------------------------------------------------
// Static singletons.
//-----------------------------------------------------------------------------
static CCvar s_ServerCvar;

//-----------------------------------------------------------------------------
// CCvar accessor.
//-----------------------------------------------------------------------------
CCvar* get_cvar_interface()
{
	return &s_ServerCvar;
}

//-----------------------------------------------------------------------------
// Exposer functions.
//-----------------------------------------------------------------------------
void export_cvar_interface();
void export_convar();
void export_concommandbase();
void export_flags();

//-----------------------------------------------------------------------------
// Exposes the cvar module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(cvar_c)
{
	export_cvar_interface();
	export_convar();
	export_concommandbase();
	export_flags();
}

//-----------------------------------------------------------------------------
// Exposes the CCvar interface.
//-----------------------------------------------------------------------------
void export_cvar_interface()
{
	BOOST_ABSTRACT_CLASS( CCvar )

		CLASS_METHOD(CCvar,
			register_con_command,
			"Registers a console command",
			args("pCommandBase")
		)

		CLASS_METHOD(CCvar,
			unregister_con_command,
			"Unregisters a console command",
			args("pCommandBase")
		)

		CLASS_METHOD(CCvar,
			find_command_base,
			"Returns a CConCommandBase instance for the given command, if it exists",
			args("szName"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CCvar,
			find_var,
			"Returns a CConVar instance for the given cvar, if it exists",
			args("szName"),
			reference_existing_object_policy()
		)

	BOOST_END_CLASS()
}

//-----------------------------------------------------------------------------
// Exposes the CConVar interface.
//-----------------------------------------------------------------------------
void export_convar()
{
	BOOST_CLASS_CONSTRUCTOR(CConVar, const char *)

		CLASS_CONSTRUCTOR(const char *, const char *, int)

		CLASS_CONSTRUCTOR(const char *, const char *, int, const char *)

		CLASS_CONSTRUCTOR(const char *, const char *, int, const char *,
			bool, float, bool, float)

		CLASS_METHOD(CConVar,
			get_help_text,
			"Returns the help text for the ConVar"
		)

		CLASS_METHOD(CConVar,
			get_name,
			"Returns the name of the ConVar"
		)

		CLASS_METHOD(CConVar,
			is_flag_set,
			"Returns whether the given flag is set or not",
			args("iFlag")
		)

		CLASS_METHOD(CConVar,
			add_flags,
			"Adds the given flags to the ConVar",
			args("iFlags")
		)

		CLASS_METHOD(CConVar,
			remove_flags,
			"Removes the given flags from the ConVar",
			args("iFlags")
		)

		CLASS_METHOD(CConVar,
			get_flags,
			"Returns all flags for the ConVar"
		)

		CLASS_METHOD(CConVar,
			is_command,
			"Returns whether the ConVar is a command"
		)

		CLASS_METHOD(CConVar,
			get_float,
			"Returns the float value of the ConVar"
		)

		CLASS_METHOD(CConVar,
			get_int,
			"Returns the integer value of the ConVar"
		)

		CLASS_METHOD(CConVar,
			get_bool,
			"Returns the boolean value of the ConVar"
		)

		CLASS_METHOD(CConVar,
			get_string,
			"Returns the string value of the ConVar"
		)

		CLASS_METHOD(CConVar,
			set_float,
			"Sets the ConVar to the given float value",
			args("flValue")
		)

		CLASS_METHOD(CConVar,
			set_int,
			"Sets the ConVar to the given integer value",
			args("iValue")
		)

		CLASS_METHOD(CConVar,
			set_bool,
			"Sets the ConVar to the given boolean value",
			args("bValue")
		)

		CLASS_METHOD(CConVar,
			set_string,
			"Sets the ConVar to the given string value",
			args("szValue")
		)

		CLASS_METHOD(CConVar,
			revert,
			"Reverts the ConVar back to its original value"
		)

		CLASS_METHOD(CConVar,
			get_default,
			"Returns the original value of the ConVar"
		)

		CLASS_METHOD(CConVar,
			has_min,
			"Returns whether the ConVar has a min value"
		)

		CLASS_METHOD(CConVar,
			has_max,
			"Returns whether the ConVar has a max value"
		)

		CLASS_METHOD(CConVar,
			get_min_value,
			"Returns the minimum float value for the ConVar"
		)

		CLASS_METHOD(CConVar,
			get_max_value,
			"Returns the maximum float value for the ConVar"
		)

	BOOST_END_CLASS()
}

//-----------------------------------------------------------------------------
// Exposes the CConCommandBase interface.
//-----------------------------------------------------------------------------
void export_concommandbase()
{
	BOOST_CLASS_CONSTRUCTOR(CConCommandBase, const char *, const char *, int)

		CLASS_METHOD(CConCommandBase,
			is_command,
			"Returns whether the command is actually a command"
		)

		CLASS_METHOD(CConCommandBase,
			is_registered,
			"Returns whether the command is registered"
		)

		CLASS_METHOD(CConCommandBase,
			is_flag_set,
			"Returns whether the given flag is set for the command",
			args("iFlag")
		)

		CLASS_METHOD(CConCommandBase,
			add_flags,
			"Adds the given flags to the command",
			args("iFlags")
		)

		CLASS_METHOD(CConCommandBase,
			remove_flags,
			"Removes the given flags from the command",
			args("iFlags")
		)

		CLASS_METHOD(CConCommandBase,
			get_flags,
			"Returns all flags for the command"
		)

		CLASS_METHOD(CConCommandBase,
			get_name,
			"Returns the command's name"
		)

		CLASS_METHOD(CConCommandBase,
			get_help_text,
			"Returns the help text for the command"
		)

	BOOST_END_CLASS()
}

void export_flags()
{
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_NONE", FCVAR_NONE);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_UNREGISTERED", FCVAR_UNREGISTERED);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_DEVELOPMENTONLY", FCVAR_DEVELOPMENTONLY);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_GAMEDLL", FCVAR_GAMEDLL);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_CLIENTDLL", FCVAR_CLIENTDLL);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_HIDDEN", FCVAR_HIDDEN);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_PROTECTED", FCVAR_PROTECTED);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_SPONLY", FCVAR_SPONLY);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_ARCHIVE", FCVAR_ARCHIVE);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_NOTIFY", FCVAR_NOTIFY);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_USERINFO", FCVAR_USERINFO);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_PRINTABLEONLY", FCVAR_PRINTABLEONLY);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_UNLOGGED", FCVAR_UNLOGGED);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_NEVER_AS_STRING", FCVAR_NEVER_AS_STRING);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_REPLICATED", FCVAR_REPLICATED);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_CHEAT", FCVAR_CHEAT);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_DEMO", FCVAR_DEMO);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_DONTRECORD", FCVAR_DONTRECORD);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_NOT_CONNECTED", FCVAR_NOT_CONNECTED);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_ARCHIVE_XBOX", FCVAR_ARCHIVE_XBOX);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_SERVER_CAN_EXECUTE", FCVAR_SERVER_CAN_EXECUTE);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_SERVER_CANNOT_QUERY", FCVAR_SERVER_CANNOT_QUERY);
	BOOST_GLOBAL_ATTRIBUTE("FCVAR_CLIENTCMD_CAN_EXECUTE", FCVAR_CLIENTCMD_CAN_EXECUTE);
}

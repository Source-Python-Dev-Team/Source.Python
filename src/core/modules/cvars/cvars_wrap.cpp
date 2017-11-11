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
// This is required for accessing m_nFlags without patching convar.h
#define private public

#include "export_main.h"
#include "modules/memory/memory_tools.h"
#include "icvar.h"
#include "convar.h"
#include "cvars.h"

#include ENGINE_INCLUDE_PATH(cvars.h)


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern ICvar* g_pCVar;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_cvar_interface(scope);
void export_convar_interface(scope);
void export_convar(scope);
void export_convar_flags(scope);


//-----------------------------------------------------------------------------
// Declare the _cvars module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_cvars)
{
	export_cvar_interface(_cvars);
	export_convar_interface(_cvars);
	export_convar(_cvars);
	export_convar_flags(_cvars);
}


//-----------------------------------------------------------------------------
// Exports ICvar.
//-----------------------------------------------------------------------------
void export_cvar_interface(scope _cvars)
{
	class_<ICvar, boost::noncopyable>("_Cvar", no_init)
		.def("register_base",
			&ICvar::RegisterConCommand,
			"Registers a console command.",
			args("con_command")
		)

		.def("unregister_base",
			&ICvar::UnregisterConCommand,
			"Unregisters a console command.",
			args("con_command")
		)

		.def("find_base",
			GET_METHOD(ConCommandBase *, ICvar, FindCommandBase, const char *),
			"Returns a ConCommandBase instance for the given command or variable, if it exists.",
			args("name"),
			reference_existing_object_policy()
		)

		.def("find_command",
			GET_METHOD(ConCommand*, ICvar, FindCommand, const char *),
			"Returns a ConCommand instance for the given command, if it exists.",
			args("name"),
			reference_existing_object_policy()
		)

		.def("find_var",
			GET_METHOD(ConVar*, ICvar, FindVar, const char *),
			"Returns a ConVar instance for the given variable, if it exists.",
			args("name"),
			reference_existing_object_policy()
		)

		.def("call_global_change_callbacks",
			&ICvar::CallGlobalChangeCallbacks,
			"Calls global change callbacks.",
			args("cvar", "old_string", "old_float")
		)

		.add_property("commands",
			make_function(
				ICVarExt::GetCommands,
				reference_existing_object_policy()
			),
			"Get first ConCommandBase to allow iteration."
		)

		ADD_MEM_TOOLS(ICvar)
	;

	BEGIN_CLASS_INFO(ICvar)
		FUNCTION_INFO(RegisterConCommand)
		FUNCTION_INFO(UnregisterConCommand)
		BEGIN_FUNCTION_INFO_LIST("FindCommandBase")
			FUNCTION_INFO_OVERLOAD(ConCommandBase*, FindCommandBase, const char *)
			FUNCTION_INFO_CONST_OVERLOAD(const ConCommandBase*, FindCommandBase, const char *)
		END_FUNCTION_INFO_LIST()
		BEGIN_FUNCTION_INFO_LIST("FindVar")
			FUNCTION_INFO_OVERLOAD(ConVar*, FindVar, const char *)
			FUNCTION_INFO_CONST_OVERLOAD(const ConVar*, FindVar, const char *)
		END_FUNCTION_INFO_LIST()
		FUNCTION_INFO(CallGlobalChangeCallbacks)
	END_CLASS_INFO()

	_cvars.attr("cvar") = object(ptr(g_pCVar));
}


//-----------------------------------------------------------------------------
// Exports IConVar.
//-----------------------------------------------------------------------------
void export_convar_interface(scope _cvars)
{
	class_<IConVar, boost::noncopyable>("_IConVar", no_init)

		.def("set_string",
			GET_METHOD(void, IConVar, SetValue, const char*),
			args("value")
		)

		.def("set_float",
			GET_METHOD(void, IConVar, SetValue, float),
			args("value")
		)

		.def("set_int",
			GET_METHOD(void, IConVar, SetValue, int),
			args("value")
		)

		.add_property("name",
			&IConVar::GetName,
			"Returns its name."
		)

		.def("is_flag_set",
			&IConVar::IsFlagSet,
			"Returns whether the given flag is set."
		)

		ADD_MEM_TOOLS(IConVar)
	;

	BEGIN_CLASS_INFO(IConVar)
		BEGIN_FUNCTION_INFO_LIST("SetValue")
			FUNCTION_INFO_OVERLOAD(void, SetValue, const char*)
			FUNCTION_INFO_OVERLOAD(void, SetValue, float)
			FUNCTION_INFO_OVERLOAD(void, SetValue, int)
		END_FUNCTION_INFO_LIST()
		FUNCTION_INFO(GetName)
		FUNCTION_INFO(IsFlagSet)
	END_CLASS_INFO()
}


//-----------------------------------------------------------------------------
// Exports ConVar.
//-----------------------------------------------------------------------------
void export_convar(scope _cvars)
{
	class_<ConVar, boost::shared_ptr<ConVar>, bases<ConCommandBase, IConVar>, boost::noncopyable>("ConVar", no_init)

		.def("__init__",
			make_constructor(&ConVarExt::__init__,
				default_call_policies(),
				(arg("name"), arg("value")="", arg("description")="", arg("flags")=0,
				arg("min_value")=object(), arg("max_value")=object())
			),
			"Called when a server-var is initilized.\n"
			"\n"
			"If the ConVar already exists, all other parameters\n"
			"except ``name`` are inconsequential.\n"
			"\n"
			":param str name: The name of the ConVar.\n"
			":param str value: The initial value of the\n"
			"	ConVar if it doesn't already exist.\n"
			":param str description: The description of the ConVar.\n"
			":param ConVarFlags flags: The initial flags of the\n"
			"	ConVar if it doesn't already exist.\n"
			":param float min_value: The minimum value allowed for the ConVar.\n"
			":param float max_value: The maximum value allowed for the ConVar.\n"
		)

		.def("get_float",
			&ConVar::GetFloat,
			"Returns the value as a float."
		)

		.def("get_int",
			&ConVar::GetInt,
			"Returns the value as an int."
		)

		.def("get_bool",
			&ConVar::GetBool,
			"Returns the value as a bool."
		)

		.def("get_string",
			&ConVar::GetString,
			"Returns the value as a string."
		)

		.def("revert",
			&ConVar::Revert,
			"Resets to default value."
		)

		.add_property("default",
			&ConVar::GetDefault,
			"Returns the default value."
		)

		.def("has_min",
			&ConVarExt::HasMin,
			"Returns wether the ConVar has a minimum value."
		)

		.def("has_max",
			&ConVarExt::HasMax,
			"Returns wether the ConVar has a maximum value."
		)

		.add_property("min",
			&ConVarExt::GetMin,
			"Returns the minimum value."
		)

		.add_property("max",
			&ConVarExt::GetMax,
			"Returns the maximum value."
		)

		.def("set_bool",
			&ConVarExt::SetValue,
			"Sets a bool value.",
			args("value")
		)

		.def("make_public",
			&ConVarExt::MakePublic,
			"Set the notify flag and makes the cvar public."
		)

		.def("remove_public",
			&ConVarExt::RemovePublic,
			"Remove the notify flag and makes the cvar no longer public."
		)
			
		// Special methods...
		.def("__float__",
			&ConVar::GetFloat,
			"Returns the value as a float."
		)

		.def("__int__",
			&ConVar::GetInt,
			"Returns the value as an int."
		)

		.def("__bool__",
			&ConVar::GetBool,
			"Returns the value as a bool."
		)

		.def("__str__",
			&ConVar::GetString,
			"Returns the value as a string."
		)

		ADD_MEM_TOOLS(ConVar)
	;

	BEGIN_CLASS_INFO(ConVar)
		FUNCTION_INFO(GetFloat)
		FUNCTION_INFO(GetInt)
		FUNCTION_INFO(GetBool)
		FUNCTION_INFO(GetString)
		FUNCTION_INFO(Revert)
		FUNCTION_INFO(GetDefault)
		FUNCTION_INFO(GetMin)
		FUNCTION_INFO(GetMax)
		BEGIN_FUNCTION_INFO_LIST("SetValue")
			FUNCTION_INFO_OVERLOAD(void, SetValue, const char*)
			FUNCTION_INFO_OVERLOAD(void, SetValue, float)
			FUNCTION_INFO_OVERLOAD(void, SetValue, int)
		END_FUNCTION_INFO_LIST()
	END_CLASS_INFO()
}


//-----------------------------------------------------------------------------
// Exports ConVarFlags.
//-----------------------------------------------------------------------------
void export_convar_flags(scope _cvars)
{
	_cvars.attr("FCVAR_NONE") = FCVAR_NONE;
	_cvars.attr("FCVAR_UNREGISTERED") = FCVAR_UNREGISTERED;
	_cvars.attr("FCVAR_DEVELOPMENTONLY") = FCVAR_DEVELOPMENTONLY;
	_cvars.attr("FCVAR_GAMEDLL") = FCVAR_GAMEDLL;
	_cvars.attr("FCVAR_CLIENTDLL") = FCVAR_CLIENTDLL;
	_cvars.attr("FCVAR_HIDDEN") = FCVAR_HIDDEN;
	_cvars.attr("FCVAR_PROTECTED") = FCVAR_PROTECTED;
	_cvars.attr("FCVAR_SPONLY") = FCVAR_SPONLY;
	_cvars.attr("FCVAR_ARCHIVE") = FCVAR_ARCHIVE;
	_cvars.attr("FCVAR_NOTIFY") = FCVAR_NOTIFY;
	_cvars.attr("FCVAR_USERINFO") = FCVAR_USERINFO;
	_cvars.attr("FCVAR_PRINTABLEONLY") = FCVAR_PRINTABLEONLY;
	_cvars.attr("FCVAR_UNLOGGED") = FCVAR_UNLOGGED;
	_cvars.attr("FCVAR_NEVER_AS_STRING") = FCVAR_NEVER_AS_STRING;
	_cvars.attr("FCVAR_REPLICATED") = FCVAR_REPLICATED;
	_cvars.attr("FCVAR_CHEAT") = FCVAR_CHEAT;
	_cvars.attr("FCVAR_DEMO") = FCVAR_DEMO;
	_cvars.attr("FCVAR_DONTRECORD") = FCVAR_DONTRECORD;
	_cvars.attr("FCVAR_NOT_CONNECTED") = FCVAR_NOT_CONNECTED;
	_cvars.attr("FCVAR_ARCHIVE_XBOX") = FCVAR_ARCHIVE_XBOX;
	_cvars.attr("FCVAR_SERVER_CAN_EXECUTE") = FCVAR_SERVER_CAN_EXECUTE;
	_cvars.attr("FCVAR_SERVER_CANNOT_QUERY") = FCVAR_SERVER_CANNOT_QUERY;
	_cvars.attr("FCVAR_CLIENTCMD_CAN_EXECUTE") = FCVAR_CLIENTCMD_CAN_EXECUTE;
}

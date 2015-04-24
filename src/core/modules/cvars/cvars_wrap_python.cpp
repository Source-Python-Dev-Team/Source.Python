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
#include "icvar.h"
#include "convar.h"
#include "export_main.h"
#include "modules/memory/memory_tools.h"

#include ENGINE_INCLUDE_PATH(cvars_wrap.h)


//-----------------------------------------------------------------------------
// External variables
//-----------------------------------------------------------------------------
extern ICvar* g_pCVar;


//-----------------------------------------------------------------------------
// Exposes the cvar module.
//-----------------------------------------------------------------------------
void export_cvar_interface(scope);
void export_convar(scope);
void export_flags(scope);

DECLARE_SP_MODULE(_cvars)
{
	export_cvar_interface(_cvars);
	export_convar(_cvars);
	export_flags(_cvars);
}

//-----------------------------------------------------------------------------
// Exposes the Cvar interface.
//-----------------------------------------------------------------------------
void export_cvar_interface(scope _cvars)
{
	class_<ICvar, boost::noncopyable>("_Cvar", no_init)
		.def("register_con_command",
			&ICvar::RegisterConCommand,
			"Registers a console command.",
			args("con_command")
		)

		.def("unregister_con_command",
			&ICvar::UnregisterConCommand,
			"Unregisters a console command.",
			args("con_command")
		)

		.def("find_command_base",
			GET_METHOD(ConCommandBase *, ICvar, FindCommandBase, const char *),
			"Returns a ConCommandBase instance for the given command, if it exists",
			args("name"),
			reference_existing_object_policy()
		)

		.def("find_var",
			GET_METHOD(ConVar*, ICvar, FindVar, const char *),
			"Returns a ConVar instance for the given cvar, if it exists",
			args("name"),
			reference_existing_object_policy()
		)

		.def("call_global_change_callbacks",
			&ICvar::CallGlobalChangeCallbacks,
			"Calls global change callbacks.",
			args("cvar", "old_string", "old_float")
		)

		.def("get_commands",
			ICVarExt::GetCommands,
			"Get first ConCommandBase to allow iteration.",
			reference_existing_object_policy()
		)

		ADD_MEM_TOOLS(ICvar, "_Cvar")
	;

	_cvars.attr("cvar") = object(ptr(g_pCVar));
}

//-----------------------------------------------------------------------------
// Exposes the CConVar interface.
//-----------------------------------------------------------------------------
class ConVarExt
{
public:
	static void Deleter(ConVar *pConVar)
	{
		// Do nothing...
	}

	static boost::shared_ptr<ConVar> __init__(const char *szName, const char *szDefaultValue, int flags,
		const char *szHelpString, bool bMin, float fMin, bool bMax, float fMax)
	{
		ConVar *pConVar = g_pCVar->FindVar(szName);
		if (!pConVar)
		{
			return boost::shared_ptr<ConVar>(new ConVar(strdup(szName), szDefaultValue, flags,
				strdup(szHelpString), bMin, fMin, bMax, fMax), &Deleter);
		}
		return boost::shared_ptr<ConVar>(pConVar, &Deleter);
	}

	static bool HasMin(ConVar* pConVar)
	{
		float fMin;
		return pConVar->GetMin(fMin);
	}

	static bool HasMax(ConVar* pConVar)
	{
		float fMax;
		return pConVar->GetMax(fMax);
	}

	static float GetMin(ConVar* pConVar)
	{
		float fMin;
		pConVar->GetMin(fMin);
		return fMin;
	}

	static bool GetMax(ConVar* pConVar)
	{
		float fMax;
		pConVar->GetMax(fMax);
		return fMax;
	}

	static void SetValue(ConVar* pConVar, bool bValue)
	{
		pConVar->SetValue(bValue);
	}
};

void export_convar(scope _cvars)
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

		.def("get_name",
			&IConVar::GetName,
			"Returns its name."
		)

		.def("is_flag_set",
			&IConVar::IsFlagSet,
			"Returns whether the given flag is set."
		)

		ADD_MEM_TOOLS(IConVar, "_IConVar")
	;
	
	class_<ConVar, boost::shared_ptr<ConVar>, bases<ConCommandBase, IConVar>, boost::noncopyable>("_ConVar", no_init)
		// We have to overload __init__. Otherwise we would get an error because of "no_init"
		.def("__init__",
			make_constructor(&ConVarExt::__init__,
				default_call_policies(),
				("szName", arg("szDefaultValue")="", arg("flags")=0, arg("szHelpString")="", arg("bMin")=false,
					arg("fMin")=0.0, arg("bMax")=false, arg("fMax")=0.0)
			),
			"Creates a new server variable. If it already exists, the existing one will be returned."
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

		.def("get_default",
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

		.def("get_min",
			&ConVarExt::GetMin,
			"Returns the minimum value."
		)

		.def("get_max",
			&ConVarExt::GetMax,
			"Returns the maximum value."
		)

		.def("set_bool",
			&ConVarExt::SetValue,
			"Sets a bool value.",
			args("value")
		)

		ADD_MEM_TOOLS(ConVar, "_ConVar")
	;
}

void export_flags(scope _cvars)
{
	_cvars.attr("FCVAR_NONE")                  = FCVAR_NONE;
	_cvars.attr("FCVAR_UNREGISTERED")          = FCVAR_UNREGISTERED;
	_cvars.attr("FCVAR_DEVELOPMENTONLY")       = FCVAR_DEVELOPMENTONLY;
	_cvars.attr("FCVAR_GAMEDLL")               = FCVAR_GAMEDLL;
	_cvars.attr("FCVAR_CLIENTDLL")             = FCVAR_CLIENTDLL;
	_cvars.attr("FCVAR_HIDDEN")                = FCVAR_HIDDEN;
	_cvars.attr("FCVAR_PROTECTED")             = FCVAR_PROTECTED;
	_cvars.attr("FCVAR_SPONLY")                = FCVAR_SPONLY;
	_cvars.attr("FCVAR_ARCHIVE")               = FCVAR_ARCHIVE;
	_cvars.attr("FCVAR_NOTIFY")                = FCVAR_NOTIFY;
	_cvars.attr("FCVAR_USERINFO")              = FCVAR_USERINFO;
	_cvars.attr("FCVAR_PRINTABLEONLY")         = FCVAR_PRINTABLEONLY;
	_cvars.attr("FCVAR_UNLOGGED")              = FCVAR_UNLOGGED;
	_cvars.attr("FCVAR_NEVER_AS_STRING")       = FCVAR_NEVER_AS_STRING;
	_cvars.attr("FCVAR_REPLICATED")            = FCVAR_REPLICATED;
	_cvars.attr("FCVAR_CHEAT")                 = FCVAR_CHEAT;
	_cvars.attr("FCVAR_DEMO")                  = FCVAR_DEMO;
	_cvars.attr("FCVAR_DONTRECORD")            = FCVAR_DONTRECORD;
	_cvars.attr("FCVAR_NOT_CONNECTED")         = FCVAR_NOT_CONNECTED;
	_cvars.attr("FCVAR_ARCHIVE_XBOX")          = FCVAR_ARCHIVE_XBOX;
	_cvars.attr("FCVAR_SERVER_CAN_EXECUTE")    = FCVAR_SERVER_CAN_EXECUTE;
	_cvars.attr("FCVAR_SERVER_CANNOT_QUERY")   = FCVAR_SERVER_CANNOT_QUERY;
	_cvars.attr("FCVAR_CLIENTCMD_CAN_EXECUTE") = FCVAR_CLIENTCMD_CAN_EXECUTE;
}

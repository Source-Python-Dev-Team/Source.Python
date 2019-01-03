/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2016 Source Python Development Team.  All rights reserved.
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
// Source.Python
#include "engines_server.h"
#include "utilities/wrap_macros.h"

// SDK
#include "convar.h"
#include "eiface.h"


//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern IVEngineServer* engine;


//-----------------------------------------------------------------------------
// Classes
//-----------------------------------------------------------------------------
tuple IServerExt::GetNetStats(IServer* pServer)
{
	float x, y;
	pServer->GetNetStats(x, y);
	return make_tuple(x, y);
}


//-----------------------------------------------------------------------------
// Commands to execute server commands
//-----------------------------------------------------------------------------
void prepare_command(tuple args, dict kwargs, ConCommand** pCommandOut, std::string* szCommandOut)
{
	if (kwargs)
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Function does not accept keywords.")

	const char* command = extract<const char*>(args[0]);
	ConCommand* pCommand = cvar->FindCommand(command);
	if (!pCommand)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find command '%s'.", command);

	*pCommandOut = pCommand;

	*szCommandOut = "";
	for(int i=0; i < len(args); ++i)
	{
		const char* temp = extract<const char*>(str(args[i]));
		*szCommandOut += temp;
		*szCommandOut += " ";
	}
}

object execute_server_command(tuple args, dict kwargs)
{
	std::string szCommand;
	ConCommand* pCommand;
	prepare_command(args, kwargs, &pCommand, &szCommand);

	CCommand c;
	if (!c.Tokenize(szCommand.c_str()))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to tokenize '%s'.", szCommand.c_str())

	pCommand->Dispatch(c);
	return object();
}

object queue_server_command(tuple args, dict kwargs)
{
	std::string szCommand;
	ConCommand* pCommand;
	prepare_command(args, kwargs, &pCommand, &szCommand);
	szCommand += ";";
	engine->ServerCommand(szCommand.c_str());
	return object();
}

void queue_command_string(const char* szCommand)
{
	std::string command = szCommand;
	command += ";";
	engine->ServerCommand(command.c_str());
}

object insert_server_command(tuple args, dict kwargs)
{
	std::string szCommand;
	ConCommand* pCommand;
	prepare_command(args, kwargs, &pCommand, &szCommand);
	szCommand += ";";
	engine->InsertServerCommand(szCommand.c_str());
	return object();
}

void insert_command_string(const char* szCommand)
{
	std::string command = szCommand;
	command += ";";
	engine->InsertServerCommand(command.c_str());
}

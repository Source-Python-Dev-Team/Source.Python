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

#include <iostream>
#include <string>
#include "commands_say.h"
#include "commands_server.h"
#include "commands.h"
#include "utilities/call_python.h"
#include "boost/python/call.hpp"
#include "boost/shared_array.hpp"
#include "boost/unordered_map.hpp"
#include "sp_main.h"
#include "modules/listeners/listeners_manager.h"
#include "convar.h"


//-----------------------------------------------------------------------------
// Global say command mapping.
//-----------------------------------------------------------------------------
typedef boost::unordered_map<std::string, CSayCommandManager*> SayCommandMap;
SayCommandMap g_SayCommandMap;

//-----------------------------------------------------------------------------
// Externs.
//-----------------------------------------------------------------------------
extern CSourcePython g_SourcePythonPlugin;
extern ICvar* g_pCVar;

//-----------------------------------------------------------------------------
// Static singletons.
//-----------------------------------------------------------------------------
static CListenerManager s_SayFilters;
static BaseSayCommand s_SayCommand;

//-----------------------------------------------------------------------------
// Helper function to get the current command index
//-----------------------------------------------------------------------------
int GetCommandIndex()
{
	return g_SourcePythonPlugin.GetCommandIndex() + 1;
}

//-----------------------------------------------------------------------------
// Registers the say and say_team commands.
//-----------------------------------------------------------------------------
void BaseSayCommand::RegisterCommands()
{
	// Register the say command.
	m_pSayCommand = SayConCommand::CreateCommand("say", "Say command wrapper", 0);

	// Register the say_team command.
	m_pTeamSayCommand = SayConCommand::CreateCommand("say_team", "Team Say command wrapper", 0);
}

//-----------------------------------------------------------------------------
// Unregisters the say and say_team commands.
//-----------------------------------------------------------------------------
void BaseSayCommand::UnregisterCommands()
{
	// Unregister the say command.
	delete m_pSayCommand;

	// Unregister the say_team command.
	delete m_pTeamSayCommand;
}

//-----------------------------------------------------------------------------
// Returns a CSayCommandManager for the given command name.
//-----------------------------------------------------------------------------
CSayCommandManager* GetSayCommand(const char* szName)
{
	CSayCommandManager* manager = NULL;
	SayCommandMap::iterator iter;
	if (!find_manager<SayCommandMap, SayCommandMap::iterator>(g_SayCommandMap, szName, iter))
	{
		manager = new CSayCommandManager(szName);
		g_SayCommandMap.insert(std::make_pair(manager->m_Name, manager));
	}
	else
	{
		manager = iter->second;
	}
	return manager;
}

//-----------------------------------------------------------------------------
// Removes a CSayCommandManager instance for the given name.
//-----------------------------------------------------------------------------
void RemoveCSayCommandManager(const char* szName)
{
	SayCommandMap::iterator iter;
	if (find_manager<SayCommandMap, SayCommandMap::iterator>(g_SayCommandMap, szName, iter))
	{
		delete iter->second;
		g_SayCommandMap.erase(iter);
	}
}

//-----------------------------------------------------------------------------
// Register function for say filter.
//-----------------------------------------------------------------------------
void RegisterSayFilter(PyObject* pCallable)
{
	s_SayFilters.RegisterListener(pCallable);
}

//-----------------------------------------------------------------------------
// Unregister function for say filter.
//-----------------------------------------------------------------------------
void UnregisterSayFilter(PyObject* pCallable)
{
	s_SayFilters.UnregisterListener(pCallable);
}

//-----------------------------------------------------------------------------
// Returns a SayConCommand instance.
//-----------------------------------------------------------------------------
SayConCommand* SayConCommand::CreateCommand(const char* szName, const char* szHelpText, int iFlags)
{
	// Copy the name and store a help text copier value
	char* szNameCopy = strdup(szName);
	char* szHelpTextCopy = NULL;

	// Find if the command already exists
	ConCommand* pConCommand = g_pCVar->FindCommand(szName);
	if( pConCommand )
	{
		// Store the current command's help text and flags
		szHelpTextCopy = strdup(pConCommand->GetHelpText());
		iFlags = pConCommand->m_nFlags;

		// Unregister the old command
		g_pCVar->UnregisterConCommand(pConCommand);
	}
	else
	{
		// Store the given help text
		szHelpTextCopy = strdup(szHelpText);
	}

	// Return a new instance of ConCommand
	return new SayConCommand(pConCommand, szNameCopy, szHelpTextCopy, iFlags);
}

//-----------------------------------------------------------------------------
// SayConCommand instance.
//-----------------------------------------------------------------------------
SayConCommand::SayConCommand(ConCommand* pConCommand,
		const char* szName, const char* szHelpText, int iFlags):
	ConCommand(szName, (FnCommandCallback_t)NULL, szHelpText, iFlags),
	m_pOldCommand(pConCommand)
{
	m_Name = szName;
}

//-----------------------------------------------------------------------------
// SayConCommand destructor.
//-----------------------------------------------------------------------------
SayConCommand::~SayConCommand()
{
	// Get the ConCommand instance
	ConCommand* pConCommand = g_pCVar->FindCommand(m_Name);

	// Unregister the ConCommand
	g_pCVar->UnregisterConCommand(pConCommand);

	// Was the command registered before we registered it?
	if( m_pOldCommand )
	{
		// Re-register the old command instance
		g_pCVar->RegisterConCommand(m_pOldCommand);
	}
}

//-----------------------------------------------------------------------------
// SayConCommand Init override.
//-----------------------------------------------------------------------------
void SayConCommand::Init()
{
	ConCommand::Init();
}

//-----------------------------------------------------------------------------
// Dispatches the say and say_team commands.
//-----------------------------------------------------------------------------
void SayConCommand::Dispatch( const CCommand& command )
{
	// This is the case if just "say" or "say_team" was entered into the server
	if (command.ArgC() == 1)
		return;

	// Get the index of the player that used the command
	int iIndex = GetCommandIndex();
	
	// Get whether the command was say or say_team
	bool bTeamOnly = strcmp(command.Arg(0), "say_team") == 0;

	// Create a new CCommand object that does not contain the first argument
	// (say or say_team) and is properly splitted
	CCommand stripped_command = CCommand();
	char szTempCommand[512];
	strcpy(szTempCommand, command.ArgS());
	char* szCommand = szTempCommand;

	// Remove quotes (if existant), so the arguments are not recognized as a
	// single argument.
	int iLastCharPos = strlen(szCommand) - 1;
	if (szCommand[0] == '"' && szCommand[iLastCharPos] == '"') {
		szCommand[iLastCharPos] = '\0';
		szCommand++;
	}

	if (!stripped_command.Tokenize(szCommand)) {
		PythonLog(0, "Failed to tokenize '%s'.", command.GetCommandString());
		return;
	}

	bool block = false;

	// Loop through all registered Say Filter callbacks
	CListenerManager* mngr = &s_SayFilters;
	FOREACH_CALLBACK_WITH_MNGR(
		mngr,
		object returnValue,
		if( !returnValue.is_none() && extract<int>(returnValue) == (int) BLOCK)
		{
			block = true;
		},
		boost::ref(stripped_command), iIndex, bTeamOnly
	)

	if (block)
		return;

	block = false;
	SayCommandMap::iterator iter;
	if (find_manager<SayCommandMap, SayCommandMap::iterator>(g_SayCommandMap, stripped_command[0], iter))
	{
		if(iter->second->Dispatch(stripped_command, iIndex, bTeamOnly) == BLOCK)
		{
			block = true;
		}
	}

	if (block)
		return;
	
	// Was the command previously registered?
	if( m_pOldCommand )
	{
		// Call the old command callback
		m_pOldCommand->Dispatch(command);
	}
}

//-----------------------------------------------------------------------------
// CSayCommandManager constructor.
//-----------------------------------------------------------------------------
CSayCommandManager::CSayCommandManager(const char* szName)
{
	m_Name = strdup(szName);
}

//-----------------------------------------------------------------------------
// CSayCommandManager destructor.
//-----------------------------------------------------------------------------
CSayCommandManager::~CSayCommandManager()
{
	free((char*)m_Name);
}

//-----------------------------------------------------------------------------
// Adds a callable to a CSayCommandManager instance.
//-----------------------------------------------------------------------------
void CSayCommandManager::AddCallback( PyObject* pCallable )
{
	m_vecCallables.RegisterListener(pCallable);
}

//-----------------------------------------------------------------------------
// Removes a callable from a CSayCommandManager instance.
//-----------------------------------------------------------------------------
void CSayCommandManager::RemoveCallback( PyObject* pCallable )
{
	m_vecCallables.UnregisterListener(pCallable);

	// Are there any more callbacks registered for this command?
	if( !m_vecCallables.GetCount() )
	{
		// Remove the CSayCommandManager instance
		RemoveCSayCommandManager(m_Name);
	}
}

//-----------------------------------------------------------------------------
// Dispatches the say command.
//-----------------------------------------------------------------------------
CommandReturn CSayCommandManager::Dispatch( const CCommand& command, int iIndex, bool bTeamOnly)
{
	bool block = false;

	CListenerManager* mngr = &m_vecCallables;
	FOREACH_CALLBACK_WITH_MNGR(
		mngr,
		object returnValue,
		if( !returnValue.is_none() && extract<int>(returnValue) == (int) BLOCK)
		{
			block = true;
		},
		boost::ref(command), iIndex, bTeamOnly
	)

	if (block)
		return BLOCK;

	return CONTINUE;
}

const char* CSayCommandManager::GetName()
{
	return m_Name;
}

//-----------------------------------------------------------------------------
// Registers the say and say_team commands.
//-----------------------------------------------------------------------------
void RegisterSayCommands()
{
	s_SayCommand.RegisterCommands();
}

//-----------------------------------------------------------------------------
// Unregisters the say and say_team commands.
//-----------------------------------------------------------------------------
void UnregisterSayCommands()
{
	s_SayCommand.UnregisterCommands();
}

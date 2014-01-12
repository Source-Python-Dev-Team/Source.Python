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
#include <iostream>
#include <string>
#include "convar.h"
#include "say_command_wrap.h"
#include "server_command_wrap.h"
#include "command_wrap.h"
#include "utility/call_python.h"
#include "boost/python/call.hpp"
#include "boost/shared_array.hpp"
#include "core/sp_main.h"

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
static BaseFilters s_SayFilters;
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
CSayCommandManager* get_say_command(const char* szName)
{
	// Find if the given name is a registered say command
	SayCommandMap::iterator commandMapIter = g_SayCommandMap.find(szName);
	if( commandMapIter == g_SayCommandMap.end())
	{
		// If the command is not already registered, add the name and the CSayCommandManager instance to the mapping
		g_SayCommandMap.insert(std::make_pair(szName, new CSayCommandManager(szName)));

		// Get the say command in the mapping
		commandMapIter = g_SayCommandMap.find(szName);
	}

	// Return the CSayCommandManager instance for the command
	return commandMapIter->second;
}

//-----------------------------------------------------------------------------
// Removes a CSayCommandManager instance for the given name.
//-----------------------------------------------------------------------------
void RemoveCSayCommandManager(const char* szName)
{
	// Find if the given name is a registered say command
	SayCommandMap::iterator commandMapIter = g_SayCommandMap.find(szName);
	if( commandMapIter != g_SayCommandMap.end() )
	{
		// If the command is registered, delete the CSayCommandManager instance
		//		and remove the command from the mapping
		delete commandMapIter->second;
		g_SayCommandMap.erase(commandMapIter);
	}
}

//-----------------------------------------------------------------------------
// Register function for say filter.
//-----------------------------------------------------------------------------
void register_say_filter(PyObject* pCallable)
{
	s_SayFilters.register_filter(pCallable);
}

//-----------------------------------------------------------------------------
// Unregister function for say filter.
//-----------------------------------------------------------------------------
void unregister_say_filter(PyObject* pCallable)
{
	s_SayFilters.unregister_filter(pCallable);
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
		iFlags = pConCommand->GetFlags();

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
void SayConCommand::Dispatch( const CCommand &command )
{
	// Get the index of the player that used the command
	int iIndex = GetCommandIndex();

	// Get the IPlayerInfo instance of the player
	IPlayerInfo* pPlayerInfo = playerinfomanager->GetPlayerInfo(PEntityOfEntIndex(iIndex));

	// Get whether the command was say or say_team
	bool bTeamOnly = command.Arg(0) == "say_team";

	// Get a CICommand instance for the CCommand
	CICommand* ccommand = new CICommand(&command);

	// Loop through all registered Say Filter callbacks
	for(int i = 0; i < s_SayFilters.m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = s_SayFilters.m_vecCallables[i].ptr();

			// Call the callable and store its return value
			object returnValue = CALL_PY_FUNC(pCallable, pPlayerInfo, bTeamOnly, ccommand);

			// Does the current Say Filter wish to block the command?
			if( !returnValue.is_none() && extract<int>(returnValue) == (int)BLOCK)
			{
				// Block the command
				return;
			}

		END_BOOST_PY_NORET()
	}

	// Get the name of the command used
	std::string szCommandString (command.Arg(1));

	// Copy the string to get a char instance
	char * szCopyCommandString = new char [szCommandString.length() + 1];
	std::strcpy(szCopyCommandString, szCommandString.c_str());

	// Split the command using <space> as the delimiter
	// This should be the actual Say Command
	char * szCommand = std::strtok(szCopyCommandString, " ");

	// Find if the command is registered
	SayCommandMap::iterator commandMapIter = g_SayCommandMap.find(szCommand);
	if( commandMapIter != g_SayCommandMap.end() )
	{
		// Get the CSayCommandManager instance for the command
		CSayCommandManager* pCSayCommandManager = commandMapIter->second;

		// Call the command and see it wants to block the command
		if( pCSayCommandManager->Dispatch(pPlayerInfo, bTeamOnly, ccommand)  == BLOCK)
		{
			// Block the command
			return;
		}
	}

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
	m_Name = szName;
}

//-----------------------------------------------------------------------------
// CSayCommandManager destructor.
//-----------------------------------------------------------------------------
CSayCommandManager::~CSayCommandManager()
{
}

//-----------------------------------------------------------------------------
// Adds a callable to a CSayCommandManager instance.
//-----------------------------------------------------------------------------
void CSayCommandManager::add_callback( PyObject* pCallable )
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Is the callable already in the vector?
	if( !m_vecCallables.HasElement(oCallable) )
	{
		// Add the callable to the vector
		m_vecCallables.AddToTail(oCallable);
	}
}

//-----------------------------------------------------------------------------
// Removes a callable from a CSayCommandManager instance.
//-----------------------------------------------------------------------------
void CSayCommandManager::remove_callback( PyObject* pCallable )
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Remove the callback from the CSayCommandManager instance
	m_vecCallables.FindAndRemove(oCallable);

	// Are there any more callbacks registered for this command?
	if( !m_vecCallables.Count() )
	{
		// Remove the CSayCommandManager instance
		RemoveCSayCommandManager(m_Name);
	}
}

//-----------------------------------------------------------------------------
// Dispatches the say command.
//-----------------------------------------------------------------------------
CommandReturn CSayCommandManager::Dispatch( IPlayerInfo* pPlayerInfo, bool bTeamOnly, CICommand* ccommand )
{
	// Loop through all callables registered for the CSayCommandManager instance
	for(int i = 0; i < m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = m_vecCallables[i].ptr();

			// Call the callable and store its return value
			object returnValue = CALL_PY_FUNC(pCallable, pPlayerInfo, bTeamOnly, ccommand);

			// Does the callable wish to block the command?
			if( !returnValue.is_none() && extract<int>(returnValue) == (int)BLOCK)
			{
				// Block the command
				return BLOCK;
			}

		END_BOOST_PY_NORET()
	}

	return CONTINUE;
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

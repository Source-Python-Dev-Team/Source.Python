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
#include "client_command_wrap.h"
#include "command_wrap.h"
#include "edict.h"
#include "convar.h"
#include "engine/iserverplugin.h"
#include "modules/entities/entities_wrap.h"
#include "utility/call_python.h"
#include "boost/python/call.hpp"
#include "boost/shared_array.hpp"

//-----------------------------------------------------------------------------
// Global Client command mapping.
//-----------------------------------------------------------------------------
typedef boost::unordered_map<std::string, CClientCommandManager*> ClientCommandMap;
ClientCommandMap g_ClientCommandMap;

//-----------------------------------------------------------------------------
// Static singletons.
//-----------------------------------------------------------------------------
static BaseFilters s_ClientCommandFilters;

//-----------------------------------------------------------------------------
// Returns a CClientCommandManager for the given command name.
//-----------------------------------------------------------------------------
CClientCommandManager* get_client_command(const char* szName)
{
	// Find if the given name is a registered client command
	ClientCommandMap::iterator commandMapIter = g_ClientCommandMap.find(szName);
	if( commandMapIter == g_ClientCommandMap.end())
	{
		// If the command is not already registered, add the name and the CClientCommandManager instance to the mapping
		g_ClientCommandMap.insert(std::make_pair(szName, new CClientCommandManager(szName)));

		// Get the client command in the mapping
		commandMapIter = g_ClientCommandMap.find(szName);
	}

	// Return the CClientCommandManager instance for the command
	return commandMapIter->second;
}

//-----------------------------------------------------------------------------
// Removes a CClientCommandManager instance for the given name.
//-----------------------------------------------------------------------------
void RemoveCClientCommandManager(const char* szName)
{
	// Find if the given name is a registered client command
	ClientCommandMap::iterator commandMapIter = g_ClientCommandMap.find(szName);
	if( commandMapIter != g_ClientCommandMap.end())
	{
		// If the command is registered, delete the CClientCommandManager instance
		//		and remove the command from the mapping
		delete commandMapIter->second;
		g_ClientCommandMap.erase(commandMapIter);
	}
}

//-----------------------------------------------------------------------------
// Register function for client command filter.
//-----------------------------------------------------------------------------
void register_client_command_filter(PyObject* pCallable)
{
	s_ClientCommandFilters.register_filter(pCallable);
}

//-----------------------------------------------------------------------------
// Unregister function for client command filter.
//-----------------------------------------------------------------------------
void unregister_client_command_filter(PyObject* pCallable)
{
	s_ClientCommandFilters.unregister_filter(pCallable);
}

//-----------------------------------------------------------------------------
// Dispatches a client command.
//-----------------------------------------------------------------------------
PLUGIN_RESULT DispatchClientCommand(edict_t* pEntity, const CCommand &command)
{
	// Get the IPlayerInfo instance of the player
	IPlayerInfo* pPlayerInfo = playerinfomanager->GetPlayerInfo(pEntity);

	// Get the CICommand instance of the CCommand
	CICommand* ccommand = new CICommand(&command);

	// Loop through all registered Client Command Filters
	for(int i = 0; i < s_ClientCommandFilters.m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = s_ClientCommandFilters.m_vecCallables[i].ptr();

			// Call the callable and store its return value
			object returnValue = CALL_PY_FUNC(pCallable, pPlayerInfo, ccommand);

			// Does the Client Command Filter want to block the command?
			if( !returnValue.is_none() && extract<int>(returnValue) == (int)BLOCK)
			{
				// Block the command
				return PLUGIN_STOP;
			}

		END_BOOST_PY_NORET()
	}

	// Get the command's name
	const char* szCommand = command.Arg(0);

	// Find if the command exists in the mapping
	ClientCommandMap::iterator commandMapIter = g_ClientCommandMap.find(szCommand);
	if( commandMapIter != g_ClientCommandMap.end() )
	{
		// If the command exists, get the CClientCommandManager instance and call its Dispatch method
		CClientCommandManager* pCClientCommandManager = commandMapIter->second;

		// Does the command need to be blocked?
		if( !pCClientCommandManager->Dispatch(pPlayerInfo, ccommand))
		{
			// Block the command
			return PLUGIN_STOP;
		}
	}

	return PLUGIN_CONTINUE;
}

//-----------------------------------------------------------------------------
// CClientCommandManager constructor.
//-----------------------------------------------------------------------------
CClientCommandManager::CClientCommandManager(const char* szName)
{
	m_Name = szName;
}

//-----------------------------------------------------------------------------
// CClientCommandManager destructor.
//-----------------------------------------------------------------------------
CClientCommandManager::~CClientCommandManager()
{
}

//-----------------------------------------------------------------------------
// Adds a callable to a CClientCommandManager instance.
//-----------------------------------------------------------------------------
void CClientCommandManager::add_callback( PyObject* pCallable )
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
// Removes a callable from a CClientCommandManager instance.
//-----------------------------------------------------------------------------
void CClientCommandManager::remove_callback( PyObject* pCallable )
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Remove the callback from the CClientCommandManager instance
	m_vecCallables.FindAndRemove(oCallable);

	// Are there any more callbacks registered for this command?
	if( !m_vecCallables.Count() )
	{
		// Remove the CClientCommandManager instance
		RemoveCClientCommandManager(m_Name);
	}
}

//-----------------------------------------------------------------------------
// Calls all callables for the command when it is called on the client.
//-----------------------------------------------------------------------------
CommandReturn CClientCommandManager::Dispatch( IPlayerInfo* pPlayerInfo, CICommand* ccommand )
{
	// Loop through all callables registered for the CClientCommandManager instance
	for(int i = 0; i < m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = m_vecCallables[i].ptr();

			// Call the callable and store its return value
			object returnValue = CALL_PY_FUNC(pCallable, pPlayerInfo, ccommand);

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

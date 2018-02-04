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
// Includes
//-----------------------------------------------------------------------------
// This is required for accessing m_nFlags without patching convar.h
#define private public

#include "commands_client.h"
#include "commands.h"
#include "edict.h"
#include "convar.h"
#include "engine/iserverplugin.h"
#include "utilities/call_python.h"
#include "boost/python/call.hpp"
#include "boost/shared_array.hpp"
#include "modules/listeners/listeners_manager.h"
#include "utilities/conversions.h"


//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern IPlayerInfoManager* playerinfomanager;

//-----------------------------------------------------------------------------
// Global Client command mapping.
//-----------------------------------------------------------------------------
typedef boost::unordered_map<std::string, CClientCommandManager*> ClientCommandMap;
ClientCommandMap g_ClientCommandMap;

//-----------------------------------------------------------------------------
// Static singletons.
//-----------------------------------------------------------------------------
static CListenerManager s_ClientCommandFilters;

//-----------------------------------------------------------------------------
// Returns a CClientCommandManager for the given command name.
//-----------------------------------------------------------------------------
CClientCommandManager* GetClientCommand(const char* szName)
{
	CClientCommandManager* manager = NULL;
	ClientCommandMap::iterator iter;
	if (!find_manager<ClientCommandMap, ClientCommandMap::iterator>(g_ClientCommandMap, szName, iter))
	{
		manager = new CClientCommandManager(szName);
		g_ClientCommandMap.insert(std::make_pair(manager->m_Name, manager));
	}
	else
	{
		manager = iter->second;
	}
	return manager;
}

//-----------------------------------------------------------------------------
// Removes a CClientCommandManager instance for the given name.
//-----------------------------------------------------------------------------
void RemoveCClientCommandManager(const char* szName)
{
	ClientCommandMap::iterator iter;
	if (find_manager<ClientCommandMap, ClientCommandMap::iterator>(g_ClientCommandMap, szName, iter))
	{
		delete iter->second;
		g_ClientCommandMap.erase(iter);
	}
}

//-----------------------------------------------------------------------------
// Register function for client command filter.
//-----------------------------------------------------------------------------
void RegisterClientCommandFilter(PyObject* pCallable)
{
	s_ClientCommandFilters.RegisterListener(pCallable);
}

//-----------------------------------------------------------------------------
// Unregister function for client command filter.
//-----------------------------------------------------------------------------
void UnregisterClientCommandFilter(PyObject* pCallable)
{
	s_ClientCommandFilters.UnregisterListener(pCallable);
}

//-----------------------------------------------------------------------------
// Dispatches a client command.
//-----------------------------------------------------------------------------
PLUGIN_RESULT DispatchClientCommand(edict_t* pEntity, const CCommand &command)
{
	unsigned int iIndex;
	if (!IndexFromEdict(pEntity, iIndex))
		return PLUGIN_CONTINUE;

	// Loop through all registered Client Command Filters
	for(int i = 0; i < s_ClientCommandFilters.m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = s_ClientCommandFilters.m_vecCallables[i].ptr();

			// Call the callable and store its return value
			object returnValue = CALL_PY_FUNC(pCallable, boost::ref(command), iIndex);

			// Does the Client Command Filter want to block the command?
			if( !returnValue.is_none() && extract<int>(returnValue) == (int)BLOCK)
			{
				// Block the command
				return PLUGIN_STOP;
			}

		END_BOOST_PY_NORET()
	}

	ClientCommandMap::iterator iter;
	if (find_manager<ClientCommandMap, ClientCommandMap::iterator>(g_ClientCommandMap, command.Arg(0), iter))
	{
		if( !iter->second->Dispatch(command, iIndex))
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
	m_Name = strdup(szName);
}

//-----------------------------------------------------------------------------
// CClientCommandManager destructor.
//-----------------------------------------------------------------------------
CClientCommandManager::~CClientCommandManager()
{
	free((char*)m_Name);
}

//-----------------------------------------------------------------------------
// Adds a callable to a CClientCommandManager instance.
//-----------------------------------------------------------------------------
void CClientCommandManager::AddCallback( PyObject* pCallable )
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
void CClientCommandManager::RemoveCallback( PyObject* pCallable )
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
CommandReturn CClientCommandManager::Dispatch( const CCommand& command, int iIndex )
{
	// Loop through all callables registered for the CClientCommandManager instance
	for(int i = 0; i < m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = m_vecCallables[i].ptr();

			// Call the callable and store its return value
			object returnValue = CALL_PY_FUNC(pCallable, boost::ref(command), iIndex);

			// Does the callable wish to block the command?
			if( !returnValue.is_none() && extract<int>(returnValue) == (int) BLOCK)
			{
				// Block the command
				return BLOCK;
			}

		END_BOOST_PY_NORET()
	}

	return CONTINUE;
}

const char* CClientCommandManager::GetName()
{
	return m_Name;
}

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

#include "boost/unordered_map.hpp"
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

	bool block = false;
	
	CListenerManager* mngr = &s_ClientCommandFilters;
	FOREACH_CALLBACK_WITH_MNGR(
		mngr,
		object returnValue,
		if( !returnValue.is_none() && extract<int>(returnValue) == (int)BLOCK)
		{
			block = true;
		},
		boost::ref(command), iIndex
	)

	if (block)
		return PLUGIN_STOP;

	block = false;
	ClientCommandMap::iterator iter;
	if (find_manager<ClientCommandMap, ClientCommandMap::iterator>(g_ClientCommandMap, command.Arg(0), iter))
	{
		if( !iter->second->Dispatch(command, iIndex))
		{
			block = true;
		}
	}

	if (block)
		return PLUGIN_STOP;

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
	m_vecCallables.RegisterListener(pCallable);
}

//-----------------------------------------------------------------------------
// Removes a callable from a CClientCommandManager instance.
//-----------------------------------------------------------------------------
void CClientCommandManager::RemoveCallback( PyObject* pCallable )
{
	m_vecCallables.UnregisterListener(pCallable);

	// Are there any more callbacks registered for this command?
	if( !m_vecCallables.GetCount() )
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
	bool block = false;
	
	CListenerManager* mngr = &m_vecCallables;
	FOREACH_CALLBACK_WITH_MNGR(
		mngr,
		object returnValue,
		if( !returnValue.is_none() && extract<int>(returnValue) == (int) BLOCK)
		{
			block = true;
		},
		boost::ref(command), iIndex
	)

	if (block)
		return BLOCK;

	return CONTINUE;
}

const char* CClientCommandManager::GetName()
{
	return m_Name;
}

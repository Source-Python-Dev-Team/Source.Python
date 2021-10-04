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

#include "utilities/call_python.h"

#include "boost/unordered_map.hpp"
#include "utilities/sp_util.h"
#include "utilities/wrap_macros.h"

#include "commands.h"
#include "commands_server.h"
#include "modules/cvars/cvars.h"

//-----------------------------------------------------------------------------
// Externs.
//-----------------------------------------------------------------------------
extern ICvar* g_pCVar;

//-----------------------------------------------------------------------------
// ConVar Accessor class and registration
//
// NOTE: This is used to ensure any new ConCommands created get registered to the
//	   active ICVar instance.
//-----------------------------------------------------------------------------
class CPluginConVarAccessor : public IConCommandBaseAccessor
{
public:
	virtual bool RegisterConCommandBase(ConCommandBase* pCommand)
	{
		g_pCVar->RegisterConCommand(pCommand);
		return true;
	}
};

CPluginConVarAccessor g_ConVarAccessor;

//-----------------------------------------------------------------------------
// Registers the ConVar accessor above to take in new ConCommand requests.
//-----------------------------------------------------------------------------
void InitServerCommands()
{
	ConVar_Register(0, &g_ConVarAccessor);
}

//-----------------------------------------------------------------------------
// Global server command mapping.
//-----------------------------------------------------------------------------
typedef boost::unordered_map<std::string, CServerCommandManager*> ServerCommandMap;
ServerCommandMap g_ServerCommandMap;

//-----------------------------------------------------------------------------
// Returns a CServerCommandManager for the given command name.
//-----------------------------------------------------------------------------
CServerCommandManager* GetServerCommand(const char* szName,
	const char* szHelpText = 0, int iFlags = 0)
{
	CServerCommandManager* manager = NULL;
	ServerCommandMap::iterator iter;
	if (!find_manager<ServerCommandMap, ServerCommandMap::iterator>(g_ServerCommandMap, szName, iter))
	{
		manager = CServerCommandManager::CreateCommand(szName, szHelpText, iFlags);
		g_ServerCommandMap.insert(std::make_pair(manager->m_Name, manager));
	}
	else
	{
		manager = iter->second;
	}
	return manager;
}

//-----------------------------------------------------------------------------
// Removes a CServerCommandManager instance for the given name.
//-----------------------------------------------------------------------------
void RemoveCServerCommandManager(const char* szName)
{
	ServerCommandMap::iterator iter;
	if (find_manager<ServerCommandMap, ServerCommandMap::iterator>(g_ServerCommandMap, szName, iter))
	{
		delete iter->second;
		g_ServerCommandMap.erase(iter);
	}
}

//-----------------------------------------------------------------------------
// Returns a CServerCommandManager instance.
//-----------------------------------------------------------------------------
CServerCommandManager* CServerCommandManager::CreateCommand(const char* szName,
	const char* szHelpText, int iFlags)
{
	// Copy the name and store a help text copier value
	char* szNameCopy = strdup(szName);
	char* szHelpTextCopy = NULL;

	// FInd if the command already exists
	ConCommand* pConCommand = g_pCVar->FindCommand(szName);
	if( pConCommand )
	{
		// Store the current command's help text and flags
		szHelpTextCopy = strdup(pConCommand->GetHelpText());
		iFlags = pConCommand->m_nFlags;

		// Unregister the old command
		g_pCVar->UnregisterConCommand(pConCommand);
	}
	else if( szHelpText != NULL )
	{
		// Store the given help text
		szHelpTextCopy = strdup(szHelpText);
	}

	// Return a new instance of ConCommand
	return new CServerCommandManager(pConCommand, szNameCopy, szHelpTextCopy, iFlags);
}

//-----------------------------------------------------------------------------
// CServerCommandManager constructor.
//-----------------------------------------------------------------------------
CServerCommandManager::CServerCommandManager(ConCommand* pConCommand,
		const char* szName, const char* szHelpText, int iFlags):
	ConCommand(szName, (FnCommandCallback_t)NULL, szHelpText, iFlags),
	m_pOldCommand(pConCommand)
{
	m_Name = strdup(szName);
	
	m_vecCallables[HOOKTYPE_PRE] = new CListenerManager();
	m_vecCallables[HOOKTYPE_POST] = new CListenerManager();
}

//-----------------------------------------------------------------------------
// CServerCommandManager destructor.
//-----------------------------------------------------------------------------
CServerCommandManager::~CServerCommandManager()
{
	// Get the ConCommand instance
	ConCommand* pConCommand = g_pCVar->FindCommand(m_Name);

	// Was the command overwritten as a ConVar or by another DLL?
	if (pConCommand && pConCommand->GetDLLIdentifier() == CVarDLLIdentifier())
	{
		// Unregister the ConCommand
		g_pCVar->UnregisterConCommand(pConCommand);
	}

	// Was the command registered before we registered it?
	if( m_pOldCommand )
	{
		// Re-register the old command instance
		g_pCVar->RegisterConCommand(m_pOldCommand);
	}

	free((char*)m_Name);

	delete m_vecCallables[HOOKTYPE_PRE];
	delete m_vecCallables[HOOKTYPE_POST];
}

//-----------------------------------------------------------------------------
// CServerCommandManager Init override.
//-----------------------------------------------------------------------------
void CServerCommandManager::Init()
{
	ConCommand::Init();
}

//-----------------------------------------------------------------------------
// Adds a callable to a CServerCommandManager instance.
//-----------------------------------------------------------------------------
void CServerCommandManager::AddCallback( PyObject* pCallable, HookType_t type )
{
	m_vecCallables[type]->RegisterListener(pCallable);
}

//-----------------------------------------------------------------------------
// Removes a callable from a CServerCommandManager instance.
//-----------------------------------------------------------------------------
void CServerCommandManager::RemoveCallback( PyObject* pCallable, HookType_t type )
{
	object oCallable = object(handle<>(borrowed(pCallable)));
	m_vecCallables[type]->UnregisterListener(pCallable);
	if( !m_vecCallables[HOOKTYPE_PRE]->GetCount() && !m_vecCallables[HOOKTYPE_POST]->GetCount() )
	{
		RemoveCServerCommandManager(m_Name);
	}
}

//-----------------------------------------------------------------------------
// Calls all callables for the command when it is called on the server.
//-----------------------------------------------------------------------------
void CServerCommandManager::Dispatch( const CCommand& command )
{
	bool block = false;

	// Pre hook callbacks
	FOREACH_CALLBACK_WITH_MNGR(
		m_vecCallables[HOOKTYPE_PRE],
		object returnValue,
		if( !returnValue.is_none() && extract<int>(returnValue) == (int) BLOCK)
		{
			block = true;
		},
		boost::ref(command)
	)

	if (block)
		return;

	// Was the command previously registered?
	if(m_pOldCommand)
	{
		m_pOldCommand->Dispatch(command);
	}

	// Post hook callbacks
	CALL_LISTENERS_WITH_MNGR(m_vecCallables[HOOKTYPE_POST], boost::ref(command))
}

//-----------------------------------------------------------------------------
// Removes all CServerCommandManager instances.
//-----------------------------------------------------------------------------
void ClearAllServerCommands()
{
	// Loop through all items in the mapping
	for(ServerCommandMap::iterator commandMapIter = g_ServerCommandMap.begin(); commandMapIter != g_ServerCommandMap.end(); ++commandMapIter)
	{
		// Remove the CServerCommandManager instance
		delete commandMapIter->second;
	}
	// Clear the mapping
	g_ServerCommandMap.clear();
}

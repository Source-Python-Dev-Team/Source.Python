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
#ifndef _COMMANDS_SERVER_H
#define _COMMANDS_SERVER_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "convar.h"

#include "modules/listeners/listeners_manager.h"
#include "hook.h"

//-----------------------------------------------------------------------------
// Server Command Manager class.
//-----------------------------------------------------------------------------
class CServerCommandManager : public ConCommand
{
public:
	static CServerCommandManager* CreateCommand(const char* szName, const char* szHelpString, int iFlags);
	~CServerCommandManager();
	virtual void Init();

	void AddCallback(PyObject* pCallable, HookType_t type);
	void RemoveCallback(PyObject* pCallable, HookType_t type);

protected:
	void Dispatch( const CCommand& command);

private:
	CServerCommandManager(ConCommand* pConCommand, const char* szName, const char* szHelpString = 0, int iFlags = 0);
	std::map< HookType_t, CListenerManager* > m_vecCallables;
	const char* m_Name;
	ConCommand* m_pOldCommand;
};

#endif // _COMMANDS_SERVER_H

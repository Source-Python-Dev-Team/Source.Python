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
#ifndef _COMMANDS_CLIENTS_H
#define _COMMANDS_CLIENTS_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "sp_python.h"
#include "utilities/sp_util.h"
#include "utilities/wrap_macros.h"
#include "edict.h"
#include "convar.h"
#include "commands.h"
#include "game/server/iplayerinfo.h"

#include "modules/listeners/listeners_manager.h"


//-----------------------------------------------------------------------------
// Client Command Manager class.
//-----------------------------------------------------------------------------
class CClientCommandManager
{
public:
	CClientCommandManager(const char* szName);
	~CClientCommandManager();

	void AddCallback(PyObject* pCallable);
	void RemoveCallback(PyObject* pCallable);

	CommandReturn Dispatch(const CCommand& ccommand, int iIndex);

	const char* GetName();

private:
	CListenerManager m_vecCallables;
	const char* m_Name;
};

#endif // _COMMANDS_CLIENTS_H

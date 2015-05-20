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
#ifndef _COMMANDS_SAY_WRAP_H
#define _COMMANDS_SAY_WRAP_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "boost/unordered_map.hpp"
#include "utilities/sp_util.h"
#include "commands_wrap.h"
#include "utlvector.h"
//#include "utilities/wrap_macros.h"
#include "edict.h"
#include "game/server/iplayerinfo.h"


//-----------------------------------------------------------------------------
// Say ConCommand instance class.
//-----------------------------------------------------------------------------
class SayConCommand: public ConCommand
{
public:
	static SayConCommand* CreateCommand(const char* szName, const char* szHelpText, int iFlags);
	~SayConCommand();
	virtual void Init();

protected:
	void Dispatch( const CCommand& command );

private:
	SayConCommand(ConCommand* pConCommand, const char* szName, const char* szHelpText, int iFlags);
	const char* m_Name;
	ConCommand* m_pOldCommand;
};

//-----------------------------------------------------------------------------
// Base Say Command Manager class.
//-----------------------------------------------------------------------------
class BaseSayCommand
{
public:
	void RegisterCommands();
	void UnregisterCommands();

private:
	SayConCommand* m_pSayCommand;
	SayConCommand* m_pTeamSayCommand;
};

//-----------------------------------------------------------------------------
// Say Command Manager class.
//-----------------------------------------------------------------------------
class CSayCommandManager
{
public:
	CSayCommandManager(const char* szName);
	~CSayCommandManager();

	void AddCallback(PyObject* pCallable);
	void RemoveCallback(PyObject* pCallable);

	CommandReturn Dispatch(IPlayerInfo* pPlayerInfo, bool bTeamOnly, const CCommand& ccommand);

private:
	const char* m_Name;
	CUtlVector<object> m_vecCallables;
};

#endif // _COMMANDS_SAY_WRAP_H

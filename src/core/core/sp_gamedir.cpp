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

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "sp_gamedir.h"
#include "sp_main.h"
#include "eiface.h"
#include "strtools.h"

//---------------------------------------------------------------------------------
// External variables we need.
//---------------------------------------------------------------------------------
extern IVEngineServer* engine;

//---------------------------------------------------------------------------------
// Static singleton
//---------------------------------------------------------------------------------
CGamePaths g_GamePaths;

//---------------------------------------------------------------------------------
// Initializes game paths.
//---------------------------------------------------------------------------------
bool CGamePaths::Initialize( void )
{
	if( !engine ) {
		Msg(MSG_PREFIX "Could not initialize game paths! Engine pointer was invalid.\n");
		return false;
	}

	engine->GetGameDir(m_szGameDir, MAX_GAME_PATH);
	V_snprintf(m_szSPDir, MAX_GAME_PATH, "%s%s", m_szGameDir, sp_ADDON_BASE);

	// Fix slashes
	V_FixSlashes(m_szGameDir);
	V_FixSlashes(m_szSPDir);

	DevMsg(1, MSG_PREFIX "Game directory is %s\n", m_szGameDir);
	DevMsg(1, MSG_PREFIX "Source-Python directory is %s\n", m_szSPDir);

	return true;
}

//---------------------------------------------------------------------------------
// Returns the path to the game directory.
//---------------------------------------------------------------------------------
char* CGamePaths::GetGameDir( void )
{
	return m_szGameDir;
}

//---------------------------------------------------------------------------------
// Returns the path to the source-python addon directory.
//---------------------------------------------------------------------------------
char* CGamePaths::GetSPDir( void )
{
	return m_szSPDir;
}

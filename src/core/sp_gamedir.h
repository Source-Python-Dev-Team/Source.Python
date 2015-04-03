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
#ifndef _sp_GAMEDIR_H
#define _sp_GAMEDIR_H

//---------------------------------------------------------------------------------
// Purpose: This file contains the CGamePaths class. This class is responsible
// for constructing frequently used paths and caching them so they don't have to
// be constantly reconstructed via engine->GameDir and the like. You must
// initialize this class before anything else in the Core or else you will crash!
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------------
#define MAX_GAME_PATH 1024

//---------------------------------------------------------------------------------
// Note, we are using '/' here as the path separator. We will later call v_fixslashes
// which will replace it with '\\' on Win32 systems.
//---------------------------------------------------------------------------------
#define sp_ADDON_BASE "/addons/source-python"

//---------------------------------------------------------------------------------
// The CGamePaths class.
//---------------------------------------------------------------------------------
class CGamePaths
{
	private:
		char m_szGameDir[MAX_GAME_PATH];
		char m_szSPDir[MAX_GAME_PATH];

	public:
		bool Initialize( void );

		char* GetGameDir( void );
		char* GetSPDir( void );
};

//---------------------------------------------------------------------------------
// Static singleton.
//---------------------------------------------------------------------------------
extern CGamePaths g_GamePaths;

#endif // _sp_GAMEDIR_H

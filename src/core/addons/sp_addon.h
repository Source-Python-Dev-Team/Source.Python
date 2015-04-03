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
#ifndef _sp_ADDON_H
#define _sp_ADDON_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "sp_python.h"
#include "utllinkedlist.h"
#include "igameevents.h"
#include "engine/iserverplugin.h"

//---------------------------------------------------------------------------------
// Addon manager.
//---------------------------------------------------------------------------------
class CAddonManager
{
	public:
		CAddonManager( void );
		~CAddonManager( void );

		void GameFrame();
        void NetworkIDValidated( const char *pszUserName, const char *pszNetworkID );
		void LevelInit( char const *pMapName );
	    void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
		void LevelShutdown( void );
		void ClientActive( edict_t *pEntity );
		void ClientDisconnect( edict_t *pEntity );
		void ClientPutInServer( edict_t *pEntity, char const *playername );
		void ClientSettingsChanged( edict_t *pEdict );
		PLUGIN_RESULT ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen );

		void OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue );

#ifdef ENGINE_CSGO
		void ClientFullyConnect( edict_t *pEntity );
		void OnEdictAllocated( edict_t *edict );
		void OnEdictFreed( const edict_t *edict );
#endif

		void OnEntityCreated( CBaseEntity *pEntity );
		void OnEntitySpawned( CBaseEntity *pEntity );
		void OnEntityDeleted( CBaseEntity *pEntity );
};

//---------------------------------------------------------------------------------
// Static singleton.
//---------------------------------------------------------------------------------
extern CAddonManager g_AddonManager;

#endif // _sp_ADDON_H

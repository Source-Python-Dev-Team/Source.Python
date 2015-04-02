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
#include "sp_addon.h"
#include "strtools.h"
#include "convar.h"
#include "filesystem.h"
#include "core/sp_gamedir.h"
#include "utility/wrap_macros.h"
#include "modules/listeners/listeners_manager.h"
#include "modules/entities/entities_wrap.h"
#include "utility/conversions.h"

//---------------------------------------------------------------------------------
// External variables
//---------------------------------------------------------------------------------
extern IFileSystem* filesystem;

//---------------------------------------------------------------------------------
// Static singleton.
//---------------------------------------------------------------------------------
CAddonManager g_AddonManager;

//---------------------------------------------------------------------------------
// Constructor
//---------------------------------------------------------------------------------
CAddonManager::CAddonManager( void )
{

}

//---------------------------------------------------------------------------------
// Destructor
//---------------------------------------------------------------------------------
CAddonManager::~CAddonManager( void )
{

}

//---------------------------------------------------------------------------------
// Calls tick listener.
//---------------------------------------------------------------------------------
void CAddonManager::GameFrame()
{
	CALL_LISTENERS(Tick);
}

//---------------------------------------------------------------------------------
// Calls network id validated listeners.
//---------------------------------------------------------------------------------
void CAddonManager::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
	CALL_LISTENERS(NetworkidValidated, pszUserName, pszNetworkID);
}

//---------------------------------------------------------------------------------
// Calls level init listeners.
//---------------------------------------------------------------------------------
void CAddonManager::LevelInit( char const *pMapName )
{
	CALL_LISTENERS(LevelInit, pMapName);
}

//---------------------------------------------------------------------------------
// Calls server activate listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	list edicts;
	for(int i=0; i < edictCount; i++)
		edicts.append(pEdictList[i]);
	
	CALL_LISTENERS(ServerActivate, edicts, edictCount, clientMax);
}

//---------------------------------------------------------------------------------
// Calls level shutdown listeners.
//---------------------------------------------------------------------------------
void CAddonManager::LevelShutdown( void )
{
	CALL_LISTENERS(LevelShutdown);
}

//---------------------------------------------------------------------------------
// Calls client active listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientActive( edict_t *pEntity )
{
	CALL_LISTENERS(ClientActive, IndexFromEdict(pEntity));
}

//---------------------------------------------------------------------------------
// Calls client disconnect listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientDisconnect( edict_t *pEntity )
{
	CALL_LISTENERS(ClientDisconnect, IndexFromEdict(pEntity));
}

//---------------------------------------------------------------------------------
// Calls client put in server listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientPutInServer( edict_t *pEntity, char const *playername )
{
	CALL_LISTENERS(ClientPutInServer, IndexFromEdict(pEntity), playername);
}

//---------------------------------------------------------------------------------
// Calls client settings changed listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientSettingsChanged( edict_t *pEdict )
{
	CALL_LISTENERS(ClientSettingsChanged, IndexFromEdict(pEdict));
}

//---------------------------------------------------------------------------------
// Calls client connect listeners.
//---------------------------------------------------------------------------------
PLUGIN_RESULT CAddonManager::ClientConnect( bool *bAllowConnect, edict_t *pEntity, 
	const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	CALL_LISTENERS(ClientConnect, ptr(new CPointer((unsigned long) bAllowConnect)), IndexFromEdict(pEntity), pszName, pszAddress, ptr(new CPointer((unsigned long) reject)), maxrejectlen);
	return PLUGIN_OVERRIDE;
}

//---------------------------------------------------------------------------------
// Calls on query cvar value finished listeners.
//---------------------------------------------------------------------------------
void CAddonManager::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, 
	edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, 
	const char *pCvarValue )
{
	CALL_LISTENERS(OnQueryCvarValueFinished, (int) iCookie, IndexFromEdict(pPlayerEntity), eStatus, pCvarName, pCvarValue);
}
//
// 
//
#ifdef ENGINE_CSGO
void CAddonManager::ClientFullyConnect( edict_t *pEntity )
{
	CALL_LISTENERS(ClientFullyConnect, IndexFromEdict(pEntity));
}

void CAddonManager::OnEdictAllocated( edict_t *edict )
{
	CALL_LISTENERS(OnEdictAllocated, IndexFromEdict(edict));
}

void CAddonManager::OnEdictFreed( const edict_t *edict )
{
	CALL_LISTENERS(OnEdictFreed, ptr(edict));
}
#endif

void CAddonManager::OnEntityCreated( CBaseEntity *pEntity )
{
	CPointer ptr = CPointer((unsigned long) pEntity);
	CALL_LISTENERS(OnEntityCreated, IndexFromPointer(&ptr));
}

void CAddonManager::OnEntitySpawned( CBaseEntity *pEntity )
{
	CPointer ptr = CPointer((unsigned long) pEntity);
	CALL_LISTENERS(OnEntitySpawned, IndexFromPointer(&ptr));
}

void CAddonManager::OnEntityDeleted( CBaseEntity *pEntity )
{
	CPointer ptr = CPointer((unsigned long) pEntity);
	CALL_LISTENERS(OnEntityDeleted, IndexFromPointer(&ptr));
}
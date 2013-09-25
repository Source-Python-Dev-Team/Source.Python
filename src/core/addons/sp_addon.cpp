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
#include "modules/listeners/listeners.h"

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
	// Dispatch all tick listeners
	get_tick_listener_manager()->call_listeners();
}

//---------------------------------------------------------------------------------
// Calls network id validated listeners.
//---------------------------------------------------------------------------------
void CAddonManager::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
	// Dispatch all NetwordIDValidatedListeners
	get_networkid_validated_listener_manager()->call_listeners(pszUserName, pszNetworkID);
}


//---------------------------------------------------------------------------------
// Calls level init listeners.
//---------------------------------------------------------------------------------
void CAddonManager::LevelInit( char const *pMapName )
{
	// Dispatch all LevelInit listeners
	get_level_init_listener_manager()->call_listeners(pMapName);
}

//---------------------------------------------------------------------------------
// Calls server activate listeners.
//---------------------------------------------------------------------------------
// TODO: will not work if this is really a list
void CAddonManager::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	// Dispatch all LevelInit listeners
	get_server_activate_listener_manager()->call_listeners(pEdictList, edictCount, clientMax);
}

//---------------------------------------------------------------------------------
// Calls level shutdown listeners.
//---------------------------------------------------------------------------------
void CAddonManager::LevelShutdown( void )
{
    // Dispatch all tick listeners
	get_level_shutdown_listener_manager()->call_listeners();
}


//---------------------------------------------------------------------------------
// Calls client active listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientActive( edict_t *pEntity )
{
	// Dispatch all LevelInit listeners
	get_client_active_listener_manager()->call_listeners(pEntity);
}

//---------------------------------------------------------------------------------
// Calls client disconnect listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientDisconnect( edict_t *pEntity )
{
	// Dispatch all LevelInit listeners
	get_client_disconnect_listener_manager()->call_listeners(pEntity);
}

//---------------------------------------------------------------------------------
// Calls client put in server listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientPutInServer( edict_t *pEntity, char const *playername )
{
	// Dispatch all LevelInit listeners
	get_client_put_in_server_listener_manager()->call_listeners(pEntity, playername);
}

//---------------------------------------------------------------------------------
// Calls client settings changed listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientSettingsChanged( edict_t *pEdict )
{
	// Dispatch all LevelInit listeners
	get_client_settings_changed_listener_manager()->call_listeners(pEdict);
}

//---------------------------------------------------------------------------------
// Calls client connect listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientConnect( bool *bAllowConnect, edict_t *pEntity, 
	const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	// Dispatch all LevelInit listeners
	get_client_connect_listener_manager()->call_listeners(bAllowConnect, pEntity, pszName, pszAddress, reject, maxrejectlen);
}

//---------------------------------------------------------------------------------
// Calls on query cvar value finished listeners.
//---------------------------------------------------------------------------------
void CAddonManager::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, 
	edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, 
	const char *pCvarValue )
{

	// Dispatch all LevelInit listeners
	get_client_fully_connect_listener_manager()->call_listeners(iCookie, pPlayerEntity, eStatus, pCvarName, pCvarValue);
}
//
// 
//
#if(SOURCE_ENGINE >= 3)
void CAddonManager::ClientFullyConnect( edict_t *pEntity )
{
	// Dispatch all LevelInit listeners
	get_client_fully_connect_listener_manager()->call_listeners(pEntity);
}

void CAddonManager::OnEdictAllocated( edict_t *edict )
{
	// Dispatch all LevelInit listeners
	get_on_edict_allocated_listener_manager()->call_listeners(edict);
}

void CAddonManager::OnEdictFreed( const edict_t *edict )
{
	// Dispatch all LevelInit listeners
	get_on_edict_freed_listener_manager()->call_listeners(const_cast<edict_t*>(edict));
}
#endif
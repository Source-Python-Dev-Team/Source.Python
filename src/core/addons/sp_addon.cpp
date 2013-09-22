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
	get_tick_listener_manager()->call_listeners(0);
}

//---------------------------------------------------------------------------------
// Calls network id validated listeners.
//---------------------------------------------------------------------------------
void CAddonManager::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
	// Wrap the parameters
	CListenerManager::Param p1, p2;
	
	p1.name="playername"; 
	p1.type=CListenerManager::CONST_CHAR_PTR; 
	p1.const_char_ptr = pszUserName;
	
	p2.name="networkid"; 
	p2.type=CListenerManager::CONST_CHAR_PTR;
	p2.const_char_ptr = pszNetworkID;
	// Dispatch all NetwordIDValidatedListeners
	get_networkid_validated_listener_manager()->call_listeners(2, p1, p2);
}


//---------------------------------------------------------------------------------
// Calls level init listeners.
//---------------------------------------------------------------------------------
void CAddonManager::LevelInit( char const *pMapName )
{
	// Wrap the parameters
	CListenerManager::Param p1;
	
	p1.name="mapname"; 
	p1.type=CListenerManager::CONST_CHAR_PTR; 
	p1.const_char_ptr= pMapName;
	// Dispatch all LevelInit listeners
	get_level_init_listener_manager()->call_listeners(1, p1);
}

//---------------------------------------------------------------------------------
// Calls server activate listeners.
//---------------------------------------------------------------------------------
// TODO: will not work if this is really a list
void CAddonManager::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	// Wrap the parameters
	CListenerManager::Param p1, p2, p3;
	
	p1.name="edictlist"; 
	p1.type=CListenerManager::EDICT_T_PTR; 
	p1.edict_t_ptr= pEdictList;

	p2.name="edict_count"; 
	p2.type=CListenerManager::INT; 
	p2.int_value=edictCount;

	p3.name="max_clients"; 
	p3.type=CListenerManager::INT; 
	p3.int_value=clientMax;
	// Dispatch all LevelInit listeners
	get_server_activate_listener_manager()->call_listeners(3, p1, p2, p3);
}

//---------------------------------------------------------------------------------
// Calls level shutdown listeners.
//---------------------------------------------------------------------------------
void CAddonManager::LevelShutdown( void )
{
    // Dispatch all tick listeners
	get_level_shutdown_listener_manager()->call_listeners(0);
}


//---------------------------------------------------------------------------------
// Calls client active listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientActive( edict_t *pEntity )
{
	// Wrap the parameters
	CListenerManager::Param p1;
	
	p1.name="edict"; 
	p1.type=CListenerManager::EDICT_T_PTR; 
	p1.edict_t_ptr= pEntity;
	// Dispatch all LevelInit listeners
	get_client_active_listener_manager()->call_listeners(1, p1);
}

//---------------------------------------------------------------------------------
// Calls client disconnect listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientDisconnect( edict_t *pEntity )
{
	// Wrap the parameters
	CListenerManager::Param p1;
	
	p1.name="edict"; 
	p1.type=CListenerManager::EDICT_T_PTR; 
	p1.edict_t_ptr= pEntity;
	// Dispatch all LevelInit listeners
	get_client_disconnect_listener_manager()->call_listeners(1, p1);
}

//---------------------------------------------------------------------------------
// Calls client put in server listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientPutInServer( edict_t *pEntity, char const *playername )
{
	// Wrap the parameters
	CListenerManager::Param p1, p2;
	
	p1.name="edict"; 
	p1.type=CListenerManager::EDICT_T_PTR; 
	p1.edict_t_ptr= pEntity;

	p2.name="playername"; 
	p2.type=CListenerManager::CONST_CHAR_PTR; 
	p2.const_char_ptr= playername;
	// Dispatch all LevelInit listeners
	get_client_put_in_server_listener_manager()->call_listeners(2, p1, p2);
}

//---------------------------------------------------------------------------------
// Calls client settings changed listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientSettingsChanged( edict_t *pEdict )
{
	// Wrap the parameters
	CListenerManager::Param p1;
	
	p1.name="edict"; 
	p1.type=CListenerManager::EDICT_T_PTR; 
	p1.edict_t_ptr= pEdict;
	// Dispatch all LevelInit listeners
	get_client_settings_changed_listener_manager()->call_listeners(1, p1);
}

//---------------------------------------------------------------------------------
// Calls client connect listeners.
//---------------------------------------------------------------------------------
void CAddonManager::ClientConnect( bool *bAllowConnect, edict_t *pEntity, 
	const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	// Wrap the parameters
	CListenerManager::Param p1, p2, p3, p4, p5, p6;
	
	p1.name="edict"; 
	p1.type=CListenerManager::EDICT_T_PTR; 
	p1.edict_t_ptr=pEntity;

	p2.name="playername"; 
	p2.type=CListenerManager::CONST_CHAR_PTR; 
	p2.const_char_ptr=pszName;

	p3.name="network_address"; 
	p3.type=CListenerManager::CONST_CHAR_PTR; 
	p3.const_char_ptr=pszAddress;

	p4.name="max_reject_len"; 
	p4.type=CListenerManager::INT; 
	p4.int_value=maxrejectlen;

	// Must be casted in order to avoid crash
	// TODO: Maybe in the future this can done properly, so people can reject the 
	// connection from python, but for now it's read-only
	const char* reject_copy = reject;
	p5.name="reject"; 
	p5.type=CListenerManager::CONST_CHAR_PTR; 
	p5.const_char_ptr=reject_copy;

	p6.name="is_allowed_to_connect"; 
	p6.type=CListenerManager::BOOL; 
	p6.bool_value=&bAllowConnect;
	
	// Dispatch all LevelInit listeners
	get_client_connect_listener_manager()->call_listeners(6, p1, p2, p3, p4, p5, p6);
}

//---------------------------------------------------------------------------------
// Calls on query cvar value finished listeners.
//---------------------------------------------------------------------------------
void CAddonManager::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, 
	edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, 
	const char *pCvarValue )
{
	// Wrap the parameters
	CListenerManager::Param p1, p2, p3, p4, p5;
	
	p1.name="cookie_id"; 
	p1.type=CListenerManager::INT;
	// iCookie is just an int
	p1.int_value=(int)iCookie;

	p2.name="edict"; 
	p2.type=CListenerManager::EDICT_T_PTR; 
	p2.edict_t_ptr=pPlayerEntity;

	p3.name="cookie_status"; 
	p3.type=CListenerManager::INT; 
	p3.int_value=static_cast<int>(eStatus);

	p4.name="cvar_name"; 
	p4.type=CListenerManager::CONST_CHAR_PTR; 
	p4.const_char_ptr=pCvarName;

	p5.name="cvar_value"; 
	p5.type=CListenerManager::CONST_CHAR_PTR; 
	p5.const_char_ptr=pCvarValue;
	// Dispatch all LevelInit listeners
	get_client_fully_connect_listener_manager()->call_listeners(5, p1, p2, p3, p4, p5);
}
//
// 
//
#if(SOURCE_ENGINE >= 3)
void CAddonManager::ClientFullyConnect( edict_t *pEntity )
{
	// Wrap the parameters
	CListenerManager::Param p1;
	
	p1.name="edict"; 
	p1.type=CListenerManager::EDICT_T_PTR; 
	p1.edict_t_ptr=pEntity;
	// Dispatch all LevelInit listeners
	get_client_fully_connect_listener_manager()->call_listeners(1, p1);
}

void CAddonManager::OnEdictAllocated( edict_t *edict )
{
	// Wrap the parameters
	CListenerManager::Param p1;
	
	p1.name="edict"; 
	p1.type=CListenerManager::EDICT_T_PTR; 
	p1.edict_t_ptr=edict;
	// Dispatch all LevelInit listeners
	get_on_edict_allocated_listener_manager()->call_listeners(1, p1);
}

void CAddonManager::OnEdictFreed( const edict_t *edict )
{
	// Wrap the parameters
	CListenerManager::Param p1;

	p1.name="edict"; 
	p1.type=CListenerManager::EDICT_T_PTR; 
	// This might not be a good idea, who knows :P
	p1.edict_t_ptr=const_cast<edict_t*>(edict);

	// Dispatch all LevelInit listeners
	get_on_edict_freed_listener_manager()->call_listeners(1, p1);

	//get_on_edict_freed_listener_manager()->call_listeners(0);
}
#endif
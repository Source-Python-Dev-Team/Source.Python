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
#ifndef _SOURCEPYTHON_MAIN_H
#define _SOURCEPYTHON_MAIN_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "engine/iserverplugin.h"
#include "igameevents.h"
#include "modules/memory/memory_tools.h"

#include "utilities/baseentity.h"
#include "game/shared/entitylist_base.h"
#include "game/server/entitylist.h"
#include "datacache/imdlcache.h"


//---------------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------------
#include "../loader/definitions.h"


//---------------------------------------------------------------------------------
// Purpose: a sample 3rd party plugin class
//---------------------------------------------------------------------------------
class CSourcePython: public IServerPluginCallbacks, public IEntityListener, public IMDLCacheNotify
{
public:
	CSourcePython();
	~CSourcePython();

	// IServerPluginCallbacks methods
	virtual bool			Load(	CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory );
	virtual void			Unload( void );
	virtual void			Pause( void );
	virtual void			UnPause( void );
	virtual const char		*GetPluginDescription( void );
	virtual void			LevelInit( char const *pMapName );
	virtual void			ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
	virtual void			GameFrame( bool simulating );
	virtual void			LevelShutdown( void );
	virtual void			ClientActive( edict_t *pEntity );
	virtual void			ClientDisconnect( edict_t *pEntity );
	virtual void			ClientPutInServer( edict_t *pEntity, char const *playername );
	virtual void			SetCommandClient( int index );
	virtual void			ClientSettingsChanged( edict_t *pEdict );
	virtual PLUGIN_RESULT	ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen );

	// -------------------------------------------
	// Orangebox.
	// -------------------------------------------
	virtual PLUGIN_RESULT	ClientCommand( edict_t *pEntity, const CCommand &args );

	// -------------------------------------------
	// Alien Swarm.
	// -------------------------------------------
#if defined(ENGINE_CSGO) || defined(ENGINE_BLADE)
	virtual void			ClientFullyConnect( edict_t *pEntity );
#endif

#if defined(ENGINE_CSGO) || defined(ENGINE_BMS) || defined(ENGINE_BLADE)
	virtual void			OnEdictAllocated( edict_t *edict );
	virtual void			OnEdictFreed( const edict_t *edict );
#endif

#if defined(ENGINE_CSGO)
	virtual bool			BNetworkCryptKeyCheckRequired( uint32 unFromIP, uint16 usFromPort, uint32 unAccountIdProvidedByClient,
								bool bClientWantsToUseCryptKey );
	virtual bool			BNetworkCryptKeyValidate( uint32 unFromIP, uint16 usFromPort, uint32 unAccountIdProvidedByClient,
								int nEncryptionKeyIndexFromClient, int numEncryptedBytesFromClient, byte *pbEncryptedBufferFromClient,
								byte *pbPlainTextKeyForNetchan );
#endif

	virtual PLUGIN_RESULT	NetworkIDValidated( const char *pszUserName, const char *pszNetworkID );
	virtual void			OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity,
								EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue );

	virtual int				GetCommandIndex() { return m_iClientCommandIndex; }

	// -------------------------------------------
	// IEntityListener Interface.
	// -------------------------------------------
#ifdef ENGINE_BMS
	virtual void OnEntityPreSpawned( CBaseEntity *pEntity );
#endif
	virtual void OnEntityCreated( CBaseEntity *pEntity );
	virtual void OnEntitySpawned( CBaseEntity *pEntity );
	virtual void OnEntityDeleted( CBaseEntity *pEntity );

	// -------------------------------------------
	// IMDLCacheNotify Interface.
	// -------------------------------------------
	virtual void OnDataLoaded( MDLCacheDataType_t type, MDLHandle_t handle );
	virtual void OnDataUnloaded( MDLCacheDataType_t type, MDLHandle_t handle );
#ifdef ENGINE_CSGO
	virtual void OnCombinerPreCache(MDLCacheDataType_t type, MDLHandle_t handle );
	virtual bool ShouldSupressLoadWarning(MDLHandle_t handle);
#endif

public:
	int m_iClientCommandIndex;
	IMDLCacheNotify* m_pOldMDLCacheNotifier;

#if defined(ENGINE_ORANGEBOX) || defined(ENGINE_BMS) || defined(ENGINE_GMOD)
	SpewOutputFunc_t m_pOldSpewOutputFunc;
#endif
};


#endif // _SOURCEPYTHON_MAIN_H

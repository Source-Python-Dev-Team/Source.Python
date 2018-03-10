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
#ifndef _LOADER_MAIN_H
#define _LOADER_MAIN_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "engine/iserverplugin.h"
#include "igameevents.h"

//---------------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------------
#include "definitions.h"


//---------------------------------------------------------------------------------
// Purpose: a sample 3rd party plugin class
//---------------------------------------------------------------------------------
class CSourcePython: public IServerPluginCallbacks
{
public:
	CSourcePython();
	~CSourcePython();

	// IServerPluginCallbacks methods
	virtual bool			Load(	CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory );
	virtual void			Unload( void );
	virtual void			Pause( void );
	virtual void			UnPause( void );
	virtual const char	 *GetPluginDescription( void );
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
	// Counter-Strike: Global Offensive
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

private:
	int						m_iClientCommandIndex;

	void*					m_pCore;
	IServerPluginCallbacks*	m_pCorePlugin;
};


#endif // _LOADER_MAIN_H

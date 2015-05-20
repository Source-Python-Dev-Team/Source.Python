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

// --------------------------------------------------------
// Includes
// --------------------------------------------------------
#include "sp_python.h"
#include "sp_main.h"
#include "interface.h"
#include "filesystem.h"
#include "eiface.h"
#include "igameevents.h"
#include "convar.h"
#include "Color.h"
#include "vstdlib/random.h"
#include "engine/IEngineTrace.h"
#include "tier2/tier2.h"
#include "game/server/iplayerinfo.h"
#include "shake.h" // Linux compile fails without this.
#include "game/shared/IEffects.h"
#include "utilities/wrap_macros.h"
#include "engine/IEngineSound.h"
#include "engine/IEngineTrace.h"
#include "public/toolframework/itoolentity.h"
#include "dyncall.h"
#include "networkstringtabledefs.h"
#include "edict.h"
#include "convar.h"
#include "utilities/call_python.h"
#include "vphysics_interface.h"
#include "datacache/imdlcache.h"

#include "manager.h"

#include "modules/listeners/listeners_manager.h"
#include "utilities/conversions.h"


//-----------------------------------------------------------------------------
// Disable warnings.
//-----------------------------------------------------------------------------
#if defined(_WIN32)
#	 pragma warning( disable : 4005 )
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Interfaces from the engine
//-----------------------------------------------------------------------------
IVEngineServer*			engine				= NULL; // helper functions (messaging clients, loading content, making entities, running commands, etc)
IGameEventManager2*		gameeventmanager	= NULL; // game events interface
IPlayerInfoManager*		playerinfomanager	= NULL; // game dll interface to interact with players
IBotManager*			botmanager			= NULL; // game dll interface to interact with bots
IServerPluginHelpers*	helpers				= NULL; // special 3rd party plugin helpers from the engine
IUniformRandomStream*	randomStr			= NULL;
IEngineTrace*			enginetrace			= NULL;
IEngineSound*			enginesound			= NULL;
CGlobalVars*			gpGlobals			= NULL;
IFileSystem*			filesystem			= NULL;
IServerGameDLL*			servergamedll		= NULL;
IServerTools*			servertools			= NULL;
IPhysics*				physics				= NULL;
IPhysicsCollision*		physcollision		= NULL;
IPhysicsSurfaceProps*	physprops			= NULL;
IMDLCache*				modelcache			= NULL;
INetworkStringTableContainer* networkstringtable = NULL;

//-----------------------------------------------------------------------------
// External globals
//-----------------------------------------------------------------------------
extern ICvar* g_pCVar;

//-----------------------------------------------------------------------------
// Extern functions
//-----------------------------------------------------------------------------
extern void InitCommands();
extern void ClearAllCommands();
extern PLUGIN_RESULT DispatchClientCommand(edict_t *pEntity, const CCommand &command);

//-----------------------------------------------------------------------------
// The plugin is a static singleton that is exported as an interface
//-----------------------------------------------------------------------------
CSourcePython g_SourcePythonPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CSourcePython, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_SourcePythonPlugin );

//-----------------------------------------------------------------------------
// Helper console variable to tell scripters what engine version we are running
// on.
//-----------------------------------------------------------------------------
ConVar sp_engine_ver("sp_engine_ver", XSTRINGIFY(SOURCE_ENGINE), 0, "Version of the engine SP is running on");

//-----------------------------------------------------------------------------
// Interface helper class.
//-----------------------------------------------------------------------------
struct InterfaceHelper_t
{
	const char* szInterface;
	void**		pGlobal;
};

//-----------------------------------------------------------------------------
// We need all of these interfaces in order to function.
//-----------------------------------------------------------------------------
InterfaceHelper_t gEngineInterfaces[] = {
	{INTERFACEVERSION_VENGINESERVER, (void **)&engine},
	{INTERFACEVERSION_GAMEEVENTSMANAGER2, (void **)&gameeventmanager},
	{INTERFACEVERSION_ISERVERPLUGINHELPERS, (void **)&helpers},
	{INTERFACEVERSION_ENGINETRACE_SERVER, (void **)&enginetrace},
	{IENGINESOUND_SERVER_INTERFACE_VERSION, (void **)&enginesound},
	{VENGINE_SERVER_RANDOM_INTERFACE_VERSION, (void **)&randomStr},
	{FILESYSTEM_INTERFACE_VERSION, (void **)&filesystem},
	{INTERFACENAME_NETWORKSTRINGTABLESERVER, (void **)&networkstringtable},
	{VPHYSICS_INTERFACE_VERSION, (void **)&physics},
	{VPHYSICS_COLLISION_INTERFACE_VERSION, (void **)&physcollision},
	{VPHYSICS_SURFACEPROPS_INTERFACE_VERSION, (void **)&physprops},
	{MDLCACHE_INTERFACE_VERSION, (void **)&modelcache},

	{NULL, NULL}
};

InterfaceHelper_t gGameInterfaces[] = {
	{INTERFACEVERSION_PLAYERINFOMANAGER, (void **)&playerinfomanager},
	{INTERFACEVERSION_PLAYERBOTMANAGER, (void **)&botmanager},
	{INTERFACEVERSION_SERVERGAMEDLL, (void **)&servergamedll},
	{VSERVERTOOLS_INTERFACE_VERSION, (void **)&servertools},
	{NULL, NULL}
};


//-----------------------------------------------------------------------------
// Get all engine interfaces.
//-----------------------------------------------------------------------------
bool GetInterfaces( InterfaceHelper_t* pInterfaceList, CreateInterfaceFn factory )
{
	InterfaceHelper_t* pInterface = pInterfaceList;
	while( pInterface->szInterface )
	{
		void** pGlobal = pInterface->pGlobal;

		// Get the interface from the given factory.
		*pGlobal = factory(pInterface->szInterface, NULL);

		// If it's not valid, bail out.
		if( *pGlobal ) {
			DevMsg(1, MSG_PREFIX "Interface %s at %x\n", pInterface->szInterface, *pGlobal);
		} else {
			Warning(MSG_PREFIX "Could not retrieve interface %s\n", pInterface->szInterface);
			return false;
		}

		pInterface++;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: constructor/destructor
//-----------------------------------------------------------------------------
CSourcePython::CSourcePython()
{
	m_iClientCommandIndex = 0;
}

CSourcePython::~CSourcePython()
{
}

//-----------------------------------------------------------------------------
// Purpose: called when the plugin is loaded, load the interface we need from the engine
//-----------------------------------------------------------------------------
bool CSourcePython::Load(	CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory )
{
	// This seems to be new with
#ifdef ENGINE_CSGO
	ConnectInterfaces(&interfaceFactory, 1);
#else
	ConnectTier1Libraries( &interfaceFactory, 1 );
	ConnectTier2Libraries( &interfaceFactory, 2 );
#endif

	// Get all engine interfaces.
	if( !GetInterfaces(gEngineInterfaces, interfaceFactory) ) {
		return false;
	}

	// Get all game interfaces.
	if( !GetInterfaces(gGameInterfaces, gameServerFactory) ) {
		return false;
	}

	gpGlobals = playerinfomanager->GetGlobalVars();

	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f );
	InitCommands();

	// Initialize python
	if( !g_PythonManager.Initialize() ) {
		Msg(MSG_PREFIX "Could not initialize python.\n");
		return false;
	}

	// TODO: Don't hardcode the 64 bytes offset
	m_pOldMDLCacheNotifier = *(IMDLCacheNotify **)(((unsigned long) modelcache) + 64);
	modelcache->SetCacheNotify(this);

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: called when the plugin is unloaded (turned off)
//-----------------------------------------------------------------------------
void CSourcePython::Unload( void )
{
	Msg(MSG_PREFIX "Unloading...\n");
	ClearAllCommands();
	ConVar_Unregister( );

	g_PythonManager.Shutdown();

	// New in CSGO...
#ifdef ENGINE_CSGO
	DisconnectInterfaces();
#else
	DisconnectTier2Libraries( );
	DisconnectTier1Libraries( );
#endif

	GetHookManager()->UnhookAllFunctions();
	Msg(MSG_PREFIX "Unloaded successfully.\n");
}

//-----------------------------------------------------------------------------
// Purpose: called when the plugin is paused (i.e should stop running but isn't unloaded)
//-----------------------------------------------------------------------------
void CSourcePython::Pause( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: called when the plugin is unpaused (i.e should start executing again)
//-----------------------------------------------------------------------------
void CSourcePython::UnPause( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: the name of this plugin, returned in "plugin_print" command
//-----------------------------------------------------------------------------
const char *CSourcePython::GetPluginDescription( void )
{
	return "Source Python, (C) 2012, Source Python Team.";
}

//-----------------------------------------------------------------------------
// Purpose: called on level start
//-----------------------------------------------------------------------------
void CSourcePython::LevelInit( char const *pMapName )
{
	CALL_LISTENERS(LevelInit, pMapName);
}

//-----------------------------------------------------------------------------
// Purpose: called on level start, when the server is ready to accept client connections
//		edictCount is the number of entities in the level, clientMax is the max client count
//-----------------------------------------------------------------------------
void CSourcePython::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	list edicts;
	for(int i=0; i < edictCount; i++)
		edicts.append(pEdictList[i]);
	
	CALL_LISTENERS(ServerActivate, edicts, edictCount, clientMax);
}

//-----------------------------------------------------------------------------
// Purpose: called once per server frame, do recurring work here (like checking for timeouts)
//-----------------------------------------------------------------------------
void CSourcePython::GameFrame( bool simulating )
{
	CALL_LISTENERS(Tick);
}

//-----------------------------------------------------------------------------
// Purpose: called on level end (as the server is shutting down or going to a new map)
//-----------------------------------------------------------------------------
void CSourcePython::LevelShutdown( void ) // !!!!this can get called multiple times per map change
{
	CALL_LISTENERS(LevelShutdown);
}

//-----------------------------------------------------------------------------
// Purpose: called when a client spawns into a server (i.e as they begin to play)
//-----------------------------------------------------------------------------
void CSourcePython::ClientActive( edict_t *pEntity )
{
	CALL_LISTENERS(ClientActive, IndexFromEdict(pEntity));
}

//-----------------------------------------------------------------------------
// Purpose: called when a client leaves a server (or is timed out)
//-----------------------------------------------------------------------------
void CSourcePython::ClientDisconnect( edict_t *pEntity )
{
	CALL_LISTENERS(ClientDisconnect, IndexFromEdict(pEntity));
}

//-----------------------------------------------------------------------------
// Purpose: called on
//-----------------------------------------------------------------------------
void CSourcePython::ClientPutInServer( edict_t *pEntity, char const *playername )
{
	CALL_LISTENERS(ClientPutInServer, IndexFromEdict(pEntity), playername);
}

//-----------------------------------------------------------------------------
// Purpose: called on level start
//-----------------------------------------------------------------------------
void CSourcePython::SetCommandClient( int index )
{
	m_iClientCommandIndex = index;
}

//-----------------------------------------------------------------------------
// Purpose: called on level start
//-----------------------------------------------------------------------------
void CSourcePython::ClientSettingsChanged( edict_t *pEdict )
{
	CALL_LISTENERS(ClientSettingsChanged, IndexFromEdict(pEdict));
}

//-----------------------------------------------------------------------------
// Purpose: called when a client joins a server
//-----------------------------------------------------------------------------
PLUGIN_RESULT CSourcePython::ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	CPointer allowConnect = CPointer((unsigned long) bAllowConnect);
	CPointer rejectMessage = CPointer((unsigned long) reject);
	CALL_LISTENERS(ClientConnect, allowConnect, IndexFromEdict(pEntity), pszName, pszAddress, rejectMessage, maxrejectlen);
	return PLUGIN_OVERRIDE;
}

//-----------------------------------------------------------------------------
// Purpose: called when a client is authenticated
//-----------------------------------------------------------------------------
PLUGIN_RESULT CSourcePython::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
	CALL_LISTENERS(NetworkidValidated, pszUserName, pszNetworkID);
	return PLUGIN_CONTINUE;
}

//-----------------------------------------------------------------------------
// Purpose: called when a cvar value query is finished
//-----------------------------------------------------------------------------
void CSourcePython::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity,
	EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
{
	PythonLog(1, "Cvar query (cookie: %d, status: %d) - name: %s, value: %s\n", iCookie, eStatus, pCvarName, pCvarValue );
	CALL_LISTENERS(OnQueryCvarValueFinished, (int) iCookie, IndexFromEdict(pPlayerEntity), eStatus, pCvarName, pCvarValue);
}

//-----------------------------------------------------------------------------
// Purpose: called when an event is fired
//-----------------------------------------------------------------------------
void CSourcePython::FireGameEvent( IGameEvent * event )
{
	const char * name = event->GetName();
	PythonLog(1, "CSourcePython::FireGameEvent: Got event \"%s\"\n", name );

	//g_AddonManager.FireGameEvent(event);
}

//-----------------------------------------------------------------------------
// Orangebox.
//-----------------------------------------------------------------------------
PLUGIN_RESULT CSourcePython::ClientCommand( edict_t *pEntity, const CCommand &args )
{
	return DispatchClientCommand(pEntity, args);
}

//-----------------------------------------------------------------------------
// Alien Swarm.
//-----------------------------------------------------------------------------
#ifdef ENGINE_CSGO
void CSourcePython::ClientFullyConnect( edict_t *pEntity )
{
	CALL_LISTENERS(ClientFullyConnect, IndexFromEdict(pEntity));
}

void CSourcePython::OnEdictAllocated( edict_t *edict )
{
	CALL_LISTENERS(OnEdictAllocated, IndexFromEdict(edict));
}

void CSourcePython::OnEdictFreed( const edict_t *edict )
{
	CALL_LISTENERS(OnEdictFreed, ptr(edict));
}
#endif

#ifdef ENGINE_BMS
void CSourcePython::OnEdictAllocated( edict_t *edict )
{
	CALL_LISTENERS(OnEdictAllocated, IndexFromEdict(edict));
}

void CSourcePython::OnEdictFreed( const edict_t *edict )
{
	CALL_LISTENERS(OnEdictFreed, ptr(edict));
}
#endif

void CSourcePython::OnEntityCreated( CBaseEntity *pEntity )
{
	int iIndex = IndexFromBaseEntity(pEntity);
	edict_t *pEdict = EdictFromIndex(iIndex);
	if (pEdict)
	{
		IServerUnknown* pServerUnknown = pEdict->GetUnknown();
		if (pServerUnknown)
			pEdict->m_pNetworkable = pServerUnknown->GetNetworkable();
	}
	CPointer pAddress = CPointer((unsigned long) pEntity);
	CALL_LISTENERS(OnEntityCreated, iIndex, ptr(&pAddress));
}

void CSourcePython::OnEntitySpawned( CBaseEntity *pEntity )
{
	CPointer pAddress = CPointer((unsigned long) pEntity);
	CALL_LISTENERS(OnEntitySpawned, IndexFromBaseEntity(pEntity), ptr(&pAddress));
}

void CSourcePython::OnEntityDeleted( CBaseEntity *pEntity )
{
	CPointer pAddress = CPointer((unsigned long) pEntity);
	CALL_LISTENERS(OnEntityDeleted, IndexFromBaseEntity(pEntity), ptr(&pAddress));
}

void CSourcePython::OnDataLoaded( MDLCacheDataType_t type, MDLHandle_t handle )
{
	if (m_pOldMDLCacheNotifier)
		m_pOldMDLCacheNotifier->OnDataLoaded(type, handle);

	CALL_LISTENERS(OnDataLoaded, type, handle);
}

void CSourcePython::OnDataUnloaded( MDLCacheDataType_t type, MDLHandle_t handle )
{
	if (m_pOldMDLCacheNotifier)
		m_pOldMDLCacheNotifier->OnDataUnloaded(type, handle);

	CALL_LISTENERS(OnDataUnloaded, type, handle);
}
	
#ifdef ENGINE_CSGO
void CSourcePython::OnCombinerPreCache(MDLCacheDataType_t type, MDLHandle_t handle )
{
	if (m_pOldMDLCacheNotifier)
		m_pOldMDLCacheNotifier->OnCombinerPreCache(type, handle);

	CALL_LISTENERS(OnCombinerPreCache, type, handle);
}

bool CSourcePython::ShouldSupressLoadWarning(unsigned short a)
{
	bool result = false;
	if (m_pOldMDLCacheNotifier)
		result = m_pOldMDLCacheNotifier->ShouldSupressLoadWarning(a);

	return result;
}
#endif

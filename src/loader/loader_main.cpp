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

//---------------------------------------------------------------------------------
// Source includes
//---------------------------------------------------------------------------------
#include "loader_main.h"
#include "updater.h"
#include "interface.h"
#include "eiface.h"
#include "strtools.h"
#ifdef _WIN32
#	include <windows.h>
#endif

#include "../core/utilities/shared_utils.h"
#include <exception>

//---------------------------------------------------------------------------------
// Disable warnings.
//---------------------------------------------------------------------------------
#if defined(_WIN32)
#	pragma warning( disable : 4005 )
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//---------------------------------------------------------------------------------
// Interfaces.
//---------------------------------------------------------------------------------
ICvar* g_pCVar = NULL; // This is required for linux linking..
IVEngineServer* engine = NULL;

//
// The plugin is a static singleton that is exported as an interface
//
CSourcePython g_EmtpyServerPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CSourcePython, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_EmtpyServerPlugin );

//---------------------------------------------------------------------------------
// Purpose: constructor/destructor
//---------------------------------------------------------------------------------
CSourcePython::CSourcePython()
{
	m_iClientCommandIndex = 0;
	m_pCore = NULL;
}

CSourcePython::~CSourcePython()
{
}

//---------------------------------------------------------------------------------
// This function will load libraries and return true if they load successfully.
//
//---------------------------------------------------------------------------------
void* SPLoadLibrary( IVEngineServer* engine, const char* libraryPath )
{
	DevMsg(1, MSG_PREFIX "Loading library: %s\n", libraryPath);
	char szFullPath[MAX_PATH_LENGTH];
	char szGamePath[MAX_PATH_LENGTH];
	char szError[MAX_PATH_LENGTH];
	V_strncpy(szError, "[SP-LOADER] - No error found\n", MAX_PATH_LENGTH);

	engine->GetGameDir(szGamePath, MAX_PATH_LENGTH);
	GenerateSymlink(szGamePath);

	V_snprintf(szFullPath, sizeof(szFullPath), "%s/addons/source-python/%s",
		szGamePath, libraryPath);

	// Fix up the slahes.
	V_FixSlashes(szFullPath);

#if defined(_WIN32)
	void* hModule = (void *)LoadLibrary(szFullPath);
	if (!hModule)
	{
		// I hate windows programming...
		DWORD nErrorCode = GetLastError();
		LPVOID lpMsgBuf;
		DWORD nBufferLength = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			nErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		if(nBufferLength == 0)

		{
			V_snprintf(szError, sizeof(szError),
			   "[SP-LOADER] Could not obtain a valid translation for error. (Code: %d)\n",
			   nErrorCode);
		}
		else
		{
			char szResult[MAX_ERROR_LENGTH];
			memset(szResult, '\0', MAX_ERROR_LENGTH);

			// Too lazy to bring across any windows functions, cast myself from wide to narrow
			for (unsigned int i=0; i < nBufferLength; ++i)
			{
				szResult[i] = static_cast<char>(static_cast<LPCSTR>(lpMsgBuf)[i]);
			}

			V_snprintf(szError, sizeof(szError),
			   "[SP-LOADER] (Code: %d) %s",
			   nErrorCode,
			   szResult/*.c_str()*/);

			LocalFree(lpMsgBuf);
		}
	}
#else
	void* hModule = (void *)dlopen(szFullPath, RTLD_NOW | RTLD_GLOBAL);
	if (!hModule)
	{
		V_snprintf(szError, sizeof(szError), "[SP-LOADER] Error Reported: %s\n",
			dlerror());
	}
#endif

	if( !hModule ) {
		Warning("=========================================================================\n");
		Warning("[SP-LOADER] Could not load library: %s\n", libraryPath);
		Warning(szError);
		Warning("=========================================================================\n");
		return hModule;
	}

	return hModule;
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is loaded, load the interface we need from the engine
//---------------------------------------------------------------------------------
bool CSourcePython::Load( CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory )
{
	Msg(MSG_PREFIX "Loading...\n");

	engine = (IVEngineServer*)interfaceFactory(INTERFACEVERSION_VENGINESERVER, NULL);

	// Was the IVEngineServer interface retrieved properly?
	if (!engine)
	{
		Msg(MSG_PREFIX "Unable to retrieve IVEngineServer interface.\n");
		return false;
	}

	// ------------------------------------------------------------------
	// Build path to python engines directory.
	// ------------------------------------------------------------------
	char szGameDir[MAX_PATH_LENGTH];

	// ------------------------------------------------------------------
	// Get the game directory.
	// ------------------------------------------------------------------
	DevMsg(1, MSG_PREFIX "Retrieving game directory...\n");
	engine->GetGameDir(szGameDir, MAX_PATH_LENGTH);
	DevMsg(1, MSG_PREFIX "Game directory: %s\n", szGameDir);
	GenerateSymlink(szGameDir);

	if (UpdateAvailable())
	{
		try
		{
			ApplyUpdateStage2();
		}
		catch (const std::exception& e)
		{
			Msg(MSG_PREFIX "An error occured during update stage 2:\n%s\n", e.what());
			return false;
		}
	}

	// ------------------------------------------------------------------
	// Load windows dependencies.
	// ------------------------------------------------------------------
#if defined(_WIN32)
	if( SPLoadLibrary(engine, VCRUNTIME_LIB) == NULL ) {
		return false;
	}
#endif

	// ------------------------------------------------------------------
	// Load python.
	// ------------------------------------------------------------------
	if( SPLoadLibrary(engine, PYLIB_NAME) == NULL ) {
		return false;
	}

	// ------------------------------------------------------------------
	// Load the Source.Python core.
	// ------------------------------------------------------------------
	m_pCore = SPLoadLibrary(engine, CORE_NAME);
	if (!m_pCore) {
		return false;
	}

	// Sys_LoadModule and CDllDemandLoader seem to be broken in CS:GO
#ifdef _WIN32
	CreateInterfaceFn pFunc = (CreateInterfaceFn) GetProcAddress((HMODULE) m_pCore, CREATEINTERFACE_PROCNAME);
#elif __linux__
	CreateInterfaceFn pFunc = (CreateInterfaceFn) dlsym(m_pCore, CREATEINTERFACE_PROCNAME);
#else
	#error Unsupported platform.
#endif
	if (!pFunc) {
		Msg(MSG_PREFIX "Failed to retrieve %s.\n", CREATEINTERFACE_PROCNAME);
		return false;
	}

	m_pCorePlugin = static_cast<IServerPluginCallbacks*>(pFunc(INTERFACEVERSION_ISERVERPLUGINCALLBACKS, NULL));
	if( !m_pCorePlugin ) {
		Warning("=========================================================================\n");
		Warning("[SP-LOADER] Could not retrieve the server plugin interface from the core!\n");
		Warning("=========================================================================\n");
		return false;
	}

	// Pass it on.
	return m_pCorePlugin->Load(interfaceFactory, gameServerFactory);
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unloaded (turned off)
//---------------------------------------------------------------------------------
void CSourcePython::Unload( void )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->Unload();
	}
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is paused (i.e should stop running but isn't unloaded)
//---------------------------------------------------------------------------------
void CSourcePython::Pause( void )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->Pause();
	}
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unpaused (i.e should start executing again)
//---------------------------------------------------------------------------------
void CSourcePython::UnPause( void )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->UnPause();
	}
}

//---------------------------------------------------------------------------------
// Purpose: the name of this plugin, returned in "plugin_print" command
//---------------------------------------------------------------------------------
const char *CSourcePython::GetPluginDescription( void )
{
	if (m_pCorePlugin != NULL)
	{
		return m_pCorePlugin->GetPluginDescription();
	}
	return "Error loading core plugin.";
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CSourcePython::LevelInit( char const *pMapName )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->LevelInit(pMapName);
	}
}

//---------------------------------------------------------------------------------
// Purpose: called on level start, when the server is ready to accept client connections
//		edictCount is the number of entities in the level, clientMax is the max client count
//---------------------------------------------------------------------------------
void CSourcePython::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->ServerActivate(pEdictList, edictCount, clientMax);
	}
}

//---------------------------------------------------------------------------------
// Purpose: called once per server frame, do recurring work here (like checking for timeouts)
//---------------------------------------------------------------------------------
void CSourcePython::GameFrame( bool simulating )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->GameFrame(simulating);
	}
}

//---------------------------------------------------------------------------------
// Purpose: called on level end (as the server is shutting down or going to a new map)
//---------------------------------------------------------------------------------
void CSourcePython::LevelShutdown( void ) // !!!!this can get called multiple times per map change
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->LevelShutdown();
	}
}

//---------------------------------------------------------------------------------
// Purpose: called when a client spawns into a server (i.e as they begin to play)
//---------------------------------------------------------------------------------
void CSourcePython::ClientActive( edict_t *pEntity )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->ClientActive(pEntity);
	}
}

//---------------------------------------------------------------------------------
// Purpose: called when a client leaves a server (or is timed out)
//---------------------------------------------------------------------------------
void CSourcePython::ClientDisconnect( edict_t *pEntity )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->ClientDisconnect(pEntity);
	}
}

//---------------------------------------------------------------------------------
// Purpose: called on
//---------------------------------------------------------------------------------
void CSourcePython::ClientPutInServer( edict_t *pEntity, char const *playername )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->ClientPutInServer(pEntity, playername);
	}
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CSourcePython::SetCommandClient( int index )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->SetCommandClient(index);
	}
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CSourcePython::ClientSettingsChanged( edict_t *pEdict )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->ClientSettingsChanged(pEdict);
	}
}

//---------------------------------------------------------------------------------
// Purpose: called when a client joins a server
//---------------------------------------------------------------------------------
PLUGIN_RESULT CSourcePython::ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	if (m_pCorePlugin != NULL)
	{
		return m_pCorePlugin->ClientConnect(bAllowConnect, pEntity, pszName, pszAddress, reject, maxrejectlen);
	}
	return PLUGIN_STOP;
}

//---------------------------------------------------------------------------------
// Purpose: called when a client is authenticated
//---------------------------------------------------------------------------------
PLUGIN_RESULT CSourcePython::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
	if (m_pCorePlugin != NULL)
	{
		return m_pCorePlugin->NetworkIDValidated(pszUserName, pszNetworkID);
	}
	return PLUGIN_STOP;
}

//---------------------------------------------------------------------------------
// Purpose: called when a cvar value query is finished
//---------------------------------------------------------------------------------
void CSourcePython::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity,
	EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->OnQueryCvarValueFinished(iCookie, pPlayerEntity, eStatus, pCvarName, pCvarValue);
	}
}

//---------------------------------------------------------------------------------
// Orangebox.
//---------------------------------------------------------------------------------
PLUGIN_RESULT CSourcePython::ClientCommand( edict_t *pEntity, const CCommand &args )
{
	if (m_pCorePlugin != NULL)
	{
		return m_pCorePlugin->ClientCommand(pEntity, args);
	}
	return PLUGIN_STOP;
}

//---------------------------------------------------------------------------------
// Alien Swarm.
//---------------------------------------------------------------------------------
#if defined(ENGINE_CSGO) || defined(ENGINE_BLADE)
void CSourcePython::ClientFullyConnect( edict_t *pEntity )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->ClientFullyConnect(pEntity);
	}
}
#endif

#if defined(ENGINE_CSGO) || defined(ENGINE_BMS) || defined(ENGINE_BLADE)
void CSourcePython::OnEdictAllocated( edict_t *edict )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->OnEdictAllocated(edict);
	}
}

void CSourcePython::OnEdictFreed( const edict_t *edict )
{
	if (m_pCorePlugin != NULL)
	{
		m_pCorePlugin->OnEdictFreed(edict);
	}
}
#endif

#if defined(ENGINE_CSGO)
bool CSourcePython::BNetworkCryptKeyCheckRequired( uint32 unFromIP, uint16 usFromPort,
	uint32 unAccountIdProvidedByClient, bool bClientWantsToUseCryptKey )
{
	if (m_pCorePlugin != NULL)
	{
		return m_pCorePlugin->BNetworkCryptKeyCheckRequired(unFromIP, usFromPort, unAccountIdProvidedByClient, bClientWantsToUseCryptKey);
	}
	return false;
}

bool CSourcePython::BNetworkCryptKeyValidate( uint32 unFromIP, uint16 usFromPort, uint32 unAccountIdProvidedByClient,
	int nEncryptionKeyIndexFromClient, int numEncryptedBytesFromClient, byte *pbEncryptedBufferFromClient,
	byte *pbPlainTextKeyForNetchan )
{
	if (m_pCorePlugin != NULL)
	{
		return m_pCorePlugin->BNetworkCryptKeyValidate( unFromIP, usFromPort, unAccountIdProvidedByClient,
			nEncryptionKeyIndexFromClient, numEncryptedBytesFromClient, pbEncryptedBufferFromClient,
			pbPlainTextKeyForNetchan );
	}
	return false;
}
#endif

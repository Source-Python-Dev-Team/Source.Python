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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "modules/export_main.h"
#include "utility/wrap_macros.h"
#include "listenermanager.h"


// Create manager accessor functions
DEFINE_MANAGER_ACCESSOR(ClientActive)
DEFINE_MANAGER_ACCESSOR(ClientConnect)
DEFINE_MANAGER_ACCESSOR(ClientDisconnect)
DEFINE_MANAGER_ACCESSOR(ClientFullyConnect)
DEFINE_MANAGER_ACCESSOR(ClientPutInServer)
DEFINE_MANAGER_ACCESSOR(ClientSettingsChanged)
DEFINE_MANAGER_ACCESSOR(LevelInit)
DEFINE_MANAGER_ACCESSOR(LevelShutdown)
DEFINE_MANAGER_ACCESSOR(NetworkidValidated)
DEFINE_MANAGER_ACCESSOR(OnEdictAllocated)
DEFINE_MANAGER_ACCESSOR(OnEdictFreed)
DEFINE_MANAGER_ACCESSOR(OnQueryCvarValueFinished)
DEFINE_MANAGER_ACCESSOR(ServerActivate)
DEFINE_MANAGER_ACCESSOR(Tick)

//-----------------------------------------------------------------------------
// Exposes the listener_c module.
//-----------------------------------------------------------------------------
void export_listener_managers();

DECLARE_SP_MODULE(listener_c)
{
	export_listener_managers();
}

void export_listener_managers() 
{
    //-------------------------------------------------------------------------
	// Exposes ListenerManager
	//-------------------------------------------------------------------------
	class_<CListenerManager, boost::noncopyable>("ListenerManager", no_init)
		.def("register_listener",
			&CListenerManager::RegisterListener,
			"Registers a callable object. If it was already registered it will be ignored.",
			args("callable")
		)

		.def("unregister_listener",
			&CListenerManager::UnregisterListener,
			"Removes a callable object. If it was not registered nothing will happen.",
			args("callable")
		)
	;

	//-------------------------------------------------------------------------
	// Expose the accessor functions
	//-------------------------------------------------------------------------
	scope().attr("ClientActiveListenerManager") = object(ptr(GetClientActiveListenerManager()));
	scope().attr("ClientConnectListenerManager") = object(ptr(GetClientConnectListenerManager()));
	scope().attr("ClientDisconnectListenerManager") = object(ptr(GetClientDisconnectListenerManager()));
	scope().attr("ClientFullyConnectListenerManager") = object(ptr(GetClientFullyConnectListenerManager()));
	scope().attr("ClientPutInServerListenerManager") = object(ptr(GetClientPutInServerListenerManager()));
	scope().attr("ClientSettingsChangedListenerManager") = object(ptr(GetClientSettingsChangedListenerManager()));
	
	scope().attr("LevelInitListenerManager") = object(ptr(GetLevelInitListenerManager()));
	scope().attr("LevelShutdownListenerManager") = object(ptr(GetLevelShutdownListenerManager()));
	
	scope().attr("NetworkidValidatedListenerManager") = object(ptr(GetNetworkidValidatedListenerManager()));
	
	scope().attr("OnEdictAllocatedListenerManager") = object(ptr(GetOnEdictAllocatedListenerManager()));
	scope().attr("OnEdictFreedListenerManager") = object(ptr(GetOnEdictFreedListenerManager()));
	
	scope().attr("OnQueryCvarValueFinishedListenerManager") = object(ptr(GetOnQueryCvarValueFinishedListenerManager()));
	
	scope().attr("ServerActivateListenerManager") = object(ptr(GetServerActivateListenerManager()));

	scope().attr("TickListenerManager") = object(ptr(GetTickListenerManager()));
}
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
#include "listeners_manager.h"


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

DECLARE_SP_MODULE(_listeners)
{
	export_listener_managers();
}

void export_listener_managers() 
{
    //-------------------------------------------------------------------------
	// Exposes ListenerManager
	//-------------------------------------------------------------------------
	class_<CListenerManager, boost::noncopyable>("_ListenerManager")
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

		.def("notify",
			raw_method(&CListenerManager::Notify),
			"Notifies all registered listeners."
		)
	;

	//-------------------------------------------------------------------------
	// Expose the accessor functions
	//-------------------------------------------------------------------------
	scope().attr("client_active_listener_manager") = object(ptr(GetClientActiveListenerManager()));
	scope().attr("client_connect_listener_manager") = object(ptr(GetClientConnectListenerManager()));
	scope().attr("client_disconnect_listener_manager") = object(ptr(GetClientDisconnectListenerManager()));
	scope().attr("client_fully_connect_listener_manager") = object(ptr(GetClientFullyConnectListenerManager()));
	scope().attr("client_put_in_server_listener_manager") = object(ptr(GetClientPutInServerListenerManager()));
	scope().attr("client_settings_changed_listener_manager") = object(ptr(GetClientSettingsChangedListenerManager()));
	
	scope().attr("level_init_listener_manager") = object(ptr(GetLevelInitListenerManager()));
	scope().attr("level_shutdown_listener_manager") = object(ptr(GetLevelShutdownListenerManager()));
	
	scope().attr("network_id_validated_listener_manager") = object(ptr(GetNetworkidValidatedListenerManager()));
	
	scope().attr("on_edict_allocated_listener_manager") = object(ptr(GetOnEdictAllocatedListenerManager()));
	scope().attr("on_edict_free_listener_manager") = object(ptr(GetOnEdictFreedListenerManager()));
	
	scope().attr("on_query_cvar_value_finished_listener_manager") = object(ptr(GetOnQueryCvarValueFinishedListenerManager()));
	
	scope().attr("server_activate_listener_manager") = object(ptr(GetServerActivateListenerManager()));

	scope().attr("tick_listener_manager") = object(ptr(GetTickListenerManager()));
}

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

//-----------------------------------------------------------------------------
// Functions that expose tick listener functionality to us.
//-----------------------------------------------------------------------------
void export_listener_managers();

//-----------------------------------------------------------------------------
// Exposes the listener_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(listener_c)
{
	export_listener_managers();
}

void export_listener_managers() 
{
    //-------------------------------------------------------------------------
	// Exposes CListenerManager
	//-------------------------------------------------------------------------
	class_<CListenerManager, boost::noncopyable>("CListenerManager", no_init)
		.def("register_listener",
			&CListenerManager::RegisterListener,
			"Registers a callable object. If it was already registered it will be ignored.",
			args("callable")
		)

		.def("register_listener",
			&CListenerManager::UnregisterListener,
			"Removes a callable object. If it was not registered nothing will happen.",
			args("callable")
		)
	;

	//-------------------------------------------------------------------------
	// Expose the accessor functions
	//-------------------------------------------------------------------------
	def("get_client_active_listener_manager",
		&GetClientActiveListenerManager,
		"Returns the clientactive listener instance",
		reference_existing_object_policy()
	);
	
	def("get_client_connect_listener_manager",
		&GetClientConnectListenerManager,
		"Returns the clientconnect listener instance",
		reference_existing_object_policy()
	);

	def("get_client_disconnect_listener_manager",
		&GetClientDisconnectListenerManager,
		"Returns the clientdisconnect listener instance",
		reference_existing_object_policy()
	);

	def("get_client_fully_connect_listener_manager",
		&GetClientFullyConnectListenerManager,
		"Returns the clientfullyconnect listener instance",
		reference_existing_object_policy()
	);

	def("get_client_put_in_server_listener_manager",
		&GetClientPutInServerListenerManager,
		"Returns the clientputinserver listener instance",
		reference_existing_object_policy()
	);

	def("get_client_settings_changed_listener_manager",
		&GetClientSettingsChangedListenerManager,
		"Returns the clientsettingschanged listener instance",
		reference_existing_object_policy()
	);

	def("get_level_init_listener_manager",
		&GetLevelInitListenerManager,
		"Returns the levelinit listener instance",
		reference_existing_object_policy()
	);

	def("get_level_shutdown_listener_manager",
		&GetLevelShutdownListenerManager,
		"Returns the levelshutdown listener instance",
		reference_existing_object_policy()
	);

	def("get_networkid_validated_listener_manager",
		&GetNetworkidValidatedListenerManager,
		"Returns the networkidvalidated listener instance",
		reference_existing_object_policy()
	);

	def("get_on_edict_allocated_listener_manager",
		&GetOnEdictAllocatedListenerManager,
		"Returns the onedictallocated listener instance",
		reference_existing_object_policy()
	);

	def("get_on_edict_freed_listener_manager",
		&GetOnEdictFreedListenerManager,
		"Returns the onedictfreed listener instance",
		reference_existing_object_policy()
	);

	def("get_on_query_cvar_value_finished_listener_manager",
		&GetOnQueryCvarValueFinishedListenerManager,
		"Returns the onquerycvarvaluefinished listener instance",
		reference_existing_object_policy()
	);

	def("get_server_activate_listener_manager",
		&GetServerActivateListenerManager,
		"Returns the serveractivate listener instance",
		reference_existing_object_policy()
	);

	def("get_tick_listener_manager",
		&GetTickListenerManager,
		"Returns the tick listener instance",
		reference_existing_object_policy()
	);
}
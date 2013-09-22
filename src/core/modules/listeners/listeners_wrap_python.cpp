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
#include "listeners.h"
#include "utility/wrap_macros.h"

//-----------------------------------------------------------------------------
// Functions that expose tick listener functionality to us.
//-----------------------------------------------------------------------------
void export_listener_managers();

//-----------------------------------------------------------------------------
// Exposes the tick_c module.
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
	// This is the base class all other listener managers inherit from
	BOOST_ABSTRACT_CLASS(CListenerManager)

		CLASS_METHOD(CListenerManager,
			register_listener,
			"Adds the given callable to the end of the listener vector.",
			args("pCallable")
		)

		CLASS_METHOD(CListenerManager,
			unregister_listener,
			"Removes the given callable from the listener vector.",
			args("pCallable")
		)

	BOOST_END_CLASS()

	//-------------------------------------------------------------------------
	// Exposes CClientActiveListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(CClientActiveListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_client_active_listener_manager,
		"Returns the CClientActiveListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes CClientDisconnectListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(CClientDisconnectListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_client_disconnect_listener_manager,
		"Returns the CClientDisconnectListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes CClientFullyConnectListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(CClientFullyConnectListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_client_fully_connect_listener_manager,
		"Returns the CClientFullyConnectListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes CClientPutInServerListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(CClientPutInServerListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_client_put_in_server_listener_manager,
		"Returns the CClientPutInServerListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes CClientSettingsChangedListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(CClientSettingsChangedListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_client_settings_changed_listener_manager,
		"Returns the CClientSettingsChangedListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes CLevelInitListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(CLevelInitListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_level_init_listener_manager,
		"Returns the CLevelInitListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes CLevelShutdownListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(CLevelShutdownListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_level_shutdown_listener_manager,
		"Returns the CLevelShutdownListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes CNetworkidValidatedListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(CNetworkIDValidatedListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_networkid_validated_listener_manager,
		"Returns the CNetworkidValidatedListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes COnEdictAllocatedListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(COnEdictAllocatedListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_on_edict_allocated_listener_manager,
		"Returns the COnEdictAllocatedListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes COnEdictFreedListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(COnEdictFreedListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_on_edict_freed_listener_manager,
		"Returns the COnEdictFreedListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes COnQueryCvarValueFinishedListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(COnQueryCvarValueFinishedListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_on_query_cvar_value_finished_listener_manager,
		"Returns the COnQueryCvarValueFinishedListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes CServerActivateListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(CServerActivateListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_server_activate_listener_manager,
		"Returns the CServerActivateListenerManager instance",
		reference_existing_object_policy()
	);

	//-------------------------------------------------------------------------
	// Exposes CTickListenerManager
	//-------------------------------------------------------------------------
	BOOST_ABSTRACT_CLASS_INHERITED(CTickListenerManager, CListenerManager)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_tick_listener_manager,
		"Returns the CTickListenerManager instance",
		reference_existing_object_policy()
	);
}
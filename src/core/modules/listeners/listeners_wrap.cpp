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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "listeners_manager.h"
#include "modules/entities/entities_collisions.h"
#include "modules/entities/entities_transmit.h"
#include "modules/players/players_movements.h"


//-----------------------------------------------------------------------------
// Listener accessors.
//-----------------------------------------------------------------------------
// Create manager accessor functions
DEFINE_MANAGER_ACCESSOR(OnClientActive)
DEFINE_MANAGER_ACCESSOR(OnClientConnect)
DEFINE_MANAGER_ACCESSOR(OnClientDisconnect)
DEFINE_MANAGER_ACCESSOR(OnClientFullyConnect)
DEFINE_MANAGER_ACCESSOR(OnClientPutInServer)
DEFINE_MANAGER_ACCESSOR(OnClientSettingsChanged)
DEFINE_MANAGER_ACCESSOR(OnLevelInit)
DEFINE_MANAGER_ACCESSOR(OnLevelShutdown)
DEFINE_MANAGER_ACCESSOR(OnNetworkidValidated)
DEFINE_MANAGER_ACCESSOR(OnEdictAllocated)
DEFINE_MANAGER_ACCESSOR(OnEdictFreed)
DEFINE_MANAGER_ACCESSOR(OnQueryCvarValueFinished)
DEFINE_MANAGER_ACCESSOR(OnServerActivate)
DEFINE_MANAGER_ACCESSOR(OnTick)
DEFINE_MANAGER_ACCESSOR(OnEntityPreSpawned)
DEFINE_MANAGER_ACCESSOR(OnNetworkedEntityPreSpawned)
DEFINE_MANAGER_ACCESSOR(OnEntityCreated)
DEFINE_MANAGER_ACCESSOR(OnNetworkedEntityCreated)
DEFINE_MANAGER_ACCESSOR(OnEntitySpawned)
DEFINE_MANAGER_ACCESSOR(OnNetworkedEntitySpawned)
DEFINE_MANAGER_ACCESSOR(OnEntityDeleted)
DEFINE_MANAGER_ACCESSOR(OnNetworkedEntityDeleted)
DEFINE_MANAGER_ACCESSOR(OnDataLoaded)
DEFINE_MANAGER_ACCESSOR(OnCombinerPreCache)
DEFINE_MANAGER_ACCESSOR(OnDataUnloaded)
DEFINE_MANAGER_ACCESSOR(OnPlayerRunCommand)
DEFINE_MANAGER_ACCESSOR(OnPlayerPostRunCommand)
DEFINE_MANAGER_ACCESSOR(OnButtonStateChanged)

static CConVarChangedListenerManager s_OnConVarChanged;
CConVarChangedListenerManager* GetOnConVarChangedListenerManager()
{
	return &s_OnConVarChanged;
}

static CServerOutputListenerManager s_OnServerOutput;
CServerOutputListenerManager *GetOnServerOutputListenerManager()
{
	return &s_OnServerOutput;
}


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_listener_managers(scope);


//-----------------------------------------------------------------------------
// Declare the _listeners module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_listeners)
{
	export_listener_managers(_listeners);
}


//-----------------------------------------------------------------------------
// Exports CListenerManager.
//-----------------------------------------------------------------------------
void export_listener_managers(scope _listeners) 
{
	class_<CListenerManager, boost::noncopyable>("ListenerManager")
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

		.def("__len__",
			&CListenerManager::GetCount,
			"Return the number of registered listeners."
		)

		.def("__contains__",
			&CListenerManager::IsRegistered,
			"Return whether or not the given callback is registered."
		)

		.def("__getitem__",
			&CListenerManager::__getitem__,
			"Return the callback at the given index."
		)

		.def("clear",
			&CListenerManager::clear,
			"Remove all registered callbacks."
		)

		.def("initialize",
			&CListenerManager::Initialize,
			"Called when the first callback is being registered."
		)

		.def("finalize",
			&CListenerManager::Finalize,
			"Called when the last callback is being unregistered."
		)
	;

	_listeners.attr("on_client_active_listener_manager") = object(ptr(GetOnClientActiveListenerManager()));
	_listeners.attr("on_client_connect_listener_manager") = object(ptr(GetOnClientConnectListenerManager()));
	_listeners.attr("on_client_disconnect_listener_manager") = object(ptr(GetOnClientDisconnectListenerManager()));
	_listeners.attr("on_client_fully_connect_listener_manager") = object(ptr(GetOnClientFullyConnectListenerManager()));
	_listeners.attr("on_client_put_in_server_listener_manager") = object(ptr(GetOnClientPutInServerListenerManager()));
	_listeners.attr("on_client_settings_changed_listener_manager") = object(ptr(GetOnClientSettingsChangedListenerManager()));

	_listeners.attr("on_convar_changed_listener_manager") = object(ptr((CListenerManager *)GetOnConVarChangedListenerManager()));

	_listeners.attr("on_level_init_listener_manager") = object(ptr(GetOnLevelInitListenerManager()));
	_listeners.attr("on_level_shutdown_listener_manager") = object(ptr(GetOnLevelShutdownListenerManager()));

	_listeners.attr("on_network_id_validated_listener_manager") = object(ptr(GetOnNetworkidValidatedListenerManager()));

	_listeners.attr("on_edict_allocated_listener_manager") = object(ptr(GetOnEdictAllocatedListenerManager()));
	_listeners.attr("on_edict_freed_listener_manager") = object(ptr(GetOnEdictFreedListenerManager()));

	_listeners.attr("on_query_cvar_value_finished_listener_manager") = object(ptr(GetOnQueryCvarValueFinishedListenerManager()));

	_listeners.attr("on_server_activate_listener_manager") = object(ptr(GetOnServerActivateListenerManager()));

	_listeners.attr("on_tick_listener_manager") = object(ptr(GetOnTickListenerManager()));
	
	_listeners.attr("on_entity_pre_spawned_listener_manager") = object(ptr(GetOnEntityPreSpawnedListenerManager()));
	_listeners.attr("on_networked_entity_pre_spawned_listener_manager") = object(ptr(GetOnNetworkedEntityPreSpawnedListenerManager()));
	_listeners.attr("on_entity_created_listener_manager") = object(ptr(GetOnEntityCreatedListenerManager()));
	_listeners.attr("on_networked_entity_created_listener_manager") = object(ptr(GetOnNetworkedEntityCreatedListenerManager()));
	_listeners.attr("on_entity_spawned_listener_manager") = object(ptr(GetOnEntitySpawnedListenerManager()));
	_listeners.attr("on_networked_entity_spawned_listener_manager") = object(ptr(GetOnNetworkedEntitySpawnedListenerManager()));
	_listeners.attr("on_entity_deleted_listener_manager") = object(ptr(GetOnEntityDeletedListenerManager()));
	_listeners.attr("on_networked_entity_deleted_listener_manager") = object(ptr(GetOnNetworkedEntityDeletedListenerManager()));
	_listeners.attr("on_entity_collision_listener_manager") = object(ptr((CListenerManager *)GetOnEntityCollisionListenerManager()));
	_listeners.attr("on_player_collision_listener_manager") = object(ptr((CListenerManager *)GetOnPlayerCollisionListenerManager()));
	_listeners.attr("on_entity_transmit_listener_manager") = object(ptr((CListenerManager *)GetOnEntityTransmitListenerManager()));
	_listeners.attr("on_player_transmit_listener_manager") = object(ptr((CListenerManager *)GetOnPlayerTransmitListenerManager()));

	_listeners.attr("on_data_loaded_listener_manager") = object(ptr(GetOnDataLoadedListenerManager()));
	_listeners.attr("on_combiner_pre_cache_listener_manager") = object(ptr(GetOnCombinerPreCacheListenerManager()));
	_listeners.attr("on_data_unloaded_listener_manager") = object(ptr(GetOnDataUnloadedListenerManager()));
	
	_listeners.attr("on_server_output_listener_manager") = object(ptr((CListenerManager *)GetOnServerOutputListenerManager()));
	
	_listeners.attr("on_player_run_command_listener_manager") = object(ptr(GetOnPlayerRunCommandListenerManager()));
	_listeners.attr("on_player_post_run_command_listener_manager") = object(ptr(GetOnPlayerPostRunCommandListenerManager()));
	_listeners.attr("on_button_state_changed_listener_manager") = object(ptr(GetOnButtonStateChangedListenerManager()));

	// Movement listeners...
	_listeners.attr("on_player_jump_listener_manager") = object(ptr(GetOnPlayerJumpListenerManager()));
	_listeners.attr("on_player_land_listener_manager") = object(ptr(GetOnPlayerLandListenerManager()));
	_listeners.attr("on_player_duck_listener_manager") = object(ptr(GetOnPlayerDuckListenerManager()));
	_listeners.attr("on_player_ducked_listener_manager") = object(ptr(GetOnPlayerDuckedListenerManager()));
	_listeners.attr("on_player_unduck_listener_manager") = object(ptr(GetOnPlayerUnduckListenerManager()));
	_listeners.attr("on_player_unducked_listener_manager") = object(ptr(GetOnPlayerUnduckedListenerManager()));
	_listeners.attr("on_player_water_jump_listener_manager") = object(ptr(GetOnPlayerWaterJumpListenerManager()));
	_listeners.attr("on_player_ground_entity_changed_listener_manager") = object(ptr(GetOnPlayerGroundEntityChangedListenerManager()));
	_listeners.attr("on_player_water_level_changed_listener_manager") = object(ptr(GetOnPlayerWaterLevelChangedListenerManager()));
	_listeners.attr("on_player_start_climbing_ladder_listener_manager") = object(ptr(GetOnPlayerStartClimbingLadderListenerManager()));
	_listeners.attr("on_player_stop_climbing_ladder_listener_manager") = object(ptr(GetOnPlayerStopClimbingLadderListenerManager()));
	_listeners.attr("on_player_check_jump_button_listener_manager") = object(ptr(GetOnPlayerCheckJumpButtonListenerManager()));
	_listeners.attr("on_player_rough_landing_effects_listener_manager") = object(ptr(GetOnPlayerRoughLandingEffectsListenerManager()));
	_listeners.attr("on_player_accelerate_listener_manager") = object(ptr(GetOnPlayerAccelerateListenerManager()));
	_listeners.attr("on_player_post_accelerate_listener_manager") = object(ptr(GetOnPlayerPostAccelerateListenerManager()));
	_listeners.attr("on_player_air_accelerate_listener_manager") = object(ptr(GetOnPlayerAirAccelerateListenerManager()));
	_listeners.attr("on_player_post_air_accelerate_listener_manager") = object(ptr(GetOnPlayerPostAirAccelerateListenerManager()));
	_listeners.attr("on_player_move_listener_manager") = object(ptr(GetOnPlayerMoveListenerManager()));
	_listeners.attr("on_player_finish_move_listener_manager") = object(ptr(GetOnPlayerFinishMoveListenerManager()));
	_listeners.attr("on_player_water_move_listener_manager") = object(ptr(GetOnPlayerWaterMoveListenerManager()));
	_listeners.attr("on_player_finish_water_move_listener_manager") = object(ptr(GetOnPlayerFinishWaterMoveListenerManager()));
	_listeners.attr("on_player_air_move_listener_manager") = object(ptr(GetOnPlayerAirMoveListenerManager()));
	_listeners.attr("on_player_finish_air_move_listener_manager") = object(ptr(GetOnPlayerFinishAirMoveListenerManager()));
	_listeners.attr("on_player_fall_move_listener_manager") = object(ptr(GetOnPlayerFallMoveListenerManager()));
	_listeners.attr("on_player_finish_fall_move_listener_manager") = object(ptr(GetOnPlayerFinishFallMoveListenerManager()));
	_listeners.attr("on_player_walk_move_listener_manager") = object(ptr(GetOnPlayerWalkMoveListenerManager()));
	_listeners.attr("on_player_finish_walk_move_listener_manager") = object(ptr(GetOnPlayerFinishWalkMoveListenerManager()));
	_listeners.attr("on_player_full_walk_move_listener_manager") = object(ptr(GetOnPlayerFullWalkMoveListenerManager()));
	_listeners.attr("on_player_finish_full_walk_move_listener_manager") = object(ptr(GetOnPlayerFinishFullWalkMoveListenerManager()));
	_listeners.attr("on_player_toss_move_listener_manager") = object(ptr(GetOnPlayerTossMoveListenerManager()));
	_listeners.attr("on_player_finish_toss_move_listener_manager") = object(ptr(GetOnPlayerFinishTossMoveListenerManager()));
	_listeners.attr("on_player_ladder_move_listener_manager") = object(ptr(GetOnPlayerLadderMoveListenerManager()));
	_listeners.attr("on_player_finish_ladder_move_listener_manager") = object(ptr(GetOnPlayerFinishLadderMoveListenerManager()));
	_listeners.attr("on_player_full_ladder_move_listener_manager") = object(ptr(GetOnPlayerFullLadderMoveListenerManager()));
	_listeners.attr("on_player_finish_full_ladder_move_listener_manager") = object(ptr(GetOnPlayerFinishFullLadderMoveListenerManager()));
	_listeners.attr("on_player_step_move_listener_manager") = object(ptr(GetOnPlayerStepMoveListenerManager()));
	_listeners.attr("on_player_finish_step_move_listener_manager") = object(ptr(GetOnPlayerFinishStepMoveListenerManager()));
}

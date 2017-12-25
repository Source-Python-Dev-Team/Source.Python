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
// Includes.
//---------------------------------------------------------------------------------
// Required to fix compilation errors after including cdll_int.h (CSGO only)
#if defined( _WIN32 )
	#include <Windows.h>
#endif

#include "engines_server.h"
#include "export_main.h"
#include "utilities/conversions.h"

#include "dt_send.h"
#include "irecipientfilter.h"
#include "server_class.h"
#include "iscratchpad3d.h"
#include "cdll_int.h"
#include "con_nprint.h"
#include "iachievementmgr.h"
#include "client_textmessage.h"
#include "steam/steamclientpublic.h"
#include "inetchannelinfo.h"
#include "eiface.h"
#include "engine/iserverplugin.h"
#include "engines.h"
#include "iserver.h"
#include "iclient.h"
#include "inetmessage.h"

#include ENGINE_INCLUDE_PATH(engines_wrap.h)


//---------------------------------------------------------------------------------
// External variables.
//---------------------------------------------------------------------------------
extern IVEngineServer* engine;
extern IServerGameDLL* servergamedll;


//---------------------------------------------------------------------------------
// Forward declarations.
//---------------------------------------------------------------------------------
static void export_engine_server(scope);
static void export_query_cvar_status(scope);
static void export_server_game_dll(scope);
static void export_iserver(scope);
static void export_functions(scope);


//---------------------------------------------------------------------------------
// Declare the _server module.
//---------------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_engines, _server)
{
	export_engine_server(_server);
	export_query_cvar_status(_server);
	export_server_game_dll(_server);
	export_iserver(_server);
	export_functions(_server);
}


//---------------------------------------------------------------------------------
// Exports IVEngineServer.
//---------------------------------------------------------------------------------
static void export_engine_server(scope _server)
{
	// Call engine specific implementation function
	IVEngineServer_Visitor(

	class_<IVEngineServer, boost::noncopyable>("_EngineServer", no_init)
		.def("change_level",
			&IVEngineServer::ChangeLevel,
			"Tells the engine to change the level. If s2 is None, the engine will execute a \
			changelevel command. If s2 is a valid map, the engine will execute a changelevel2 \
			command",
			args("s1", "s2")
		)

		.def("is_map_valid",
			&IVEngineServerExt::IsMapValid,
			"Returns true if filename refers to a valid map.",
			args("filename")
		)

		.def("is_dedicated_server",
			&IVEngineServer::IsDedicatedServer,
			"Returns true if the engine is running in dedicated mode."
		)

		.def("is_in_edit_mode",
			&IVEngineServer::IsInEditMode,
			"Returns false if the engine is not in hammer editing mode."
		)

		.def("precache_model",
			&IVEngineServerExt::precache_model,
			"Precaches a model and returns an integer containing its index.",
			("s", arg("preload")=false)
		)

		.def("precache_decal",
			&IVEngineServerExt::precache_decal,
			"Precaches a decal file and returns an integer containing its index.",
			("s", arg("preload")=false)
		)

		.def("precache_generic",
			&IVEngineServerExt::precache_generic,
			"Precaches a generic asset file and returns an integer containing its index.",
			("s", arg("preload")=false)
		)

		.def("is_model_precached",
			&IVEngineServer::IsModelPrecached,
			"Returns true if the given model is precached.",
			args("s")
		)

		.def("is_decal_precached",
			&IVEngineServer::IsDecalPrecached,
			"Returns true if the given decal is precached.",
			args("s")
		)

		.def("is_generic_precached",
			&IVEngineServer::IsGenericPrecached,
			"Returns true if the given generic asset is precached.",
			args("s")
		)

		.def("get_cluster_for_origin",
			&IVEngineServer::GetClusterForOrigin,
			"Returns the cluster number for the specified position.",
			args("origin")
		)

		.def("get_pvs_for_cluster",
			&IVEngineServer::GetPVSForCluster,
			"Gets the PVS bits for a specified cluster and copies the bits into outputpvs.",
			args("cluster", "outputpvslength", "outputpvs")
		)

		.def("check_origin_in_pvs",
			&IVEngineServer::CheckOriginInPVS,
			"Check whether the specified origin is inside the PVS",
			args("org", "checkpvs", "checkpvssize")
		)

		.def("check_box_in_pvs",
			&IVEngineServer::CheckBoxInPVS,
			"Checks whether the specified worldspace bounding box is inside the specified PVS",
			args("mins", "maxs", "checkpvs", "checkpvssize")
		)

		.add_property("entity_count",
			&IVEngineServer::GetEntityCount,
			"Returns the number of used edict slots."
		)

		.def("get_player_net_info",
			&IVEngineServer::GetPlayerNetInfo,
			"Returns stats info interface for a client netchannel.",
			args("player_index"),
			reference_existing_object_policy()
		)

		.def("create_edict",
			&IVEngineServer::CreateEdict,
			"Creates an edict. If <force_edict_index> is not -1, then it return the edict with that index.",
			args("force_edict_index"),
			reference_existing_object_policy()
		)

		.def("remove_edict",
			&IVEngineServer::RemoveEdict,
			"Remove the specified edict and place back into the free edict list.",
			args("edict_instance")
		)

		.def("emit_ambient_sound",
			&IVEngineServer::EmitAmbientSound,
			"Emits an ambient sound associated with the specified entity",
			args("entindex", "pos", "samp", "vol", "soundlevel", "flags", "pitch", "delay")
		)

		.def("fade_client_volume",
			&IVEngineServer::FadeClientVolume,
			"Fade out the client's volume level toward silence (or fadePercent)",
			args("edict", "fade_percent", "fade_out_seconds", "hold_time", "fade_in_seconds")
		)

		.def("sentence_group_pick",
			&IVEngineServer::SentenceGroupPick,
			args("group_index", "name", "name_buf_len")
		)

		.def("sentence_group_pick_sequential",
			&IVEngineServer::SentenceGroupPickSequential,
			args("group_index", "name", "name_buf_len", "sentence_index", "reset")
		)

		.def("sentence_index_from_name",
			&IVEngineServer::SentenceIndexFromName,
			args("sentence_name")
		)

		.def("sentence_name_from_index",
			&IVEngineServer::SentenceNameFromIndex,
			args("sentence_index")
		)

		.def("sentence_group_index_from_name",
			&IVEngineServer::SentenceGroupIndexFromName,
			args("group_name")
		)

		.def("sentence_group_name_from_index",
			&IVEngineServer::SentenceGroupNameFromIndex,
			args("group_index")
		)

		.def("sentence_length",
			&IVEngineServer::SentenceLength,
			args("sentence_index")
		)

		.def("client_command",
			&IVEngineServerExt::ClientCommand,
			"Runs a command on the client.",
			("edict", "command", arg("server_side")=false)
		)

		.def("light_style",
			&IVEngineServer::LightStyle,
			"Set the lightstyle to the specified value and network the change to any connected clients.",
			args("style", "val")
		)

		.def("static_decal",
			&IVEngineServer::StaticDecal,
			"Project a static decal onto the specified entity / model (for level placed decals in the .bsp)",
			args("origin_in_entity_space", "decal_index", "entity_index", "model_index", "low_priority")
		)

		.def("message_determine_multicast_recipients",
			&IVEngineServer::Message_DetermineMulticastRecipients,
			"Given the current PVS (or PAS), determine which players should hear/recieve the message.",
			args("use_pas", "origin", "player_bits")
		)

		.def("client_printf",
			&IVEngineServer::ClientPrintf,
			"Prints szMsg to the client's console.",
			args("edict", "message")
		)

		.def("con_nprintf",
			&IVEngineServerExt::Con_NPrintf,
			"LISTEN SERVER ONLY: Prints a string to the notification area of the screen.",
			args("pos", "string")
		)

		.def("con_nxprintf",
			&IVEngineServerExt::Con_NXPrintf,
			"LISTEN SERVER ONLY: Same as con_nprintf but allows you to specify additional information.",
			args("info", "string")
		)

		.def("set_view",
			&IVEngineServer::SetView,
			"Change a specified player's \"view entity\"",
			args("client", "view_entity")
		)

		.def("crosshair_angle",
			&IVEngineServer::CrosshairAngle,
			"Sets the player's crosshair angle",
			args("client", "pitch", "yaw")
		)

		.def("lock_network_string_tables",
			&IVEngineServer::LockNetworkStringTables,
			"Locks/unlocks the network string tables.",
			args("lock")
		)

		.def("create_fake_client",
			&IVEngineServer::CreateFakeClient,
			"Creates a bot with the given name. Returns NULL if this call fails.",
			args("netname"),
			reference_existing_object_policy()
		)

		.def("get_client_convar_value",
			&IVEngineServer::GetClientConVarValue,
			"Get a convar keyvalue for specified client.",
			args("client_index", "name")
		)

		.def("reset_pvs",
			&IVEngineServer::ResetPVS,
			"Resets the potentially visible set. pvssize is the size in bytes of the buffer pointed to by pvs.",
			args("pvs", "pvssize")
		)

		.def("add_origin_to_pvs",
			&IVEngineServer::AddOriginToPVS,
			"Merge the pvs bits into the current accumulated pvs based on the specified origin.",
			args("origin")
		)

		.def("set_area_portal_state",
			&IVEngineServer::SetAreaPortalState,
			"Marks a specified area portal as open or closed.",
			args("portal_number", "is_open")
		)

		.def("playback_temp_entity",
			&IVEngineServer::PlaybackTempEntity,
			"Queue a temp entity for transmission",
			args("filter", "delay", "sender", "st", "class_id")
		)

		.def("check_head_node_visible",
			&IVEngineServer::CheckHeadnodeVisible,
			"Given a node number and the specified PVS, return with the node is in the PVS.",
			args("nodenum", "pvs", "vissize")
		)

		.def("check_areas_connected",
			&IVEngineServer::CheckAreasConnected,
			"Using area bits, check whether area 1 flows into area 2 and vice versa (depends on portal state)",
			args("area1", "area2")
		)

		.def("get_area",
			&IVEngineServer::GetArea,
			"Given an origin, determine which area index the origin is within.",
			args("origin")
		)

		.def("get_area_bits",
			&IVEngineServer::GetAreaBits,
			"Get area portal bit set.",
			args("area", "bits", "buflen")
		)

		.def("get_area_portal_plane",
			&IVEngineServer::GetAreaPortalPlane,
			"Given a view origin and a portal key, fill in the plane that leads out of this area.",
			args("view_origin", "portal_key", "plane")
		)

		.def("load_game_state",
			&IVEngineServer::LoadGameState,
			"Save/Restore wrapper - FIXME: At some point we should move this to its own interface.",
			args("mapname", "create_players")
		)

		.def("load_adjacent_ents",
			&IVEngineServer::LoadAdjacentEnts,
			args("old_level", "landmark_name")
		)

		.add_property("map_entities_string",
			&IVEngineServer::GetMapEntitiesString,
			"Get the pristine map entity lump string."
		)

		.def("text_message_get",
			&IVEngineServer::TextMessageGet,
			"Text message system -- lookup the text message of the specified name",
			args("name"),
			reference_existing_object_policy()
		)

		.def("log_print",
			&IVEngineServer::LogPrint,
			"Print a message to the server log file.",
			args("msg")
		)

		.def("build_entity_cluster_list",
			&IVEngineServer::BuildEntityClusterList,
			"Builds PVS information for an entity.",
			args("edict", "pvs_info")
		)

		.def("solid_moved",
			&IVEngineServer::SolidMoved,
			"A solid entity moved, update spatial partition.",
			args("solid_ent", "solid_collide", "prev_abs_origin", "test_surrounding_bounds_only")
		)

		.def("trigger_moved",
			&IVEngineServer::TriggerMoved,
			"A trigger entity moved, update the spatial partition",
			args("trigger_ent", "test_surrounding_bounds_only")
		)

		.def("create_spatial_partition",
			&IVEngineServer::CreateSpatialPartition,
			"Create a custom spatial partition",
			args("worldmin", "worldmax"),
			reference_existing_object_policy()
		)

		.def("destroy_spatial_partition",
			&IVEngineServer::DestroySpatialPartition,
			"Destroy a custom spatial partition",
			args("partition")
		)

		.def("draw_map_to_scratch_pad",
			&IVEngineServer::DrawMapToScratchPad,
			"Draw the brush geometry in the map into the scratch pad.",
			args("pad", "flags")
		)

		.def("get_entity_transmit_bits_for_client",
			&IVEngineServer::GetEntityTransmitBitsForClient,
			"This returns which entities, to the server's knowledge the client currently knows about.",
			args("client_index"),
			reference_existing_object_policy()
		)

		.def("is_paused",
			&IVEngineServer::IsPaused,
			"Returns true if the game is paused"
		)

		.def("force_exact_file",
			&IVEngineServer::ForceExactFile,
			"Marks the filename for consistency checking. this should be called after precaching the file.",
			args("file")
		)

		.def("force_model_bounds",
			&IVEngineServer::ForceModelBounds,
			"Marks a precached model as having a maximum bounding size on the client.",
			args("s", "mins", "maxs")
		)

		.def("clear_save_dir_after_client_load",
			&IVEngineServer::ClearSaveDirAfterClientLoad
		)

		.def("set_fake_client_convar_value",
			&IVEngineServer::SetFakeClientConVarValue,
			"Sets a USERINFO client ConVar for a fakeclient.",
			args("entity", "cvar", "value")
		)

		.def("force_simple_material",
			&IVEngineServer::ForceSimpleMaterial,
			"Marks the material (vmt file) for consistency checking.",
			args("s")
		)

		.def("set_area_portal_states",
			&IVEngineServer::SetAreaPortalStates,
			"Mark some area portals as open/closed.",
			args("portal_numbers", "is_open", "portals")
		)

		.def("notify_edict_flags_change",
			&IVEngineServer::NotifyEdictFlagsChange,
			"Called when relevant edict state flags change.",
			args("edict")
		)

		.def("get_prev_check_transmit_info",
			&IVEngineServer::GetPrevCheckTransmitInfo,
			args("pPlayerEdict"),
			reference_existing_object_policy()
		)

		.add_property("shared_edict_change_info",
			make_function(
				&IVEngineServer::GetSharedEdictChangeInfo,
				reference_existing_object_policy()
			)
		)

		.def("allow_immediate_edict_reuse",
			&IVEngineServer::AllowImmediateEdictReuse,
			"Tells the engine we can immediately re-use all the edict indices."
		)

		.def("is_internal_build",
			&IVEngineServer::IsInternalBuild,
			"Returns true if the engine is an internal build."
		)

		.def("get_change_accessor",
			&IVEngineServer::GetChangeAccessor,
			args("edict"),
			reference_existing_object_policy()
		)

		.def("clean_up_entity_cluster_list",
			&IVEngineServer::CleanUpEntityClusterList,
			"Cleans up the cluster list.",
			args("pvs_info")
		)

		.add_property("app_id",
			&IVEngineServer::GetAppID,
			"Returns the game's appid."
		)

		.def("is_low_violence",
			&IVEngineServer::IsLowViolence,
			"Returns true if the game is in low-violence mode."
		)

		.def("start_query_cvar_value",
			&IVEngineServerExt::StartQueryCvarValue,
			"Starts a query to retrieve the value of a client's console variable. Result of the query will be passed to the OnQueryCvarValueFinished listeners.",
			args("edict", "cvar_name")
		)

		.def("get_player_info",
			&IVEngineServer::GetPlayerInfo,
			"Fill in the player info structure for the specified player.",
			args("ent_num", "pinfo")
		)

		.def("is_client_fully_authenticated",
			&IVEngineServer::IsClientFullyAuthenticated,
			"Returns true if this client has been fully authenticated by Steam.",
			args("edict")
		)

		.def("set_dedicated_server_benchmark_mode",
			&IVEngineServer::SetDedicatedServerBenchmarkMode,
			"Makes the host run 1 tick per frame.",
			args("benchmark_mode")
		)

		.add_property("cluster_count",
			&IVEngineServer::GetClusterCount,
			"Returns total number of clusters."
		)

		/*
		.def("is_split_screen_player",
			&IVEngineServer::SetDedicatedServerBenchmarkMode,
			args("ent_num")
		)
		*/

		/*
		TODO: Create a wrapper that requires a list or tuple with bbox_t elements
		.def("get_all_cluster_bounds",
			&IVEngineServer::GetAllClusterBounds,
			"Gets a list of all clusters and bounds.",
			args("bbox_list", "max_bbox")
		)

		TODO: Patch SDK, so we can access CGameStatsData
		.def("set_gamestats_data",
			&IVEngineServer::SetGamestatsData,
			"Sets the gamestats data container.",
			args("gamestats_data")
		)
		
		.def("get_gamestats_data",
			&IVEngineServer::GetGamestatsData,
			"Returns the gamestats data container.",
			reference_existing_object_policy()
		)
		*/

		.def("get_client_steamid",
			&IVEngineServer::GetClientSteamID,
			"Returns the SteamID of the specified player. Returns NULL if the player isn't authenticated.",
			args("client"),
			reference_existing_object_policy()
		)

		.def("client_command_key_values",
			&IVEngineServer::ClientCommandKeyValues,
			"Sends a client command keyvalues which are deleted inside this function.",
			args("edict", "key_values")
		)

		// Not in Blade
		.NOT_IMPLEMENTED_ATTR("achievement_mgr")

		// Not on L4D2
		.NOT_IMPLEMENTED_ATTR("server_version")
		.NOT_IMPLEMENTED_ATTR("game_server_steamid")

		// OB specific methods
		.NOT_IMPLEMENTED("multiplayer_end_game")
		.NOT_IMPLEMENTED("create_fake_client_ex")
		.NOT_IMPLEMENTED("replay")

		// BM:S specific
		.NOT_IMPLEMENTED_ATTR("server")

		// CS:GO specific methods
		.NOT_IMPLEMENTED_ATTR("launch_options")
		.NOT_IMPLEMENTED("is_userid_in_use")
		.NOT_IMPLEMENTED("get_loading_process_for_userid")
		.NOT_IMPLEMENTED("is_log_enabled")
		.NOT_IMPLEMENTED_ATTR("timescale")
		.NOT_IMPLEMENTED("is_level_main_menu_background")
		.NOT_IMPLEMENTED("is_any_client_low_violence")
		.NOT_IMPLEMENTED("is_split_screen_player")
		.NOT_IMPLEMENTED("get_split_screen_player_for_edict")
		.NOT_IMPLEMENTED("is_override_load_game_ents_on")
		.NOT_IMPLEMENTED("force_flush_entity")
		.NOT_IMPLEMENTED("get_single_player_shared_memory_space")
		.NOT_IMPLEMENTED("alloc_level_static_data")
		.NOT_IMPLEMENTED("is_creating_reslist")
		.NOT_IMPLEMENTED("get_replayis_creating_xbox_reslist")
		.NOT_IMPLEMENTED("is_dedicated_server_for_xbox")
		.NOT_IMPLEMENTED("is_dedicated_server_for_ps3")
		.NOT_IMPLEMENTED("pause")
		.NOT_IMPLEMENTED("host_validated_session")
		.NOT_IMPLEMENTED("refresh_screen_if_necessary")
		.NOT_IMPLEMENTED("has_paint_map")
		.NOT_IMPLEMENTED("sphere_paint_surface")
		.NOT_IMPLEMENTED("sphere_trace_paint_surface")
		.NOT_IMPLEMENTED("remove_all_paint")
		.NOT_IMPLEMENTED("paint_all_surfaces")
		.NOT_IMPLEMENTED("get_client_xuid")
		.NOT_IMPLEMENTED("is_active_app")
		.NOT_IMPLEMENTED("set_noclip_enabled")
		.NOT_IMPLEMENTED("get_paint_map_data_rle")
		.NOT_IMPLEMENTED("load_paint_map_data_rle")
		.NOT_IMPLEMENTED("send_paint_map_data_to_client")
		.NOT_IMPLEMENTED("latency_for_choreo_sounds")
		.NOT_IMPLEMENTED("get_client_cross_play_platform")
		.NOT_IMPLEMENTED("ensure_instance_baseline")
		.NOT_IMPLEMENTED("reserve_server_for_queued_game")
		.NOT_IMPLEMENTED("get_engine_hltv_info")

	) ADD_MEM_TOOLS(IVEngineServer); // IVEngineServer_Visitor

	BEGIN_CLASS_INFO(IVEngineServer)
		FUNCTION_INFO(ChangeLevel)
		FUNCTION_INFO(IsMapValid)
		FUNCTION_INFO(IsDedicatedServer)
		FUNCTION_INFO(IsInEditMode)
		FUNCTION_INFO(PrecacheModel)
		FUNCTION_INFO(PrecacheDecal)
		FUNCTION_INFO(PrecacheGeneric)
		FUNCTION_INFO(IsModelPrecached)
		FUNCTION_INFO(IsDecalPrecached)
		FUNCTION_INFO(IsGenericPrecached)
		FUNCTION_INFO(GetClusterForOrigin)
		FUNCTION_INFO(GetPVSForCluster)
		FUNCTION_INFO(CheckOriginInPVS)
		FUNCTION_INFO(CheckBoxInPVS)
		FUNCTION_INFO(GetEntityCount)
		FUNCTION_INFO(GetPlayerNetInfo)
		FUNCTION_INFO(CreateEdict)
		FUNCTION_INFO(RemoveEdict)
		FUNCTION_INFO(EmitAmbientSound)
		FUNCTION_INFO(FadeClientVolume)
		FUNCTION_INFO(SentenceGroupPick)
		FUNCTION_INFO(SentenceGroupPickSequential)
		FUNCTION_INFO(SentenceIndexFromName)
		FUNCTION_INFO(SentenceNameFromIndex)
		FUNCTION_INFO(SentenceGroupIndexFromName)
		FUNCTION_INFO(SentenceGroupNameFromIndex)
		FUNCTION_INFO(SentenceLength)
		FUNCTION_INFO(ServerCommand)
		FUNCTION_INFO(ServerExecute)
		FUNCTION_INFO(ClientCommand)
		FUNCTION_INFO(LightStyle)
		FUNCTION_INFO(StaticDecal)
		FUNCTION_INFO(Message_DetermineMulticastRecipients)
		FUNCTION_INFO(ClientPrintf)
		FUNCTION_INFO(Con_NPrintf)
		FUNCTION_INFO(Con_NXPrintf)
		FUNCTION_INFO(SetView)
		FUNCTION_INFO(CrosshairAngle)
		FUNCTION_INFO(LockNetworkStringTables)
		FUNCTION_INFO(CreateFakeClient)
		FUNCTION_INFO(GetClientConVarValue)
		FUNCTION_INFO(ResetPVS)
		FUNCTION_INFO(AddOriginToPVS)
		FUNCTION_INFO(SetAreaPortalState)
		FUNCTION_INFO(PlaybackTempEntity)
		FUNCTION_INFO(CheckHeadnodeVisible)
		FUNCTION_INFO(CheckAreasConnected)
		FUNCTION_INFO(GetArea)
		FUNCTION_INFO(GetAreaBits)
		FUNCTION_INFO(GetAreaPortalPlane)
		FUNCTION_INFO(LoadGameState)
		FUNCTION_INFO(LoadAdjacentEnts)
		FUNCTION_INFO(GetMapEntitiesString)
		FUNCTION_INFO(TextMessageGet)
		FUNCTION_INFO(LogPrint)
		FUNCTION_INFO(BuildEntityClusterList)
		FUNCTION_INFO(SolidMoved)
		FUNCTION_INFO(TriggerMoved)
		FUNCTION_INFO(CreateSpatialPartition)
		FUNCTION_INFO(DestroySpatialPartition)
		FUNCTION_INFO(DrawMapToScratchPad)
		FUNCTION_INFO(GetEntityTransmitBitsForClient)
		FUNCTION_INFO(IsPaused)
		FUNCTION_INFO(ForceExactFile)
		FUNCTION_INFO(ForceModelBounds)
		FUNCTION_INFO(ClearSaveDirAfterClientLoad)
		FUNCTION_INFO(SetFakeClientConVarValue)
		FUNCTION_INFO(ForceSimpleMaterial)
		FUNCTION_INFO(SetAreaPortalStates)
		FUNCTION_INFO(NotifyEdictFlagsChange)
		FUNCTION_INFO(GetPrevCheckTransmitInfo)
		FUNCTION_INFO(GetSharedEdictChangeInfo)
		FUNCTION_INFO(AllowImmediateEdictReuse)
		FUNCTION_INFO(IsInternalBuild)
		FUNCTION_INFO(GetChangeAccessor)
		FUNCTION_INFO(CleanUpEntityClusterList)
		FUNCTION_INFO(GetAppID)
		FUNCTION_INFO(IsLowViolence)
		FUNCTION_INFO(StartQueryCvarValue)
		FUNCTION_INFO(InsertServerCommand)
		FUNCTION_INFO(GetPlayerInfo)
		FUNCTION_INFO(IsClientFullyAuthenticated)
		FUNCTION_INFO(SetDedicatedServerBenchmarkMode)
		FUNCTION_INFO(GetClusterCount)
		FUNCTION_INFO(GetClientSteamID)
		FUNCTION_INFO(ClientCommandKeyValues)
	END_CLASS_INFO()

	_server.attr("engine_server") = object(ptr(engine));
}


//---------------------------------------------------------------------------------
// Exports EQueryCvarValueStatus.
//---------------------------------------------------------------------------------
static void export_query_cvar_status(scope _server)
{
	enum_<EQueryCvarValueStatus> QueryCvarStatus("QueryCvarStatus");

	// Values...
	QueryCvarStatus.value("SUCCESS", eQueryCvarValueStatus_ValueIntact);
	QueryCvarStatus.value("NOT_FOUND", eQueryCvarValueStatus_CvarNotFound);
	QueryCvarStatus.value("INVALID", eQueryCvarValueStatus_NotACvar);
	QueryCvarStatus.value("PROTECTED", eQueryCvarValueStatus_CvarProtected);
}


//-----------------------------------------------------------------------------
// Exports IServerGameDLL.
//-----------------------------------------------------------------------------
static void export_server_game_dll(scope _server)
{
	class_<IServerGameDLL, boost::noncopyable> ServerGameDLL("_ServerGameDLL", no_init);
	
	// Methods...
	ServerGameDLL.add_property(
		"all_server_classes",
		make_function(
			&IServerGameDLL::GetAllServerClasses,
			reference_existing_object_policy()
		),
		"Return the first server class. This base can be used to iterate through all server classes.\n\n"
		":rtype: ServerClass"
	);

	ServerGameDLL.add_property(
		"game_description",
		&IServerGameDLL::GetGameDescription,
		"Return the game's description.\n\n"
		":rtype: str"
	);

	// Black Mesa and Orangebox specific methods...
	ServerGameDLL.NOT_IMPLEMENTED("set_server_hibernation");

	// Blade, CS:GO and Left 4 Dead 2 specific methods...
	ServerGameDLL.NOT_IMPLEMENTED("server_hibernation_update");

	// Class info...
	BEGIN_CLASS_INFO(IServerGameDLL)
		FUNCTION_INFO(GetGameDescription)
	END_CLASS_INFO()

	// Engine specific stuff...
	export_engine_specific_server_game_dll(_server, ServerGameDLL);

	// Add memory tools...
	ServerGameDLL ADD_MEM_TOOLS(IServerGameDLL);
	
	// Singleton...
	_server.attr("server_game_dll") = object(ptr(servergamedll));
}


//-----------------------------------------------------------------------------
// Exports IServer.
//-----------------------------------------------------------------------------
static void export_iserver(scope _server)
{
	class_<IConnectionlessPacketHandler, IConnectionlessPacketHandler*, boost::noncopyable> _IConnectionlessPacketHandler("ConnectionlessPacketHandler", no_init);

	class_< IServer, IServer*, bases<IConnectionlessPacketHandler>, boost::noncopyable > _IServer("Server", no_init);

	_IServer.add_property(
		"num_clients",
		&IServer::GetNumClients,
		"Return the current number of clients.\n\n"
		":rtype: int"
	);

	_IServer.add_property(
		"num_proxies",
		&IServer::GetNumProxies,
		"Return the number of attached HLTV proxies.\n\n"
		":rtype: int"
	);

	_IServer.add_property(
		"num_fake_clients",
		&IServer::GetNumFakeClients,
		"Return the number of fake clients.\n\n"
		":rtype: int"
	);

	_IServer.add_property(
		"max_clients",
		&IServer::GetMaxClients,
		"Return the current client limit.\n\n"
		":rtype: int"
	);

	_IServer.def(
		"get_client",
		&IServer::GetClient,
		"Return the interface to a client.\n\n"
		":rtype: Client",
		reference_existing_object_policy()
	);

	_IServer.add_property(
		"client_count",
		&IServer::GetClientCount,
		"Return the number of client slots (used and unused).\n\n"
		":rtype: int"
	);

	_IServer.add_property(
		"udp_port",
		&IServer::GetUDPPort,
		"Return the currently used UDP port.\n\n"
		":rtype: int"
	);

	_IServer.add_property(
		"time",
		&IServer::GetTime,
		"Return the game world time.\n\n"
		":rtype: float"
	);

	_IServer.add_property(
		"tick",
		&IServer::GetTick,
		"Return the game world tick.\n\n"
		":rtype: int"
	);

	_IServer.add_property(
		"tick_interval",
		&IServer::GetTickInterval,
		"Return the tick interval in seconds.\n\n"
		":rtype: float"
	);

	_IServer.add_property(
		"name",
		&IServer::GetName,
		"Return the public server name.\n\n"
		":rtype: str"
	);

	_IServer.add_property(
		"map_name",
		&IServer::GetMapName,
		"Return the current map name.\n\n"
		":rtype: str"
	);

	_IServer.add_property(
		"spawn_count",
		&IServer::GetSpawnCount,
		"Return the spawn count.\n\n"
		":rtype: int"
	);

	_IServer.add_property(
		"num_classes",
		&IServer::GetNumClasses,
		":rtype: int"
	);

	_IServer.add_property(
		"class_bits",
		&IServer::GetClassBits,
		":rtype: int"
	);

	_IServer.add_property(
		"net_stats",
		&IServerExt::GetNetStats,
		"Return a tuple containing the total net in/out in bytes/sec.\n\n"
		":rtype: tuple"
	);

	_IServer.add_property(
		"num_players",
		&IServer::GetNumPlayers,
		":rtype: int"
	);

	_IServer.def(
		"get_player_info",
		&IServer::GetPlayerInfo,
		":rtype: bool"
	);

	_IServer.def(
		"is_active",
		&IServer::IsActive,
		":rtype: bool"
	);

	_IServer.def(
		"is_loading",
		&IServer::IsLoading,
		":rtype: bool"
	);

	_IServer.def(
		"is_dedicated",
		&IServer::IsDedicated,
		":rtype: bool"
	);

	_IServer.def(
		"is_multiplayer",
		&IServer::IsMultiplayer,
		":rtype: bool"
	);

	_IServer.def(
		"is_pausable",
		&IServer::IsPausable,
		":rtype: bool"
	);

	_IServer.def(
		"is_hltv",
		&IServer::IsHLTV,
		":rtype: bool"
	);

	/*
	_IServer.def(
		"is_replay",
		&IServer::IsReplay
	);
	*/

	_IServer.add_property(
		"password",
		&IServer::GetPassword,
		&IServer::SetPassword,
		"Return the server password.\n\n"
		":rtype: str"
	);

	_IServer.add_property(
		"paused",
		&IServer::IsPaused,
		&IServer::SetPaused,
		":rtype: bool"
	);

	_IServer.def(
		"broadcast_message",
		GET_METHOD(void, IServer, BroadcastMessage, INetMessage&, bool, bool)
	);

	_IServer.def(
		"broadcast_message",
		GET_METHOD(void, IServer, BroadcastMessage, INetMessage&, IRecipientFilter&)
	);

	_IServer.def(
		"disconnect_client",
		&IServer::DisconnectClient
	);

	_IServer ADD_MEM_TOOLS(IServer)
}


//-----------------------------------------------------------------------------
// Exports functions.
//-----------------------------------------------------------------------------
static void export_functions(scope _server)
{
	def("execute_server_command",
		raw_function(execute_server_command, 1)
	);
	// Passing the doc string in combination with raw_function causes compiler errors :(
	_server.attr("execute_server_command").attr("__doc__") = "Execute a server command immediately.\n\n"
		"Additional arguments can be passed.\n\n"
		":param str command: Command to execute.\n"
		":param *args: Additional arguments that should be passed to the command.\n"
		":raise TypeError: Raised if keyword arguments have been passed.\n"
		":raise ValueError: Raised if the command has not been found or if the command couldn't be tokenized.";

	def("queue_server_command",
		raw_function(queue_server_command, 1)
	);
	_server.attr("queue_server_command").attr("__doc__") = "Same as :func:`execute_server_command`, but does not execute the server command immediately. "
		"Instead it is added to the server command queue. Thus, it can be executed with a little delay.";

	def("queue_command_string",
		&queue_command_string,
		"Adds a string to the server command queue. It's the same like typing something into the server console."
	);
}

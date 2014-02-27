/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
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
// Includes
//---------------------------------------------------------------------------------
// Required to fix compilation errors after including cdll_int.h
#if defined( _WIN32 )
#include <Windows.h>
#endif

#include "modules/export_main.h"
#include "modules/conversions/conversions_wrap.h"

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
#include "engine/IEngineSound.h"
#include "engine/IEngineTrace.h"

#include ENGINE_INCLUDE_PATH(engine_wrap_python.h)


extern IVEngineServer* engine;
extern IEngineSound* enginesound;
extern IEngineTrace* enginetrace;
extern CGlobalVars* gpGlobals;

//---------------------------------------------------------------------------------
// Exposes the engine module.
//---------------------------------------------------------------------------------
void export_engine_server();
void export_engine_sound();
void export_engine_trace();

DECLARE_SP_MODULE(engine_c)
{
	export_engine_server();
	export_engine_sound();
	export_engine_trace();
}


//---------------------------------------------------------------------------------
// Exposes IVEngineServer.
//---------------------------------------------------------------------------------
class IVEngineServerExt
{
public:
	void ClientCommand(IVEngineServer* pEngine, edict_t* pEdict, const char* szCommand)
	{
		pEngine->ClientCommand(pEdict, szCommand);
	}
	
	void Con_NPrintf(IVEngineServer* pEngine, int pos, const char* fmt)
	{
		pEngine->Con_NPrintf(pos, fmt);
	}

	void Con_NXPrintf(IVEngineServer* pEngine, const struct con_nprint_s* info, const char* fmt)
	{
		pEngine->Con_NXPrintf(info, fmt);
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(precache_model_overload, PrecacheModel, 1, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(precache_sentence_file_overload, PrecacheSentenceFile, 1, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(precache_decal_overload, PrecacheDecal, 1, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(precache_generic_overload, PrecacheGeneric, 1, 2);

void export_engine_server()
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
			&IVEngineServer::IsMapValid,
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
			&IVEngineServer::PrecacheModel,
			precache_model_overload(
				"Precaches a model and returns an integer containing its index.",
				args("s", "preload")
			)
		)

		.def("precache_decal",
			&IVEngineServer::PrecacheDecal,
			precache_decal_overload(
				"Precaches a decal file and returns an integer containing its index.",
				args("s", "preload")
			)
		)

		.def("precache_generic",
			&IVEngineServer::PrecacheGeneric,
			precache_generic_overload(
				"Precaches a generic asset file and returns an integer containing its index.",
				args("s", "preload")
			)
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

		.def("get_player_userid",
			&IVEngineServer::GetPlayerUserId,
			"Returns the server assigned userid for this player. Returns -1 if the edict could not \
			be found.",
			args("player_instance")
		)

		.def("get_player_network_id_string",
			&IVEngineServer::GetPlayerNetworkIDString,
			"Returns the player's network id as a string.",
			args("player_instance")
		)

		.def("get_entity_count",
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

		.def("server_command",
			&IVEngineServer::ServerCommand,
			"Issues a command to the command parser as if it was typed at the server console.",
			args("command")
		)

		.def("server_execute",
			&IVEngineServer::ServerExecute,
			"Executes any commands currently in the command parser immediately (instead of once per frame)."
		)

		.def("client_command",
			&IVEngineServerExt::ClientCommand,
			"Runs a command on the client.",
			args("edict", "command")
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

		.def("entity_message_begin",
			&IVEngineServer::EntityMessageBegin,
			"Begin a message from a server side entity to its client side counterpart (func_breakable glass, e.g.)",
			args("ent_index", "ent_class", "reliable"),
			reference_existing_object_policy()
		)

		.def("message_end",
			&IVEngineServer::MessageEnd,
			"Finish the Entity or UserMessage and dispatch to network layer"
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

		.def("get_game_dir",
			&IVEngineServer::GetGameDir,
			"Returns the path to the MOD's game directory.",
			args("max_length")
		)

		.def("compare_file_time",
			&IVEngineServer::CompareFileTime,
			args("filename1", "filename2", "icompare")
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

		.def("parse_file",
			&IVEngineServer::ParseFile,
			"Parse a token from a file",
			args("data", "token", "maxlen")
		)

		.def("copy_file",
			&IVEngineServer::CopyFile,
			"Copies a file. Returns true if the operation succeeded.",
			args("source", "destination")
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

		.def("get_area",
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

		.def("get_map_entities_string",
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

		.def("get_shared_edict_change_info",
			&IVEngineServer::GetSharedEdictChangeInfo,
			reference_existing_object_policy()
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

		.def("set_achievement_mgr",
			&IVEngineServer::SetAchievementMgr,
			"Sets the achievement manager."
		)

		.def("get_achievement_mgr",
			&IVEngineServer::GetAchievementMgr,
			"Returns the current achievement manager.",
			reference_existing_object_policy()
		)

		.def("get_app_id",
			&IVEngineServer::GetAppID,
			"Returns the game's appid."
		)

		.def("is_low_violence",
			&IVEngineServer::IsLowViolence,
			"Returns true if the game is in low-violence mode."
		)

		.def("start_query_cvar_value",
			&IVEngineServer::StartQueryCvarValue,
			"Returns the value of a cvar on the client.",
			args("edict", "cvar_name")
		)

		.def("insert_server_command",
			&IVEngineServer::InsertServerCommand,
			"Inserts a command into the server's command buffer.",
			args("command_string")
		)

		.def("get_player_info",
			&IVEngineServer::GetPlayerInfo,
			"Fill in the player info structure for the specified player.",
			args("ent_num", "pinfo")
		)

		.def("is_client_fully_authenticated",
			&IVEngineServer::IsClientFullyAuthenticated,
			"Returns true if this client has been fully authenticated by Steam.",
			args("pEdict")
		)

		.def("set_dedicated_server_benchmark_mode",
			&IVEngineServer::SetDedicatedServerBenchmarkMode,
			"Makes the host run 1 tick per frame.",
			args("benchmark_mode")
		)

		.def("is_split_screen_player",
			&IVEngineServer::SetDedicatedServerBenchmarkMode,
			args("ent_num")
		)

		.def("get_cluster_count",
			&IVEngineServer::GetClusterCount,
			"Returns total number of clusters."
		)

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

		.def("get_game_server_steamid",
			&IVEngineServer::GetGameServerSteamID,
			"Returns the SteamID of the game server.",
			reference_existing_object_policy()
		)

		.def("get_game_server_steamid",
			&IVEngineServer::GetGameServerSteamID,
			"Returns the SteamID of the game server.",
			reference_existing_object_policy()
		)

		.def("client_command_key_values",
			&IVEngineServer::ClientCommandKeyValues,
			"Sends a client command keyvalues which are deleted inside this function.",
			args("edict", "key_values")
		)

		// OB specific methods
		.NOT_IMPLEMENTED("user_message_begin")
		.NOT_IMPLEMENTED("get_time")
		.NOT_IMPLEMENTED("multiplayer_end_game")
		.NOT_IMPLEMENTED("change_team")
		.NOT_IMPLEMENTED("create_fake_client_ex")
		.NOT_IMPLEMENTED("get_server_version")
		.NOT_IMPLEMENTED("get_replay")

		// CS:GO specific methods
		.NOT_IMPLEMENTED("get_launch_options")
		.NOT_IMPLEMENTED("is_userid_in_use")
		.NOT_IMPLEMENTED("get_loading_process_for_userid")
		.NOT_IMPLEMENTED("send_user_message")
		.NOT_IMPLEMENTED("is_log_enabled")
		.NOT_IMPLEMENTED("get_timescale")
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
		.NOT_IMPLEMENTED("set_timescale")
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
		.NOT_IMPLEMENTED("get_latency_for_choreo_sounds")
		.NOT_IMPLEMENTED("get_client_cross_play_platform")
		.NOT_IMPLEMENTED("ensure_instance_baseline")
		.NOT_IMPLEMENTED("reserve_server_for_queued_game")
		.NOT_IMPLEMENTED("get_engine_hltv_info")

	); // IVEngineServer_Visitor

	scope().attr("EngineServer") = object(ptr(engine));
}


//---------------------------------------------------------------------------------
// Exposes IEngineSound.
//---------------------------------------------------------------------------------
// Overloads
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(precache_sound_overload, PrecacheSound, 1, 3);
BOOST_PYTHON_FUNCTION_OVERLOADS(emit_sound_overload, IEngineSound_EmitSound, 7, 15);

void export_engine_sound()
{
	// Call engine specific implementation function
	IEngineSound_Visitor(

	class_<IEngineSound, boost::noncopyable>("_EngineSound", no_init)

		.def("precache_sound",
			&IEngineSound::PrecacheSound,
			precache_sound_overload(
				args("sample", "preload", "is_ui_sound"),
				"Precaches a particular sample."
			)
		)

		.def("is_sound_precached",
			&IEngineSound::IsSoundPrecached,
			args("sample"),
			"Returns True if the given sound is precached."
		)

		.def("prefetch_sound",
			&IEngineSound::PrefetchSound,
			args("sample"),
			"Prefetches a sample."
		)

		.def("get_sound_duration",
			&IEngineSound::GetSoundDuration,
			args("sample"),
			"Returns the sound duration."
		)

		.def("emit_sound",
			&IEngineSound_EmitSound,
			emit_sound_overload(
				args("filter", "entity_index", "channel", "sample", "volume", "attenuation", "flags", "pitch", "origin", "direction", "origins", "update_positions", "sound_time", "speaker_entity"),
				"Emits a sound from an entity."
			)
		)

		// TODO: Can we use IVEngineServer::SentenceNameFromIndex() and then call IEngineSound::EmitSound()?
		.NOT_IMPLEMENTED("emit_sentence_by_index")

		.def("stop_sound",
			&IEngineSound::StopSound,
			args("entity_index", "channel", "sample"),
			"Stops a sound from being emitted from an entity."
		)

		// TODO: StopAllSounds, SetRoomType, SetPlayerDSP. Are they really just client only?

		.def("get_dist_gain_from_sound_level",
			&IEngineSound::GetDistGainFromSoundLevel,
			args("sound_level", "distance"),
			"Returns the distance gain value from a sound level"
		)

		// Only available for CS:GO
		.NOT_IMPLEMENTED("is_looping_sound")

	); // IEngineSound_Visitor

	scope().attr("EngineSound") = object(ptr(enginesound));

	// Channels
	enum_<int>("Channels")
		.value("REPLACE", CHAN_REPLACE)
		.value("AUTO", CHAN_AUTO)
		.value("WEAPON", CHAN_WEAPON)
		.value("VOICE", CHAN_VOICE)
		.value("ITEM", CHAN_ITEM)
		.value("BODY", CHAN_BODY)
		.value("STREAM", CHAN_STREAM)
		.value("STATIC", CHAN_STATIC)
		.value("VOICE_BASE", CHAN_VOICE_BASE)
		.value("USER_BASE", CHAN_USER_BASE)
	;

	// Common volume values
	scope().attr("VOL_NORM") = VOL_NORM;

	// Common attenuation values
	scope().attr("ATTN_NONE") = ATTN_NONE;
	scope().attr("ATTN_NORM") = ATTN_NORM;
	scope().attr("ATTN_IDLE") = ATTN_IDLE;
	scope().attr("ATTN_STATIC") = ATTN_STATIC;
	scope().attr("ATTN_RICOCHET") = ATTN_RICOCHET;
	scope().attr("ATTN_GUNFIRE") = ATTN_GUNFIRE;
	scope().attr("MAX_ATTENUATION") = MAX_ATTENUATION;

	// Flags for iFlags fields
	enum_<SoundFlags_t>("SoundFlags")
		.value("NOFLAGS", SND_NOFLAGS)
		.value("CHANGE_VOL", SND_CHANGE_VOL)
		.value("CHANGE_PITCH", SND_CHANGE_PITCH)
		.value("STOP", SND_STOP)
		.value("SPAWNING", SND_SPAWNING)
		.value("DELAY", SND_DELAY)
		.value("STOP_LOOPING", SND_STOP_LOOPING)
		.value("SPEAKER", SND_SPEAKER)
		.value("SHOULDPAUSE", SND_SHOULDPAUSE)
		.value("IGNORE_PHONEMES", SND_IGNORE_PHONEMES)
		.value("IGNORE_NAME", SND_IGNORE_NAME)
	;

	// Common pitch values
	scope().attr("PITCH_NORM") = PITCH_NORM;
	scope().attr("PITCH_LOW") = PITCH_LOW;
	scope().attr("PITCH_HIGH") = PITCH_HIGH;

	scope().attr("SOUND_FROM_LOCAL_PLAYER") = SOUND_FROM_LOCAL_PLAYER;
	scope().attr("SOUND_FROM_WORLD") = SOUND_FROM_WORLD;
}

//---------------------------------------------------------------------------------
// Exposes IEngineTrace.
//---------------------------------------------------------------------------------
class ITraceFilterWrap: public ITraceFilter, public wrapper<ITraceFilter>
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntity, int mask)
	{ return get_override("should_hit_entity")(ptr(pEntity), mask); }

	virtual TraceType_t	GetTraceType() const
	{ return get_override("get_trace_type")(); }
};


class IEntityEnumeratorWrap: public IEntityEnumerator, public wrapper<IEntityEnumerator>
{
public:
	virtual bool EnumEntity(IHandleEntity* pEntity)
	{ return get_override("enum_entity")(ptr(pEntity)); }
};


class IEngineTraceExt
{
public:
	static tuple GetPointContents(IEngineTrace* pEngineTrace, const Vector& vec)
	{
		IHandleEntity** ppEntities = new IHandleEntity*[gpGlobals->maxEntities];
		memset(ppEntities, NULL, sizeof(IHandleEntity*) * gpGlobals->maxEntities);

		int iMask = ::GetPointContents(vec, ppEntities);

		list entities;
		for(int i=0; i < gpGlobals->maxEntities; i++)
		{
			if(ppEntities[i])
			{
				entities.append(ptr(ppEntities[i]));
			}
		}
		delete ppEntities;
		return make_tuple(iMask, entities);
	}
};


class CGameTraceExt
{
public:
	static IServerEntity* GetEntity(CGameTrace* pTrace)
	{
		return (IServerEntity *) pTrace->m_pEnt;
	}
};


class Ray_tExt
{
public:
	static Ray_t* CreateRay1(Vector vec1, Vector vec2)
	{
		Ray_t* pRay = new Ray_t;
		pRay->Init(vec1, vec2);
		return pRay;
	}

	static Ray_t* CreateRay2(Vector vec1, Vector vec2, Vector vec3, Vector vec4)
	{
		Ray_t* pRay = new Ray_t;
		pRay->Init(vec1, vec2, vec3, vec4);
		return pRay;
	}
};


CBaseEntity* GetWorldEntity()
{
	return EdictFromIndex(0)->GetNetworkable()->GetBaseEntity();
}


// These two functions are not implemented in the SDK
bool CGameTrace::DidHitWorld() const
{
	return GetWorldEntity() == m_pEnt;
}

int CGameTrace::GetEntityIndex() const
{
	// TODO: Use a more efficient way
	return m_pEnt ? IndexFromPointer(object(CPointer((unsigned long) m_pEnt))) : -1;
}


void export_engine_trace()
{
	// Since Ray_t has members of the type AlignedVector that uses ALIGN16, we have
	// to declare this class as noncopyable.
	class_<Ray_t, boost::noncopyable>("Ray", no_init)
		.def("__init__", make_constructor(&Ray_tExt::CreateRay1))
		.def("__init__", make_constructor(&Ray_tExt::CreateRay2))
	;

	class_<IEngineTrace, boost::noncopyable>("_EngineTrace", no_init)
		.def("get_point_contents",
			&IEngineTraceExt::GetPointContents,
			"Returns the contents mask and the entities at the given position.",
			args("position")
		)

		.def("get_point_contents_of_collidable",
			&IEngineTrace::GetPointContents_Collideable,
			"Returns the content mask, but only tests the given entity.",
			args("entity", "position")
		)

		.def("clip_ray_to_entity",
			&IEngineTrace::ClipRayToEntity,
			"Traces a ray against a particular entity.",
			args("ray", "mask", "entity", "trace")
		)

		.def("clip_ray_to_collidable",
			&IEngineTrace::ClipRayToCollideable,
			"Traces a ray against a particular entity.",
			args("ray", "mask", "entity", "trace")
		)

		.def("trace_ray",
			&IEngineTrace::TraceRay,
			"A version that simply accepts a ray (can work as a traceline or tracehull).",
			args("ray", "mask", "filter", "trace")
		)

		.def("enumerate_entities",
			GET_METHOD(void, IEngineTrace, EnumerateEntities, const Ray_t&, bool, IEntityEnumerator*),
			"Enumerates over all entities along a ray.",
			args("ray", "triggers", "enumerator")
		)

		.def("enumerate_entities_in_box",
			GET_METHOD(void, IEngineTrace, EnumerateEntities, const Vector&, const Vector&, IEntityEnumerator*),
			"Enumerates over all entities within a box.",
			args("mins", "maxs", "enumerator")
		)

		.def("get_collideable",
			&IEngineTrace::GetCollideable,
			"Convert a handle entity to a collideable.",
			args("entity"),
			reference_existing_object_policy()
		)

		.def("is_point_outside_of_world",
			&IEngineTrace::PointOutsideWorld,
			"Tests a point to see if it's outside any playable area.",
			args("position")
		)

		.def("sweep_collideable",
			&IEngineTrace::SweepCollideable,
			"Sweeps a collideable through the world.",
			args("collideable", "start", "end", "angles", "mask", "filter", "trace")
		)

		/*
		//finds brushes in an AABB, prone to some false positives
		virtual void GetBrushesInAABB( const Vector &vMins, const Vector &vMaxs, CUtlVector<int> *pOutput, int iContentsMask = 0xFFFFFFFF ) = 0;

		//retrieve brush planes and contents, returns true if data is being returned in the output pointers, false if the brush doesn't exist
		virtual bool GetBrushInfo( int iBrush, CUtlVector<Vector4D> *pPlanesOut, int *pContentsOut ) = 0;
		*/
	;

	scope().attr("EngineTrace") = object(ptr(enginetrace));

	class_<CBaseTrace, boost::noncopyable>("BaseTrace")
		.def_readwrite("start_position",
			&CBaseTrace::startpos
		)

		.def_readwrite("end_position",
			&CBaseTrace::endpos
		)

		.def_readwrite("plane",
			&CBaseTrace::plane
		)

		.def_readwrite("fraction",
			&CBaseTrace::fraction
		)

		.def_readwrite("contents",
			&CBaseTrace::contents
		)

		.def_readwrite("displacement_flags",
			&CBaseTrace::dispFlags
		)

		.def_readwrite("all_solid",
			&CBaseTrace::allsolid
		)

		.def_readwrite("start_solid",
			&CBaseTrace::startsolid
		)
	;
	
	scope().attr("DISPSURF_FLAG_SURFACE") = DISPSURF_FLAG_SURFACE;
	scope().attr("DISPSURF_FLAG_WALKABLE") = DISPSURF_FLAG_WALKABLE;
	scope().attr("DISPSURF_FLAG_BUILDABLE") = DISPSURF_FLAG_BUILDABLE;
	scope().attr("DISPSURF_FLAG_SURFPROP1") = DISPSURF_FLAG_SURFPROP1;
	scope().attr("DISPSURF_FLAG_SURFPROP2") = DISPSURF_FLAG_SURFPROP2;

	class_<CGameTrace, bases<CBaseTrace>, boost::noncopyable>("GameTrace")
		.def("did_hit_world",
			&CGameTrace::DidHitWorld,
			"Returns True if the ray hit the world entity."
		)

		.def("did_hit",
			&CGameTrace::DidHit,
			"Returns true if there was any kind of impact at all"
		)

		.def("get_entity_index",
			&CGameTrace::GetEntityIndex,
			"Returns the index of the entity that was hit."
		)

		.def("get_entity",
			&CGameTraceExt::GetEntity,
			"Returns the entity instance that was hit.",
			reference_existing_object_policy()
		)

		.def_readwrite("fraction_left_solid",
			&CGameTrace::fractionleftsolid
		)

		.def_readwrite("surface",
			&CGameTrace::surface
		)

		.def_readwrite("hitgroup",
			&CGameTrace::hitgroup
		)

		.def_readwrite("physicsbone",
			&CGameTrace::physicsbone
		)

		.def_readwrite("hitbox",
			&CGameTrace::hitbox
		)
	;

	class_<csurface_t>("Surface")
		.def_readwrite("name",
			&csurface_t::name
		)

		.def_readwrite("surface_props",
			&csurface_t::surfaceProps
		)

		.def_readwrite("flags",
			&csurface_t::flags
		)
	;

	// Trace filter baseclass
	class_<ITraceFilterWrap, boost::noncopyable>("TraceFilter")
		.def("should_hit_entity",
			pure_virtual(&ITraceFilterWrap::ShouldHitEntity),
			"Returns True if the a should hit the entity."
		)

		.def("get_trace_type",
			pure_virtual(&ITraceFilterWrap::GetTraceType),
			"Returns the trace type."
		)
	;

	// Enumerator baseclass
	class_<IEntityEnumeratorWrap, boost::noncopyable>("EntityEnumerator")
		.def("enum_entity",
			&IEntityEnumeratorWrap::EnumEntity,
			"Gets called with each handle."
		)
	;

	// Trace types
	enum_<TraceType_t>("TraceType")
		.value("EVERYTHING", TRACE_EVERYTHING)
		.value("WORLD_ONLY", TRACE_WORLD_ONLY)
		.value("ENTITIES_ONLY", TRACE_ENTITIES_ONLY)
		.value("EVERYTHING_FILTER_PROPS", TRACE_EVERYTHING_FILTER_PROPS)
	;

	// Content flags
	scope().attr("CONTENTS_EMPTY") = CONTENTS_EMPTY;
	scope().attr("CONTENTS_SOLID") = CONTENTS_SOLID;
	scope().attr("CONTENTS_WINDOW") = CONTENTS_WINDOW;
	scope().attr("CONTENTS_AUX") = CONTENTS_AUX;
	scope().attr("CONTENTS_GRATE") = CONTENTS_GRATE;
	scope().attr("CONTENTS_SLIME") = CONTENTS_SLIME;
	scope().attr("CONTENTS_WATER") = CONTENTS_WATER;
	scope().attr("CONTENTS_BLOCKLOS") = CONTENTS_BLOCKLOS;
	scope().attr("CONTENTS_OPAQUE") = CONTENTS_OPAQUE;
	scope().attr("LAST_VISIBLE_CONTENTS") = LAST_VISIBLE_CONTENTS;
	scope().attr("ALL_VISIBLE_CONTENTS") = ALL_VISIBLE_CONTENTS;
	scope().attr("CONTENTS_TESTFOGVOLUME") = CONTENTS_TESTFOGVOLUME;
	scope().attr("CONTENTS_UNUSED") = CONTENTS_UNUSED;
	scope().attr("CONTENTS_TEAM1") = CONTENTS_TEAM1;
	scope().attr("CONTENTS_TEAM2") = CONTENTS_TEAM2;
	scope().attr("CONTENTS_IGNORE_NODRAW_OPAQUE") = CONTENTS_IGNORE_NODRAW_OPAQUE;
	scope().attr("CONTENTS_MOVEABLE") = CONTENTS_MOVEABLE;
	scope().attr("CONTENTS_AREAPORTAL") = CONTENTS_AREAPORTAL;
	scope().attr("CONTENTS_PLAYERCLIP") = CONTENTS_PLAYERCLIP;
	scope().attr("CONTENTS_MONSTERCLIP") = CONTENTS_MONSTERCLIP;
	scope().attr("CONTENTS_CURRENT_0") = CONTENTS_CURRENT_0;
	scope().attr("CONTENTS_CURRENT_90") = CONTENTS_CURRENT_90;
	scope().attr("CONTENTS_CURRENT_180") = CONTENTS_CURRENT_180;
	scope().attr("CONTENTS_CURRENT_270") = CONTENTS_CURRENT_270;
	scope().attr("CONTENTS_CURRENT_UP") = CONTENTS_CURRENT_UP;
	scope().attr("CONTENTS_CURRENT_DOWN") = CONTENTS_CURRENT_DOWN;
	scope().attr("CONTENTS_ORIGIN") = CONTENTS_ORIGIN;
	scope().attr("CONTENTS_MONSTER") = CONTENTS_MONSTER;
	scope().attr("CONTENTS_DEBRIS") = CONTENTS_DEBRIS;
	scope().attr("CONTENTS_DETAIL") = CONTENTS_DETAIL;
	scope().attr("CONTENTS_TRANSLUCENT") = CONTENTS_TRANSLUCENT;
	scope().attr("CONTENTS_LADDER") = CONTENTS_LADDER;
	scope().attr("CONTENTS_HITBOX") = CONTENTS_HITBOX;

	// Masks
	scope().attr("MASK_ALL") = MASK_ALL;
	scope().attr("MASK_SOLID") = MASK_SOLID;
	scope().attr("MASK_PLAYERSOLID") = MASK_PLAYERSOLID;
	scope().attr("MASK_NPCSOLID") = MASK_NPCSOLID;
	scope().attr("MASK_WATER") = MASK_WATER;
	scope().attr("MASK_OPAQUE") = MASK_OPAQUE;
	scope().attr("MASK_OPAQUE_AND_NPCS") = MASK_OPAQUE_AND_NPCS;
	scope().attr("MASK_BLOCKLOS") = MASK_BLOCKLOS;
	scope().attr("MASK_BLOCKLOS_AND_NPCS") = MASK_BLOCKLOS_AND_NPCS;
	scope().attr("MASK_VISIBLE") = MASK_VISIBLE;
	scope().attr("MASK_VISIBLE_AND_NPCS") = MASK_VISIBLE_AND_NPCS;
	scope().attr("MASK_SHOT") = MASK_SHOT;
	scope().attr("MASK_SHOT_HULL") = MASK_SHOT_HULL;
	scope().attr("MASK_SHOT_PORTAL") = MASK_SHOT_PORTAL;
	scope().attr("MASK_SOLID_BRUSHONLY") = MASK_SOLID_BRUSHONLY;
	scope().attr("MASK_PLAYERSOLID_BRUSHONLY") = MASK_PLAYERSOLID_BRUSHONLY;
	scope().attr("MASK_NPCWORLDSTATIC") = MASK_NPCWORLDSTATIC;
	scope().attr("MASK_SPLITAREAPORTAL") = MASK_SPLITAREAPORTAL;
	scope().attr("MASK_CURRENT") = MASK_CURRENT;
	scope().attr("MASK_DEADSOLID") = MASK_DEADSOLID;
	
	// Surface flags
	scope().attr("SURF_LIGHT") = SURF_LIGHT;
	scope().attr("SURF_SKY2D") = SURF_SKY2D;
	scope().attr("SURF_SKY") = SURF_SKY;
	scope().attr("SURF_WARP") = SURF_WARP;
	scope().attr("SURF_TRANS") = SURF_TRANS;
	scope().attr("SURF_NOPORTAL") = SURF_NOPORTAL;
	scope().attr("SURF_TRIGGER") = SURF_TRIGGER;
	scope().attr("SURF_NODRAW") = SURF_NODRAW;
	scope().attr("SURF_HINT") = SURF_HINT;
	scope().attr("SURF_SKIP") = SURF_SKIP;
	scope().attr("SURF_NOLIGHT") = SURF_NOLIGHT;
	scope().attr("SURF_BUMPLIGHT") = SURF_BUMPLIGHT;
	scope().attr("SURF_NOSHADOWS") = SURF_NOSHADOWS;
	scope().attr("SURF_NODECALS") = SURF_NODECALS;
	scope().attr("SURF_NOCHOP") = SURF_NOCHOP;
	scope().attr("SURF_HITBOX") = SURF_HITBOX;
}
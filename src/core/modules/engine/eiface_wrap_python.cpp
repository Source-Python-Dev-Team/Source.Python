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

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------

// Required to fix compilation errors after including cdll_int.h
#if defined( _WIN32 )
#include <Windows.h>
#endif

#include "modules/export_main.h"
#include "dt_send.h"
#include "server_class.h"
#include "iscratchpad3d.h"
#include "cdll_int.h"
#include "con_nprint.h"
#include "iachievementmgr.h"
#include "client_textmessage.h"
#include "steam/steamclientpublic.h"
#include "eiface_wrap.h"

//---------------------------------------------------------------------------------
// Namespaces to use
//---------------------------------------------------------------------------------
using namespace boost::python;

//---------------------------------------------------------------------------------
// Static singletons.
//---------------------------------------------------------------------------------
static CEngineServer s_EngineServer;

//---------------------------------------------------------------------------------
// Engine accessor.
//---------------------------------------------------------------------------------
CEngineServer* get_engine_interface()
{
	return &s_EngineServer;
}

//---------------------------------------------------------------------------------
// Exposer functions.
//---------------------------------------------------------------------------------
void export_engine_interface();

//---------------------------------------------------------------------------------
// Method overloads
//---------------------------------------------------------------------------------
DECLARE_CLASS_METHOD_OVERLOAD(CEngineServer, precache_model,			1, 2);
DECLARE_CLASS_METHOD_OVERLOAD(CEngineServer, precache_sentence_file,	1, 2);
DECLARE_CLASS_METHOD_OVERLOAD(CEngineServer, precache_decal,			1, 2);
DECLARE_CLASS_METHOD_OVERLOAD(CEngineServer, precache_generic,			1, 2);

//---------------------------------------------------------------------------------
// Exposes the engine module.
//---------------------------------------------------------------------------------
DECLARE_SP_MODULE(engine_c)
{
	export_engine_interface();
}

//---------------------------------------------------------------------------------
// Exposes IVEngineServer.
//---------------------------------------------------------------------------------
void export_engine_interface()
{
	// ----------------------------------------------------------
	// Engine accessor.
	// ----------------------------------------------------------
	BOOST_FUNCTION(get_engine_interface,
		"Returns the engine interface.",
		reference_existing_object_policy()
	);

	// ----------------------------------------------------------
	// The engine interface.
	// ----------------------------------------------------------
	BOOST_ABSTRACT_CLASS( CEngineServer )

		CLASS_METHOD(CEngineServer,
			change_level,
			"Tells the engine to change the level. If s2 is None, the engine will execute a \
			changelevel command. If s2 is a valid map, the engine will execute a changelevel2 \
			command",
			args("s1", "s2")
		)

		CLASS_METHOD(CEngineServer,
			is_map_valid,
			"Returns true if filename refers to a valid map.",
			args("filename")
		)

		CLASS_METHOD(CEngineServer,
			is_dedicated_server,
			"Returns true if the engine is running in dedicated mode."
		)

		CLASS_METHOD(CEngineServer,
			is_in_edit_mode,
			"Returns false if the engine is not in hammer editing mode."
		)

		CLASS_METHOD(CEngineServer,
			get_launch_options,
			"Get arbitrary launch options",
			reference_existing_object_policy()
		)

		CLASS_METHOD_OVERLOAD(CEngineServer,
			precache_model,
			"Precaches a model and returns an integer containing its index.",
			args("s", "preload")
		)

		CLASS_METHOD_OVERLOAD(CEngineServer,
			precache_sentence_file,
			"Precaches a sentence file and returns an integer containing its index.",
			args("s", "preload")
		)

		CLASS_METHOD_OVERLOAD(CEngineServer,
			precache_decal,
			"Precaches a decal file and returns an integer containing its index.",
			args("name", "preload")
		)

		CLASS_METHOD_OVERLOAD(CEngineServer,
			precache_generic,
			"Precaches a generic asset file and returns an integer containing its index.",
			args("s", "preload")
		)

		CLASS_METHOD(CEngineServer,
			is_model_precached,
			"Returns true if the given model is precached.",
			args("s")
		)

		CLASS_METHOD(CEngineServer,
			is_decal_precached,
			"Returns true if the given decal is precached.",
			args("s")
		)

		CLASS_METHOD(CEngineServer,
			is_generic_precached,
			"Returns true if the given generic asset is precached.",
			args("s")
		)

		CLASS_METHOD(CEngineServer,
			get_cluster_for_origin,
			"Returns the cluster number for the specified position.",
			args("origin")
		)

		CLASS_METHOD(CEngineServer,
			get_pvs_for_cluster,
			"Gets the PVS bits for a specified cluster and copies the bits into outputpvs.",
			args("cluster", "outputpvslength", "outputpvs")
		)

		CLASS_METHOD(CEngineServer,
			check_origin_in_pvs,
			"Check whether the specified origin is inside the PVS",
			args("org", "checkpvs", "checkpvssize")
		)

		CLASS_METHOD(CEngineServer,
			check_box_in_pvs,
			"Checks whether the specified worldspace bounding box is inside the specified PVS",
			args("mins", "maxs", "checkpvs", "checkpvssize")
		)

		CLASS_METHOD(CEngineServer,
			get_player_userid,
			"Returns the server assigned userid for this player. Returns -1 if the edict could not \
			be found.",
			args("playerInstance")
		)

		CLASS_METHOD(CEngineServer,
			get_player_network_id_string,
			"Returns the player's network id as a string.",
			args("playerInstance")
		)

		CLASS_METHOD(CEngineServer,
			is_userid_in_use,
			"Returns true if the userid given is in use.",
			args("userID")
		)

		CLASS_METHOD(CEngineServer,
			get_loading_progress_for_userid,
			"Returns percentage complete a player is while loading.",
			args("userID")
		)

		CLASS_METHOD(CEngineServer,
			get_entity_count,
			"Returns the number of used edict slots."
		)

		CLASS_METHOD(CEngineServer,
			get_player_net_info,
			"Returns stats info interface for a client netchannel.",
			args("playerIndex"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			create_edict,
			"Creates an edict. If iForceEdictIndex is not -1, then it return the edict with that index.",
			args("iForceEdictIndex"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			remove_edict,
			"Remove the specified edict and place back into the free edict list.",
			args("edictInstance")
		)

		CLASS_METHOD(CEngineServer,
			emit_ambient_sound,
			"Emits an ambient sound associated with the specified entity",
			args("entindex", "pos", "samp", "vol", "soundlevel", "fFlags", "pitch", "delay")
		)

		CLASS_METHOD(CEngineServer,
			fade_client_volume,
			"Fade out the client's volume level toward silence (or fadePercent)",
			args("pEdict", "fadePercent", "fadeOutSeconds", "holdTime", "fadeInSeconds")
		)

		CLASS_METHOD(CEngineServer,
			sentence_group_pick,
			args("groupIndex", "name", "nameBufLen")
		)

		CLASS_METHOD(CEngineServer,
			sentence_group_pick_sequential,
			args("groupIndex", "name", "nameBufLen", "sentenceIndex", "reset")
		)

		CLASS_METHOD(CEngineServer,
			sentence_index_from_name,
			args("pSentenceName")
		)

		CLASS_METHOD(CEngineServer,
			sentence_name_from_index,
			args("sentenceIndex")
		)

		CLASS_METHOD(CEngineServer,
			sentence_group_index_from_name,
			args("pGroupName")
		)

		CLASS_METHOD(CEngineServer,
			sentence_group_name_from_index,
			args("groupIndex")
		)

		CLASS_METHOD(CEngineServer,
			sentence_length,
			args("sentenceIndex")
		)

		CLASS_METHOD(CEngineServer,
			server_command,
			"Issues a command to the command parser as if it was typed at the server console.",
			args("command")
		)

		CLASS_METHOD(CEngineServer,
			server_execute,
			"Executes any commands currently in the command parser immediately (instead of once per frame).",
			args("command")
		)

		CLASS_METHOD(CEngineServer,
			client_command,
			"Runs a command on the client.",
			args("pEdict", "szCmd")
		)

		CLASS_METHOD(CEngineServer,
			light_style,
			"Set the lightstyle to the specified value and network the change to any connected clients.",
			args("style", "val")
		)

		CLASS_METHOD(CEngineServer,
			static_decal,
			"Project a static decal onto the specified entity / model (for level placed decals in the .bsp)",
			args("originInEntitySpace", "decalIndex", "entityIndex", "modelIndex", "bLowPriority")
		)

		CLASS_METHOD(CEngineServer,
			message_determine_multicast_recipients,
			"Given the current PVS (or PAS), determine which players should hear/recieve the message.",
			args("usePas", "origin", "playerBits")
		)

		CLASS_METHOD(CEngineServer,
			entity_message_begin,
			"Begin a message from a server side entity to its client side counterpart (func_breakable glass, e.g.)",
			args("ent_index", "ent_class", "bReliable"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			send_user_message,
			"Sends a user message off to the intended recipients.",
			args("msg")
		)

// 		CLASS_METHOD(CEngineServer,
// 			message_end
// 		)
// 
// 		CLASS_METHOD(CEngineServer,
// 			send_user_message
// 		)
		
		CLASS_METHOD(CEngineServer,
			client_printf,
			"Prints szMsg to the client's console.",
			args("pEdict", "szMsg")
		)

		CLASS_METHOD(CEngineServer,
			con_nprintf,
			"LISTEN SERVER ONLY: Prints a string to the notification area of the screen.",
			args("pos", "szString")
		)

		CLASS_METHOD(CEngineServer,
			con_nxprintf,
			"LISTEN SERVER ONLY: Same as con_nprintf but allows you to specify additional information.",
			args("info", "szString")
		)

		CLASS_METHOD(CEngineServer,
			set_view,
			"Change a specified player's \"view entity\"",
			args("pClient", "pViewent")
		)

		CLASS_METHOD(CEngineServer,
			crosshair_angle,
			"Sets the player's crosshair angle",
			args("pClient", "pitch", "yaw")
		)
		
		CLASS_METHOD(CEngineServer,
			get_game_dir,
			"Returns the path to the MOD's game directory.",
			args("maxLength")
		)

		CLASS_METHOD(CEngineServer,
			compare_file_time,
			args("filename1", "filename2", "icompare")
		)

		CLASS_METHOD(CEngineServer,
			lock_network_string_tables,
			"Locks/unlocks the network string tables.",
			args("bLock")
		)

		CLASS_METHOD(CEngineServer,
			create_fake_client,
			"Creates a bot with the given name. Returns NULL if this call fails.",
			args("netname"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			get_client_convar_value,
			"Get a convar keyvalue for specified client.",
			args("clientIndex", "name")
		)

		CLASS_METHOD(CEngineServer,
			parse_file,
			"Parse a token from a file",
			args("data", "token", "maxlen")
		)

		CLASS_METHOD(CEngineServer,
			copy_file,
			"Copies a file. Returns true if the operation succeeded.",
			args("source", "destination")
		)

		CLASS_METHOD(CEngineServer,
			reset_pvs,
			"Resets the potentially visible set. pvssize is the size in bytes of the buffer pointed to by pvs.",
			args("pvs", "pvssize")
		)

		CLASS_METHOD(CEngineServer,
			add_origin_to_pvs,
			"Merge the pvs bits into the current accumulated pvs based on the specified origin.",
			args("origin")
		)

		CLASS_METHOD(CEngineServer,
			set_area_portal_state,
			"Marks a specified area portal as open or closed.",
			args("portalNumber", "isOpen")
		)

		CLASS_METHOD(CEngineServer,
			playback_temp_entity,
			"Queue a temp entity for transmission",
			args("filter", "delay", "pSender", "pST", "classID")
		)

		CLASS_METHOD(CEngineServer,
			check_head_node_visible,
			"Given a node number and the specified PVS, return with the node is in the PVS.",
			args("nodenum", "pvs", "vissize")
		)

		CLASS_METHOD(CEngineServer,
			check_areas_connected,
			"Using area bits, check whether area 1 flows into area 2 and vice versa (depends on portal state)",
			args("area1", "area2")
		)

		CLASS_METHOD(CEngineServer,
			get_area,
			"Given an origin, determine which area index the origin is within.",
			args("origin")
		)

		CLASS_METHOD(CEngineServer,
			get_area_bits,
			"Get area portal bit set.",
			args("area", "bits", "buflen")
		)

		CLASS_METHOD(CEngineServer,
			get_area_portal_plane,
			"Given a view origin and a portal key, fill in the plane that leads out of this area.",
			args("vViewOrigin", "portalKey", "pPlane")
		)

		CLASS_METHOD(CEngineServer,
			load_game_state,
			"Save/Restore wrapper - FIXME: At some point we should move this to its own interface.",
			args("pMapName", "bCreatePlayers")
		)

		CLASS_METHOD(CEngineServer,
			load_adjacent_ents,
			args("pOldLevel", "pLandmarkName")
		)

		CLASS_METHOD(CEngineServer,
			get_map_entities_string,
			"Get the pristine map entity lump string."
		)

		CLASS_METHOD(CEngineServer,
			text_message_get,
			"Text message system -- lookup the text message of the specified name",
			args("pName"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			log_print,
			"Print a message to the server log file.",
			args("msg")
		)

		CLASS_METHOD(CEngineServer,
			is_log_enabled,
			"Returns true if server side logging is enabled."
		)

		CLASS_METHOD(CEngineServer,
			build_entity_cluster_list,
			"Builds PVS information for an entity.",
			args("pEdict", "pPVSInfo")
		)

		CLASS_METHOD(CEngineServer,
			solid_moved,
			"A solid entity moved, update spatial partition.",
			args("pSolidEnt", "pSolidCollide", "pPrevAbsOrigin", "bTestSurroundingBoundsOnly")
		)

		CLASS_METHOD(CEngineServer,
			trigger_moved,
			"A trigger entity moved, update the spatial partition",
			args("pTriggerEnt", "testSurroundingBoundsOnly")
		)

		CLASS_METHOD(CEngineServer,
			create_spatial_partition,
			"Create a custom spatial partition",
			args("worldmin", "worldmax"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			destroy_spatial_partition,
			"Destroy a custom spatial partition",
			args("pPartition")
		)

		CLASS_METHOD(CEngineServer,
			draw_map_to_scratch_pad,
			"Draw the brush geometry in the map into the scratch pad.",
			args("pPad", "iFlags")
		)

		CLASS_METHOD(CEngineServer,
			get_entity_transmit_bits_for_client,
			"This returns which entities, to the server's knowledge the client currently knows about.",
			args("iClientIndex"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			is_paused,
			"Returns true if the game is paused"
		)

		CLASS_METHOD(CEngineServer,
			get_timescale,
			"Returns the game timescale multiplied by host_timescale."
		)

		CLASS_METHOD(CEngineServer,
			force_exact_file,
			"Marks the filename for consistency checking. this should be called after precaching the file.",
			args("file")
		)

		CLASS_METHOD(CEngineServer,
			force_model_bounds,
			"Marks a precached model as having a maximum bounding size on the client.",
			args("s", "mins", "maxs")
		)
			
		CLASS_METHOD(CEngineServer,
			clear_save_dir_after_client_load
		)

		CLASS_METHOD(CEngineServer,
			set_fake_client_convar_value,
			"Sets a USERINFO client ConVar for a fakeclient.",
			args("pEntity", "cvar", "value")
		)

		CLASS_METHOD(CEngineServer,
			force_simple_material,
			"Marks the material (vmt file) for consistency checking.",
			args("s")
		)
			
		CLASS_METHOD(CEngineServer,
			set_area_portal_states,
			"Mark some area portals as open/closed.",
			args("portalNumbers", "isOpen", "nPortals")
		)

		CLASS_METHOD(CEngineServer,
			notify_edict_flags_change,
			"Called when relevant edict state flags change.",
			args("iEdict")
		)

		CLASS_METHOD(CEngineServer,
			get_prev_check_transmit_info,
			args("pPlayerEdict"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			get_shared_edict_change_info,
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			allow_immediate_edict_reuse,
			"Tells the engine we can immediately re-use all the edict indices."
		)

		CLASS_METHOD(CEngineServer,
			is_internal_build,
			"Returns true if the engine is an internal build."
		)

		CLASS_METHOD(CEngineServer,
			get_change_accessor,
			args("pEdict"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			clean_up_entity_cluster_list,
			"Cleans up the cluster list.",
			args("pPVSInfo")
		)

		CLASS_METHOD(CEngineServer,
			set_achievement_mgr,
			"Sets the achievement manager."
		)

		CLASS_METHOD(CEngineServer,
			get_achievement_mgr,
			"Returns the current achievement manager.",
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			get_app_id,
			"Returns the game's appid."
		)

		CLASS_METHOD(CEngineServer,
			is_low_violence,
			"Returns true if the game is in low-violence mode."
		)

		CLASS_METHOD(CEngineServer,
			is_any_client_low_violence,
			"Returns true if a low-violence client is playing."
		)

		CLASS_METHOD(CEngineServer,
			start_query_cvar_value,
			"Returns the value of a cvar on the client."
		)

		CLASS_METHOD(CEngineServer,
			insert_server_command,
			"Inserts a command into the server's command buffer."
		)

		CLASS_METHOD(CEngineServer,
			get_player_info,
			"Fill in the player info structure for the specified player.",
			args("ent_num", "pinfo")
		)

		CLASS_METHOD(CEngineServer,
			is_client_fully_authenticated,
			"Returns true if this client has been fully authenticated by Steam.",
			args("pEdict")
		)

		CLASS_METHOD(CEngineServer,
			set_dedicated_server_benchmark_mode,
			"Makes the host run 1 tick per frame.",
			args("bBenchmarkMode")
		)

		CLASS_METHOD(CEngineServer,
			is_split_screen_player,
			args("ent_num")
		)

		CLASS_METHOD(CEngineServer,
			get_split_screen_player_attach_to_edict,
			args("ent_num"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			get_num_split_screen_users_attached_to_edict,
			args("ent_num")
		)

		CLASS_METHOD(CEngineServer,
			get_split_screen_player_for_edict,
			args("ent_num", "nSlot"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			get_cluster_count,
			"Returns total number of clusters."
		)

		CLASS_METHOD(CEngineServer,
			get_all_cluster_bounds,
			"Gets a list of all clusters and bounds.",
			args("pBBoxList", "maxBBox")
		)

		CLASS_METHOD(CEngineServer,
			is_creating_reslist
		)

		CLASS_METHOD(CEngineServer,
			pause,
			"Pauses the game.",
			args("bPause", "bForce")
		)

		CLASS_METHOD(CEngineServer,
			set_timescale,
			"Sets simulation timescale.",
			args("flTimescale")
		)

		/*
		CLASS_METHOD(CEngineServer,
			set_gamestats_data,
			"Sets the gamestats data container.",
			args("pGamestatsData")
		)

		CLASS_METHOD(CEngineServer,
			get_gamestats_data,
			"Returns the gamestats data container.",
			reference_existing_object_policy()
		) 
		*/

		CLASS_METHOD(CEngineServer,
			get_client_steamid,
			"Returns the SteamID of the specified player. Returns NULL if the player isn't authenticated.",
			args("pPlayerEdict"),
			reference_existing_object_policy()
		)
	
		CLASS_METHOD(CEngineServer,
			get_game_server_steamid,
			"Returns the SteamID of the game server.",
			reference_existing_object_policy()
		)

		CLASS_METHOD(CEngineServer,
			host_validate_session,
			"Validates the session."
		)

		CLASS_METHOD(CEngineServer,
			client_command_key_values,
			"Sends a client command keyvalues which are deleted inside this function.",
			args("pEdict", "pCommand")
		)

		CLASS_METHOD(CEngineServer,
			set_no_clip_enabled,
			"Enables / Disables noclip.",
			args("bEnabled")
		)

		CLASS_METHOD(CEngineServer,
			get_latency_for_choreo_sounds,
			"Returns latency for choreo sounds."
		)

		CLASS_METHOD(CEngineServer,
			get_client_cross_play_platform,
			args("client_index")
		)
	
		CLASS_METHOD(CEngineServer,
			ensure_instance_baseline,
			args("ent_num")
		)

		CLASS_METHOD(CEngineServer,
			reserver_server_for_queued_game,
			args("szReservationPayload")
		)

	BOOST_END_CLASS()
}

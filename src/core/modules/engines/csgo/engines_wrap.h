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

#ifndef _ENGINES_WRAP_CSGO_H
#define _ENGINES_WRAP_CSGO_H

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
#include "eiface.h"
#include "ispsharedmemory.h"
#include "engine/IEngineSound.h"
#include "engine/IEngineTrace.h"


//---------------------------------------------------------------------------------
// External variables.
//---------------------------------------------------------------------------------
extern IEngineTrace* enginetrace;


//---------------------------------------------------------------------------------
// IVEngineServer visitor function.
//---------------------------------------------------------------------------------
template<class T>
T IVEngineServer_Visitor(T cls)
{
	cls
		.add_property("game_server_steamid",
			make_function(
				&IVEngineServer::GetGameServerSteamID,
				reference_existing_object_policy()
			),
			"Returns the SteamID of the game server."
		)

		.add_property("server_version",
			&IVEngineServer::GetServerVersion,
			"Returns the version of the server."
		)

		.add_property("launch_options",
			make_function(
				&IVEngineServer::GetLaunchOptions,
				reference_existing_object_policy()
			),
			"Returns abitrary launch options"
		)

		.def("is_userid_in_use",
			&IVEngineServer::IsUserIDInUse,
			"Returns whether the user ID is in use.",
			args("userid")
		)

		.def("get_loading_process_for_userid",
			&IVEngineServer::GetLoadingProgressForUserID,
			args("userid")
		)

		.def("is_log_enabled",
			&IVEngineServer::IsLogEnabled,
			"Returns whether logging is enabled."
		)

		.add_property("timescale",
			&IVEngineServer::GetTimescale,
			&IVEngineServer::SetTimescale
		)

		.def("is_level_main_menu_background",
			&IVEngineServer::IsLevelMainMenuBackground,
			"Returns whether the engine is running a background map."
		)

		.def("is_any_client_low_violence",
			&IVEngineServer::IsAnyClientLowViolence,
			args("ent_num")
		)

		.def("is_split_screen_player",
			&IVEngineServer::IsSplitScreenPlayer,
			args("ent_num")
		)

		.def("get_split_screen_player_attack_to_edict",
			&IVEngineServer::GetSplitScreenPlayerAttachToEdict,
			args("ent_num"),
			reference_existing_object_policy()
		)

		.def("get_num_split_screen_users_attached_to_edict",
			&IVEngineServer::GetNumSplitScreenUsersAttachedToEdict,
			args("ent_num")
		)

		.def("get_split_screen_player_for_edict",
			&IVEngineServer::GetSplitScreenPlayerForEdict,
			args("ent_num", "slot"),
			reference_existing_object_policy()
		)

		.def("is_override_load_game_ents_on",
			&IVEngineServer::IsOverrideLoadGameEntsOn
		)

		.def("force_flush_entity",
			&IVEngineServer::ForceFlushEntity,
			args("entity")
		)

		.def("get_single_player_shared_memory_space",
			&IVEngineServer::GetSinglePlayerSharedMemorySpace,
			"Finds or creates a shared memory_space.",
			("name", arg("ent_num")=MAX_EDICTS),
			reference_existing_object_policy()
		)

		/*
		// TODO: void*
		.def("alloc_level_specific_data",
			&IVEngineServer::AllocLevelStaticData,
			"Allocates hunk memory.",
			args("bytes"),
			reference_existing_object_policy()
		)
		*/

		.def("is_creating_reslist",
			&IVEngineServer::IsCreatingReslist
		)

		.def("is_creating_xbox_reslist",
			&IVEngineServer::IsCreatingXboxReslist
		)

		.def("is_dedicated_server_for_xbox",
			&IVEngineServer::IsDedicatedServerForXbox
		)

		.def("is_dedicated_server_for_ps3",
			&IVEngineServer::IsDedicatedServerForPS3
		)

		.def("pause",
			&IVEngineServer::Pause,
			args("pause", "force")
		)

		.add_property("host_validated_session",
			&IVEngineServer::HostValidateSession,
			"Validates session."
		)

		.def("refresh_screen_if_necessary",
			&IVEngineServer::RefreshScreenIfNecessary,
			"Update the 360 pacifier/spinner."
		)

		.def("has_paint_map",
			&IVEngineServer::HasPaintmap,
			"Tells the engine to allocate paint surfaces."
		)
		/*
		.def("sphere_paint_surface",
			&IVEngineServer::SpherePaintSurface,
			args("model", "vector", "uchar", "float", "float")
		)

		.def("sphere_trace_paint_surface",
			&IVEngineServer::SphereTracePaintSurface,
			args("model", "vector", "vector", "float", "CUtlVector<unsigned char>")
		)
		*/

		.def("remove_all_paint",
			&IVEngineServer::RemoveAllPaint
		)

		.def("paint_all_surfaces",
			&IVEngineServer::PaintAllSurfaces,
			args("uchar")
		)

		/*
		.def("remove_paint",
			&IVEngineServer::RemovePaint,
			args("model")
		)
		*/

		.def("get_client_xuid",
			&IVEngineServer::GetClientXUID,
			args("edict")
		)

		.def("is_active_app",
			&IVEngineServer::IsActiveApp
		)

		.def("set_noclip_enabled",
			&IVEngineServer::SetNoClipEnabled,
			args("enabled")
		)

		.def("get_paint_map_data_rle",
			&IVEngineServer::GetPaintmapDataRLE,
			args("mapdata")
		)

		.def("load_paint_map_data_rle",
			&IVEngineServer::LoadPaintmapDataRLE,
			args("mapdata")
		)

		.def("send_paint_map_data_to_client",
			&IVEngineServer::SendPaintmapDataToClient,
			args("edict")
		)

		.add_property("latency_for_choreo_sounds",
			&IVEngineServer::GetLatencyForChoreoSounds
		)

		.def("get_client_cross_player_platform",
			&IVEngineServer::GetClientCrossPlayPlatform,
			args("ent_num")
		)

		.def("ensure_instance_baseline",
			&IVEngineServer::EnsureInstanceBaseline,
			args("ent_num")
		)

		.def("reserve_server_for_queued_game",
			&IVEngineServer::ReserveServerForQueuedGame,
			args("reservation_payload")
		)
		
		/*
		.def("get_engine_hltv_info",
			&IVEngineServer::GetEngineHltvInfo,
			args("out")
		)
		*/
	;

	return cls;
}


//---------------------------------------------------------------------------------
// IEngineSound
//---------------------------------------------------------------------------------
// Visitor function
template<class T>
T IEngineSound_Visitor(T cls)
{
	cls
		.def("is_looping_sound",
			&IEngineSound::IsLoopingSound,
			args("sample"),
			"Returns True if the given sample is looping."
		)
	;
	
	scope().attr("CONTENTS_BLOCKLIGHT") = CONTENTS_BLOCKLIGHT;

	scope().attr("SURF_NOPAINT") = SURF_NOPAINT;

	scope().attr("MASK_NPCFLUID") = MASK_NPCFLUID;
	scope().attr("MASK_SHOT_BRUSHONLY") = MASK_SHOT_BRUSHONLY;
	scope().attr("MASK_NPCWORLDSTATIC_FLUID") = MASK_NPCWORLDSTATIC_FLUID;

	return cls;
}


#endif // _ENGINES_WRAP_CSGO_H

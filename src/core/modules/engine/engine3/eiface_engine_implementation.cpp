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

#include "eiface_engine_implementation.h"

KeyValues * CEngineServerImplementation::get_launch_options()
{
	return engine->GetLaunchOptions();
}

bool CEngineServerImplementation::is_userid_in_use( int userID )
{
	return engine->IsUserIDInUse(userID);
}

int CEngineServerImplementation::get_loading_progress_for_userid( int userID )
{
	return engine->GetLoadingProgressForUserID(userID);
}

bool CEngineServerImplementation::is_log_enabled()
{
	return engine->IsLogEnabled();
}

ISpatialPartition * CEngineServerImplementation::create_spatial_partition(
	const CVector &worldmin, const CVector &worldmax)
{
	return engine->CreateSpatialPartition(worldmin, worldmax);
}

float CEngineServerImplementation::get_timescale() const
{
	return engine->GetTimescale();
}

bool CEngineServerImplementation::is_level_main_menu_background()
{
	return engine->IsLevelMainMenuBackground();
}

bool CEngineServerImplementation::is_any_client_low_violence()
{
	return engine->IsAnyClientLowViolence();
}

bool CEngineServerImplementation::is_split_screen_player(int ent_num)
{
	return engine->IsSplitScreenPlayer(ent_num);
}

CEdict* CEngineServerImplementation::get_split_screen_player_attach_to_edict(int ent_num)
{
	return new CEdict(engine->GetSplitScreenPlayerAttachToEdict(ent_num));
}

int CEngineServerImplementation::get_num_split_screen_users_attached_to_edict(int ent_num)
{
	return engine->GetNumSplitScreenUsersAttachedToEdict(ent_num);
}

CEdict* CEngineServerImplementation::get_split_screen_player_for_edict(int ent_num, int slot)
{
	return new CEdict(engine->GetSplitScreenPlayerForEdict(ent_num, slot));
}

bool CEngineServerImplementation::is_override_load_game_ents_on()
{
	return engine->IsOverrideLoadGameEntsOn();
}


void CEngineServerImplementation::force_flush_entity(int ent_index)
{
	engine->ForceFlushEntity(ent_index);
}

ISPSharedMemory *CEngineServerImplementation::get_single_player_shared_memory_space(const char *name, int ent_num)
{
	return engine->GetSinglePlayerSharedMemorySpace(name, ent_num);
}

void *CEngineServerImplementation::alloc_level_static_data(size_t bytes)
{
	return engine->AllocLevelStaticData(bytes);
}

bool CEngineServerImplementation::is_creating_reslist()
{
	return engine->IsCreatingReslist();
}

bool CEngineServerImplementation::is_creating_xbox_reslist()
{
	return engine->IsCreatingXboxReslist();
}

bool CEngineServerImplementation::is_dedicated_server_for_xbox()
{
	return engine->IsDedicatedServerForXbox();
}

bool CEngineServerImplementation::is_dedicated_server_for_ps3()
{
	return engine->IsDedicatedServerForPS3();
}

void CEngineServerImplementation::pause( bool bPause, bool bForce /*= false */ )
{
	engine->Pause(bPause, bForce);
}

void CEngineServerImplementation::set_timescale( float flTimescale )
{
	engine->SetTimescale(flTimescale);
}

void CEngineServerImplementation::host_validate_session()
{
	engine->HostValidateSession();
}

void CEngineServerImplementation::refresh_screen_if_necessary()
{
	engine->RefreshScreenIfNecessary();
}

bool CEngineServerImplementation::has_paintmap()
{
	return engine->HasPaintmap();
}

bool CEngineServerImplementation::sphere_paint_surface( const model_t *pModel, const CVector & vPosition, unsigned char color, float flSphereRadius, float flPaintCoatPercent )
{
	return engine->SpherePaintSurface(pModel, vPosition, color, flSphereRadius, flPaintCoatPercent);
}

void CEngineServerImplementation::sphere_trace_paint_surface( const model_t *pModel, const CVector & vPosition, const CVector & vContactNormal, float flSphereRadius, CUtlVector<unsigned char> & surfColors )
{
	engine->SphereTracePaintSurface(pModel, vPosition, vContactNormal, flSphereRadius, surfColors);
}

void CEngineServerImplementation::remove_all_paint()
{
	engine->RemoveAllPaint();
}

void CEngineServerImplementation::paint_all_surfaces( unsigned char color )
{
	engine->PaintAllSurfaces(color);
}

void CEngineServerImplementation::remove_paint( const model_t *pModel )
{
	engine->RemovePaint(pModel);
}

uint64 CEngineServerImplementation::get_client_xuid( CEdict* edict )
{
	return engine->GetClientXUID(edict->get_edict());
}

bool CEngineServerImplementation::is_active_app()
{
	return engine->IsActiveApp();
}

void CEngineServerImplementation::set_no_clip_enabled( bool bEnabled )
{
	engine->SetNoClipEnabled(bEnabled);
}

void CEngineServerImplementation::get_paint_map_data_rle( CUtlVector<unsigned int> &mapdata )
{
	engine->GetPaintmapDataRLE(mapdata);
}

void CEngineServerImplementation::load_paint_map_data_rle( CUtlVector<unsigned int> &mapdata )
{
	engine->LoadPaintmapDataRLE(mapdata);
}

void CEngineServerImplementation::send_paint_map_data_to_client( CEdict* edict )
{
	engine->SendPaintmapDataToClient(edict->get_edict());
}

float CEngineServerImplementation::get_latency_for_choreo_sounds()
{
	return engine->GetLatencyForChoreoSounds();
}

int CEngineServerImplementation::get_client_cross_play_platform( int client_index )
{
	return engine->GetClientCrossPlayPlatform(client_index);
}

void CEngineServerImplementation::ensure_instance_baseline( int ent_num )
{
	engine->EnsureInstanceBaseline(ent_num);
}

bool CEngineServerImplementation::reserver_server_for_queued_game( const char *szReservationPayload )
{
	return engine->ReserveServerForQueuedGame(szReservationPayload);
}
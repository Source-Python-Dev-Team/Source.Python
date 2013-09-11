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
#include "eiface_engine_base.h"
#include "../../utility/sp_util.h"

//---------------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------------
extern CGlobalVars *gpGlobals;

//---------------------------------------------------------------------------------
// Methods
//---------------------------------------------------------------------------------
KeyValues * CEngineServerImplementationBase::get_launch_options()
{
	return NULL;
}

bool CEngineServerImplementationBase::is_userid_in_use( int userID )
{
	for(int i = 1; i <= gpGlobals->maxClients; i++)
	{
		edict_t *pPlayer = PEntityOfEntIndex(i);

		if( !pPlayer || pPlayer->IsFree() )
		{
			continue;
		}
		if (engine->GetPlayerUserId(pPlayer) == userID)
		{
			return true;
		}
	}
	return false;
}

int CEngineServerImplementationBase::get_loading_progress_for_userid( int userID )
{
	return 0;
}

bool CEngineServerImplementationBase::is_log_enabled()
{
	return false;
}

ISpatialPartition * CEngineServerImplementationBase::create_spatial_partition( const CVector &worldmin, const CVector &worldmax )
{
	return NULL;
}

float CEngineServerImplementationBase::get_timescale() const
{
	return 1.0f;
}

bool CEngineServerImplementationBase::is_level_main_menu_background()
{
	return false;
}

bool CEngineServerImplementationBase::is_any_client_low_violence()
{
	return false;
}

bool CEngineServerImplementationBase::is_split_screen_player( int ent_index )
{
	return false;
}

CEdict* CEngineServerImplementationBase::get_split_screen_player_attach_to_edict( int ent_num )
{
	return NULL;
}

int CEngineServerImplementationBase::get_num_split_screen_users_attached_to_edict( int ent_num )
{
	return 0;
}

CEdict* CEngineServerImplementationBase::get_split_screen_player_for_edict(int ent_num, int slot)
{
	return NULL;
}

bool CEngineServerImplementationBase::is_override_load_game_ents_on()
{
	return false;
}

void CEngineServerImplementationBase::force_flush_entity(int ent_index)
{
}

ISPSharedMemory * CEngineServerImplementationBase::get_single_player_shared_memory_space( const char *name, int ent_num )
{
	return NULL;
}

void * CEngineServerImplementationBase::alloc_level_static_data( size_t bytes )
{
	return NULL;
}

bool CEngineServerImplementationBase::is_creating_reslist()
{
	return false;
}

bool CEngineServerImplementationBase::is_creating_xbox_reslist()
{
	return false;
}

bool CEngineServerImplementationBase::is_dedicated_server_for_xbox()
{
	return false;
}

bool CEngineServerImplementationBase::is_dedicated_server_for_ps3()
{
	return false;
}

void CEngineServerImplementationBase::pause( bool bPause, bool bForce /*= false */ )
{
}

void CEngineServerImplementationBase::set_timescale( float flTimescale )
{
}

void CEngineServerImplementationBase::host_validate_session()
{
}

void CEngineServerImplementationBase::refresh_screen_if_necessary()
{
}

bool CEngineServerImplementationBase::has_paintmap()
{
	return false;
}

bool CEngineServerImplementationBase::sphere_paint_surface( const model_t *pModel, const CVector & vPosition, unsigned char color, float flSphereRadius, float flPaintCoatPercent )
{
	return false;
}

void CEngineServerImplementationBase::sphere_trace_paint_surface( const model_t *pModel, const CVector & vPosition, const CVector & vContactNormal, float flSphereRadius, CUtlVector<unsigned char> & surfColors )
{
}

void CEngineServerImplementationBase::remove_all_paint()
{
}

void CEngineServerImplementationBase::paint_all_surfaces( unsigned char color )
{
}

void CEngineServerImplementationBase::remove_paint( const model_t *pModel )
{
}

uint64 CEngineServerImplementationBase::get_client_xuid( CEdict* edict )
{
	return 0;
}

bool CEngineServerImplementationBase::is_active_app()
{
	return true;
}

void CEngineServerImplementationBase::set_no_clip_enabled( bool bEnabled )
{
}

void CEngineServerImplementationBase::get_paint_map_data_rle( CUtlVector<unsigned int> &mapdata )
{
}

void CEngineServerImplementationBase::load_paint_map_data_rle( CUtlVector<unsigned int> &mapdata )
{
}

void CEngineServerImplementationBase::send_paint_map_data_to_client( CEdict* edict )
{
}

float CEngineServerImplementationBase::get_latency_for_choreo_sounds()
{
	return 0.0f;
}

int CEngineServerImplementationBase::get_client_cross_play_platform( int client_index )
{
	return 0;
}

void CEngineServerImplementationBase::ensure_instance_baseline( int ent_num )
{
}

bool CEngineServerImplementationBase::reserver_server_for_queued_game( const char *szReservationPayload )
{
	return false;
}

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
#include "eiface.h"
#include "GameStats.h"

#include "../eiface_engine_base.h"

//---------------------------------------------------------------------------------
// Global externs we need.
//---------------------------------------------------------------------------------
extern IVEngineServer * engine;

//---------------------------------------------------------------------------------
// Purpose: Source Engine 3 Specific engine implementation calls
//---------------------------------------------------------------------------------

// CS:GO SDK has a typedef CPlayerBitVec which could mean something different
// depending on what the ABSOLUTE_MAX_PLAYERS define is set to. Make sure our
// engine wrapper supports both CPlayerBitVec typedef as well as the old OB
// method of CBitVec< ABSOLUTE_MAX_PLAYERS >
class CPlayerBitVecWrapperImplementation : public CPlayerBitVec
{
};

class CEngineServerImplementation : public CEngineServerImplementationBase
{
public:
	virtual KeyValues *get_launch_options();
	virtual bool is_userid_in_use( int userID );
	virtual int get_loading_progress_for_userid( int userID );
	virtual bool is_log_enabled();
	virtual ISpatialPartition * create_spatial_partition(const CVector &worldmin,
		const CVector &worldmax);
	virtual float get_timescale() const;
	virtual bool is_level_main_menu_background();
	virtual bool is_any_client_low_violence();

	virtual bool is_split_screen_player(int ent_index);
	virtual CEdict* get_split_screen_player_attach_to_edict(int ent_num);
	virtual int get_num_split_screen_users_attached_to_edict(int ent_num);
	virtual CEdict* get_split_screen_player_for_edict(int ent_num, int slot);

	virtual bool is_override_load_game_ents_on();
	virtual void force_flush_entity(int ent_index);
	virtual ISPSharedMemory *get_single_player_shared_memory_space(const char *name, int ent_num);
	
	virtual void *alloc_level_static_data(size_t bytes);
	
	virtual bool is_creating_reslist();
	virtual bool is_creating_xbox_reslist();
	virtual bool is_dedicated_server_for_xbox();
	virtual bool is_dedicated_server_for_ps3();
	
	virtual void pause( bool bPause, bool bForce /*= false */ );
	virtual void set_timescale( float flTimescale );
	
	virtual void host_validate_session();
	
	virtual void refresh_screen_if_necessary();
	virtual bool has_paintmap();
	virtual bool sphere_paint_surface( const model_t *pModel, const CVector & vPosition, unsigned char color, float flSphereRadius, float flPaintCoatPercent );
	virtual void sphere_trace_paint_surface( const model_t *pModel, const CVector & vPosition, const CVector & vContactNormal, float flSphereRadius, CUtlVector<unsigned char> & surfColors );
	virtual void remove_all_paint();
	virtual void paint_all_surfaces( unsigned char color );
	virtual void remove_paint( const model_t *pModel );
	
	virtual uint64 get_client_xuid( CEdict* edict );
	virtual bool is_active_app();
	virtual void set_no_clip_enabled( bool bEnabled );
	virtual void get_paint_map_data_rle( CUtlVector<unsigned int> &mapdata );
	virtual void load_paint_map_data_rle( CUtlVector<unsigned int> &mapdata );
	virtual void send_paint_map_data_to_client( CEdict* edict );
	virtual float get_latency_for_choreo_sounds();
	virtual int get_client_cross_play_platform( int client_index );
	virtual void ensure_instance_baseline( int ent_num );
	virtual bool reserver_server_for_queued_game( const char *szReservationPayload );
	
};
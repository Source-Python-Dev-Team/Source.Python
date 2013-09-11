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
// Includes we need
//-----------------------------------------------------------------------------
#include "eiface.h"
#include "modules/entities/entities_wrap.h"
#include "modules/players/players_wrap.h"
#include "inetchannelinfo.h"

//-----------------------------------------------------------------------------
// Forward declares
//-----------------------------------------------------------------------------
abstract_class ISPSharedMemory;

//-----------------------------------------------------------------------------
// Purpose: Abstract class for all the specific engine implementation calls
//-----------------------------------------------------------------------------

abstract_class CEngineServerImplementationBase
{
	// Most likely most of these base functions will be stubs unless we can achieve
	// what they do with other engine calls
public:
	// Default functionality - return NULL
	virtual KeyValues *get_launch_options();

	// Default functionality - loop through all edicts to see if the userid of that
	// player matches the parameter passed in
	virtual bool is_userid_in_use( int userID );

	// Default functionality - return 0. What should be the max? 0 - 100?
	virtual int get_loading_progress_for_userid( int userID );

	// Default functionality - return false;
	virtual bool is_log_enabled();

	// Default functionality - return NULL
	virtual ISpatialPartition * create_spatial_partition(const CVector &worldmin, const CVector &worldmax);

	// Default functionality - return 1.0
	virtual float get_timescale() const;

	// Default functionality - return false
	virtual bool is_level_main_menu_background();

	// Default functionality - return false
	virtual bool is_any_client_low_violence();

	// Default functionality - return false
	virtual bool is_split_screen_player(int ent_index);

	// Default functionality - return NULL
	virtual CEdict* get_split_screen_player_attach_to_edict(int ent_num);

	// Default functionality - return 0
	virtual int get_num_split_screen_users_attached_to_edict(int ent_num);

	// Default functionality - return NULL
	virtual CEdict* get_split_screen_player_for_edict(int ent_num, int slot);

	// Default functionality - return false
	virtual bool is_override_load_game_ents_on();

	// Default functionality - does nothing.
	virtual void force_flush_entity(int ent_index);

	// Default functionality - return NULL
	virtual ISPSharedMemory *get_single_player_shared_memory_space(const char *name, int ent_num);

	// Default functionality - return NULL. I was consisdering returning a malloc() but that's
	// probably a bad idea, and this probably won't even be used anyway...
	void *alloc_level_static_data(size_t bytes);

	// Default functionality - return false
	virtual bool is_creating_reslist();

	// Default functionality - return false
	virtual bool is_creating_xbox_reslist();

	// Default functionality - return false
	virtual bool is_dedicated_server_for_xbox();

	// Default functionality - return false
	virtual bool is_dedicated_server_for_ps3();

	// Default functionality - do nothing
	virtual void pause( bool bPause, bool bForce /*= false */ );

	// Default functionality - do nothing
	virtual void set_timescale( float flTimescale );

	// Default functionality - do nothing
	virtual void host_validate_session();

	// Default functionality - do nothing
	virtual void refresh_screen_if_necessary();

	// Default functionality - return false
	virtual bool has_paintmap();

	// Default functionality - return false
	virtual bool sphere_paint_surface( const model_t *pModel, const CVector & vPosition, unsigned char color, float flSphereRadius, float flPaintCoatPercent );

	// Default functionality - do nothing
	virtual void sphere_trace_paint_surface( const model_t *pModel, const CVector & vPosition, const CVector & vContactNormal, float flSphereRadius, CUtlVector<unsigned char> & surfColors );

	// Default functionality - do nothing
	virtual void remove_all_paint();

	// Default functionality - do nothing
	virtual void paint_all_surfaces( unsigned char color );

	// Default functionality - do nothing
	virtual void remove_paint( const model_t *pModel );

	// Default functionality - return 0
	virtual uint64 get_client_xuid( CEdict* edict );

	// Default functionality - return true (assume app is always active)
	virtual bool is_active_app();

	// Default functionality - do nothing
	virtual void set_no_clip_enabled( bool bEnabled );

	// Default functionality - do nothing
	virtual void get_paint_map_data_rle( CUtlVector<unsigned int> &mapdata );

	// Default functionality - do nothing
	virtual void load_paint_map_data_rle( CUtlVector<unsigned int> &mapdata );

	// Default functionality - do nothing
	virtual void send_paint_map_data_to_client( CEdict* edict );

	// Default functionality - return 0.0f
	virtual float get_latency_for_choreo_sounds();

	// Default functionality - return 0
	virtual int get_client_cross_play_platform( int client_index );

	// Default functionality - do nothing
	virtual void ensure_instance_baseline( int ent_num );

	// Default functionality - return false
	virtual bool reserver_server_for_queued_game( const char *szReservationPayload );
};
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
#ifndef _EIFACE_WRAP_H
#define _EIFACE_WRAP_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "eiface.h"
#include "../../utility/wrap_macros.h"
#include "usermessage/usermessage.h"

// Include the engine specific include
#include ENGINE_INCLUDE_PATH(eiface_engine_implementation.h)

//---------------------------------------------------------------------------------
// Global externs we need.
//---------------------------------------------------------------------------------
extern IVEngineServer* engine;

//---------------------------------------------------------------------------------
// Purpose: Allow the engine to decide which class to use for player bit vec
//---------------------------------------------------------------------------------
class CPlayerBitVecWrapper : public CPlayerBitVecWrapperImplementation
{
};

//---------------------------------------------------------------------------------
// Purpose: Interface the engine exposes to the game DLL
//---------------------------------------------------------------------------------
class CEngineServer
{
public:
	virtual void change_level( const char *s1, const char *s2 );

	virtual int	is_map_valid( const char *filename );
	virtual bool is_dedicated_server( void );
	virtual int	is_in_edit_mode( void );

	virtual KeyValues* get_launch_options( void );

	virtual int	precache_model( const char *s, bool preload = false );
	virtual int	precache_sentence_file( const char *s, bool preload = false );
	virtual int	precache_decal( const char *name, bool preload = false );
	virtual int	precache_generic( const char *s, bool preload = false );
	virtual bool is_model_precached( char const *s ) const;
	virtual bool is_decal_precached( char const *s ) const;
	virtual bool is_generic_precached( char const *s ) const;

	virtual int	get_cluster_for_origin( const CVector &org );
	virtual int	get_pvs_for_cluster( int cluster, int outputpvslength, unsigned char *outputpvs );
	virtual bool check_origin_in_pvs( const CVector &org, const unsigned char *checkpvs, int checkpvssize );
	virtual bool check_box_in_pvs( const CVector &mins, const CVector &maxs, const unsigned char *checkpvs, int checkpvssize );

	virtual int	get_player_userid( CEdict* edict );
	virtual const char* get_player_network_id_string( CEdict* edict );
	virtual bool is_userid_in_use( int userID );
	virtual int get_loading_progress_for_userid( int userID );
	virtual int get_entity_count();
	virtual CNetChannelInfo* get_player_net_info( int playerIndex );

	virtual CEdict* create_edict( int iForceEdictIndex = -1 );
	virtual void remove_edict( CEdict* edict );

	// Memory allocation for entity class data
	virtual void* pv_alloc_ent_private_data( long cb );
	virtual void  free_ent_private_data( void *pEntity );
	virtual void *save_alloc_memory( size_t num, size_t size );
	virtual void save_free_memory( void *pSaveMem );

	virtual void emit_ambient_sound( int entindex, const CVector &pos, const char *samp, float vol, soundlevel_t soundlevel, int fFlags, int pitch, float delay = 0.0f );
	virtual void fade_client_volume( CEdict* edict, float fadePercent, float fadeOutSeconds, float holdTime, float fadeInSeconds );

	virtual int	sentence_group_pick( int groupIndex, char *name, int nameBufLen );
	virtual int	sentence_group_pick_sequential( int groupIndex, char *name, int nameBufLen, int sentenceIndex, int reset );
	virtual int	sentence_index_from_name( const char *pSentenceName );
	virtual const char *sentence_name_from_index( int sentenceIndex );
	virtual int	sentence_group_index_from_name( const char *pGroupName );
	virtual const char *sentence_group_name_from_index( int groupIndex );
	virtual float sentence_length( int sentenceIndex );

	virtual void server_command( const char *str );
	virtual void server_execute( void );

	/**
	 * TODO: Look into figuring out variadic passing?
	 */
	virtual void client_command( CEdict* edict, const char *szCommand );

	virtual void light_style( int style, const char *val );

	virtual void static_decal( const CVector &originInEntitySpace, int decalIndex, int entityIndex, int modelIndex, bool lowpriority );

	virtual void message_determine_multicast_recipients( bool usepas, const CVector& origin, CPlayerBitVecWrapper& playerbits );
	virtual bf_write* entity_message_begin( int ent_index, ServerClass * ent_class, bool reliable );

	virtual void send_user_message(CUserMessage &msg);
	//virtual void message_end( void );
	//virtual void send_user_message( IRecipientFilter &filter, int message, const google::protobuf::Message &msg );
	
	virtual void client_printf( CEdict* edict, const char *szMsg );
	virtual void con_nprintf( int pos, const char* szString );
	virtual void con_nxprintf( const struct con_nprint_s *info, const char* szString );

	virtual void set_view( CEdict* pClient, CEdict* pViewent );
	virtual void crosshair_angle( CEdict* edict, float pitch, float yaw );

	/**
	 * Python managed return type!
	 */
	virtual const char* get_game_dir( int maxlength );
	virtual int compare_file_time( const char *filename1, const char *filename2, int *iCompare );

	virtual bool lock_network_string_tables( bool lock );

	virtual CEdict* create_fake_client( const char *netname );

	virtual const char* get_client_convar_value( int clientIndex, const char *name );

	virtual const char* parse_file( const char *data, char *token, int maxlen );
	virtual bool copy_file( const char *source, const char *destination );

	virtual void reset_pvs( byte *pvs, int pvssize );
	virtual void add_origin_to_pvs( const CVector &origin );

	virtual void set_area_portal_state( int portalNumber, int isOpen );

	virtual void playback_temp_entity( CMRecipientFilter& filter, float delay, const void *pSender, const SendTable *pST, int classID  );
	
	virtual int check_head_node_visible( int nodenum, const byte *pvs, int vissize );
	virtual int	check_areas_connected( int area1, int area2 );
	virtual int	get_area( const CVector &origin );
	virtual void get_area_bits( int area, unsigned char *bits, int buflen );
	virtual bool get_area_portal_plane( CVector const &vViewOrigin, int portalKey, VPlane *pPlane );

	virtual bool load_game_state( char const *pMapName, bool createPlayers );
	virtual void load_adjacent_ents( const char *pOldLevel, const char *pLandmarkName );

	virtual void clear_save_dir();

	virtual const char*	get_map_entities_string();

	virtual client_textmessage_t *text_message_get( const char *pName );
	virtual void log_print( const char *msg );
	virtual bool is_log_enabled();

	virtual void build_entity_cluster_list( CEdict* edict, PVSInfo_t *pPVSInfo );

	virtual void solid_moved( CEdict* edict, ICollideable *pSolidCollide, const CVector* pPrevAbsOrigin, bool testSurroundingBoundsOnly );
	virtual void trigger_moved( CEdict* edict, bool testSurroundingBoundsOnly );

	virtual ISpatialPartition *create_spatial_partition( const CVector& worldmin, const CVector& worldmax );
	virtual void destroy_spatial_partition( ISpatialPartition *partition );

	virtual void draw_map_to_scratch_pad( IScratchPad3D *pPad, unsigned long iFlags );

	virtual const CBitVec<MAX_EDICTS>* get_entity_transmit_bits_for_client( int iClientIndex );

	virtual bool  is_paused();

	virtual float get_timescale() const;

	virtual void  force_exact_file( const char *s );

	virtual void  force_model_bounds( const char *s, const CVector &mins, const CVector &maxs );

	virtual void  clear_save_dir_after_client_load();

	virtual void  set_fake_client_convar_value( CEdict* edict, const char *cvar, const char *value );

	virtual void  force_simple_material( const char *s );

	virtual int   is_in_commentary_mode();
	virtual bool  is_level_main_menu_background();

	virtual void  set_area_portal_states( const int *portalNumbers, const int *isOpen, int nPortals );

	virtual void  notify_edict_flags_change( int iEdict );

	virtual const CCheckTransmitInfo* get_prev_check_transmit_info( CEdict* edict );
	virtual CSharedEdictChangeInfo* get_shared_edict_change_info();

	virtual void   allow_immediate_edict_reuse();

	virtual bool  is_internal_build();

	virtual IChangeInfoAccessor *get_change_accessor( CEdict* edict );
	virtual char const *get_most_recently_loaded_file_name();
	virtual char const *get_save_file_name();

	virtual void clean_up_entity_cluster_list( PVSInfo_t *pPVSInfo );

	virtual void set_achievement_mgr( IAchievementMgr* mgr );
	virtual IAchievementMgr* get_achievement_mgr();

	virtual int     get_app_id();

	virtual bool is_low_violence();
	virtual bool is_any_client_low_violence();

	virtual QueryCvarCookie_t start_query_cvar_value( CEdict* edict, const char *pName );

	virtual void insert_server_command( const char *str );

	virtual bool get_player_info( int ent_num, player_info_t *pinfo );

	virtual bool is_client_fully_authenticated( CEdict* edict );

	virtual void set_dedicated_server_benchmark_mode( bool bBenchmarkMode );

	virtual bool is_split_screen_player( int ent_num );
	virtual CEdict* get_split_screen_player_attach_to_edict( int ent_num );
	virtual int     get_num_split_screen_users_attached_to_edict( int ent_num );
	virtual CEdict* get_split_screen_player_for_edict( int ent_num, int nSlot );

	virtual bool is_override_load_game_ents_on();

	virtual void force_flush_entity( int iEntity );

	virtual ISPSharedMemory *get_single_player_shared_memory_space( const char *szName, int ent_num = MAX_EDICTS );

	virtual void *alloc_level_static_data( size_t bytes );

	virtual int get_cluster_count();
	virtual int get_all_cluster_bounds( bbox_t *pBBoxList, int maxBBox );

	virtual bool is_creating_reslist();
	virtual bool is_creating_xbox_reslist();
	virtual bool is_dedicated_server_for_xbox();
	virtual bool is_dedicated_server_for_ps3();

	virtual void pause( bool bPause, bool bForce = false );

	virtual void set_timescale( float flTimescale );

	virtual void set_gamestats_data( CGamestatsData *pGamestatsData );
	virtual CGamestatsData *get_gamestats_data();

	virtual const CSteamID  *get_client_steamid( CEdict* edict );
	virtual const CSteamID  *get_game_server_steamid();

	virtual void host_validate_session();

	virtual void refresh_screen_if_necessary();

	virtual bool has_paintmap();
	virtual bool sphere_paint_surface( const model_t *pModel, const CVector & vPosition, unsigned char color, float flSphereRadius, float flPaintCoatPercent);
	virtual void sphere_trace_paint_surface( const model_t *pModel, const CVector & vPosition, const CVector & vContactNormal, float flSphereRadius, CUtlVector<unsigned char> & surfColors );
	virtual void remove_all_paint();
	virtual void paint_all_surfaces( unsigned char color);
	virtual void remove_paint( const model_t *pModel );

	virtual void client_command_key_values( CEdict* edict, KeyValues *pCommand );

	virtual uint64 get_client_xuid( CEdict* edict );

	virtual bool is_active_app();

	virtual void set_no_clip_enabled( bool bEnabled );

	virtual void get_paint_map_data_rle( CUtlVector<unsigned int> &mapdata );
	virtual void load_paint_map_data_rle( CUtlVector<unsigned int> &mapdata );
	virtual void send_paint_map_data_to_client( CEdict* edict );

	virtual float get_latency_for_choreo_sounds();

	virtual int get_client_cross_play_platform( int client_index );

	virtual void ensure_instance_baseline( int ent_num);

	virtual bool reserver_server_for_queued_game( const char *szReservationPayload );

protected:
	const CEngineServerImplementation &get_engine_implementation() const;
	CEngineServerImplementation &get_engine_implementation_non_const();

private:
	// Engine specific implementation calls
	CEngineServerImplementation m_engine_server_implementation;
};

#endif
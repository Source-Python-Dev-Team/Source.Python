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
// Includes
//-----------------------------------------------------------------------------
#include "eiface_wrap.h"

//-----------------------------------------------------------------------------
// CEngineServer Methods
//-----------------------------------------------------------------------------
const CEngineServerImplementation & CEngineServer::get_engine_implementation() const
{
	return m_engine_server_implementation;
}

CEngineServerImplementation & CEngineServer::get_engine_implementation_non_const()
{
	return m_engine_server_implementation;
}

void CEngineServer::change_level( const char *s1, const char *s2 )
{
	engine->ChangeLevel(s1, s2);
}

int CEngineServer::is_map_valid( const char *filename )
{
	return engine->IsMapValid( filename );
}

bool CEngineServer::is_dedicated_server( void )
{
	return engine->IsDedicatedServer();
}

int CEngineServer::is_in_edit_mode( void )
{
	return engine->IsInEditMode();
}

KeyValues* CEngineServer::get_launch_options( void )
{
	return m_engine_server_implementation.get_launch_options();
}

int CEngineServer::precache_model( const char *s, bool preload /*= false */ )
{
	return engine->PrecacheModel(s, preload);
}

int CEngineServer::precache_sentence_file( const char *s, bool preload /*= false */ )
{
	return engine->PrecacheSentenceFile(s, preload);
}

int CEngineServer::precache_decal( const char *name, bool preload /*= false */ )
{
	return engine->PrecacheDecal(name, preload);
}

int CEngineServer::precache_generic( const char *s, bool preload /*= false */ )
{
	return engine->PrecacheGeneric(s, preload);
}

bool CEngineServer::is_model_precached( char const *s ) const
{
	return engine->IsModelPrecached(s);
}

bool CEngineServer::is_generic_precached( char const *s ) const
{
	return engine->IsGenericPrecached(s);
}

int CEngineServer::get_cluster_for_origin( const CVector &org )
{
	return engine->GetClusterForOrigin(org);
}

int CEngineServer::get_pvs_for_cluster( int cluster, int outputpvslength, unsigned char *outputpvs )
{
	return engine->GetPVSForCluster(cluster, outputpvslength, outputpvs);
}

bool CEngineServer::check_origin_in_pvs( const CVector &org, const unsigned char *checkpvs, int checkpvssize )
{
	return engine->CheckOriginInPVS(org, checkpvs, checkpvssize);
}

bool CEngineServer::check_box_in_pvs( const CVector &mins, const CVector &maxs, const unsigned char *checkpvs, int checkpvssize )
{
	return engine->CheckBoxInPVS(mins, maxs, checkpvs, checkpvssize);
}

int CEngineServer::get_player_userid( CEdict* edict )
{
	return engine->GetPlayerUserId(edict->get_edict());
}

const char* CEngineServer::get_player_network_id_string( CEdict* edict )
{
	return engine->GetPlayerNetworkIDString(edict->get_edict());
}

bool CEngineServer::is_userid_in_use( int userID )
{
	return m_engine_server_implementation.is_userid_in_use(userID);
}

int CEngineServer::get_loading_progress_for_userid( int userID )
{
	return m_engine_server_implementation.get_loading_progress_for_userid(userID);
}

int CEngineServer::get_entity_count( void )
{
	return engine->GetEntityCount();
}

CNetChannelInfo* CEngineServer::get_player_net_info( int playerIndex )
{
	return new CNetChannelInfo(engine->GetPlayerNetInfo(playerIndex));
}

bool CEngineServer::is_decal_precached( char const *s ) const
{
	return engine->IsDecalPrecached(s);
}

CEdict* CEngineServer::create_edict( int iForceEdictIndex /*= -1 */ )
{
	return new CEdict(engine->CreateEdict(iForceEdictIndex));
}

void CEngineServer::remove_edict( CEdict* edict )
{
	engine->RemoveEdict(edict->get_edict());
}

void* CEngineServer::pv_alloc_ent_private_data( long cb )
{
	return engine->PvAllocEntPrivateData(cb);
}

void CEngineServer::free_ent_private_data( void *pEntity )
{
	engine->FreeEntPrivateData(pEntity);
}

void * CEngineServer::save_alloc_memory( size_t num, size_t size )
{
	return engine->SaveAllocMemory(num, size);
}

void CEngineServer::save_free_memory( void *pSaveMem )
{
	engine->SaveFreeMemory(pSaveMem);
}

void CEngineServer::emit_ambient_sound( int entindex, const CVector &pos, const char *samp, float vol, soundlevel_t soundlevel, int fFlags, int pitch, float delay /*= 0.0f */ )
{
	engine->EmitAmbientSound(entindex, pos, samp, vol, soundlevel, fFlags, pitch, delay);
}

void CEngineServer::fade_client_volume( CEdict* edict, float fadePercent, float fadeOutSeconds, float holdTime, float fadeInSeconds )
{
	engine->FadeClientVolume(edict->get_edict(), fadePercent, fadeOutSeconds, holdTime, fadeInSeconds);
}

int CEngineServer::sentence_group_pick( int groupIndex, char *name, int nameBufLen )
{
	return engine->SentenceGroupPick(groupIndex, name, nameBufLen);
}

int CEngineServer::sentence_group_pick_sequential( int groupIndex, char *name, int nameBufLen, int sentenceIndex, int reset )
{
	return engine->SentenceGroupPickSequential(groupIndex, name, nameBufLen, sentenceIndex, reset);
}

int CEngineServer::sentence_index_from_name( const char *pSentenceName )
{
	return engine->SentenceIndexFromName(pSentenceName);
}

const char * CEngineServer::sentence_name_from_index( int sentenceIndex )
{
	return engine->SentenceNameFromIndex(sentenceIndex);
}

int CEngineServer::sentence_group_index_from_name( const char *pGroupName )
{
	return engine->SentenceGroupIndexFromName(pGroupName);
}

const char * CEngineServer::sentence_group_name_from_index( int groupIndex )
{
	return engine->SentenceGroupNameFromIndex(groupIndex);
}

float CEngineServer::sentence_length( int sentenceIndex )
{
	return engine->SentenceLength(sentenceIndex);
}

void CEngineServer::server_command( const char *str )
{
	engine->ServerCommand(str);
}

void CEngineServer::server_execute( void )
{
	engine->ServerExecute();
}

void CEngineServer::client_command( CEdict* edict, const char *szCommand )
{
	engine->ClientCommand(edict->get_edict(), szCommand);
}

void CEngineServer::light_style( int style, const char *val )
{
	engine->LightStyle(style, val);
}

void CEngineServer::static_decal( const CVector &originInEntitySpace, int decalIndex, int entityIndex, int modelIndex, bool lowpriority )
{
	engine->StaticDecal(originInEntitySpace, decalIndex, entityIndex, modelIndex, lowpriority);
}

void CEngineServer::message_determine_multicast_recipients( bool usepas, const CVector& origin, CPlayerBitVecWrapper& playerbits )
{
	engine->Message_DetermineMulticastRecipients(usepas, origin, playerbits);
}

bf_write* CEngineServer::entity_message_begin( int ent_index, ServerClass * ent_class, bool reliable )
{
	return engine->EntityMessageBegin(ent_index, ent_class, reliable);
}

void CEngineServer::send_user_message( CUserMessage &msg )
{
	msg.send_message();
}

void CEngineServer::client_printf( CEdict* edict, const char *szMsg )
{
	engine->ClientPrintf(edict->get_edict(), szMsg);
}

void CEngineServer::con_nprintf( int pos, const char* szString )
{
	engine->Con_NPrintf(pos, szString);
}

void CEngineServer::con_nxprintf( const struct con_nprint_s *info, const char* szString )
{
	engine->Con_NXPrintf(info, szString);
}

void CEngineServer::set_view( CEdict* pClient, CEdict* pViewent )
{
	engine->SetView(pClient->get_edict(), pViewent->get_edict());
}

void CEngineServer::crosshair_angle( CEdict* edict, float pitch, float yaw )
{
	engine->CrosshairAngle(edict->get_edict(), pitch, yaw);
}

const char* CEngineServer::get_game_dir( int maxlength )
{
	char* szGetGameDir = new char[maxlength];
	engine->GetGameDir(szGetGameDir, maxlength);
	return szGetGameDir;
}

int CEngineServer::compare_file_time( const char *filename1, const char *filename2, int *iCompare )
{
	return engine->CompareFileTime(filename1, filename2, iCompare);
}

bool CEngineServer::lock_network_string_tables( bool lock )
{
	return engine->LockNetworkStringTables(lock);
}

CEdict* CEngineServer::create_fake_client( const char *netname )
{
	return new CEdict(engine->CreateFakeClient(netname));
}

const char* CEngineServer::get_client_convar_value( int clientIndex, const char *name )
{
	return engine->GetClientConVarValue(clientIndex, name);
}

const char* CEngineServer::parse_file( const char *data, char *token, int maxlen )
{
	return engine->ParseFile(data, token, maxlen);
}

bool CEngineServer::copy_file( const char *source, const char *destination )
{
	return engine->CopyFile(source, destination);
}

void CEngineServer::reset_pvs( byte *pvs, int pvssize )
{
	engine->ResetPVS(pvs, pvssize);
}

void CEngineServer::add_origin_to_pvs( const CVector &origin )
{
	engine->AddOriginToPVS(origin);
}

void CEngineServer::set_area_portal_state( int portalNumber, int isOpen )
{
	engine->SetAreaPortalState(portalNumber, isOpen);
}

void CEngineServer::playback_temp_entity( CMRecipientFilter& filter, float delay, const void *pSender, const SendTable *pST, int classID )
{
	engine->PlaybackTempEntity(filter, delay, pSender, pST, classID);
}

int CEngineServer::check_head_node_visible( int nodenum, const byte *pvs, int vissize )
{
	return engine->CheckHeadnodeVisible(nodenum, pvs, vissize);
}

int CEngineServer::check_areas_connected( int area1, int area2 )
{
	return engine->CheckAreasConnected(area1, area2);
}

int CEngineServer::get_area( const CVector &origin )
{
	return engine->GetArea(origin);
}

void CEngineServer::get_area_bits( int area, unsigned char *bits, int buflen )
{
	engine->GetAreaBits(area, bits, buflen);
}

bool CEngineServer::get_area_portal_plane( CVector const &vViewOrigin, int portalKey, VPlane *pPlane )
{
	return engine->GetAreaPortalPlane(vViewOrigin, portalKey, pPlane);
}

bool CEngineServer::load_game_state( char const *pMapName, bool createPlayers )
{
	return engine->LoadGameState(pMapName, createPlayers);
}

void CEngineServer::load_adjacent_ents( const char *pOldLevel, const char *pLandmarkName )
{
	engine->LoadAdjacentEnts(pOldLevel, pLandmarkName);
}

void CEngineServer::clear_save_dir()
{
	engine->ClearSaveDir();
}

const char* CEngineServer::get_map_entities_string()
{
	return engine->GetMapEntitiesString();
}

client_textmessage_t * CEngineServer::text_message_get( const char *pName )
{
	return engine->TextMessageGet(pName);
}

void CEngineServer::log_print( const char *msg )
{
	engine->LogPrint(msg);
}

bool CEngineServer::is_log_enabled()
{
	return m_engine_server_implementation.is_log_enabled();
}

void CEngineServer::build_entity_cluster_list( CEdict* edict, PVSInfo_t *pPVSInfo )
{
	engine->BuildEntityClusterList(edict->get_edict(), pPVSInfo);
}

void CEngineServer::solid_moved( CEdict* edict, ICollideable *pSolidCollide, const CVector* pPrevAbsOrigin, bool testSurroundingBoundsOnly )
{
	engine->SolidMoved(edict->get_edict(), pSolidCollide, (Vector *) pPrevAbsOrigin, testSurroundingBoundsOnly);
}

void CEngineServer::trigger_moved( CEdict* edict, bool testSurroundingBoundsOnly )
{
	engine->TriggerMoved(edict->get_edict(), testSurroundingBoundsOnly);
}

ISpatialPartition * CEngineServer::create_spatial_partition( const CVector& worldmin, const CVector& worldmax )
{
	return m_engine_server_implementation.create_spatial_partition(worldmin, worldmax);
}

void CEngineServer::destroy_spatial_partition( ISpatialPartition *partition )
{
	engine->DestroySpatialPartition( partition );
}

void CEngineServer::draw_map_to_scratch_pad( IScratchPad3D *pPad, unsigned long iFlags )
{
	engine->DrawMapToScratchPad(pPad, iFlags);
}

const CBitVec<MAX_EDICTS>* CEngineServer::get_entity_transmit_bits_for_client( int iClientIndex )
{
	return engine->GetEntityTransmitBitsForClient(iClientIndex);
}

bool CEngineServer::is_paused()
{
	return engine->IsPaused();
}

float CEngineServer::get_timescale() const
{
	return m_engine_server_implementation.get_timescale();
}

void CEngineServer::force_exact_file( const char *s )
{
	engine->ForceExactFile(s);
}

void CEngineServer::force_model_bounds( const char *s, const CVector &mins, const CVector &maxs )
{
	engine->ForceModelBounds(s, mins, maxs);
}

void CEngineServer::clear_save_dir_after_client_load()
{
	engine->ClearSaveDirAfterClientLoad();
}

void CEngineServer::set_fake_client_convar_value( CEdict* edict, const char *cvar, const char *value )
{
	engine->SetFakeClientConVarValue(edict->get_edict(), cvar, value);
}

void CEngineServer::force_simple_material( const char *s )
{
	engine->ForceSimpleMaterial(s);
}

int CEngineServer::is_in_commentary_mode()
{
	return engine->IsInCommentaryMode();
}

bool CEngineServer::is_level_main_menu_background()
{
	return m_engine_server_implementation.is_level_main_menu_background();
}

void CEngineServer::set_area_portal_states( const int *portalNumbers, const int *isOpen, int nPortals )
{
	engine->SetAreaPortalStates(portalNumbers, isOpen, nPortals);
}

void CEngineServer::notify_edict_flags_change( int iEdict )
{
	engine->NotifyEdictFlagsChange(iEdict);
}

const CCheckTransmitInfo* CEngineServer::get_prev_check_transmit_info( CEdict* edict )
{
	return engine->GetPrevCheckTransmitInfo(edict->get_edict());
}

CSharedEdictChangeInfo* CEngineServer::get_shared_edict_change_info()
{
	return engine->GetSharedEdictChangeInfo();
}

void CEngineServer::allow_immediate_edict_reuse()
{
	engine->AllowImmediateEdictReuse();
}

bool CEngineServer::is_internal_build()
{
	return engine->IsInternalBuild();
}

IChangeInfoAccessor * CEngineServer::get_change_accessor( CEdict* edict )
{
	return engine->GetChangeAccessor(edict->get_edict());
}

char const * CEngineServer::get_most_recently_loaded_file_name()
{
	return engine->GetMostRecentlyLoadedFileName();
}

char const * CEngineServer::get_save_file_name()
{
	return engine->GetSaveFileName();
}

void CEngineServer::clean_up_entity_cluster_list( PVSInfo_t *pPVSInfo )
{
	engine->CleanUpEntityClusterList(pPVSInfo);
}

void CEngineServer::set_achievement_mgr( IAchievementMgr* mgr )
{
	engine->SetAchievementMgr(mgr);
}

IAchievementMgr* CEngineServer::get_achievement_mgr()
{
	return engine->GetAchievementMgr();
}

int CEngineServer::get_app_id()
{
	return engine->GetAppID();
}

bool CEngineServer::is_low_violence()
{
	return engine->IsLowViolence();
}

bool CEngineServer::is_any_client_low_violence()
{
	return m_engine_server_implementation.is_any_client_low_violence();
}

QueryCvarCookie_t CEngineServer::start_query_cvar_value( CEdict* edict, const char *pName )
{
	return engine->StartQueryCvarValue(edict->get_edict(), pName);
}

void CEngineServer::insert_server_command( const char *str )
{
	engine->InsertServerCommand(str);
}

bool CEngineServer::get_player_info( int ent_num, player_info_t *pinfo )
{
	return engine->GetPlayerInfo(ent_num, pinfo);
}

bool CEngineServer::is_client_fully_authenticated( CEdict* edict )
{
	return engine->IsClientFullyAuthenticated(edict->get_edict());
}

void CEngineServer::set_dedicated_server_benchmark_mode( bool bBenchmarkMode )
{
	engine->SetDedicatedServerBenchmarkMode(bBenchmarkMode);
}

bool CEngineServer::is_split_screen_player( int ent_num )
{
	return m_engine_server_implementation.is_split_screen_player(ent_num);
}

CEdict* CEngineServer::get_split_screen_player_attach_to_edict( int ent_num )
{
	return m_engine_server_implementation.get_split_screen_player_attach_to_edict(ent_num);
}

int CEngineServer::get_num_split_screen_users_attached_to_edict( int ent_num )
{
	return m_engine_server_implementation.get_num_split_screen_users_attached_to_edict(ent_num);
}

CEdict* CEngineServer::get_split_screen_player_for_edict( int ent_num, int nSlot )
{
	return m_engine_server_implementation.get_split_screen_player_for_edict(ent_num, nSlot);
}

bool CEngineServer::is_override_load_game_ents_on()
{
	return m_engine_server_implementation.is_override_load_game_ents_on();
}

void CEngineServer::force_flush_entity( int entity )
{
	m_engine_server_implementation.force_flush_entity(entity);
}

ISPSharedMemory * CEngineServer::get_single_player_shared_memory_space( const char *szName, int ent_num /*= MAX_EDICTS */ )
{
	return m_engine_server_implementation.get_single_player_shared_memory_space(szName, ent_num);
}

void * CEngineServer::alloc_level_static_data( size_t bytes )
{
	return m_engine_server_implementation.alloc_level_static_data(bytes);
}

int CEngineServer::get_cluster_count()
{
	return engine->GetClusterCount();
}

int CEngineServer::get_all_cluster_bounds( bbox_t *pBBoxList, int maxBBox )
{
	return engine->GetAllClusterBounds(pBBoxList, maxBBox);
}

bool CEngineServer::is_creating_reslist()
{
	return m_engine_server_implementation.is_creating_reslist();
}

bool CEngineServer::is_creating_xbox_reslist()
{
	return m_engine_server_implementation.is_creating_xbox_reslist();
}

bool CEngineServer::is_dedicated_server_for_xbox()
{
	return m_engine_server_implementation.is_dedicated_server_for_xbox();
}

bool CEngineServer::is_dedicated_server_for_ps3()
{
	return m_engine_server_implementation.is_dedicated_server_for_ps3();
}

void CEngineServer::pause( bool bPause, bool bForce /*= false */ )
{
	m_engine_server_implementation.pause(bPause, bForce);
}

void CEngineServer::set_timescale( float flTimescale )
{
	m_engine_server_implementation.set_timescale(flTimescale);
}

void CEngineServer::set_gamestats_data( CGamestatsData *pGamestatsData )
{
	engine->SetGamestatsData(pGamestatsData);
}

CGamestatsData * CEngineServer::get_gamestats_data()
{
	return engine->GetGamestatsData();
}

const CSteamID  * CEngineServer::get_client_steamid( CEdict* edict )
{
	return engine->GetClientSteamID(edict->get_edict());
}

const CSteamID  * CEngineServer::get_game_server_steamid()
{
	return engine->GetGameServerSteamID();
}

void CEngineServer::host_validate_session()
{
	m_engine_server_implementation.host_validate_session();
}

void CEngineServer::refresh_screen_if_necessary()
{
	m_engine_server_implementation.refresh_screen_if_necessary();
}

bool CEngineServer::has_paintmap()
{
	return m_engine_server_implementation.has_paintmap();
}

bool CEngineServer::sphere_paint_surface( const model_t *pModel, const CVector & vPosition, unsigned char color, float flSphereRadius, float flPaintCoatPercent )
{
	return m_engine_server_implementation.sphere_paint_surface(pModel, vPosition, color, flSphereRadius, flPaintCoatPercent);
}

void CEngineServer::sphere_trace_paint_surface( const model_t *pModel, const CVector & vPosition, const CVector & vContactNormal, float flSphereRadius, CUtlVector<unsigned char> & surfColors )
{
	m_engine_server_implementation.sphere_trace_paint_surface(pModel, vPosition, vContactNormal, flSphereRadius, surfColors);
}

void CEngineServer::remove_all_paint()
{
	m_engine_server_implementation.remove_all_paint();
}

void CEngineServer::paint_all_surfaces( unsigned char color )
{
	m_engine_server_implementation.paint_all_surfaces(color);
}

void CEngineServer::remove_paint( const model_t *pModel )
{
	m_engine_server_implementation.remove_paint(pModel);
}

void CEngineServer::client_command_key_values( CEdict* edict, KeyValues *pCommand )
{
	engine->ClientCommandKeyValues(edict->get_edict(), pCommand);
}

uint64 CEngineServer::get_client_xuid( CEdict* edict )
{
	return m_engine_server_implementation.get_client_xuid(edict);
}

bool CEngineServer::is_active_app()
{
	return m_engine_server_implementation.is_active_app();
}

void CEngineServer::set_no_clip_enabled( bool bEnabled )
{
	m_engine_server_implementation.set_no_clip_enabled(bEnabled);
}

void CEngineServer::get_paint_map_data_rle( CUtlVector<unsigned int> &mapdata )
{
	m_engine_server_implementation.get_paint_map_data_rle(mapdata);
}

void CEngineServer::load_paint_map_data_rle( CUtlVector<unsigned int> &mapdata )
{
	m_engine_server_implementation.load_paint_map_data_rle(mapdata);
}

void CEngineServer::send_paint_map_data_to_client( CEdict* edict )
{
	m_engine_server_implementation.send_paint_map_data_to_client(edict);
}

float CEngineServer::get_latency_for_choreo_sounds()
{
	return m_engine_server_implementation.get_latency_for_choreo_sounds();
}

int CEngineServer::get_client_cross_play_platform( int client_index )
{
	return m_engine_server_implementation.get_client_cross_play_platform(client_index);
}

void CEngineServer::ensure_instance_baseline( int ent_num )
{
	m_engine_server_implementation.ensure_instance_baseline(ent_num);
}

bool CEngineServer::reserver_server_for_queued_game( const char *szReservationPayload )
{
	return m_engine_server_implementation.reserver_server_for_queued_game(szReservationPayload);
}

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "listeners.h"

//-----------------------------------------------------------------------------
// ClientActiveListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_ClientActiveListenerManager;

CListenerManager* get_client_active_listener_manager()
{
	return &s_ClientActiveListenerManager;
}

//-----------------------------------------------------------------------------
// ClientConnectListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_ClientConnectListenerManager;

CListenerManager* get_client_connect_listener_manager()
{
	return &s_ClientConnectListenerManager;
}

//-----------------------------------------------------------------------------
// ClientDisconnectListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_ClientDisconnectListenerManager;

CListenerManager* get_client_disconnect_listener_manager()
{
	return &s_ClientDisconnectListenerManager;
}

//-----------------------------------------------------------------------------
// ClientFullyConnectListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_ClientFullyConnectListenerManager;

CListenerManager* get_client_fully_connect_listener_manager()
{
	return &s_ClientFullyConnectListenerManager;
}

//-----------------------------------------------------------------------------
// ClientPutInServerListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_ClientPutInServerListenerManager;

CListenerManager* get_client_put_in_server_listener_manager()
{
	return &s_ClientPutInServerListenerManager;
}

//-----------------------------------------------------------------------------
// ClientSettingsChangedListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_ClientSettingsChangedListenerManager;

CListenerManager* get_client_settings_changed_listener_manager()
{
	return &s_ClientSettingsChangedListenerManager;
}

//-----------------------------------------------------------------------------
// LevelInitListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_LevelInitListenerManager;

CListenerManager* get_level_init_listener_manager()
{
	return &s_LevelInitListenerManager;
}

//-----------------------------------------------------------------------------
// LevelShutdownListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_LevelShutdownListenerManager;

CListenerManager* get_level_shutdown_listener_manager()
{
	return &s_LevelShutdownListenerManager;
}

//-----------------------------------------------------------------------------
// NetworkidValidatedListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_NetworkidValidatedListenerManager;

CListenerManager* get_networkid_validated_listener_manager()
{
	return &s_NetworkidValidatedListenerManager;
}

//-----------------------------------------------------------------------------
// OnEdictAllocatedListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_OnEdictAllocatedListenerManager;

CListenerManager* get_on_edict_allocated_listener_manager()
{
	return &s_OnEdictAllocatedListenerManager;
}

//-----------------------------------------------------------------------------
// OnEdictFreedListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_OnEdictFreedListenerManager;

CListenerManager* get_on_edict_freed_listener_manager()
{
	return &s_OnEdictFreedListenerManager;
}

//-----------------------------------------------------------------------------
// OnQueryCvarValueFinishedListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_OnQueryCvarValueFinishedListenerManager;

CListenerManager* get_on_query_cvar_value_finished_listener_manager()
{
	return &s_OnQueryCvarValueFinishedListenerManager;
}

//-----------------------------------------------------------------------------
// ServerActivateListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_ServerActivateListenerManager;

CListenerManager* get_server_activate_listener_manager()
{
	return &s_ServerActivateListenerManager;
}

//-----------------------------------------------------------------------------
// TickListenerManager instance and accessor
//-----------------------------------------------------------------------------
static CListenerManager s_TickListenerManager;

CListenerManager* get_tick_listener_manager()
{
	return &s_TickListenerManager;
}
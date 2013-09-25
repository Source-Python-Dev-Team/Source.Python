//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "listenermanager.h"

// 
// Listener Manager accessors
//
CListenerManager* get_client_active_listener_manager();
CListenerManager* get_client_connect_listener_manager();
CListenerManager* get_client_disconnect_listener_manager();
CListenerManager* get_client_fully_connect_listener_manager();
CListenerManager* get_client_put_in_server_listener_manager();
CListenerManager* get_client_settings_changed_listener_manager();
CListenerManager* get_level_init_listener_manager();
CListenerManager* get_level_shutdown_listener_manager();
CListenerManager* get_networkid_validated_listener_manager();
CListenerManager* get_on_edict_allocated_listener_manager();
CListenerManager* get_on_edict_freed_listener_manager();
CListenerManager* get_on_query_cvar_value_finished_listener_manager();
CListenerManager* get_server_activate_listener_manager();
CListenerManager* get_tick_listener_manager();
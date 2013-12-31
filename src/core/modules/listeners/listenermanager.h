#ifndef _LISTENERMANAGER_H
#define _LISTENERMANAGER_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "utlvector.h"
#include "utility/wrap_macros.h"
#include "utility/call_python.h"


//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
// This creates a static manager (only visible in file that uses this macro)
// and an inline function that returns a pointer to the manager. That way we
// avoid multiple definitions
#define DEFINE_MANAGER_ACCESSOR(name) \
	static CListenerManager s_##name; \
	inline CListenerManager* name() \
	{ return &s_##name; }

// Calls all listeners of the given manager (must be a pointer)
#define CALL_LISTENERS(manager, ...) \
	for(int i = 0; i < manager->m_vecCallables.Count(); i++) \
	{ \
		BEGIN_BOOST_PY() \
			CALL_PY_FUNC(manager->m_vecCallables[i].ptr(), ##__VA_ARGS__); \
	    END_BOOST_PY_NORET() \
	} 


//-----------------------------------------------------------------------------
// CListenerManager class
//-----------------------------------------------------------------------------
class CListenerManager
{
public:
	void register_listener(PyObject* pCallable);
	void unregister_listener(PyObject* pCallable);

public:
	CUtlVector<object> m_vecCallables;
};

// Create manager accessor functions
DEFINE_MANAGER_ACCESSOR(get_client_active_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_client_connect_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_client_disconnect_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_client_fully_connect_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_client_put_in_server_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_client_settings_changed_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_level_init_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_level_shutdown_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_networkid_validated_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_on_edict_allocated_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_on_edict_freed_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_on_query_cvar_value_finished_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_server_activate_listener_manager)
DEFINE_MANAGER_ACCESSOR(get_tick_listener_manager)

#endif // _LISTENERMANAGER_H
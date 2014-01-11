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
	inline CListenerManager* Get##name##ListenerManager() \
	{ return &s_##name; }

// Calls all listeners of the given manager
#define CALL_LISTENERS(manager, ...) \
	for(int i = 0; i < s_##manager.m_vecCallables.Count(); i++) \
	{ \
		BEGIN_BOOST_PY() \
		CALL_PY_FUNC(s_##manager.m_vecCallables[i].ptr(), ##__VA_ARGS__); \
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
DEFINE_MANAGER_ACCESSOR(ClientActive)
DEFINE_MANAGER_ACCESSOR(ClientConnect)
DEFINE_MANAGER_ACCESSOR(ClientDisconnect)
DEFINE_MANAGER_ACCESSOR(ClientFullyConnect)
DEFINE_MANAGER_ACCESSOR(ClientPutInServer)
DEFINE_MANAGER_ACCESSOR(ClientSettingsChanged)
DEFINE_MANAGER_ACCESSOR(LevelInit)
DEFINE_MANAGER_ACCESSOR(LevelShutdown)
DEFINE_MANAGER_ACCESSOR(NetworkidValidated)
DEFINE_MANAGER_ACCESSOR(OnEdictAllocated)
DEFINE_MANAGER_ACCESSOR(OnEdictFreed)
DEFINE_MANAGER_ACCESSOR(OnQueryCvarValueFinished)
DEFINE_MANAGER_ACCESSOR(ServerActivate)
DEFINE_MANAGER_ACCESSOR(Tick)

#endif // _LISTENERMANAGER_H
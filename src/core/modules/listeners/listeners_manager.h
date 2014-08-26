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
// This creates a static manager and a function that returns a pointer to the
// manager. Must be used in a *.cpp file!
#define DEFINE_MANAGER_ACCESSOR(name) \
	static CListenerManager s_##name; \
	CListenerManager* Get##name##ListenerManager() \
	{ return &s_##name; }

// Calls all listeners of the given manager
#define CALL_LISTENERS(name, ...) \
	extern CListenerManager* Get##name##ListenerManager(); \
	for(int i = 0; i < Get##name##ListenerManager()->m_vecCallables.Count(); i++) \
	{ \
		BEGIN_BOOST_PY() \
			CALL_PY_FUNC(Get##name##ListenerManager()->m_vecCallables[i].ptr(), ##__VA_ARGS__); \
		END_BOOST_PY_NORET() \
	}


//-----------------------------------------------------------------------------
// CListenerManager class
//-----------------------------------------------------------------------------
class CListenerManager
{
public:
	void RegisterListener(PyObject* pCallable);
	void UnregisterListener(PyObject* pCallable);
	void Notify(boost::python::tuple args, dict kwargs);

public:
	CUtlVector<object> m_vecCallables;
};

#endif // _LISTENERMANAGER_H

#ifndef _LISTENERMANAGER_H
#define _LISTENERMANAGER_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "utlvector.h"
#include "utility/wrap_macros.h"

//-----------------------------------------------------------------------------
// CListenerManager class
//-----------------------------------------------------------------------------
class CListenerManager
{
public:

	void register_listener(PyObject* pCallable);
	void unregister_listener(PyObject* pCallable);

	void call_listeners();

protected:
	CUtlVector<object> m_vecCallables;
};

#endif // _LISTENERMANAGER_H
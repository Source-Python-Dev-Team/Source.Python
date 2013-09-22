#ifndef _LISTENERMANAGER_H
#define _LISTENERMANAGER_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "utlvector.h"
#include "utility/wrap_macros.h"
#include "edict.h"

//-----------------------------------------------------------------------------
// CListenerManager class
//-----------------------------------------------------------------------------
class CListenerManager
{
public:

	void register_listener(PyObject* pCallable);
	void unregister_listener(PyObject* pCallable);

	void call_listeners();
    // A lot of listener just pass an entity pointer
    void call_listeners( edict_t *pEntity );

protected:
	CUtlVector<object> m_vecCallables;
};

#endif // _LISTENERMANAGER_H
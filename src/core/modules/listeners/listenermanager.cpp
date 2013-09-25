//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "listenermanager.h"
#include "utility/call_python.h"
#include "boost/python.hpp"
#include "modules/entities/entities_wrap.h"

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

#define BEGIN_CALL_LISTENER() \
	for(int i = 0; i < m_vecCallables.Count(); i++) \
	{ \
		BEGIN_BOOST_PY() \
			PyObject* pCallable = m_vecCallables[i].ptr(); \

#define END_CALL_LISTENER( ... ) \
	    END_BOOST_PY_NORET() \
	} 

//-----------------------------------------------------------------------------
// Adds a callable to the end of the CListenerManager vector.
//-----------------------------------------------------------------------------
void CListenerManager::register_listener(PyObject* pCallable)
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Is the callable already in the vector?
	if( !m_vecCallables.HasElement(oCallable) )
	{
		// Add the callable to the vector
		m_vecCallables.AddToTail(oCallable);
	}
}

//-----------------------------------------------------------------------------
// Removes all instances of a callable from the CListenerManager vector.
//-----------------------------------------------------------------------------
void CListenerManager::unregister_listener(PyObject* pCallable)
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Remove the callback from the ServerCommandManager instance
	m_vecCallables.FindAndRemove(oCallable);
}

//-----------------------------------------------------------------------------
// Calls all registered listeners.
//-----------------------------------------------------------------------------

void CListenerManager::call_listeners()
{
	BEGIN_CALL_LISTENER()
		CALL_PY_FUNC(pCallable);
	END_CALL_LISTENER()
}

void CListenerManager::call_listeners( edict_t *pEntity )
{
	CEdict *entity = new CEdict(pEntity); 

	BEGIN_CALL_LISTENER()
		// Call the callable
		CALL_PY_FUNC(pCallable, entity);
	END_CALL_LISTENER()
}

// LevelIinit
void CListenerManager::call_listeners(char const *pMapName)
{
	BEGIN_CALL_LISTENER()
		// Call the callable
		CALL_PY_FUNC(pCallable, pMapName);
	END_CALL_LISTENER()
}

// ClientFullyConnect
void CListenerManager::call_listeners( edict_t *pEntity, const char *playername )
{
	CEdict *entity = new CEdict(pEntity); 

	BEGIN_CALL_LISTENER()
		// Call the callable
		CALL_PY_FUNC(pCallable, entity, playername);
	END_CALL_LISTENER()
}

// NetworkID
void CListenerManager::call_listeners(const char *pszUserName, const char *pszNetworkID)
{
	BEGIN_CALL_LISTENER()
		// Call the callable
		CALL_PY_FUNC(pCallable, pszUserName, pszNetworkID);
	END_CALL_LISTENER()
}

// ServerActivate
void CListenerManager::call_listeners(edict_t *pEdictList, int edictCount, int clientMax)
{
	// TODO? Edictlist
	CEdict *entity = new CEdict(pEdictList); 

	BEGIN_CALL_LISTENER()
		CALL_PY_FUNC(pCallable, entity, edictCount, clientMax);
	END_CALL_LISTENER()
}
// ClientConnect
void CListenerManager::call_listeners(bool *bAllowConnect, edict_t *pEntity, 
	const char *pszName, const char *pszAddress, char *reject, 
	int maxrejectlen)
{
	bool bAllowConnect_safe = &bAllowConnect;
	CEdict *entity = new CEdict(pEntity); 
	const char *reject_safe = reject;

	BEGIN_CALL_LISTENER()
		CALL_PY_FUNC(pCallable, bAllowConnect_safe, entity, pszName, pszAddress, reject_safe, maxrejectlen);
	END_CALL_LISTENER()
}
// OnQueryCvarValueFinished
void CListenerManager::call_listeners(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, 
	EQueryCvarValueStatus eStatus, const char *pCvarName, 
	const char *pCvarValue)
{
	int cookie = (int)iCookie;
	CEdict *entity = new CEdict(pPlayerEntity);

	BEGIN_CALL_LISTENER()
		CALL_PY_FUNC(pCallable, cookie, entity, eStatus, pCvarName, pCvarValue);
	END_CALL_LISTENER()
}
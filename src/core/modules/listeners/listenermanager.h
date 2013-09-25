#ifndef _LISTENERMANAGER_H
#define _LISTENERMANAGER_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "utlvector.h"
#include "utility/wrap_macros.h"
#include "edict.h"
#include "public/engine/iserverplugin.h"

//-----------------------------------------------------------------------------
// CListenerManager class
//-----------------------------------------------------------------------------
class CListenerManager
{
public:

	enum types {
		BOOL,
		INT,
		CONST_CHAR_PTR,
		EDICT_T_PTR
	};

	struct Param {
		int type;
		const char* name; // For naming in the dict
		union {
			bool bool_value;
			int int_value;
			const char* const_char_ptr;
			edict_t* edict_t_ptr;
		};
	};

	void register_listener(PyObject* pCallable);
	void unregister_listener(PyObject* pCallable);

	void call_listeners();
	void call_listeners(edict_t *pEntity);
	// LevelIinit
	void call_listeners(char const *pMapName);
	// OnPlayerFullyConnect
	void call_listeners(edict_t *pEntity, const char *playername);
	// NetworkID
    void call_listeners(const char *pszUserName, const char *pszNetworkID);
	// ServerActivate
	void call_listeners(edict_t *pEdictList, int edictCount, int clientMax);
	// ClientConnect
	void call_listeners(bool *bAllowConnect, edict_t *pEntity, 
	    const char *pszName, const char *pszAddress, char *reject, 
		int maxrejectlen);
	// OnQueryCvarValueFinished
	void call_listeners(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, 
		EQueryCvarValueStatus eStatus, const char *pCvarName, 
	    const char *pCvarValue);

protected:
	CUtlVector<object> m_vecCallables;
};

#endif // _LISTENERMANAGER_H
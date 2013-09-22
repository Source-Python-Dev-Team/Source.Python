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

	enum types {
		INT=0,
		BOOL, // Only used in ClientConnect
		CONST_CHAR_PTR,
		CHAR_PTR, // Only used in ClientConnect
		EDICT_T_PTR,
	};

	struct Param {
		int type;
		const char* name; // For naming in the dict
		union {
			int int_value;
			bool bool_value;
			char* char_ptr;
			const char* const_char_ptr;
			edict_t* edict_t_ptr;
		};
	};

	void register_listener(PyObject* pCallable);
	void unregister_listener(PyObject* pCallable);

	void call_listeners(int argc, ...);
	/*void call_listeners();
    // A lot of listener just pass an entity pointer
    void call_listeners( edict_t *pEntity );*/

protected:
	CUtlVector<object> m_vecCallables;
};

#endif // _LISTENERMANAGER_H
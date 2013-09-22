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

	void call_listeners(int argc, ...);

protected:
	CUtlVector<object> m_vecCallables;
};

#endif // _LISTENERMANAGER_H
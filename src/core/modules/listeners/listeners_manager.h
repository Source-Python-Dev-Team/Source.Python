/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/

#ifndef _LISTENERS_MANAGER_H
#define _LISTENERS_MANAGER_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "utilities/call_python.h"
#include "utlvector.h"


//-----------------------------------------------------------------------------
// Helper macros.
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

#define GET_LISTENER_MANAGER(name, ret_var) \
	extern CListenerManager* Get##name##ListenerManager(); \
	CListenerManager* ret_var = Get##name##ListenerManager();


//-----------------------------------------------------------------------------
// CListenerManager class.
//-----------------------------------------------------------------------------
class CListenerManager
{
public:
	void RegisterListener(PyObject* pCallable);
	void UnregisterListener(PyObject* pCallable);
	void Notify(boost::python::tuple args, dict kwargs);
	int GetCount();
	bool IsRegistered(object oCallback);
	object __getitem__(unsigned int index);
	void clear();

	int FindCallback(object oCallback);

public:
	CUtlVector<object> m_vecCallables;
};


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
bool is_same_func(object f1, object f2);


#endif // _LISTENERS_MANAGER_H

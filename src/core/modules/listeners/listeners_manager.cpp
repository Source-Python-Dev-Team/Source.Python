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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "listeners_manager.h"


//-----------------------------------------------------------------------------
// Adds a callable to the end of the CListenerManager vector.
//-----------------------------------------------------------------------------
void CListenerManager::RegisterListener(PyObject* pCallable)
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Is the callable already in the vector?
	if( !m_vecCallables.HasElement(oCallable) )
	{
		m_vecCallables.AddToTail(oCallable);
	}
	else {
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Callback already registered.")
	}
}


//-----------------------------------------------------------------------------
// Removes all instances of a callable from the CListenerManager vector.
//-----------------------------------------------------------------------------
void CListenerManager::UnregisterListener(PyObject* pCallable)
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	int index = m_vecCallables.Find(oCallable);

	if (index == -1) {
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Callback not registered.")
	}
	else {
		m_vecCallables.Remove(index);
	}
}


//-----------------------------------------------------------------------------
// Notifies all registered callbacks.
//-----------------------------------------------------------------------------
void CListenerManager::Notify(tuple args, dict kwargs)
{
	for(int i = 0; i < m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()
			eval("lambda func, args, kwargs: func(*args, **kwargs)")(m_vecCallables[i], args, kwargs);
		END_BOOST_PY_NORET()
	}
}


//-----------------------------------------------------------------------------
// Return the number of registered callbacks.
//-----------------------------------------------------------------------------
int CListenerManager::GetCount()
{
	return m_vecCallables.Count();
}


//-----------------------------------------------------------------------------
// Return whether or not the given callback is registered.
//-----------------------------------------------------------------------------
bool CListenerManager::IsRegistered(object oCallback)
{
	return m_vecCallables.HasElement(oCallback);
}

object CListenerManager::__getitem__(unsigned int index)
{
	if (index >= (unsigned int) m_vecCallables.Count())
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")

	return m_vecCallables[index];
}

void CListenerManager::clear()
{
	m_vecCallables.RemoveAll();
}

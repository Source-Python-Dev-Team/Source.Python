/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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
// Includes
//-----------------------------------------------------------------------------
#include "ticklisteners_wrap.h"
#include "utility/call_python.h"

//-----------------------------------------------------------------------------
// Static singletons.
//-----------------------------------------------------------------------------
static CTickListenerManager s_TickListenerManager;

//-----------------------------------------------------------------------------
// TickListenerManager accessor.
//-----------------------------------------------------------------------------
CTickListenerManager* get_tick_listener_manager()
{
	return &s_TickListenerManager;
}

//-----------------------------------------------------------------------------
// Adds a callable to the end of the CTickListenerManager vector.
//-----------------------------------------------------------------------------
void CTickListenerManager::register_listener(PyObject* pCallable)
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
// Removes all instances of a callable from the CTickListenerManager vector.
//-----------------------------------------------------------------------------
void CTickListenerManager::unregister_listener(PyObject* pCallable)
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Remove the callback from the ServerCommandManager instance
	m_vecCallables.FindAndRemove(oCallable);
}

//-----------------------------------------------------------------------------
// Calls all registered tick listeners.
//-----------------------------------------------------------------------------
void CTickListenerManager::call_tick_listeners()
{
	for(int i = 0; i < m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = m_vecCallables[i].ptr();

			// Call the callable
			CALL_PY_FUNC(pCallable);

		END_BOOST_PY_NORET()
	}
}

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
#include "networkid_validated_listeners_wrap.h"

//-----------------------------------------------------------------------------
// Static singletons.
//-----------------------------------------------------------------------------
static CNetworkIDValidatedListenerManager s_NetworkIDValidatedListenerManager;

//-----------------------------------------------------------------------------
// Overload for passing the arguments
//-----------------------------------------------------------------------------
void CNetworkIDValidatedListenerManager::call_listeners( const char *pszUserName, const char *pszNetworkID )
{
	for(int i = 0; i < m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = m_vecCallables[i].ptr();

			// Call the callable
			CALL_PY_FUNC(pCallable, pszUserName, pszNetworkID);

		END_BOOST_PY_NORET()
	}
}

//-----------------------------------------------------------------------------
// CNetworkIDValidatedListenerManager accessor.
//-----------------------------------------------------------------------------
CNetworkIDValidatedListenerManager* get_networkid_validated_listener_manager()
{
	return &s_NetworkIDValidatedListenerManager;
}

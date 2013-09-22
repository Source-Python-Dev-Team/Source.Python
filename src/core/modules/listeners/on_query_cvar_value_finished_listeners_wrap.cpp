/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2013 Source Python Development Team.  All rights reserved.
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
#include "on_query_cvar_value_finished_listeners_wrap.h"
#include "utility/call_python.h"

//-----------------------------------------------------------------------------
// Static singletons.
//-----------------------------------------------------------------------------
static COnQueryCvarValueFinishedListenerManager s_OnQueryCvarValueFinishedListenerManager;

//-----------------------------------------------------------------------------
// Overload for passing the arguments
//-----------------------------------------------------------------------------
void COnQueryCvarValueFinishedListenerManager::call_listeners( 
    QueryCvarCookie_t iCookie, edict_t *pPlayerEntity,
	EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
{
	for(int i = 0; i < m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = m_vecCallables[i].ptr();

			// Call the callable
			CALL_PY_FUNC(pCallable, iCookie, pPlayerEntity, eStatus, pCvarName, pCvarValue);

		END_BOOST_PY_NORET()
	}
}

//-----------------------------------------------------------------------------
// OnQueryCvarValueFinishedListenerManager accessor.
//-----------------------------------------------------------------------------
COnQueryCvarValueFinishedListenerManager* get_on_query_cvar_value_finished_listener_manager()
{
	return &s_OnQueryCvarValueFinishedListenerManager;
}

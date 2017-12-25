/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2016 Source Python Development Team.  All rights reserved.
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
// Source.Python
#include "utilities/wrap_macros.h"
#include "events_generator.h"


//-----------------------------------------------------------------------------
// CGameEventDescriptor
//-----------------------------------------------------------------------------
const char* CGameEventDescriptor::GetName()
{
#if defined(ENGINE_CSGO) || defined(ENGINE_LEFT4DEAD2) || defined(ENGINE_BLADE)
	CGameEventManager2* manager = (CGameEventManager2*) gameeventmanager;
	return manager->event_names[name_index].key;
#elif defined(ENGINE_INSURGENCY)
	CGameEventManager2* manager = (CGameEventManager2*) gameeventmanager;
	return manager->events.GetElementName(this->eventid);
#else
	return name;
#endif
}

//-----------------------------------------------------------------------------
// CGameEventDescriptorIter
//-----------------------------------------------------------------------------
CGameEventDescriptorIter::CGameEventDescriptorIter(CUtlVector<CGameEventDescriptor>* game_events) 
{
	this->game_events = game_events;
	this->current_index = 0;
}

object CGameEventDescriptorIter::__iter__(PyObject* self)
{
	return object(handle<>(borrowed(self)));
}

CGameEventDescriptor& CGameEventDescriptorIter::__next__()
{
	if (current_index >= game_events->Count())
		BOOST_RAISE_EXCEPTION(PyExc_StopIteration, "No more descriptors.")
		
	return game_events->Element(current_index++);
}

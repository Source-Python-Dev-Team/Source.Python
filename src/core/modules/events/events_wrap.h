/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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

#ifndef _EVENTS_WRAP_H
#define _EVENTS_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "igameevents.h"


//-----------------------------------------------------------------------------
// Definitions.
//-----------------------------------------------------------------------------
#ifndef EVENT_DEBUG_ID_INIT
	#define EVENT_DEBUG_ID_INIT 42
#endif


//-----------------------------------------------------------------------------
// IGameEvent extension class.
//-----------------------------------------------------------------------------
class IGameEventExt
{
public:
	static PyObject* GetString(IGameEvent* pEvent, const char* szKey, const char* szDefault)
	{
		const char* szValue = pEvent->GetString(szKey, szDefault);
		return PyUnicode_DecodeUTF8(szValue, strlen(szValue), "ignore");
	}
};

//-----------------------------------------------------------------------------
// IGameEventListener2 wrapper class.
//-----------------------------------------------------------------------------
class CGameEventListener2: public IGameEventListener2, public wrapper<IGameEventListener2>
{
public:
	virtual void FireGameEvent(IGameEvent* pEvent)
	{
		BEGIN_BOOST_PY()
			get_override("fire_game_event")(ptr(pEvent));
		END_BOOST_PY()
	}

	virtual int GetEventDebugID()
	{
		return EVENT_DEBUG_ID_INIT;
	}
};


#endif // _EVENTS_WRAP_H

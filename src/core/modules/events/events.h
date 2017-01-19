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

#ifndef _EVENTS_H
#define _EVENTS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "igameevents.h"
#include "modules/keyvalues/keyvalues.h"
#include "events_generator.h"


//-----------------------------------------------------------------------------
// Definitions.
//-----------------------------------------------------------------------------
#ifndef EVENT_DEBUG_ID_INIT
	#define EVENT_DEBUG_ID_INIT 42
#endif


//-----------------------------------------------------------------------------
// IGameEvent extension class.
//-----------------------------------------------------------------------------
class IGameEventExt: public IGameEvent
{
public:
	static PyObject* GetString(IGameEvent* pEvent, const char* szKey, const char* szDefault)
	{
		const char* szValue = pEvent->GetString(szKey, szDefault);
		return PyUnicode_DecodeUTF8(szValue, strlen(szValue), "ignore");
	}

	static KeyValues* GetVariables(IGameEvent* pEvent)
	{
		return ((IGameEventExt *) pEvent)->m_pVariables;
	}

	static object __getitem__(IGameEvent* pEvent, const char* item)
	{
		object return_value = object();
		try
		{
			return KeyValuesExt::__getitem__(GetVariables(pEvent), item);
		}
		catch(...)
		{
			KeyValues *field = GetDescriptor(pEvent)->keys->FindKey(item);
			if (field != NULL)
			{
				switch ((EventVarType)atoi(field->GetString()))
				{
					case TYPE_STRING:
					case TYPE_WSTRING:
						return_value = object("");
						break;
					case TYPE_FLOAT:
						return_value = object(0.0f);
						break;
					case TYPE_LONG:
					case TYPE_SHORT:
					case TYPE_BYTE:
					case TYPE_UINT64:
						return_value = object(0);
						break;
					case TYPE_BOOL:
						return_value = object(false);
						break;
				}
			}

			if (return_value.ptr() == Py_None && PyErr_Occurred())
				throw_error_already_set();

			PyErr_Clear();
		}
		return return_value;
	}

	static void __setitem__(IGameEvent* pEvent, const char* item, object value)
	{
		KeyValuesExt::__setitem__(GetVariables(pEvent), item, value);
	}

	static CGameEventDescriptor* GetDescriptor(IGameEvent* pEvent)
	{
		return ((IGameEventExt *) pEvent)->m_pDescriptor;
	}

public:
	CGameEventDescriptor* m_pDescriptor;
	KeyValues* m_pVariables;
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


//-----------------------------------------------------------------------------
// IGameEventManager2 extension class.
//-----------------------------------------------------------------------------
class IGameEventManager2Ext
{
public:
	static CGameEventDescriptorIter* GetGameEvents(IGameEventManager2* _manager)
	{
		CGameEventManager2* manager = (CGameEventManager2*) _manager;
		return new CGameEventDescriptorIter(&manager->game_events);
	}
};


#endif // _EVENTS_H

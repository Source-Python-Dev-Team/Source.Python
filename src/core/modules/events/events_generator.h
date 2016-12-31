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

#ifndef _EVENTS_GENERATOR_H
#define _EVENTS_GENERATOR_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// Boost.Python
#include "boost/python/object.hpp"
using namespace boost::python;

// SDK
#include "igameevents.h"
#include "tier1/utlmap.h"
#include "tier1/utlvector.h"


//-----------------------------------------------------------------------------
// Definitions.
//-----------------------------------------------------------------------------
#define MAX_EVENT_NAME_LENGTH 32


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern IGameEventManager2* gameeventmanager;


//-----------------------------------------------------------------------------
// EventVarType.
//-----------------------------------------------------------------------------
enum EventVarType
{
	TYPE_LOCAL = 0,	// not networked
	TYPE_STRING,	// zero terminated ASCII string
	TYPE_FLOAT,		// float 32 bit
	TYPE_LONG,		// signed int 32 bit
	TYPE_SHORT,		// signed int 16 bit
	TYPE_BYTE,		// unsigned int 8 bit
	TYPE_BOOL		// unsigned int 1 bit
};


//-----------------------------------------------------------------------------
// CGameEventCallback
//-----------------------------------------------------------------------------
class CGameEventCallback
{
public:
	void*		m_pCallback;		// callback pointer
	int			m_nListenerType;	// client or server side ?
};


//-----------------------------------------------------------------------------
// CGameEventDescriptor
//-----------------------------------------------------------------------------
class CGameEventDescriptor
{
public:
#if defined(ENGINE_CSGO) || defined(ENGINE_LEFT4DEAD2) || defined(ENGINE_BLADE)
	int			eventid;	// 0 - Same like name_index
	int			name_index; // 4
	KeyValues	*keys;		// 8
    CUtlVector<CGameEventCallback*>	listeners;	// 12
	bool		local;		// 32
	bool		reliable;	// 33
	char		unknown[18]; // 34
	// Total size: 52
#else
	char		name[MAX_EVENT_NAME_LENGTH];	// 0
	int			eventid;	// 32
	KeyValues	*keys;		// 36
	bool		local;		// 40
	bool		reliable;	// 42
    CUtlVector<CGameEventCallback*>	listeners;	// 44
	// Total size: 64
#endif

	const char* GetName();
};

//-----------------------------------------------------------------------------
// CGameEventManager2
//-----------------------------------------------------------------------------
class CGameEventManager2: public IGameEventManager2
{
public:
	CUtlVector<CGameEventDescriptor> game_events;
#if defined(ENGINE_CSGO) || defined(ENGINE_LEFT4DEAD2) || defined(ENGINE_BLADE)
	char unknown[96];
	CUtlRBTree< CUtlMap<const char*, int, int>::Node_t, int > event_names;
#endif
};


//-----------------------------------------------------------------------------
// CGameEventDescriptorIter
//-----------------------------------------------------------------------------
class CGameEventDescriptorIter
{
public:
	CGameEventDescriptorIter(CUtlVector<CGameEventDescriptor>* game_events);
	static object			__iter__(PyObject* self);
	CGameEventDescriptor&	__next__();

private:
	CUtlVector<CGameEventDescriptor>* game_events;
	int current_index;
};

#endif // _EVENTS_GENERATOR_H

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

#ifndef _ENGINES_WRAP_H
#define _ENGINES_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "engine/iserverplugin.h"
#include "eiface.h"
#include "engine/IEngineTrace.h"

#include ENGINE_INCLUDE_PATH(engines_wrap.h)


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IServerPluginHelpers *helpers;


//-----------------------------------------------------------------------------
// IVEngineServer extension class.
//-----------------------------------------------------------------------------
class IVEngineServerExt
{
public:
	static void ClientCommand(IVEngineServer* pEngine, edict_t* pEdict, const char* szCommand)
	{
		pEngine->ClientCommand(pEdict, szCommand);
	}
	
	static void Con_NPrintf(IVEngineServer* pEngine, int pos, const char* fmt)
	{
		pEngine->Con_NPrintf(pos, fmt);
	}

	static void Con_NXPrintf(IVEngineServer* pEngine, const struct con_nprint_s* info, const char* fmt)
	{
		pEngine->Con_NXPrintf(info, fmt);
	}
	
	static QueryCvarCookie_t StartQueryCvarValue(IVEngineServer* pEngine, edict_t *pEntity, const char *pName)
	{
		return helpers->StartQueryCvarValue(pEntity, pName);
	}
};


//-----------------------------------------------------------------------------
// ITraceFilter wrapper class.
//-----------------------------------------------------------------------------
class ITraceFilterWrap: public ITraceFilter, public wrapper<ITraceFilter>
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* pEntity, int mask)
	{
		return get_override("should_hit_entity")(ptr(pEntity), mask);
	}

	virtual TraceType_t	GetTraceType() const
	{
		return get_override("get_trace_type")();
	}
};


//-----------------------------------------------------------------------------
// IEntityEnumerator wrapper class.
//-----------------------------------------------------------------------------
class IEntityEnumeratorWrap: public IEntityEnumerator, public wrapper<IEntityEnumerator>
{
public:
	virtual bool EnumEntity(IHandleEntity* pEntity)
	{
		return get_override("enum_entity")(ptr(pEntity));
	}
};


//-----------------------------------------------------------------------------
// IEngineTrace extension class.
//-----------------------------------------------------------------------------
class IEngineTraceExt
{
public:
	static tuple GetPointContents(IEngineTrace* pEngineTrace, const Vector& vec)
	{
		IHandleEntity** ppEntities = new IHandleEntity*[gpGlobals->maxEntities];
		memset(ppEntities, NULL, sizeof(IHandleEntity*) * gpGlobals->maxEntities);

		int iMask = ::GetPointContents(vec, ppEntities);

		list entities;
		for(int i=0; i < gpGlobals->maxEntities; i++)
		{
			if(ppEntities[i])
			{
				entities.append(ptr(ppEntities[i]));
			}
		}
		delete ppEntities;
		return make_tuple(iMask, entities);
	}
};


//-----------------------------------------------------------------------------
// CGameTrace extension class.
//-----------------------------------------------------------------------------
class CGameTraceExt
{
public:
	static IServerEntity* GetEntity(CGameTrace* pTrace)
	{
		return (IServerEntity *) pTrace->m_pEnt;
	}
};


//-----------------------------------------------------------------------------
// Ray_t extension class.
//-----------------------------------------------------------------------------
class Ray_tExt
{
public:
	static Ray_t* CreateRay1(const Vector& vec1, const Vector& vec2)
	{
		Ray_t* pRay = new Ray_t;
		pRay->Init(vec1, vec2);
		return pRay;
	}

	static Ray_t* CreateRay2(const Vector& vec1, const Vector& vec2, const Vector& vec3, const Vector& vec4)
	{
		Ray_t* pRay = new Ray_t;
		pRay->Init(vec1, vec2, vec3, vec4);
		return pRay;
	}
};


#endif // _ENGINES_WRAP_H

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
#include "utilities/wrap_macros.h"
#include "utilities/baseentity.h"
#include "game/shared/entitylist_base.h"
#include "game/server/entitylist.h"
#include "utilities/conversions.h"
#include "game/shared/ipredictionsystem.h"
#include "memalloc.h"

#include ENGINE_INCLUDE_PATH(undefined_symbols.h)


//-----------------------------------------------------------------------------
// g_pPredictionSystems definition.
//-----------------------------------------------------------------------------
IPredictionSystem* IPredictionSystem::g_pPredictionSystems = NULL;


//-----------------------------------------------------------------------------
// SendProp destructor declaration.
//-----------------------------------------------------------------------------
SendProp::~SendProp()
{
}


//-----------------------------------------------------------------------------
// Returns true if the trace did hit the world, false otherwise.
//-----------------------------------------------------------------------------
bool CGameTrace::DidHitWorld() const
{
	return BaseEntityFromIndex(WORLD_ENTITY_INDEX) == m_pEnt;
}


//-----------------------------------------------------------------------------
// Returns the index of the entity that was hit by the trace.
//-----------------------------------------------------------------------------
int CGameTrace::GetEntityIndex() const
{
	return IndexFromBaseEntity(m_pEnt);
}


//-----------------------------------------------------------------------------
// Sets the Variant value as an entity.
//-----------------------------------------------------------------------------
void variant_t::SetEntity(CBaseEntity *pValue) 
{ 
	eVal = pValue;
	fieldType = FIELD_EHANDLE; 
}


//-----------------------------------------------------------------------------
// Returns the IChangeInfoAccessor instance of the CBaseEdict.
//-----------------------------------------------------------------------------
IChangeInfoAccessor *CBaseEdict::GetChangeAccessor()
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}

const IChangeInfoAccessor *CBaseEdict::GetChangeAccessor() const
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}


//-----------------------------------------------------------------------------
// Adds the given listener to the list of callbacks.
//-----------------------------------------------------------------------------
void CGlobalEntityList::AddListenerEntity( IEntityListener *pListener )
{
    m_entityListeners.AddToTail( pListener );
}


//-----------------------------------------------------------------------------
// Removes the given listener from the list of callbacks.
//-----------------------------------------------------------------------------
void CGlobalEntityList::RemoveListenerEntity( IEntityListener *pListener )
{
    m_entityListeners.FindAndRemove( pListener );
}


//-----------------------------------------------------------------------------
// Default OnAddEntity callback definitions.
//-----------------------------------------------------------------------------
void CBaseEntityList::OnAddEntity( IHandleEntity *pEnt, CBaseHandle handle )
{
}

void CGlobalEntityList::OnAddEntity( IHandleEntity *pEnt, CBaseHandle handle )
{
}

//-----------------------------------------------------------------------------
// Default OnRemoveEntity callback definitions.
//-----------------------------------------------------------------------------
void CBaseEntityList::OnRemoveEntity( IHandleEntity *pEnt, CBaseHandle handle )
{
}

void CGlobalEntityList::OnRemoveEntity( IHandleEntity *pEnt, CBaseHandle handle )
{
}

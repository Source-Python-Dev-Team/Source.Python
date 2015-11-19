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
#include "../conversions.h"


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given Edict instance.
//-----------------------------------------------------------------------------
bool BaseHandleFromEdict( edict_t *pEdict, CBaseHandle& output )
{
	if (!pEdict || pEdict->IsFree())
		return false;

	IServerNetworkable *pServerNetworkable = pEdict->GetNetworkable();
	if (!pServerNetworkable)
		return false;

	IHandleEntity* pHandleEntity = pServerNetworkable->GetEntityHandle();
	if (!pHandleEntity)
		return false;

	CBaseHandle hBaseHandle = pHandleEntity->GetRefEHandle();
	if (!hBaseHandle.ToInt())
		return false;

	output = hBaseHandle;
	return true;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given index.
//-----------------------------------------------------------------------------
bool BaseHandleFromIndex( unsigned int iEntityIndex, CBaseHandle& output )
{
	edict_t* pEdict;
	if (!EdictFromIndex(iEntityIndex, pEdict))
		return false;

	return BaseHandleFromEdict(pEdict, output);
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given IntHandle.
//-----------------------------------------------------------------------------
bool BaseHandleFromIntHandle( unsigned int iEntityHandle, CBaseHandle& output )
{
	CBaseHandle hBaseHandle = CBaseHandle(iEntityHandle);
	unsigned int iIndex;
	if (!IndexFromBaseHandle(hBaseHandle, iIndex))
		return false;

	output = hBaseHandle;
	return true;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
bool BaseHandleFromBaseEntity( CBaseEntity *pBaseEntity, CBaseHandle& output )
{
	if (!pBaseEntity)
		return false;

	CBaseHandle hBaseHandle = pBaseEntity->GetRefEHandle();
	if (!hBaseHandle.ToInt())
		return false;

	output = hBaseHandle;
	return true;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given Pointer instance.
//-----------------------------------------------------------------------------
bool BaseHandleFromPointer( CPointer *pEntityPointer, CBaseHandle& output )
{
	CBaseEntity* pBaseEntity;
	if (!BaseEntityFromPointer(pEntityPointer, pBaseEntity))
		return false;

	return BaseHandleFromBaseEntity(pBaseEntity, output);
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given UserID.
//-----------------------------------------------------------------------------
bool BaseHandleFromUserid( unsigned int iUserID, CBaseHandle& output )
{
	edict_t* pEdict;
	if (!EdictFromUserid(iUserID, pEdict))
		return false;

	return BaseHandleFromEdict(pEdict, output);
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
bool BaseHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, CBaseHandle& output )
{
	edict_t* pEdict;
	if (!EdictFromPlayerInfo(pPlayerInfo, pEdict))
		return false;

	return BaseHandleFromEdict(pEdict, output);
}

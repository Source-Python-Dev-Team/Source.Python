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
// Returns a Pointer instance from the given Edict instance.
//-----------------------------------------------------------------------------
bool PointerFromEdict( edict_t *pEdict, CPointer& output)
{
	CBaseEntity* pBaseEntity;
	if (!BaseEntityFromEdict(pEdict, pBaseEntity))
		return false;

	return PointerFromBaseEntity(pBaseEntity, output);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given index.
//-----------------------------------------------------------------------------
bool PointerFromIndex( unsigned int iEntityIndex, CPointer& output)
{
	CBaseEntity *pBaseEntity;
	if (!BaseEntityFromIndex(iEntityIndex, pBaseEntity))
		return false;

	return PointerFromBaseEntity(pBaseEntity, output);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
bool PointerFromBaseEntity( CBaseEntity *pBaseEntity, CPointer& output)
{
	if (!pBaseEntity)
		return false;

	output = CPointer((unsigned long) pBaseEntity);
	return true;
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
bool PointerFromBaseHandle( CBaseHandle hBaseHandle, CPointer& output)
{
	CBaseEntity *pBaseEntity;
	if (!BaseEntityFromBaseHandle(hBaseHandle, pBaseEntity))
		return false;

	return PointerFromBaseEntity(pBaseEntity, output);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given IntHandle.
//-----------------------------------------------------------------------------
bool PointerFromIntHandle( unsigned int iEntityHandle, CPointer& output)
{
	CBaseEntity *pBaseEntity;
	if (!BaseEntityFromIntHandle(iEntityHandle, pBaseEntity))
		return false;

	return PointerFromBaseEntity(pBaseEntity, output);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given UserID.
//-----------------------------------------------------------------------------
bool PointerFromUserid( unsigned int iUserID, CPointer& output)
{
	CBaseEntity *pBaseEntity;
	if (!BaseEntityFromUserid(iUserID, pBaseEntity))
		return false;

	return PointerFromBaseEntity(pBaseEntity, output);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
bool PointerFromPlayerInfo( IPlayerInfo *pPlayerInfo, CPointer& output)
{
	CBaseEntity *pBaseEntity;
	if (!BaseEntityFromPlayerInfo(pPlayerInfo, pBaseEntity))
		return false;

	return PointerFromBaseEntity(pBaseEntity, output);
}

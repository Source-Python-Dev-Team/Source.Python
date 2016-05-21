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
// Returns a BaseEntity instance from the given Edict instance.
//-----------------------------------------------------------------------------
bool BaseEntityFromEdict( edict_t *pEdict, CBaseEntity*& output )
{
	if (!pEdict || pEdict->IsFree())
		return false;

	IServerUnknown *pServerUnknown = pEdict->GetUnknown();
	if (!pServerUnknown)
		return false;

	output = pServerUnknown->GetBaseEntity();
	return true;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given Pointer instance.
//-----------------------------------------------------------------------------
bool BaseEntityFromPointer( CPointer *pEntityPointer, CBaseEntity*& output )
{
	if (!IsValidBaseEntityPointer(pEntityPointer))
		return false;

	output = (CBaseEntity *) pEntityPointer->m_ulAddr;
	return true;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given index.
//-----------------------------------------------------------------------------
bool BaseEntityFromIndex( unsigned int iEntityIndex, CBaseEntity*& output )
{
	edict_t* pEdict;
	if (!EdictFromIndex(iEntityIndex, pEdict))
		return false;

	return BaseEntityFromEdict(pEdict, output);
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
bool BaseEntityFromBaseHandle( CBaseHandle hBaseHandle, CBaseEntity*& output )
{
	edict_t* pEdict;
	if (!EdictFromBaseHandle(hBaseHandle, pEdict))
		return false;

	return BaseEntityFromEdict(pEdict, output);
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given IntHandle.
//-----------------------------------------------------------------------------
bool BaseEntityFromIntHandle( unsigned int iEntityHandle, CBaseEntity*& output )
{
	edict_t* pEdict;
	if (!EdictFromIntHandle(iEntityHandle, pEdict))
		return false;

	return BaseEntityFromEdict(pEdict, output);
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given UserID.
//-----------------------------------------------------------------------------
bool BaseEntityFromUserid( unsigned int iUserID, CBaseEntity*& output )
{
	edict_t* pEdict;
	if (!EdictFromUserid(iUserID, pEdict))
		return false;

	return BaseEntityFromEdict(pEdict, output);
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
bool BaseEntityFromPlayerInfo( IPlayerInfo *pPlayerInfo, CBaseEntity*& output )
{
	edict_t* pEdict;
	if (!EdictFromPlayerInfo(pPlayerInfo, pEdict))
		return false;

	return BaseEntityFromEdict(pEdict, output);
}

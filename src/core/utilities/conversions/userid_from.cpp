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
// Returns a UserID from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
bool UseridFromPlayerInfo( IPlayerInfo *pPlayerInfo, unsigned int& output )
{
	if (!pPlayerInfo)
		return false;

	int iUserID = pPlayerInfo->GetUserID();
	if (iUserID == INVALID_PLAYER_USERID)
		return false;

	output = iUserID;
	return true;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given index.
//-----------------------------------------------------------------------------
bool UseridFromIndex( unsigned int iEntityIndex, unsigned int& output )
{
	IPlayerInfo* pPlayerInfo;
	if (!PlayerInfoFromIndex(iEntityIndex, pPlayerInfo))
		return false;

	return UseridFromPlayerInfo(pPlayerInfo, output);
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given Edict instance.
//-----------------------------------------------------------------------------
bool UseridFromEdict( edict_t *pEdict, unsigned int& output )
{
	unsigned int iEntityIndex;
	if (!IndexFromEdict(pEdict, iEntityIndex))
		return false;

	return UseridFromIndex(iEntityIndex, output);
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given BaseHandle instance.
//-----------------------------------------------------------------------------
bool UseridFromBaseHandle( CBaseHandle hBaseHandle, unsigned int& output )
{
	unsigned int iEntityIndex;
	if (!IndexFromBaseHandle(hBaseHandle, iEntityIndex))
		return false;

	return UseridFromIndex(iEntityIndex, output);
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given IntHandle.
//-----------------------------------------------------------------------------
bool UseridFromIntHandle( unsigned int iEntityHandle, unsigned int& output )
{
	CBaseHandle hBaseHandle;
	if (!BaseHandleFromIntHandle(iEntityHandle, hBaseHandle))
		return false;

	return UseridFromBaseHandle(hBaseHandle, output);
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given BaseEntity instance.
//-----------------------------------------------------------------------------
bool UseridFromBaseEntity( CBaseEntity *pBaseEntity, unsigned int& output )
{
	IPlayerInfo* pPlayerInfo;
	if (!PlayerInfoFromBaseEntity(pBaseEntity, pPlayerInfo))
		return false;

	return UseridFromPlayerInfo(pPlayerInfo, output);
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given Pointer instance.
//-----------------------------------------------------------------------------
bool UseridFromPointer( CPointer *pEntityPointer, unsigned int& output )
{
	if (!IsValidPlayerPointer(pEntityPointer))
		return false;

	return UseridFromBaseEntity((CBaseEntity*) pEntityPointer->m_ulAddr, output);
}

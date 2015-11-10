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
int UseridFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException )
{
	int iUserID = INVALID_PLAYER_USERID;

	if (pPlayerInfo)
		iUserID = pPlayerInfo->GetUserID();

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given PlayerInfo instance (%x).", pPlayerInfo);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given index.
//-----------------------------------------------------------------------------
int UseridFromIndex( int iEntityIndex, bool bRaiseException )
{
	int iUserID = UseridFromPlayerInfo(PlayerInfoFromIndex(iEntityIndex));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given index (%i).", iEntityIndex);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given Edict instance.
//-----------------------------------------------------------------------------
int UseridFromEdict( edict_t *pEdict, bool bRaiseException )
{
	int iUserID = UseridFromIndex(IndexFromEdict(pEdict));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given Edict instance (%x).", pEdict);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given BaseHandle instance.
//-----------------------------------------------------------------------------
int UseridFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException )
{
	int iUserID = UseridFromIndex(IndexFromBaseHandle(hBaseHandle));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given IntHandle.
//-----------------------------------------------------------------------------
int UseridFromIntHandle( int iEntityHandle, bool bRaiseException )
{
	int iUserID = INVALID_PLAYER_USERID;

	if (iEntityHandle != (int) INVALID_EHANDLE_INDEX)
		iUserID = UseridFromBaseHandle(BaseHandleFromIntHandle(iEntityHandle));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given IntHandle (%i).", iEntityHandle);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given BaseEntity instance.
//-----------------------------------------------------------------------------
int UseridFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException )
{
	int iUserID = UseridFromPlayerInfo(PlayerInfoFromBaseEntity(pBaseEntity));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given BaseEntity instance (%x).", pBaseEntity);

	return iUserID;
}


//-----------------------------------------------------------------------------
// Returns a UserID from the given Pointer instance.
//-----------------------------------------------------------------------------
int UseridFromPointer( CPointer *pEntityPointer, bool bRaiseException )
{
	int iUserID = UseridFromBaseEntity(BaseEntityFromPointer(pEntityPointer));

	if (iUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a UserID from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return iUserID;
}

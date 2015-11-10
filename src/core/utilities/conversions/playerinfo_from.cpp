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
// Returns a PlayerInfo instance from the given index.
//-----------------------------------------------------------------------------
IPlayerInfo *PlayerInfoFromIndex( int iEntityIndex, bool bRaiseException )
{
	IPlayerInfo *pPlayerInfo = NULL;

	if (iEntityIndex > WORLD_ENTITY_INDEX && iEntityIndex <= gpGlobals->maxClients)
	{
		edict_t *pEdict = EdictFromIndex(iEntityIndex);
		if (pEdict)
			pPlayerInfo = playerinfomanager->GetPlayerInfo(pEdict);
	}

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given index (%i).", iEntityIndex);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
IPlayerInfo *PlayerInfoFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(IndexFromBaseEntity(pBaseEntity));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given BaseEntity instance (%x).", pBaseEntity);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given Pointer instance.
//-----------------------------------------------------------------------------
IPlayerInfo *PlayerInfoFromPointer( CPointer *pEntityPointer, bool bRaiseException )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromBaseEntity(BaseEntityFromPointer(pEntityPointer));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given Edict instance.
//-----------------------------------------------------------------------------
IPlayerInfo *PlayerInfoFromEdict( edict_t *pEdict, bool bRaiseException )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(IndexFromEdict(pEdict));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given Edict instance (%x).", pEdict);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
IPlayerInfo *PlayerInfoFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(hBaseHandle.GetEntryIndex());

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given IntHandle.
//-----------------------------------------------------------------------------
IPlayerInfo *PlayerInfoFromIntHandle( int iEntityHandle, bool bRaiseException )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(IndexFromIntHandle(iEntityHandle));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given IntHandle (%i).", iEntityHandle);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Returns a PlayerInfo instance from the given UserID.
//-----------------------------------------------------------------------------
IPlayerInfo *PlayerInfoFromUserid( int iUserID, bool bRaiseException )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromEdict(EdictFromUserid(iUserID));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given UserID (%i).", iUserID);

	return pPlayerInfo;
}

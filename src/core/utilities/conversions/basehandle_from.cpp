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
CBaseHandle BaseHandleFromEdict( edict_t *pEdict, bool bRaiseException )
{
	CBaseHandle hBaseHandle = NULL;

	if (pEdict && !pEdict->IsFree())
	{
		IServerNetworkable *pServerNetworkable = pEdict->GetNetworkable();
		if (pServerNetworkable)
			hBaseHandle = pServerNetworkable->GetEntityHandle();
	}

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given Edict instance (%x).", pEdict);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given index.
//-----------------------------------------------------------------------------
CBaseHandle BaseHandleFromIndex( int iEntityIndex, bool bRaiseException )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromIndex(iEntityIndex));

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given index (%i).", iEntityIndex);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given IntHandle.
//-----------------------------------------------------------------------------
CBaseHandle BaseHandleFromIntHandle( int iEntityHandle, bool bRaiseException )
{
	CBaseHandle hBaseHandle = CBaseHandle(iEntityHandle);

	if (IndexFromBaseHandle(hBaseHandle) == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given IntHandle (%i).", iEntityHandle);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
CBaseHandle BaseHandleFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException )
{
	CBaseHandle hBaseHandle = pBaseEntity->GetRefEHandle();
	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given BaseEntity instance (%x).", pBaseEntity);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given Pointer instance.
//-----------------------------------------------------------------------------
CBaseHandle BaseHandleFromPointer( CPointer *pEntityPointer, bool bRaiseException )
{
	CBaseHandle hBaseHandle = BaseHandleFromBaseEntity(BaseEntityFromPointer(pEntityPointer));

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given UserID.
//-----------------------------------------------------------------------------
CBaseHandle BaseHandleFromUserid( int iUserID, bool bRaiseException )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromUserid(iUserID));

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given UserID (%i).", iUserID);

	return hBaseHandle;
}


//-----------------------------------------------------------------------------
// Returns a BaseHandle instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
CBaseHandle BaseHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (!hBaseHandle.ToInt() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given PlayerInfo instance (%x).", pPlayerInfo);

	return hBaseHandle;
}

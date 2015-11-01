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
CBaseEntity *BaseEntityFromEdict( edict_t *pEdict, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = NULL;

	if (pEdict && !pEdict->IsFree())
	{
		IServerUnknown *pServerUnknown = pEdict->GetUnknown();
		if (pServerUnknown)
			pBaseEntity = pServerUnknown->GetBaseEntity();
	}

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given Edict instance (%x).", pEdict);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given Pointer instance.
//-----------------------------------------------------------------------------
CBaseEntity *BaseEntityFromPointer( CPointer *pEntityPointer, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = NULL;

	if (IndexFromPointer(pEntityPointer) != INVALID_ENTITY_INDEX)
		pBaseEntity = (CBaseEntity *)pEntityPointer->m_ulAddr;

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given index.
//-----------------------------------------------------------------------------
CBaseEntity *BaseEntityFromIndex( int iEntityIndex, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromIndex(iEntityIndex));

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given index (%i).", iEntityIndex);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
CBaseEntity *BaseEntityFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromBaseHandle(hBaseHandle));

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given IntHandle.
//-----------------------------------------------------------------------------
CBaseEntity *BaseEntityFromIntHandle( int iEntityHandle, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromIntHandle(iEntityHandle));

	if (!pBaseEntity  && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given IntHandle (%i).", iEntityHandle);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given UserID.
//-----------------------------------------------------------------------------
CBaseEntity *BaseEntityFromUserid( int iUserID, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromUserid(iUserID));

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given UserID (%i).", iUserID);

	return pBaseEntity;
}


//-----------------------------------------------------------------------------
// Returns a BaseEntity instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
CBaseEntity *BaseEntityFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseEntity instance from the given PlayerInfo instance (%x).", pPlayerInfo);

	return pBaseEntity;
}
